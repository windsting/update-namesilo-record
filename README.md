# Update IP address for namesilo record

Update the ipaddress for a record in [namesilo](https://www.namesilo.com/index.php), like `myhome-dynamic-ip.example.com`.

## prerequire for using

I'm gonna use the `myhome-dynamic-ip.example.com` as the **example**, so just replace with your real values.

- **Python 3** installed in your OS
- **Your API Key**: get it from [namesilo API manager page](https://www.namesilo.com/account/api-manager)
- **Your domain name**: the `example.com` part in **example**
- **Your record name**: the `myhome-dynamic-ip` part in **example**

## script usage

1. Install required packages:

    `pip install -r requirements.txt`

1. Set environment variable to `API_KEY`, `DOMAIN`, `RECORD_NAME`, it depends on the OS you're using, assume you knew it
1. run script

    `python updateNamesiloRecord.py`
