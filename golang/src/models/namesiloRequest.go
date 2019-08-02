package models

import (
	"encoding/xml"
)

type NamesiloRequest struct {
	XMLNAME		xml.Name 	`xml:"request"`
	Operation	string		`xml:"operation"`
	Ip    		string		`xml:"ip"`
}