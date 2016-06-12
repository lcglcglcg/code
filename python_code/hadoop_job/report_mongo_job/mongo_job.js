
//标准报告
db.runCommand({ mapreduce: "report_standard_temp",
	map: function(){
	emit({display_style:this.display_style,bidding_model:this.bidding_model,account_id:this.account_id,campaign_id:this.campaign_id,group_id:this.group_id,creative_id:this.creative_id,time:ISODate(this.time)},{impression:this.impression,click:this.click,cost:this.cost});},
	reduce:function(key,values){return values},
	out:{merge:"report_standard"}});

//媒体报告
db.runCommand({ mapreduce: "report_media_temp",
	map: function(){
	emit({account_id:this.account_id,visit_domain:this.visit_domain,time:ISODate(this.time)},{impression:this.impression,click:this.click,cost:this.cost});},
	reduce:function(key,values){return values},
	out:{merge:"report_media"}});

//地域报告
db.runCommand({ mapreduce: "report_region_temp",
	map: function(){
	emit({display_style:this.display_style,bidding_model:this.bidding_model,account_id:this.account_id,region_code:this.region_code,time:ISODate(this.time)},{impression:this.impression,click:this.click,cost:this.cost});},
	reduce:function(key,values){return values},
	out:{merge:"report_region"}});

db.report_standard_temp.drop();
db.report_media_temp.drop();
db.report_region_temp.drop();
