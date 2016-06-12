//自动显示
function fd_Show()   
{
  document.getElementById("fd-mid").style.visibility = "";
}
var skipdivtime = 10000; 
window.setInterval("fd_Show()",skipdivtime);
//自动显示

//中间浮动
function fd_mid_hide()
{
  document.getElementById("fd-mid").style.visibility = "hidden";
}
document.write('<style>#fd-mid{pointer-events:none;left:0px;width:100%;top:100px;position:fixed;z-index:10020;}');
document.write('#fd-mid .fd-con{width:240px;height:120px;pointer-events:auto;margin:0px auto;overflow:hidden;position:relative;background:url(http://www.ncdxbk.com/swt/244/m/junhai.gif);}</style>');
document.write('<DIV id="fd-mid"><DIV class="fd-con">');
document.write('<a href="javascript:" onclick="fd_mid_hide();return false;"><div style="float:right;width:20px;height:20px;"></div></a>');
document.write('<a href="/templets/default/swt.htm" target="_blank"><div style="float:left;width:260px;height:20px"></div><div style="width:280px;height:107px"></div></a>');
document.write('</DIV></DIV>');
//中间浮动

document.write ('<div style="display:none;"><script language="javascript" type="text/javascript" src="http://js.users.51.la/17298685.js"></script></div>');