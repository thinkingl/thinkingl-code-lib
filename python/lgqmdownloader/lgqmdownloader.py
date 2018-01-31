import export as export
import requests

#export HTTP_PROXY="http://127.0.0.1::808"
#export HTTPS_PROXY="https://127.0.0.1::808"

proxies = {
  "http": "socks5://127.0.0.1:1080",
  "https": "socks5://127.0.0.1:1080",
}

session = requests.session()

session.proxies = {'http': 'socks5://127.0.0.1:1080',
                   'https': 'socks5://127.0.0.1:1080'}



r = requests.get('http://m.23wx.cm/0/767/201981.html',  proxies=proxies)

print(r.url)
