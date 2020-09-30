import requests
import time

t = int(time.time())*1000

url = 'https://mpay.ximalaya.com/mobile/track/pay/167446426/' + str(t) + '?device=pc&isBackend=true&_=' + str(t)
url = 'http://mpay.ximalaya.com/mobile/track/pay/167446426/1601451871056?device=pc&isBackend=true&_=1601451871056'
cookies = {}
cookies['1&_token'] = '06864530140N8EEBE829879AE9A64838044E69BF0652BAEF52DD33B6B65AF1436A6AF583B3CA139M0C7DF47949C1A0C_'
cookies['_token'] = '06864530140N8EEBE829879AE9A64838044E69BF0652BAEF52DD33B6B65AF1436A6AF583B3CA139M0C7DF47949C1A0C_'

cookies['1_l_flag']='45738789&06864530140N8EEBE829879AE9A64838044E69BF0652BAEF52DD33B6B65AF1436A6AF583B3CA139M0C7DF47949C1A0C__2020-09-3015:42:22'
cookies['s&e'] = '6e665fe66df66e9766314869eb6f5c01'
cookies['device_id'] = 'xm_1586433331088_k8spg0i8x6yo1l'
cookies['_xmLog'] = 'h5&3519c4f4-a666-49a2-b5b4-feec0e0fc7f1&2.1.2'
cookies['fds_otp'] = '8713797323340702960'
cookies['s&a'] = '@%0EUY[%09%1EY@%0F%05YT%06BX@QXRZ[AZ%1F%0DU%09[%04KRV^CR^OMUVZVU^OBTA'
cookies['trackType'] = 'web'
cookies['1&remember_me'] = 'y'
cookies['x_xmly_traffic'] = 'utm_source%253A%2526utm_medium%253A%2526utm_campaign%253A%2526utm_content%253A%2526utm_term%253A%2526utm_from%253A'
r = requests.get(url, cookies=cookies)
print( r.text )
