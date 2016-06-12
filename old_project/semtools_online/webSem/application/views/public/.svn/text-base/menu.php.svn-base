<script>
var login_code='<?php echo $sess_user['code']; ?>';
</script>
<div class="header">
  <div class="logo">
	<div class="logo_img"><img src="/static/sem_web_1.0/web/images/logo_01_1_02.gif" ><img src="/static/sem_web_1.0/web/images/logo_02.png" class="img_word"></div>   
    <div class="welcome">
        <ul class="rg">
         <li>欢迎您,<?php echo $sess_user['loginname']; ?>,今天是<?php echo date("Y年m月d日"); ?></li>　
         <li class="w60"><a onClick="editloginpass();">修改密码</a></li> | 
         <li><a href="/logout">退出</a></li>
        </ul>
        <ul class="rg w270">
         <li class="w30"><img src="/static/sem_web_1.0/web/images/warn.gif" width="24" height="24" /></li>
         <?php if($sess_user['code']<>0){ ?>
         <li id="menu_errMsg" class="w270 red" >
         	<?php if($sess_user['code']==-2){ ?>
         	<marquee direction="left" behavior="scroll" scrollamount="10" scrolldelay="200">账户已过期！请联系服务商！</marquee>
            <?php }else if($sess_user['code']==-1){ ?>
            <marquee direction="left" behavior="scroll" scrollamount="10" scrolldelay="200">账户验证失败！请联系服务商！</marquee>
            <?php }else if($sess_user['code']==-4){ ?>
            <marquee direction="left" behavior="scroll" scrollamount="10" scrolldelay="200">未知服务器错误，稍后再试</marquee>
            <?php }else{ ?>
            <marquee direction="left" behavior="scroll" scrollamount="10" scrolldelay="200">账户已冻结！请联系服务商！</marquee>
            <?php } ?>
         </li>
        <?php }else{ ?>
         <li id="menu_errMsg" class="w270 red" style="display:<?php if($usercode>0) echo ';';else echo 'none;';?>" ><marquee direction="left" behavior="scroll" scrollamount="10" scrolldelay="200">百度账户密码无效，请及时更正。</marquee></li>　
         <?php } ?>
        </ul>
     </div>
  </div>
<div class="nav">
      <ul>
       	  <li><a <?php if($url=='main') echo 'class="dh_hover" ';?> href="/main">定制报告</a></li>
       	  <li><a <?php if($url=='user') echo 'class="dh_hover" ';?> href="/user">百度账户</a></li>
      </ul>
  </div>
</div>