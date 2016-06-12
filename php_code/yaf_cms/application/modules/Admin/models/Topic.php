<?php
	if (!defined('APP_PATH')) exit('No direct script access allowed');

	include_once(PATH_APP . 'modules/Admin/models/Admin.php');
	
	/**
	 * 
	 */
	class TopicModel extends AdminModel 
	{

	    public function __construct() {
	        parent::__construct();
	    }

	    /**
	     * 返回作者信息
	     * @param string $table 表名
	     * @param mixed  $fields 要查询的字段
		 * @param array  $where 查询条件
	     */
	    public function get_anthor_userid($table,$fields,$where){
	    	return $this->db->get($table,$fields,$where);
	    }

	    /**
	     * 获取专题文章列表
	     */
	    public function get_topicdata_list($start,$offset,$and,$where="",$fields = '*'){
	    	$where['ORDER'] = 'order_id DESC';
	        $where['LIMIT'] = [$start, $offset];

	    	return $this->db->select('topic_data',$fields,["AND" => $and,$where]);
	    }

	    public function select($table,$fields,$where){
	    	return $this->db->select($table,$fields,$where);
	    }

	    public function get($table,$fields,$where){
	    	return $this->db->get($table,$fields,$where);
	    }

	    public function add($table,$data){
	    	return $this->db_write->insert($table,$data);
	    }

	    /**
	     * 更新信息
	     */
	    public function update($table,$data,$where){
	    	return $this->db->update($table,$data,$where);
	    }

	    /**
	     * 删除信息
	     */
	    public function delete($table,$where){
	    	return $this->db_write->delete($table,$where);
	    }

	    /**
	     * 统计信息
	     */
	    public function count($table,$where){
	    	return $this->db->count($table,$where);
	    }

	    public function is_exists($table,$fields,$where){
	    	return $this->db->select($table,$fields,$where);
	    }

	    public function last_query(){
	    	return $this->db->last_query();
	    }

	    public function query($sql){
	    	return (array)$this->db->query($sql)->fetchAll(PDO::FETCH_ASSOC);
	    }

	    /**
	     * 获取专题的当月文章数
	     * @param array $data 要处理的专题列表
	     * @param array $time 时间数组$time['firstday']:每月第一天 ，$time['lastday']:每月最后一天
	     * @return array 经过数据处理之后的专题列表
	     */
	    public function count_topicdata_by_month($data,$time){
    		foreach($data as $key=>&$value){
    			$value['monthcount'] = $this->count("topic_data",
    				[
    				"AND"	=>[
    						"tid"	=>$value['topic_id'],
    						"inputtime[<>]" => [ $time["firstday"] , $time["lastday"]]
                		]
    				]);
    			$value['total'] = $this->count("topic_data",["tid"=>$value["topic_id"]]);
    		}

	    	return $data;
	    }
	}
?>