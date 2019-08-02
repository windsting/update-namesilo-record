#!/bin/sh

go build -o app -ldflags="-s -w" src/main.go
