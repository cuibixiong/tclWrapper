#include <tcl.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <assert.h>

#include "tclConsoleBase.h"

void exitHandler (ClientData tclPlatform)
{}

TclConsoleBase :: TclConsoleBase (string welcomeText, Platform *platform)
    : platform(platform),
    cmd(),
    curCommand(),
    curCursorPos(0)
{
    if (!interp)
    {
        //Initialize the Tcl interpreter
        interp = Tcl_CreateInterp();
        Tcl_Init(interp);
    }

    Tcl_CreateExitHandler(exitHandler, reinterpret_cast<ClientData> (platform));
}

TclConsoleBase :: ~TclConsoleBase ()
{
    delete interp;

    TclCallBack<TclConsoleBase>::unregisterAll();
    if (interp)
    {
        //Delete the Tcl interpreter
        Tcl_DeleteInterp(interp);
        interp = NULL;
    }

}

void TclConsoleBase :: runScript(const char *filename)
{
    platform->getMsgAdapter().printMessage("running scripts : \"%s\"\n", filename);

    int ret = Tcl_EvalFile (interp, filename);

    fflush(stdout);

    if (ret == TCL_OK)
        platform->getMsgAdapter().printMessage("%s\n", Tcl_GetStringResult(interp));
    else
        platform->getMsgAdapter().printMessage("Error : %s\n", Tcl_GetStringResult(interp));
}

void TclConsoleBase :: processShell()
{
    while (1) {
        char key = getchar();
        switch (key) {
            case EOF:
                if (curCommand != "")
                    HandleEnter('\n');
                return;
            case 0x1B:
                key = getchar();
                switch (key) {
                    case EOF:
                        if (curCommand != "")
                            HandleEnter('\n');
                        return;
                    case 0x5B:
                        key = getchar();
                        switch (key) {
                            case EOF:
                                if (curCommand != "")
                                    HandleEnter('\n');
                                return;
                            case 0x33:
                                key = getchar();
                                switch (key) {
                                    case 0x7E:
                                        HandleDelete(key);
                                        break;
                                    default:
                                        break;
                                }
                                break;
                            case 0x43:
                                HandleRight(key); break;
                            case 0x44:
                                HandleLeft(key); break;
#if 0
                            case 0x46:
                                HandleEnd(key); break;
                            case 0x48:
                                HandleHome(key); break;
#endif
                            default:
                                break;
                        }
                        break;
                    case 0x4F:
                        key = getchar();
                        break;
                    default:
                        break;
                }
                break;
#if 0
            case 0x1:
                HandleHome(key);break;
            case 0x5:
                HandleEnd(key);break;
#endif
            case 0x8:
                HandleBackspace(key);break;
#if 0
            case 0x15:
                HandleErase(key);break;
#endif
            case 0xA:
                HandleEnter(key);break;
            case 0x7F:
                HandleBackspace(key);break;
            default:
                HandleChar(key);break;
        }
    }
}

void *TclConsoleBase :: startShell(void *shell)
{
    TclConsoleBase *tcl_shell = reinterpret_cast<TclConsoleBase *> (shell);
    tcl_shell->processShell(); 
    return NULL;
}

void TclConsoleBase :: runShell()
{
    pthread_t threadID;
    pthread_create(&threadID, NULL, startShell, this);
    pthread_detach(threadID);

    while (1)
        Tcl_DoOneEvent(TCL_ALL_EVENTS);
}

inline void TclConsoleBase :: moveCursor(const size_t from_pos, const size_t to_pos, const unsigned short int ws)
{
    if (from_pos == to_pos)
        return;

    int cur_row = (prompt_length + from_pos) / ws;
    int cur_col = (prompt_length + from_pos) % ws;
    int new_row = (prompt_length + to_pos) / ws;
    int new_col = (prompt_length + to_pos) % ws;

    int row_diff = cur_row - new_row;
    int col_diff = cur_col - new_col;

    if (row_diff) {
        if (row_diff > 0)
            fprintf(stdout, "%c%c%d%c", 0x1B, 0x5B, row_diff, 0x44);
        else
            fprintf(stdout, "%c%c%d%c", 0x1B, 0x5B, -row_diff, 0x42);
    }

    if (col_diff) {
        if (col_diff > 0)
            fprintf(stdout, "%c%c%d%c", 0x1B, 0x5B, col_diff, 0x44);
        else
            fprintf(stdout, "%c%c%d%c", 0x1B, 0x5B, -col_diff, 0x43);
    }

    fflush(stdout);
}

