<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class G_editlogin extends G_Controller {
	function __construct() {
		parent::__construct();
		$this->load->model('user_model', 'user');
	}
	public function index()
	{
		$pass=$this->input->post('pass');
		$newpass=$this->input->post('newpass');
		$where['STATUS'] = '1';
		$where['password'] = md5($pass);
		$data=array(
			'password'	=>	md5($newpass)
		,	'lasttime'	=>	time()
		);
		echo $this->user->updateUserById($data,$where);
	}
}