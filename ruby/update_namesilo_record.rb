#!/usr/bin/env ruby

require './lib/namesilo'
require './lib/env_vars'
require './lib/utility'

def update_ip(envVars, ip)
    ns = Namesilo.new(envVars.API_KEY)
    rid = ns.get_record_id(envVars.DOMAIN, envVars.RECORD_NAME)

    # puts "rid is: [#{rid}]"

    result = ns.update_record_ip(envVars.DOMAIN, envVars.RECORD_NAME, rid, ip)
end

SUCCESS = "success"

def check_ip(envVars, myIp, sameCount)
    newIp = get_ip(envVars.IP_ECHO)
    if newIp == myIp
        sameCount += 1
        if sameCount % envVars.PRINT_SAME_COUNT_STEP == 0
            result = update_ip(envVars, newIp)
            log = "[#{now()}] make sure it's still [#{myIp}] after [#{sameCount}] times got result: [#{result}]"
        end
    elsif newIp != nil    # public ip address changed
        result = update_ip(envVars, newIp)
        log = "[#{now()}] replace [#{myIp}] to [#{newIp}] after [#{sameCount}] times got result: [#{result}]"

        if result == SUCCESS
            sameCount = 0
            myIp = newIp
        else
            sameCount += 1
        end
    end

    puts log if log

    return myIp, sameCount
end

def main()
    envVars = EnvVars.Instance
    if envVars == nil
        return
    end

    puts "#{$PROGRAM_NAME} start at [#{now()}]"

    myIp = 'None'
    sameCount = 0

    while
        begin
            myIp, sameCount = check_ip(envVars, myIp, sameCount)
        rescue => error
            puts "[#{now()}] #{error.inspect}"
            next
        ensure
            sleep envVars.CHECK_INTERVAL_SECONDS
        end
    end
end

main
