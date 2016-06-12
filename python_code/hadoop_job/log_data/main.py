#!/usr/bin/python
#-*-coding:UTF-8 -*-

import urlparse
import re

topHostPostfix = ( 
        '.com','.la','.io','.co','.info','.net','.org','.me','.mobi',
        '.us','.biz','.xxx','.ca','.co.jp','.com.cn','.net.cn',
        '.org.cn','.mx','.tv','.ws','.ag','.com.ag','.net.ag',
        '.org.ag','.am','.asia','.at','.be','.com.br','.net.br',
        '.bz','.com.bz','.net.bz','.cc','.com.co','.net.co',
        '.nom.co','.de','.es','.com.es','.nom.es','.org.es',
        '.eu','.fm','.fr','.gs','.in','.co.in','.firm.in','.gen.in',
        '.ind.in','.net.in','.org.in','.it','.jobs','.jp','.ms',
        '.com.mx','.nl','.nu','.co.nz','.net.nz','.org.nz',
        '.se','.tc','.tk','.tw','.com.tw','.idv.tw','.org.tw',
        '.hk','.co.uk','.me.uk','.org.uk','.vg', '.com.hk', '.cn')

regx = r'[^\.]+('+'|'.join([h.replace('.',r'\.') for h in topHostPostfix])+')$'

def m_url_parse_domain_root(url):
        if url:
		if url.find('58-JIANKANG-QIUYI.COM') != -1:
		      return 'jiankang.58.com/qiuyi'

		pattern = re.compile(regx,re.IGNORECASE)
                parts = urlparse.urlparse(url)
                host = parts.netloc
                if host == 'm.58.com' or host == 'jiankang.58.com': return host
                m = pattern.search(host)
                res =  m.group() if m else host
                return "null" if not res else res
        else: return "null"

def m_url_parse_domain_child(url):
        if url:
		if url.find('58-JIANKANG-QIUYI.COM') != -1:
		      return 'jiankang.58.com/qiuyi'
                
		ret = urlparse.urlparse(url).netloc
                if ret: return ret
                else: return "null"
        else: return "null"

def m_url_parse_domain_ipaddr(ipaddr):
        addr = ipaddr.strip().split('.')
        if len(addr) != 4: return 0
        for i in range(4):
                try:
                        addr[i]=int(addr[i])
                except:
                        return 0
                if addr[i]>255 and addr[i]<0:
                        return 0
                i = i + 1
                return -1

def m_url_parse_domain(filename, url, display_style):
        if filename.find('DSP') != -1:
                if display_style == '2':
                        return 'DSP-TEXT','DSP-TEXT'
                else:
                        return 'DSP-IMG','DSP-IMG'
        return m_url_parse_domain_root(url), m_url_parse_domain_child(url)

def main():
	print m_url_parse_domain('123', '58-jiankang-qiuyi.com/123.html', 1)

if __name__=='__main__':
	main()



