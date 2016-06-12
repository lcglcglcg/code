<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');
class Main_Model extends G_Model {
	
	/* 报告表 */
	const TABLE_NAME = 'report';		

	function __construct()  {
		parent:: __construct();
	}
	public function create($sql) {
			$time = date('Y-m-d H:i:s');
			$ip = getIpAddr();
			$sql['createtime']	= $time;
			$sql['createip']	= $ip;
			//$sql['mktime']		= $time;
			$sql['STATUS']		= 1;
		return $this->add(self::TABLE_NAME, $sql);
	}
	public function getlist($page=1,$where,$ordre,$sorts,$pagesize,$where_in){
		$query_param = array(
			WHERE_IN	=> $where_in,
			WHERE 		=> $where,
			ORDER_TYPE 	=> $sorts,
			ORDER_BY	=> $ordre
		);
		$query_param[PAGE] = $page;
		$query_param[PAGE_NUM] = $pagesize;
		$query = self::getData(self::TABLE_NAME, $query_param);
		return $query->result_array();
	}
	public function upreport($id,$type,$downloadflie,$errmsg){
		$sql = "UPDATE report SET mktime=?,mktype=?,downloadflie=?,errmsg=? where id=?"; 
		return $this->db->query($sql, array(date('Y-m-d H:i:s'),$type,$downloadflie,$errmsg,$id)); 
	}
	public function delreport($where){
		$sql = "UPDATE report SET deltime=?,STATUS=0 where id=?"; 
		return $this->db->query($sql, array(date('Y-m-d H:i:s'),$where)); 
	}
	public function count($where,$where_in){
		$query_param = array(
			WHERE 		=> $where
		,	WHERE_IN	=> $where_in
		);
		return self::getCount(self::TABLE_NAME, $query_param);
	}
}