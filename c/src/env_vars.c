#include "env_vars.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

const char* getEnvD(const char* name, const char* defaultValue);
const char* getEnv(const char*);
int getEnvInt(const char*, const char*);

const int Valid = 1;
const int Invalid = 0;
int isVarsValid(EnvVars vars)
{
	if( vars.ApiKey != NULL
		&& vars.Domain != NULL
		&& vars.RecordName != NULL
		&& vars.checkIntervalSeconds > 0
		&& vars.printSameCountStep > 0)
		return Valid;
	return Invalid;
}

EnvVars gEnvVars;

EnvVars* getEnvVars()
{ 
	EnvVars* p = &gEnvVars;
	memset(p, 0, sizeof(gEnvVars));
	p->ApiKey = getEnv("API_KEY");
	p->Domain = getEnv("DOMAIN");
	p->RecordName = getEnv("RECORD_NAME");
	p->IpEcho = getEnvD("IP_ECHO", "http://icanhazip.com");
	p->checkIntervalSeconds = getEnvInt("CHECK_INTERVAL_SECONDS", "60");
	p->printSameCountStep = getEnvInt("PRINT_SAME_COUNT_STEP", "360");

	if(isVarsValid(gEnvVars) == Invalid)
		return NULL;
	
	return p;
}

const char* getEnvD(const char* name, const char* defaultValue)
{
	char* value = getenv(name);
	if(value)
		return value;
	if(defaultValue)
		return defaultValue;

	fprintf(stderr, "please set your [%s] in environment variable\n", name);
	return NULL;
}

const char* getEnv(const char* name)
{
	return getEnvD(name, NULL);
}

int getEnvInt(const char* name, const char* defaultValue)
{
	int i = 1;
	const char *v = getEnvD(name, defaultValue);
	if(v){
		i = atoi(v);
	}
	return i;

}


