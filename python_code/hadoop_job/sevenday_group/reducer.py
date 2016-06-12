#!/usr/bin/python
#-*-coding:UTF-8 -*-

import os
import sys
import string
import pymongo
import random

def main():

    	last_key = ''
	last_account_id = 0
	last_campaign_id = 0
	last_group_id = 0
	last_display_style = 0
	last_impression = 0
	last_click = 0
	last_cost = 0

	conn = pymongo.Connection(os.environ['MONGODB_HOST'], 27017)
	db = conn.adx
	db.sevenday_group.drop()

	for line in sys.stdin:
		value_str = line[:-1]
		value = value_str.split("\t")
		value_len = len(value)
		key = value[0]
		
    		account_id = 0
		try: account_id = int(value[1])
		except: account_id = 0
		
		campaign_id = 0
		try: campaign_id = int(value[2])
		except: campaign_id = 0
		
		group_id = 0
		try: group_id = int(value[3])
		except: group_id = 0
		
		
		display_style = 0
		try: display_style = int(value[4])
		except: display_style = 0
		
    		impression = 0
		try: impression = int(value[5])
		except: impression = 0
		
    		click = 0
		try: click = int(value[6])
		except: click = 0

    		cost = 0.00
		try: cost = float(value[7])
		except: click = 0.00

		if key == last_key:
			last_impression = last_impression + impression
			last_click = last_click + click
			last_cost = last_cost + cost

		else:
			if last_key:
				db.sevenday_group.insert({
					'account_id':last_account_id,
					'campaign_id':last_campaign_id,
					'group_id':last_group_id,
					'display_style':last_display_style,
					'impression':last_impression, 
					'click':last_click, 
					'cost':last_cost
					})

			last_key = key
			last_account_id = account_id
			last_campaign_id = campaign_id
			last_group_id = group_id
			last_display_style = display_style
			last_impression = impression
			last_click = click
			last_cost = cost

	db.sevenday_group.insert({
		'account_id':last_account_id,
		'campaign_id':last_campaign_id,
		'group_id':last_group_id,
		'display_style':last_display_style,
		'impression':last_impression,
		'click':last_click,
		'cost':last_cost
		})

if __name__=='__main__':
	main()


