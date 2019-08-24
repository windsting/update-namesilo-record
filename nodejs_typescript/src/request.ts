
import { request as http_request } from "http";
import { request as https_request } from "https";
import { log } from "./utility";

export const request = async function (url: string): Promise<string> {
    // log(`gonna request url: ${url}`);
    let req_func = http_request;
    if (url.startsWith('https://')) {
        req_func = https_request;
    }

    return new Promise<string>((resolve, reject) => {
        let req = req_func(url, (resp) => {
            let all_chunks: Uint8Array[] = [];
            resp.on('data', (chunk) => all_chunks.push(chunk));
            resp.on('end', () => { resolve(Buffer.concat(all_chunks).toString()) });
            resp.on('error', (error) => { reject(error) });
        });

        req.on('error', (error) => { reject(error) });
        req.end();
    });
}

