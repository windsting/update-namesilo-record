package requests

import (
	"../util"
	"../models"
	"encoding/xml"
	"errors"
	"fmt"
)

const RecordIdTemplate = "https://www.namesilo.com/api/dnsListRecords?version=1&type=xml&key=%s&domain=%s"
func FetchRecordId(vars *util.EnvVars) (string, error) {
	host := fmt.Sprintf("%s.%s", vars.RecordName, vars.Domain)
	url := fmt.Sprintf(RecordIdTemplate, vars.ApiKey, vars.Domain)
	// byteValue, err := fetchUrl("_list_temp.xml")
	byteValue, err := fetchUrl(url)
	if err != nil {
		fmt.Println(err)
		return EmptyString, err
	}

	var records models.DnsListRecords
	xml.Unmarshal(byteValue, &records)

	// fmt.Println("Request Operation: " + records.Request.Operation)
	// fmt.Println("Reply code: " + records.Reply.Code)
	for i := 0; i < len(records.Reply.Records); i++ {
		if records.Reply.Records[i].Host == host {
			recordId := records.Reply.Records[i].RecordId
			return recordId, nil
		}
	}
	msg := fmt.Sprintf("no record found for host: %s", host)
	return EmptyString, errors.New(msg)
}

const UpdateRecordTemplate = "https://www.namesilo.com/api/dnsUpdateRecord?version=1&type=xml&key=%s&domain=%s&rrid=%s&rrhost=%s&rrvalue=%s&rrttl=7207"
func UpdateRecord(vars *util.EnvVars, recordId string, ip string) (bool, error) {
	url := fmt.Sprintf(UpdateRecordTemplate, vars.ApiKey, vars.Domain, recordId, vars.RecordName, ip)
	// byteValue, err := fetchUrl("../shell_script/_update_temp.xml")
	byteValue, err := fetchUrl(url)
	if err != nil {
		fmt.Println(err)
		return false, err
	}

	var result models.DnsUpdateRecord
	xml.Unmarshal(byteValue, &result)

	return result.Reply.Detail == "success", nil
}