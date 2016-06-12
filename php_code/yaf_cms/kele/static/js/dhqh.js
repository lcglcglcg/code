// JavaScript Document
$(function(){
	var index22=0;
	var start1;
	$("#slider").hover(function(){
		clearInterval(start1);
	},function(){
		start1=setInterval(function(){
			index22++;
			show1(index22);
			if(index22>1){index22=0;show1(index22);}
		},5000)
	}).mouseout();
	$(".prev,.next").hover(function(){
		clearInterval(start1);
	},function(){
		$("#slider").mouseout();
	})
	$(".prev").click(function(){
		index22++;
		show1(index22);
		if(index22>1){index22=0;show1(index22);}
	})
	$(".next").click(function(){
		index22--;
		show1(index22);
		if(index22<0){index22=1;show1(index22);}
	})
	
	$("#zk img").click(function(){
		$("#zk .content").toggleClass("none");
	})
})
function show1(index22){
	if(index22==1){
		$(".btn222").addClass("xuant").siblings(".btn111").removeClass("xuant")
	}else{
		$(".btn111").addClass("xuant").siblings(".btn222").removeClass("xuant")
	}
	$("#slider").find(".mm").eq(index22).show().siblings(".mm").hide();
}
