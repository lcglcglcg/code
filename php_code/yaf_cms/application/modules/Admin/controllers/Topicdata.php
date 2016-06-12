<?php

if (!defined('APP_PATH'))
    exit('No direct script access allowed');

Yaf_Loader::import(PATH_APP . 'modules/Admin/controllers/Topicbase.php');

/**
 * 专题文章相关操作类
 */
class TopicdataController extends TopicbaseController {

    public function init() {
        parent::init();
    }

    /**
     * 专题文章列表
     */
    public function indexAction() {
        //获取参数
        $page   = isset($_REQUEST['page']) && intval($_REQUEST['page']) ? (int)$_REQUEST['page'] : 1 ;  // 页码id
        $tid    = isset($_REQUEST['tid']) && intval($_REQUEST['tid']) ? (int)$_REQUEST['tid'] : 0 ;
        $sid    = isset($_REQUEST['sid']) && intval($_REQUEST['sid']) ? (int)$_REQUEST['sid'] : 0 ;    // 
        $id     = isset($_REQUEST['id']) && intval($_REQUEST['id']) ? (int)$_REQUEST['sid'] : 0 ;
        $title  = isset($_REQUEST['title']) && !empty($_REQUEST['title']) ? trim($_REQUEST['title']) : '' ;

        $data   = [];
        $offset = 10;
        $start  = ($page - 1) * $offset;

        //拼接where 查询条件
        $where['AND']['status'] = 1;
        
        //判断当前用户对该专题是否有操作权限
        if(!$this->is_admin()){

            $topic_list = $this->topic->select('topic',['topic_id','topic_name'],['topic_userid'=>$this->_user['uid']]);
            
            $where['userid'] = $this->_user['uid'];

        } else {

            $topic_list = $this->topic->select('topic',['topic_id','topic_name'],['topic_id[>]'=>0]);
        }

        //根据不同的条件来查询专题文章
        if($tid){
            $where['AND']['tid'] = $tid;
        }

        if($sid){
            $where['AND']['sid'] = $sid;
        }

        if($id){
            $where['AND']['id'] = $id;
        }

        if($title){
            $where['LIKE']['title'] = $title; 
        }



        $where['ORDER'] = 'id DESC';
        $where['LIMIT'] = [$start,$offset];

        $data['data'] = $this->topic->select('topic_data','*',$where);

        //获取专题path
        foreach ($data['data'] as $k=>$v) {
            $data['data'][$k]['topic_path'] = $this->topic->get("topic", "topic_path", ["topic_id" => $v['tid'], "LIMIT" => 1]);
        }

        //统计文章总数
        unset($where['ORDER']);
        unset($where['LIMIT']);
        $total = $this->topic->count('topic_data',$where);
        $totalpage = ceil($total / $offset);

        //超过一页显示分页
        if ($totalpage > 1) {
            $data['page'] = (new multi())->getSubContent('/admin/topicdata/index?tid='. $tid .'&sid='.$sid.'&page=', '', $totalpage, $page, 9, ' target="_self" ', '', '', 'no');
        }

        unset($where);

        //获取当期用户所有的栏目
        if (!$this->is_admin()) {
            $where['userid'] = $this->_user['uid'];
        }

        $where['ORDER'] = ["parent_id ASC", "id ASC"];

        $subclass_list = $this->topic->select('topic_subclass', ['id', 'title','path'], $where);
        foreach($subclass_list as $k=>$v){
            $subclass[$v['id']] = $v;
        }

        //将参数赋值给模板
        $this->getView()->assign('page',$page);
        $this->getView()->assign('tid',$tid);
        $this->getView()->assign('sid',$sid);
        $this->getView()->assign('id',$id);
        $this->getView()->assign('subclass',$subclass);
        $this->getView()->assign('subclass_list', $subclass_list);
        $this->getView()->assign('topic_list',$topic_list);     
        $this->getView()->assign('data', $data); 
    }

