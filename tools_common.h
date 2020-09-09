/*
* 一些基础通用的定义和函数
* Copyright (c) 2020 深圳先牛信息技术有限公司
*/

#ifndef XN_COMMON_SHARNEW_INCLUDE_H
#define XN_COMMON_SHARNEW_INCLUDE_H

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <time.h>

#ifndef DEBUG
    #define DEBUG 1
    #define DEBUG_TIME 0
    #define DEBUG_SAVE_FILE 1
#endif

#ifndef my_printf
    #if DEBUG == 1
        #define my_printf(format, ...) printf(format, ##__VA_ARGS__)
        #if DEBUG_TIME == 1
            #define my_printf_tag(format, ...) \
            do{ \
                if(tag_debug == 1){\
                    time_t time_val=time(NULL);\
                    char* ctr = ctime(&time_val);\
                    ctr[24] = '\0';\
                    printf("%s : %s : %d D:"#format " \n", ctr, tag, __LINE__, ##__VA_ARGS__); \
                } \
            }while(0)
        #else
            #define my_printf_tag(format, ...) \
            do{ \
                if(tag_debug == 1){\
                    printf("%s : %d D:"#format " \n", tag, __LINE__, ##__VA_ARGS__); \
                }\
            }while(0)
        #endif
    #else
        #define my_printf(format, ...)  
        #define my_printf_tag(format, ...)   
    #endif
#endif


#if DEBUG_TIME == 1
    #define my_printf_error(format, ...)  \
    do{ \
        time_t time_val = time(NULL);\
        char* ctr = ctime(&time_val);\
        ctr[24] = '\0';\
        fprintf(stderr, "%s : %s : %d E:"#format " \n", ctr, tag, __LINE__, ##__VA_ARGS__); \
    }while(0)
#else
    #define my_printf_error(format, ...) fprintf(stderr, "%s : %d E:"#format " \n", tag, __LINE__, ##__VA_ARGS__);
#endif

#if DEBUG == 1
    #define my_assert(a) assert(a)
#else
    #define my_assert(a) 
#endif

#endif
