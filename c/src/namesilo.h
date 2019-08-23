#pragma once

#include <stddef.h>

void xmlFetchPath(const char * strXml, size_t size, const char* strXpath, char* strResult, size_t sizeResult);
void testXml(const char * strXml, int size, const char* strXpath);
void genRecordIdUrl(char* s, size_t size, const char* apiKey, const char* domain);
void genUpdateUrl(char* s, size_t size, const char* apiKey, const char* domain, const char* rid, const char* recordName, const char* ip);
