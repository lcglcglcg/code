<?php 
	if(!defined('APP_PATH')) exit('No direct script access allowed');

	class TopicModel extends BaseModel{
	    /**
	     * 初始化
	     */
	    public function __construct(){
	        parent::__construct();
	    }

	    // public function get($table,$field,$where){
	    // 	return $this->db->get($table,$field,$where);
	    // }

	    // public function select($table,$field,$where){
	    // 	return $this->db->select($table,$field,$where);
	    // }

	    // public function insert($table,$data){
	    // 	return $this->db_write->insert($table,$data);
	    // }

	    // public function last_query(){
	    // 	return $this->db->last_query();
	    // }

	    /**
	     * 获取专题信息
	     * @param $path string 专题的path
	     * @return array 该专题的详细信息
	     */
	    public function get_topic_detail($path){
	    	
	    	$redisKey = 'topic:' . $path;
	    	
	    	$fields = [
			    		'topic_id',
			    		'topic_name',
			    		'topic_path',
			    		'topic_title',
			    		'topic_keywords',
			    		'topic_description',
			    		'topic_thumb',
			    		'topic_banner',
			    		'topic_template',
			    		'create_time',
			    		'show_data'
			    	];

			$where['AND'] = ['topic_path' =>	$path,'is_show'	 =>	 1];
			$where['LIMIT'] = 1;

	    	return $this->Cache2Select('topic', 'get', $redisKey, 3600 * 12, $fields, $where);

	    	// if($data == false){
	    	// 	$data = $this->db->get('topic',
	    	// 		    		[
	    	// 		    		'topic_id',
	    	// 		    		'topic_name',
	    	// 		    		'topic_path',
	    	// 		    		'topic_title',
	    	// 		    		'topic_keywords',
	    	// 		    		'topic_description',
	    	// 		    		'topic_thumb',
	    	// 		    		'topic_banner',
	    	// 		    		'topic_template',
	    	// 		    		'create_time',
	    	// 		    		'show_data'
	    	// 		    		],[
	    	// 		    			"AND" => [
	    	// 		    				'topic_path' =>	$path,
	    	// 		    				'is_show'	 =>	 1
	    	// 		    		  ],
	    	// 		    			'LIMIT'	=>	1
	    	// 		    		  ]
	    	// 		    	);
	    	// 	$this->redis->set($redisKey, $data, 3600*12);
	    	// }

	    	// return $data;
	    }

	    /**
	     * 根据专题id获取专题的二级菜单栏目
	     * @param $tid int 专题的id
	     * @return array 返回专题的所有的二级栏目
	     */
	    public function get_category($tid){

	    	$redisKey = 'topic_subclass:'. $tid;

	    	return $this->Cache2Select('topic_subclass', 'select', $redisKey, 3600 * 12, ['id','title'], ['AND'=>['tid'=>$tid,'is_menu'=>1]] );

	    	// $data = $this->redis->get($redisKey);

	    	// if($data == false){
	    	// 	$data = $this->db->select('topic_subclass',['id','title'],['AND'=>['tid'=>$tid,'is_menu'=>1]]);
	    	// 	$this->redis->set($redisKey,$data,3600*12);
	    	// }

	    	// return $data;
	    }

	    /**
	     * 根据专题的栏目id获取专题栏目的详细信息
	     * @param $sid int 栏目的id
	     * @return array 返回该专题栏目的详细信息
	     */
	    public function get_subclass_detail($sid){
	    	$redisKey = 'topic_subclass:subclassinfo:'.$sid;

	    	return $this->Cache2Select('topic_subclass', 'get', $redisKey, 3600 * 12,
							    		[
								    		'id',
								    		'tid',
								    		'parent_id',
								    		'title',
								    		'keywords',
								    		'description',
								    		'show_data',
								    		'url'
							    		], [
								    		'status'=>1,
								    		'LIMIT'=>1
							    		]);

	    }


	    /**
	     * 获取子类列表
	     * @param $fields maxed 要查询的字段
	     * @param $where  array 查询的条件
	     * @return array 返回专题栏目列表
	     */
	    public function get_subclass_list($fields,$where){
	    	$redisKey = 'topic_subclass:list:';

	    	return $this->Cache2Select('topic_subclass', 'select', $redisKey, 3600 * 12, $fields, $where);

	    	//return $this->db->select('topic_subclass',$fields,$where);
	    }

	    /**
	     * 获取子类文章列表
	     * @param $fields maxed 查询的字段；
	     * @param $where array 查询的条件 
	     * @return array 返回文章列表
	     */
	    public function get_topicdata_list($fields,$where){
	    	$redisKey = 'topic_data:';

	    	return $this->Cache2Select('topic_data', 'select', $redisKey, 3600 * 12, $fields, $where);

	    	return $this->db->select('topic_data',$fields,$where);
	    }

	    /**
	     * 获取文章详情信息
	     */
	    public function get_topicdata_detail($id){

	    	$redisKey = 'topic_detail:' . $id;

	    	$data =  $this->Cache2Select('topic_data', 'get', $redisKey, 3600 * 12,[
                        'id',
                        'title',
                        'content',
                        'tid',
                        'sid',
                        'userid',
                        'username',
                        'description',
                        'keywords',
                        'inputtime'
                    ],
                    ['AND'=>['status'=>1,'id'=>$id],'LIMIT'=>1]);
	    	
	    	$data['topic_path'] = $this->db->get("topic","topic_path",["topic_id"=>$data['tid'],"LIMIT"=>1]);

	    	return $data;

	    }

	    /**
	     * 获取所有的专题path
	     */
	    public function gettopicpath(){
	    	$redisKey = 'topic_data:pathlist:';

	    	return $this->Cache2Select('topic', 'select', $redisKey, 3600 * 12,'topic_path',['is_show'=>1,'ORDER'=>'create_time']);

	    	//return $this->db->select('topic','topic_path',['is_show'=>1,'ORDER'=>'create_time']);
	    }

	    /*
	     * 根据文章id获得该文章的上一篇下一篇
	     * int $id
	     * return array
	     */
	    public function preNext($inputtime , $cond ,$topicPath, $subTopicPath) {
	        $data = false;
	        
	        if (is_numeric($inputtime)) {
	            $redisKey =  'topic:prenext' . $inputtime;
                    
                    //若存在专题栏目
	            if (isset($cond['sid']) && $cond['sid']) {
	                $where['AND']['sid'] = $cond['sid'];
	                $redisKey .= '.sid' . $cond['sid'];
	            }
                    //若存在专题ID
	            if (isset($cond['tid']) && $cond['tid']) {
	                $where['AND']['tid'] = $cond['tid'];
	                $redisKey .= '.tid' . $cond['tid'];
	            }

	            $where['AND']['status'] = 1;
	            
	            $data = $this->redis->get($redisKey);
	            
	            if ($data == false) {
	                //上一篇
	                $where['AND']['inputtime[>]'] = $inputtime;
	                $where['ORDER'] = 'inputtime ASC';
	                $previous = $this->db->get('topic_data', ['id', 'sid', 'title'], $where);
	                if ($previous == false){
	                    $data['previous'] = '没有了';
	                }else{
	                    $data['previous'] = '上一篇：<a href="/' . SITEDIR . '/' . $topicPath .'/' . $subTopicPath . '/' . $previous['id'] . '.html" title="' . $previous['title'] . '">' . $previous['title'] . '</a>';
	                }

	                //下一篇
	                unset($where['AND']['inputtime[>]']);
	                $where['ORDER'] = 'inputtime DESC';
	                $where['AND']['inputtime[<]'] = $inputtime;
	                $next = $this->db->get('topic_data', ['id', 'sid', 'title'], $where);
	                
	                if ($next == false){
	                    $data['next'] = '没有了';
	                }else{
	                    $data['next'] = '下一篇：<a href="/' . SITEDIR . '/' . $topicPath .'/' . $subTopicPath . '/' . $next['id'] . '.html" title="' . $next['title'] . '">' . $next['title'] . '</a>';
	                }

	                $this->redis->set($redisKey, $data,  3600 * 24);
	            }
	        }
	        return $data;
	    }

	    /**
	     * 查询所有的专题子栏目列表
	     * @param int $topic_id 专题id
	     */
	    public function select_subclass_list($topic_id) {

	    	$redisKey = 'topic_subclass:list:'.$topic_id;

	    	return $this->Cache2Select('topic_subclass', 'select', $redisKey, 3600 * 12,['id','tid','parent_id','title','path'],['AND'=>['tid'=>$topic_id,'status'=>1],'ORDER'=>'id']);

	    	//return $this->db->select('topic_subclass',['id','tid','parent_id','title','path'],['AND'=>['tid'=>$topic_id,'status'=>1],'ORDER'=>'id']);
	    }

	    /**
	     * 根据专题path来获取专题信息
	     */
	    public function get_topicinfo_by_path($topic_path){
	    	$redisKey = 'topic:detail:'.$topic_path;

	    	return $this->Cache2Select('topic', 'get', $redisKey, 3600 * 12,['topic_id','topic_name','topic_guidec'],['topic_path'=>$topic_path,'LIMIT'=>1]);

	    	//return $this->db->get('topic',['topic_id','topic_name','topic_guidec'],['topic_path'=>$topic_path,'LIMIT'=>1]);
	    }

	    /**
	     * 获取某个分类下面的文章数量
	     */
	    public function count_subclass_posts($subclass_id){
	    	return $this->db->count("topic_data",["AND"=>["sid"=>$subclass_id,"status"=>1]]);
	    }

	}
?>