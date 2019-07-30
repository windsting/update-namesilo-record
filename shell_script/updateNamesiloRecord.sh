#!/usr/bin/env sh

print(){
  echo $1
}

getMyIp(){
  echo $(curl -s ifconfig.co)
}

LIST_XML_FILE=_list_temp.xml
getRecordId(){
  curl -s "https://www.namesilo.com/api/dnsListRecords?version=1&type=xml&key=$API_KEY&domain=$DOMAIN" > $LIST_XML_FILE
  VAL=$(xmllint --xpath "/namesilo/reply/resource_record[host=\"$RECORD_NAME.$DOMAIN\"]/record_id/text()" $LIST_XML_FILE)
  echo $VAL
}

UPDATE_XML_FILE=_update_temp.xml
updateRecord(){
  RRID=$(getRecordId)
  if [ -z "$RRID" ]
  then
    print "got invalid RRID"
    return 1
  fi
  curl -s "https://www.namesilo.com/api/dnsUpdateRecord?version=1&type=xml&key=$API_KEY&domain=$DOMAIN&rrid=$RRID&rrhost=$RECORD_NAME&rrvalue=$MY_IP&rrttl=7207" > $UPDATE_XML_FILE
  RESULT=$(xmllint --xpath '/namesilo/reply/detail/text()' $UPDATE_XML_FILE)
  echo $RESULT
}

updateAndLog(){
  UPDATE_RESULT=$(updateRecord)
  # print $UPDATE_RESULT
  echo $UPDATE_RESULT
}

MY_LAST_IP=None
checkMyIp(){
  MY_IP=$(getMyIp)
  # print "new ip [$MY_IP]"
  NOW=$(now)
  if [ "$MY_LAST_IP" = "$MY_IP" ]
  then
    SAME_COUNT=$(expr $SAME_COUNT + 1)
    MODULO=$(expr $SAME_COUNT % $PRINT_SAME_COUNT_STEP)
    # print "Ip unchanged: $MY_IP"
    if [ "$MODULO" = "0"  ]
    then
      UPDATE_LOG="[$NOW] make sure it's still [$MY_IP] after [$SAME_COUNT] times"
      UPDATE_RESULT=$(updateRecord)
      print "$UPDATE_LOG [$UPDATE_RESULT]"
    fi
    return 0
  fi
  UPDATE_LOG="[$NOW] replace [$MY_LAST_IP] to [$MY_IP] after [$SAME_COUNT] times"
  UPDATE_RESULT=$(updateRecord)
  print "$UPDATE_LOG [$UPDATE_RESULT]"
  # print "[$NOW] replace [$MY_LAST_IP] to [$MY_IP] $UPDATE_RESULT"
  if [ "$UPDATE_RESULT" = "success" ]
  then
    MY_LAST_IP=$MY_IP
    SAME_COUNT=0
  fi
}

now(){
  echo $(date +'%Y-%b-%d %T')
}

exitScript(){
  trap "exit 1" TERM
  export TOP_PID=$$
  kill -s TERM $TOP_PID
}

validate(){
  VAR_NAME=$1
  VAR_VALUE=$2
  if [ -z "${VAR_VALUE}" ]
  then
    print "please set your [$VAR_NAME] in environment variable"
    exitScript
  fi
}

# log every (CHECK_INTERVAL_SECONDS * PRINT_SAME_COUNT_STEP) seconds for same ip
CheckIntervalSecondsDefault=60
PrintSameCountStepDefault=360

check_CHECK_INTERVAL_SECONDS(){
  if [ -z "${CHECK_INTERVAL_SECONDS}" ]
  then
    CHECK_INTERVAL_SECONDS=$CheckIntervalSecondsDefault
  fi
}

check_PRINT_SAME_COUNT_STEP(){
  if [ -z "${PRINT_SAME_COUNT_STEP}" ]
  then
    PRINT_SAME_COUNT_STEP=$PrintSameCountStepDefault
  fi
}

validateEnvironmentVariable(){
  validate API_KEY $API_KEY
  validate DOMAIN $DOMAIN
  validate RECORD_NAME $RECORD_NAME
  check_CHECK_INTERVAL_SECONDS
  check_PRINT_SAME_COUNT_STEP
}

printAll(){
  print "API_KEY: $API_KEY"
  print "DOMAIN: $DOMAIN"
  print "RECORD_NAME: $RECORD_NAME"
  print "CHECK_INTERVAL_SECONDS: $CHECK_INTERVAL_SECONDS"
  print "PRINT_SAME_COUNT_STEP: $PRINT_SAME_COUNT_STEP"
}

checkArithmetic(){
  print "SAME_COUNT:  $SAME_COUNT"
  print "SAME_COUNT + 1 : $(expr $SAME_COUNT + 1)"
  SAME_COUNT=$(expr $SAME_COUNT + 1)
  print 'SAME_COUNT after SAME_COUNT=$(expr $SAME_COUNT + 1)'
  print "\t$SAME_COUNT"
  EQUAL=$(expr $SAME_COUNT = 2)
  print "EQUAL: $EQUAL"
}

SAME_COUNT=0
COMMAND_NAME=$0
main(){
  validateEnvironmentVariable
  # printAll
  print "$COMMAND_NAME start at [$(now)]"
  while :
  do
    checkMyIp
    sleep $CHECK_INTERVAL_SECONDS
  done
}

# checkArithmetic
main
