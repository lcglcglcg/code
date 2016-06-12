<?php

if (!defined('APP_PATH'))
    exit('No direct script access allowed');

class ListController extends BaseController {

    /**
     * 初始化
     */
    public $offset;
    public $totalpage;
    public $total;

    function init() {
        parent::init();
        //实例化文章数据库操作类
        $this->article = new ArticleModel();
        //载入专题控制器和模型
        $this->topic = new TopicModel();
        $this->TopicController = new TopicController();
        $this->subclasslist = array();
        //定义
        $this->offset = (int)$this->option['offset'];
        $this->totalpage = 0;
        $this->total = 0;
    }

    /**
     * 文章列表页
     * dir----dirname
     */
    public function IndexAction($dir = '', $page = 1) {
        //专题处理
        //获取专题path列表
        $topic_arr = $this->topic->gettopicpath();
        if(in_array($dir,$topic_arr)){
            $this->topic($dir);
        }

        //获取dirname为键值----->catid为值的数组
        $category = $this->base->category('en_key');

        //获取cid
        $cid = isset($category[$dir]) ? intval($category[$dir]['catid']) : 0;
        $data['category'] = $category[$dir]['catname'];

        if ($cid > 0 || strcmp($dir, 'lywz') === 0 || strcmp($dir, 'zxyd') === 0) {
            //手机端
            if ($this->mobile) {
                if (!strpos($_SERVER['REQUEST_URI'], '/m/')) {
                    header('Location:' . PHONEURL  . SITEDIR . '/m/' . $dir . '/default_' . $page . '.html');
                    exit();
                }


                //推荐阅读
                $data['tjyd'] = $this->article->articleList(['catid' => $cid, 'attribute' => 1], 0, 3);
                //精华文章
                $data['jhwz'] = $this->article->articleList(['catid' => $cid, 'attribute' => 2], 0, 1);
                //阅读热点
                $data['rdyd'] = $this->article->hothit('', 6);
                
                $this->offset = 7;
                $start = ($page - 1) * $this->offset;
                
                //列表
                $data['data'] = $this->article->articleList(['catid' => $cid], $start, $this->offset);
                
                $data['data_h'] = $this->article->articleList(['catid' => $cid, 'attribute' => 1], 0, 15);

                //获得分类下的文章总条数
                $total = $this->article->getTitleCount(['catid' => $cid]);
                $totalpage = ceil($total / $this->offset);
                
                //超过一页显示分页
                if ($totalpage > 1) {
                    $data['page'] = (new multi())->getSubContent('/' . SITEDIR . '/' . $dir . '/default_', '.html', $totalpage, $page, 3, ' target="_self" ', '', '', 'no');
                }
            } else {

                $pre = 'default';

                if (strrchr($_SERVER['REQUEST_URI'], '/') <> '/' && strrchr($_SERVER['REQUEST_URI'], '.html') <> '.html') {
                    header('Location:' . BASEURL . SITEDIR . '/' . $dir . '/');
                }

                //调用一天内的1w条
                if (strpos($_SERVER['REQUEST_URI'], '-') && $page > 50) {
                    $page = 50;
                }

                $data['dir'] = $dir;
                $start = ($page - 1) * $this->offset;

                //文章页的其他进入
                $urlArr = explode('/', $_SERVER['REQUEST_URI']);

                //来源网站
                if (strcmp($dir, 'lywz') === 0 || strpos($_SERVER['REQUEST_URI'], '-')) {

                    $this->lywz($data,$page,$urlArr);

                //最新阅读
                } else if (strcmp($dir, 'zxyd') === 0) {

                    $this->zxyd($data,$start,$urlArr);

                } else {

                    //列表页/hyzx/xxxx/20150515/禁止进入
                    if(preg_match('#^(\d+)$#',$urlArr[3]))
                        misc::show404();

                    //列表
                    $data['data'] = $this->article->articleList(['catid' => $cid, 'attribute' => 1], 0, 15);
                    $data['data_h'] = $this->article->articleList(['catid' => $cid], $start, $this->offset);

                    if(preg_match('#^(\d+)$#', $urlArr[3]) && empty($data['data'])){
                        misc::show404();
                    }

                    //过滤default_.html引起的非法URL，eg : sssdefault_4.html/defaultdd_7.html   xxx/20150203/
                    if(isset($urlArr[3]) && $urlArr[3] && $urlArr[2] != 'search' && (!preg_match('#^default_(\d+).html$#', $urlArr[3]) && !preg_match('#^(\d+)$#', $urlArr[3]))) {
                        misc::show404();
                    }
                    
                    //获得分类下的文章总条数
                    $this->total = $this->article->getTitleCount(['catid' => $cid]);
                }

                $this->totalpage = ceil($this->total / $this->offset);
                
                //超过一页显示分页
                if (!isset($data['page']) && $this->totalpage > 1) {
                    $data['page'] = (new multi())->getSubContent('/' . SITEDIR . '/' . $dir . '/default_', '.html', $this->totalpage, $page, 50, ' target="_self" ', '', '', 'no');
                    $pattern = '/&nbsp;共记录&nbsp;(\d+)\/(\d+)/';
                    $data['page'] = preg_replace($pattern, "", $data['page']);
                    $data['page'] .= "&nbsp;共" . $this->total . "记录&nbsp;{$page}/{$totalpage}";
                    $tmp = preg_replace("/>></", '>下一页<', $data['page']);
                    $tmp = preg_replace('/><</', '>上一页<', $tmp);
                    unset($data['page']);
                    $data['page'] = $tmp;
                }


                // 根据option表中的listpage中的设置进行调用
                // 包括相关阅读/最新资讯/最新阅读
                foreach($this->option['listpage'] as $k=>$v){
                    //是否全站：1是，0 不是；如果不是全站则根据当前cid 调用文章列表
                    
                    if($v['isall'] == 1){
                        $data[$k] = $this->article->articleList(['status'=>99],$v['start'],$v['offset'],'inputtime DESC',$v['cachetime']);
                    }else{
                        $data[$k] = $this->article->articleList(['catid'=>$cid,'status'=>99],$v['start'],$v['offset'],'inputtime DESC',$v['cachetime']);
                    }

                    //判断是否需要随机：1是，0不是
                    if($v['isrand'] == 1){
                        shuffle($data[$k]);
                    }
                }
            }

            //渲染模板
            $this->getView()->assign("title", $data['category']);
            $this->getView()->assign("keywords", $category[$dir]['keyword']);
            $this->getView()->assign("description", $category[$dir]['description']);
            $this->getView()->assign('hover', $dir);
            $this->getView()->assign("data", $data);
            $this->getView()->assign("totalpage", $this->totalpage);
            $this->getView()->assign("total", $this->total);
            $this->getView()->assign("page", $page);

            if ($this->mobile) {
                $this->getView()->display('./list/mobile.phtml');
                exit;
            } elseif (strpos($_SERVER['REQUEST_URI'], '-')) {
                $this->getView()->assign("title", '本站');
                $this->getView()->assign("keywords", '');
                $this->getView()->assign("description", '');
                $this->getView()->display('./list/other.phtml');
                exit;
            } else if (strcmp($dir, 'zxyd') === 0) {
                $this->getView()->assign("title", '最新阅读');
                $this->getView()->assign("keywords", '');
                $this->getView()->assign("description", '');
                $this->getView()->display('./list/map.phtml');
                exit;
            } else if (strcmp($dir, 'taiji') === 0 ) {
                $this->getView()->display('./list/taiji.phtml');
                exit;
            } else if (strcmp($dir, 'zdyy') === 0 ) {
                $this->getView()->display('./list/zdyy.phtml');
                exit;
            }
        } else {
            misc::show404();
        }
    }

