<?php

if (!defined('APP_PATH'))
    exit('No direct script access allowed');

/**
  文档类
  @author lucas lucasphper@gmail.com
  @package admin
  @access public
  @version 1.0 2014/11/27
 */
include_once(PATH_APP . 'modules/Admin/models/Admin.php');

class DocumentModel extends AdminModel {

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
    public function newsList($cond, $start = 0, $offset = 20, $order = 'inputtime DESC', $select = ['id', 'catid', 'title', 'keywords', 'status', 'inputtime', 'updatetime', 'attribute']) {

        $cacheKey = 'admin:list';

        $where = [];

        $news_table_name = 'news';

        if (isset($cond['id'])) {
            $news_table_name = $this->get_table_id($cond['id']);
            $where['AND']['id'] = $cond['id'];
            $cacheKey .= ' .id' . $cond['id'];
        }

        if (isset($cond['catid'])) {
            $where['AND']['catid'] = $cond['catid'];
            $cacheKey .= '.catid' . $cond['catid'];
        }

        if (isset($cond['status'])) {
            $where['AND']['status'] = $cond['status'];
            $cacheKey .= '.status' . $cond['status'];
        }

        if (isset($cond['title'])) {
            $where['LIKE']['title%'] = $cond['title'];
            $cacheKey .= '.like' . $cond['title'];
            return $this->multi_news_search($where,$start , $offset);
        }

        $where['LIMIT'] = [$start, $offset];

        $where['ORDER'] = $order;

        $cacheKey .= '.limit' . ($start + $offset) . $order;

        $data = $this->cache->get(md5($cacheKey));

        if (!$data) {

            $data = $this->db->select($news_table_name, $select, $where);
            if($data){
                $this->cache->set(md5($cacheKey), $data, 5);
            }
        }

        return $data;
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

        if (isset($cond['status'])) {
            $where['AND']['status'] = $cond['status'];
        }

        if (isset($cond['title'])) {
            $where['LIKE']['title'] = $cond['title'];
        }

        if (isset($cond['inputtime[>]'])) {
            $where['AND']['inputtime[>]'] = $cond['inputtime[>]'];
        }

        if (isset($cond['inputtime[<]'])) {
            $where['AND']['inputtime[<]'] = $cond['inputtime[<]'];
        }

        //$where['AND']['inputtime[<]'] = $time;

        return $this->db->count('news', $where);
    }

    /**
     * 根据id一条新闻
     * @param type $cond
     * @param type $select
     * @return type
     * @author yangguofeng
     */
    public function getNewsById($id) {
        $news_table_name = $this->get_table_id($id);
        $news_data_table_name = $this->get_table_id($id,'news_data');

        return $this->db->select($news_table_name, [
                    "[>]$news_data_table_name" => "id"
                        ], [
                    "$news_table_name.id",
                    "$news_table_name.title",
                    "$news_table_name.catid",
                    "$news_table_name.keywords",
                    "$news_table_name.description",
                    "$news_table_name.attribute",
                    "$news_data_table_name.content",
                        ], [
                    "$news_table_name.id" => $id
        ]);
    }

    /**
     * 添加文章
     * @param type $data
     * @param type $table
     * @return type
     * @author yangguofeng
     */
    public function addNews($data) {
        $this->db_write->query('BEGIN');
        try {
            $id = $this->db_write->insert('news', [
                'title' => $data['title'],
                'catid' => $data['catid'],
                'listorder' => isset($data['listorder']) ? $data['listorder'] : 0,
                'keywords' => isset($data['keywords']) ? $data['keywords'] : '',
                'url' => isset($data['url']) ? $data['url'] : '',
                'uid' => isset($data['uid']) ? $data['uid'] : 0,
                'attribute' => isset($data['attribute']) ? $data['attribute'] : 0,
                'description' => isset($data['description']) ? $data['description'] : '',
                'inputtime' => isset($data['inputtime']) ? $data['inputtime'] : $_SERVER['REQUEST_TIME'],
                'updatetime' => isset($data['updatetime']) ? $data['updatetime'] : (isset($data['inputtime']) ? $data['inputtime'] : $_SERVER['REQUEST_TIME']),
                'status' => 99,
                'en_keywords' => $data['en_keywords']
                    ]
            );

            if ($id) {
                $this->db_write->insert('news_data', [
                    'id' => $id,
                    'content' => $data['content'],
                    'defaultpic' => $data['defaultpic']
                        ]
                );

                //更新memcache中table的信息
                $this->update_memcache($id);

            } else {
                throw new Exception;
            }

            $this->db_write->query('COMMIT');
            return TRUE;
        } catch (Exception $exc) {
            $this->db_write->query('ROLLBACK');
        }
        return FALSE;
    }

