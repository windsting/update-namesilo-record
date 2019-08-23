#include "util.h"

#include <time.h>

void removeChar(char* s, char c)
{
	const char* d = s;
	do {
		while(*d == c){
			++d;
		}
	} while(*s++ = *d++);
}

void removeWhitespace(char* s)
{
	removeChar(s, ' ');
	removeChar(s, '\t');
	removeChar(s, '\n');
	removeChar(s, '\r');
}

void getNow(char* buf, size_t size)
{
	time_t timer;
	struct tm* tm_info;

	time(&timer);
	tm_info = localtime(&timer);
	strftime(buf, size, "%Y-%m-%d %H:%M:%S", tm_info);
}

