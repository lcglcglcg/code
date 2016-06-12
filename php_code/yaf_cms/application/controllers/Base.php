<?php

if (!defined('APP_PATH'))
    exit('No direct script access allowed');

/**
 * controller基础类
 * User: kele
 * Date: 13-8-26
 * Time: 下午11:42
 */
class BaseController extends Yaf_Controller_Abstract {

    /**
     * 所有配置信息.
     * @var Yaf_Config_Ini
     */
    protected $config;
    protected $option;
    //传递给模板的数据
    protected $data = [];
    //是否是手机端
    protected $mobile;
    //全局user
    protected $_user = ['uid' => 0, 'username' => ''];

    function init() {
        // Assign application config file to this controller
        $this->config = Yaf_Application::app()->getConfig();
        //基础model
        $this->base = new BaseModel();
        $this->mobile = misc::checkmobile();
        $this->data['category'] = $this->base->category();
        $this->data['topicsubclass'] = $this->base->topic_subclass();
        //选中状态
        $this->getView()->assign('hover', '');
        //导航
        $this->getView()->assign('category', $this->data['category']);
        $this->getView()->assign('topicsubclass', $this->data['topicsubclass']);
        $this->_initFrontConfig();
    }

    protected function _initFrontConfig() {
        header('Content-type:text/html;charset=utf-8');
        $this->option = $this->base->get_option_info();
        //检查PC网站是否关闭
        if ($this->option['webstatus'] == 'off') {
            die('网站已经关闭');
        }

        //检查手机站点是否关闭
        if ($this->option['wapstatus'] == 'off' && $this->mobile) {
           die('手机网站已经关闭');
        }

        //获取网站配置信息
        $config = array();
        $config['sitename'] = $this->option['sitename'];
        $config['title']    = $this->option['title'];
        $config['keywords'] = $this->option['keywords'];
        $config['description'] = $this->option['description'];
        $config['footer_tel'] = $this->option['footer_tel'];
        $config['footer_info'] = $this->option['footer_info'];
        
        //文章页和内容页文章调用规则
        $this->option['listpage'] = json_decode($this->option['listpage'],true);
        $this->option['contentpage'] = json_decode($this->option['contentpage'],true);

        //获取时间偏差（用于老站搬迁时的时间差）
        $config['timeoffset'] =  $this->option['timeoffset'] ? (int)$this->option['timeoffset'] : 0 ;
        $this->getView()->assign('config', $config);
    }

    function message($msg = '', $status = 0, $data = []) {
        header('Content-type:application/json;charset=utf-8');
        exit(json_encode(['status' => intval($status), 'msg' => $msg, 'data' => $data], JSON_UNESCAPED_UNICODE));
    }

    function form_submit() {
        return misc::form_submit(AUTHKEY);
    }

    // 检查是否登录
    function check_login($type = '') {
        if ($this->_user['uid'] <= 0) {
            if ($type == 'json') {
                return -10;
            }
            echo '<script> alert("对不起，您还没有登陆，请您前往登陆页面");window.location.href="/user/login"</script>';
            exit;
        } else {
            return 10;
        }
    }

    //检测用户发帖权限
    protected function check_forbidden_group($user) {
        if ($user['groupid'] == 7) {
            $this->message('对不起，您的账户被禁止发帖。');
        }
    }

    protected function check_user_exists($user) {
        if (empty($user)) {
            $this->message('用户不存在！可能已经被删除。');
        }
    }

}
