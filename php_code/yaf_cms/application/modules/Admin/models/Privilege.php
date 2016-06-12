<?php if (!defined('APP_PATH')) exit('No direct script access allowed');

/**
权限类
@author lucas lucasphper@gmail.com
@package admin
@access public
@version 1.0 2014/11/25
 */
include_once(PATH_APP.'modules/Admin/models/Admin.php');
class PrivilegeModel extends AdminModel
{
    function __construct()
    {
        parent::__construct();
    }

    /**
     * 获取所有权限
     * @param void
     * @return array
     */
    public function get_privilege_list($privilege_id = null,$column = ['privilege_id', 'privilege_name', 'privilege_description', 'menu_id', 'url', 'page_url', 'add_time'])
    {
        if(!is_null($privilege_id))
            $where['privilege_id'] = $privilege_id;
        $where['ORDER'] = 'menu_id DESC';
        return $this->db->select('privileges',$column , $where);
    }

    /**
     * 为角色添加权限
     * @param int $role_id 角色id
     * @param array $privilege_list 权限列表
     * @return bool
     */
    public function add_privilege_for_role($role_id, $privilege_list)
    {
        $temp_list = array();
        $data = array();
        foreach($privilege_list as $key => $value){
            $temp_list['role_id'] = $role_id;
            $temp_list['privilege_id'] = $value;
            $data[] = $temp_list;
            $temp_list = array();
        }
        return $this->db_write->insert("role_privilege", $data);
    }

    /**
     * 删除角色的所有权限
     * @param int $role_id 角色id
     * @return bool
     */
    public function delete_privilege_for_role($role_id)
    {
        return $this->db_write->delete("role_privilege", array('role_id' => $role_id));
    }

    /**
     * 获取角色的权限
     * @param int $role_id 角色id
     * @return array
    */
    public function get_role_privilege_list($role_id)
    {
        $where['role_id'] = $role_id;
        return $this->db->select('role_privilege', ['id', 'role_id', 'privilege_id'], $where);
    }

    /**
     * 权限详情
     * @param int $privilege_id 权限id
     * @return array
    */
    public function get_privilege_detail($privilege_id)
    {
        return $this->db->select('privileges', ['privilege_id', 'privilege_name', 'privilege_description', 'menu_id', 'url', 'page_url', 'add_time'], ['privilege_id' => $privilege_id]);
    }
}