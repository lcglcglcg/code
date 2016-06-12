<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');
/**
 * 公共Model类
 */
define('ASC',					'asc');				/* 升序 				*/
define('DESC',					'desc');			/* 降序 				*/
define('PAGE',					'page');			/* 页数 				*/
define('LEFT',					'left');			/* join left 		*/
define('RIGHT',					'right');			/* join right 		*/
define('JOIN',					'join');			/* join 			*/
define('WHERE',					'where');			/* 查询条件 			*/
define('RANDOM',				'random');			/* 随机排序 			*/
define('SELECT',				'select');			/* 查询字段 			*/
define('ORDER_BY',				'order_by');		/* 排序字段 			*/
define('GROUP_BY',				'group_by');		/* 分组字段 			*/
define('PAGE_NUM',				'page_num');		/* 每页显示数量 			*/
define('JOIN_TYPE',				'join_type');		/* join类型 			*/
define('ORDER_TYPE',			'order_type');		/* 每页显示数量 			*/
define('JOIN_TABLE',			'join_table');		/* join表 			*/
define('WHERE_IN',				'where_in');		/* where 字段 in(f,f) */
class G_Model extends CI_Model {	
	var $logger;
	/**
	 *---------------------------------
	 * 构造函数
	 *---------------------------------
	 *
	 * @access	public
	 */
	function __construct()  {
		parent:: __construct();
		$this->logger = Logger::getLogger('G_model');
	}
	
	// --------------------------------------------------------------------
	
	/**
	 *---------------------------------
	 * 插入数据
	 *---------------------------------
	 *
	 * @param table  String	数据库表名
	 * @param data   Array	数据
	 * @return id	int 	插入后编号
	 * @access	
	 */
	 function add($table, $data) {
		$this->db->insert($table, $data);
		return $this->db->insert_id();
	}
	
	// --------------------------------------------------------------------
	
	/**
	 *---------------------------------
	 * 修改数据
	 *---------------------------------
	 *
	 * @param table  String	数据库表名
	 * @param data   Array	数据
	 * @param where  Array	修改条件
	 * @return id	 int 	所影响的行数
	 * @access	
	 */
	 function modify($table, $set, $where = NULL){
		$this->db->update($table, $set, $where);
		$this->logger->info($this->db->last_query());
		return $this->db->affected_rows();
	}
	
	// // --------------------------------------------------------------------
	// 
	//  function remove($table, $where){
	// 	$this->db->detele($table, $where);
	// } 
	
	// --------------------------------------------------------------------
	
	/**
	 *---------------------------------
	 * 获取结果中的某一条数据
	 *---------------------------------
	 * 
	 * @param result  	查询结果集
	 * @param row	 	获取的行数 默认:1
	 * @param object	Array	对象数组
	 * @access	
	 */
	 function getRow($query, $row = 1){
		if($this->isNotEmpty($query)){
			$object = $query->row($row);
			return $object;
		}
	}
	
	// --------------------------------------------------------------------
	
	/**
	 *---------------------------------
	 * 获取结果中的某一条数据
	 *---------------------------------
	 *
	 * @param query  	查询记录集
	 * @param row	 	获取的行数 默认:1
	 * @return object 	Array  关联数组
	 * @access	
	 */
	 function getRowWithArray($query, $row = 1){
		if($this->isNotEmpty($query)){
			$array = $query->row_array($row);
			return $array;
		}
	}
	
	// --------------------------------------------------------------------
	
