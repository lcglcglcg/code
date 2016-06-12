<?php

if (!defined('APP_PATH'))
    exit('No direct script access allowed');
/**
  用户类
  @author yangguofeng
  @access public
  @package admin
  @version 1.0 2014/11/26
 */
include_once(PATH_APP . 'modules/Admin/models/Admin.php');

class MemberModel extends AdminModel {

    function __construct() {
        parent::__construct();
    }

    /*
     * 获取前台用户列表
     * array $cond where条件 
     * int $start 分页起始页
     * int $offset 分页结束页
     * $order 排序
     * @param array $field 查询字段
     * return array
     */

    public function memberList($cond, $start = 0, $offset = 10, $order = 'regtime DESC', $field = ['id', 'username', 'nickname', 'email', 'phone', 'status', 'last_login_time']) {
        //$where['AND']['status'] = 1;
        $where['LIMIT'] = [$start, $offset];
        $where['ORDER'] = $order;
        return $this->db->select('member', $field, $where);
    }

    /**
      添加用户
      @param array
      @return int
     */
    public function save($data = array()) {
        return $this->db_write->insert('member', $data);
    }

    /**
     * 统计用户数
     * @param type $where
     * @return type
     */
    public function count($where = array()) {
        //$where['AND']['status'] = 1;
        return $this->db->count('member', $where);
    }

    /**
     * 根据$where判断member是否存在
     * @param type $where
     * @return type
     * 成功放回int
     * 失败返回false
     */
    public function has($where = array()) {
        return $this->db->get('member', 'id', $where);
    }

    /**
     * 删除用户
     * @param type $where
     * @return type
     */
    function delete($where = array()) {
        return $this->db_write->delete('member', $where);
    }

    /**
     * 跟新用户
     * @param type $value
     * @param type $where
     * @return type
     */
    function update($value, $where) {
        return $this->db_write->update('member', $value, $where);
    }

    /**
     * 根据条件查找用户信息
     * @param type $where
     * @return type
     */
    public function getMemberByAttribute($where = array()) {
        return $this->db->get('member', [
                    'id',
                    'username',
                    'name',
                    'nickname',
                    'gender',
                    'userface',
                    'phone',
                    'email',
                    'birthday',
                    'regtime',
                    'status'
                        ], $where
        );
    }

    public function getParentMenu($roleId) {
        $sql = "SELECT m.menu_id,m.menu_name
          FROM role_privilege rp
          JOIN `privileges` p
          ON rp.privilege_id = p.privilege_id
          JOIN menus m
          ON m.menu_id = p.menu_id
          WHERE rp.role_id = $roleId
          GROUP BY menu_id
          ORDER BY m.menu_order DESC";

        return $this->db->query($sql)->fetchAll(PDO::FETCH_ASSOC);
    }

    public function getChildMenu($parentId, $roleId) {
        $sql = "SELECT  menus.menu_id,menus.menu_name,temp.url,menus.parent_id
                FROM menus  
                JOIN  (SELECT p.menu_id,p.url
                                        FROM role_privilege rp
                                        JOIN `privileges` p
                                        ON rp.privilege_id = p.privilege_id
                                        WHERE rp.role_id = $roleId) temp
                ON menus.parent_id = temp.menu_id
                WHERE menus.parent_id = $parentId
                GROUP BY menus.menu_id
                ORDER BY menus.menu_order";
        return $this->db->query($sql)->fetchAll(PDO::FETCH_ASSOC);
    }

}
