<?php

if (!defined('APP_PATH'))
    exit('No direct script access allowed');
/**
  文档类
  @author lucas lucasphper@gmail.com
  @access public
  @package base
  @version 1.0 2014/11/27
 */

include_once(PATH_APP . 'modules/Admin/controllers/Admin.php');
include_once(PATH_APP . 'modules/Admin/models/Document.php');
include_once(PATH_APP . 'modules/Admin/models/Category.php');
include_once(PATH_APP . 'modules/Admin/models/User.php');
include_once(PATH_APP . 'modules/Admin/models/Modelfields.php');
include_once(PATH_APP . 'modules/Admin/models/Modeldata.php');
include_once(PATH_APP . '../library/image.php');

class DocumentController extends AdminController {

    /**
     * 初始化
     * @param void
     * @return void
     */
    function init() {
        parent::init();

        //db 实例化
        $this->db_document = new DocumentModel();
        $this->db_category = new CategoryModel();
        $this->db_user = new UserModel();
        $this->db_modelFields = new ModelfieldsModel();
        $this->db_modelData = new ModeldataModel();
        $this->db_admin = new AdminModel();
    }

    /**
     * 文章列表
     * @author yangguofeng
     */
    public function indexAction() {

        $data = [];
        $cond = [];
        $cond['status'] = 99;
        $url = '/admin/document/index?1=1';
        $page = $this->getRequest()->getQuery('page') ? intval($this->getRequest()->getQuery('page')) : 1;

        if ($this->getRequest()->getQuery('dosubmit')) {
            $url .= '&dosubmit=dosubmit';
            if ($id = intval($this->getRequest()->getQuery('id'))) {
                $cond['id'] = $id;
                $url .= '&id=' . $cond['id'];
            }
            if ($catid = intval($this->getRequest()->getQuery('catid'))) {
                $cond['catid'] = $catid;
                $url .= '&catid=' . $cond['catid'];
            }
            if ($title = core::safe_str($this->getRequest()->getQuery('title'))) {
                $cond['title'] = $title;
                $url .= '&title=' . $cond['title'];
            }
            $this->getView()->assign('cond', $cond);
        }

        //栏目
        $data['category'] = $this->db_category->categoryList(NULL, 0, 100, 'orderid');

        //每页显示数
        $offset = 10;
        $start = ($page - 1) * $offset;
        $data['data'] = $this->db_document->newsList($cond, $start, $offset);
        
        $total = $cond['title'] ? $this->db_document->count_multisearch($cond) : $this->db_document->count($cond);
        $titlesearch = $cond['title'] ? true : false;
        $totalpage = ceil($total / $offset);
        //超过一页显示分页
        if ($totalpage > 1) {
            $url .= '&page=';
            $data['page'] = (new multi())->getSubContent($url, '', $totalpage, $page, 20, ' target="_self" ', '', '', 'no');
            //每页实际显示数
            $data['perNum'] = $page > $totalpage ? 0 : $page == $totalpage ? $total % $offset : $offset;
        }
        
        $this->getView()->assign('data', $data);
        $this->getView()->assign('titlesearch', $titlesearch);
    }

