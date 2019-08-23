#pragma once

typedef struct structEnvVars{
	const char *ApiKey;
	const char *Domain;
	const char *RecordName;
	const char *IpEcho;
	int checkIntervalSeconds;
	int printSameCountStep;
} EnvVars;

EnvVars* getEnvVars();

