#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#pragma once
namespace WINDOWS{
	#include <windows.h>
}

static FILE *logFile = NULL;
char *logFilename = "file.out";

/**
 Performs a strstr case-insensitive. Taken from http://www.dateiliste.com/en/source-code/16-cc/577-stristr-case-insensitive-strstr-function.html
 @param s1 a pointer to the null-terminated string to search
 @param s2 a pointer to the substring to search for
 @return the address of the first occurrence of the matching substring if successful, or NULL otherwise.
  XXX Should this work with UNICODE strings? Assumed to be, *but* need to be checked
 */
static char *stristr (char *s1, char *s2)
{
  char  *chN1, *chN2;
  char  *chNdx;
  char  *chRet        = NULL;
 
  chN1 = _strdup (s1);
  chN2 = _strdup (s2);
  if (chN1 && chN2)
  {
    chNdx = chN1;
    while (*chNdx)
    {
      *chNdx = (char) tolower (*chNdx);
      chNdx ++;
    }
    chNdx = chN2;
    while (*chNdx)
    {
      *chNdx = (char) tolower (*chNdx);
      chNdx ++;
    }
    chNdx = strstr (chN1, chN2);
    if (chNdx)
      chRet = s1 + (chNdx - chN1);
  }
  free (chN1);
  free (chN2);
  return chRet;
}

static void printMessage(char *msg)
{
	if(logFile == NULL)
		logFile = fopen(logFilename, "w");

	char s[4096];
	sprintf(s, "[PinVMShield] %s", msg);
	fprintf(logFile, s);
}

