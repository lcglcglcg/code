<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class G_login extends G_Controller {
	function __construct() {
		parent::__construct();
		$this->load->model('user_model', 'user');
	}
	public function index()
	{
		$this->load->view('user/login');
	}
	function getUser(){
		$users = $this->user->getUser(array(
			'loginname'	=> $this->input->post('username')
		,	'password'	=> md5($this->input->post('password'))
		,	'status'	=> 1
		));
		if($users)
		{
			foreach($users as $user)
			{
				return 0;
			}
		}
		else
		{
			return 2;
		}
	}
	public function ck()
	{
		if($_POST){
			$Err_code=0;
			$this->load->library('Authcode');
			if ($this->authcode->check($this->input->post('ckcode'))) {
				$Err_code=$this->getUser();
			} else {
				$Err_code=1;
			}
			echo json_encode(array(
				'err' 	=> $Err_code
			));
		}else{
			$this->index();
		}
	}
}