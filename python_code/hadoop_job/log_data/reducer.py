#!/usr/bin/python
#-*-coding:UTF-8 -*-

import sys

def report_out(value, click, cost):
        show_id = 		value[0]
        creative_id =           value[1]
        account_id =            value[2]
        campaign_id =           value[3]
        group_id =              value[4]
        bidding =               value[5]
        bidding_model =         value[6]
        display_style =         value[7]
        ip =                    value[8]
        region_code =           value[9]
        url =                   value[10]
        visit_domain =          value[11]
        visit_domain_child =    value[12]
        refer =                 value[13]
        source_domain =         value[14]
        source_domain_child =   value[15]
        time =                  value[16][:-1]

        print chr(22).join((time,
                            account_id,
                            campaign_id,
                            group_id,
                            creative_id,
                            bidding,
                            bidding_model,
                            display_style,
                            ip,
                            region_code,
                            url,
                            visit_domain,
                            refer,
                            source_domain,
                            str(click),
                            str(cost),
                            visit_domain_child,
                            source_domain_child))

def main():
        last_key = None
        last_click_cpc = 0
        last_click_cpm = 0
        for line in sys.stdin:
                value_str = line[:-1]
                value = value_str.split(chr(22))
                value_len = len(value)
                key = value[0] + value[1]

                if value_len == 3:
                        try: charging = int(value[2])
                        except: charging = 0

                        if last_key == key:
                                if charging == 1: last_click_cpc = last_click_cpc + 1
                                last_click_cpm = last_click_cpm + 1
                        else:
                                last_key = key
                                last_click_cpc = 0
                                if charging == 1: last_click_cpc = 1
                                last_click_cpm = 1

                else:
                        try: bidding = float(value[5])
                        except: bidding = 0.00

                        try: bidding_model = int(value[6])
                        except: bidding_model = 2

                        if bidding_model == 1 and key == last_key:
                                report_out(value, last_click_cpm, bidding) #cpm on click
                        elif bidding_model == 1:
                                report_out(value, 0, bidding) #cpm no click

                        elif bidding_model == 2 and key == last_key and last_click_cpc:
                                report_out(value, last_click_cpc, bidding * last_click_cpc) #cpc on click
                        else: report_out(value, 0, 0.00) #cpc no click

if __name__=='__main__':
        main()



