<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<title>登陆界面</title>
<link rel="shortcut icon" href="/static/favicon.ico" type=image/x-icon>
<link rel="stylesheet" type="text/css" href="/static/sem_web_1.0/web/css/login.css">
<?php
	$this->load->view('public/getjs');
?>
<script language="javascript" src="/static/web/js/page/login.js"></script>
</head>
<body>
<div class="l_box">
	<ul>
    	<li><span>登录名：</span><input tabindex="1" name="name" type="text" value=""></li>
        <li style="display:none;" name="msg"></li>
    	<li><span>密　码：</span><input tabindex="2" name="password" type="password" value=""></li>
        <li style="display:none;" name="msg"></li>
    	<li><span>验证码：</span><input tabindex="3" name="verification-code" type="text" class="v_code"><b>
        <img src="<?php echo base_url('/code/show_captcha/'.time());?>"></b></li>
        <li style="display:none;" name="msg"></li>
    </ul>
    <div class="l_btn" align="center"><input tabindex="4" name="" type="button"/></div>
</div>
</body>
</html>