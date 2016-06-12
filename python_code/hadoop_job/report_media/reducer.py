#!/usr/bin/python
#-*-coding:UTF-8 -*-

import os
import sys
import string
import datetime
import pymongo
import random

def report_out(db, time,
               last_account_id,
               last_visit_domain,
               last_show,
               last_click,
               last_cost):
        if last_visit_domain:
                db.domain_list.insert({'domain':last_visit_domain})
                db.report_media.insert({
                        'time':time,
                        'account_id':last_account_id,
                        'visit_domain':last_visit_domain,
                        'impression':last_show,
                        'click':last_click, 
                        'cost':last_cost})
                print chr(22).join((
                        str(last_account_id),
                        last_visit_domain,
                        str(last_show),
                        str(last_click),
                        str(last_cost)))

def main():
        last_key = None
        last_account_id = 0
        last_visit_domain = None
        last_show = 0
        last_click = 0
        last_cost = 0.00
        time = datetime.datetime.strptime(os.environ['JOB_DATE'], '%Y-%m-%d')
        conn = pymongo.Connection(os.environ['MONGODB_HOST'], 27017)
        db = conn.adx
        db.report_media.remove({'time':time})
        for line in sys.stdin:
                value_str = line[:-1]
                value = value_str.split(chr(22))
                value_len = len(value)
                key = value[0] + value[1]

                try: account_id = int(value[0])
                except: account_id = 0

                visit_domain = value[1]

                try: click = int(value[2])
                except: click = 0

                try: cost = float(value[3])
                except: cost = 0.00

                if last_key == key:
                        last_show = last_show + 1
                        last_click = last_click + click
                        last_cost = last_cost + cost
                else:
                        if last_key: report_out(db, time,
                                                last_account_id,
                                                last_visit_domain,
                                                last_show,
                                                last_click,
                                                last_cost)
                        last_key = key
                        last_account_id = account_id
                        last_visit_domain = visit_domain
                        last_click = click
                        last_cost = cost
                        last_show = 1

        if last_key: report_out(db, time,
                                last_account_id,
                                last_visit_domain,
                                last_show,
                                last_click,
                                last_cost)

if __name__=='__main__':
        main()


