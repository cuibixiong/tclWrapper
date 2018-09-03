
#include "console.h"

#include <string>
#include <string.h>

int main (int argc, char *argv[])
{
    Console *console = new Console("tcl shell$> ");

    const char *tclWrapper_version = get_version();
#if 0
    if (tclWrapper_version)
        platform.getMsgAdapter().printMessage("tclWrapper Shell Version is %s\n", tclWrapper_version);
#endif
    console->runShell();
}
