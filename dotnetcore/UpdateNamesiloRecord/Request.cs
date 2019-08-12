using System;
using System.Collections.Generic;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;

namespace UpdateNamesiloRecord {
    class Request {
        public async Task<string> Get(string url) {
            HttpClient client = new HttpClient();
            var response = await client.GetAsync(url);
            response.EnsureSuccessStatusCode();
            //string responseBody = await response.Content.ReadAsStringAsync();
            // Above three lines can be replaced with new helper method below
            // string responseBody = await client.GetStringAsync(uri);

            //Console.WriteLine(responseBody);
            return await response.Content.ReadAsStringAsync();
        }
    }
}
