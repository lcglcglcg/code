<?php

if (!defined('APP_PATH'))
    exit('No direct script access allowed');

/**
  后台默认类
  @author lucas lucasphper@gmail.com
  @access public
  @package admin
  @version 1.0 2014/11/19
 */
include_once(PATH_APP . 'modules/Admin/controllers/Admin.php');
include_once(PATH_APP . 'modules/Admin/models/Role.php');
include_once(PATH_APP . 'modules/Admin/models/User.php');
include_once(PATH_APP . 'modules/Admin/models/Document.php');

class IndexController extends AdminController {

    function init() {
        parent::init();
        $this->db_role = new RoleModel();
        $this->db_user = new UserModel();
        $this->db_privilege = new PrivilegeModel();
        $this->db_document = new DocumentModel();
    }

    function indexAction() {

        //获取管理员个人信息
        $user_profile = $this->db_user->get_detail_by_userid($this->_user['uid']);
        $this->getView()->assign('user_profile', $user_profile[0]);

        //橘色详情
        $role_detail = $this->db_role->get_role_detail($this->_user['roleid']);
        $this->getView()->assign('role_detail', $role_detail);

        //获取文章总量
        $news_count = $this->db_document->count([]);
        $this->getView()->assign('news_count', $news_count);

        //获取当天文章发布的总量
        $start_time = strtotime(date('Y-m-d') . ' 00:00:00');
        $end_time = strtotime(date('Y-m-d') . ' 23:59:59');
        $cond['inputtime[>]'] = $start_time;
        $cond['inputtime[<]'] = $end_time;
        $day_news_count = $this->db_document->count($cond);
        $this->getView()->assign("day_news_count", $day_news_count);
        //当前登录管理员权限
        if ($this->_user['roleid'] != 1) {
            $privilege_list = (array) $this->db_privilege->get_role_privilege_list($this->_user['roleid']);
            $this->getView()->assign('privilege_list', $privilege_list);

            //权限map
            $privilege_id_list = array();
            foreach ($privilege_list as $key => $value) {
                $privilege_id_list[] = $value['privilege_id'];
            }

            $result = (array) $this->db_privilege->get_privilege_detail($privilege_id_list);
            $privilege_map = array();
            foreach ($result as $key => $value) {
                $privilege_map[$value['privilege_id']] = $value['privilege_name'];
            }
            $this->getView()->assign('privilege_map', $privilege_map);
            $this->getView()->assign('privilege_foribd', true);
        } else {
            $this->getView()->assign('privilege_foribd', false);
        }
    }
    
}
