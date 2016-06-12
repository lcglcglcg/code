<?php

if (!defined('APP_PATH'))
    exit('No direct script access allowed');

/**
单页面类
@author lucas lucasphper@gmail.com
@package admin
@access public
@Date: 14-12-24
@Time: 上午11:22
@version 1.0 2014/11/27
 */

include_once(PATH_APP . 'modules/Admin/models/Admin.php');

class PageModel extends AdminModel {

    function __construct() {
        parent::__construct();
    }

    /**
     * 获取文章列表
     * @param array $cond where条件 根据id、栏目、标题、
     * @param int $start 分页起始页
     * @param int $offset 分页结束页
     * @param String $order 排序 默认根据listorder,然后根据inputtime DESC
     * @param array $select
     * @return array
     * @author yangguofeng
     */
    public function pageList($cond, $start = 0, $offset = 20, $order = 'inputtime DESC,listorder', $select = ['id', 'catid', 'title', 'keywords', 'url', 'status', 'inputtime', 'updatetime']) {
        $where = [];
        if (isset($cond['catid'])) {
            $where['AND']['catid'] = $cond['catid'];
        }

        if (isset($cond['id'])) {
            $where['AND']['id'] = $cond['id'];
        }

        if (isset($cond['title'])) {
            $where['LIKE']['title'] = $cond['title'];
        }

        $where['LIMIT'] = [$start, $offset];
        $where['ORDER'] = $order;

        return $this->db->select('page', $select, $where);
    }

    /**
     * 获取文章条数
     * @author yangguofeng
     */
    public function count($cond) {
        $where = [];
        if (isset($cond['catid'])) {
            $where['AND']['catid'] = $cond['catid'];
        }

        if (isset($cond['id'])) {
            $where['AND']['id'] = $cond['id'];
        }

        if (isset($cond['title'])) {
            $where['LIKE']['title'] = $cond['title'];
        }


        return $this->db->count('page', $where);
    }

    /**
     * 根据id取某个页面
     * @param type $cond
     * @param type $select
     * @return type
     */
    public function getPageById($id) {
        return $this->db->get('page', [
            'id',
            'title',
            'catid',
            'description',
            'url',
            'status',
            'content',
        ], [
            'id' => $id
        ]);
    }



    /**
     * 获取单页面数
     * @param array $condition 条件参数
     * @param string $tableName 表名
     * @return int
     */
    public function get_page_count($condition, $tableName = 'page') {
        $where = array();
        if (isset($condition['catid'])) {
            $where['AND']['catid'] = $condition['catid'];
        }

        if (isset($condition['status'])) {
            $where['AND']['status'] = $condition['status'];
        }


        return $this->db->count($tableName, $where);
    }

    /**
     * 获取页面列表
     * @param int $first 起始位置
     * @param int $limit 条数
     * @param array $condition 条件参数
     * @param string $tableName 表名
     * @return array
     */
    public function get_page_list($first, $limit, $condition, $tableName = 'page') {
        $where = array();
        if (isset($condition['catid'])) {
            $where['AND']['catid'] = $condition['catid'];
        }

        if (isset($condition['status'])) {
            $where['AND']['status'] = $condition['status'];
        }


        $where['ORDER'] = 'id DESC';
        $where['LIMIT'] = [$first, $limit];
        return $this->db->select($tableName, ['id', 'catid', 'uid', 'status', 'updatetime', 'title'], $where);
    }

    /**
     * 删除单页面
     * @param int $document_id 单页面id
     * @param string $tableName 表名
     * @return bool
     */
    public function delete_page($page_id, $tableName = 'page') {

        //清redis缓存
        $this->cache->delete( array( 'article:hits' . $page_id,'category', 'article:detail' . $page_id));

        return $this->db_write->delete($tableName, array('id' => $page_id));
    }

    /**
     * 显示隐藏单页面
     * @param array $page_detail 单页面详情
     * @param string $tableName 表名
     * @return bool
     */
    public function page_display($page_detail, $tableName = 'page') {

        return $this->db_write->update($tableName, $page_detail, array('id' => $page_detail['id']));
    }

    /**
     * 单页面详情
     * @param int $page_id 单页面id
     * @return array
     */
    public function get_page_detail($page_id, $column = "*", $tableName = 'page') {
        if ($tableName == 'page') {
            $basic = $this->db->get($tableName, array('id', 'catid', 'uid', 'status', 'updatetime', 'url', 'title', 'keywords', 'description', 'listorder'), ['id' => $page_id]);
            $detail = $this->db->get('page_data', ['id', 'content'], ['id' => $page_id]);
            if ($detail) {
                return array_merge($basic, $detail);
            } else {
                return $basic;
            }
        } else {
            return $this->db->get($tableName, $column, ['id' => $page_id]);
        }
    }

    /**
     * 添加单页面
     * @param array $page_detail 单页面详情
     * @param string $tableName 表名
     * @return bool
     */
    public function page_add($data, $tableName = 'page') {
        return $this->db_write->insert($tableName, $data);
    }

    /**
     * 更新单页面
     * @param array $page_detail 单页面详情
     * @param string $tableName 表名
     * @return bool
     */
    public function page_update($page_id, $data, $tableName = 'page') {
        //清redis缓存
        $this->cache->delete( array( 'category', 'page:detail' . $page_id));
        return $this->db_write->update($tableName, $data, ['id' => $page_id]);
    }

}
