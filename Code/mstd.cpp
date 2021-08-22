// ============================================================================
// mstd.h
// ---------
// Author: Colin Phillips
// Description:
// Generic library functions.
// ============================================================================

#include "mstd.h"
#include <stdio.h>
#include <windows.h>

__int64 Mstd_gettime(void)
{
	SYSTEMTIME cSTime;
	FILETIME cFTime;

	GetSystemTime(&cSTime);
	SystemTimeToFileTime(&cSTime, &cFTime); // returns time in 100 nanoseconds i.e. 0.1us increments

	__int64 iTime;

	iTime = cFTime.dwHighDateTime;
	iTime<<=32;
	iTime|=cFTime.dwLowDateTime;
	return iTime;
}

int Mstd_log(const char *format, ...)
{
	char sBuf[256];
    
	va_list arg;

	va_start(arg, format);
	vsprintf(sBuf,format,arg);
	va_end(arg);	
	
 	char sFilename[128];
	sprintf(sFilename, "log_mstd.txt");
	if(FILE *fptr = fopen(sFilename, "a+b"))
	{
		fprintf(fptr, "%s\r\n", sBuf);
		fclose(fptr);
	}

    return 1;
}
