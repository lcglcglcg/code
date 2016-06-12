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
	last_display_style = 0
	last_impression = 0
	last_click = 0
	last_cost = 0

	conn = pymongo.Connection(os.environ['MONGODB_HOST'], 27017)
	db = conn.adx
	db.sevenday_account.drop()

	for line in sys.stdin:
		value_str = line[:-1]
		value = value_str.split("\t")
		value_len = len(value)
		key = value[0]
	
		account_id = 0
		try: account_id = int(value[1])
		except: account_id = 0
		
		display_style = 0
		try: display_style = int(value[2])
		except: display_style = 0
		
    		impression = 0
		try: impression = int(value[3])
		except: impression = 0
		
    		click = 0
		try: click = int(value[4])
		except: click = 0

    		cost = 0.00
		try: cost = float(value[5])
		except: cost = 0.00

		if key == last_key:
			last_impression = last_impression + impression
			last_click = last_click + click
			last_cost = last_cost + cost

		else:
			if last_key:
				db.sevenday_account.insert({
					'account_id':last_account_id,
					'display_style':last_display_style,
					'impression':last_impression, 
					'click':last_click, 
					'cost':last_cost
					})

			last_key = key
			last_account_id = account_id
			last_display_style = display_style
			last_impression = impression
			last_click = click
			last_cost = cost
	
	db.sevenday_account.insert({
		'account_id':last_account_id,
		'display_style':last_display_style,
		'impression':last_impression,
		'click':last_click,
		'cost':last_cost
		})

if __name__=='__main__':
	main()


