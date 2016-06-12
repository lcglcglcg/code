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
class G_Controller extends CI_Controller {	
	
	/**
	 * 为view封装数据 
	 */
	protected $data 		= array();
	
	/**
	 * session 用户 
	 */
	protected $sess_user	= array(); 

	/**
	 *---------------------------------
	 * 构造函数
	 *---------------------------------
	 *
	 * @access	public
	 */
	function __construct() {
		parent::__construct();
		$this->sess_user = self::get_session_user();
		$this->data['sess_user'] = $this->sess_user;
		$this->data['usercode'] =self::get_session_user_code();
	}
	
	// --------------------------------------------------------------------
	
	/**
	 *---------------------------------
	 * 判断是否属于post请求
	 *---------------------------------
	 *
	 * @access	protected
	 */
	 function is_post() {
	 	if($_SERVER['REQUEST_METHOD']=='POST'){
	 		return TRUE;
	 	}else {
	 		return FALSE;
	 	}	
	}
	
	// --------------------------------------------------------------------
	
	/**
	 *---------------------------------
	 * 判断是否属于ajax请求
	 *---------------------------------
	 *
	 * @access	protected
	 */
	 function is_ajax() {
		if(isset($_SERVER['HTTP_X_REQUESTED_WITH']) && strtolower($_SERVER['HTTP_X_REQUESTED_WITH']) == 'xmlhttprequest'){
			return TRUE;
		}else{
			return FALSE;
		}
	}
	
	// --------------------------------------------------------------------	
	
	/**
	 *---------------------------------
	 * 将post数组赋给data数组, 确保页面表单名称和数据库字段名一致.
	 *---------------------------------
	 */
	 function model_drive(){
		return $_POST ;
	}
	
	// --------------------------------------------------------------------	
	
	/**
	 *---------------------------------
	 * 获取Session中的用户信息
	 *---------------------------------
	 *
	 * @access	protected
	 */
	 function get_session_user(){
		return $this->session->userdata(SESSION_USER_OBJ);
	}
	function get_session_user_code(){
		return $this->session->userdata(SESSION_USER_CODE);
	}
	
}
/* End of file PF_Controller.php */
/* Location: ./core/PF_Controller.php */
