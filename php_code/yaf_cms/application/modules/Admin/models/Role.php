<?php if (!defined('APP_PATH')) exit('No direct script access allowed');

/**
角色类
@author lucas lucasphper@gmail.com
@package admin
@access public
@version 1.0 2014/11/25
 */
include_once(PATH_APP.'modules/Admin/models/Admin.php');
class RoleModel extends AdminModel
{
    function __construct()
    {
        parent::__construct();
    }

   /**
    * 获取所有角色
    * @param void
    * @return array
   */
   public function get_role_list()
   {
       $where['ORDER'] = 'role_id DESC';
       return $this->db->select('roles', ['role_id', 'role_name', 'role_description', 'add_time', 'is_forbid'], $where);
   }

    /**
     * 添加角色
     * @param array
     * @return int
    */
    public function add_role($role_detail = array())
    {
        return $this->db_write->insert('roles', $role_detail);
    }

    /**
     * 删除角色
     * @param $role_id 角色ID
     * @return bool
    */
    public function delete_role($role_id)
    {
        return $this->db_write->delete("roles", array('role_id' => $role_id));
    }

    /**
     * 获取角色详情
     * @param int $role_id 角色ID
     * @return array
    */
    public function get_role_detail($role_id)
    {
        return $this->db->get('roles', ['role_id', 'role_name', 'role_description', 'is_forbid'], ['role_id' => $role_id]);
    }

    /**
     * 修改角色
     * @param array
     * @return bool
    */
    public function modify_role($role_detail = array())
    {
        return $this->db_write->update('roles', $role_detail, array('role_id' => $role_detail['role_id']));
    }
}