<?php

if (!defined('APP_PATH'))
    exit('No direct script access allowed');
/**
单页类
@author zhangteng
@access public
@package base
@version 1.0 2014/12/24
 */
Yaf_Loader::import(PATH_APP . 'modules/Admin/controllers/Admin.php');
Yaf_Loader::import(PATH_APP . 'modules/Admin/models/Category.php');
Yaf_Loader::import(PATH_APP . 'modules/Admin/models/Modeldata.php');
Yaf_Loader::import(PATH_APP . 'modules/Admin/models/Page.php');

class PageController extends AdminController {

    /**
    初始化
    @param void
    @return void
     */
    function init() {
        parent::init();

        //db 实例化
        $this->category = new CategoryModel();
        $this->page = new PageModel();
        $this->modeldata = new ModeldataModel();
    }

    //查看目录文件
    function listDir($dir){

        if(!file_exists($dir)||!is_dir($dir)){
            return '';
        }

        $dirList=array('dirNum'=>0,'fileNum'=>0,'lists'=>'');
        $dir=opendir($dir);
        $i=0;
        while($file=readdir($dir)){
            if($file!=='.'&&$file!=='..'){

                $dirList['lists'][$i]['name']=$file;

                if(is_dir($file)){

                    $dirList['lists'][$i]['isDir']=true;

                    $dirList['dirNum']++;

                }else{

                    $dirList['lists'][$i]['isDir']=false;

                    $dirList['fileNum']++;
                }
                $i++;
            };
        };
        closedir($dir);

        return $dirList;

    }

    //修改首页
    function indexAction(){

        $article = PATH_APP . 'views/article/mobile.phtml';
        $list = PATH_APP . 'views/list/mobile.phtml';
        $index = PATH_APP . 'views/index/mobile.phtml';

        //修改权限
        chmod($article, 777);
        chmod($list, 777);
        chmod($index, 777);

        //详情页模板
        $data['article'] = file_get_contents($article);
        //列表页模板
        $data['list'] = file_get_contents($list);
        //首页模板
        $data['index'] = file_get_contents($index);
//        var_dump($data);
//        var_dump( self::listDir(PATH_APP . 'views/article/'));


        //如果有值传递过来
        if($this->getRequest()->isPost()){
            $data['index'] = $this->getRequest()->getPost('index', '');
            $data['list'] = $this->getRequest()->getPost('list', '');
            $data['article'] = $this->getRequest()->getPost('article', '');

//            file_put_contents('t.txt',$data['index']);
//            file_put_contents('t.txt',$data['list']);
           file_put_contents($article,$data['article']);
           chmod(PATH_APP . 'views/',755);
        }

        $this->getView()->assign('data', $data);

    }

    //接收编辑后的内容，保存文件
    function update(){
        if($this->getRequest()->isPost()){
            $data['index'] = $this->getRequest()->getPost('index', '');
            $data['list'] = $this->getRequest()->getPost('list', '');
            $data['article'] = $this->getRequest()->getPost('article', '');
        }
            $this->getView()->assign('data', $data);
    }



    /**
    默认页
    @param void
    @return void
     */
    function dexAction() {
        $data = [];
        $cond = [];
        $url = '/admin/page/index?1=1';

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
        $page = $this->getRequest()->getQuery('page') ? intval($this->getRequest()->getQuery('page')) : 1;

        //栏目
        $data['category'] = $this->category->categoryList(['parentid' => 7], 0, 50);

        //每页显示数
        $offset = 20;
        $start = ($page - 1) * $offset;
        $data['data'] = $this->page->pageList($cond, $start, $offset);

        $total = $this->page->count($cond);
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

        //栏目
        $data['category'] = $this->category->categoryList(['parentid' => 7], 0, 50);

        //判断是否有值提交
        if ($this->getRequest()->isPost()) {
            //内容插入数据库
            $ret = $this->page->insert('page',
                [
                    'title' => $this->getRequest()->getPost('title', ''),
                    'catid' => $this->getRequest()->getPost('catid', 0),
                    'keywords' => $this->getRequest()->getPost('keywords', ''),
                    'listorder' => intval($this->getRequest()->getPost('listorder')),
                    'url' => filter_var($this->getRequest()->getPost('url'), FILTER_VALIDATE_URL) ? $this->getRequest()->getPost('url') : '',
                    'description' => $this->getRequest()->getPost('descripion', '') ? $this->getRequest()->getPost('descripion', '0') : trim(misc::remove_nbsp(mb_substr(strip_tags($content), 0, 78, 'UTF-8'))),
                    'inputtime' => $_SERVER['REQUEST_TIME'],
                    'status' => 99,
                    'content' => addslashes(stripslashes($this->getRequest()->getPost('content', '')))
                ]
            );
            if ($ret) {
                Alert::success('添加成功！');
                $this->redirect('/admin/page/index');
            } else {
                $referer = isset($_SERVER["HTTP_REFERER"]) ? $_SERVER["HTTP_REFERER"] : '/admin/page/index';
                $this->redirect($referer);
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
            Yaf_Controller_Abstract::redirect("/admin/page/index");
            exit;
        }

        //根据id和删除表中相应记录
        $result = $this->page->delete('page', ['id'=>$page_id]);

        //memecache缓存清除
        if ($result) {
            //删除前台缓存
            $memkey = MEMPREFIX . 'page:detail' . $page_id;
            $this->memcache->delete($memkey);
            $referer = $_SERVER['HTTP_REFERER'];
            echo "<script>alert('删除成功');location.href='{$referer}'</script>";
        } else {
            Yaf_Controller_Abstract::redirect("/admin/page/index");
            exit;
        }
        exit;
    }


    /**
     * 编辑页面
     * @author zhangteng
     */
    public function dateAction() {
        $data = [];
        $id = intval($this->getRequest()->getQuery('id'));

        //展示栏目分类
        $this->getView()->assign('categorys', $this->category->categoryList(NULL, 0, 50, 'orderid', ['catid', 'catname']));

        //如果有数据提交就插入数据库
        if ($this->getRequest()->isPost()) {
            $id = intval($this->getRequest()->getPost('id'));
            $page['title'] = $this->getRequest()->getPost('title');
            $page['catid'] = intval($this->getRequest()->getPost('catid'));
            $page['description'] = $this->getRequest()->getPost('desc');
            $page['url'] = filter_var($this->getRequest()->getPost('url'), FILTER_VALIDATE_URL) ? $this->getRequest()->getPost('url') : '';
            $page['updatetime'] = $_SERVER['REQUEST_TIME'];
            $page['content'] = $this->getRequest()->getPost('content');
            $ret = $this->page->page_update($id, $page);

            //更新成功页面跳转
            if($ret) {
                echo "<script>alert('更新成功');</script>";
                $this->redirect($this->getRequest()->getPost('referer'));
            }

      } else {
            //展示编辑页面
            $data['referer'] = isset($_SERVER["HTTP_REFERER"]) ? $_SERVER["HTTP_REFERER"] : '/admin/page/index';

            //根据id取数据并展示在页面上
            if ($page = $this->page->getPageById($id)) {
                $data['page'] = $page;
                $this->getView()->assign('data', $data);
            } else {
                //失败跳回上一页
               Yaf_Controller_Abstract::redirect("/admin/page/index");
            }
       }
    }


}
