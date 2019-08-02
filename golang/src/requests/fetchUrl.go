package requests

import (
	"fmt"
	"io/ioutil"
	"net/http"
	"strings"
	"os"
)

func fetchUrl(url string)([]byte, error){
	if strings.HasPrefix(url, "http") == false {
		return fetchUrlFake(url)
	}

	resp, err := http.Get(url)
	if err != nil {
		fmt.Println(err)
		return nil, err
	}

	defer resp.Body.Close()

	return ioutil.ReadAll(resp.Body)
}

func fetchUrlFake(url string) ([]byte, error) {
	xmlFile, err := os.Open(url)
	if err != nil {
		fmt.Println(err)
		return nil, err
	}

	fmt.Printf("Successfully Opened %s\n", url)
	defer xmlFile.Close()

	byteValue, _ := ioutil.ReadAll(xmlFile)
	return byteValue, nil
}