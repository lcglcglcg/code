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
	last_campaign_id = ''
	last_group_id = ''
	last_creative_id = ''
	last_display_style = ''
	last_bidding_model = ''
	last_show = 1
	last_click = 0
	last_cost = 0

	mongodb_host = os.environ['MONGODB_HOST']
	conn = pymongo.Connection(mongodb_host, 27017)
	
	db = conn.adx58
	db.report_standard.remove({'_id.time':time})

	for line in sys.stdin:
		value_str = line[:-1]
		value = value_str.split(chr(22))
		value_len = len(value)
		
		key = 		value[0] + '-' + value[1] + '-' + value[2] + '-' + value[3] + '-' + value[4] + '-' + value[5]
		account_id =    value[0]
		campaign_id =   value[1]
		group_id =      value[2]
		creative_id =   value[3]
		bidding_model = value[4]
		display_style = value[5]
		click =         value[6]
		cost =          value[7]

		if key == last_key:
			last_show = int(last_show) + int(1)
			last_click = int(last_click) + int(click)
			last_cost = float(last_cost) + float(cost)

		elif last_key:
			
			print '\t'.join((time_str,
				last_account_id, 
				last_campaign_id, 
				last_group_id, 
				last_creative_id, 
				last_display_style,
				last_bidding_model,
				str(last_show),
				str(last_click), 
				str(last_cost)))

			last_display_style_number = 0
			try: last_display_style_number = int(last_display_style)
			except: last_display_style_number = 0    
	
			last_bidding_model_number = 0
			try: last_bidding_model_number = int(last_bidding_model)
			except: last_bidding_model_number = 0    

			last_show_number = 0
			try: last_show_number = int(last_show)
			except: last_show_number = 0
	
			last_click_number = 0
			try: last_click_number = int(last_click)
			except: last_click_number = 0
	
			last_cost_number = 0
			try: last_cost_number = float(last_cost)
			except: last_cost_number = 0
			
			db.report_standard.insert({
				'_id':{
					'time':time,
					'account_id':last_account_id, 
					'campaign_id':last_campaign_id, 
					'group_id':last_group_id, 
					'creative_id':last_creative_id, 
					'display_style':last_display_style_number,
					'bidding_model':last_bidding_model_number
					},
				'value':{
					'impression':last_show_number,
					'click':last_click_number, 
					'cost':last_cost_number
					}})
	
			last_key = key
			last_account_id = account_id
			last_campaign_id = campaign_id
			last_group_id = group_id
			last_creative_id = creative_id
			last_display_style = display_style
			last_bidding_model = bidding_model
			last_click = click
			last_cost = cost
			last_show = 1

		else:		
			last_key = key
			last_account_id = account_id
			last_campaign_id = campaign_id
			last_group_id = group_id
			last_creative_id = creative_id
			last_display_style = display_style
			last_bidding_model = bidding_model
			last_click = click
			last_cost = cost
			last_show = 1

	print '\t'.join((time_str,
		last_account_id, 
		last_campaign_id, 
		last_group_id, 
		last_creative_id, 
		last_display_style, 
		last_bidding_model,
		str(last_show),
		str(last_click), 
		str(last_cost)))
	
	last_display_style_number = 0
	try: last_display_style_number = int(last_display_style)
	except: last_display_style_number = 0    
	
	last_bidding_model_number = 0
	try: last_bidding_model_number = int(last_bidding_model)
	except: last_bidding_model_number = 0    

	last_show_number = 0
	try: last_show_number = int(last_show)
	except: last_show_number = 0
	
	last_click_number = 0
	try: last_click_number = int(last_click)
	except: last_click_number = 0
	
	last_cost_number = 0
	try: last_cost_number = float(last_cost)
	except: last_cost_number = 0
	
	db.report_standard.insert({
		'_id':{
			'time':time,
			'account_id':last_account_id, 
			'campaign_id':last_campaign_id, 
			'group_id':last_group_id, 
			'creative_id':last_creative_id, 
			'display_style':last_display_style_number,
			'bidding_model':last_bidding_model_number
			},
		'value':{
			'impression':last_show_number,
			'click':last_click_number, 
			'cost':last_cost_number
			}})
	
	conn.close()

if __name__=='__main__':
	main()



