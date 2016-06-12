<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');
/**
 * log4php 日志类库
 */
class G_Logger {
	
	// /* CI超级对象 */
	// private $CI;
	
	
	/**
	 * 初始化log4php配置 
	 *
	 * @access	public
	 */
	public function __construct() {
		$CI =& get_instance();
		$CI->load->file(APPPATH.'third_party/log4php/Logger.php');
		Logger::configure(APPPATH.'config/log4php.properties');
	}
}

/* End of file Logger.php */
/* Location: ./application/libraries/Logger.php */