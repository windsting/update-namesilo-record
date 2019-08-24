
import { log } from "./utility";

export interface EnvVars {
    API_KEY: string,
    DOMAIN: string,
    RECORD_NAME: string,
    IP_ECHO: string,
    CHECK_INTERVAL_SECONDS: number,
    PRINT_SAME_COUNT_STEP: number,
}

const IP_ECHO_DEFAULT = 'http://icanhazip.com/';
const CHECK_INTERVAL_SECONDS_DEFAULT = 60;
const PRINT_SAME_COUNT_STEP_DEFAULT = 6 * 60;

function isEnvVarsVallid(obj: EnvVars): boolean {
    if (obj.API_KEY
        && obj.DOMAIN
        && obj.RECORD_NAME
        && obj.IP_ECHO
        && obj.CHECK_INTERVAL_SECONDS > 0
        && obj.PRINT_SAME_COUNT_STEP > 0)
        return true;

    return false;
}

function getEnv(key: string, defaultValue: string | undefined = undefined): string {
    if (process.env[key] !== undefined) {
        let value: any = process.env[key]
        return value;
    } else if (defaultValue !== undefined) {
        return defaultValue;
    }

    log(`Please set your ${key} in environment variable`)
    return "";
}

function getEnvInt(key: string, defaultValue: string | undefined): number {
    const strValue = getEnv(key, defaultValue);
    if (strValue !== "") {
        return parseInt(strValue);
    }

    return 0;
}

export const getEnvVars = function (): EnvVars | null {
    const envVars: EnvVars = {
        API_KEY: getEnv('API_KEY'),
        DOMAIN: getEnv('DOMAIN'),
        RECORD_NAME: getEnv('RECORD_NAME'),
        IP_ECHO: getEnv('IP_ECHO', IP_ECHO_DEFAULT),
        CHECK_INTERVAL_SECONDS: getEnvInt('CHECK_INTERVAL_SECONDS', `${CHECK_INTERVAL_SECONDS_DEFAULT}`),
        PRINT_SAME_COUNT_STEP: getEnvInt('PRINT_SAME_COUNT_STEP', `${PRINT_SAME_COUNT_STEP_DEFAULT}`),
    }

    if (isEnvVarsVallid(envVars)) {
        if (envVars.IP_ECHO.startsWith('http') === false) {
            envVars.IP_ECHO = 'http://' + envVars.IP_ECHO;
        }
        return envVars;
    }

    return null;
}