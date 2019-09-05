# Update IP address for namesilo record

This is ruby version.

## script usage

Commands below assume executed in the directory where **update_namesilo_record.rb** can be found.

1. Make sure **ruby** installed
1. Install required gems:

    `bundle`

    in case of error occured for installation of **nokogiri**, please check [nokogiri's installation tutorial](http://www.nokogiri.org/tutorials/installing_nokogiri.html)

1. Set environment variable to `API_KEY`, `DOMAIN`, `RECORD_NAME`, it depends on the OS you're using, assume you knew how to do it
1. run script

    `ruby update_namesilo_record.rb`
