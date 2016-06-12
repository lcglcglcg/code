<?php
	if (!defined('APP_PATH')) exit('No direct script access allowed');

	Yaf_Loader::import(PATH_APP . 'modules/Admin/controllers/Topicbase.php');

	/**
	 * 专题子类相关操作类
	 */
	class TopicsubclassController extends TopicbaseController{

		public function init(){
			parent::init();
		}

		/**
		 * 获取专题子类
		 */
		public function indexAction(){
			$topic_id =  $this->getRequest()->getQuery('tid');
			if(!$topic_id){
				Alert::danger("非法请求");
				Yaf_Controller_Abstract::redirect($this->referer);
				exit;
			}

			$data = [];
	        $page = $this->getRequest()->getQuery('page') ? $this->getRequest()->getQuery('page') : 1;

	        $offset = 10;
	        $start = ($page - 1) * $offset;
	        $where_count = $where = array();
	        
	        if(!$this->is_admin()){
                    $where_count['AND']['userid'] = $where['AND']['userid'] = $this->_user['uid'];
	        }

	        $where_count['AND']['tid'] = $where['AND']['tid'] = $topic_id;
	        $where['ORDER'] = 'order_id DESC';
	        $where['LIMIT'] = [$start,$offset];

	        $data['data'] = $this->topic->select('topic_subclass','*',$where);
	        unset($where);

	        //获取总数
	        $total = $this->topic->count('topic_subclass',$where_count);
	        $totalpage = ceil($total / $offset);
	        //超过一页显示分页
	        if ($totalpage > 1) {
	            $data['page'] = (new multi())->getSubContent('/admin/topicsubclass/index?tid='. $topic_id .'&page=', '', $totalpage, $page, 9, ' target="_self" ', '', '', 'no');
	        }
	        $this->getView()->assign('topic_id',$topic_id);
			$this->getView()->assign('data',$data);
		}


		/**
		 * 添加专题子类
		 */
		public function addAction(){
			$topic_id = $this->getRequest()->getQuery('tid');

			if(!$topic_id){
				Alert::danger("非法请求");
				Yaf_Controller_Abstract::redirect($this->referer);
				exit;
			}

			$this->getView()->assign('topic_id',$topic_id);

			// 判断该专题是否所有者
			if(!$this->checkrole($topic_id)){
				Alert::danger("权限错误");
				Yaf_Controller_Abstract::redirect($this->referer);
				exit;
			}

			if($this->getRequest()->getPost('dosubmit',0)){
				$data = $this->getRequest()->getPost('subclass',0);
				if(!$data){
					Alert::danger("非法请求");
					Yaf_Controller_Abstract::redirect("/admin/topic/index/");
					exit;
				}

				$data['username'] 	= $this->_user['username'];
				$data['userid'] 	= $this->_user['uid'];
				$data['create_time'] = $_SERVER['time'];
				$data['show_data']	= '{"base":{"tjwz":{"num":"10","status":"0","isrand":"0"},"jhwz":{"num":"10","status":"1","isrand":"0"},"zxwz":{"num":"1","status":"1","isrand":"0"}},"setting":{"prepage":"100","template":"list.phtml"}}';
				$data['content_data'] = '{"base":{"tjwz":{"num":"10","status":"0","isrand":"0"},"jhwz":{"num":"10","status":"1","isrand":"0"},"zxwz":{"num":"1","status":"1","isrand":"0"}},"setting":{"template":"content.phtml"}}';

				if($this->topic->add('topic_subclass',$data)){
					Alert::success("操作成功");
				}else{
					Alert::danger("操作失败");
				}

				Yaf_Controller_Abstract::redirect("/admin/topicsubclass/index?tid=".$topic_id);exit;

			}else{
				if(!$this->is_admin()){
					$where['AND']['userid'] = $this->_user['uid'];
					$where['AND']['tid'] = $topic_id;
				}else{
					$where['tid'] = $topic_id;
				}

				//获取子类列表
				$subclass_list = $this->topic->select('topic_subclass',['title','id','parent_id'],$where);

				//根据parent_id 排序
				$data['list'] = $this->tree($subclass_list);
				$this->getView()->assign('data',$data);
			}
		}

		/**
		 * 编辑专题子类
		 */
		public function editAction(){
			$sid 		= $this->getRequest()->getQuery('sid');
			$topic_id 	= $this->getRequest()->getQuery('tid');

			if(!$sid){
				Alert::danger("非法请求");
				Yaf_Controller_Abstract::redirect($this->referer);
			}

			//判断当前操作者是否具有权限
			if(!$this->checkrole($sid,"topic_subclass")){
				Alert::danger("权限错误");
				Yaf_Controller_Abstract::redirect($this->referer);
				exit;
			}

			$this->getView()->assign('tid',$topic_id);
			$this->getView()->assign('sid',$sid);


			if($this->getRequest()->getPost('dosubmit',0)){
				$data = $this->getRequest()->getPost('subclass',0);
				if(!$data){
					Alert::danger("非法请求");
					Yaf_Controller_Abstract::redirect($this->referer);
					exit;
				}

				$data['update_time'] = $_SERVER['time'];
				if($this->topic->update('topic_subclass',$data,['id'=>$sid])){
					Alert::success("操作成功");
				}else{
					Alert::danger("操作失败");
				}

				Yaf_Controller_Abstract::redirect("/admin/topicsubclass/index?tid=".$data['tid']);exit;

			}else{
				//根据当前用户的角色不同 拼接不用的查询条件
				if(!$this->is_admin()){
					$where['AND']['userid'] = $this->_user['uid'];
					$where['AND']['id'] 	= $sid;
				}else{
					$where['id'] 			= $sid;
				}

				$data = $this->topic->get('topic_subclass','*',$where);
				unset($where['id']);
				$where['tid'] = $topic_id;
				//获取子类的show_data
				$show_data = json_decode($data['show_data'],true);
				$data['subclass_template'] = $show_data['setting']['template'] ? $show_data['setting']['template'] : 'list.phtml';
				$data['subclass_prepage'] = $show_data['setting']['prepage'] ? (int)$show_data['setting']['prepage']:100;

				//获取子类下各个页面的content_data
				$content_data = json_decode($data['content_data'],true);
				$data['content_template'] = $content_data['setting']['template'] ? $content_data['setting']['template'] : 'content.phtml';
				

				//获取子类列表
				$subclass_list = $this->topic->select('topic_subclass',['title','id','parent_id'],$where);
				$data['list'] = $this->tree($subclass_list);
				$this->getView()->assign('data',$data);
			}
			
		}
		
		/**
		 * 删除专题子类
		 */
		public function deleteAction(){
			$subclass_id = $this->getRequest()->getQuery('sid');
			$topic_id = $this->getRequest()->getQuery('tid');

			if(!$subclass_id || !$topic_id){
				Alert::danger("非法请求");
				Yaf_Controller_Abstract::redirect($this->referer);
				exit;
			}

			//判断当前操作者是否具有权限
			if(!$this->checkrole($subclass_id,"topic_subclass")){
				Alert::danger("权限错误");
				Yaf_Controller_Abstract::redirect($this->referer);
				exit;
			}

			if($this->topic->delete('topic_subclass',['id'=>$subclass_id])){
				Alert::success("操作成功");
			}else{
				Alert::danger("操作失败");
			}
			Yaf_Controller_Abstract::redirect("/admin/topicsubclass/index?tid=".$topic_id);
			exit;
		}

		/**
		 * 专题子类页面显示的数据
		 */
		public function showdataAction(){
			//获取参数
			$tid = $this->getRequest()->getQuery("tid");
			$sid = $this->getRequest()->getQuery("sid");
			
			if(!($tid && $sid)){
				Alert::danger("非法请求");
				Yaf_Controller_Abstract::redirect($this->referer);
				exit;
			}

			if($this->getRequest()->getPost("dosubmit",0)){
				$data = $this->getRequest()->getPost("show_data",0);
				if(!$data){
					Alert::danger("数据错误");
					Yaf_Controller_Abstract::redirect($this->referer);
					exit;
				}
				
				//获取模板信息和列表页每一页显示数量
				$data['setting']['template'] = 'list.phtml';
				$data['setting']['prepage'] = 100;
				
				//数据过滤 清除掉没有数据的
				foreach($data['data'] as $key=>$value){
					if($value){	
						foreach($value as $k=>$v){	
							if(empty($v)){
								unset($value[$k]);
								unset($data['data'][$key]);
							}
						}
					}
				}

				//判断数据是否正常
				$show_data = json_encode($data);
				if($this->topic->update("topic_subclass",["show_data"=>$show_data,"update_time"=>$_SERVER['time']],["tid"=>$tid])){
					Alert::success("操作成功");
				}else{
					Alert::danger("操作失败");
				}

				Yaf_Controller_Abstract::redirect("/admin/topicsubclass/index?tid=$tid");
				exit;

			}else{
				//获取配置信息
				$show_data = $this->topic->get("topic_subclass","show_data",["id"=>$sid,"LIMIT"=>1]);
				$data = json_decode($show_data,true);
				$data['title'] = array('zxwz'=>'最新文章','rdwz'=>'热点文章','tjwz'=>'推荐文章','jhwz'=>'精华文章','xgwz'=>'相关文章');
				$data['tid'] = $tid;
				$data['sid'] = $sid;
				
				//获取子类列表
				$subclass_list = $this->topic->select('topic_subclass',['title','id','parent_id','path','title'],["tid"=>$tid]);
				$data['list'] = $this->tree($subclass_list);
				$this->getView()->assign("data",$data);
			}
		}

		/**
		 * 内容页显示数据（设置页面）
		 */
		public function contentdataAction(){
			$tid = $this->getRequest()->getQuery("tid");
			$sid = $this->getRequest()->getQuery("sid");

			if(!($tid && $sid)){
				Alert::danger("非法请求");
				Yaf_Controller_Abstract::redirect($this->referer);
				exit;
			}

			if($this->getRequest()->getPost("dosubmit",0)){
				$data = $this->getRequest()->getPost("show_data",0);
				if(!$data){
					Alert::danger("数据错误");
					Yaf_Controller_Abstract::redirect($this->referer);
					exit;
				}

				//设置模板信息
				$data['setting']['template'] = 'content.phtml';

				//数据过滤 清除掉没有数据的
				foreach($data['data'] as $key=>$value){
					if($value){	
						foreach($value as $k=>$v){	
							if(empty($v)){
								unset($value[$k]);
								unset($data['data'][$key]);
							}
						}
					}
				}

				//判断数据是否正常
				$content_data = json_encode($data);
				if($this->topic->update("topic_subclass",["content_data"=>$content_data,"update_time"=>$_SERVER['time']],["tid"=>$tid])){
					Alert::success("操作成功");
				}else{
					Alert::danger("操作失败");
				}

				Yaf_Controller_Abstract::redirect("/admin/topicsubclass/index?tid=$tid");
				exit;

			}else{
				//显示内容数据页
				$content_data = $this->topic->get("topic_subclass","content_data",["id"=>$sid,"LIMIT"=>1]);
				$data = json_decode($content_data,true);
				$data['title'] = array('zxwz'=>'最新文章','rdwz'=>'热点文章','tjwz'=>'推荐文章','jhwz'=>'精华文章','xgwz'=>'相关文章');

				$data['tid'] = $tid;
				$data['sid'] = $sid;
				
				//获取子类列表
				$subclass_list = $this->topic->select('topic_subclass',['title','id','parent_id','path'],
					[
						"tid"	=>$tid
					]);
				$data['list'] = $this->tree($subclass_list);
				$this->getView()->assign("data",$data);
			}
		}

		/**
		 * 改变子类的状态
		 */
		public function changeSubStatus(){
			$subclass_id = $this->getRequest()->getQuery('sid');
			if(!$subclass_id){
				Alert::danger("非法请求");
				Yaf_Controller_Abstract::redirect($this->referer);
				exit;
			}

			//判断当前操作者是否具有权限
			if(!$this->checkrole($subclass_id,"topic_subclass")){
				Alert::danger("权限出错");
				Yaf_Controller_Abstract::redirect($this->referer);
				exit;
			}


			$data['status'] = $this->getRequest()->getQuery('status');
			if($this->topic->update_subclass($subclass_id,$data)){
				Alert::success("权限出错");
			}else{
				Alert::danger("权限出错");
			}

			Yaf_Controller_Abstract::redirect("/admin/topicsubclass/index/");exit;
		}

	}
?>