    /**
     * 添加专题文章
     */
    public function addAction() {
        if ($this->getRequest()->getPost('dosubmit', 0)) {

            $data = $this->getRequest()->getPost('topicdata', 0);

            if ($data && $data['sid'] && $data['tid']) {
                //判断文章数量是否超过设置数
                if (!$this->checkmaxpost($data['tid'])) {
                    Alert::danger("本月文章数量已发完");
                    Yaf_Controller_Abstract::redirect("/admin/topicsubclass/index?tid=" . $data['tid']);
                    exit;
                }

                $data['userid'] = $this->_user['uid'];
                $data['username'] = $this->_user['username'];
                $data['inputtime'] = $_SERVER['time'];

                if ($this->topic->add("topic_data", $data)) {
                    Alert::success("操作成功");
                    Yaf_Controller_Abstract::redirect("/admin/topicsubclass/index?tid=" . $data['tid']);
                } else {
                    Alert::danger("操作失败");
                    Yaf_Controller_Abstract::redirect($this->referer);
                }
            } else {
                Alert::danger("非法请求");
                Yaf_Controller_Abstract::redirect($this->referer);
            }

            exit;
        } else {
            $sid = $this->getRequest()->getQuery('sid');
            if (!$sid) {
                Alert::danger("非法请求");
                Yaf_Controller_Abstract::redirect($this->referer);
                exit;
            }

            $tid = $this->getRequest()->getQuery('tid');
            $this->getView()->assign('tid', $tid);
            $this->getView()->assign('sid', $sid);
        }
    }

    /**
     * 编辑专题文章
     */
    public function editAction() {
        $id = $this->getRequest()->getQuery('id');
        if (!$id) {
            Alert::danger("非法请求");
            Yaf_Controller_Abstract::redirect($this->referer);
            exit;
        }

        if ($this->getRequest()->getPost('dosubmit', 0)) {
            $data = $this->getRequest()->getPost('topicdata', 0);

            if ($data) {
                $data['updatetime'] = $_SERVER['time'];
                $tid = $data['tid'];
                unset($data['tid']);

                $where['id'] = $id;
                if ($this->topic->update("topic_data", $data, $where)) {
                    Alert::success("操作成功");
                    Yaf_Controller_Abstract::redirect("/admin/topicdata/index");
                } else {
                    Alert::danger("操作失败");
                    Yaf_Controller_Abstract::redirect($this->referer);
                }

                exit;
            }
        } else {
            $where['AND']['id'] = $id;
            $where['AND']['status'] = 1;
            $data = $this->topic->get("topic_data", "*", $where);
            $this->getView()->assign('data', $data);
            $this->getView()->assign('id', $id);
        }
    }

    /**
     * 删除专题文章
     */
    public function deleteAction() {
        $id = $this->getRequest()->getQuery('id');
        if (!$id) {
            Alert::danger("非法请求");
            Yaf_Controller_Abstract::redirect($this->referer);
            exit;
        }

        //判断权限
        if (!$this->checkrole($id, "topic_data")) {
            Alert::danger("权限不足");
            Yaf_Controller_Abstract::redirect($this->referer);
            exit;
        }

        //非管理员不能直接删除数据，否则会引起限制发布文章的数量错误
        if ($this->is_admin) {
            $result = $this->topic->delete("topic_data", ['id' => $id]);
        } else {
            $data['status'] = 4;
            $data['updatetime'] = $_SERVER['time'];
            $result = $this->topic->update("topic_data", $data, ['id' => $id]);
        }

        if ($result) {
            Alert::success("操作成功");
            Yaf_Controller_Abstract::redirect("/admin/topicdata/index?tid=" . $tid);
        } else {
            Alert::danger("操作失败");
            Yaf_Controller_Abstract::redirect($this->referer);
        }

        exit;
    }

    /**
     * 将专题文章放入回收站
     */
    public function trashAction() {

        $id = $this->getRequest()->getQuery('id');
        if (!$id) {
            Alert::danger("非法请求");
            Yaf_Controller_Abstract::redirect($this->referer);
            exit;
        }

        //判断权限
        if (!$this->checkrole($id, "topic_data")) {
            Alert::danger("权限不足");
            Yaf_Controller_Abstract::redirect($this->referer);
            exit;
        }

        if ($this->topic->update("topic_data", ["status" => 5], ['id' => $id])) {
            Alert::success("操作成功");
            Yaf_Controller_Abstract::redirect("/admin/topicdata/index");
        } else {
            Alert::danger("操作失败");
            Yaf_Controller_Abstract::redirect($this->referer);
        }
        exit;
    }

    /**
     * 还原专题文章
     */
    public function reductionAction() {
        $id = $this->getRequest()->getQuery('id');
        if (!$id) {
            Alert::danger("非法请求");
            Yaf_Controller_Abstract::redirect($this->referer);
            exit;
        }

        //判断权限
        if (!$this->checkrole($id, "topic_data")) {
            Alert::danger("权限不足");
            Yaf_Controller_Abstract::redirect($this->referer);
            exit;
        }

        if ($this->topic->update("topic_data", ["status" => 1], ['id' => $id])) {
            Alert::success("操作成功");
            Yaf_Controller_Abstract::redirect("/admin/topicdata/trashindex");
        } else {
            Alert::danger("操作失败");
            Yaf_Controller_Abstract::redirect($this->referer);
        }
        exit;
    }

