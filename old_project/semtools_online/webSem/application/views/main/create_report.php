<script language="javascript" src="/static/web/js/thirdparty/swfupload.js"></script>
<script language="javascript" src="/static/web/js/thirdparty/swfupload.queue.js"></script>
<script language="javascript" src="/static/web/js/common/common_upload.js"></script>
<link rel="stylesheet" type="text/css" href="/static/sem_web_1.0/web/css/progress.css"/>
<div class="report_box" id="create_report" style="display:none;">
  <div class="report_title"><b>创建报告</b><span><input name="close_bnt" type="image" src="/static/sem_web_1.0/web/images/view_close.gif"  style="cursor:pointer;"></span></div>
<div class="report_main">
    <div class="report_cont">
      <div class="cont_lf">
        <ul>
          <li class="h20"><span class="red">*</span> 报告名称：</li>
          <li class="h20"><span class="red">*</span> 选择创建报告的方式：</li>
        </ul>
      </div>
      <div class="cont_rg">
        <ol>
          <li class="h20">
            <input name="time3" type="text" class="input_text" value="">
          </li>
          <li class="h20">
            <select name="ss2" class="select_text"  id="madeType" onchange="">
              <option value="1" selected="selected" >上传最新的对话记录</option>
              <option value="2" >使用已上传的对话记录</option>
            </select>
          </li>
        </ol>
      </div>
    </div>
    <div class="report_cont" id="d1">
      <div class="cont_lf">
        <ul>
          <li class="h20"><span class="red">*</span> 选择对话咨询系统：</li>
          <li class="h20"><span class="red">*</span> 选择对话记录文件：</li>
          <li class="h20">选择预约标识列：</li>
          <li class="h20">对话预约标识：</li>
          <li class="h16"></li>
        </ul>
      </div>
      <div class="cont_rg">
        <ol>
          <li class="h20">
            <select name="ss1" class="noselect_text" disabled=true>
              <option value="1">商务通</option>
            </select>
          </li>
          <li class="h20">
            <input name="time1" type="text" class="input_text" value="">&nbsp;
            <input name="" type="image" src="/static/sem_web_1.0/web/images/shangc.jpg"  class="input_img"/>
          </li>
          <li class="h20">
            <select name="ss" class="select_text">
              <option value="1">选择包含预约标识字符的数据列</option>
              <option value="2">访客名称</option>
              <option value="3">访客备注</option>
              <option value="4">访客分类</option>
            </select>
          </li>
          <li class="h20">
            <input name="time3" type="text" class="input_text" >
          </li>
          <li class="h16"><span class="red">(多个标识之间用英文逗号分隔)</span></li>
        </ol>
      </div>
    </div>
    <div class="report_cont" style="display:none;" id="d2" >
      <div class="cont_lf">
        <ul>
          <li class="h20"><span class="red">*</span> 选择时间范围：</li>
        </ul>
      </div>
      <div class="cont_rg">
        <ol>
          <li class="h20">
              <input name="time1" type="text" class="times" value="">
          </li>
        </ol>
      </div>
      <div class="select_times" id="show_date"><img src="/static/sem_web_1.0/web/images/time1.jpg"/></div>
      <div class="report_tishi red">提示：粉色字体，表示对话数据已上传；紫色背景，表示百度数据已下载。</div>
    </div>
  </div>
  <div class="report_btn" align="center">
    <ol>
      <li><a>确定</a><a>取消</a></li>
    </ol>
  </div>
  
</div>
<div class="progress_box" id="progress" style="display:none;">
	<div class="progress_title"><span id="progress_name"></span><a id="progress_colse">取消</a></div>
	<div class="progress_border"><div id="progress_bar" class="progress_bar" style="width:20%"></div></div>
    <span class="baifenshu" id="progress_ing">0%</span>
    <div class="upload">
    	<ul>
       		<li class="w1"><s>总大小：</s><span id="progress_size"></span></li>
        	<li class="w2"><s>已上传：</s><span id="progress_sizeing"></span></li>
        	<li class="w3"><s>速度：</s><span id="progress_kb"></span></li>
        	<li class="w4"><s>剩余时间：</s><span id="progress_time">00：00:00</span></li>
        </ul>
    </div>
</div>