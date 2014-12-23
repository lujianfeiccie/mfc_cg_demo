#include "StdAfx.h"
#include "Util.h"
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#define MY_DEBUG
Util::Util(void)
{
}
Util::~Util(void)
{
}
void __cdecl Util::LOG(const TCHAR *format, ...)
{
#ifdef MY_DEBUG
	char buf[4096], *p = buf;
	va_list args;
	va_start(args, format);
	int size = sizeof buf/2 - 1;
	p += _vsnprintf(p, size, format, args);
	va_end(args);
	while ( p > buf  &&  iswspace(p[-1])) 
 	{ 
 		*--p = '\0';		 
 	} 
 	*p = '\r'; 
 	*++p = '\n'; 
 	*++p = '\0'; 

	OutputDebugString(buf);
#endif
}
