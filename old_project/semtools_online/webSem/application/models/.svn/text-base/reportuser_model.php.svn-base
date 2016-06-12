<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');
/**
 * www.pinfun.com 
 *
 * 基于Codeigniter的
 * 
 * @package		PINFUN
 * @author		Ghostdom.wj <ghostdom.wj@gmail.com>
 * @copyright	Copyright (c) 20011 - 2012, pinfun.com.
 * @link		http://www.pinfun.com
 * @version		0.1.0
 */

// --------------------------------------------------------------------

/**
 * PinFun User Model
 *
 * PinFun 用户模型
 *
 * @package		PINFUN
 * @subpackage	Model
 * @category	Model
 * @author		Ghostdom.wj <ghostdom.wj@gmail.com>
 * @link		http://www.pinfun.com
 */
class Reportuser_Model extends G_Model {
	
	/* 用户表 */
	const TABLE_NAME = 'report_user';		
	/**
	 *---------------------------------
	 * 构造函数
	 *---------------------------------
	 *
	 * @access	public
	 */
	function __construct()  {
		parent:: __construct();
	}
	/**
	 *---------------------------------
	 * 根据条件查询用户数据
	 *---------------------------------
	 * 
	 * @param where 	查询条件
	 */
	public function getUser($where){
		$query = $this->db->get_where(self::TABLE_NAME, $where);
		$user = parent::getRowWithArray($query);
		return $user;
	}
	
	public function getlist($where,$ordre,$sorts){
		$query_param = array(
			ORDER_TYPE 	=> $sorts,
			ORDER_BY	=> $ordre
		);
		$query = self::getData(self::TABLE_NAME, $query_param);
		return $query->result_array();
	}
	public function count($where){
		$query_param = array(
			WHERE 		=> $where
		);
		return self::getCount(self::TABLE_NAME, $query_param);
	}
	public function updateUser($data, $where){
		return parent::modify(self::TABLE_NAME, $data, $where);
	}
	// --------------------------------------------------------------------	
	
	/**
	 *---------------------------------
	 * 根据用户id修改用户信息
	 *---------------------------------
	 *
	 * @param $user		所要修改的用户内容
	 * @param $id		用户编号
	 * @return			影响的行数
	 */
	public function updateUserById($data, $id = 0){
		$where['id'] = $id;
		return self::updateUser($data, $where);
	}
	// --------------------------------------------------------------------	
	
	/**
	 *---------------------------------
	 * 更新用户最后一次登陆信息
	 *---------------------------------
	 *
	 * @param id	用户编号
	 */
	public function updateLastLogin($id){
		$this->load->helper('http');
		$update = array(
			'user_lastlogin_time'	=> time(),
			'user_lastlogin_ip'		=> getIpAddr()
		);
		self::updateUserById($update,$id);
	}	
	
}