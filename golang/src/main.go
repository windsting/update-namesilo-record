package main

import (
	"./requests"
	"./util"
	"fmt"
	"os"
	"strings"
	"time"
)

const EmptyString = util.EmptyString

func main() {
	var vars util.EnvVars
	succeed := util.GetEnvVars(&vars)
	if succeed == false {
		return
	}
	// vars.Print()

	checkIp(&vars)
}

func checkIp(vars *util.EnvVars) {
	myIp := "None"
	sameCount := 0

	for {
		now := time.Now().Format("2006-01-02 15:04:05")

		ip, err := requests.GetIp(vars.IpEcho)
		if err != nil {
			fmt.Fprintf(os.Stderr, "[%s] GetIp got error: %s\n", now, err)
			continue
		}

		ip = strings.Replace(ip, "\n", EmptyString, -1)
		// fmt.Println("GetIp result: " + ip)
		if ip != EmptyString {
			if ip != myIp {
				log := fmt.Sprintf("[%s] replace [%s] to [%s] after [%d] times", now, myIp, ip, sameCount)
				changeIp(vars, ip, log)
				sameCount = 0
				myIp = ip
			} else {
				sameCount += 1
				if sameCount%vars.PrintSameCountStep == 0 {
					log := fmt.Sprintf("[%s] make sure it's still [%s] after [%d] times", now, myIp, sameCount)
					changeIp(vars, ip, log)
				}
			}
		}

		time.Sleep(time.Duration(vars.CheckIntervalSeconds) * time.Second)
	}
}

func changeIp(vars *util.EnvVars, ip string, log string) {
	recordId, err := requests.FetchRecordId(vars)
	if err != nil {
		fmt.Println(err)
		return
	}

	// fmt.Printf("got record_id: %s\n", recordId)

	succeed, err := requests.UpdateRecord(vars, recordId, ip)
	if err != nil {
		fmt.Println(err)
		return
	}

	if succeed {
		fmt.Println(log)
	}
}
