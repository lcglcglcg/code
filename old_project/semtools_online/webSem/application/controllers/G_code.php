<?php
Class G_code extends G_Controller{

function __construct()
{
	parent::__construct();
	$this->load->library('Authcode');
}
public function show_captcha(){    //此方法用于显示验证码图片，归一个view中的img的src调用
	$this->authcode->show();        
}

}
?>