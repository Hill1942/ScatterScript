#include <ctype.h>

#include "ssutil.h"

char* strtoupper(char* str)
{
    char* p = str;

    *p = toupper(*p);
    while (*p)
        p++;

    return str;
}