    /**
     * 专题文章回收站
     */
    public function trashindexAction() {
        $page = $this->getRequest()->getQuery('page') ? $this->getRequest()->getQuery('page') : 1;
        $data = [];
        $offset = 10;
        $start = ($page - 1) * $offset;

        $where = '';
        if (!$this->is_admin()) {
            $where .= "userid=" . $this->_user['uid'];
            $where .= " AND status=5";
        } else {
            $where .= "status=5";
        }


        if ($this->getRequest()->getPost('sid')) {
            $where .= " AND sid=" . $this->getRequest()->getPost('sid');
            $this->getView()->assign('sid', $this->getRequest()->getPost('sid'));
        } elseif ($this->getRequest()->getQuery('sid')) {
            $where .= " AND sid=" . $this->getRequest()->getQuery('sid');
            $this->getView()->assign('sid', $this->getRequest()->getQuery('sid'));
        }

        //判断是否有搜索条件
        if ($this->getRequest()->getPost('id', 0)) {
            $where .= " AND id=" . $this->getRequest()->getPost('id');
            $this->getView()->assign('id', $this->getRequest()->getPost('id'));
        }

        if ($this->getRequest()->getPost('title', 0)) {
            $where .= " AND title LIKE '%" . $this->getRequest()->getPost('title');
            $where .= "%'";
            $this->getView()->assign('title', $this->getRequest()->getPost('title'));
        }

        $where2 = $where;
        $where .= " ORDER BY id DESC";
        $where .= " LIMIT $start,$offset";
        $sql = "SELECT * FROM topic_data WHERE $where";
        $data['data'] = $this->topic->query($sql);

        //获取专题path
        foreach ($data['data'] as &$v) {
            $v['topic_path'] = $this->topic->get("topic", "topic_path", ["topic_id" => $v['tid'], "LIMIT" => 1]);
        }

        $result = $this->topic->query("SELECT COUNT(id) total FROM topic_data WHERE $where2");
        $total = $result['total'];

        $totalpage = ceil($total / $offset);

        //超过一页显示分页
        if ($totalpage > 1) {
            $data['page'] = (new multi())->getSubContent('/admin/topicdata/trashindex?page=', '', $totalpage, $page, 9, ' target="_self" ', '', '', 'no');
        }

        unset($where);
        //获取当期用户所有的栏目
        if (!$this->is_admin()) {
            $where['userid'] = $this->_user['uid'];
        }

        $where['ORDER'] = ["parent_id ASC", "id ASC"];

        $subclass_list = $this->topic->select('topic_subclass', ['id', 'title'], $where);
        $this->getView()->assign('subclass_list', $subclass_list);
        $this->getView()->assign('data', $data);
    }

    /**
     * 改变专题文章状态
     */
    public function changeStatusAction() {
        $id = $this->getRequest()->getQuery('id');

        if (!$id) {
            Alert::danger("非法请求");
            Yaf_Controller_Abstract::redirect($this->referer);
            exit;
        }

        $status = (int) $this->getRequest()->getQuery('status');

        if (!$this->checkrole($id, "topic_data")) {
            Alert::danger("权限不足");
            Yaf_Controller_Abstract::redirect($this->referer);
            exit;
        }

        if ($this->topic->update("topic_data", ['status' => $status], ['id' => $news_id])) {
            Alert::success("操作成功");
            Yaf_Controller_Abstract::redirect("/admin/topicsubclass/index?tid=" . $tid);
        } else {
            Alert::danger("操作失败");
            Yaf_Controller_Abstract::redirect($this->referer);
        }

        exit;
    }

