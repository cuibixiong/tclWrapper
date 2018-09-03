#include <stdio.h>
#include <vector>
#include <assert.h>

class MsgAdapter 
{
    public:
        MsgAdapter();
        ~MsgAdapter();

        virtual void printMessage(const char *format, ...);

    private:
        char * alloc_vprintf(const char *format, va_list args);
        void vprintfMessage(const char *format, va_list args);
};

class Platform
{
    public:
        Platform();
        ~Platform();

        MsgAdapter &getMsgAdapter ();

    private:
        MsgAdapter *msgAdapter;
};

const char *get_version();
