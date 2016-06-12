#!/usr/bin/python
#-*-coding:UTF-8 -*-

import os 
import sys

def main():

	for line in sys.stdin:
		value_str = line[:-1]
		value = value_str.split('\t')
		value_len = len(value)
		
		time = 		value[0]
		account_id =    value[1]
		campaign_id =   value[2]
		group_id =      value[3]
		creative_id =   value[4]
		display_style = value[5]
		bidding_model = value[6]
		impression =    value[7]
		click =         value[8]
		cost =          value[9]
		
		key = account_id + '-' + campaign_id + '-' + display_style
		print '\t'.join((key, account_id, campaign_id, display_style, impression, click, cost))

if __name__=='__main__':
	main()

