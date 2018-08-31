#include <mutex>
#include <string>
#include <tcl.h>

#include "tclConsoleBase.h"

class Console : public TclConsoleBase
{
		friend int ConsoleOutput(ClientData, CONST char * buf, int toWrite, int *errorCode);
		friend int ConsoleError(ClientData, CONST char * buf, int toWrite, int *errorCode);
public:
		//constructor
		Console(const string &welcomeText = "");
		//destructor
		~Console();

        // register command Handler
		int AA(ClientData client_data, Tcl_Interp* interp, int argc, const char *argv[]);
		int BB(ClientData client_data, Tcl_Interp* interp, int argc, const char *argv[]);

		//get the Console instance
		static Console *getInstance(const string &welcomeText = "");
		Platform *getPlatform() { return this->platform; };

private:
		//The instance
		static Console *theInstance;
private:
};
