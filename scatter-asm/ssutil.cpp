#include <ctype.h>
#include <iostream>
#include <sstream>

#include "ssutil.h"

char* strtoupper(char* str)
{
    char* p = str;

    *p = toupper(*p);
    while (*p)
        p++;

    return str;
}

void swrite(const void* src, size_t size, size_t count, char* buf)
{
	char* src_c = (char*) src;
	char c = ' ';
	unsigned int len = size * count * 2 + size * count / 2 - 1;
	for (unsigned int i = 0; i < len;)
	{
		if (i % 5 == 4) 
		{
			buf[i] = ' ';
			i++;
		}
		else
		{
			memcpy(&c, src_c, 1);
			src_c++;

			char h = 0xf0 & c;
			char l = 0x0f & c;

			h >>= 4;

			if (0 <= h && h <= 9)
				h += 48;
			else
				h += 97;

			if (0 <= l && l <= 9)
				l += 48;
			else
				l += 97;

			buf[i] = h;
			buf[i + 1] = l;
			i+=2;
		}
	}
	buf[len] = '\0';
}