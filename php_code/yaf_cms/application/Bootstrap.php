<?php

/**
 *
 * 所有在Bootstrap类中, 以_init开头的方法, 都会被Yaf调用,
 * 这些方法, 都接受一个参数:\Yaf\Dispatcher $dispatcher
 * 调用的次序, 和申明的次序相同
 * 注意:方法在Bootstrap类中的定义出现顺序, 决定了它们的被调用顺序.
 * User: kele
 * Date: 13-8-26
 * Time: 下午11:42
 */
class Bootstrap extends Yaf_Bootstrap_Abstract {

    var $superglobal = array(
        'GLOBALS'   => 1,
        '_GET'      => 1,
        '_POST'     => 1,
        '_REQUEST'  => 1,
        '_COOKIE'   => 1,
        '_SERVER'   => 1,
        '_ENV'      => 1,
        '_FILES'    => 1,
    );

    function _init_set() {
        //配置文件
        Yaf_Registry::set("config", Yaf_Application::app()->getConfig());
        //网站名字
        define('SITENAME', Yaf_Registry::get("config")->sitename);
        //网站路径
        define('SITEDIR', Yaf_Registry::get("config")->sitedir);
        
        //网址url
        define('BASEURL', Yaf_Registry::get("config")->url);
        define('PHONEURL', Yaf_Registry::get("config")->phoneurl);
        
        //图片url
        define('IMGURL', Yaf_Registry::get("config")->imgurl);
        
        //上传路径
        define('UPLOADPATH', APP_PATH . '/kele/uploadfile/');
        define('PATH_APP', Yaf_Registry::get("config")->application->directory);
        define('PATH_VIEW', PATH_APP . 'views');
        
        //专题路径和专题URL
        define('TOPIC_PATH',APP_PATH.'/kele/topic/');
        define('TOPIC_URL',IMGURL.'topic/');
        
        //判断站点类型:1老站,2新站
        define('SITE_TYPE',Yaf_Registry::get("config")->site->type->old);
        //自动分表提示:超过设置的数量则自动创建新的表，并发送email通知
        define('ARTNUM',Yaf_Registry::get("config")->site->type->tablenum);
        //判断缓存类型:redis或者memcache
        define('CACHE_TYPE',Yaf_Registry::get("config")->site->type->cache);
        //时间类型1:unix time 2:timestamp
        define('TIME_TYPE',Yaf_Registry::get("config")->site->type->timeformate);
        //Api返回链接的类型：1=>/hyzx/catpath/id.html 2:/hyzx/catpath/date/id.html
        define('URL_TYPE',Yaf_Registry::get("config")->site->type->url);
        
        //线下开启调试模式
        ENV == 'development' && define('DEBUG', 1);
        ENV == 'production' && define('DEBUG', 0);
        
        define('AUTHKEY', Yaf_Registry::get("config")->auth_key);
        //写日志目录
        define('LOG_PATH', APP_PATH . '/log/');
        //memcache前缀
        define('MEMPREFIX', Yaf_Registry::get("config")->get('memcache')->default->prefix);

        //地球半径，平均半径为6371km
        //----------------------------------> 全局设置:
        // 错误报告DEBUG
        if (DEBUG) {
            // E_ALL | E_STRICT
            error_reporting(E_ALL);
            //error_reporting(E_ALL & ~(E_NOTICE | E_STRICT));
            ini_set('display_errors', 'ON');
        } else {
            error_reporting(0);
            ini_set('display_errors', 'Off');
        }

        // 关闭运行期间的自动增加反斜线
        @set_magic_quotes_runtime(0);

        // 最低版本需求判断
        PHP_VERSION < '5.0' && exit('Required PHP version 5.0.* or later.');
        ini_set('memory_limit', '512m');
        //设置市区
        date_default_timezone_set('Asia/Shanghai');

        // 将更多有用的信息放入 $_SERVER 变量
        $_SERVER['starttime'] = microtime(1);
        $_SERVER['time'] = isset($_SERVER['REQUEST_TIME']) ? $_SERVER['REQUEST_TIME'] : time();
        $_SERVER['ip'] = isset($_SERVER['REMOTE_ADDR']) ? $_SERVER['REMOTE_ADDR'] : '';
        $_SERVER['sqls'] = [];

        //注销所有的超级变量
        foreach ($GLOBALS as $key => $value) {
            if (!isset($this->superglobal[$key])) {
                $GLOBALS[$key] = null;
                unset($GLOBALS[$key]);
            }
        }

        //禁止globals变量
        if (isset($_GET['GLOBALS']) || isset($_POST['GLOBALS']) || isset($_COOKIE['GLOBALS']) || isset($_FILES['GLOBALS'])) {
            exit();
        }
        // GPC 安全过滤，关闭，数据的正确性可能会受到影响。
        if (!get_magic_quotes_gpc()) {
            $_GET <> false && core::stripslashes($_GET);
            $_POST <> false && core::stripslashes($_POST);
            $_COOKIE <> false && core::stripslashes($_COOKIE);
            $_REQUEST <> false && core::stripslashes($_REQUEST);
        }
        //开启xss过滤
        //$_SERVER['REQUEST_METHOD'] == 'GET' && !empty($_SERVER['REQUEST_URI']) && core::_xss_check();
    }

