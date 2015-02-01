/* 
Barueri, 26 september 2014.
    This code was created to train debug skills, especially with assert calls. Aditionally, this 
    code was created to serve as a repository for code reuse and thus speed up programming.
    If you have any suggestions, contact me or check out my github!
    thayloxavier@gmail.com
    https://github.com/Thaylo/Linked-List
    
*/

#ifndef _COMMON_DEBUG
#define _COMMON_DEBUG


#ifdef DEBUG

    #define ASSERTMSG(f,str)            \
        if(f)                           \
            {}                          \
        else                            \
            _Assert_Msg(str)
            
    #define ASSERTMSG_F(f,str,func)     \
            if(f)                       \
            {}                          \
        else                            \
            _Assert_Msg_f(str,func)

    #define common_assert(f)     \
            assert(f)
       
#else
    #define ASSERTMSG(f,str)
    #define ASSERTMSG_F(f,str,func)
    #define common_assert(f)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef DEBUG

#include <limits.h>
#include <assert.h>

void _Assert_Msg(char *msg);
void _Assert_Msg_f(char *msg, const char *func);

#endif 

#endif /* _COMMON_DEBUG */