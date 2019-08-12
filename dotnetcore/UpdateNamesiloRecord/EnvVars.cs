using System;
using System.Collections.Generic;
using System.Text;

namespace UpdateNamesiloRecord {
    class EnvVars {
        public string API_KEY { get; set; }
        public string DOMAIN { get; set; }
        public string RECORD_NAME { get; set; }
        public string IP_ECHO { get; set; }
        public int CHECK_INTERVAL_SECONDS;
        public int PRINT_SAME_COUNT_STEP;

        private bool IsValid { get; set; } = true;

        public const string IP_ECHO_DEFAULT = "http://ifconfig.co/ip";
        public const int CHECK_INTERVAL_SECONDS_DEFAULT = 60;
        public const int PRINT_SAME_COUNT_STEP_DEFAULT = 60*6;

        string GetEnvVar(string key, string defaultValue = null) {
            var value = Environment.GetEnvironmentVariable(key);
            if(value == null) {
                if (defaultValue == null) {
                    Utility.Log($"Please set your {key} in environment variable");
                    IsValid = false;
                    return null;
                } else {
                    return defaultValue;
                }
            }
            return value;
        }

        void GetIntEnvVar(string key, ref int value, string defaultValue = null) {
            var strValue = GetEnvVar(key, defaultValue);
            var succeed = int.TryParse(strValue, out value);
            if(succeed == false) {
                Utility.Log($"Please set {key} correctly in environment variables");
                if (defaultValue != null) {
                    value = int.Parse(defaultValue);
                } else {
                    IsValid = false;
                }
            }
        }

        public static EnvVars CheckVars() {
            var vars = new EnvVars();

            vars.API_KEY = vars.GetEnvVar(nameof(API_KEY));
            vars.DOMAIN = vars.GetEnvVar(nameof(DOMAIN));
            vars.RECORD_NAME = vars.GetEnvVar(nameof(RECORD_NAME));
            vars.IP_ECHO = vars.GetEnvVar(nameof(IP_ECHO), IP_ECHO_DEFAULT);

            vars.GetIntEnvVar(nameof(CHECK_INTERVAL_SECONDS), ref vars.CHECK_INTERVAL_SECONDS, CHECK_INTERVAL_SECONDS_DEFAULT.ToString());
            vars.GetIntEnvVar(nameof(PRINT_SAME_COUNT_STEP), ref vars.PRINT_SAME_COUNT_STEP, PRINT_SAME_COUNT_STEP_DEFAULT.ToString());

            //vars.LogVars();

            if (vars.IsValid == false) {
                return null;
            }

            return vars;
        }

        void LogVars() {
            Utility.Log($"API_KEY: [{API_KEY}]");
            Utility.Log($"DOMAIN: [{DOMAIN}]");
            Utility.Log($"RECORD_NAME: [{RECORD_NAME}]");
            Utility.Log($"IP_ECHO: [{IP_ECHO}]");
            Utility.Log($"CHECK_INTERVAL_SECONDS: [{CHECK_INTERVAL_SECONDS}]");
            Utility.Log($"PRINT_SAME_COUNT_STEP: [{PRINT_SAME_COUNT_STEP}]");
        }
    }
}
