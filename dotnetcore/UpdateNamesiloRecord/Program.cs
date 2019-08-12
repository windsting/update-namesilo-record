using System;
using System.Threading;
using System.Threading.Tasks;

namespace UpdateNamesiloRecord {
    class Program {
        static void Main(string[] args) {
            var envVars = EnvVars.CheckVars();
            if(envVars == null) {
                return;
            }
            CheckIp(envVars);
            Console.CancelKeyPress += new ConsoleCancelEventHandler(OnExit);
            _closing.WaitOne();
        }

        static async void CheckIp(EnvVars vars) {
            Utility.Log($"{AppDomain.CurrentDomain.FriendlyName} start at [{Utility.Now()}]");
            var ipFetcher = new IpFetcher(vars.IP_ECHO);
            var namesilo = new Namesilo(vars.API_KEY, vars.DOMAIN, vars.RECORD_NAME);

            Func<string, string, Task> update = async (string ip, string logPrefix) => {
                var recordId = await namesilo.GetRecordId();

                var succeed = await namesilo.UpdateRecordIp(recordId, ip);
                Utility.Log($"{logPrefix} got result: {succeed}");
            };

            string myIp = "None";
            int sameCount = 0;


            Func<Task> check = async () => {
                var t = Utility.Now();
                var newIp = await ipFetcher.GetMyIp();
                //Utility.Log($"ip is [{newIp}]");
                if (newIp != myIp) {
                    await update(newIp, $"[{t}] replace [{myIp}] to [{newIp}] after [{sameCount}] times");
                    myIp = newIp;
                } else {
                    sameCount += 1;
                    if ((sameCount % vars.PRINT_SAME_COUNT_STEP) == 0) {
                        await update(newIp, $"[{t}] make sure it's still [{myIp}] after [{sameCount}] times");
                    }
                }
            };

            while (Running) {
                try {
                    await check();
                } catch(Exception ex) {
                    Utility.Log($"[{Utility.Now()}] got exception:[{ex.ToString()}]");
                }

                await Task.Delay(vars.CHECK_INTERVAL_SECONDS * 1000);
            }
            _closing.Set();
        }

        private static bool Running { get; set; } = true;
        private static readonly AutoResetEvent _closing = new AutoResetEvent(false);
        protected static void OnExit(object sender, ConsoleCancelEventArgs args) {
            Running = false;
            Utility.Log($"gonna exit at {Utility.Now()}");
        }
    }
}
