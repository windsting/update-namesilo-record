#include "request.h"
#include <curl/curl.h>
#include <stdio.h>

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	Data *mem = (Data*)userp;

	char *ptr = realloc(mem->memory, mem->size + realsize + 1);
	if(!ptr){
		printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}

	mem->memory = ptr;
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}

void releaseMemoryStruct(Data* data)
{
	free(data->memory);
}

void initMemoryStruct(Data* data)
{
	data->memory = malloc(1);
	data->size = 0;
}

void request(const char* url, char* buf, size_t size)
{
	CURL *curl;
	CURLcode res;
	Data chunk;
	char errbuf[CURL_ERROR_SIZE];

	initMemoryStruct(&chunk);
	buf[0] = 0; // empty string as default result

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if(curl){
		curl_easy_setopt(curl, CURLOPT_URL, url);
		// curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);
		errbuf[0] = 0;
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);

		curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

		res = curl_easy_perform(curl);
		if (res != CURLE_OK){
			size_t len = strlen(errbuf);
			if(len)
				fprintf(stderr, "%s%s", errbuf, ((errbuf[len-1]!='\n')?"\n":""));
			else
				fprintf(stderr, "curl_easy_perform() failed: %s for url: [%s]\n", curl_easy_strerror(res), url);
			return;
		} else {
			//printf("%s\n", chunk.memory);
		}

		curl_easy_cleanup(curl);

		curl_global_cleanup();
		snprintf(buf, size, "%s", chunk.memory);
	}
	releaseMemoryStruct(&chunk);
	//return chunk;
}

int parse_url(char* url)
{
	CURLU *h;
	CURLUcode uc;
	char *host;
	char *path;

	h = curl_url();
	if(!h)
		return 1;

	uc = curl_url_set(h, CURLUPART_URL, url, 0);
	if(uc)
		goto fail;

	uc = curl_url_get(h, CURLUPART_HOST, &host, 0);
	if(!uc){
		printf("Host: %s\n", host);
		curl_free(host);
	}

	uc = curl_url_get(h, CURLUPART_PATH, &path, 0);
	if(!uc){
		printf("Path: %s\n", path);
		curl_free(path);
	}

	uc = curl_url_set(h, CURLUPART_URL, "../ahother/second.html", 0);
	if(uc)
		goto fail;

	uc = curl_url_get(h, CURLUPART_PATH, &path, 0);
	if(!uc){
		printf("Path: %s\n", path);
		curl_free(path);
	}

fail:
	curl_url_cleanup(h);
	return 0;

}