inline void TclConsoleBase :: moveLeft(const size_t cur_pos, const unsigned short int ws) 
{
    if (!cur_pos)
        return;

    if (((prompt_length + cur_pos) % ws) == 0)
        fprintf(stdout, "%c%c%c%c%c%d%c", 0x1B, 0x5B, 0x41, 0x1B, 0x5B, ws - 1, 0x43);
    else
        fprintf(stdout, "%c%c%c", 0x1B, 0x5B, 0x44);

    fflush(stdout);
}

inline void TclConsoleBase :: moveRight(const size_t cur_pos, const unsigned short int ws) 
{
    if (cur_pos == curCommand.size())
        return;

    if (((prompt_length + cur_pos + 1) % ws) == 0)
        fprintf(stdout, "%c%c%c%c%c%d%c", 0x1B, 0x5B, 0x42, 0x1B, 0x5B, ws - 1, 0x44);
    else
        fprintf(stdout, "%c%c%c", 0x1B, 0x5B, 0x43);

    fflush(stdout);
}

inline void TclConsoleBase :: printStdout(int c)
{
    putc(c, stdout);
    fflush(stdout);
}

inline void TclConsoleBase :: printCommand(const size_t from_pos, const unsigned short int ws)
{
   for (size_t i = from_pos; i < curCommand.size(); i++) 
       printStdout(curCommand[i]);

   if ((prompt_length + curCommand.size()) % ws == 0)
       printStdout('\n');
}

void TclConsoleBase :: HandleChar(int key)
{
    curCommand.insert(curCursorPos++, 1, key);
    printStdout(key);

    struct winsize ws;
    if (ioctl(fileno(stdout), TIOCGWINSZ, &ws) == 0 && ws.ws_col != 0) {
        printCommand(curCursorPos, ws.ws_col);
        moveCursor(curCommand.size(), curCursorPos, ws.ws_col);
    }
}

void TclConsoleBase :: HandleEnter(int key)
{
    if (curCursorPos != curCommand.size()) {
        struct winsize ws;
        if (ioctl(fileno(stdout), TIOCGWINSZ, &ws) == 0 && ws.ws_col != 0) {
            moveCursor(curCursorPos, curCommand.size(), ws.ws_col);
        }
    }

    cmd += curCommand;
    cmd += static_cast<char> (key);

    curCursorPos = 0;
    printStdout(key);

    curCommand.clear();


    char *tclCmd = strdup(cmd.c_str());
    cmd.clear();

    Tcl_Eval( interp, tclCmd);
    free(tclCmd);

    //printPrompt();
}

void TclConsoleBase :: HandleLeft(int key)
{
    if (!curCursorPos)
        return;

    struct winsize ws;
    if (ioctl(fileno(stdout), TIOCGWINSZ, &ws) == 0 && ws.ws_col != 0) {
        moveLeft(curCursorPos--, ws.ws_col);
    }
}

void TclConsoleBase :: HandleRight(int key)
{
    if (curCursorPos == curCommand.size())
        return;

    struct winsize ws;
    if (ioctl(fileno(stdout), TIOCGWINSZ, &ws) == 0 && ws.ws_col != 0) {
        moveRight(curCursorPos--, ws.ws_col);
        curCursorPos = 0;
    }
}

void TclConsoleBase :: HandleBackspace (int key)
{
    if (!curCursorPos)
        return;

    HandleLeft(key);
    HandleDelete(key);
}

void TclConsoleBase :: HandleDelete(int key)
{
    if (curCursorPos == curCommand.size())
        return;

    struct winsize ws;
    if (ioctl(fileno(stdout), TIOCGWINSZ, &ws) == 0 && ws.ws_col != 0) {
        curCommand.erase(curCursorPos, 1);
        if (curCursorPos != curCommand.size())
            printCommand(curCursorPos, ws.ws_col);
        fprintf(stdout, "%c%c%c", 0x1B, 0x5B, 0x4B);
        fflush(stdout);
        moveCursor(curCommand.size(), curCursorPos, ws.ws_col);
    }
}

TclConsoleBase *TclConsoleBase::theInstance = NULL;

TclConsoleBase *TclConsoleBase::getInstance(Tcl_Interp* interp)
{
    assert(!theInstance);

    return theInstance;
}

void TclConsoleBase::registerFunction(char *commandName, commandType function,
        char *helpMsg, void *param)
{
    commandsHelp[std::string(commandName)] = std::string(helpMsg);
    Tcl_CreateCommand(interp, commandName, function, param, (Tcl_CmdDeleteProc*) NULL);
}

