<?php
	$this->load->view('public/header');
?>
<?php
	$this->load->view('public/menu');
?>
<?php
	$this->load->view('public/getjs');
?>
<link rel="stylesheet" type="text/css" href="/static/sem_web_1.0/web/css/account_alter.css">
<script language="javascript" src="/static/web/js/page/userlist.js"></script>
<div class="ad_box">
  <div class="main">
    <div class="curr_position">当前位置：<span>百度账户</span></div>
    <!-- 查询条件s_content start -->
    <div class="s_content" >
      <ul class="b_line">
            <li class="w220 f_bold">百度APIToken</li>
            <li class="w200 f_bold">百度用户名</li>
            <li class="w200 f_bold">密码</li>
            <li class="w110 f_bold">最终验证时间</li>
            <li class="w150 f_bold">状态</li>
            <li class="w100 f_bold">操作</li>
       </ul>
      <span id="echolist">
      </span>
      <ul class="t_line">
        <li></li>
      </ul>
    </div>
    <!-- 查询条件s_content end -->
    <div class="kong_bottom"> </div>
  </div>
</div>
<?php
	$this->load->view('user/list_change');
	$this->load->view('public/msg');
	$this->load->view('user/change');
	$this->load->view('public/foot');
?>