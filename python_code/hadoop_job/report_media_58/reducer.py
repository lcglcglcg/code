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
	time_str = os.environ['JOB_DATE']
	time = datetime.datetime.strptime(time_str, '%Y-%m-%d')
	last_account_id = ''
	last_visit_domain = ''
	last_show = 0
	last_click = 0
	last_cost = 0

	conn = pymongo.Connection(os.environ['MONGODB_HOST'], 27017)
   	db = conn.adx58
	db.report_media.remove({'_id.time':time})

	for line in sys.stdin:
		value_str = line[:-1]
		value = value_str.split(chr(22))
		value_len = len(value)
		key =		value[0] + value[1] 
		account_id =    value[0]
		visit_domain = 	value[1]
		click =         value[2]
		cost =          value[3]

		if key == last_key:
			last_show = int(last_show) + int(1)
			last_click = int(last_click) + int(click)
			last_cost = float(last_cost) + float(cost)

		elif last_key:
    			print '\t'.join((time_str,
				last_account_id,
				last_visit_domain,
				str(last_show),
				str(last_click), 
				str(last_cost)))
			
			last_show_number = 0
			try: last_show_number = int(last_show)
			except: last_show_number = 0
	
			last_click_number = 0
			try: last_click_number = int(last_click)
			except: last_click_number = 0
	
			last_cost_number = 0
			try: last_cost_number = float(last_cost)
			except: last_cost_number = 0
			
			db.report_media.insert({
				'_id':{
					'time':time,
					'account_id':last_account_id,
					'visit_domain':last_visit_domain
					},
				'value':{
					'impression':last_show_number,
					'click':last_click_number, 
					'cost':last_cost_number
					}})
			
			last_key = key
			last_account_id = account_id
			last_visit_domain = visit_domain
			last_click = click
			last_cost = cost
			last_show = 1

		else:			
			last_key = key
			last_account_id = account_id
			last_visit_domain = visit_domain
			last_click = click
			last_cost = cost
			last_show = 1
    			
	print '\t'.join((time_str,
		last_account_id,
		last_visit_domain,
		str(last_show),
		str(last_click), 
		str(last_cost)))
			
	last_show_number = 0
	try: last_show_number = int(last_show)
	except: last_show_number = 0
	
	last_click_number = 0
	try: last_click_number = int(last_click)
	except: last_click_number = 0
	
	last_cost_number = 0
	try: last_cost_number = float(last_cost)
	except: last_cost_number = 0
			
	db.report_media.insert({
		'_id':{
			'time':time,
			'account_id':last_account_id,
			'visit_domain':last_visit_domain
			},
		'value':{
			'impression':last_show_number,
			'click':last_click_number, 
			'cost':last_cost_number
			}})

if __name__=='__main__':
	main()


