<?php

if (!defined('APP_PATH'))
    exit('No direct script access allowed');

/**
  公共模型类
  @author lucas lucasphper@gmail.com
  @access public
  @package core
  @version 1.0 2014/11/19
 */
class AdminModel {

    //配置信息
    protected $config;
    //db对象:$db_write:主库,$db:从库
    protected $db;
    protected $db_write;
    //缓存
    protected $cache;
    //memcache
    protected $memcache;
    //redis
    protected $redis;

    function __construct() {
        //初始化属性
        $this->config = Yaf_Registry::get('config');

        $this->db = Yaf_Registry::get('db_read');

        $this->db_write = Yaf_Registry::get('db_write');

        $this->cache = CACHE_TYPE == 'redis' ? Yaf_Registry::get('redis') : Yaf_Registry::get('memcache');

        //cookie初始化
        $this->cookie = array(
            'cookie_pre' => Yaf_Registry::get('config')->get('cookie')->pre,
            'cookie_path' => Yaf_Registry::get('config')->get('cookie')->path,
            'cookie_domain' => Yaf_Registry::get('config')->get('cookie')->domain
        );
    }

    /**
     * 查询多条
     * @param type $table
     * @param type $join
     * @param type $columns
     * @param type $where
     * @author yangguofeng
     */
    public function select($table, $join, $columns = null, $where = null) {
        return $this->db->select($table, $join, $columns = null, $where = null);
    }

    /**
     * 查询一条
     * @param type $table
     * @param type $columns
     * @param type $where
     * @return type
     * @author yangguofeng
     */
    public function get($table, $columns, $where = null) {
        return $this->db->get($table, $columns, $where);
    }

    /**
     * 判断是否存在
     * @param type $table
     * @param type $join
     * @param type $where
     * @author yangguofeng
     */
    public function has($table, $join, $where = null) {
        return $this->db->has($table, $join, $where = null);
    }

    /**
     * 删除
     * @param type $table
     * @param type $condition
     * @author yangguofeng
     */
    public function delete($table, $condition = '') {
        return $this->db_write->delete($table, $condition);
    }

    /**
     * 更新
     * @param type $data
     * @param type $table
     * @param type $condition
     * @return type
     * @author yangguofeng
     */
    public function update($table, $data, $condition = '') {
        return $this->db_write->update($table, $data, $condition);
    }

    /**
     * 添加
     * @param $table
     * @param $datas
     * @return mixed
     */
    public function insert($table, $datas) {
        return $this->db_write->insert($table, $datas);
    }

    /**
     * 统计数量
     * @param $table
     * @param $where
     * @return mixed
     */
    public function count($table, $join = null, $column = null, $where = null) {
        return $this->db->count($table, $join, $column, $where);
    }

    public function get_last_query() {
        return $this->db->last_query();
    }

    /**
     * 执行sql语句
     */
    public function query($sql){
        if(preg_match('/^(\s*select)|(\c*count)|(max)|(min)/i',$sql)){
            return $this->db->query($sql);
        }else{
            return $this->db_write->query($sql);
        }
    }

    public function replace($table, $column, $search, $replace, $where){
        $this->db_write->replace($table, $column, $search, $replace, $where);
    }

    /**
     * 根据文章id  返回改id所在的 表id;
     * @param int $id 文章的id
     * @return int 返回表的id
     */
    public function get_table_id($id,$type = 'news'){

        $data = false;

        $cacheKey = 'tableinfo';

        $data = $this->cache->get($cacheKey);

        if(!$data){

            $data = $this->db->select('table',['table_id','start_id','end_id'],['id[>]'=>0]);

            $this->cache->set($cacheKey, $data, 3600 * 12);

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

        if(empty($type)){
            return $table_id;
        }

        return $table_id == 0 ? $type : $type.'_'.$table_id;
    }

    /**
     * 从memcache中读取table表的缓存，如果缓存失效，则重新查询
     * @return $data array table信息
     */
    public function get_table_info(){
        $cacheKey =  'tableinfo';

        $data = false;

        $data = $this->cache->get($cacheKey);

        if(!$data){
            $data = $this->db->select('table',['id','table_id','table_count','table_total','start_id','end_id'],['id[>]'=>0]);

            $this->cache->set($cacheKey, $data, 3600 * 12);
        }

        return $data;
    }

}
