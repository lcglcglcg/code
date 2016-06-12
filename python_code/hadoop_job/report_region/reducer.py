#!/usr/bin/python
#-*-coding:UTF-8 -*-

import os
import sys
import string
import pymongo
import random
import datetime

def main():

        last_key = ''
        last_account_id = 0
        last_region_code = 0
        last_display_style = 0
        last_bidding_model = 0
        last_show = 1
        last_click = 0
        last_cost = 0

        time = datetime.datetime.strptime(os.environ['JOB_DATE'], '%Y-%m-%d')
        conn = pymongo.Connection(os.environ['MONGODB_HOST'], 27017)
        db = conn.adx
        db.report_region.remove({"time":time})

        for line in sys.stdin:
                value_str = line[:-1]
                value = value_str.split(chr(22))
                value_len = len(value)
                key = value[0] + '-' + value[1] + '-' + value[2] + '-' + value[3]

                try: account_id = int(value[0])
                except: account_id = 0

                try: region_code = int(value[1])
                except: region_code = 0

                try: bidding_model = int(value[2])
                except: bidding_model = 0

                try: display_style = int(value[3])
                except: display_style = 0

                try: click = int(value[4])
                except: click = 0

                try: cost = float(value[5])
                except: cost = 0.00

                if key == last_key:
                        last_show = last_show + 1
                        last_click = last_click + click
                        last_cost = last_cost + cost

                else:
                        if last_key:
                                print '\t'.join((
                                        str(last_account_id),
                                        str(last_region_code),
                                        str(last_display_style),
                                        str(last_bidding_model),
                                        str(last_show),
                                        str(last_click),
                                        str(last_cost)))

                                db.report_region.insert({
                                        'time':time,
                                        'account_id':last_account_id, 
                                        'region_code':last_region_code,
                                        'display_style':last_display_style, 
                                        'bidding_model':last_bidding_model,
                                        'impression':last_show,
                                        'click':last_click, 
                                        'cost':last_cost
                                })

                        last_key = key
                        last_account_id = account_id
                        last_region_code = region_code
                        last_display_style = display_style
                        last_bidding_model = bidding_model
                        last_click = click
                        last_cost = cost
                        last_show = 1

        print '\t'.join((
                str(last_account_id),
                str(last_region_code),
                str(last_display_style),
                str(last_bidding_model),
                str(last_show),
                str(last_click),
                str(last_cost)))

        db.report_region.insert({
                'time':time,
                'account_id':last_account_id, 
                'region_code':last_region_code,
                'display_style':last_display_style, 
                'bidding_model':last_bidding_model,
                'impression':last_show,
                'click':last_click, 
                'cost':last_cost
        })

if __name__=='__main__':
        main()



