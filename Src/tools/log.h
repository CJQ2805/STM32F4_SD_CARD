#ifndef __LOG_H
#define __LOG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <SysCommDef.h>
#include <usart.h>

#define PRINGT_DEBUG 

#ifdef PRINGT_DEBUG

#define logerr(format, argument...)     do{printf("\033[1;31m[ %s ]  %s (line %d) -> "format"\r\n\033[0m", __FUNCTION__,  __FILE__ ,__LINE__, ##argument );}while(0)
#define logwhite(format, argument...)   do{printf(format,##argument);LogGiveMutex(); }while(0)
#define loggreen(format, argument...)   do{printf("\033[1;32m"format"\r\n\033[0m", ##argument );}while(0) 
#define logyellow(format, argument...)  do{printf("\033[1;33m"format"\r\n\033[0m", ##argument );}while(0)   
#define logred(format, argument...) 	do{printf("\033[1;31m"format"\r\n\033[0m", ##argument );}while(0)
#define logblue(format, argument...)    do{printf("\033[1;34m"format"\r\n\033[0m", ##argument );}while(0)

#define logn() do{ printf("\r\n");}while(0) 



#define loxwhite(buf,len)   do{for(uint32_t i=0;i<len;i++)printf("%02x ",((uint8_t*)buf)[i]);printf("\r\n");}while(0)
#define loxwhite10(buf,len) do{for(uint32_t i=0;i<len;i++)printf("%d ",((uint8_t*)buf)[i]);printf("\r\n");}while(0)


#else

#endif


#endif



