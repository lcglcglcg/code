<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class G_mkreport extends CI_Controller {
	function __construct() {
		parent::__construct();
		$this->load->model('main_model', 'main');
	}
	public function index(){
		
	}
	public function mkreport($id,$type,$downloadflie='',$errmsg=''){
		$this->main->upreport($id,$type,$downloadflie,$errmsg);
	}
}
