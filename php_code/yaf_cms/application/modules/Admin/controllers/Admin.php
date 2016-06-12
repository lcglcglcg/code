<?php

if (!defined('APP_PATH'))
    exit('No direct script access allowed');
/**
 * 基础公共类
 * @author lucas lucasphper@gmail.com
 * @access public
 * @package core
 * @version 1.0 2014/11/19
 */

include_once(PATH_APP . 'modules/Admin/models/Menu.php');
include_once(PATH_APP . 'modules/Admin/models/Privilege.php');
Yaf_Loader::import(PATH_APP . '../library/Vcode.php');
Yaf_Loader::import(PATH_APP . 'library/Alert.php');

class AdminController extends Yaf_Controller_Abstract {

    //应用配置
    protected $config;
    //redis信息
    protected $redis;
    //memcache信息
    protected $memcache;
    //用户信息
    protected $_user = array(
        'uid' => 0,
        'username' => ''
    );

    /**
     * 初始化
     * @param void
     * @return void
     */
    protected function init() {
        //获取配置信息
        $this->config = Yaf_Application::app()->getConfig();

        //缓存信息
        //$this->redis = Yaf_Registry::get('redis');
        //$this->memcache = Yaf_Registry::get('memcache');
        $this->cache = CACHE_TYPE == 'redis' ? Yaf_Registry::get('redis') : Yaf_Registry::get('memcache');

        //cookie信息
        $this->cookie = array(
            'cookie_pre' => Yaf_Registry::get('config')->get('cookie')->pre,
            'cookie_path' => Yaf_Registry::get('config')->get('cookie')->path,
            'cookie_domain' => Yaf_Registry::get('config')->get('cookie')->domain
        );

        //用户模型初始化
        $this->db_user = new UserModel();
        $this->db_menu = new MenuModel();
        $this->db_privilege = new PrivilegeModel();


        //检查终端cookie
        if (isset($_COOKIE[$this->cookie['cookie_pre'] . 'user_auth']) && $_COOKIE[$this->cookie['cookie_pre'] . 'user_auth']) {
            $user_auth_cleartext = core::authcode($_COOKIE[$this->cookie['cookie_pre'] . 'user_auth'], 'DECODE');
            list($this->_user['uid'], $this->_user['username'], $this->_user['roleid']) = explode('\t', $user_auth_cleartext);
        }

        //路由地址
        $request_info = Yaf_Controller_Abstract::getRequest();
        $request_info = get_object_vars($request_info);
        //登陆状态
        if (intval($this->_user['uid'])) {

            if (strtolower($request_info['module']) == 'admin' && strtolower($request_info['controller']) == 'user' && strtolower($request_info['action']) == 'login') {
                Yaf_Controller_Abstract::redirect("/admin/index/index/");
            }
            $this->getView()->assign('user_profile', $this->_user);

            //权限控制
            if ($this->_user['roleid'] == 1) {
                //超级管理员，权限无限制
            } else {
                session_start();

                if (!(isset($_SESSION['cms']['privilege_allowed_url']) && count($_SESSION['cms']['privilege_allowed_url']))) {
                    //获取授权的url
                    $privilege_list = (array) $this->db_privilege->get_role_privilege_list($this->_user['roleid']);

                    //权限map
                    $privilege_id_list = array();
                    foreach ($privilege_list as $key => $value) {
                        $privilege_id_list[] = $value['privilege_id'];
                    }
                    $privilege_map_list_result = $this->db_privilege->get_privilege_detail($privilege_id_list);
                    $privilege_map_list = array();
                    foreach ($privilege_map_list_result as $key => $value) {
                        $privilege_map_list[$value['privilege_id']]['url'] = $value['url'];
                        $privilege_map_list[$value['privilege_id']]['page_url'] = $value['page_url'];
                    }

                    //格式化url列表
                    $allowed_url = array();
                    $allowed_page = array();
                    foreach ($privilege_list as $key => $value) {
                        $allowed_url[] = $privilege_map_list[$value['privilege_id']]['url'];
                        if ($privilege_map_list[$value['privilege_id']]['page_url']) {
                            $allowed_page[] = $privilege_map_list[$value['privilege_id']]['page_url'];
                        }
                    }
                    $allowed_url[] = "/admin/index/index";
                    $this->_user['privilege_list'] = $_SESSION['cms']['privilege_allowed_url'] = $allowed_url;
                    $this->_user['privilege_page_list'] = $_SESSION['cms']['privilege_allowed_page'] = $allowed_page;
                } else {
                    $this->_user['privilege_page_list'] = $_SESSION['cms']['privilege_allowed_page'];
                    $this->_user['privilege_list'] = $_SESSION['cms']['privilege_allowed_url'];
                }

                //不检查的动作列表
                $privilege_not_check_list = array('/admin/user/deallogout');


                //不检查的页面列表
                $page_not_check_list = array('/admin/index/index');

                //检查授权
                $current_url = '/' . strtolower($request_info['module']) . '/' . strtolower($request_info['controller']) . '/' . strtolower($request_info['action']);
                if (in_array($current_url, $privilege_not_check_list) or in_array($current_url, $page_not_check_list)) {
                    //过滤不检查的动作 和 页面
                } else {

                    if (!in_array($current_url, array_merge($this->_user['privilege_list'], $this->_user['privilege_page_list']))) {
                        $referer = $_SERVER['HTTP_REFERER'];
                        echo "<script>alert('权限不足，操作失败，若操作，请联系管理员');location.href='{$referer}'</script>";
                        exit;
                    }
                }
            }
        } else {
            //访问控制
            $allowed_controller = array('user');
            $allowed_action = array('login', 'deallogin', 'logout', 'code', 'forgetpassword', 'dealforgetpassword', 'resetpassword', 'getcode');

            if (!(in_array(strtolower($request_info['controller']), $allowed_controller) && in_array(strtolower($request_info['action']), $allowed_action))) {

                //redirect到登陆页
                Yaf_Controller_Abstract::redirect('/admin/user/login');
            }
        }
        
        //菜单信息
        $menu_list = array();
        $top_menu = array();
        $menu_list = $this->db_menu->get_menu_list();

        //格式化菜单
        $menu_format = array();
        foreach ($menu_list as $key => $value) {
            if (!$value['parent_id']) {
                $top_menu[] = $value;
            }
        }

        foreach ($top_menu as $key => $value) {
            $parent_id = $value['menu_id'];
            $tmp_arr1 = array();
            foreach ($menu_list as $k => $v) {
                if ($v['parent_id'] == $parent_id && in_array($v['url'], array_merge($this->_user['privilege_list'], $this->_user['privilege_page_list'])) || ($v['parent_id'] == $parent_id && $this->_user['roleid'] == 1)) {
                    $tmp_arr1[] = $v;
                    unset($menu_list[$k]);
                }
            }
//            a 2   2   5var_dump(array_merge($this->_user['privilege_list'], $this->_user['privilege_page_list']));
            $tmp_arr2['top'] = $value;
            $tmp_arr2['child'] = $tmp_arr1;
            if ($this->_user['roleid'] != 1 && count($tmp_arr2['child']) == 0) {
                unset($tmp_arr2);
                continue;
            }
            $menu_format[] = $tmp_arr2;
        }
        $this->getView()->assign('menu_list', $menu_format);
    }

