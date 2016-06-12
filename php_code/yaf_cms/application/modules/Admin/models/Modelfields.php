<?php if(!defined('APP_PATH')) exit('No direct script access allowed');
	/**
	 * 字段模型类
	 * @author guoxiaochuan maczhis@gmail.com
	 * @access public
	 * @version 1.0 2014/11/27
	 */
	include_once(PATH_APP.'modules/Admin/models/Admin.php');
	class ModelfieldsModel extends AdminModel
	{
		public function __construct()
		{
			parent::__construct();
		}

		/**
		 * 获取模型下的所有字段
		 * @param int $modleid 模型id
		 * @return array 返回该模型下的所有字段信息
		 */
		public function get_model_fields($modelid){
			return $this->db->select('model_field',['fieldid','modelid','field','name','defaultvalue','maxlength','formtype','issearch','isbase','iscore','listorder','status','remark'],['modelid'=>$modelid,'ORDER'=>'listorder,fieldid ASC']);
		}

		/**
		 * 统计某个模型下的字段数量
		 * @param int $modelid 模型id
		 * @return int 返回属于modelid的字段数
		 */
		public function get_count_model_fields($modelid){
			return $this->db->count('model_field','*');
		}

		/**
		 * 根据modelid 获取相关的tablename
		 * @param 	int $modelid   模型的id
		 * @return 	string   表名称
		 */
		public function get_model_name($modelid){
			return $this->db->get('model','tablename',['modelid'=>$modelid]);
		}

		/**
		 * 检测字段是否存在
		 * @param string $field 字段名称
		 * @return int 该字段的数量
		 */
		public function is_field_exists($fieldname){
			return $this->db->count('model_field',['field'=>$fieldname]);
		}


		/**
		 * 获取字段详情
		 * @param  int 		$fieldsid  字段id
		 * @return array 	返回字段详情
		 */
		public function get_fields_detail($fieldsid){
			return $this->db->select('model_field','*',['fieldid'=>$fieldsid]);
		}

		/**
		 * 添加字段
		 * @param array $data 要添加的字段数据
		 * @return int 返回last_insert_id
		 */
		public function add_model_fields($data){
			return $this->db_write->insert('model_field',$data);
		}

		/**
		 * 编辑字段
		 * @param array $data 要修改的字段数据
		 * @param int 	$fieldsid 字段id
		 * @return int 影响的行数
		 */
		public function edit_model_fields($data,$fieldsid){
			return $this->db_write->update('model_field',$data,['fieldid'=>$fieldsid]);
		}

		/**
		 * 删除字段
		 * @param int $fieldsid  要删除的字段的id
		 * @return int 影响的行数
		 */
		public function delete_model_fields($fieldsid){
			return $this->db_write->delete('model_field',['fieldid'=>$fieldsid]);
		}

		/**
		 * 启用字段
		 * @param int $fieldsid 要启用的字段id
		 * @return int 影响的行数
		 */
		public function enable_model_fields($fieldsid){
			return $this->db_write->update('model_field',['status'=>1,'update_time'=>$_SERVER['time']],['fieldid'=>$fieldsid]);
		}

		/**
		 * 禁用字段
		 * @param int $fieldsid 要禁用的字段id
		 * @return int 影响的行数
		 */
		public function disable_model_fields($fieldsid){
			return $this->db_write->update('model_field',['status'=>0,'update_time'=>$_SERVER['time']],['fieldid'=>$fieldsid]);
		}

		/**
		 * 从数据表中删除字段
		 * @param string $tablename 数据表名称
		 * @param string $field_name 要删除的字段名
		 * @return int 删除字段影响的行数
		 */
		public function drop_fields($tablename,$field_name){
			$sql = "ALTER TABLE `$table_name` DROP `$field_name`";
			return (boolean)$this->db_write->query($sql);
		}

		/**
		 * 处理字段
		 * @param string $sql 要执行的sql语句
		 * @return int 影响的结果
		 */
		public function deal_fields($sql){
			return (boolean)$this->db_write->query($sql);
		}
                
	    /*
	     * 根据条件取出某些字段
	     */
		public function get_column_by_where($column,$where){
			return $this->db->select("model_field",$column,$where);
		}

	}