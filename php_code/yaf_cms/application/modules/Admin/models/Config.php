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

class ConfigModel extends AdminModel {

    function __construct() {
        parent::__construct();
    }

    /**
     * 获取前台信息列表
     * @param int $start 分页起始页
     * @param int $offset 分页结束页
     * @param array $select 
     * @return array
     * @author wangzejun
     */
    public function config_info_list( $start, $offset,$select = '*',$where = array()) {
        $where['LIMIT'] = [$start, $offset];
        return $this->db->select('config', $select, $where);
    }

    //获取前台信息列表条数
    public function config_info_count($cond = []) {

        return $this->db->count('config',$cond);
    }

    /**
     * 获取一条记录
     * @param type $cond
     * @param type $select
     * @return type
     */
    public function getOne($select,$cond) {
        return $this->db->get('config',$select,$cond);
    }

    /**
     * 前台信息跟新
     * @param type $data
     * @param type $condition
     * @return type
     */
    public function update_config($data, $condition) {
        $this->db_write->delete('config',$condition);
        return $this->db_write->insert('config',$data);
    }

    /**
     * 添加前台信息
     * @param type $data
     * @return mixed
     */
    public function add_config($data) {
        return $this->db_write->insert('config',$data);
    }

   /**
     * 删除前台信息
     * @param type $data
     * @return mixed
     */
    public function delete_config($where) {
        return $this->db_write->delete('config',$where);
    }

    /**
     * 获取网站配置信息
     */
    public function get_config(){
        $data = [];
        $result = $this->db->select('option','*');
        if($result){
            foreach($result as $key=>$value){
                $data[$value['conf_name']] = $value['conf_value'];
            }
        }
        return $data;        
    }

    /**
     * 
     */
    public function update_option($data,$where){
        return $this->db_write->update('option',$data,$where);
    }

    /**
     * 获取网站文章页 和 列表页的调用
     */
    public function get_articles_call(){
       $data = [];
       $list_data = $this->db->get('option','conf_value',['conf_id'=>'listpage']);
       $content_data = $this->db->get('option','conf_value',['conf_id'=>'contentpage']);
       $data['listpage'] = json_decode($list_data,true);
       $data['contentpage'] = json_decode($content_data,true);
       return $data;
    }

}