    /**
     * 专题模块
     * @param $topicpath string $topicpath 专题path
     */
    public function topic($topicpath){
        //模板调用
        $template = $this->mobile ? 'wap' : 'default';
        
        //URL分析链接
        $url_arr  = misc::URLAnalysis($_SERVER['REQUEST_URI']);
        $this->TopicController->init();
        
        //获取专题信息
        $topicinfo = $this->topic->get_topicinfo_by_path($topicpath);
        $guidec_links = $topicinfo['topic_guidec'];
        
        //获取子类列表
        $subclasslist = $this->TopicController->select_subclass_list($topicinfo['topic_id']);
        $this->getView()->assign('guidec_links',$guidec_links);
        $this->getView()->assign('topic',$topicpath);
        $this->getView()->assign('subclasslist',$subclasslist);
        $this->getView()->assign('topicinfo',$topicinfo);
        
        //进入专题子栏目列表
        if( isset($url_arr['subtopic']) && $url_arr['subtopic'] ){
            $subtopic = isset($url_arr['subtopic']) ? $url_arr['subtopic'] : null;
            $this->getView()->assign('subtopic',$subtopic);
            
            // 进入专题文章详情页
            if ( isset($url_arr['id']) && $url_arr['id'] ) {
                $id =  $url_arr['id'];
                $data = $this->TopicController->content($topicpath,$subtopic,$id);
                $this->getView()->assign('data',$data);
                $this->getView()->display(TOPIC_PATH.$topicpath."/$template/content.html");
            }else{
                
                // 进入专题子栏目分页
                $page = 1;
                if( isset($url_arr['page']) && $url_arr['page'] ){
                    $page = $url_arr['page'];
                    //$data = $this->TopicController->topiclist($topicpath,$subtopic,$page);
                }

                //否则进入专题子分类默认列表页
                $data = $this->TopicController->topiclist($topicpath,$subtopic,$page);
                $this->getView()->assign('data',$data);
                $this->getView()->assign('page',$page);
                $this->getView()->display(TOPIC_PATH.$topicpath."/$template/list.html");
            }
            exit;
        }

        //显示专题首页
        $data = $this->TopicController->index($topicpath);
        $this->getView()->assign('data',$data);
        $this->getView()->display(TOPIC_PATH.$topicpath."/$template/index.html");
        exit;
    }

