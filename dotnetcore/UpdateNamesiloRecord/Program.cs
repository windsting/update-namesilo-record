using System;
using System.Threading.Tasks;

namespace UpdateNamesiloRecord {
    class Program {
        static void Main(string[] args) {
            var envVars = EnvVars.CheckVars();
            CheckIp(envVars).Wait();
        }

        static async Task CheckIp(EnvVars vars) {
            var ipFetcher = new IpFetcher(vars.IP_ECHO);
            var ip = await ipFetcher.GetMyIp();
            Console.WriteLine($"ip is [{ip}]");

            var namesilo = new Namesilo(vars.API_KEY, vars.DOMAIN, vars.RECORD_NAME);
            var recordId = await namesilo.GetRecordId();

            var succeed = await namesilo.UpdateRecordIp(recordId, ip);
            Console.WriteLine($"UpdateRecordIp got result: {succeed}");

            Console.ReadKey();
        }
    }
}
