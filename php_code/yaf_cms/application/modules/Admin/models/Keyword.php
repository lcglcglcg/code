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

class KeywordModel extends AdminModel {

    function __construct() {
        parent::__construct();
    }

    /**
     * 获取内链关键词列表
     * @param array $cond where条件 根据id、栏目、标题、
     * @param int $start 分页起始页
     * @param int $offset 分页结束页
     * @param String $order 排序 默认根据listorder,然后根据inputtime DESC
     * @param array $select
     * @return array
     * @author yangguofeng
     */
    public function get_content_keywords_List() {

        return $this->db->select('content_keywords', '*');
    }

    /**
     *获取文章关键词列表
     */
    public function get_keywords_List() {

        return $this->db->select('keywords', '*');
    }

    /**
     * 获取关键词条数
     * @author yangguofeng
     */
    public function count($cond,$tableName = 'content_keywords') {
        $where = [];

        if (isset($cond['keywords'])) {
            $where['LIKE']['keywords'] = $cond['keywords'];
        }


        return $this->db->count($tableName, $where);
    }

    /**
     * 根据id取文章关键词
     * @param type $cond
     * @param type $select
     * @return type
     */
    public function get_article_Keywords($id) {
        return $this->get('keywords', [
            'id',
            'description',
            'keywords',
            'replace_words',
        ], [
            'id' => $id
        ]);
    }

    /**
     * 根据id取内链关键词
     * @param int $id 内链关键词id
     * @return array
     */
    public function get_contentwords($id) {
        return $this->db->get('content_keywords', [
            'id',
            'description',
            'keywords',
            'href_url',
        ], [
            'id' => $id
        ]);
    }



    /**
     * 获取关键词数
     * @param array $condition 条件参数
     * @param string $tableName 表名
     * @return int
     */
    public function get_keywords_count($condition, $tableName = 'content_keywords') {
        $where = array();
        if (isset($condition['keywords'])) {
            $where['AND']['keywords'] = $condition['keywords'];
        }

        return $this->count($tableName, $where);
    }

    /**
     * 获取关键词列表
     * @param int $first 起始位置
     * @param int $limit 条数
     * @param array $condition 条件参数
     * @param string $tableName 表名
     * @return array
     */
    public function search_keywords_list( $condition, $first = 0, $limit = 100) {
        $where = array();
        if (isset($condition['keywords'])) {
            $where['LIKE']['keywords'] = $condition['keywords'];

        }

        $where['ORDER'] = 'id DESC';
        $where['LIMIT'] = [$first, $limit];

        return $this->db->select('content_keywords', ['id', 'description', 'keywords', 'href_url'], $where);

    }

    /**
     * 获取替换关键词列表
     * @param int $first 起始位置
     * @param int $limit 条数
     * @param array $condition 条件参数
     * @param string $tableName 表名
     * @return array
     */
    public function search_replacewords_list( $condition, $first = 0, $limit = 100) {
        $where = [];
        if (isset($condition['keywords'])) {
            $where['LIKE']['keywords'] = $condition['keywords'];

        }

        $where['ORDER'] = 'id DESC';
        $where['LIMIT'] = [$first, $limit];

        return $this->db->select('keywords', ['id', 'description', 'keywords', 'replace_words'], $where);

    }

    /**
     * 删除关键词
     * @param int $keywords_id 关键词id
     * @param string $tableName 表名
     * @return bool
     */
    public function delete_keywords($keywords_id, $tableName = 'content_keywords') {

        //清memcache缓存??

        return $this->delete($tableName, ['id' => $keywords_id]);
    }

    /**
     * 删除替换词内容
     * @param int $keywords_id 关键词id
     * @param string $tableName 表名
     * @return bool
     */
    public function delete_replace_words($keywords_id, $tableName = 'keywords') {

        //清memcache缓存??

        return $this->delete($tableName, ['id' => $keywords_id]);
    }


    /**
     * 关键词页面详情
     * @param int $keywords_id 关键词id
     * @param array $column 字段名
     * @return array
     */
    public function get_page_detail($keywords_id, $column = "*", $tableName = 'content_keywords') {

            return $this->get($tableName, $column, ['id' => $keywords_id]);

    }

    /**
     * 添加关键词
     * @param array $data 关键词内链
     * @param string $tableName 表名
     * @return bool
     */
    public function content_keywords_add($data, $tableName = 'content_keywords') {
        //取出内链关键词表数据
        $words = $this->get_content_keywords_List();
        //遍历表中是否已有该关键词
        foreach($words as $v){
            //若存在，则返回false
            if($data['keywords'] == $v['keywords']){
                return false;
            }
        }
        //不存在则插入数据表中
        return $this->insert($tableName, $data);
    }

    /**
     * 添加文章替换词
     * @param array $data 替换关键词
     * @param string $tableName 表名
     * @return bool
     */
    public function replace_keywords_add($data, $tableName = 'keywords') {
        //取出关键词表数据
        $words = $this->get_keywords_List();
        //遍历表中是否已有该关键词
        foreach($words as $v){
        //若存在，则返回false
        if($data['keywords'] == $v['keywords']){
            return false;
        }
        }
        //不存在则插入数据表中
        return $this->insert($tableName, $data);
    }

    /**
     * 更新关键词页面
     * @param int $keywords_id 关键词id
     * @param array $data 关键词内容
     * @param string $tableName 表名
     * @return bool
     */
    public function content_keywords_update($keywords_id, $data, $tableName = 'content_keywords') {
        //清memcache缓存??

        return $this->db_write->update($tableName, $data, ['id' => $keywords_id]);
    }

    /**
     * 更新替换词页面
     * @param int $keywords_id 关键词id
     * @param array $data 关键词内容
     * @param string $tableName 表名
     * @return bool
     */
    public function replace_keywords_update($keywords_id, $data, $tableName = 'keywords') {
        //清memcache缓存??

        return $this->db_write->update($tableName, $data, ['id' => $keywords_id]);
    }

}