    /**
     * 递归生成栏目书
     * @param array $category 栏目列表
     * @param int $pid 父节点id
     * @param int $level 层级数
     * @param string $html 填充物
     */
    protected function treeForCategory(&$category, $pid = 0, $level = 0, $html = '&nbsp;&nbsp;-|&nbsp;') {
        static $tree = array();
        foreach ($category as $k => $v) {
            if ($v['parentid'] == $pid) {
                $v['orderid'] = $level;
                $v['html'] = str_repeat($html, $level);
                $tree[] = $v;
                $this->treeForCategory($category, $v['catid'], $level + 1);
            }
        }
        return $tree;
    }

    /**
     * 判断当前用户是否是管理员
     */
    protected function is_admin() {
        if ($this->_user['roleid'] == 1) {
            return true;
        }else{
            return false;
        }
    }

    /**
     * 根据文章id  返回改id所在的 表id;
     * @param int $id 文章的id
     * @return int 返回表的id
     */
    public function get_table_id($id,$type = 'news'){

        $data = false;

        $redisKey = 'tableinfo';

        $data = $this->cache->get($redisKey);

        if($data == false){

            $data = $this->db->select('table',['table_id','start_id','end_id'],['id[>]'=>0]);

            $this->cache->set($redisKey, $data, 3600 * 12);

        }
        

        $table_id = 0;

        foreach($data as $k=>$v){
            if($id >= $v['start_id'] && $id <= $v['end_id']){
                $table_id = $v['table_id'];
            }
        }

        // 需要判断该id是否存在于news_xx表中,如果不存在，则在news表中查找，news_0表不存在。
        if( $table_id && !$this->db->get('news_'.$table_id,'catid',['id'=>$id]) ){
            $table_id = 0;
        }

        return $table_id == 0 ? $type : $type.'_'.$table_id;
    }

    /**
     * 根据文章时间  返回改该时间所在的 表id;
     * @param int $starttime 开始时间
     * @param int $endtime 结束时间
     * @return int 返回表的id
     */
    public function get_table_id_by_time($starttime,$endtime,$type = 'news'){

        $data = false;

        $redisKey = 'tableinfobytime';

        $data = $this->cache->get($redisKey);

        if($data == false){

            $data = $this->db->select('table',['table_id','start_id','end_id','starttime','endtime'],['id[>]'=>0]);

            $this->cache->set($redisKey, $data, 3600 * 12);

        }
        


        $table_id = 0;
        foreach($data as $k=>$v){

            if($starttime >= $v['starttime'] && $endtime <= $v['endtime']){
                
                $table_id = $v['table_id'];
                break;
            }
        }

        return $table_id == 0 ? $type : $type.'_'.$table_id;
    }

}
