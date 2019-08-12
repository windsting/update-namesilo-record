using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

namespace UpdateNamesiloRecord {
    class IpFetcher {
        public readonly string IpEcho;
        public IpFetcher(string ipEcho) {
            IpEcho = ipEcho;
        }

        public async Task<string> GetMyIp() {
            var request = new Request();
            var responseBody = await request.Get(IpEcho);
            return responseBody.Replace("\n", "");
        }
    }
}
