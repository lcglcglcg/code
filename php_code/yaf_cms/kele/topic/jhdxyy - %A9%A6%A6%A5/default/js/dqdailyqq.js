if(typeof document.compatMode!='undefined'&&document.compatMode!='BackCompat'){
    cot_t1_DOCtp="_top:expression(document.documentElement.scrollTop+document.documentElement.clientHeight-this.clientHeight-35);_left:expression(document.documentElement.scrollLeft + document.documentElement.clientWidth - offsetWidth-470);}";
}else{
    cot_t1_DOCtp="_top:expression(document.body.scrollTop+document.body.clientHeight-this.clientHeight-180);_left:expression(document.body.scrollLeft + document.body.clientWidth - offsetWidth);}";
}

if(typeof document.compatMode!='undefined'&&document.compatMode!='BackCompat'){
    cot_t2_DOCtp="_top:expression(document.documentElement.scrollTop+document.documentElement.clientHeight-this.clientHeight-35);_left:expression(document.documentElement.scrollLeft + document.documentElement.clientWidth - offsetWidth);}";
}else{
    cot_t2_DOCtp="_top:expression(document.body.scrollTop+document.body.clientHeight-this.clientHeight-35);_left:expression(document.body.scrollLeft + document.body.clientWidth - offsetWidth);}";
}
 
 function hidDiv(obj){
   var fldiv = $Obj(obj);
   if(!fldiv) return false;
   if(fldiv.style.display!="none"){
    fldiv.style.display="none";
    return true;
   }else{
    return false;
   }
 }
 function showDiv(obj){
   var fldiv = $Obj(obj);
   if(!fldiv) return false;
   if(fldiv.style.display=="none"){
    fldiv.style.display="";
    return true;
   }else{
    return false;
   }
 }

var from="";
if(document.getElementById("jh_swt_ny")){
	var src=document.getElementById("jh_swt_ny").src;
	var tt=src.split("?");
	for(var i=0;i<tt.length;i++){
		if(tt[i].indexOf("e=")>=0){
		from=tt[i].substr(2);	
		}
	}
}

function hidright(){
   document.getElementById("floatrightdiv").style.display="none";
   _runs=false;
   return false;
 }

function $Obj(objname)
{
return document.getElementById(objname);
}
 
function openurl(){
  var tstt=window.open("");
}






function hidd(){
    hidDiv('piaofubody');
	setTimeout("showd()",60000);
   _runs=false;
   return false;
 }
function showd(){
	showDiv('piaofubody');
	}

function bjbookmark(){
	var isHmark = parseInt(getCookie('homemark'));
	if(isHmark<2){
    setCookie('homemark',isHmark+1,1);
		bookmark();
	} 
	else{
		return '';
	}
}

function getCookie(c_name)
{
if (document.cookie.length>0)
  {
  c_start=document.cookie.indexOf(c_name + "=")
  if (c_start!=-1)
    { 
    c_start=c_start + c_name.length+1 
    c_end=document.cookie.indexOf(";",c_start)
    if (c_end==-1) c_end=document.cookie.length
    return unescape(document.cookie.substring(c_start,c_end))
    } 
  }
return ""
}

function setCookie(c_name,value,expiredays)
{
var exdate=new Date()
exdate.setDate(exdate.getDate()+expiredays)
document.cookie=c_name+ "=" +escape(value)+
((expiredays==null) ? "" : ";expires="+exdate.toGMTString())
}

var from="";
if(document.getElementById("jh_swt_mty")){
	var src=document.getElementById("jh_swt_mty").src;
	var tt=src.split("?");
	for(var i=0;i<tt.length;i++){
		if(tt[i].indexOf("e=")>=0){
		from=tt[i].substr(2);	
		}
	}
}



document.writeln("<div id=\"Yuye_lta\" style=\"width:226px;height:106px;right:2px; margin-right:2px;z-index: 10000;\" ><img src=\"http://cmn.08lu.com/style/junhai/junhaiqq.jpg\"  border=\"0\" usemap=\"#winMap\" id=\"win\" style=\'position:relative;border:0; z-index:100;\'><map name=\"winMap\" id=\"winMap\"> <area shape=\"rect\" coords=\"177,3,215,21\"  target=\"_blank\" href=\"http://bft.jhdxyy.com/LR/Chatpre.aspx?id=PHX53894226&e=xinmeiti-dqdaily-qq&r=xinmeiti-dqdaily-qq&p=xinmeiti-dqdaily-qq\" \/> <area shape=\"rect\" coords=\"4,31,210,115\" target=\"_blank\"  href=\"http://bft.jhdxyy.com/LR/Chatpre.aspx?id=PHX53894226&e=xinmeiti-dqdaily-qq&r=xinmeiti-dqdaily-qq&p=xinmeiti-dqdaily-qq\" \/><\/map><\/div>");

function BFloatClose()              
{
 var popup = document.getElementById("Yuye_lta").style.display='none'; 
}



function scrollx(p){	var d = document,dd = d.documentElement,db = d.body,w = window,o = d.getElementById(p.id),ie6 = /msie 6/i.test(navigator.userAgent),style,timer;
	if(o){
		o.style.cssText +=";position:"+(p.f&&!ie6?'fixed':'absolute')+";"+(p.t!=undefined?'top:'+p.t+'px':'bottom:0');

		if(p.f&&ie6){
			o.style.cssText +=';left:expression(documentElement.scrollLeft + '+(p.l==undefined?dd.clientWidth-o.offsetWidth:p.l)+' + "px");top:expression(documentElement.scrollTop +'+(p.t==undefined?dd.clientHeight-o.offsetHeight:p.t)+'+ "px" );';
			dd.style.cssText +=';background-image: url(about:blank);background-attachment:fixed;';
		}else{
			if(!p.f){
				w.onresize = w.onscroll = function(){
					clearInterval(timer);
					timer = setInterval(function(){
					//双选择为了修复chrome 下xhtml解析时dd.scrollTop为 0
					var st = (dd.scrollTop||db.scrollTop),c;
					c = st - o.offsetTop + (p.t!=undefined?p.t:(w.innerHeight||dd.clientHeight)-o.offsetHeight);
						if(c!=0){
						o.style.top = o.offsetTop + Math.ceil(Math.abs(c)/10)*(c<0?-1:1) + 'px';
						}else{
						clearInterval(timer);
						}
					},10)
				}
			}
		}
	}
}

scrollx({id:'Yuye_lta',f:1})

window.setInterval(zd, 3000); 
zd();
function zd(u){ 
	var a=['top','left'],b=0; 
	u=setInterval(function(){ 
		document.getElementById('win').style[a[b%2]]=(b++)%4<2?0:4 + 'px'; 
		if(b>15){clearInterval(u);b=0} 
	},32) 
}
// JavaScript Document