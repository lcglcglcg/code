<?php if(!defined('APP_PATH')) exit('No direct script access allowed');
	/**
	 * 字段管理类
	 * @author guoxiaochuan maczhis@gmail.com
	 * @access public
	 * @version 1.0 2014/11/27
	 */
	Yaf_Loader::import(PATH_APP.'modules/Admin/controllers/Admin.php');
	Yaf_Loader::import(PATH_APP.'modules/Admin/models/Modelfields.php');
	class ModelfieldsController extends AdminController
	{	
		public function init(){
			parent::init();
			$this->modelfields = new ModelfieldsModel();
		}

		/**
		 * 获某个模型下的所有字段详细
		 */
		public function indexAction(){
			//获取模型id
			$model_id = isset($_GET['mid']) ? intval(trim($_GET['mid'])) : 0 ;
			if(!$model_id){
				echo '<script>alert("非法请求");window.location.href="/admin/modeldata/index/";</script>';
				exit;
			}

			//获取模型字段详情
			$field_detail = $this->modelfields->get_model_fields($model_id);
			//显示
			$this->getView()->assign('model_id',$model_id);
			$this->getView()->assign('field_detail',$field_detail);

		}

		/**
		 * 添加字段
		 */
		public function addAction(){
			//获取模型id
			$model_id = isset($_GET['mid']) ? intval(trim($_GET['mid'])) : 0 ;
			if(!$model_id){
				echo '<script>alert("非法请求");window.location.href="/admin/modeldata/index/";</script>';
				exit;
			}

			//判断操作
			if(trim($this->getRequest()->getPost('dosubmit',0))){
				//获取参数
				$info = $this->getRequest()->getPost('info',0);

				//判断信息是否合法
				if(!$info){
					echo '<script>alert("请检查信息是否完整");window.history.go(-1);</script>';
					exit;
				}
				
				if(isset($info['field']) && trim($info['field'])){
					$info['field'] = trim($info['field']);
				}else{
					echo '<script>alert("请填写字段名称");window.history.go(-1);</script>';
					exit;
				}
				
				// 检测表明是否存在
				if($this->modelfields->is_field_exists($info['field'])){
					echo '<script>alert("该字段已经存在");window.history.go(-1);</script>';
					exit;
				}

				$info['maxlength'] = isset($info['maxlength']) && intval(trim($info['maxlength'])) ? intval(trim($info['maxlength'])) : 32;
				//判断字段类型
				$field_type = $this->get_fields_type($info['type_id'],$info['date_type']);
				
				$info['modelid'] 	= $model_id;
				$info['formtype'] 	= $field_type;
				$info['listorder'] 	= 15;
				$info['update_time'] = $_SERVER['time'];
				$param = array(
					'defaultvalue'	=> $info['defaultvalue'],
					'maxlength'		=> $info['maxlength'],
					'minnumber'		=> 1
				);

				unset($info['date_type']);
				unset($info['type_id']);
				//获取数据表名称
				$tablename = $this->modelfields->get_model_name($model_id);
				//将该字段添加至model_field表中
				$result_1 = $this->modelfields->add_model_fields($info);

				//判断操作结果
				$result_2 = $this->deal_add_fields($tablename,$field_type,$info['field'],$param);
				if($result_1 && $result_2){
					echo '<script>alert("操作成功");window.location.href="/admin/modelfields/index?mid='.$result_1.'";</script>';
				}else{
					echo '<script>alert("操作失败");window.location.href="/admin/modeldata/index/";</script>';
				}
			}else{
				//获取字段类型
				$this->getView()->assign('model_id',$model_id);
			}
			
		}

		/**
		 * 编辑字段
		 */
		public function editAction(){
			//获取模型id
			$model_id = isset($_GET['mid']) ? intval(trim($_GET['mid'])) : 0 ;
			$field_id = isset($_GET['fid']) ? intval(trim($_GET['fid'])) : 0 ;

			if(!$model_id || !$field_id){
				echo '<script>alert("非法请求");window.location.href="/admin/modeldata/index/";</script>';exit;
			}

			//判断操作
			if(trim($this->getRequest()->getPost('dosubmit',0))){
				//检测信息
				$info = $this->getRequest()->getPost('info',0);
				if(!$info){
					echo '<script>alert("请检查信息是否填写完整");window.history.go(-1);</script>';exit;
				}

				
				if(isset($info['field']) && trim($info['field'])){
					$info['field'] = trim($info['field']);
				}else{
					echo '<script>alert("请填写字段名称");window.history.go(-1);</script>';exit;
				}

				$oldfield = $info['field_old'];
				$info['maxlength'] = isset($info['maxlength']) && intval(trim($info['maxlength'])) ? intval(trim($info['maxlength'])) : 32;
				$info['date_type'] = isset($info['date_type']) ? $info['date_type'] : 1;
				$field_type = $this->get_fields_type($info['type_id'],$info['date_type']);

				$info['modelid'] 	= $model_id;
				$info['formtype'] 	= $field_type;
				$info['update_time'] = $_SERVER['time'];

				$param = array(
					'defaultvalue'	=> $info['defaultvalue'],
					'maxlength'		=> $info['maxlength'],
					'minnumber'		=> 1
				);

				unset($info['date_type']);
				unset($info['field_old']);
				unset($info['type_id']);

				//获取数据表名称
				$tablename = $this->modelfields->get_model_name($model_id);
				//执行修改操作
				$result_1 = $this->modelfields->edit_model_fields($info,$field_id);
				$result_2 = $this->deal_edit_fields($tablename,$field_type,$oldfield,$info['field'],$param);

				if($result_1 && $result_2){
					echo '<script>alert("操作成功");window.location.href="/admin/modelfields/index?mid='.$model_id.'";</script>';
				}else{
					echo '<script>alert("操作失败");window.location.href="/admin/modeldata/index/";</script>';
				}
				
			}else{
				//获取字段详情
				$fields_detail = $this->modelfields->get_fields_detail($field_id);
				$this->getView()->assign('field_id',$field_id);
				$this->getView()->assign('fields_detail',$fields_detail[0]);
				$this->getView()->assign('model_id',$model_id);
			}
		}

		/**
		 * 删除字段
		 */
		public function deleteAction(){
			$field_id = isset($_GET['fid']) ? intval(trim($_GET['fid'])) : 0 ;
			$model_id = isset($_GET['mid']) ? intval(trim($_GET['mid'])) : 0 ;

			if(!$field_id || !$model_id){
				echo '<script>alert("非法请求");window.history.go(-1);</script>';exit;
			}

			if($this->modelfields->delete_model_fields($field_id)){
				echo "<script>alert('操作成功');window.location.href='/admin/modelfields/index?mid=$model_id';</script>";
			}else{
				echo "<script>alert('操作失败');window.location.href='/admin/modelfields/index?mid=$model_id';</script>";
			}
		}

		/**
		 * 启用字段
		 */
		public function enableAction(){
			$field_id = isset($_GET['fid']) ? intval(trim($_GET['fid'])) : 0 ;
			$model_id = isset($_GET['mid']) ? intval(trim($_GET['mid'])) : 0 ;

			if(!$field_id || !$model_id){
				echo '<script>alert("非法请求");window.location.href="/admin/modeldata/index/";</script>';exit;
			}

			if($this->modelfields->enable_model_fields($field_id)){
				echo "<script>alert('操作成功');window.location.href='/admin/modelfields/index?mid=$model_id';</script>";
			}else{
				echo "<script>alert('操作失败');window.location.href='/admin/modelfields/index?mid=$model_id';</script>";
			}

		}

		/**
		 * 禁用字段
		 */
		public function disableAction(){
			$field_id = isset($_GET['fid']) ? intval(trim($_GET['fid'])) : 0 ;
			$model_id = isset($_GET['mid']) ? intval(trim($_GET['mid'])) : 0 ;

			if(!$field_id || !$model_id){
				echo '<script>alert("非法请求");window.location.href="/admin/modeldata/index/";</script>';exit;
			}

			if($this->modelfields->disable_model_fields($field_id)){
				echo "<script>alert('操作成功');window.location.href='/admin/modelfields/index?mid=$model_id';</script>";
			}else{
				echo "<script>alert('操作失败');window.location.href='/admin/modelfields/index?mid=$model_id';</script>";
			}
		}

		/**
		 * 处理添加字段相关操作
		 * @param string $talename  数据表名
		 * @param string $fieldtype 要添加字段的类型
		 * @param string $field     字段名
		 * @param array  $param 	其他参数array('defaultvalue'=>'默认值','maxlength'=>'最大长度','minnumber'=>'最小长度');
		 * @return int   影响的行数
		 */
		public function deal_add_fields($tablename,$fieldtype,$field,$param){
			$defaultvalue = isset($param['defaultvalue']) ? $param['defaultvalue'] : '';
			$maxlength = isset($param['maxlength']) ? intval(trim($param['maxlength'])) : 255;
			//正整数 UNSIGNED && SIGNED
			$minnumber = isset($param['minnumber']) ? intval(trim($param['minnumber'])) : 1;
			$sql = '';
			switch($fieldtype) {
				case 'varchar':
					$sql = "ALTER TABLE `$tablename` ADD `$field` VARCHAR( $maxlength ) NOT NULL DEFAULT '$defaultvalue'";
				break;
				case 'int':
					$minnumber = intval($minnumber);
					$defaultvalue = intval($defaultvalue);
					$sql = "ALTER TABLE `$tablename` ADD `$field` INT ".($minnumber >= 0 ? 'UNSIGNED' : '')." NOT NULL DEFAULT '$defaultvalue'";
				break;
				
				case 'text':
					$sql = "ALTER TABLE `$tablename` ADD `$field` TEXT NOT NULL";
				break;

				case 'date':
					$sql = "ALTER TABLE `$tablename` ADD `$field` DATE NULL";
				break;
				
				case 'datetime':
					$sql = "ALTER TABLE `$tablename` ADD `$field` DATETIME NULL";
				break;
				
				case 'timestamp':
					$sql = "ALTER TABLE `$tablename` ADD `$field` TIMESTAMP NOT NULL";
				break;
				
			}
			
			return $this->modelfields->deal_fields($sql);
		}



		/**
		 * 处理修改字段相关操作
		 * @param string $talename  数据表名
		 * @param string $fieldtype 要编辑的类型
		 * @param string $oldfield  要修改的字段名
		 * @param string $field     修改之后的字段
		 * @param array  $param 	其他参数array('defaultvalue'=>'默认值','maxlength'=>'最大长度','minnumber'=>'最小长度');
		 * @return int   影响的行数
		 */
		public function deal_edit_fields($tablename,$fieldtype,$oldfield,$field,$param){
			$defaultvalue = isset($param['defaultvalue']) ? $param['defaultvalue'] : '';
			$maxlength = isset($param['maxlength']) ? intval(trim($param['maxlength'])) : 255;
			$minnumber = isset($param['minnumber']) ? intval(trim($param['minnumber'])) : 1;
			$sql = '';
			switch($fieldtype) {
				case 'varchar':
					$sql = "ALTER TABLE `$tablename` CHANGE `$oldfield` `$field` $fieldtype( $maxlength ) NOT NULL DEFAULT '$defaultvalue'";
				break;

				case 'int':
					$defaultvalue = intval($defaultvalue);
					$sql ="ALTER TABLE `$tablename` CHANGE `$oldfield` `$field` INT ".($minnumber >= 0 ? 'UNSIGNED' : '')." NOT NULL DEFAULT '$defaultvalue'";
				break;

				case 'text':
					$sql = "ALTER TABLE `$tablename` CHANGE `$oldfield` `$field` TEXT NOT NULL";
				break;

				case 'date':
					$sql = "ALTER TABLE `$tablename` CHANGE `$oldfield` `$field` DATE NULL";
				break;
				
				case 'datetime':
					$sql = "ALTER TABLE `$tablename` CHANGE `$oldfield` `$field` DATETIME NULL";
				break;
				
				case 'timestamp':
					$sql = "ALTER TABLE `$tablename` CHANGE `$oldfield` `$field` TIMESTAMP NOT NULL";
				break;
			}

			return $this->modelfields->deal_fields($sql);
		}

		/**
		 * 获取字段类型
		 * @param int $typeid 类型id
		 * @return string 字段类型
		 */
		public function get_fields_type($typeid,$datetype = 1){
			switch ($typeid) {
					case 1:
						$field_type = 'varchar';
						break;
					case 2:
						$field_type = 'text';
						break;
					case 3:
						$field_type = $datetype == 1 ? 'timestamp' : 'date';
						break;
					case 4:
						$field_type = 'int';
						break;
					default:
						$field_type = 'varchar';
						break;
			}

			return $field_type;
		}
	}