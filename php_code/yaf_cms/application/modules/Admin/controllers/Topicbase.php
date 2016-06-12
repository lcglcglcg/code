<?php
	if (!defined('APP_PATH')) exit('No direct script access allowed');

	Yaf_Loader::import(PATH_APP . 'modules/Admin/controllers/Admin.php');
	Yaf_Loader::import(PATH_APP . 'modules/Admin/models/Topic.php');
	Yaf_Loader::import(PATH_APP . '../library/Upload.php');
	

	/**
	 * 专题基类相关操作类
	 * @author maczhis@gmail.com
	 */
	class TopicbaseController extends AdminController
	{
		/**
		 * 初始化
		 */
		public function init(){
            parent::init();
            $this->topic        = new TopicModel();
            $this->referer      = isset($_SERVER["HTTP_REFERER"]) ? str_replace('http://' . $_SERVER['HTTP_HOST'], '', $_SERVER["HTTP_REFERER"]) : '/admin/index/index';

            //设置白名单（在白名单内不需要tid,否则需要提供tid
            //这样可以避免在专题相关的控制器和方法中再次判断 用户对专题权限
            $url_arr = array(
                    "/admin/topic/index",
                    "/admin/topic/index/",
                    "/admin/topicdata/index",
                    "/admin/topicdata/index/",
                    "/admin/topic/add/",
                    "/admin/topic/add",
                    "/admin/topicdata/trashindex",
                    "/admin/topicdata/trashindex/"
                );

            if(!in_array($_SERVER['REQUEST_URI'],$url_arr)){
                $tid = $this->getRequest()->getQuery("tid");

                //如果不存在tid 非法请求
                if(!$tid){
                    Alert::danger("非法请求");
                    Yaf_Controller_Abstract::redirect("/admin/topic/index/");
                    exit;
                }

                //根据tid查询当前用户是否是管理员或者是该专题的所有者，如果不是则提示权限不足
                if(!$this->checkrole($tid)){
                    Alert::danger("权限不足");
                    Yaf_Controller_Abstract::redirect("/admin/topic/index/");
                    exit;
                }
            }

            //判断是否是管理员 并赋值给模板
            $this->getView()->assign("is_admin",$this->is_admin());
        }

        /**
         * 检查专题下的允许的最大数量
         * @param intval $topic_id 栏目id
         * @param boolean true ：返回当月已发文章数
         * @return mx true：可以继续发文章  false 本月文章数量已满/当第二个参数为true时 返回当月文章数
         */
        public function checkmaxpost( $topic_id , $type = false) {
            //获取该专题配置信息（最大允许发文数量）
            $max_post = $this->topic->get("topic","max_post",["topic_id"=>$topic_id]);
            
            //统计topic_data中该专题的本月的发文数量
            $date_info = $this->getdate( "" , true );

            $count_topic_post = $this->topic->count( "topic_data" , [
                "AND" => [ 
                    "tid" => $topic_id , 
                    "inputtime[<>]" => [ 
                        $date_info["firstday"] , 
                        $date_info["lastday"] 
                    ]
                ]
            ]);

            if($type){
                return $count_topic_post;
            }

            //比较文章数量
            if($max_post > $count_topic_post){
                return true;
            }

            return false;
        }

        /**
         * 获取当前的月份和时间信息
         * @param string $date 日期
         * @param boolean $type 返回形式：$type=true:返回时间戳的形式，$type=false:返回日期形式
         * @return array 
         */
        public function getdate( $date = "" , $type = false ) {
            if ($date) {
                $firstday = date("Y-m-01",strtotime($date));
            }else{
                $firstday = date("Y-m-01",time());
            }
            
            $lastday = date("Y-m-d",strtotime("$firstday+1 month-1 day"));
            
            if ($type) {
                return array('firstday'=>strtotime($firstday),'lastday'=>strtotime($lastday));
            }

            return array('firstday'=>$firstday,'lastday'=>$lastday);
        }

        /**
		 *判断当前用户数是否有权限执行该操作（admin）
		 * @param intval $id 专题id
		 * @return boolean true:有权限 false：没有权限
		 */
		public function checkrole($id,$type = "topic"){
            if($type == "topic"){
                $userid = $this->topic->get("topic","topic_userid",["topic_id"=>$id,"LIMIT"=>1]);
            }else{
                $userid = $this->topic->get($type,"userid",["id"=>$id,"LIMIT"=>1]);
            }

			if($this->is_admin() || $this->_user['uid'] == $userid){
				return true;
			}else{
				return false;
			}
		}

		/**
		 * 专题子类树结构
		 */
		public function tree(&$list,$pid=0,$level=0,$html='─'){
		    static $tree = array();

		    foreach($list as $v){
		        if($v['parent_id'] == $pid){
		            $v['title'] = $level ? '└'.str_repeat($html,$level).$v['title'] :str_repeat($html,$level).$v['title'] ;
		            $tree[] = $v;
		            $this->tree($list,$v['id'],$level+1);
		        } 
		    }

		    return $tree;
		}

        public function dump($max){
            echo '<pre>';
            var_dump($max);
            echo '</pre>';
        }
	}

?>