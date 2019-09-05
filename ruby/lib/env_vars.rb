
class EnvVars
    attr_accessor :API_KEY,
                    :DOMAIN,
                    :RECORD_NAME,
                    :IP_ECHO,
                    :CHECK_INTERVAL_SECONDS,
                    :PRINT_SAME_COUNT_STEP

    def self.get_var(key, default_value=nil)
        value = ENV[key]
        unless value
            value = default_value
        end

        unless value
            p "Please set your #{key} in environment variable"
        end
        value
    end

    def self.get_var_int(key, default_value=nil)
        value = get_var(key, default_value)
        unless value
            value = default_value
        end
        value.to_i
    end

    def self.Instance
        apiKey = get_var('API_KEY')
        domain = get_var('DOMAIN')
        recordName = get_var('RECORD_NAME')
        ipEcho = get_var('IP_ECHO', "http://icanhazip.com/")
        checkIntervalSeconds = get_var_int('CHECK_INTERVAL_SECONDS', 60)
        printSameCountStep = get_var_int('PRINT_SAME_COUNT_STEP', 6 * 60)

        if apiKey && domain && recordName && ipEcho && checkIntervalSeconds && printSameCountStep
            EnvVars.new(apiKey, domain, recordName, ipEcho, checkIntervalSeconds, printSameCountStep)
        else
            nil
        end
    end

    def initialize(apiKey, domain, recordName, ipEcho, checkIntervalSeconds, printSameCountStep)
        @API_KEY = apiKey
        @DOMAIN = domain
        @RECORD_NAME = recordName
        @IP_ECHO = ipEcho
        @CHECK_INTERVAL_SECONDS = checkIntervalSeconds
        @PRINT_SAME_COUNT_STEP = printSameCountStep
    end
end