void TclConsoleBase::unregisterFunction(const char *commandName)
{
    commandsHelp.erase(std::string(commandName));
    Tcl_DeleteCommand(interp, commandName);
}

void TclConsoleBase::registerVariable(char *varName, int &Var, char *helpMsg)
{
    varsHelp[varName] = helpMsg;
    Tcl_LinkVar(interp, varName, (char *)&Var, TCL_LINK_INT);
}

void TclConsoleBase::registerVariable(char *varName, bool &Var, char *helpMsg)
{
    varsHelp[varName] = helpMsg;
    Tcl_LinkVar(interp, varName, (char *)&Var, TCL_LINK_BOOLEAN);
}

void TclConsoleBase::registerVariable(char *varName, char* &Var, char *helpMsg)
{
    varsHelp[varName] = helpMsg;
    Tcl_LinkVar(interp, varName, (char *)&Var, TCL_LINK_STRING);
}

int TclConsoleBase::help(ClientData, Tcl_Interp* interp, int argc,
        const char *[])
{
    // Reset result data
    Tcl_ResetResult(interp);

    //Help message in case of wrong parameters
    if (argc != 1)
    {
        Tcl_AppendResult(interp, "Usage: help\n", (char*) NULL);
        return TCL_ERROR;
    }

    std::map<std::string, std::string>::iterator it;

    //determine the max length of the commands/vars to improve formatting
    int maxCommandLength = 0;

    for (it = commandsHelp.begin(); it != commandsHelp.end(); ++it)
        if (maxCommandLength < (int)it->first.length())
            maxCommandLength = it->first.length();

    for (it = varsHelp.begin(); it != varsHelp.end(); ++it)
        if (maxCommandLength < (int)it->first.length())
            maxCommandLength = it->first.length();

    if (commandsHelp.size())
    {
        Tcl_AppendResult(interp, "Commands :\n", (char*) NULL);
        for (it = commandsHelp.begin(); it != commandsHelp.end(); ++it)
        {
#if 0
            Tcl_AppendResult(interp, (QString("%1:%2%3\n").arg(it->first).arg(QString().fill(' ',
                                maxCommandLength - it->first.length() + 1)).arg(it->second)), (char*) NULL);
#endif
        }
    }

    if (varsHelp.size())
    {
        Tcl_AppendResult(interp, "\nVariables :\n", (char*) NULL);

        for (it = varsHelp.begin(); it != varsHelp.end(); ++it)
        {
#if 0
            Tcl_AppendResult(interp, (QString("%1:%2%3\n").arg(it->first).arg(QString().fill(' ', maxCommandLength - it->first.length() + 1)).arg(it->second)), (char*) NULL);
#endif
        }
    }
    return TCL_OK;
}

//callback method that implements the set_prompt command
void TclConsoleBase :: setPrompt(const string &prompt, bool display)
{
}

bool TclConsoleBase :: isCommandComplete(const string &command)
{
    return Tcl_CommandComplete(command.c_str());
}

string TclConsoleBase::interpretCommand(const string &command, int *res)
{
    if (!mutex.try_lock()) {
        *res = 1;
        return "Command cannot be executed!";
    }

    string result;

    if (!command.empty())
    {
        string modifiedCommand = command;

        //Do the Tcl evaluation
        *res = Tcl_Eval(interp, modifiedCommand.c_str());
        //Get the string result of the executed command
        result = Tcl_GetString(Tcl_GetObjResult(interp));
        //Call the parent implementation
        interpretCommand(modifiedCommand, res);
    }

    mutex.unlock();
    return result;

}

bool TclConsoleBase::execCommand(const string &command, bool showPrompt, string *result)
{
    string modifiedCommand = command;
    correctPathName(modifiedCommand);

    //Display the prompt with the command first
    //execute the command and get back its text result and its return value
    int res = 0;
    string strRes = interpretCommand(modifiedCommand, &res);
    //According to the return value, display the result either in red or in blue
   
#if 0
    if (res == 0)
        setTextColor(outColor_);
    else
        setTextColor(errColor_);
#endif

    if(result){
        *result = strRes;
    }

    if (!strRes.empty())
        strRes.append("\n");

    platform->getMsgAdapter().printMessage("%s\n", strRes.c_str());

    struct winsize ws;
    if (ioctl(fileno(stdout), TIOCGWINSZ, &ws) == 0 && ws.ws_col != 0) {
        moveCursor(curCommand.size(), curCursorPos, ws.ws_col);
    }

    //Display the prompt again
    if (showPrompt)
        displayPrompt();

    return !res;
}

void TclConsoleBase::correctPathName(string& pathName)
{}