    /**
     * 批量删除文章
     * @param type $where
     * @return boolean
     * @throws Exception
     * @author yangguofeng
     */
    public function batchDeleteNews($where) {
        $this->db_write->query('BEGIN');
        try {
            // 根据$where 条件中的id，返回相对应的表
            $news_table_name = $this->get_table_id($where['id']);
            $news_data_table_name = $this->get_table_id($where['id'],'news_data');

            if ($this->delete($news_table_name, $where)) {
                if ($this->delete($news_data_table_name, $where)) {
                    //删除成功，更新table表中的数据
                    $table_id = $this->get_table_id($where['id'],'');
                    $this->db_write->update('table',['table_count[-]'=>1],['table_id'=>$table_id]);
                    $this->db_write->update('table',['table_total[-]'=>1],['table_id'=>0]);

                    //更新memcache缓存中的table信息
                    $data = get_table_info();
                    $cacheKey =  'tableinfo';

                    foreach($data as $k=>&$v){
                        if($v['table_id'] == $table_id){

                            $data[0]['table_total']--;
                            $data[$k]['table_total']--;

                            
                        }
                    }
                    
                    $this->cache->set(md5($cacheKey), $data,  3600 * 12);
                    
                } else {
                    throw new Exception;
                }
            } else {
                throw new Exception;
            }

            $this->db_write->query('COMMIT');
            return TRUE;
        } catch (Exception $exc) {
            $this->db_write->query('ROLLBACK');
        }
        return FALSE;
    }

