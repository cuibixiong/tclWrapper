#ifndef COMMANDS_MANAGER_H
#define COMMANDS_MANAGER_H

#include <tcl.h>
#include <string>
#include <list>
#include <map>

/* Singleton helper class to register/unregister tcl callback functions & vars
   along with help messages */
class commandsManager
{
    public:
        typedef int (*commandType) (void*, Tcl_Interp*, int, const char**);
    public:
        ~commandsManager();
        static commandsManager *getInstance(Tcl_Interp* interp = NULL);
        Tcl_Interp *tclInterp() {return interp;}
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
        int help(ClientData client_data, Tcl_Interp* interp, int argc,
                const char *argv[]);
#endif
    private:
        Tcl_Interp* interp;
        bool createdInterp;
        static commandsManager *theInstance;
        std::map<std::string, std::string> commandsHelp;
        std::map<std::string, std::string> varsHelp;
    private:
        commandsManager(Tcl_Interp* interp = NULL);
};

/* Template Helper class that enables registering methods
   as callbacks. Usage:
   TclCallBack<ClassType>::registerMethod(classInstance, "tcl_command_name", &ClassType::methodName, "Help message");
   */
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
            commandsManager::getInstance()->registerFunction(commandName, callBackMethod, helpMsg, param);
        }

        static void unregisterAll()
        {
            std::list<std::string>::iterator it ;
            for (it = methodNames.begin(); it != methodNames.end(); ++it)
                commandsManager::getInstance()->unregisterFunction(it->c_str());
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

#endif
