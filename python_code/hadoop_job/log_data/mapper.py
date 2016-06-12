#!/usr/bin/python
#-*-coding:UTF-8 -*-

import os
import sys
import datetime
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

def region_code_parse(setting_region, url_region, ip_region):
        if url_region == 'null': return ip_region
        setting_region_list = setting_region.split(',')
        for str in setting_region_list:
                if str == url_region:
                        return url_region
        for str in setting_region_list:
                if str == ip_region:
                        return ip_region
        return url_region

def main():
        filepath = os.environ['mapreduce_map_input_file']
        filename = os.path.split(filepath)[-1]
        for line in sys.stdin:
                value_str = line[:-1]
                value = value_str.split(chr(22))
                value_len = len(value)
                if value_len == 11:
                        log_time = 	value[0]
                        creative_id = 	value[1]
                        url = 		value[2]
                        show_id = 	value[3]
                        click_id = 	value[4]
                        cookie = 	value[5]
                        ip = 		value[6]
                        user_agent = 	value[7]
                        time = 		value[8]
                        state = 	value[9]
                        charging = 	value[10]
                        print chr(22).join((show_id, creative_id, charging))
                elif value_len >= 27:
                        log_time = 		value[0]
                        place_id = 		value[1]
                        creative_id = 		value[2]
                        group_id = 		value[3]
                        campaign_id = 		value[4]
                        account_id = 		value[5]
                        bidding = 		value[6]
                        target = 		value[7]
                        setting_region = 	value[8]
                        url = 			value[9]
                        refer = 		value[10]
                        keyword = 		value[11]
                        show_id = 		value[12]
                        cookie = 		value[13]
                        ip = 			value[14]
                        ip_region = 		value[15]
                        url_region = 		value[16]
                        department = 		value[17]
                        user_agent = 		value[18]
                        time = 			value[19]
                        state = 		value[20]
                        style = 		value[21]
                        title = 		value[22]
                        mk = 			value[23]
                        display_style = 	value[24]
                        bidding_model = 	value[25]
                        device = 		value[26]

                        visit_domain, visit_domain_child = m_url_parse_domain(filename, url, display_style)
                        source_domain, source_domain_child = m_url_parse_domain(filename, refer, display_style)
                        region_code = region_code_parse(setting_region, url_region, ip_region)

                        print chr(22).join((
                                show_id,#-->key 
                                creative_id,#-->key
                                account_id,
                                campaign_id,
                                group_id,
                                bidding,
                                bidding_model,
                                display_style,
                                ip,
                                region_code,
                                url,
                                visit_domain,
                                visit_domain_child,
                                refer,
                                source_domain,
                                source_domain_child,
                                time))

if __name__=='__main__':
        main()