    /**
     * 新闻批量添加
     * @author yangguofeng
     */
    public function batchAddAction() {
        $data = [];

        if ($_FILES['news']['name']) {

            $title = explode('.', $_FILES['news']['name']);
            $news = file_get_contents($_FILES['news']['tmp_name']);

            //新闻内容转编码
            $content = strcmp(mb_detect_encoding($news, ['UTF-8', 'GBK']), 'UTF-8') === 0 ? $news : mb_convert_encoding($news, "UTF-8", "GBK");

            //关键字(如果没写关键字，在标题中取。如果没取到，标题作为关键词)
            $keywords = empty($this->getRequest()->getPost('keywords', '')) ?
                    implode('   ', misc::getKeywords($title[0])) :
                    $this->getRequest()->getPost('keywords', '');
            $keywords = empty($keywords) ? $title[0] : $keywords;

            $ret = $this->topic->add("topic_data",
                    [
                        'title' => $title[0],
                        'tid' => intval($this->getRequest()->getQuery('tid')),
                        'sid'=>intval($this->getRequest()->getQuery('sid')),
                        'keywords' => $keywords,
                        'status'=>1,
                        'attribute'=>0,
                        'userid'=>$this->_user['uid'],
                        'username'=>$data['username'] = $this->_user['username'],
                        'description' => mb_substr(strip_tags($content), 0, 78, 'UTF-8'),
                        'inputtime' => $_SERVER['REQUEST_TIME'],
                        'content' => $content,
                    ]
            );
            if ($ret) {
                exit($ret);
            } else {
                header("HTTP/1.1 500 Internal Server Error");
                exit(0);
            }
        }
        $this->getView()->assign('tid', intval($this->getRequest()->getQuery('tid')));
        $this->getView()->assign('sid', intval($this->getRequest()->getQuery('sid')));
    }

    /**
     * 将专题文章导出为excel表格
     */
    public function export2excelAction(){
        $data = [];
        $where = '';
        $startTime = $this->getRequest()->getQuery('starttime') ? $this->getRequest()->getQuery('starttime') : date('Ymd',time());
        $endTime = $this->getRequest()->getQuery('endtime') ? $this->getRequest()->getQuery('endtime') : date('Ymd',time());

        if (!$this->is_admin()) {
            $where .= "userid=" . $this->_user['uid'];
            $where .= " AND status=1";
        } else {
            $where .= " status=1";
        }

        if((int)$this->getRequest()->getQuery('tid')){

            $tid = (int)$this->getRequest()->getQuery('tid');
            $data['topic_name'] = $this->topic->get('topic','topic_name',['topic_id'=>$tid]);
            $where .= " AND tid= '$tid'";
        }else{
            Alert::danger("参数错误");
            Yaf_Controller_Abstract::redirect($this->referer);
            exit;
        }

        if ($this->getRequest()->getPost('sid')) {
            $where .= " AND sid=" . $this->getRequest()->getPost('sid');
            $this->getView()->assign('sid', $this->getRequest()->getPost('sid'));
        } elseif ($this->getRequest()->getQuery('sid')) {
            $where .= " AND sid=" . $this->getRequest()->getQuery('sid');
            $this->getView()->assign('sid', $this->getRequest()->getQuery('sid'));
        }

        //判断是否有搜索条件
        if ($this->getRequest()->getPost('id', 0)) {
            $where .= " AND id=" . $this->getRequest()->getPost('id');
            $this->getView()->assign('id', $this->getRequest()->getPost('id'));
        }

        if ($this->getRequest()->getPost('title', 0)) {
            $where .= " AND title LIKE '%" . $this->getRequest()->getPost('title');
            $where .= "%'";
            $this->getView()->assign('title', $this->getRequest()->getPost('title'));
        }

        if($startTime && $endTime){
            $startTime = strtotime($startTime.'000000');
            $endTime = strtotime($endTime.'235959');

            if($startTime < $endTime){
                $where .= " AND (inputtime BETWEEN '$startTime' AND '$endTime')";
            }else{
                Alert::danger("参数错误");
                Yaf_Controller_Abstract::redirect($this->referer);
                exit;
            } 
        }else{
            Alert::danger("参数错误");
            Yaf_Controller_Abstract::redirect($this->referer);
            exit;
        }

        $where2 = $where;
        $where .= " ORDER BY id ASC";
        $sql = "SELECT id,title,tid,sid,userid,username,status,inputtime FROM topic_data WHERE $where";
        //echo $sql;
        $data['data'] = $this->topic->query($sql);
        
        //获取专题path
        foreach ($data['data'] as $k=>$v) {
            $data['data'][$k]['topic_path'] = $this->topic->get("topic", "topic_path", ["topic_id" => $v['tid'], "LIMIT" => 1]);
        }

        unset($where);

        //获取当期用户所有的栏目
        if (!$this->is_admin()) {
            $where['userid'] = $this->_user['uid'];
        }

        $where['ORDER'] = ["parent_id ASC", "id ASC"];

        $subclass_list = $this->topic->select('topic_subclass', ['id', 'title','path'], $where);
        
        foreach($subclass_list as $k=>$v){
            $subclass[$v['id']] = $v;
        }

        $this->getView()->assign('subclass',$subclass);
        $this->getView()->assign('subclass_list', $subclass_list);        
        $this->getView()->assign('data', $data);
    }

}
