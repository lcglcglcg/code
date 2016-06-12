<?php
class UserHook {
	
	var $CI;			/* CI超级对象 */
	
	// --------------------------------------------------------------------
	
	/**
	 * 构造函数	 初始化CI超级对象
	 * 
	 * @access	public
	 */
	function __construct() {
		$this->CI =& get_instance();
	}

	// --------------------------------------------------------------------
	
	/**
	 * 用户session判断
	 *
	 * @access	public
	 */
	public function checkUserSession() {
			$method = $this->CI->uri->rsegment(1);
			$user = $this->CI->session->userdata(SESSION_USER_OBJ);
			if(empty($user)){
				$exclude_methods = $this->CI->config->item('no_user_sess_methods');
				if(!in_array($method, $exclude_methods)){
					redirect('/login','location');
				}
			}else {
				$include_methods = $this->CI->config->item('sess_user_no_call');
				if(in_array($method, $include_methods)){
					redirect('/main','location');
				}
			}
	}
	
	// --------------------------------------------------------------------
	
}

/* End of file user_hook.php */
/* Location: ./hooks/autoload.php */