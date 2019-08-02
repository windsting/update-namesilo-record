package models

import (
	"encoding/xml"
)

type DnsUpdateRecord struct {
	XMLNAME xml.Name				`xml:"namesilo"`
	Request	NamesiloRequest	`xml:"request"`
	Reply		DurReply				`xml:"reply"`
}

type DurReply struct {
	XMLNAME 	xml.Name	`xml:"reply"`
	Code			string		`xml:"code"`
	Detail		string		`xml:"detail"`
	RecordId	string		`xml:"record_id"`
}