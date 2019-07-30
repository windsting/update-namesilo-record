#!/usr/bin/env python

import os, time, sys
from datetime import datetime
from urllib.request import Request, urlopen
from lxml import objectify

def log(content):
    print(content)

def atoi(stringToInt, defaultValue):
    value = None
    try:
        value = int(stringToInt)
    except ValueError:
        value = defaultValue
    return value

def askForEnvironmentVariable(key, default_value, value):
    if default_value is None and value is None:
        log(f"Please set your {key} in environment variable")

def getEnv(key, default_value = None):
    value = None
    try:
        value = os.getenv(key, default_value)
        askForEnvironmentVariable(key, default_value, value)
    except KeyError:
        askForEnvironmentVariable(key, default_value, None)
    finally:
        pass
    # log(f"got: {value}, for:{key}")
    return value

def getEnvInt(key, default_value):
    sValue = getEnv(key, default_value)
    return atoi(sValue, default_value)

def getMyIp(t):
    newIp = None
    try:
        newIp = urlopen("http://icanhazip.com/").read()
        newIp = newIp.decode("utf-8").replace("\n", "")
    except Exception as ex:
        log(f"[{t}] got except[{str(ex)}] while get newIp")
    finally:
        pass
    # log(f"newIp: {newIp}")
    return newIp

def fetchRecord(data):
    "fetch record id from the result of request on dnsListRecords"
    global RECORD_NAME
    record_id = None
    reply = data.reply
    for child in reply.resource_record:
        # log(child.host)
        if child.host.text.startswith(f"{RECORD_NAME}."):
            # log(f"got record_id:[{child.record_id}] for:[{child.host.text}]")
            record_id = child.record_id.text
            break
    return record_id

def req(path, func):
    "send a request to namesilo, use func to process result data"
    baseUrl = 'https://www.namesilo.com'
    url = baseUrl + path
    # log(url)
    reqObject = Request(url, headers={'User-Agent' : "Magic Browser"})
    try:
        resp = urlopen(reqObject).read()
        # log(resp)
        resp = objectify.fromstring(resp)
        if func != None:
            func(resp)
    except Exception as ex:
        log(f"got except[{str(ex)}] while get {path}")
    finally:
        pass

def logUpdateResult(data, updateLog):
    log(f"{updateLog} [{data.reply.detail}]")

def processDomainList(data, ip, updateLog):
    "set ip for record_id fetched from data"
    global RECORD_NAME
    record_id = fetchRecord(data)
    if record_id is None:
        log(f"no record_id found for RECORD_NAME:[{RECORD_NAME}], please check it")
        sys.exit()
        return
    pathUpdateRecord = pathUpdateRecordTemplate.replace("RRID", record_id).replace("IPADDRESS", ip)
    # log(pathUpdateRecord)
    req(pathUpdateRecord, lambda data: logUpdateResult(data, updateLog))

CheckIntervalSecondsDefault = 60
PrintSameCountStepDefault = 60 * 6 # log every (CHECK_INTERVAL_SECONDS * PRINT_SAME_COUNT_STEP) seconds for same ip

RECORD_NAME = None
dnsListRecords = None
pathUpdateRecordTemplate = None
CHECK_INTERVAL_SECONDS = CheckIntervalSecondsDefault
PRINT_SAME_COUNT_STEP = PrintSameCountStepDefault
def initTemplates():
    global RECORD_NAME, dnsListRecords, pathUpdateRecordTemplate, CHECK_INTERVAL_SECONDS, PRINT_SAME_COUNT_STEP
    API_KEY = getEnv('API_KEY')
    DOMAIN = getEnv('DOMAIN')
    RECORD_NAME = getEnv('RECORD_NAME')
    CHECK_INTERVAL_SECONDS = getEnvInt("CHECK_INTERVAL_SECONDS", CheckIntervalSecondsDefault)
    PRINT_SAME_COUNT_STEP = getEnvInt("PRINT_SAME_COUNT_STEP", PrintSameCountStepDefault)
    if API_KEY is None or DOMAIN is None or RECORD_NAME is None:
        return False

    dnsListRecords = f'/api/dnsListRecords?version=1&type=xml&key={API_KEY}&domain={DOMAIN}'
    pathUpdateRecordTemplate = f'/api/dnsUpdateRecord?version=1&type=xml&key={API_KEY}&domain={DOMAIN}&rrid=RRID&rrhost={RECORD_NAME}&rrvalue=IPADDRESS&rrttl=7207'
    # log(f"API_KEY: {API_KEY}")
    # log(f"DOMAIN: {DOMAIN}")
    # log(f"RECORD_NAME: {RECORD_NAME}")
    # log(f"CHECK_INTERVAL_SECONDS: {CHECK_INTERVAL_SECONDS}")
    # log(f"PRINT_SAME_COUNT_STEP: {PRINT_SAME_COUNT_STEP}")
    # log(f"dnsListRecords: {dnsListRecords}")
    # log(f"pathUpdateRecordTemplate: {pathUpdateRecordTemplate}")
    return True

def now():
    t = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    return t

def scriptName():
    return os.path.basename(__file__)

def checkMyIp():
    if False == initTemplates():
        return
    log(f"{scriptName()} start at [{now()}]")
    myIp = None
    sameCount = 0
    while True:
        t = now()
        newIp = getMyIp(t)
        if newIp != None:
            if newIp != myIp:
                replaceLog = f"[{t}] replace [{myIp}] to [{newIp}] after [{sameCount}] times"
                req(dnsListRecords, lambda data: processDomainList(data, newIp, replaceLog))
                sameCount = 0
                myIp = newIp
            else :
                sameCount += 1
                if sameCount % PRINT_SAME_COUNT_STEP == 0:
                    replaceLog = f"[{t}] make sure it's still [{myIp}] after [{sameCount}] times"
                    req(dnsListRecords, lambda data: processDomainList(data, newIp, replaceLog))

        time.sleep(CHECK_INTERVAL_SECONDS)

checkMyIp()