    /**
     * 新建文章
     * @author yangguofeng
     */
    public function createAction() {
        //栏目
        $data['category'] = $this->db_category->categoryList(['parentid' => 7], 0, 100);

        if ($this->getRequest()->isPost()) {
            $continue = TRUE;

            //缩略图
            if ($_FILES['defaultpic']['name']) {
                //相对路径
                $dir = 'upload' . DIRECTORY_SEPARATOR . 'news' . DIRECTORY_SEPARATOR . date('Y', $_SERVER['REQUEST_TIME']) . DIRECTORY_SEPARATOR . date('m', $_SERVER['REQUEST_TIME']) . DIRECTORY_SEPARATOR . date('d', $_SERVER['REQUEST_TIME']);
                //绝对路径
                $path = APP_PATH . DIRECTORY_SEPARATOR . 'kele' . DIRECTORY_SEPARATOR . $dir;

                if (!is_dir($path)) {
                    $continue = mkdir($path, 0777, true);
                }

                if ($continue) {
                    $continue = move_uploaded_file($_FILES['defaultpic']['tmp_name'], $path . DIRECTORY_SEPARATOR . $_FILES['defaultpic']['name']);

                    //保存到db中的相对路径
                    $savePath = $dir . DIRECTORY_SEPARATOR . $_SERVER['REQUEST_TIME'] . '-' . $_FILES['defaultpic']['name'];
                }
            }

            if ($continue) {
                //标题
                $title = $this->getRequest()->getPost('title', '');
                //内容
                $content = $this->getRequest()->getPost('content', '');
                //描述
                $descripion = $this->getRequest()->getPost('descripion', '') ?
                        $this->getRequest()->getPost('descripion', '0') :
                        trim(misc::remove_nbsp(mb_substr(strip_tags($content), 0, 78, 'UTF-8')));
                //关键字(如果没写关键字，在标题中取。如果没取到，标题作为关键词)
                $keywords = empty($this->getRequest()->getPost('keywords', '')) ?
                        implode('   ', misc::getKeywords($title)) :
                        $this->getRequest()->getPost('keywords', '');
                $keywords = empty($keywords) ? $title : $keywords;

                $ret = $this->db_document->addNews(
                        [
                            'title' => $title,
                            'catid' => $this->getRequest()->getPost('catid', 0),
                            'keywords' => $keywords,
                            'listorder' => intval($this->getRequest()->getPost('listorder')),
                            'attribute' => intval($this->getRequest()->getPost('attribute')),
                            'url' => filter_var($this->getRequest()->getPost('url'), FILTER_VALIDATE_URL) ? $this->getRequest()->getPost('url') : '',
                            'description' => $descripion,
                            'inputtime' => $_SERVER['REQUEST_TIME'],
                            'status' => 99,
                            'content' => addslashes(stripslashes($content)),
                            'defaultpic' => $savePath
                        ]
                );
                if ($ret) {
                    Alert::success('添加成功！');
                    $this->redirect('/admin/document/index');
                } else {
                    Alert::danger('添加失败！');
                    $this->redirect('/admin/document/create');
                }
            } else {
                Alert::danger('缩略图上传失败');
                $this->redirect('/admin/document/create');
            }
            exit();
        }
        $this->getView()->assign('data', $data);
    }

    /**
     * 编辑文章
     * @author yangguofeng
     */
    public function updateAction() {
        $data = [];
        $id = intval($this->getRequest()->getQuery('id'));
        $this->getView()->assign('categorys', $this->db_category->categoryList(NULL, 0, 100, 'orderid', ['catid', 'catname']));

        if ($this->getRequest()->isPost()) {
            $news['title'] = $this->getRequest()->getPost('title');
            //关键字(如果没写关键字，在标题中取。如果没取到，标题作为关键词)
            $news['keywords'] = $keywords = empty($this->getRequest()->getPost('keywords', '')) ?
                    implode('   ', misc::getKeywords($news['title'])) :
                    $this->getRequest()->getPost('keywords', '');
            $news['keywords'] = empty($news['keywords']) ? $news['title'] : $news['keywords'];
            $news['catid'] = intval($this->getRequest()->getPost('catid'));
            $news['description'] = $this->getRequest()->getPost('desc');
            $news['attribute'] = intval($this->getRequest()->getPost('attribute'));
            $news['updatetime'] = $_SERVER['REQUEST_TIME'];
            $newdata['content'] = $this->getRequest()->getPost('content');

            $news_table_name = $this->db_document->get_table_id($this->getRequest()->getPost('id'));
            $news_data_table_name = $this->db_document->get_table_id($this->getRequest()->getPost('id'),'news_data');

            $this->db_document->update($news_table_name, $news, ['id' => $this->getRequest()->getPost('id')]);
            $this->db_document->update($news_data_table_name, $newdata, ['id' => $this->getRequest()->getPost('id')]);

            //删除前台缓存
            $redisKey = 'article:detail' . $this->getRequest()->getPost('id');
            $this->cache->delete($redisKey);

            Alert::success('更新成功！');
            $this->redirect($this->getRequest()->getPost('referer'));
            exit();
        } else {
            $data['referer'] = isset($_SERVER["HTTP_REFERER"]) ? $_SERVER["HTTP_REFERER"] : '/admin/document/index';
            if ($news = $this->db_document->getNewsById($id)) {
                $data['news'] = $news[0];
                $this->getView()->assign('data', $data);
            } else {
                Alert::success('此新闻不存在！');
                $this->redirect($data['referer']);
                exit();
            }
        }
    }

