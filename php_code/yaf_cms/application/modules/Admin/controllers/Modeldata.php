<?php if(!defined('APP_PATH')) exit('No direct script access allowed');
	/**
	 * 数据模型管理类
	 * @author guoxiaochuan maczhis@gmail.com
	 * @access public
	 * @version 1.0 2014/11/26
	 */
	Yaf_Loader::import(PATH_APP.'modules/Admin/controllers/Admin.php');
	Yaf_Loader::import(PATH_APP.'modules/Admin/models/Modeldata.php');
	class ModeldataController extends AdminController
	{	
		public function init(){
			parent::init();
			$this->modeldata = new ModeldataModel();
		}

		/**
		 * 显示模型列表
		 */
		public function indexAction(){
            $data = [];
            $page = $this->getRequest()->getQuery('page') ? $this->getRequest()->getQuery('page') : 1;

            //每页显示数
            $offset = 10;
            $start = ($page - 1) * $offset;
            $data['data'] = $model_list = $this->modeldata->get_model_list($start,$offset);

            //总页数
            $total = $this->modeldata->get_model_count();
            $totalpage = ceil($total / $offset);
            //超过一页显示分页
            if ($totalpage > 1) {
                $data['page'] = (new multi())->getSubContent('/admin/user/adminlist?page=', '', $totalpage, $page, 9, ' target="_self" ', '', '', 'no');
            }
            $this->getView()->assign('data', $data);

			$this->getView()->assign('model_list',$model_list);
		}

		/**
		 * 添加模型
		 */
		public function addAction(){
			//判断是否添加
			if(trim($this->getRequest()->getPost('model_submit',0))){
				//获取数据
				$model_name = trim($this->getRequest()->getPost('model_name',0));
				$model_tablename = trim($this->getRequest()->getPost('model_tablename',0));
				$model_description = trim($this->getRequest()->getPost('model_description',0));
				$model_status = trim($this->getRequest()->getPost('model_status',0));

				//判断数据是否正常
				if(!$model_name){
					echo '<script>alert("请填写模型名称！");window.history.go(-1);</script>';
					exit;
				}

				if(!$model_tablename){
					echo '<script>alert("请填写模型表名！");window.history.go(-1);</script>';
					exit;
				}

				if($this->modeldata->is_modelname_exists($model_tablename)){
					echo '<script>alert("模型表名已存在，请重新填写！");window.history.go(-1);</script>';
					exit;
				}

				//组合数据
				$data['name'] 			= $model_name;
				$data['description'] 	= $model_description;
				$data['tablename'] 		= $model_tablename;
				$data['status'] 		= $model_status;
				$data['create_time'] 	= $_SERVER['time'];

				//执行操作
				$model_id = $this->modeldata->add_model($data);
				$result = $this->create_model($model_tablename,$model_id);
				//将信息插入模型列表中
				if($model_id && $result){
					echo '<script>alert("操作成功");window.location.href="/admin/modeldata/index/";</script>';
				}else{
					echo '<script>alert("操作失败");window.location.href="/admin/modeldata/index/";</script>';
				}

			}else{
				//显示添加页面
				$this->getView();
			}

		}

		/**
		 * 编辑模型
		 */
		public function editAction(){
			//判断操作
			if(trim($this->getRequest()->getPost('model_submit',0))){
				//获取数据
				$model_id = intval(trim($this->getRequest()->getPost('model_id',0)));
				$model_name = trim($this->getRequest()->getPost('model_name',0));
				$model_tablename = trim($this->getRequest()->getPost('model_tablename',0));
				$model_description = trim($this->getRequest()->getPost('model_description',0));
				$model_status = trim($this->getRequest()->getPost('model_status',0));

				//判断数据是否正常
				if(!$model_id){
					echo '<script>alert("非法请求！");window.location.href="/admin/modeldata/index/";</script>';
					exit;
				}

				//组合数据
				$data['name'] = $model_name;
				$data['description'] = $model_description;
				$data['tablename'] = $model_tablename;
				$data['status'] = $model_status;
				$data['update_time'] = $_SERVER['time'];
				
				//执行操作
				if($this->modeldata->edit_model($data,$model_id)){
					echo '<script>alert("操作成功");window.location.href="/admin/modeldata/index/";</script>';
				}else{
					echo '<script>alert("操作失败");window.location.href="/admin/modeldata/index/";</script>';
				}

			}else{
				//获取模型id
				$model_id = isset($_GET['mid']) ? intval(trim($_GET['mid'])) : 0;
				if(!$model_id){
					echo '<script>alert("非法请求");window.location.href="/admin/modeldata/index/";</script>';
					exit;
				}

				//获取模型详情
				$model_detail = $this->modeldata->get_model_detail($model_id);
				if(!$model_detail){
					echo '<script>alert("未找到该条记录");window.location.href="/admin/modeldata/index/";</script>';
				}
				//显示编辑页面
				$this->getView()->assign('model_detail',$model_detail[0]);
			}
		}

		/**
		 * 删除模型
		 */
		public function deleteAction(){
			$modle_id = isset($_GET['mid']) ? intval(trim($_GET['mid'])) : 0;
			if(!$modle_id){
				echo '<script>alert("非法请求！");window.location.href="/admin/modeldata/index/";</script>';
				exit;
			}

			//获取该model_id的表名
			$result = $this->modeldata->get_modelname_by_id($modle_id);

			//删除数据表
			$result1 = $this->modeldata->drop_model_table($result[0]);

			//删除modelfield 下model_id字段
			$result2 = $this->modeldata->delete_model_fields($modle_id);

			if($this->modeldata->delete_model($modle_id) && ($result1 || $result2)){
				echo '<script>alert("操作成功");window.location.href="/admin/modeldata/index/";</script>';
			}else{
				echo '<script>alert("操作失败");window.location.href="/admin/modeldata/index/";</script>';
			}
		}

		/**
		 * 禁用模型
		 */
		public function disableAction(){
			$modle_id = isset($_GET['mid']) ? intval(trim($_GET['mid'])) : 0;
			if(!$modle_id){
				echo '<script>alert("非法请求！");window.location.href="/admin/modeldata/index/";</script>';
				exit;
			}

			if($this->modeldata->disable_model($modle_id)){
				echo '<script>alert("操作成功");window.location.href="/admin/modeldata/index/";</script>';
			}else{
				echo '<script>alert("操作失败");window.location.href="/admin/modeldata/index/";</script>';
			}
		}

		/**
		 * 启用模型
		 */
		public function enableAction(){
			$modle_id = isset($_GET['mid']) ? intval(trim($_GET['mid'])) : 0;
			if(!$modle_id){
				echo '<script>alert("非法请求！");window.location.href="/admin/modeldata/index/";</script>';
				exit;
			}

			if($this->modeldata->enable_model($modle_id)){
				echo '<script>alert("操作成功");window.location.href="/admin/modeldata/index/";</script>';
			}else{
				echo '<script>alert("操作失败");window.location.href="/admin/modeldata/index/";</script>';
			}
		}

		/**
		 * 创建模型
		 * @param string $tablename 要创建的表名
		 * @param int 	 $modelid   模型id
		 * @param string $table_model_field  替换之后的字段
		 * @return boolean 是否执行成功
		 */
		public function create_model($tablename,$modelid,$table_model_field = 'model_field'){
			//获取建表sql语句
			$model_sql = file_get_contents(APP_PATH."/shell/databases/createmodel.sql");
			//替换字符串
			$model_sql = str_replace('$basic_table', $tablename, $model_sql);
			$model_sql = str_replace('$table_model_field', $table_model_field,$model_sql);
			$model_sql = str_replace('$modelid', $modelid,$model_sql);

			//执行操作
			return $this->modeldata->create_model($model_sql);			
		}
	}