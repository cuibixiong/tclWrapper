#include <map>
#include <vector>
#include <list>
#include <string>
#include <mutex>
#include <tcl.h>

#include "platform.h"

using namespace std;

class Platform;

class TclConsoleBase
{
    public:
        typedef int (*commandType) (void*, Tcl_Interp*, int, const char**);

    public:
        TclConsoleBase (string welcomeText, Platform *);
        ~TclConsoleBase();

        void runScript(const char *filename);
        void runShell();

        void clear();
        void reset(const string &welcomeText = "");

        void correctPathName(string& pathName);

        //Return false if the command is incomplete (e.g. unmatched braces)
        virtual bool isCommandComplete(const string &command);

        //Get the command to validate
        inline string getCurrentCommand() { return curCommand; };

        void setPrompt(const string &prompt, bool display = true);
        void displayPrompt();

        bool execCommand(const string &command, bool showPrompt = true, string *result = NULL);

        //execute a validated command (should be reimplemented and called at the end)
        //the return value of the function is the string result
        //res must hold back the return value of the command (0: passed; else: error)
        virtual string interpretCommand(const string &command, int *res);

        static TclConsoleBase *getInstance(Tcl_Interp* interp = NULL);

        inline Tcl_Interp *tclInterp() { return interp; }

        void registerFunction(char *commandName, commandType function, char *helpMsg, void *param = NULL);
        void unregisterFunction(const char *commandName);
#if 1
        //Registers an integer variable
        void registerVariable(char *varName, int &Var, char *helpMsg);
        //Registers a boolean variable
        void registerVariable(char *varName, bool &Var, char *helpMsg);
        //Registers a string variable
        void registerVariable(char *varName, char *&Var, char *helpMsg);
        //callback method that displays a help message
        int help(ClientData client_data, Tcl_Interp* interp, int argc, const char *argv[]);
#endif
    protected:
        std::string cmd;
        size_t prompt_length;

        std::string curCommand;
        size_t curCursorPos;

        Platform *platform;
        Tcl_Interp* interp;

        void processShell();
        //thread callback function
        static void *startShell(void *shell);

        typedef void HandleKey(int key);
        virtual HandleKey HandleChar;
        virtual HandleKey HandleEnter;
#if 0
        virtual HandleKey HandleHome;
        virtual HandleKey HandleEnd;
#endif
        virtual HandleKey HandleLeft;
        virtual HandleKey HandleRight;
#if 0
        virtual HandleKey HandleUp;
        virtual HandleKey HandleDown;
#endif
        virtual HandleKey HandleDelete;
        virtual HandleKey HandleBackspace;
#if 0
        virtual HandleKey HandleErase;
        virtual HandleKey HandleTab;
#endif

        virtual inline void moveCursor(const size_t from_pos, const size_t to_pos, const unsigned short int ws);
        virtual inline void moveLeft(const size_t from_pos, const unsigned short int ws);
        virtual inline void moveRight(const size_t from_pos, const unsigned short int ws);
        virtual inline void printCommand(const size_t from_pos, const unsigned short int ws);
        virtual inline void printStdout(int ch);

    private:
        static TclConsoleBase *theInstance;
        std::map<std::string, std::string> commandsHelp;
        std::map<std::string, std::string> varsHelp;
        std::mutex mutex;

    private:
        TclConsoleBase(Tcl_Interp* interp = NULL);

};

template <class Class>
class TclCallBack
{
    private:
        typedef int (Class::*commandType) (void*, Tcl_Interp*, int, const char**);

        static int callBackMethod(void* client_data, Tcl_Interp* interp, int argc, const char** argv)
        {
            typename std::list<commandType>::iterator it1 = method.begin();
            std::list<std::string>::iterator it;
            for (it = methodNames.begin(); it != methodNames.end(); ++it)
            {
                if ((*it) == std::string(argv[0]))
                    break;
                it1 ++;
            }
            commandType meth = *it1;
            return (instance->*meth)(client_data, interp, argc, argv);
        }

    public:
        static void registerMethod(Class *_instance, char * commandName, commandType _method, char *helpMsg, void *param = NULL)
        {
            instance = _instance;
            method.push_back(_method);
            methodNames.push_back(std::string(commandName));
            TclConsoleBase::getInstance()->registerFunction(commandName, callBackMethod, helpMsg, param);
        }

        static void unregisterAll()
        {
            std::list<std::string>::iterator it ;
            for (it = methodNames.begin(); it != methodNames.end(); ++it)
                TclConsoleBase::getInstance()->unregisterFunction(it->c_str());
            method.clear();
            methodNames.clear();
        }

    private:
        static Class *instance;
        static std::list<std::string> methodNames;
        static std::list<commandType> method;
};

template <class Class>
Class * TclCallBack<Class>::instance;

template <class Class>
std::list<typename TclCallBack<Class>::commandType> TclCallBack<Class>::method;

template <class Class>
std::list<std::string> TclCallBack<Class>::methodNames;


