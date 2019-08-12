using System;
using System.Collections.Generic;
using System.Text;

namespace UpdateNamesiloRecord {
    class EnvVars {
        public string API_KEY { get; set; }
        public string DOMAIN { get; set; }
        public string RECORD_NAME { get; set; }
        public string CHECK_INTERVAL_SECONDS { get; set; }
        public string PRINT_SAME_COUNT_STEP { get; set; }
        public string IP_ECHO { get; set; }

        public const string IP_ECHO_DEFAULT = "http://ifconfig.co/ip";

        static string GetEnvVar(string key, string defaultValue = null) {
            var value = Environment.GetEnvironmentVariable(key);
            if(value == null) {
                if (defaultValue == null) {
                    Console.WriteLine($"Please set your {key} in environment variable");
                    return null;
                } else {
                    return defaultValue;
                }
            }
            return value;
        }

        public static EnvVars CheckVars() {
            return new EnvVars {
                API_KEY = GetEnvVar(nameof(API_KEY)),
                DOMAIN = GetEnvVar(nameof(DOMAIN)),
                RECORD_NAME = GetEnvVar(nameof(RECORD_NAME)),
                IP_ECHO = GetEnvVar(nameof(IP_ECHO), IP_ECHO_DEFAULT),
                //API_KEY = GetEnvVar(nameof(API_KEY)),
            };
        }
    }
}