	/**
	 *---------------------------------
	 * 查询接口封装 
	 *---------------------------------
	 *
	 * @param table 	String	表名
	 * @param where		Array	查询的where条件
	 * @param othar		Array 	其他过滤条件
	 * @return 查询对象结果集
	 */
	public function getData($table, $query_param = array()){
		if(!empty($query_param)){
			// $this->load->helper('array');
			// $other_key = array(SELECT, WHERE, PAGE, PAGE_NUM, ORDER_BY, ORDER_TYPE, JOIN, JOIN_TABLE, JOIN_TYPE, GROUP_BY, WHERE_IN);
			// $query_param = elements($other_key, $query_param, NULL);
			
			/* select */
			if(!empty($query_param[SELECT])){
				if(is_array($query_param[SELECT])){
					$this->db->select(implode(',', $query_param[SELECT]));
				}else{
					$this->db->select($query_param[SELECT]);
				}
			}
			/* order by */
			if(!empty($query_param[ORDER_BY])){
				$query_param[ORDER_TYPE] = check_empty($query_param[ORDER_TYPE], ASC);
				$this->db->order_by($query_param[ORDER_BY], $query_param[ORDER_TYPE]);
			}
			/* limit */
			if(!empty($query_param[PAGE])){
				$query_param[PAGE_NUM] = check_empty($query_param[PAGE_NUM], $this->config->item('page_num_default')); 
				$this->db->limit($query_param[PAGE_NUM], self::getLimitOffset($query_param[PAGE], $query_param[PAGE_NUM]));
			}
			/* join */
			if(!empty($query_param[JOIN_TABLE]) && !empty($query_param[JOIN])){
				if(is_array($query_param[JOIN_TABLE])){
					foreach($query_param[JOIN_TABLE] as $key => $join_tb){
						if(!isset($query_param[JOIN_TYPE][$key]))
							$query_param[JOIN_TYPE][$key] = LEFT;
						$this->db->join($join_tb, $query_param[JOIN][$key], $query_param[JOIN_TYPE][$key]);
					}
				}else {
					if(empty($query_param[JOIN_TYPE])){ $query_param[JOIN_TYPE] = LEFT;}
					$this->db->join($query_param[JOIN_TABLE], $query_param[JOIN], $query_param[JOIN_TYPE]);
				}
			}
			/* where */
			if(!empty($query_param[WHERE])){
				$this->db->where($query_param[WHERE]);
			}
			/* where_in */
			if(!empty($query_param[WHERE_IN])){
				foreach($query_param[WHERE_IN] as $field => $in){
					$this->db->where_in($field, $in);
				}
			}
			/* group by*/
			if(!empty($query_param[GROUP_BY])){
				$query_param[GROUP_BY] = check_empty($query_param[GROUP_BY]);
				$this->db->group_by($query_param[GROUP_BY]);
			}
			
		}	
		$query = $this->db->get($table);
		//$this->logger->info($this->db->last_query());
		return $query;
	}
	
	// --------------------------------------------------------------------
	
	/**
	 *---------------------------------
	 * 只有where条件的查询
	 *---------------------------------
	 * 
	 * @param table 	String	表名
	 * @param where		Array	查询条件
	 * @return 查询结果集
	 */
	public function getDataOnlyWhere($table, $where = array()){
		$query_param[WHERE] = $where;
		return self:: getData($table, $query_param);
	}
	
	// --------------------------------------------------------------------
	
	/**
	 *---------------------------------
	 * 根据条件统计查询结果数量
	 *---------------------------------
	 *
	 * @param table 		String	表名
	 * @param $query_param	Array 	查询条件
	 */
	public function getCount($table, $query_param = array()){
		$query_param[SELECT] = 'COUNT(*) as count';
		$query = self::getData($table, $query_param);
		$count = $this->getRowWithArray($query);
		return $count['count'];
	}
	
	/**
	 *---------------------------------
	 * 判断结果集是否不为空    
	 *---------------------------------
	 *
	 * @param query 	查询记录集
	 * @param boolean 	true => 不为空, false => 为空
	 * @access	
	 */
	 function isNotEmpty($query){
		if($query->num_rows() > 0){
			return TRUE;
		}else {
			return FALSE;
		}
	}
	
	// --------------------------------------------------------------------
	
	/**
	 *---------------------------------
	 * 判断结果集是否为空    
	 *---------------------------------
	 *
	 * @param query 	查询记录集
	 * @param boolean 	true => 为空, false => 不为空
	 * @access	
	 */
	 function isEmpty($query){
		$boolean = $this->isNotEmpty($query);
		return !$boolean;
	}
	
	// --------------------------------------------------------------------
	
	/**
	 *---------------------------------
	 * 获取指定表下一个自增长id号   
	 *---------------------------------
	 *
	 * @param table 	表名
	 * @return id		下一个id号
	 */
	 function nextId($table){
		$sql = 'show table status like \''.$this->db->dbprefix($table).'\'';
		$query = $this->db->query($sql);
		$status = self::getRow($query);
		return $status->Auto_increment ;
	}
	
	// --------------------------------------------------------------------
	
	/**
	 *---------------------------------
	 * 获取带前缀的表名 
	 *---------------------------------
	 *
	 * @param table 	不带前缀的表名
	 * @return String	带前缀表名
	 */
	 function tableName($table) {
		return $this->db->dbprefix($table);
	}
	
	// --------------------------------------------------------------------
	
	/**
	 *---------------------------------
	 * 根据页数计算出limit的偏移量
	 *---------------------------------
	 *
	 * @param page 		页数
	 * @param page_num	每页显示数量
	 */
	function getLimitOffset($page, $page_num = 0){
		if($page_num == 0){
			 $page_num = $this->config->item('page_num_default'); 
		}
		return ($page - 1) * $page_num;
	}	
}
