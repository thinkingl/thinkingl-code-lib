import requests
from bs4 import BeautifulSoup

urlBeginning = "http://www.javlibrary.com/tw/?v=javlijb6si";

rep = requests.get( urlBeginning )

print( rep )
