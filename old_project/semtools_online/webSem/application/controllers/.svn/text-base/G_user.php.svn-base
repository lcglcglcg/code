<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class G_user extends G_Controller {
	function __construct() {
		parent::__construct();
		$this->load->model('reportuser_model', 'account');
	}
	public function index()
	{
		$this->data['url'] = 'user';
		$this->load->view('user/list',$this->data);
	}
	public function editaccount(){
		$data = array(
			'account'	=>	$this->input->post('account')
		,	'password'	=>	$this->input->post('password')
		,	'token'	=>	$this->input->post('token')	
		,	'date'	=>	date("Y-m-d")
		,	'status'	=>	1
		,	'code'	=>	0
		);
		$id=$this->input->post('id');
		$this->account->updateUserById($data,$id);	
		$wheres['STATUS'] = '0';	
		$errUser=$this->account->count($wheres);	
		$this->session->set_userdata(SESSION_USER_CODE, $errUser);
		echo $errUser;
	}
	public function userlist(){
		$order=$this->input->get('order');
		$sorts=$this->input->get('sorts');
		$where['STATUS'] = '1';
		echo json_encode(array(
				'list' 	=> $this->account->getlist($where,$order,$sorts)
			));
	}
	public function ckaccount(){
		$this->load->library('curl');
		$account=$this->input->post('account');
		$password=$this->input->post('password');
		$token=$this->input->post('token');
		$urls='http://127.0.0.1:8000/&2';
		$urls=$urls.'&'.strlen($account).'&'.$account;
		$urls=$urls.'&'.strlen($password).'&'.$password;
		$urls=$urls.'&'.strlen($token).'&'.$token;
		echo $code = $this->curl->simple_get($urls);
	}
}
