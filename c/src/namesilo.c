#include "namesilo.h"
#include <libxml/parser.h>
#include <libxml/xpath.h>

#include <string.h>


const char* NamesiloHost = "https://www.namesilo.com";

void genRecordIdUrl(char* s, size_t size, const char* apiKey, const char* domain)
{
	snprintf(s, size, "%s/api/dnsListRecords?version=1&type=xml&key=%s&domain=%s", NamesiloHost, apiKey, domain);
}

void genUpdateUrl(char* s, size_t size, const char* apiKey, const char* domain, const char* rid, const char* recordName, const char* ip)
{
	snprintf(s, size,
			"%s/api/dnsUpdateRecord?version=1&type=xml&key=%s&domain=%s&rrid=%s&rrhost=%s&rrvalue=%s&rrttl=7207",
			NamesiloHost, apiKey, domain, rid, recordName, ip);
}
xmlXPathObjectPtr getnodeset(xmlDocPtr doc, xmlChar *xpath)
{
	xmlXPathContextPtr context;
	xmlXPathObjectPtr result;

	context = xmlXPathNewContext(doc);
	if (context == NULL) {
		printf("Error in xmlXPathNewContext\n");
		return NULL;
	}
	result = xmlXPathEvalExpression(xpath, context);
	xmlXPathFreeContext(context);
	if (result == NULL) {
		printf("Error in xmlXPathEvalExpression\n");
		return NULL;
	}
	if(xmlXPathNodeSetIsEmpty(result->nodesetval)){
		xmlXPathFreeObject(result);
		printf("No result\n");
		return NULL;
	}
	return result;
}

void xmlFetchPath(const char * strXml, size_t size, const char* strXpath, char* strResult, size_t sizeResult)
{
	xmlDocPtr doc;
	xmlChar *xpath = (xmlChar*)strXpath;
	xmlNodeSetPtr nodeset;
	xmlXPathObjectPtr result;
	int i;
	xmlChar *value;

	if(strXml == NULL){
		fprintf(stderr, "strXml can not be NULL");
		return;
	}

	doc = xmlParseMemory(strXml, size);
	if(doc == NULL){
		fprintf(stderr, "Document not parsed successfully. \n");
		return;
	}

	result = getnodeset(doc, xpath);
	if(result){
		nodeset = result -> nodesetval;
		for(i=0; i<nodeset->nodeNr; i++) {
			value = xmlNodeListGetString(doc, nodeset->nodeTab[i]->xmlChildrenNode, 1);
			snprintf(strResult, sizeResult,"%s", value);
			xmlFree(value);
			break;
		}
		xmlXPathFreeObject(result);
	}
	xmlFreeDoc(doc);
	xmlCleanupParser();

//	printf("xml is:\n%s", xml_example);
}

void testXml(const char * strXml, int size, const char* strXpath)
{
	char buf[1024 * 64];
	xmlFetchPath(strXml, size, strXpath, buf, sizeof(buf));
	printf("value:[%s], len:%ld\n",buf, strlen(buf));

}

