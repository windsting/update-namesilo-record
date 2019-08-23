#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct MemoryStruct {
	char *memory;
	size_t size;
} Data;

void releaseMemoryStruct(Data* data);
void request(const char* url, char* buf, size_t size);

