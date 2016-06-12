#!/usr/bin/python
#-*-coding:UTF-8 -*-

import os 
import sys

def main():
        for line in sys.stdin:
                value_str = line[:-1]
                value = value_str.split(chr(22))
                value_len = len(value)
                time = 			value[0]
                account_id =            value[1]
                campaign_id =           value[2]
                group_id =              value[3]
                creative_id =           value[4]
                bidding =               value[5]
                bidding_model =         value[6]
                display_style =         value[7]
                ip =                    value[8]
                region_code =           value[9]
                url =                   value[10]
                visit_domain =          value[11]
                refer =                 value[12]
                source_domain =         value[13]
                click =         	value[14]
                cost =         		value[15]
                visit_domain_child =    value[16]
                source_domain_child =   value[17]
                if visit_domain == '58.com':
                        print chr(22).join((account_id, visit_domain, click, cost))

if __name__=='__main__':
        main()


