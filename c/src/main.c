#include "request.h"
#include "env_vars.h"
#include "namesilo.h"
#include "util.h"

#include <unistd.h>
#include <signal.h>

const size_t BufSizeShort = 1024;
const size_t BufSize = 1024 * 32;

void checkMyIp(EnvVars*);

void sigfunc(int sig)
{
	// printf("got sig: %d\n", sig);
	exit(0);
}

int main(int argc, char **argv)
{
	signal(SIGINT, sigfunc);
	EnvVars* pEnvVars = getEnvVars();
	if(pEnvVars == NULL){
		return 1;
	}
	char now[BufSizeShort];
	getNow(now, sizeof(now));
	printf("%s start at [%s]\n", argv[0], now);

	checkMyIp(pEnvVars);
}

void getIp(const char* ipEcho, char *s, size_t size)
{
	request(ipEcho, s, size);
	removeWhitespace(s);
}

int getRecordId(EnvVars* pVars, char* s, size_t size)
{
	char ridUrl[BufSizeShort];
	char idPath[BufSizeShort];
	char buf[BufSize];
	genRecordIdUrl(ridUrl, sizeof(ridUrl), pVars->ApiKey, pVars->Domain);
	// printf("ridUrl is: [%s]\n", ridUrl);
	snprintf(idPath, sizeof(idPath), "//record_id[../host='%s.%s']", pVars->RecordName, pVars->Domain);
	// printf("idPath is: [%s]\n", idPath);

	s[0] = 0;
	request(ridUrl, buf, sizeof(buf));
	// fprintf(stderr, "getRecordId buf: [%s], len: [%ld]\n", buf, strlen(buf));
	if(strlen(buf)>0)
		xmlFetchPath(buf, sizeof(buf), idPath, s, size);
	// fprintf(stderr, "rid is:[%s], len: [%ld]\n", s, strlen(s));
}

void updateRecord(EnvVars* pVars, const char* rid, const char* ip, char* s, size_t size)
{
	char updateUrl[BufSizeShort];
	char buf[BufSize];
	genUpdateUrl(updateUrl, sizeof(updateUrl),pVars->ApiKey,pVars->Domain, rid, pVars->RecordName, ip);
	// printf("updateUrl is: [%s]\n", updateUrl);

	s[0] = 0;
	request(updateUrl, buf, sizeof(buf));
	// fprintf(stderr, "updateRecord buf: [%s]\n", buf);
	if(strlen(buf)>0)
		xmlFetchPath(buf, sizeof(buf), "//detail", s, size);
	// fprintf(stderr, "update detail is:[%s]\n", s);
}

void checkMyIp(EnvVars* pVars)
{
	char myIp[BufSizeShort];
	char ip[BufSizeShort];
	char rid[BufSizeShort];
	char buffer[BufSize];
	char updateDetail[BufSizeShort];
	char now[BufSizeShort];
	char log[BufSizeShort];
	int sameCount = 0;

	snprintf(myIp, BufSizeShort, "None"); // init myIp
	getNow(now, sizeof(now));

	while(1) {
		getIp(pVars->IpEcho, ip, BufSizeShort);
		// printf("newIp is: [%s]\n", ip);
		if(strlen(ip) > 0) {
			if(strcmp(myIp,ip) != 0){
				// ip changed
				snprintf(log, sizeof(log), "[%s] replace [%s] to [%s] after [%d] times", now, myIp, ip, sameCount);
				snprintf(myIp, sizeof(myIp), "%s", ip);
				sameCount = 0;
			} else {
				sameCount++;
				if(sameCount % pVars->printSameCountStep == 0){
					snprintf(log, sizeof(log), "[%s] makesure it's still [%s] after [%d] times", now, myIp, sameCount);
				} else {
					log[0] = '\0';
				}
			}
			if(strlen(log) > 0) {
				getRecordId(pVars, rid, sizeof(rid));
				if(strlen(rid) > 0) {
					updateRecord(pVars, rid, ip, updateDetail, sizeof(updateDetail));
					printf("%s, got result: %s\n", log, updateDetail);
				}
			}

		}
		sleep(pVars->checkIntervalSeconds);
	}

}

