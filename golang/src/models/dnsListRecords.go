package models

import (
	"encoding/xml"
)

type DnsListRecords struct {
	XMLNAME xml.Name		`xml:"namesilo"`
	Request	NamesiloRequest	`xml:"request"`
	Reply		DlrReply		`xml:"reply"`
}

type DlrReply struct {
	XMLNAME xml.Name 	`xml:"reply"`
	Code		string		`xml:"code"`
	Detail	string		`xml:"detail"`
	Records []ResourceRecord `xml:"resource_record"`
}

type ResourceRecord struct {
	XMLNAME xml.Name 	`xml:"resource_record"`
	RecordId	string	`xml:"record_id"`
	Type			string	`xml:"type"`
	Host			string	`xml:"host"`
	Value			string	`xml:"value"`
	Ttl				string	`xml:"ttl"`
	Distance	string	`xml:"distance"`
}