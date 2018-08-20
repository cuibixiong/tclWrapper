#include <iostream>
#include "commands.h"

//Global callback function that implements the msgbox command
int CallQMessageBox( ClientData, Tcl_Interp* interp, int argc, char *argv[])
{
    std::string usageMsg = "Usage: " + std::string(argv[0]) + "title text\n";
    Tcl_ResetResult(interp);

    //Help message in case of wrong parameters
    if (argc != 3)
    {
        Tcl_AppendResult(interp, usageMsg.c_str(), (char*) NULL);
        return TCL_ERROR;
    }

    return TCL_OK;
}

