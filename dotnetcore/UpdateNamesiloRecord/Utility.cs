using System;
using System.Collections.Generic;
using System.Text;

namespace UpdateNamesiloRecord {
    public class Utility {
        public static void Log(string content) {
            Console.WriteLine(content);
        }

        public static void Log(string fmt, params object[] args) {
            Log(string.Format(fmt, args));
        }

        public static string Now() {
            return DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"); ;
        }
    }
}