    /**
     * 来源网站
     */
    public function lywz(&$data,$page,$urlArr){
        $endArr = explode('.', $urlArr[count($urlArr) - 1]);

        //分页/hyzx/lybj/2014-12-29_46.html
        if (strpos($endArr[0], '_')) {
            $dateArr = explode('_', $endArr[0]);
            $pre = $dateArr[0];
        } else {
            $pre = $endArr[0];
        }

        $page = isset($dateArr[1]) ? $dateArr[1] : 1;
        $start = ($page - 1) * $this->offset;

        $startTime = strtotime($pre . ' 00:00:00');
        $endTime = strtotime($pre . ' 23:59:59');
        $data['data'] = $this->article->articleList(['startTime' => $startTime, 'endTime' => $endTime], $start, $this->offset);
        //过滤通过网站地图进来以及分页是不存在的页码页面非法的URL
        // if(empty($data['data'])) {
        //     misc::show404();
        // }
        //获得分类下的文章总条数
        $this->total = $this->article->getTitleCount(['startTime' => $startTime, 'endTime' => $endTime]); 
        $this->totalpage = ceil($this->total / $this->offset);
        //超过一页显示分页
        if ($this->totalpage > 1) {
            if(!isset($dateArr[1]))
                $data['page'] = (new multi())->getSubContent($pre.'_', '.html', $this->totalpage, 1, 46, ' target="_self" ', '', '', 'no');
            else
                $data['page'] = (new multi())->getSubContent($pre.'_', '.html', $this->totalpage, $dateArr[1], 46, ' target="_self" ', '', '', 'no');
            $data['page'] .= "&nbsp;共" . $this->total . "记录&nbsp;{$page}/{$totalpage}";
            $pattern = '/&nbsp;共记录&nbsp;(\d+)\/(\d+)/';
            $data['page'] = preg_replace($pattern, "", $data['page']);
            $tmp = preg_replace("/>></", '>下一页<', $data['page']);
            $tmp = preg_replace('/><</', '>上一页<', $tmp);
            unset($data['page']);
            $data['page'] = $tmp;
        }

    }

    /**
     * 最新阅读
     * @param $data array 要处理的数据
     * @param $start int 
     */
    public function zxyd(&$data,$start,$urlArr){
        //列表
        $data['data'] = $this->article->articleList('', $start, $this->offset);
        
        //过滤default_.html引起的非法URL，eg : sssdefault_4.html/defaultdd_7.html
        if(isset($urlArr[3]) && $urlArr[3] && !preg_match('#^default_(\d+).html$#', $urlArr[3]) || empty($data['data'])) {
            misc::show404();
        }
        //获得分类下的文章总条数
        $this->total = $this->article->getTitleCount('');
        
        //调用一天内的1w条
        if ($this->total > 10000) {
            $this->total = 10000;
        }
    }

}
