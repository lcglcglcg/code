
<?php

include 'medoo.php';
date_default_timezone_set('PRC');

class Bootstrap extends Yaf_Bootstrap_Abstract
{

    function _init_Config(Yaf_Dispatcher $dispatcher) {

	$include_path = ini_get('include_path');
	echo 'include_path='.$include_path.'<br>';

	echo "Bootstrap _init_Config... <br>\n";

	//配置文件
	Yaf_Registry::set("config", Yaf_Application::app()->getConfig());
	echo 'url='.Yaf_Registry::get("config")->url.'<br>';
	echo 'redis-host='.Yaf_Registry::get("config")->get('redis')->host.'<br>';
	echo 'redis-port='.Yaf_Registry::get("config")->get('redis')->port.'<br>';

	//错误处理
	ini_set('error_log', APP_PATH.'/log/'.date('Y-m-d').'.log');
	$dispatcher->setErrorHandler([get_class($this), 'error_handler']);
    }

    function _init_Route(Yaf_Dispatcher $dispatcher) {

	echo "Bootstrap _init_Route... <br>\n";

	//添加路由协议
	$dispatcher->getRouter()->addConfig((new Yaf_Config_Ini(APP_PATH . '/conf/routes.ini'))->routes);
	$routes = Yaf_Dispatcher::getInstance()->getRouter()->getRoutes();
	print_r($routes);
	echo '<br>';
    }

    static function error_handler($errno, $errstr, $errfile, $errline) {

	// 防止死循环
	$errortype = array(
		E_ERROR => 'Error',
		E_WARNING => 'Warning',
		E_PARSE => 'Parsing Error', # uncatchable
		E_NOTICE => 'Notice',
		E_CORE_ERROR => 'Core Error', # uncatchable
		E_CORE_WARNING => 'Core Warning', # uncatchable
		E_COMPILE_ERROR => 'Compile Error', # uncatchable
		E_COMPILE_WARNING => 'Compile Warning', # uncatchable
		E_USER_ERROR => 'User Error',
		E_USER_WARNING => 'User Warning',
		E_USER_NOTICE => 'User Notice',
		E_STRICT => 'Runtime Notice',
		//E_RECOVERABLE_ERRROR => 'Catchable Fatal Error'
		);

	$errnostr = isset($errortype[$errno]) ? $errortype[$errno] : 'Unknonw';
	$s = "[$errnostr] : $errstr in File $errfile, Line: $errline";

	if (DEBUG && empty($_SERVER['exception'])) {
	    throw new Exception($s);
	} else {

	    log::write($s . date('Y-M-D H:i:s', $_SERVER['time']));
	    if (core::gpc('ajax', 'R')) {
		core::ob_clean();

		if ($errno != E_NOTICE && $errno != E_USER_ERROR && $errno != E_USER_NOTICE && $errno != E_USER_WARNING) {
		    $s = core::json_encode(array('servererror' => $s));
		    throw new Exception($s);
		    exit;
		} else {
		    $_SERVER['notice_error'] .= $s;
		}
	    } else {
	    }
	}

	return 0;
    }
}


