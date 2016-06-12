<?php

if (!defined('APP_PATH'))
    exit('No direct script access allowed');
/**
  用户类
  @author lucas lucasphper@gmail.com
  @access public
  @package admin
  @version 1.0 2014/11/19
 */

include_once(PATH_APP . 'modules/Admin/models/Admin.php');

class UserModel extends AdminModel {

    function __construct() {
        parent::__construct();
    }

    /**
      根据用户名获取用户详情
      @param string $username 用户名
      @return array
     */
    public function get_detail_by_username($username) {
        $sql = "select * from admin where username = '{$username}' limit 1";
        return (array) $this->db->query($sql)->fetchAll(PDO::FETCH_ASSOC);
    }

    /**
     * 根据用户id来获取用户详情
     * @param int $userid 用户id
     * @return array 
     */
    public function get_detail_by_userid($userid) {
        if ($userid) {
            $sql = "SELECT * FROM admin WHERE id = '{$userid}' LIMIT 1";
            return (array) $this->db->query($sql)->fetchAll(PDO::FETCH_ASSOC);
        }
    }

    /**
     * 修改用户密码
     * @param int  $userid 用户id
     * @param string $newPassword 用户的新密码
     * @return int  
     */
    public function changePassword($userid, $newPassword) {

        return $this->db_write->update('admin', ['password' => $newPassword, 'updatedate' => $_SERVER['time']], ['id' => intval($userid)]);
    }

    /**
     * 获取密保问题列表
     * @return array 密保问题列表
     */
    public function getQuestionList() {
        return $this->db->select('questions', ['qid', 'content']);
    }

    /**
     * 获取管理员的数量
     * @return int 用户总数
     */
    public function get_count_admin() {
        return $this->db->count('admin', '*');
    }

    /**
     * 获取所有的管理员列表
     * @param int $offset 	偏移量
     * @param int $pre_page 每页数量
     * @return array 用户列表
     */
    public function get_admin_list($offset, $pre_page) {
        $sql = "SELECT id,username,createdate,classid,permissions,name,email,status,roleid,role_name FROM admin ad LEFT JOIN roles ro ON ad.roleid = ro.role_id ORDER BY createdate DESC LIMIT $offset,$pre_page";
        return (array) $this->db->query($sql)->fetchAll(PDO::FETCH_ASSOC);
    }

    /**
     * 判断用户名是否存在
     * @param string $username 用户名
     * @return boolean 用户用户名存在 返回true ,用户名不存在返回false
     */
    public function is_username_exists($username) {
        $result = $this->db->count('admin', '*', ['username' => $username]);
        if ($result > 0)
            return true;
        return false;
    }

    /**
     * 判断email地市是否存在
     * @param  string $email 邮箱地址
     * @param  boolean $option 修改邮箱时验证邮箱是否存在
     * @return boolean 如果email存在返回true;否则返回false;
     */
    public function is_email_exists($email, $option = false) {
        $result = $this->db->count('admin', '*', ['email' => $email]);

        if ($option) {
            if ($result > 1)
                return true;
            return false;
        }

        if ($result > 0)
            return true;
        return false;
    }

    /**
     * 添加新的用户
     * @param array $data 要添加的用户的信息
     * @return int 返回last_insert_id
     */
    public function insert_admin_user($data) {
        return $this->db_write->insert('admin', $data);
    }

    /**
     * 修改用户信息
     * @param array $data 要修改的用户的信息
     * @param int 	$userid 要修改的用户的id
     * @return int 返回影响的行数
     */
    public function update_admin_user($data, $userid) {
        return $this->db_write->update('admin', $data, ['id' => $userid]);
    }

    /**
     * 删除用户
     * @param int $userid 要删除的用户的id
     * @return int 返回影响的行数
     */
    public function delete_admin_user($userid) {
        return $this->db_write->delete('admin', ['id' => $userid]);
    }

    /**
     * 停用用户账户
     * @param int $userid 用户的id
     * @return int 返回影响的行数
     */
    public function disable_admin_user($userid) {
        return $this->db_write->update('admin', ['status' => 0], ['id' => $userid]);
    }

    /**
     * 启用用户账户
     * @param int $userid 用户的id
     * @return int 返回影响的行数
     */
    public function enable_admin_user($userid) {
        return $this->db_write->update('admin', ['status' => 1], ['id' => $userid]);
    }

    /**
     * 获取角色列表
     * @return array 返回角色列表
     */
    public function get_role_list() {
        return $this->db->select('roles', ['role_id', 'role_name']);
    }

    /**
     * 筛选用户
     * @param array $columns 显示字段
     * @param array $where 筛选条件
     * @return array
     */
    public function filter_user($columns, $where) {
        return $this->db->select('admin', $columns, $where);
    }

}
