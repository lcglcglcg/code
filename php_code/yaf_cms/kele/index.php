<?php
/**
 * User: kele
 * Date: 13-8-25
 * Time: 上午1:15
 * 入口文件
 * php版本只支持5.3以上
 */
$_SERVER['starttime'] = microtime(1);
// 站点根目录
define("APP_PATH", dirname(__DIR__));
//开发者环境
$servers = ['anhao'];
$config_env = 'production';
in_array(gethostname(), $servers) && $config_env = 'development';
define('ENV', $config_env);
/*实例化Bootstrap, 依次调用Bootstrap中所有_init开头的方法*/
(new Yaf_Application(APP_PATH . DIRECTORY_SEPARATOR . 'conf' . DIRECTORY_SEPARATOR . $config_env . '.ini', 'common'))->bootstrap()->run();