<link rel="stylesheet" type="text/css" href="/static/sem_web_1.0/web/css/change_pwd.css">
<div class="pwd_box" id="change_pwd" style="display:none;">
  <div class="pwd_title"><b>修改密码</b><span><input onclick="colse_editloginpass();" type="image" src="/static/sem_web_1.0/web/images/view_close.gif" style="cursor:pointer;"></span></div>
  <div class="pwd_main">
  	<div class="pwd_main_lf">
    	<ul>
        	<li><span class="red">*</span>旧密码：</li>
        	<li><span class="red">*</span>新密码：</li>
        	<li><span class="red">*</span>确认密码：</li>
        </ul></div>
  	<div class="pwd_main_rg">
    	<ol>
        	<li><input name="" type="password"></li>
         	<li><input name="" type="password"></li>
        	<li><input name="" type="password"></li>
       </ol>
    </div>
  </div>
  <div class="pwd_dtn">
  	<ol>
    	<li><a onclick="sub_editloginpass();">保存</a><a onclick="colse_editloginpass();" >取消</a></li>
    </ol>
  </div>
</div>