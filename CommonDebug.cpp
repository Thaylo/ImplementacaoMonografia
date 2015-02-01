#include "CommonDebug.h"

#ifdef DEBUG

void _Assert_Msg(char *msg)
{
    fflush(NULL);
    fprintf(stderr,"Assertion faliure. %s\n",msg);
    fflush(stderr);
    exit(0);
}

void _Assert_Msg_f(char *msg, const char *func)
{
    fflush(NULL);
    fprintf(stderr,"Assertion failure at %s call: %s.\n",func, msg);
    fflush(stderr);
    exit(0);
}


#endif /* debug */