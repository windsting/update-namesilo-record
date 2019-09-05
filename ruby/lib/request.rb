
require 'faraday'

class Request
    def self.get(url)
        conn = Faraday.new do |c|
            c.adapter :net_http
        end
        conn.get(url).body
    end
end