    /**
     * 获取文档数
     * @param array $condition 条件参数
     * @param string $tableName 表名
     * @return int
     */
    public function get_document_count($condition, $tableName = 'news') {
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
     * 获取文章列表
     * @param int $first 起始位置
     * @param int $limit 条数
     * @param array $condition 条件参数
     * @param string $tableName 表名
     * @return array
     */
    public function get_document_list($first, $limit, $condition, $tableName = 'news') {
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
     * 删除文档
     * @param int $document_id 文档id
     * @param string $tableName 表名
     * @return bool
     */
    public function delete_document($document_id, $tableName = 'news') {
        
        //清memcache缓存
        $this->cache->delete( 'article:hits' . $document_id);
        $this->cache->delete( 'category');
        $this->cache->delete( 'article:detail' . $document_id);
        $tableName = $this->get_table_id($document_id);
        return $this->db_write->delete($tableName, array('id' => $document_id));
    }

    /**
     * 显示隐藏文档
     * @param array $document_detail 文档详情
     * @param string $tableName 表名
     * @return bool
     */
    public function document_display($document_detail, $tableName = 'news') {

        return $this->db_write->update($tableName, $document_detail, array('id' => $document_detail['id']));
    }

    /**
     * 文档详情
     * @param int $document_id 文档id
     * @return array
     */
    public function get_document_detail($document_id, $column = "*", $tableName = 'news') {
        $news_table_name = $this->get_table_id($document_id);
        $news_data_table_name = 'news_data_'.$this->get_table_id($document_id,'');
        
        $basic = $this->db->get($news_table_name, array('id', 'catid', 'uid', 'status', 'updatetime', 'url', 'title', 'keywords', 'description', 'listorder'), ['id' => $document_id]);
        
        $detail = $this->db->get($news_data_table_name, ['id', 'content'], ['id' => $document_id]);
        
        if ($detail) {
            return array_merge($basic, $detail);
        } else {
            return $basic;
        }
    }

    /**
     * 添加文档
     * @param array $document_detail 文档详情
     * @param string $tableName 表名
     * @return bool
     */
    public function document_add($data, $tableName = 'news') {
        $insertid = $this->db_write->insert($tableName, $data);

        $this->update_memcache($insertid);

        return $insertid;
    }

    /**
     * 更新文档
     * @param array $document_detail 文档详情
     * @param string $tableName 表名
     * @return bool
     */
    public function document_update($document_id, $data, $tableName = 'news') {
        //清memcache缓存
        $this->cache->delete( array('article:hits' . $document_id,'category','article:detail' . $document_id));
        $tableName = $this->get_table_id($document_id);
        return $this->db_write->update($tableName, $data, array('id' => $document_id));
    }

    /**
     * @param int $document_id 文档id
     * @param array $data 要修改的数据
     */
    public function document_trash($document_id, $data) {
        //清memcache缓存
        $this->cache->delete( array( 'article:hits' . $document_id,'category','article:detail' . $document_id));
        $news_table_name = $this->get_table_id($document_id);
        return $this->db_write->update($news_table_name, $data, ['id' => $document_id]);
    }

    /**
     * 批量删除文章
     * @param type $where
     * @return boolean
     * @throws Exception
     * @author yangguofeng
     */
    public function batch_trash($where) {
        $this->db_write->query('BEGIN');
        try {
            if ($this->document_trash($where['id'], ['status' => 5, 'updatetime' => $_SERVER['time']])) {
                
            } else {
                throw new Exception;
            }

            $this->db_write->query('COMMIT');
            return TRUE;
        } catch (Exception $exc) {
            $this->db_write->query('ROLLBACK');
        }
        return FALSE;
    }

    /**
     * 更新memcache中table记录的数据
     * @param $insertid int 插入文章的id
     */
    public function update_memcache($insertid){
        $cacheKey =  'tableinfo';
        $data = false;
        $data = $this->cache->get($cacheKey);

        if($insertid){
            if($data ==  false){
                $data = $this->db->select('table',['id','table_id','table_count','table_total','start_id','end_id'],['id[>]'=>0]);
            }

            //实时更新memcache缓存中的信息
            $mem_count = count($data);
            ++$data[$mem_count-1]['table_count'];
            ++$data[$mem_count-1]['table_total'];
            $data[$mem_count-1]['end_id'] = $insertid;
            
            ++$data[0]['table_count'];
            ++$data[0]['table_total'];
            $data[0]['end_id'] = $insertid;

            $this->cache->set($cacheKey, $data,  3600 * 12);
            unset($data);
            //更新table表信息
            $this->db_write->update('table',['table_count[+]'=>1,'table_total[+]'=>1,'end_id'=>$insertid],['ORDER'=>'id DESC','LIMIT'=>1]);

            $this->db_write->update('table',['table_count[+]'=>1,'table_total[+]'=>1,'end_id'=>$insertid],['id'=>1]);
        }
    }

    /**
     * news表分表后根据title标题来搜索文章
     * @param $where array 查询的条件
     * @return array  查询好的数据
     */
    public function multi_news_search($where,$start = 0, $offset = 20){
        //获取news表信息
        $cacheKey =  'tableinfo';
        
        $data = false;
        
        $data = $this->cache->get($cacheKey);

        if(!$data){
            $data = $this->db->select('table',['id','table_id','table_count','table_total','start_id','end_id'],['id[>]'=>0]);
            $this->cache->set($cacheKey, $data,  3600 * 12);

        }

        $cacheKey =  'multinewssearch:'.$where['LIKE']['title%'];

        $search_list = false;

        $search_list = $this->cache->get($cacheKey);

        if(!$search_list){
            $tmp = [];
            foreach($data as $k=>$v){

                $news_table_name = $v['table_id'] == 0 ? 'news' : 'news_'.$v['table_id'];

                $tmp[] = $this->db->select($news_table_name,['id','catid','title'],$where);
            }

            unset($data);
            $search_list = [];

            foreach($tmp  as $k=>$v){
                foreach($v as $j){
                    $search_list[] = $j;
                }
            }
            $this->cache->set( $cacheKey, $search_list, 3600);

            $cacheKey = 'multinewssearch:count:'.$where['LIKE']['title%'];
            
            $this->cache->set( $cacheKey, count($search_list), 3600);
        }

        $return_data = [];

        foreach($search_list as $k=>$v){
            if($k >= $start && $k <= ($start+$offset)){
                $return_data[] = $v;
            }
            
        }

        return $return_data;

    }

    /**
     * 统计缓存中的$where['LIKE']['title%']中搜索结果数量
     */
    public function count_multisearch($where){
        $cacheKey =  'multinewssearch:count:'.$where['title'];

        $data = false;
        
        $data = $this->cache->get($cacheKey);

        return $data;
    }

}