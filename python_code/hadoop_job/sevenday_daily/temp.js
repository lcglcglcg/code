db.runCommand({ mapreduce: "log.data",
	map: function(){
	emit({display_style:this.display_style,account_id:this.account_id,time:this.time2},{impression:this.impression,click:this.click,cost:this.cost})},
	reduce:function(key,values){
	var ret = {impression:0,click:0,cost:0};
	for(var i in values){ret.impression += values[i].impression;ret.click += values[i].click;ret.cost += values[i].cost;}
	return ret;},
	
	
	out:"account_sevenday_daily_temp"});

