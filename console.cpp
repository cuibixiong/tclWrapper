#include <mutex>

#include "console.h"

void Dummy_Tcl_DriversWatchProc(ClientData instanceData, int mask)
{}

int ConsoleOutput(ClientData, CONST char * buf, int toWrite, int *errorCode)
{
    static bool lastCR = false;
    *errorCode = 0;
    Tcl_SetErrno(0);
    if (!lastCR)
    {
        Console *console = Console::getInstance();
#if 0
        console->append(buf);
#endif
    }
    lastCR = !lastCR;
    return toWrite;
}

int ConsoleError(ClientData, CONST char * buf, int toWrite, int *errorCode)
{
    static bool lastCR = false;
    *errorCode = 0;
    Tcl_SetErrno(0);
    if (!lastCR)
    {
        Console *console = Console::getInstance();
#if 0
        console->append(buf);
#endif
    }
    lastCR = !lastCR;
    return toWrite;
}

/*Tcl_DriverCloseProc(ClientData instanceData, Tcl_Interp* interp);*/
int closeProcedureForBothTcl_ChannelType(ClientData instanceData, Tcl_Interp *interp)
{
    return -1;
}

Tcl_ChannelType consoleOutputChannelType =
{
    (char*)"console1", /* const char* typeName*/
    NULL, /*Tcl_ChannelTypeVersion version*/
    (closeProcedureForBothTcl_ChannelType) /*NULL*/, /*Tcl_DriverCloseProc* closeProc*/
    NULL, /*Tcl_DriverInputProc* inputProc*/
    ConsoleOutput, /*Tcl_DriverOutputProc* outputProc*/
    NULL, /*Tcl_DriverSeekProc* seekProc*/
    NULL, /*Tcl_DriverSetOptionProc*  setOptionProc*/
    NULL, /*Tcl_DriverGetOptionProc* getOptionProc*/
    Dummy_Tcl_DriversWatchProc, /*Tcl_DriverWatchProc* watchProc*/
    NULL, /*Tcl_DriverGetHandleProc* getHandleProc*/
    NULL, /*Tcl_DriverClose2Proc* close2Proc*/
    NULL, /*Tcl_DriverBlockModeProc* blockModeProc*/
    NULL, /*Tcl_DriverFlushProc* flushProc*/
    NULL, /*Tcl_DriverHandlerProc * handlerProc*/
    NULL, /*Tcl_DriverWideSeekProc * wideSeekProc*/
    NULL, /*Tcl_DriverThreadActionProc* threadActionProc*/
    NULL /*Tcl_DriverTruncateProc* truncateProc*/
};

//Tcl_ChannelType consoleOutputChannelType =
//{
//		(char*)"console1", NULL, NULL, NULL, ConsoleOutput,
//		NULL, NULL, NULL, NULL, NULL, NULL, NULL,
//		NULL, NULL, NULL, NULL
//};
Tcl_ChannelType consoleErrorChannelType =
{
    (char*)"console2", /* const char* typeName*/
    NULL, /*Tcl_ChannelTypeVersion version*/
    (closeProcedureForBothTcl_ChannelType) /*NULL*/, /*Tcl_DriverCloseProc* closeProc*/
    NULL, /*Tcl_DriverInputProc* inputProc*/
    ConsoleError, /*Tcl_DriverOutputProc* outputProc*/
    NULL, /*Tcl_DriverSeekProc* seekProc*/
    NULL, /*Tcl_DriverSetOptionProc*  setOptionProc*/
    NULL, /*Tcl_DriverGetOptionProc* getOptionProc*/
    Dummy_Tcl_DriversWatchProc, /*Tcl_DriverWatchProc* watchProc*/
    NULL, /*Tcl_DriverGetHandleProc* getHandleProc*/
    NULL, /*Tcl_DriverClose2Proc* close2Proc*/
    NULL, /*Tcl_DriverBlockModeProc* blockModeProc*/
    NULL, /*Tcl_DriverFlushProc* flushProc*/
    NULL, /*Tcl_DriverHandlerProc * handlerProc*/
    NULL, /*Tcl_DriverWideSeekProc * wideSeekProc*/
    NULL, /*Tcl_DriverThreadActionProc* threadActionProc*/
    NULL /*Tcl_DriverTruncateProc* truncateProc*/
};

//Tcl_ChannelType consoleErrorChannelType =
//{
//		(char*)"console2", NULL, NULL, NULL, ConsoleError,
//		NULL, NULL, NULL, NULL, NULL, NULL, NULL,
//		NULL, NULL, NULL, NULL
//};

Console *Console::theInstance = NULL;

Console *Console::getInstance()
{
    assert(theInstance);
    return theInstance;
}

int Console :: AA (ClientData client_data, Tcl_Interp* interp, int argc, const char *argv[])
{
    return TCL_OK;
}

int Console :: BB (ClientData client_data, Tcl_Interp* interp, int argc, const char *argv[])
{
    return TCL_OK;
}

//QTcl console constructor (init the QTextEdit & the attributes)
Console :: Console(const string &welcomeText)
    : TclConsoleBase(welcomeText)
{
    Platform *platform = new Platform(); 
    theInstance = this;

    TclConsoleBase::getInstance()->setPlatform(platform);

    //Register the set_prompt command
    TclCallBack<Console>::registerMethod(this, (char*)"A-1", &Console::AA, (char*)"oh, yes, I'm aa");

    //Register the set_prompt command
    TclCallBack<Console>::registerMethod(this, (char*)"B-1", &Console::BB, (char*)"oh, yes, I'm bb");

    //Get the Tcl interpreter
    interp = TclConsoleBase::getInstance()->tclInterp();

    //init tcl channels to redirect them to the console
    //stdout
    Tcl_Channel outConsoleChannel = Tcl_CreateChannel(&consoleOutputChannelType, "stdout",
            (ClientData) TCL_STDOUT, TCL_WRITABLE);

    if (outConsoleChannel)
    {
        Tcl_SetChannelOption(NULL, outConsoleChannel,
                "-translation", "lf");
        Tcl_SetChannelOption(NULL, outConsoleChannel,
                "-buffering", "none");
        Tcl_RegisterChannel(interp, outConsoleChannel);
        Tcl_SetStdChannel(outConsoleChannel, TCL_STDOUT);
    }

    //stderr
    Tcl_Channel errConsoleChannel = Tcl_CreateChannel(&consoleErrorChannelType, "stderr",
            (ClientData) TCL_STDERR, TCL_WRITABLE);

    if (errConsoleChannel)
    {
        Tcl_SetChannelOption(NULL, errConsoleChannel,
                "-translation", "lf");
        Tcl_SetChannelOption(NULL, errConsoleChannel,
                "-buffering", "none");
        Tcl_RegisterChannel(interp, errConsoleChannel);
        Tcl_SetStdChannel(errConsoleChannel, TCL_STDERR);
    }

    //set the Tcl Prompt
    TclConsoleBase::setPrompt(welcomeText);
}

//Destructor
Console::~Console()
{
    //unregister all the methods
    TclCallBack<Console>::unregisterAll();
}
