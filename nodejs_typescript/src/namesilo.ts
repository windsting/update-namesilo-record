import { parseString } from "xml2js";

interface Request {
    operation: string,
    ip: string
}

interface Reply {
    code: string[],
    detail: string[],
}

interface ResourceRecord {
    record_id: string[],
    type: string[],
    host: string[],
    value: string[],
    ttl: string[],
    distance: string[],
}

interface RecordListReply extends Reply {
    resource_record: ResourceRecord[],
}

interface RecordListResult {
    request: Request[],
    reply: RecordListReply[],
}

interface ObjectResult {
    request: Request[],
    reply: Reply[],
}

interface RecordUpdateReply extends Reply {
    record_id: string[]
}

interface RecordUpdateResult {
    request: Request[],
    reply: RecordUpdateReply[],
}

interface ObjectNS {
    namesilo: ObjectResult
}

interface RecordListNS {
    namesilo: RecordListResult
}

interface RecordUpdateNS {
    namesilo: RecordUpdateResult;
}

const SucceedDetail: string = 'success';
function getReplyDetail(obj: ObjectNS): string {
    if (obj != null
        && obj.namesilo != null
        && obj.namesilo.reply != null && obj.namesilo.reply.length > 0
        && obj.namesilo.reply[0].detail != null && obj.namesilo.reply[0].detail.length > 0) {
        return obj.namesilo.reply[0].detail[0];
    }

    return "";
}

async function parseXml(xml: string): Promise<any> {
    return new Promise<any>((resolve, reject) => {
        parseString(xml, (error, result) => {
            if (error) {
                reject(error);
            } else {
                resolve(result);
            }
        });
    });
}

export const fetchRecordIdFromXml = async function (xml: string, recordHost: string): Promise<string> {
    const recordListObj: RecordListNS = await parseXml(xml);
    return new Promise<string>((resolve, reject) => {
        const detail = getReplyDetail(recordListObj);
        if (detail !== SucceedDetail) {
            reject(new Error(`fetchRecordIdFromXml got detail: ${detail}`));
            return;
        }
        let recordList = recordListObj.namesilo.reply[0].resource_record;
        for (let index = 0; index < recordList.length; index++) {
            const record = recordList[index];
            if (record.host != null
                && record.host.length > 0
                && record.host[0] == recordHost
                && record.record_id != null
                && record.record_id.length > 0) {
                resolve(record.record_id[0]);
            }
        }
        reject(new Error(`no record found for recordHost: ${recordHost}`));
    });
}

export const updateResult = async function (xml: string): Promise<string> {
    const updateResultObj: RecordUpdateNS = await parseXml(xml);
    return new Promise<string>((resolve, _) => {
        resolve(getReplyDetail(updateResultObj));
    });
}

const NamesiloHost: string = "https://www.namesilo.com";
export const recordListUrl = function (apiKey: string, domain: string): string {
    return `${NamesiloHost}/api/dnsListRecords?version=1&type=xml&key=${apiKey}&domain=${domain}`;
}

export const updateRecordUrl = function (apiKey: string, domain: string, recordName: string, recordId: string, ipAddress: string): string {
    return `${NamesiloHost}/api/dnsUpdateRecord?version=1&type=xml&key=${apiKey}&domain=${domain}&rrid=${recordId}&rrhost=${recordName}&rrvalue=${ipAddress}&rrttl=7207`;
}