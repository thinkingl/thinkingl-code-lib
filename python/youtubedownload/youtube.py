from pytube import YouTube

proxies = {'http':'http://192.168.84.162:8118','https':'https://192.168.84.162:8118'}
yt = YouTube('https://www.youtube.com/watch?v=Dcv3U4MTTlc',proxies=proxies)

streams = yt.streams

print( streams )