    /**
     * 新闻批量添加
     * @author yangguofeng
     */
    public function batchAddAction() {
        $data = [];
        $data['cid'] = $this->getRequest()->getQuery('cid');

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
            
            $ret = $this->db_document->addNews(
                    [
                        'title' => $title[0],
                        'catid' => intval($this->getRequest()->getQuery('cid')),
                        'keywords' => $keywords,
                        'url' => '',
                        'description' => mb_substr(strip_tags($content), 0, 78, 'UTF-8'),
                        'inputtime' => $_SERVER['REQUEST_TIME'],
                        'status' => 99,
                        'content' => $content
                    ]
            );
            if ($ret) {
                exit($ret);
            } else {
                header("HTTP/1.1 500 Internal Server Error");
                exit(0);
            }
        }

        $this->getView()->assign('data', $data);
    }

    /**
     * 批量删除
     * @author yangguofeng
     */
    public function batchDeleteAction() {
        $referer = isset($_SERVER["HTTP_REFERER"]) ? str_replace('http://' . $_SERVER['HTTP_HOST'], '', $_SERVER["HTTP_REFERER"]) : '/admin/document/index';

        if ($this->getRequest()->isPost()) {
            if (empty($this->getRequest()->getPost())) {
                Alert::warning('没有选中任何文章！');
            } else {
                $data = $this->getRequest()->getPost();
                
                //删除db
                $ret = $this->db_document->batchDeleteNews(['id' => $data['id']]);
                
                //删除缓存
                foreach ($data['id'] as $id) {
                    $redisKey = 'article:detail' . $id;
                    $this->cache->delete($redisKey);
                }

                if ($ret) {
                    Alert::success('删除成功！');
                } else {
                    Alert::success('删除失败！');
                }
            }
        } else {
            Alert::warning('删除失败！');
        }
        $this->redirect($referer);
    }

    /**
     * 批量把文章作为推荐、精华
     * @author yangguofeng
     */
    public function batchRecommendAction() {
        $referer = isset($_SERVER["HTTP_REFERER"]) ? str_replace('http://' . $_SERVER['HTTP_HOST'], '', $_SERVER["HTTP_REFERER"]) : '/admin/document/index';
        if (in_array($this->getRequest()->getQuery('attribute'), [0, 1, 2])) {

            $data = $this->getRequest()->getPost();

            $ret = $this->db_document->update('news', ['attribute' => $this->getRequest()->getQuery('attribute')], ['id' => $data['id']]);

            if ($ret) {
                Alert::success('操作成功！');
            } else {
                Alert::success('操作失败！');
            }
        }else if($this->getRequest()->getQuery('status')){
            $data = $this->getRequest()->getPost();
            $ret = $this->db_document->update('news', ['status' => $this->getRequest()->getQuery('status')], ['id' => $data['id']]);

            if ($ret) {
                Alert::success('操作成功！');
            } else {
                Alert::success('操作失败~');
            }
        } else {
            Alert::danger('操作失败！');
        }
        $this->redirect($referer);
    }

    /**
     * 筛选文档
     * @param void
     * @return void
     */
    public function filterDocumentAction() {
        session_start();
        //文档分类
        $document_category = intval($this->getRequest()->getPost('document_category', 0));
        if ($document_category) {
            $_SESSION['cms']['document_filter_condition']['document_category'] = $document_category;
        } else {
            if (isset($_SESSION['cms']['document_filter_condition']['document_category'])) {
                unset($_SESSION['cms']['document_filter_condition']['document_category']);
            }
        }

        //文档模型
        $document_model = intval($this->getRequest()->getPost('document_model', 0));
        if ($document_model) {
            $_SESSION['cms']['document_filter_condition']['document_model'] = $document_model;
        } else {
            if (isset($_SESSION['cms']['document_filter_condition']['document_model'])) {
                unset($_SESSION['cms']['document_filter_condition']['document_model']);
            }
        }

        //文档状态
        $document_status = intval($this->getRequest()->getPost('document_status', 0));
        if ($document_status) {
            $_SESSION['cms']['document_filter_condition']['document_status'] = $document_status;
        } else {
            if (isset($_SESSION['cms']['document_filter_condition']['document_status'])) {
                unset($_SESSION['cms']['document_filter_condition']['document_status']);
            }
        }


        Yaf_Controller_Abstract::redirect('/admin/document/index');
    }

    /**
     * 删除文档
     * @param void
     * @return void
     */
    public function deleteDocumentAction() {
        $document_id = (int) $_GET['id'];
        if (!$document_id) {
            Yaf_Controller_Abstract::redirect("/admin/document/index");
            exit;
        }
        //获取表名，及表中的字段
        $catId = isset($_GET['catid']) ? (int) $_GET['catid'] : 0;
        $categoty_detail = (array) $this->db_category->get_category_detail($catId);
        $modelid = $categoty_detail[0]['modelid'];

        //获取表名
        $table_name = $this->db_modelData->get_modelname_by_id($modelid);
        if (empty($table_name))
            $table_name[0] = "news";
        $result = $this->db_document->delete_document($document_id, $table_name[0]);
        if ($result) {
            //删除前台缓存
            $redisKey = 'article:detail' . $document_id;
            $this->cache->delete($redisKey);
            $referer = $_SERVER['HTTP_REFERER'];
            echo "<script>alert('删除成功');location.href='{$referer}'</script>";
        } else {
            Yaf_Controller_Abstract::redirect("/admin/document/index");
            exit;
        }
        exit;
    }

    /**
     * 将文章移动到回收站
     * @author maczhis@gmail.com
     */
    public function trashAction() {
        $id = $this->getRequest()->getQuery('id');
        if ($id) {
            $data['status'] = 5;
            $data['updatetime'] = $_SERVER['time'];

            if ($this->db_document->document_trash($id, $data)) {
                //删除前台缓存
                $redisKey = 'article:detail' . $id;
                $this->cache->delete($redisKey);
                Alert::success("操作成功");
            } else {
                Alert::danger("操作失败");
            }

            Yaf_Controller_Abstract::redirect("/admin/document/index");
            exit;
        }
    }

    /**
     * 将回收站里面的文章还原到文章列表中
     * @author maczhis@gmail.com
     */
    public function reductionAction() {
        $id = $this->getRequest()->getQuery('id');
        if ($id) {
            $data['status'] = 99;
            $data['updatetime'] = $_SERVER['time'];

            if ($this->db_document->document_trash($id, $data)) {
                $redisKey = 'article:detail' . $id;
                $this->cache->delete($redisKey);
                Alert::success("操作成功");
            } else {
                Alert::danger("操作失败");
            }

            Yaf_Controller_Abstract::redirect("/admin/document/trashindex");
            exit;
        }
    }

    public function batchtrashAction() {
        $referer = isset($_SERVER["HTTP_REFERER"]) ? str_replace('http://' . $_SERVER['HTTP_HOST'], '', $_SERVER["HTTP_REFERER"]) : '/admin/document/index';

        if ($this->getRequest()->isPost()) {
            if (empty($this->getRequest()->getPost())) {
                Alert::warning('没有选中任何文章！');
            } else {
                $data = $this->getRequest()->getPost();

                //删除db
                $ret = $this->db_document->batch_trash(['id' => $data['id']]);

                //删除缓存
                foreach ($data['id'] as $id) {
                    $redisKey = 'article:detail' . $id;
                    $this->cache->delete($redisKey);
                }

                if ($ret) {
                    Alert::success('删除成功！');
                } else {
                    Alert::success('删除失败！');
                }
            }
        } else {
            Alert::warning('删除失败！');
        }
        $this->redirect($referer);
    }

    /**
     * 回收站文章列表
     * @author maczhis@gmail.com
     */
    public function trashindexAction() {
        $data = [];
        $cond = [];
        
        $url = '/admin/document/trashindex?1=1';
        $page = $this->getRequest()->getQuery('page') ? intval($this->getRequest()->getQuery('page')) : 1;

        if ($this->getRequest()->getQuery('dosubmit')) {
            $url .= '&dosubmit=dosubmit';
            if ($id = intval($this->getRequest()->getQuery('id'))) {
                $cond['id'] = $id;
                $url .= '&id=' . $cond['id'];
            }
            if ($catid = intval($this->getRequest()->getQuery('catid'))) {
                $cond['catid'] = $catid;
                $url .= '&catid=' . $cond['catid'];
            }
            if ($title = core::safe_str($this->getRequest()->getQuery('title'))) {
                $cond['title'] = $title;
                $url .= '&title=' . $cond['title'];
            }
            $this->getView()->assign('cond', $cond);
        }

        //栏目
        $data['category'] = $this->db_category->categoryList(NULL, 0, 100, 'orderid');

        //每页显示数
        $offset = 10;
        $start = ($page - 1) * $offset;
        $cond['status'] = 5;
        $data['data'] = $this->db_document->newsList($cond, $start, $offset);

        $total = $this->db_document->count($cond);
        $totalpage = ceil($total / $offset);
        //超过一页显示分页
        if ($totalpage > 1) {
            $url .= '&page=';
            $data['page'] = (new multi())->getSubContent($url, '', $totalpage, $page, 20, ' target="_self" ', '', '', 'no');
        }

        //每页实际显示数
        $data['perNum'] = $page > $totalpage ? 0 : $page == $totalpage ? $total % $offset : $offset;

        $this->getView()->assign('data', $data);
    }

    /**
     * 隐藏/显示 文档
     * @param void
     * @return void
     */
    public function documentDisplayAction() {
        $document_detail = array();

        //文档id
        $document_id = (int) $_GET['id'];
        if (!$document_id) {
            Yaf_Controller_Abstract::redirect("/admin/document/index");
            exit;
        }
        $document_detail['id'] = $document_id;

        //文档状态
        $status = (int) $_GET['status'];
        if (!$status) {
            Yaf_Controller_Abstract::redirect("/admin/document/index");
            exit;
        }
        $document_detail['status'] = ($status == 99) ? 1 : 99;
        $result = $this->db_document->document_display($document_detail);
        if ($result) {
            $referer = $_SERVER['HTTP_REFERER'];
            echo "<script>alert('操作成功');location.href='{$referer}'</script>";
        } else {
            Yaf_Controller_Abstract::redirect("/admin/document/index");
            exit;
        }
        exit;
    }

    public function newDocumentAction() {
        //栏目列表
        $category_count = $this->db_category->get_category_count();
        $category_list = (array) $this->db_category->get_category_list(0, $category_count);
        $category_list_format = $this->treeForCategory($category_list);
        $this->getView()->assign('category_list_format', $category_list_format);
    }

    /*
     * 添加文档具体数据的处理
     * @param void
     * @return void
     */

    public function addAction() {
        //判断是否添加
        if (trim($this->getRequest()->getPost('document_submit', 0))) {
            //获取数据
            //**************依据模型处理与模型关联的数据表**********************
            $docData = $this->getRequest()->getPost();
            $document_catid = trim($this->getRequest()->getPost('document_catid', 0));
            if (!$document_catid) {
                echo '<script>alert("请选择正确的所属栏目！");window.history.go(-1);</script>';
                exit;
            }

            $document_detail = (array) $this->db_category->get_category_detail($document_catid);
            $modelid = $document_detail[0]['modelid'];
            $where = array("AND" => array("status" => 1, "modelid" => $modelid));
            //获取该模型的所有启用字段
            $fields = $this->db_modelFields->get_column_by_where("field", $where);
            //获取表名
            $table_name = $this->db_modelData->get_modelname_by_id($document_detail[0]['modelid']);
//            var_dump($table_name);die;
            //自动接收表单数据并过滤
            foreach ($docData as $key => $val) {
                if (in_array($key, $fields))
                    $data[$key] = trim($val);
            }
            if (in_array('inputtime', $fields))
                $data['inputtime'] = $_SERVER['time'];
            if (in_array('updatetime', $fields))
                $data['updatetime'] = $_SERVER['time'];
            if (in_array('uid', $fields))
                $data['uid'] = $this->_user['uid'];
            if (in_array('thumb', $fields)) {
                $fileInfo = $this->getRequest()->getFiles();
                $filename = image::safe_name($fileInfo['thumb']['name'], '/uploadfile/image/');
                $data['thumb'] = image::thumb($fileInfo['thumb']['tmp_name'], '/uploadfile/image/' . $filename);
            }
            $data['catid'] = $document_catid;
//            var_dump($data);die;
            //判断数据是否正常
            if (isset($data['title']) && empty($data['title'])) {
                echo '<script>alert("请填写标题！");window.history.go(-1);</script>';
                exit;
            }
            if (isset($data['content']) && empty($data['content'])) {
                echo '<script>alert("请填写内容！");window.history.go(-1);</script>';
                exit;
            }
            //将数据添加到表中
            if ($table_name[0] == 'news') {
                $content = $data['content'];
                unset($data['content']);
                if (($id = $this->db_document->document_add($data, $table_name[0])) && $this->db_document->document_add(array('id' => $id, 'content' => $content), 'news_data') !== FALSE) {
                    echo '<script>alert("操作成功");window.location.href="/admin/document/index?list=1";</script>';
                } else {
                    echo '<script>alert("操作失败");window.history.go(-1);</script>';
                }
            } else {
                if (($id = $this->db_document->document_add($data, $table_name[0]))) {
                    echo '<script>alert("操作成功");window.location.href="/admin/document/index?list=1";</script>';
                } else {
                    echo '<script>alert("操作失败");window.history.go(-1);</script>';
                }
            }
            exit;
            // ****************************************************/
        } else {
            Yaf_Controller_Abstract::redirect("/admin/document/index");
            exit;
        }
    }

    /*
     * 编辑文档页面展示
     * @param void
     * @return void
     */

    public function editDocumentAction() {
        //栏目列表
        $category_count = $this->db_category->get_category_count();
        $category_list = (array) $this->db_category->get_category_list(0, $category_count);
        $category_list_format = $this->treeForCategory($category_list);
        $this->getView()->assign('category_list_format', $category_list_format);
        if (isset($_GET['pageid']) && (int) $_GET['pageid']) {
            $pageId = "pageid=" . $_GET['pageid'];
        } else {
            $pageId = "list=1";
        }
        $this->getView()->assign('pageid', $pageId);
        //编辑状态获取文档详情
        if (isset($_GET['id']) && (int) $_GET['id']) {
            $document_id = (int) $_GET['id'];
            //$document_detail = (array)$this->db_document->get_document_detail($document_id);
            //获取表名，及表中的字段
            $catId = isset($_GET['catid']) ? (int) $_GET['catid'] : 0;
            $categoty_detail = (array) $this->db_category->get_category_detail($catId);
            $modelid = $categoty_detail[0]['modelid'];
            $where = array("AND" => array("status" => 1, "modelid" => $modelid));
            //获取该模型的所有启用字段
            $fields = $this->db_modelFields->get_column_by_where(array("field", "name", "formtype"), $where);
            //获取表名
            $table_name = $this->db_modelData->get_modelname_by_id($modelid);
            if (empty($table_name)) {
                $table_name[0] = "news";
                $fields = array(array('field' => 'title', 'name' => '文章标题'), array('field' => 'keywords', 'name' => '关键字'), array('field' => 'content', 'name' => '内容'), array('field' => 'description', 'name' => '描述'), array('field' => 'listorder', 'name' => '排序'), array('field' => 'status', 'name' => '状态'));
            }
            $document_detail = (array) $this->db_document->get_document_detail($document_id, '*', $table_name[0]);
            $this->getView()->assign('model_fields', $fields);
            $this->getView()->assign('table_name', $table_name[0]);
            $this->getView()->assign('document_detail', $document_detail);
        }
    }

    /*
     * 编辑文档具体数据的处理
     * @param void
     * @return void
     */

    public function editAction() {
        //判断是否添加
        if (trim($this->getRequest()->getPost('document_submit', 0))) {
            //获取数据
            $document_id = trim($this->getRequest()->getPost('document_id', 0));
            $pageId = $this->getRequest()->getPost('document_pageid', 0);
            $table_name = $this->getRequest()->getPost('table_name', 0);
            //**************依据模型处理与模型关联的数据表**********************
            $docData = $this->getRequest()->getPost();
            $document_catid = trim($this->getRequest()->getPost('document_catid', 0));
            if (!$document_catid) {
                echo '<script>alert("请选择正确的所属栏目！");window.history.go(-1);</script>';
                exit;
            }

            $document_detail = (array) $this->db_category->get_category_detail($document_catid);
            $modelid = $document_detail[0]['modelid'];
            $where = array("AND" => array("status" => 1, "modelid" => $modelid));
            //获取该模型的所有启用字段
            $fields = $this->db_modelFields->get_column_by_where("field", $where);
            //自动接收表单数据并过滤
            foreach ($docData as $key => $val) {
                if (in_array($key, $fields))
                    $data[$key] = trim($val);
            }
            if (in_array('updatetime', $fields))
                $data['updatetime'] = $_SERVER['time'];
            if (in_array('uid', $fields))
                $data['uid'] = $this->_user['uid'];
            if (in_array('thumb', $fields)) {
                $fileInfo = $this->getRequest()->getFiles();
//                var_dump($fileInfo);
                $filename = image::safe_name($fileInfo['thumb']['name'], '/uploadfile/image/');
                $data['thumb'] = image::thumb($fileInfo['thumb']['tmp_name'], '/uploadfile/image/' . $filename);
            }
            $data['catid'] = $document_catid;
//            var_dump($data);die;
            //判断数据是否正常
            if (isset($data['title']) && empty($data['title'])) {
                echo '<script>alert("请填写标题！");window.history.go(-1);</script>';
                exit;
            }
            if (isset($data['content']) && empty($data['content'])) {
                echo '<script>alert("请填写内容！");window.history.go(-1);</script>';
                exit;
            }
            //将数据添加到表中
            if ($table_name == 'news') {
                $content = $data['content'];
                unset($data['content']);
                if (($this->db_document->document_update($document_id, $data, $table_name) && $this->db_document->document_update($document_id, array('content' => $content), 'news_data')) || $this->db_document->document_update($document_id, $data, $table_name) || $this->db_document->document_update($document_id, array('content' => $content), 'news_data')) {
                    echo '<script>alert("操作成功");window.location.href="/admin/document/index?' . $pageId . '";</script>';
                } else {
                    echo '<script>alert("操作失败");window.history.go(-1);</script>';
                }
            } else {
                if (($id = $this->db_document->document_update($document_id, $data, $table_name))) {
                    echo '<script>alert("操作成功");window.location.href="/admin/document/index?' . $pageId . '";</script>';
                } else {
                    echo '<script>alert("操作失败");window.history.go(-1);</script>';
                }
            }
            exit;
        } else {
            Yaf_Controller_Abstract::redirect("/admin/document/index");
            exit;
        }
    }

    /*
     * 处理AJAX请求
     * 得到某个模型中的字段
     */

    public function ajaxFieldAction() {
        $catId = $_GET['document_catid'] + 0;
        $document_detail = (array) $this->db_category->get_category_detail($catId);
        $modelid = $document_detail[0]['modelid'];
        $where = array("AND" => array("status" => 1, "modelid" => $modelid));
        $fields = $this->db_modelFields->get_column_by_where(array("field", "name", "formtype"), $where);
        echo json_encode($fields);
        die;
    }

    /*
     * 处理AJAX请求
     * 得到分类中的数据
     */

    public function ajaxCategoryDataAction() {

        //获得当前分类id
        $model_id = $_GET['document_model'] + 0;

        //栏目列表
        $category_count = $this->db_category->get_category_count();
        $category_list = (array) $this->db_category->get_category_list(0, $category_count);
        $category_list_format = $this->treeForCategory($category_list);

        //获得分类数据
        foreach ($category_list_format as $key => $value) {

            if ($value['modelid'] == $model_id) {
                $cates[] = $value;
            }
        }
        echo json_encode($cates);
        die;
    }

}
