<?php
	$this->load->view('public/header');
?>
<?php
	$this->load->view('public/menu');
?>
<?php
	$this->load->view('public/getjs');
?>
<link rel="stylesheet" type="text/css" href="/static/sem_web_1.0/web/css/create_report.css">
<script language="javascript" src="/static/web/js/page/main.js"></script>
<div class="ad_box">
  <div class="main">
  	<div class="curr_position">当前位置：<span>定制报告</span></div>
<!-- 查询条件conditions start -->
    <div class="conditions">
		<ol>
            <li>报告名称：</li>
            <li><input name="" type="text" class="input01"/></li>
            <li class="padl4">创建时间：</li>
            <li><input name="time" type="text" class="times" disabled="disabled"><input name="calendar" type="image" src="/static/sem_web_1.0/web/images/calendar.gif" class="calendar"></li>
            <li class="padl4 c_select"><a id="search" href="javascript:;">查询</a></li>
		</ol>
    </div>
<!-- 查询条件conditions end -->
<!-- 查询条件s_content start -->
    <div class="s_content">
    	<ul class="b_line" id="listtitlie">
            <li class="w430 f_bold">报告名称</li>
            <li class="w200 f_bold">创建时间</li>
            <li class="w200 f_bold">生成时间</li>
            <li class="w140 f_bold">操作</li>
       </ul>
       <span id="echolist">
    	
       </span>
    	<ul class="t_line">
            <li></li>
		</ul>
    </div>
     <div class="s_bottom">
      	<div class="b_lf lf">
        	<ol>
        	  <li><a id="create">创建报告</a></li>
              </ol>
        </div>
<?php
	$this->load->view('public/page');
?>
      </div>
<!-- 查询条件s_content end -->
  </div>
</div>
<?php
	$this->load->view('main/create_report');
	$this->load->view('user/change');
	$this->load->view('public/msg');
	$this->load->view('public/foot');
?>