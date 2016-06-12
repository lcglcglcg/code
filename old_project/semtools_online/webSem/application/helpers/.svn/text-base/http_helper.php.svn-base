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
 * PinFun Helpers
 *
 * PinFun http辅助方法
 *
 * @package		PINFUN
 * @subpackage	Helper
 * @category	Helper
 * @author		Ghostdom.wj <ghostdom.wj@gmail.com>
 * @link		http://www.pinfun.com
 */

// --------------------------------------------------------------------

/**
 *---------------------------------
 * 获取客户ip地址 支持使用代理服务器
 *---------------------------------
 *
 * @return ip	客户端ip
 */
function getIpAddr(){
    if (!empty($_SERVER['HTTP_CLIENT_IP'])){
        $ip=$_SERVER['HTTP_CLIENT_IP'];
    }elseif (!empty($_SERVER['HTTP_X_FORWARDED_FOR'])){
        $ip=$_SERVER['HTTP_X_FORWARDED_FOR'];
    }else{
        $ip=$_SERVER['REMOTE_ADDR'];
    }
    return $ip;
}

// --------------------------------------------------------------------	
	
/**
 *---------------------------------
 * 得到给予的URL的域名
 *---------------------------------
 *
 * @param url  链接
 * @return void
 */
function get_domain($url){
	if(empty($url) || $url == "http://") return "";
	$raw_url = parse_url($url);
	preg_match ('@^(?:http://)?([^/]+)@i', $raw_url['host'], $domain_only); 
	return $domain_only[1];
}
	
	

// --------------------------------------------------------------------

/** 
 * 计算绝对路径 
 * @param 当前路径 $baseurl 
 * @param 相对路径 $srcurl 
 */  
function rel2abs($srcurl, $baseurl) {     
	$srcinfo = parse_url($srcurl);   
	if(isset($srcinfo['scheme'])) {   
		return $srcurl;   
	}   
	$baseinfo = parse_url($baseurl);   
	$url = $baseinfo['scheme'].'://'.$baseinfo['host'];   
	if(substr($srcinfo['path'], 0, 1) == '/') {   
		$path = $srcinfo['path'];   
	}else{
		// $baseinfo['path'] . '1'
		// 是因为假如有这样的链接 http://open.t.qq.com/apps/follow/ 以斜杠为结尾，
		// 那dirname()只能取到/apps/ 所以加一个无意义的字符串 1
		$path = dirname($baseinfo['path'] . '1').'/'.$srcinfo['path'];   
	}   
	$rst = array();   
	$path_array = explode('/', $path);   
	if(!$path_array[0]) {   
		$rst[] = '';   
	}   
	foreach ($path_array AS $key => $dir) {   
		if ($dir == '..') {   
			if (end($rst) == '..') {   
				$rst[] = '..';   
			}elseif(!array_pop($rst)) {   
				$rst[] = '..';   
			}   
			}elseif($dir && $dir != '.') {   
				$rst[] = $dir;   
			}   
		}   
		if(!end($path_array)) {   
		$rst[] = '';   
	}   
	$url .= implode('/', $rst);   
	return str_replace('\\', '/', $url);  
}  



















/* End of file http_helper.php */
/* Location: ./helpers/http_helper.php */
