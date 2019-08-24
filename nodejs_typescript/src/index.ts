#!/usr/bin/env node

import { log, now, normalIp, sleep } from "./utility";
import { request } from "./request";
import { fetchRecordIdFromXml, updateResult, recordListUrl, updateRecordUrl } from "./namesilo";
import { getEnvVars, EnvVars } from "./env_var";

async function updateRecordIp(envVars: EnvVars, recordId: string, ip: string): Promise<string> {
    const url = updateRecordUrl(envVars.API_KEY, envVars.DOMAIN, envVars.RECORD_NAME, recordId, ip);
    const xml = await request(url);
    return updateResult(xml);
}

async function fetchRecordIdFromUrl(envVars: EnvVars, ip: string, strLog: string): Promise<string> {
    const url = recordListUrl(envVars.API_KEY, envVars.DOMAIN);
    // log(`gonna request url: [${url}] with ip: [${ip}]`);
    const xml = await request(url);
    const recordHost = `${envVars.RECORD_NAME}.${envVars.DOMAIN}`;
    return fetchRecordIdFromXml(xml, recordHost);
}

async function getMyIp(envVars: EnvVars): Promise<string> {
    let newIp = await request(envVars.IP_ECHO);
    newIp = normalIp(newIp);
    return new Promise<string>((resolve, _) => {
        resolve(newIp);
    });
}

async function main(): Promise<void> {
    log(`update_namesilo_record start at [${now()}]`)
    const envVars: EnvVars | null = getEnvVars();
    if (envVars === null) {
        return;
    }

    let myIp: string = 'None';
    let sameCount: number = 0;
    const EmptyString = "";
    while (true) {
        try {
            const newIp = await getMyIp(envVars);
            let logContent: string = EmptyString;
            if (newIp === myIp) {
                sameCount += 1;
                if (sameCount % envVars.PRINT_SAME_COUNT_STEP == 0) {
                    logContent = `[${now()}] make sure it's still [${myIp}] after [${sameCount}] times`;
                }
            } else {
                logContent = `[${now()}] replace [${myIp}] to [${newIp}] after [${sameCount}] times`;
                myIp = newIp;
                sameCount = 0;
            }

            if (logContent !== EmptyString) {
                const recordId = await fetchRecordIdFromUrl(envVars, myIp, logContent);
                const result = await updateRecordIp(envVars, recordId, myIp);
                log(`${logContent} got result: ${result}`);
            }

            await sleep(envVars.CHECK_INTERVAL_SECONDS);
        } catch (error) {
            log(error.toString());
        }
    }
}

main();