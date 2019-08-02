package requests

import (
	"../util"
	"errors"
	"fmt"
)

const EmptyString = util.EmptyString

func GetIp(ipEcho string) (string, error) {
	if ipEcho == EmptyString {
		return EmptyString, errors.New("no IP_ECHO to get local ip")
	}

	body, err := fetchUrl(ipEcho)
	if err != nil {
		fmt.Println(err)
		return EmptyString, err
	}

	ip := string(body)
	return ip, nil
}
