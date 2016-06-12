<?php

if (!defined('APP_PATH'))
    exit('No direct script access allowed');

/**
  栏目频道类
  @author lucas lucasphper@gmail.com
  @package admin
  @access public
  @version 1.0 2014/11/21
 */
include_once(PATH_APP . 'modules/Admin/models/Admin.php');

class CategoryModel extends AdminModel {

    function __construct() {
        parent::__construct();
    }

    /**
     * 获取栏目列表
     * array $cond where条件 根据id、栏目、标题、
     * int $start 分页起始页
     * int $offset 分页结束页
     * $order 排序 默认根据listorder
     * return array
     * @author yangguofeng
     */
    public function categoryList($cond, $start = 0, $offset = 20, $order = 'orderid', $select = ['catid', 'catname', 'catpath']) {
        $ret = [];
        if (isset($cond['catid'])) {
            $where['AND']['catid'] = $cond['catid'];
        }

        if (isset($cond['parentid'])) {
            $where['AND']['parentid'] = $cond['parentid'];
        }

        $where['LIMIT'] = [$start, $offset];
        $where['ORDER'] = $order;

        $data = $this->db->select('category', $select, $where);

        foreach ($data as $v) {
            $ret[$v['catid']] = $v;
        }

        return $ret;
    }

    /**
      获取文章栏目
      @param int $offset 偏移量
      @param int $per_page 每页显示条数
      @return array
     */
    function get_category_list($offset, $per_page) {
        $where['ORDER'] = 'orderid DESC';
        $where['LIMIT'] = [$offset, $per_page];
        return $this->db->select("category", ['catid', 'catpath', 'catname', 'modelid', 'parentid', 'description', 'orderid', 'showtype', 'en', 'keyword'], $where);
    }

    /**
      获取文章数量
      @param void
      @return int
     */
    function get_category_count() {
        return $this->db->count('category');
    }

    /**
      添加栏目
      @param array
      @return int
     */
    function add_category($category_detail = array()) {
        return $this->db_write->insert('category', $category_detail);
    }

    /**
      删除栏目
      @param int
      @return bool
     */
    function delete_category($category_id) {
        return $this->db_write->delete('category', array('catid' => $category_id));
    }

    /**
      栏目详情
      @param int $category_id 栏目id
      @return array
     */
    function get_category_detail($category_id) {
        return $this->db->select('category', ['catid', 'catname', 'modelid', 'parentid', 'description', 'orderid', 'showtype', 'en', 'keyword'], ['catid' => $category_id]);
    }

    /**
      更新栏目
      @param array
      @return bool
     */
    function update_category($category_detail) {
        $category_id = (int) $category_detail['catid'];
        return $this->db_write->update('category', $category_detail, ['catid' => $category_id]);
    }

    /**
     * 筛选栏目
     * @param array $columns 显示字段
     * @param array $where 筛选条件
     * @return array
     */
    public function filter_category($columns, $where) {
        return $this->db->select('category', $columns, $where);
    }

}
