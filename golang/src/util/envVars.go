package util

import (
	"fmt"
	"os"
	"strconv"
)

const EmptyString = ""
const IpEchoDefault = "http://icanhazip.com/"

// log every (CHECK_INTERVAL_SECONDS * PRINT_SAME_COUNT_STEP) seconds for same ip
var CheckIntervalSecondsDefault = strconv.Itoa(60)
var PrintSameCountStepDefault = strconv.Itoa(60 * 6)

type EnvVars struct {
	ApiKey string
	Domain string
	RecordName string
	IpEcho string
	CheckIntervalSeconds int
	PrintSameCountStep int
}

func (v *EnvVars) Print (){
	fmt.Printf("API_KEY: %s\n", v.ApiKey)
	fmt.Printf("DOMAIN: %s\n", v.Domain)
	fmt.Printf("RECORD_NAME: %s\n", v.RecordName)
	fmt.Printf("IP_ECHO: %s\n", v.IpEcho)
	fmt.Printf("CHECK_INTERVAL_SECONDS: %d\n", v.CheckIntervalSeconds)
	fmt.Printf("PRINT_SAME_COUNT_STEP: %d\n", v.PrintSameCountStep)
}

func GetEnvVars(vars *EnvVars) bool {
	var result uint = 0
	vars.ApiKey = getEnv("API_KEY", EmptyString, &result)
	vars.Domain = getEnv("DOMAIN", EmptyString, &result)
	vars.RecordName = getEnv("RECORD_NAME", EmptyString, &result)
	vars.IpEcho = getEnv("IP_ECHO", IpEchoDefault, &result)
	vars.CheckIntervalSeconds = getEnvInt("CHECK_INTERVAL_SECONDS", CheckIntervalSecondsDefault, &result)
	vars.PrintSameCountStep = getEnvInt("PRINT_SAME_COUNT_STEP", PrintSameCountStepDefault, &result)
	return result == 0
}

func getEnvInt(key string, defaultValue string, zeroForSuccess *uint) int {
	sValue := getEnv(key, defaultValue, zeroForSuccess)
	// fmt.Printf("getEnvInt got sValue: [%s]\n", sValue)
	iValue, err := strconv.Atoi(sValue)
	if err != nil {
		if defaultValue != EmptyString {
			// fmt.Printf("gonna parse default value from: [%s]\n", defaultValue)
			iValue, _ = strconv.Atoi(defaultValue)
		} else {
			*zeroForSuccess |= uint(1)
		}
	}

	// fmt.Printf("getEnvInt got iValue: [%d]\n", iValue)
	return iValue
}

func getEnv(key string, defaultValue string, zeroForSuccess *uint) (string) {
	value := os.Getenv(key)
	if value == EmptyString {
		if defaultValue == EmptyString {
			fmt.Printf("Please set your %s in environment variable\n", key)
			*zeroForSuccess |= 1
		} else {
			value = defaultValue
		}
	}
	return value
}