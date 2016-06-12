<?php

if (!defined('APP_PATH'))
    exit('No direct script access allowed');
/**
关键词处理类
@author zhangteng
@access public
@package base
@version 1.0 2015/1/8
 */
Yaf_Loader::import(PATH_APP . 'modules/Admin/controllers/Admin.php');
Yaf_Loader::import(PATH_APP . 'modules/Admin/models/Category.php');
Yaf_Loader::import(PATH_APP . 'modules/Admin/models/Modeldata.php');
Yaf_Loader::import(PATH_APP . 'modules/Admin/models/Keyword.php');

class KeywordController extends AdminController {

    /**
    初始化
    @param void
    @return void
     */
    function init() {
        parent::init();

        //db 实例化
        $this->category = new CategoryModel();
        $this->Keyword = new KeywordModel();
        $this->modeldata = new ModeldataModel();
    }

    /**
    默认页
    @param void
    @return void
     */
    function indexAction() {
        $data = [];
        $cond = [];
        $url = '/admin/keyword/page?1=1';

        if ($this->getRequest()->getQuery('dosubmit')) {
            $url .= '&dosubmit=dosubmit';

            if ($Keywords = core::safe_str($this->getRequest()->getQuery('keywords'))) {
                $cond['keywords'] = $Keywords;
                $url .= '&Keywords=' . $cond['keywords'];
            }
            $this->getView()->assign('cond', $cond);
        }
        $page = $this->getRequest()->getQuery('page') ? intval($this->getRequest()->getQuery('page')) : 1;

        //获得表中数据
        //$data['data'] = $this->Keyword->get_content_keywords_List();

        //每页显示数
        $offset = 15;
        $start = ($page - 1) * $offset;
        $data['data'] = $this->Keyword->search_keywords_list($cond, $start, $offset);
        $total = $this->Keyword->count($cond);
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
     * 新建页面
     * @author zhangteng
     */
    public function createAction() {

        //判断是否有值提交
        if ($this->getRequest()->isPost()) {
            //内容插入数据库
            $ret = $this->Keyword->content_keywords_add(
                [
                    'id' => intval($this->getRequest()->getPost('id', 0)),
                    'description' => trim($this->getRequest()->getPost('desc', '')),
                    'keywords' => trim($this->getRequest()->getPost('keywords', '')),
                    'href_url' => filter_var($this->getRequest()->getPost('href_url'), FILTER_VALIDATE_URL) ? $this->getRequest()->getPost('href_url') : '#',
                ]
            );
            if ($ret) {
                Alert::success('添加成功！');
                Yaf_Controller_Abstract::redirect("/admin/keyword/index");
                exit;
            } else {
                Alert::warning('关键词已存在或非法字符');
                $referer = isset($_SERVER["HTTP_REFERER"]) ? $_SERVER["HTTP_REFERER"] : '/admin/keyword/index';
                $this->redirect($referer);
                exit;
            }
        }
        $this->getView()->assign('data', $data);
    }

    /**
     * 删除页面
     * @param void
     * @return void
     */
    public function deletePageAction() {
        $page_id = intval($this->getRequest()->getQuery('id'));
        if (!$page_id) {
            Yaf_Controller_Abstract::redirect("/admin/keyword/index");
            exit;
        }

        //根据id和删除表中相应记录
        $result = $this->Keyword->delete_keywords($page_id);

        //memecache缓存清除
        if ($result) {
            //删除前台缓存
            $memkey = MEMPREFIX . 'keywords:detail' . $page_id;
            $this->memcache->delete($memkey);
            $referer = $_SERVER['HTTP_REFERER'];
            Alert::success("删除成功");
            Yaf_Controller_Abstract::redirect("/admin/keyword/index");
            exit;
            //echo "<script>alert('删除成功');location.href='{$referer}'</script>";
        } else {
            Yaf_Controller_Abstract::redirect("/admin/keyword/index");
            exit;
        }
        exit;
    }


    /**
     * 编辑页面
     * @author zhangteng
     */
    public function updateAction() {
        $data = [];

        //如果有数据提交就插入数据库
        if ($this->getRequest()->isPost()) {
            $id = intval($this->getRequest()->getPost('id'));
            $page['description'] = trim($this->getRequest()->getPost('desc'));
            $page['keywords'] = trim($this->getRequest()->getPost('keywords'));
            $page['href_url'] = filter_var($this->getRequest()->getPost('href_url'), FILTER_VALIDATE_URL) ? $this->getRequest()->getPost('href_url') : '';
            $ret = $this->Keyword->content_keywords_update($id, $page);

            //更新成功页面跳转
            if($ret !== false) {
                $referer = $this->redirect($this->getRequest()->getPost('referer'));
                Alert::success("更新成功");
                Yaf_Controller_Abstract::redirect("/admin/keyword/index");
                exit;
            }

        } else {
            //展示编辑页面
            $data['referer'] = isset($_SERVER["HTTP_REFERER"]) ? $_SERVER["HTTP_REFERER"] : '/admin/keyword/index';
            $id = intval($this->getRequest()->getQuery('id'));

            //根据id取数据并展示在页面上
            if ($page = $this->Keyword->get_contentwords($id)) {
                $data['page'] = $page;
                $this->getView()->assign('data', $data);
            } else {
                //失败跳回上一页
                Yaf_Controller_Abstract::redirect("/admin/keyword/index");
            }
        }
    }


}
