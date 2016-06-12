<?php

if (!defined('APP_PATH'))
    exit('No direct script access allowed');

class BaseModel {

    /**
     * 所有配置信息.
     * @var Yaf_Config_Ini
     */
    protected $config;
    // db
    protected $db;
    protected $db_write;
    // memcache
    protected $memcache;
    // redis
    protected $redis;

    function __construct() {
        //配置文件
        $this->config = Yaf_Registry::get('config');
        //数据库
        $this->db = Yaf_Registry::get('db_read');
        $this->db_write = Yaf_Registry::get('db_write');
        //memcache
        $this->memcache = Yaf_Registry::get('memcache');
        //redis
        $this->redis = Yaf_Registry::get('redis');

        //cookie初始化
        $this->cookie = ['cookie_pre' => Yaf_Registry::get("config")->get('cookie')->pre, 'cookie_path' => Yaf_Registry::get("config")->get('cookie')->path, 'cookie_domain' => Yaf_Registry::get("config")->get('cookie')->domain];
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
     * @param type $table
     * @param type $join
     * @param type $column
     * @param type $where
     * @return type
     */
    public function count($table, $join = null, $column = null, $where = null) {
        return $this->db->count($table, $join, $column, $where);
    }

    /**
     * 查询多条
     * @param type $table
     * @param type $join
     * @param type $columns
     * @param type $where
     * @return type
     */
    public function select($table, $join, $columns = null, $where = null) {
        return $this->db->select($table, $join, $columns, $where);
    }
    
    /**
     * 输出错误
     */
    public function getLastQuery() {
        return $this->db->last_query();
    }

    /**
     * 执行sql语句
     */
    public function query($sql){
        if(preg_match('/^(\s*select)|(count)|(max)|(min)|(has)/i',$sql)){
            return $this->db->query($sql);
        }else{
            return $this->db_write->query($sql);
        }
    }

    public function replace($table, $column, $search, $replace, $where){
        $this->db_write->replace($table, $column, $search, $replace, $where);
    }
    
    /**
     * 获取专题的分类
     * array
     */
    public function topic_subclass($limit = 0) {
        $redisKey =  'subclass' . $limit;

        $data = false;

        $data = $this->redis->get($redisKey);
        
        if (!$data) {
            $data = [];
            $limit = $limit ?  $limit : 30;
            $temp = $this->db->select('topic_subclass', [
                'id',
                'tid',
                'path'
                ], [
                'status' => 1,
                "ORDER" => "tid DESC",
                "LIMIT" => $limit
                ]
            );

            foreach ($temp as $v){
                $data[$v['id']] = $v;
            }

            //缓存
            $data <> false && $this->redis->set($redisKey, $data, 3600 * 24);
        }
        
        return $data;
    }

    /**
     * 根据文章id  返回改id所在的 表id;
     * @param int $id 文章的id
     * @return int 返回表的id
     */
    public function get_table_id($id,$type = 'news'){

        $data = false;

        $redisKey = 'tableinfo';

        $data = $this->redis->get($redisKey);

        if(!$data){

            $data = $this->db->select('table',['table_id','start_id','end_id'],['id[>]'=>0]);

            $this->redis->set($redisKey, $data, 3600 * 12);

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

        return $table_id == 0 ? $type : $type.'_'.$table_id;
    }

    /**
     * 根据文章时间  返回改该时间所在的 表id;
     * @param int $starttime 开始时间
     * @param int $endtime 结束时间
     * @return int 返回表的id
     */
    public function get_table_id_by_time($starttime,$endtime,$type = 'news'){

        $data = false;

        $redisKey = 'tableinfobytime';

        $data = $this->redis->get($redisKey);

        if(!$data){

            $data = $this->db->select('table',['table_id','start_id','end_id','starttime','endtime'],['id[>]'=>0]);

            $this->redis->set($redisKey, $data, 3600 * 12);

        }
        


        $table_id = 0;
        foreach($data as $k=>$v){

            if($starttime >= $v['starttime'] && $endtime <= $v['endtime']){
                
                $table_id = $v['table_id'];
                break;
            }
        }

        return $table_id == 0 ? $type : $type.'_'.$table_id;
    }


    /**
     * 获取category栏目分类列表信息，并缓存
     * @param $by string 排序规则
     * @param $limit int 
     * @return array 栏目分类列表
     */
    public function category($by = '', $limit = 99) {
        $redisKey =  'category' . $by . $limit;

        $data = false;

        $data = $this->redis->get($redisKey);

        if (!$data) {
            $data = [];
            $temp = $this->db->select('category', [
                'catid',
                'en',
                'pc_show_limit',
                'wap_show_limit',
                'catname',
                'catpath',
                'keyword',
                'description'
                    ], [
                'parentid' => 7,
                "ORDER" => "orderid DESC",
                "LIMIT" => $limit
                    ]
            );


            if ($by == "en_key"){
                foreach ($temp as $v){
                    $data[$v['catpath']] = $v;
                }
            }else{
                foreach ($temp as $v){
                    $data[$v['catid']] = $v;
                }
            }

            //缓存24小时
            $data <> false && $this->redis->set($redisKey, $data, 3600 * 24);
        }
        return $data;
    }

    /**
     * 获取option网站配置信息表
     */
    public function get_option_info(){
        $redisKey = 'option';

        $tmp = false;

        $tmp = $this->redis->get($redisKey);

        if(!$tmp){
            $tmp = $this->db->select('option',['conf_name','conf_value'],['conf_id[>]'=>0]);

            $data = [];

            foreach($tmp as $v){
                $data[$v['conf_name']] = $v['conf_value'];
            }

            $this->redis->set($redisKey,$data,3600 * 24 * 31);

            return $data;
        }else{
            return $tmp;
        }
    }

    /**
     * 通用方法：
     * 缓存&查询：如果存在缓存，就返回缓存，否则就查询数据库，并将查询得让结果保存到缓存中
     * @param $table_name string 要查询的表名
     * @param $type string 查询的方法：默认select
     * @param $cacheKey string 缓存的key
     * @param $cacheTime int 缓存的时间
     * @param $fields  maxed 要查询的字段（string 和 array）
     * @param $where array 查询的条件
     * @return array 返回查询的结果
     */
    public function Cache2Select(string $table_name,string $type,string $cacheKey,int $cacheTime,maxed $fields,array $where){
        $data = false;

        $data = $this->redis->get($cacheKey);

        if (!$data) {
            if ($type == 'select') {
                $data = $this->db->select($table_name,$fields,$where);

            } else {
                $data = $this->db->get($table_name,$fields,$where);
            }

            if($data){
                $this->redis->set($cacheKey,$data,$cacheTime);
            }
        }

        return $data;
    }
}