<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class G_main extends G_Controller {
	function __construct() {
		parent::__construct();
		$this->load->model('main_model', 'main');
	}
	public function index()
	{
		$this->data['url'] = 'main';
		$this->load->view('main/main',$this->data);
	}
	public function recreate(){
		$type=$this->input->post('type');
		$name=$this->input->post('name');
		$ftime_1=$this->input->post('ftime_1');
		$ftime_2=$this->input->post('ftime_2');
		echo json_encode(array(
				'id' 	=> $this->main->create(array(
													'type'	=> $type
												,	'path'	=> ''
												,	'yycolumn'	=> ''
												,	'yy'	=> ''
												,	'name'	=> $name
												))
			));
		
	}
	public function createreport(){
		$type=$this->input->post('type');
		$path=$this->input->post('path');
		$yycolumn=$this->input->post('yycolumn');
		$yy=$this->input->post('yy');
		$name=$this->input->post('name');
		echo json_encode(array(
				'id' 	=> $this->main->create(array(
													'type'	=> $type
												,	'path'	=> $path
												,	'yycolumn'	=> $yycolumn
												,	'yy'	=> $yy
												,	'name'	=> $name
												))
			));
		
	}
	public function reportlist(){
		$page=$this->input->get('page');
		$pagesize=$this->input->get('pagesize');
		$ids=$this->input->get('ids');
		$keyword=$this->input->get('keysearch');
		$time1=$this->input->get('time1');
		$time2=$this->input->get('time2');
		$order=$this->input->get('order');
		$sorts=$this->input->get('sorts');
		$n=0;
		$where['STATUS'] = '1';
		$where_in=array();
		if(!empty($ids)){
			$where_in['id'] = explode(',',$ids);
		}
		if(!empty($keyword)){
			$where['name like'] = '%'.$keyword.'%';
		}
		if(!empty($time1)){
			$where['createtime >'] = $time1.' 00:00:00';
			$where['createtime <'] = $time2.' 23:59:59';
		}
		if(empty($ids)){
			$n=$this->main->count($where,$where_in);
		}
		echo json_encode(array(
				'list' 	=> $this->main->getlist($page,$where,$order,$sorts,$pagesize,$where_in)
			,	'max'	=> $n
			,	'time'	=> date('Y-m-d H:i:s')
			));
	}
	public function reportdel($id){
		$this->main->delreport($id);
	}
	public function getmkxls(){
		$this->load->library('curl');
		$mkfilename=$this->input->post('path');
		$id=$this->input->post('id');
		$type=$this->input->post('type');
		$yycolumn=$this->input->post('yycolumn');
		$yy=$this->input->post('yy');
		
		$ftime_1=$this->input->post('ftime_1');
		$ftime_2=$this->input->post('ftime_2');
		
		$urls='http://127.0.0.1:8000/&';
		if(!empty($ftime_1) && !empty($ftime_2)){
			$urls=$urls.'3';
			$urls=$urls.'&'.strlen($id).'&'.$id;
			$urls=$urls.'&'.strlen($ftime_1).'&'.$ftime_1;
			$urls=$urls.'&'.strlen($ftime_2).'&'.$ftime_2;
		}else{
			$urls=$urls.'1&'.strlen($mkfilename).'&'.$mkfilename;
			$urls=$urls.'&'.strlen($id).'&'.$id;
			$urls=$urls.'&'.strlen($yycolumn).'&'.$yycolumn;
			$urls=$urls.'&'.strlen($yy).'&'.$yy;
		}
		echo $code = $this->curl->simple_get($urls);
	}
}
