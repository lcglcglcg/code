<?php
	if (!defined('APP_PATH')) exit('No direct script access allowed');

	Yaf_Loader::import(PATH_APP.'modules/Admin/controllers/Topicbase.php');

	/**
	 * 专题操作类
	 */
	class TopicController extends TopicbaseController
	{

		public function init(){
			parent::init();
		}

		/**
		 * 专题列表
		 */
		public function indexAction(){
			//分页
			$data = [];
	        $page = $this->getRequest()->getQuery('page') ? $this->getRequest()->getQuery('page') : 1;
	        $offset = 10;
	        $start = ($page - 1) * $offset;
	        $where = array();
	        //判断是否是管理员
	        if(!$this->is_admin()){
	        	$where['topic_userid'] = (int)$this->_user['uid'];
	        }

	        $where['ORDER'] = 'order_id DESC';
	        $where['LIMIT'] = [$start,$offset];
	       	
	        //获取数据
	        $data['data'] = $this->topic->select('topic','*',$where);
	        //获取专题本月已发文章数
	        $time = $this->getdate("",true);
	        $data['data'] = $this->topic->count_topicdata_by_month($data['data'],$time);
	        
	        //统计数据量
	        $total = $this->topic->count('topic',[$this->_user['uid']]);
	        //获取总页数
	        $totalpage = ceil($total / $offset);
	        
	        //超过一页显示分页
	        if ($totalpage > 1) {
	            $data['page'] = (new multi())->getSubContent('/admin/topic/index?page=', '', $totalpage, $page, 9, ' target="_self" ', '', '', 'no');
	        }

			$this->getView()->assign('data',$data);
		}

		/**
		 * 添加专题
		 */
		public function addAction(){
			//判断是否添加专题
			if($this->getRequest()->getPost('dosubmit',0)){
				$topic = $this->getRequest()->getPost('topic',0);
				
				//检测topic_path 是否存在
				if($this->topic->is_exists('topic','topic_path',['topic_path'=>$topic['topic_path']])){
					Alert::danger("目录名已存在");
					Yaf_Controller_Abstract::redirect($this->referer);
					exit;
				}

				//如果不是admin 无法添加和修改专题文章数
                if(!$this->is_admin()){
                    unset($topic['max_post']);
                }
                
				$topic['create_time'] = $_SERVER['time'];
				
				//添加专题时可以选择把专题分配给一个指定的（专题）用户
				if($topic['topic_userid'] == 0){
					$topic['topic_username'] 	= $this->_user['username'];
					$topic['topic_userid'] 		= $this->_user['uid'];
				}else{
					$topic['topic_username'] 	= $this->topic->get("admin","username",
						["id"=>$topic["topic_userid"]]);
				}
				
				//判断是否存在模板信息
				$topic['topic_template'] = isset($topic['topic_template']) && $topic['topic_template'] ? $topic['topic_template'] : 'index.phtml';

				//添加信息
				if($topic_id = $this->topic->add('topic',$topic)){
					Alert::success("操作成功");
					Yaf_Controller_Abstract::redirect("/admin/topic/index/");
				}else{
                                    echo $this->topic->get_last_query();die;
					Alert::danger("操作失败");
					Yaf_Controller_Abstract::redirect("/admin/topic/index/");
				}

				exit;

			}else{
				//获取专题用户列表
				$userlist = $this->topic->select("admin",["id","username"],["AND"=>["roleid"=>13,"status"=>1]]);
				
				$this->getView()->assign("userlist",$userlist);
			}
			
		}

		/**
		 * 删除专题
		 */
		public function deleteAction(){
			$topic_id = $this->getRequest()->getQuery('tid');

			if(!$topic_id){
				Alert::danger('非法请求');
				exit;
			}

			// 判断当期操作者是否是管理员或者 是 专题创建者
			if(!$this->checkrole($topic_id)){
				Alert::danger('权限不够');
				exit;
			}

			// 获取path
			// $topic_path = $this->topic->get_topic('topic','topic_path',['topic_id'=>$topic_id]);

			// $file_path = APP_PATH.'/kele/topic/'.$topic_path;
			
			// //判断是否存在path文件目录
			// if($topic_path && file_exists($file_path)){

			// 	$this->delete_files($file_path);
			// }

			//删除相关记录
			if($this->topic->delete('topic',['topic_id'=>$topic_id])){
				Alert::success("操作成功");
			}else{
				Alert::danger("操作失败");
			}

			Yaf_Controller_Abstract::redirect("/admin/topic/index");
		}

		/**
		 * 编辑专题
		 */
		public function editAction(){
			$topic_id = $this->getRequest()->getQuery('tid');
			if(!$topic_id){
				Alert::danger('非法请求');
				Yaf_Controller_Abstract::redirect("/admin/topic/index/");
                exit;
			}
			
			//判断当前用户是否为管理员 或者是专题创建者
			if(!$this->checkrole($topic_id)){
				Alert::danger('权限不够');
				Yaf_Controller_Abstract::redirect("/admin/topic/index/");
				exit;
			}

			//是否更新专题
			if($this->getRequest()->getPost('dosubmit',0)){
				$data = $this->getRequest()->getPost('topic',0);
				
				if($data){
					//判断是否存在模板信息
					$data['topic_template'] = isset($data['topic_template']) && $data['topic_template'] ? $data['topic_template'] : 'index.phtml';
					//判断是否要重新给专题分配用户
					if($this->is_admin() && isset($data['topic_userid'])){
						if(0 == (int)$data['topic_userid']){
							$data['topic_username'] 	= $this->_user['username'];
							$data['topic_userid'] 		= $this->_user['uid'];
						}else{
							
							$data['topic_username'] 	= $this->topic->get("admin","username",["id"=>$data["topic_userid"]]);
						}

						// 分配专题给用户的时候 ,会把该专题下的所有的栏目，子栏目 ，和文章的 userid 都批量修改为所分配的用户的id
						if($data['topic_username'] && $data['topic_userid']){
							$this->topic->update('topic_subclass',[
								'username'	=>$data['topic_username'],
								'userid'	=>$data['topic_userid'],
								'update_time'=>$_SERVER['time']
								],['tid'=>$topic_id]);
							
							$this->topic->update('topic_data',[
								'username'	=>$data['topic_username'],
								'userid'	=>$data['topic_userid'],
								'updatetime'=>$_SERVER['time']
								],['tid'=>$topic_id]);
						}
					}

					$data['update_time'] = $_SERVER['time'];
					//不允许修改topi_path
					if(isset($data['topic_path'])){
						unset($data['topic_path']);
					}
					
					//判断是否成功修改信息
					if($this->topic->update('topic',$data,['topic_id'=>$topic_id])){
						Alert::success("操作成功");
					}else{
						Alert::danger("操作失败");	
					}

					Yaf_Controller_Abstract::redirect("/admin/topic/index");
					exit;
				}
			}else{
				//获取专题用户列表
				$userlist = $this->topic->select("admin",["id","username"],["AND"=>["roleid"=>88,"status"=>1]]);
				$data = $this->topic->get('topic','*',['topic_id'=>$topic_id]);
				$this->getView()->assign('data',$data);
				$this->getView()->assign('userlist',$userlist);
			}
		}

		public function showdataAction(){
			$tid = $this->getRequest()->getQuery("tid");
			if(!$tid){
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

				//判断数据是否正常
				$show_data = json_encode($data);
				// echo $show_data;
				// exit;
				if($this->topic->update("topic",["show_data"=>$show_data,"update_time"=>$_SERVER['time']],["topic_id"=>$tid])){
					Alert::success("操作成功");
				}else{
					Alert::danger("操作失败");
				}

				Yaf_Controller_Abstract::redirect("/admin/topic/index");
				exit;

			}else{
				$show_data = $this->topic->get("topic","show_data",["topic_id"=>$tid,"LIMIT"=>1]);
				$data = json_decode($show_data,true);
				$data['title'] = array('zxwz'=>'最新文章','rdwz'=>'热点文章','tjwz'=>'推荐文章','jhwz'=>'精华文章');
				$data['tid'] = $tid;
				//获取子类列表

				$subclass_list = $this->topic->select('topic_subclass',['title','id','parent_id','path','title'],
					[
						"tid"	=>$tid
					]);
				$data['list'] = $this->tree($subclass_list);
				$this->getView()->assign("data",$data);
			}
		}

		

		/**
		 * 改变专题状态
		 */
		public function changeTopicStatusAction(){
			(int)$topic_id = $this->getRequest()->getQuery('tid');
			$data['is_show'] = $this->getRequest()->getQuery('isshow');

			if(!$topic_id){
				Alert::danger("非法请求");
				Yaf_Controller_Abstract::redirect($this->referer);
				exit;
			}

			//判断当前操作者是否具有权限
			if(!$this->checkrole($topic_id)){
				Alert::danger("权限错误");
				Yaf_Controller_Abstract::redirect($this->referer);
				exit;
				
			}

			if($this->topic->update('topic',$data,['topic_id'=>$topic_id])){
				Alert::success("操作成功");
				Yaf_Controller_Abstract::redirect($this->referer);
			}else{
				Alert::danger("操作失败");
				Yaf_Controller_Abstract::redirect($this->referer);
			}
			exit;
		}

		/**
		 * 生成专题文件夹
		 */
		public function create_file($topic_path){
			$file_path = APP_PATH.'/kele/topic/'.$topic_path;

			if(file_exists($file_path)){
				return false;
			}else{
				if(mkdir($file_path,0775) && mkdir($file_path.'/static',0775)){
					return true;
				}else{
					return false;
				}
			}
		}

		/**
		 * 生成专题页面
		 */
		public function create_page($topic_path,$template_name){
			$template_path = APP_PATH."/kele/template/".$template_name;
			
			$topic_path = APP_PATH."/kele/topic/".$topic_path."/".$template_name;

			if(copy($template_path,$topic_path)){
				return true;
			}else{
				return false;
			}
		}
	}
?>