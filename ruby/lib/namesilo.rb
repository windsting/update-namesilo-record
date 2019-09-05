require 'faraday'
require 'addressable'
require 'nokogiri'

# The codes are extracted from https://rubygems.org/gems/namesilo_client
#   thanks to the author Yuxi

class Namesilo
    def initialize(apiKey)
        @apiKey = apiKey
        @host = 'https://www.namesilo.com/api/'
    end
    
    def get_connection()
        conn = Faraday.new(:url => @host) do |c|
            c.request :url_encoded
            c.adapter :net_http
        end
    end

    def get_default_parameters()
        {"version":"1","type":"xml","key":@apiKey}
    end

    def get_request(endpoint)
        get_connection().get endpoint
    end

    def get_url_parameters(params)
        uri = Addressable::URI.new
        uri.query_values = params.merge(get_default_parameters())
        uri.query
    end

    def get_domain_records(domain)
        get_request('dnsListRecords?'+get_url_parameters({"domain": domain})).body
    end

    def get_record_id(domain, recordName)
        doc =Nokogiri::XML(get_domain_records(domain))
        rid = doc.xpath("/namesilo/reply/resource_record[host=\"#{recordName}.#{domain}\"]/record_id").text()
    end

    def update_record_ip(domain, recordName, recordId, ip)
        body = get_request('dnsUpdateRecord?'+get_url_parameters({"domain": domain,"rrid":recordId,"rrhost":recordName,"rrvalue":ip})).body
        doc = Nokogiri::XML(body)
        result = doc.xpath('/namesilo/reply/detail').text()
    end
end
