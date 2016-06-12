<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class G_upflie extends G_Controller {
	function __construct() {
		parent::__construct();
		$this->load->helper('string');
		$this->load->helper('date');
	}
	public function index()
	{
		$datestring = "%Y_%m_%d";
		$path=mdate($datestring, time());

		$config['upload_path'] = './uploads/'.$path.'/';
		if(!is_dir($config['upload_path'])){
			mkdir($config['upload_path'], 0777); 
		}
		$config['allowed_types'] = '*';
		$config['max_size'] = '0';
		$config['file_name'] = now().'_'.random_string('alnum', 8);
		$this->load->library('upload', $config);
		
		if (!$this->upload->do_upload("Filedata"))
		{
			$error = array('error' => $this->upload->display_errors());
			echo json_encode($error);
		} 
		else
		{
			$data = array('upload_data' => $this->upload->data(),'path'=> $path.'/'.$config['file_name'].'.xls');
			echo json_encode($data);
		}
	}
	
}

/* End of file welcome.php */
/* Location: ./application/controllers/welcome.php */