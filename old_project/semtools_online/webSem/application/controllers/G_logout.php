<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class G_logout extends G_Controller {
	function __construct() {
		parent::__construct();
	}
	public function index()
	{
		$this->session->sess_destroy();
		redirect('/login');
	}
}