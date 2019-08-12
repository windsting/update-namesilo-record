using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace UpdateNamesiloRecord {
    class Namesilo {
        public readonly string ApiKey;
        public readonly string Domain;
        public readonly string RecordName;

        public Namesilo(string apiKey, string domain, string recordName) {
            ApiKey = apiKey;
            Domain = domain;
            RecordName = recordName;
        }

        public async Task<string> GetRecordId() {
            string dnsRecordsListUrl = $"/api/dnsListRecords?version=1&type=xml&key={ApiKey}&domain={Domain}";
            return await GetResponseNodeText(dnsRecordsListUrl, $"/namesilo/reply/resource_record[host='{RecordName}.{Domain}']/record_id");
        }

        public async Task<bool> UpdateRecordIp(string recordId, string ipAddress) {
            string dnsUpdateRecordUrl = $"/api/dnsUpdateRecord?version=1&type=xml&key={ApiKey}&domain={Domain}&rrid={recordId}&rrhost={RecordName}&rrvalue={ipAddress}&rrttl=7207";
            return await GetResponseNodeText(dnsUpdateRecordUrl, $"/namesilo/reply/detail") == "success";
        }

        async Task<string> GetResponseNodeText(string nsPath, string xmlPath) {
            var request = new Request();
            var strBody = await request.Get($"https://www.namesilo.com{nsPath}");
            XmlDocument document = new XmlDocument();
            document.LoadXml(strBody);
            var nodeText = document.SelectSingleNode(xmlPath).InnerText;
            return nodeText;
        }
    }
}
