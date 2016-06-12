#python

import urllib
import urllib2

for i in range(1, 1000):
	ret = urllib2.urlopen("http://www.baidu.com")
	print ret.read()