    //基础配置文件
    function _initBase(Yaf_Dispatcher $dispatcher) {
        // 如果非命令行，则输出 header 头
        if (!core::is_cmd()) {
            header("Expires: 0");
            header("Cache-Control: private, post-check=0, pre-check=0, max-age=0");
            header("Pragma: no-cache");
            header('Content-Type: text/html; charset=UTF-8');
            header('X-Powered-By: http://www.lianchuangbrothers.com/'); // 隐藏 PHP 版本 X-Powered-By: PHP/5.5.9
            header('Server: Microsoft-IIS/11.11');
        }
        //错误处理
        $dispatcher->setErrorHandler([get_class($this), 'error_handler']);

        //添加路由协议
        $dispatcher->getRouter()->addConfig((new Yaf_Config_Ini(APP_PATH . '/conf/routes.ini'))->routes);
        //注册本地类
        Yaf_Loader::getInstance()->registerLocalNameSpace(['helper', 'misc']);
        
        // 加载数据库(master)
        Yaf_Registry::set('db_read', new medoo(Yaf_Registry::get("config")->get('database')->read->toArray()));
        // 加载数据库(slava)
        Yaf_Registry::set('db_write', new medoo(Yaf_Registry::get("config")->get('database')->write->toArray()));

        //memcache
        Yaf_Registry::set('memcache', new Memcache);
        Yaf_Registry::get('memcache')->pconnect(Yaf_Registry::get("config")->get('memcache')->default->host, Yaf_Registry::get("config")->get('memcache')->default->port, 2.5);
        //redis
        Yaf_Registry::set('redis', new redis());
        //连接redis
        Yaf_Registry::get('redis')->pconnect(Yaf_Registry::get("config")->get('redis')->default->host, Yaf_Registry::get("config")->get('redis')->default->port, 2.5);
        //redis密码
        Yaf_Registry::get('redis')->auth(Yaf_Registry::get("config")->get('redis')->default->password);
        //使用php内置的serialize/unserialize 方法对数据进行处理
        Yaf_Registry::get('redis')->setOption(Redis::OPT_SERIALIZER, Redis::SERIALIZER_PHP);
        //rediskey加前缀
        Yaf_Registry::get('redis')->setOption(Redis::OPT_PREFIX, Yaf_Registry::get("config")->get('redis')->default->prefix);

        core::ob_start();
    }

    /**
     * Custom error handler.
     *
     * Catches all errors (not exceptions) and creates an ErrorException.
     * ErrorException then can caught by Yaf\ErrorController.
     *
     * @param integer $errno the error number.
     * @param string $errstr the error message.
     * @param string $errfile the file where error occured.
     * @param integer $errline the line of the file where error occured.
     *
     * @throws ErrorException
     */
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

        // 运行时致命错误，直接退出。并且 debug_backtrace()
        $s = "[$errnostr] : $errstr in File $errfile, Line: $errline";
        // 抛出异常，记录到日志
        //echo $errstr;
        if (DEBUG && empty($_SERVER['exception'])) {
            throw new Exception($s);
        } else {
            log::write($s . date('Y-M-D H:i:s', $_SERVER['time']));
            //$s = preg_replace('# \S*[/\\\\](.+?\.php)#', ' \\1', $s);
            if (core::gpc('ajax', 'R')) {
                core::ob_clean();
                //$s = preg_replace('#[\\x80-\\xff]{2}#', '?', $s);// 替换掉 gbk， 否则 json_encode 会报错！
                // 判断错误级别，决定是否退出。

                if ($errno != E_NOTICE && $errno != E_USER_ERROR && $errno != E_USER_NOTICE && $errno != E_USER_WARNING) {
                    $s = core::json_encode(array('servererror' => $s));
                    throw new Exception($s);
                    exit;
                } else {
                    $_SERVER['notice_error'] .= $s;
                    // 继续执行。
                }
            } else {
                //echo $s;
                // 继续执行。
            }
        }
        return 0;
    }

}