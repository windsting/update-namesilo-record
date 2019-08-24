import { setTimeout } from "timers";

export const log = function (content: string): void {
    console.log(content);
}

function pad(num: number, size: number): string {
    var s = num + "";
    while (s.length < size) s = "0" + s;
    return s;
}

function pad2(num: number): string {
    return pad(num, 2);
}

export const now = function (): string {
    const t = new Date();
    return `${t.getFullYear()}-${pad2(t.getMonth() + 1)}-${pad2(t.getDate())} ${pad2(t.getHours())}:${pad2(t.getMinutes())}:${pad2(t.getSeconds())}`;
}

function replaceAll(target: string, search: string, replacement: string) {
    return target.split(search).join(replacement);
}

function replaceForIp(target: string, search: string): string {
    return replaceAll(target, search, "");
}

export const normalIp = function (orgIp: string): string {
    let temp = orgIp;
    temp = replaceForIp(temp, '\n');
    temp = replaceForIp(temp, '\r');
    temp = replaceForIp(temp, '\t');
    temp = replaceForIp(temp, ' ');
    return temp;
}

export const sleep = async function (seconds: number): Promise<void> {
    return new Promise<void>((resolve, _) => {
        setTimeout(() => { resolve(); }, seconds * 1000);
    });
}
