#include <string>
#include "commandsManager.h"

commandsManager *commandsManager::theInstance = NULL;

commandsManager::commandsManager(Tcl_Interp* _interp)
{
    createdInterp = false;
    if (!_interp)
    {
        //Initialize the Tcl interpreter
        interp = Tcl_CreateInterp();
        Tcl_Init(interp);
        createdInterp = true;
    } else
        interp = _interp;
}

commandsManager::~commandsManager()
{
    TclCallBack<commandsManager>::unregisterAll();
    if (interp && createdInterp)
    {
        //Delete the Tcl interpreter
        Tcl_DeleteInterp(interp);
        interp = NULL;
    }
}

commandsManager *commandsManager::getInstance(Tcl_Interp* interp)
{
    if (!theInstance)
    {
        theInstance = new commandsManager(interp);
        //Register the help command
        TclCallBack<commandsManager>::registerMethod(theInstance, (char*)"help",
                &commandsManager::help, (char*)"displays this help message");
    }
    return theInstance;
}

void commandsManager::registerFunction(char *commandName, commandType function,
        char *helpMsg, void *param)
{
    commandsHelp[std::string(commandName)] = std::string(helpMsg);
    Tcl_CreateCommand(interp, commandName, function, param,
            (Tcl_CmdDeleteProc*) NULL);
}

void commandsManager::unregisterFunction(const char *commandName)
{
    commandsHelp.erase(std::string(commandName));
    Tcl_DeleteCommand(interp, commandName);
}

void commandsManager::registerVariable(char *varName, int &Var, char *helpMsg)
{
    varsHelp[varName] = helpMsg;
    Tcl_LinkVar(interp, varName, (char *)&Var, TCL_LINK_INT);
}

void commandsManager::registerVariable(char *varName, bool &Var, char *helpMsg)
{
    varsHelp[varName] = helpMsg;
    Tcl_LinkVar(interp, varName, (char *)&Var, TCL_LINK_BOOLEAN);
}

void commandsManager::registerVariable(char *varName, char* &Var, char *helpMsg)
{
    varsHelp[varName] = helpMsg;
    Tcl_LinkVar(interp, varName, (char *)&Var, TCL_LINK_STRING);
}

int commandsManager::help(ClientData, Tcl_Interp* interp, int argc,
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
        Tcl_AppendResult(interp, "==========\n", (char*) NULL);
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
        Tcl_AppendResult(interp, "===========\n", (char*) NULL);
        for (it = varsHelp.begin(); it != varsHelp.end(); ++it)
        {
#if 0
            Tcl_AppendResult(interp, (QString("%1:%2%3\n").arg(it->first).arg(QString().fill(' ', maxCommandLength - it->first.length() + 1)).arg(it->second)), (char*) NULL);
#endif
        }
    }

    return TCL_OK;
}
