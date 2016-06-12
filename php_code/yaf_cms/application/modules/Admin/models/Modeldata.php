<?php if(!defined('APP_PATH')) exit('No direct script access allowed');
	/**
	 * 数据模型类
	 */
	include_once(PATH_APP.'modules/Admin/models/Admin.php');
	class ModeldataModel extends AdminModel
	{
		public function __construct()
		{
			parent::__construct();
		}

		/**
		 * 获取所有的model列表
		 * @param int $offset 分页偏移向
		 * @param int $per_page 每页显示数量
		 * @return array 返回所有的数据模型
		 */
		public function get_model_list($offset,$per_page){
			return $this->db->select('model',['modelid','siteid','name','description','tablename','status','create_time'],['ORDER'=>'modelid ASC','LIMIT'=>"$offset,$per_page"]);
			
		}

		/**
		 * 获取所有模型总数
		 * @return int 模型总数
		 */
		public function get_model_count(){
			 return $this->db->count('model','*');
		}

		/**
		 * 获取字段数量
		 * @param int $modelid 模型id
		 * @return int 该模型下的所有字段数量
		 */
		public function get_model_fields_count($modelid){
			return $this->db->count('model_field',['modelid'=>$modelid]);
		}

		/**
		 * 获取模型详情
		 * @param int $modelid 模型id
		 * @return array 模型详情
		 */
		public function get_model_detail($modelid){
			return $this->db->select('model',['modelid','name','description','tablename','status'],['modelid'=>$modelid]);
		}

		/**
		 * 根据modelid来获取相对应的数据表名称
		 * @param int $modelid 模型id
		 * @return string 数据表名
		 */
		public function get_modelname_by_id($modelid){
			return $this->db->select('model','tablename',['modelid'=>$modelid]);
		}

		/**
		 * 判断模型表名是否已经存在
		 * @param string $modelname 模型名称
		 * @return int 数量
		 */
		public function is_modelname_exists($modelname){
			return $this->db->count('model','*',['tablename'=>$modelname]);
		}

		/**
		 * 添加模型
		 * @param  array $data 要修改的数据
		 * @return int   返回插入后的modelid
		 */
		public function add_model($data){
			return $this->db_write->insert('model',$data);
		}

		/**
		 * 编辑模型
		 * @param array $data  		修改的数据
		 * @param int 	$modelid 	模型id
		 * @return int   影响的行数
		 */
		public function edit_model($data,$modelid){
			return $this->db_write->update('model',$data,['modelid'=>$modelid]);
		}

		/**
		 * 删除模型
		 * @param int $modelid  模型id
		 * @return int 影响的行数
		 */
		public function delete_model($modelid){
			return $this->db_write->delete('model',['modelid'=>$modelid]);
		}

		/**
		 * 删除模型字段
		 * @param int $modelid 模型id
		 * @return int 删除影响的行数
		 */
		public function delete_model_fields($modelid){
			return $this->db_write->delete('model_field',['modelid'=>$modelid]);
		}

		/**
		 * 删除数据表
		 * @param 要删除的表名
		 * @return bool 是否删除成功
		 */
		public function drop_model_table($tablename){
			return (boolean)$this->db_write->query("DROP TABLE IF EXISTS `$tablename`");
		}

		/**
		 * 启用模型
		 * @param int $modelid 模型id
		 * @return int 影响的行数
		 */
		public function enable_model($modelid){
			return $this->db_write->update('model',['status'=>1,'update_time'=>$_SERVER['time']],['modelid'=>$modelid]);
		}

		/**
		 * 禁用模型
		 * @param int $modelid 模型id
		 * @return int 影响的行数
		 */
		public function disable_model($modelid){
			return $this->db_write->update('model',['status'=>0,'update_time'=>$_SERVER['time']],['modelid'=>$modelid]);
		}

		/**
		 * 创建模型
		 * @param string $sql  创建表的sql语句
		 * @return boolean   真/假  
		 */
		public function create_model($sql){
			return (boolean)$this->db_write->query($sql);
		}
	}