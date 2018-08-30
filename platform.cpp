#include <string>

#include <stdio.h>
#include <stdarg.h>
#include "platform.h"

const char *get_version()
{
    static char build_info[256];
    std::string version("Dev");
    std::string build_date("Oh..Ha.., Guess it?");

#ifdef VERSION
    version = VERSION;
#endif
#ifdef BUILD_DATE
    build_date = BUILD_DATE;
#endif

    sprintf(build_info, "%s - %s", version.c_str(), build_date.c_str());
    
    return build_info;
}

Platform :: Platform()
    :  msgAdapter (NULL)
{
    msgAdapter = new MsgAdapter();
}

Platform :: ~Platform()
{}

MsgAdapter :: MsgAdapter ()
{}

MsgAdapter :: ~MsgAdapter ()
{}

char * MsgAdapter :: alloc_vprintf (const char *format, va_list args)
{
    va_list args_copy;
    int len;
    char *str;

    va_copy(args_copy, args);
    len = vsnprintf(NULL, 0, format, args_copy);

    str = (char *)malloc(len + 2);
    if (str == NULL)
        return NULL;

    vsnprintf(str, len + 1, format, args);

    return str;
}

void MsgAdapter :: vprintfMessage(const char *format, va_list args)
{
    char *tmp;
    
    tmp = alloc_vprintf(format, args);
    if (!tmp)
        return;
    // real output implement.
    free(tmp);
}

void MsgAdapter :: printMessage (const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vprintfMessage(format, args);    
    va_end(args);
}
