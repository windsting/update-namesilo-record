require 'date'
require 'ipaddress'
require 'faraday'

def request(url)
    conn = Faraday.new do |c|
        c.adapter :net_http
    end
    conn.get(url).body
end

def now
    DateTime.now().strftime("%Y-%m-%d %H:%M:%S")
end

def get_ip(ipEcho)
    sBody = request(ipEcho).chomp
    
    return sBody if IPAddress.valid? sBody

    puts "got invalid ip: [#{sBody}]"
    nil
end
