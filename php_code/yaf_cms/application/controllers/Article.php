<?php

/**
 * 文章控制类
 *
 * User: zhangxudong@lianchuangbrothers.com
 * Date: 14-11-14
 * Time: 10:02
 */
class ArticleController extends BaseController {

    /**
     * 初始化
     */
    function init() {
        parent::init();
        //实例化文章数据库操作类
        $this->article = new ArticleModel();
        $this->multi = new multi();
    }

    /**
     * 文章详情
     */
    public function indexAction($dir = '', $id = 0) {
        if ($id <> false && is_numeric($id)) {
            //去掉url末尾的字符，如：+
            $ret = explode('html', $_SERVER['REQUEST_URI']);
            if ($ret[1]) {
                Header("HTTP/1.1 301 Moved Permanently");
                Header("Location: " . rtrim($_SERVER['REQUEST_URI'], $ret[1]));
            }
            $data = $this->article->detail($id);
            
            //去除文章中的‘\’，否则js无法运行
            $data['content'] = stripslashes($data['content']);

            // Tag 关键字加链接
            if ($keywords = trim($data['en_keywords'])) {
                $data['tag'] = $this->keywordsAddUrl($keywords,$data['keywords']);
            }

            // 获取文章的关键字并逐一替换（title keywords description 和 content）
            $this->replace_keywords($data);

            // 对文章内容中的关键词追加链接
            $this->keywords_add_url($data);
            

            // 判断dir
            if (!isset($this->data['category'][$data['catid']]['catpath']) || $dir <> $this->data['category'][$data['catid']]['catpath'] || empty($data)){
                misc::show404();
            }

            // 获取分类名称
            $category = $this->base->category('en_key');
            $cid = isset($category[$dir]) ? intval($category[$dir]['catid']) : 0;
            $data['category'] = $category[$dir]['catname'];

            // 手机端
            if ($this->mobile) {
                if (!strpos($_SERVER['REQUEST_URI'], '/m/')) {
                    header('Location:' . PHONEURL . SITEDIR . '/m/' . $dir . '/' . $id . '.html');
                    exit();
                }

                //推荐阅读
                $data['tjyd'] = $this->article->articleList(['catid' => $cid, 'attribute' => 1], 0, 4);

                //精华阅读
                $data['jhyd'] = $this->article->articleList(['catid' => $cid, 'attribute' => 1], 5, 8);
            } else {

                // 根据option表中的listpage中的设置进行调用(包括相关阅读/最新资讯/最新阅读)
                foreach($this->option['contentpage'] as $k=>$v){
                    //isid:是否根据id调用文章：1是，0否
                    switch ($v['isid']) {
                        case 1:
                            //isall:是否调用全站文章：1是，0否
                            if($v['isall'] == 1){
                                //推荐文章：调用本栏目400随机10条
                                $data[$k] = $this->getListById(['status' => 99,'id[>]'=>$id], $v['start'], $v['offset']);
                            }else{
                                //推荐文章：调用本栏目400随机10条
                                $data[$k] = $this->getListById(['status' => 99,'catid'=>$cid,'id[>]'=>$id], $v['start'], $v['offset']);
                            }
                            break;
                        case 0:
                        default:
                            if($v['isall'] == 1){
                                $data[$k] = $this->article->articleList(['status'=>99],$v['start'],$v['offset'],'inputtime DESC',$v['cachetime']);
                            }else{
                                $data[$k] = $this->article->articleList(['catid'=>$cid,'status'=>99],$v['start'],$v['offset'],'inputtime DESC',$v['cachetime']);
                            }
                            break;
                    }

                    //判断是否需要随机：1是，0不是
                    if($v['isrand'] == 1){
                        shuffle($data[$k]);
                    }
                }

                //上一篇下一篇
                $data['prenext'] = $this->article->preNext($data['inputtime'], ['catid' => $cid],$id);
            }

            //渲染模板
            $this->getView()->assign("categoryname", $data['category']);
            $this->getView()->assign("title", $data['title']);
            $this->getView()->assign("keywords", $data['title']);
            $this->getView()->assign("hits", $hits[0]['views']);
            $this->getView()->assign("description", $data['description'] ? strip_tags($data['description']) : mb_substr(strip_tags($data['content']), 0, 78) . '...');
            $this->getView()->assign("hover", $dir);
            $this->getView()->assign("data", $data);
            
            if ($this->mobile) {
                $this->getView()->display('./article/mobile.phtml');
                exit;
            }else if (strcmp($dir, 'taiji') === 0) {
                $this->getView()->display('./article/taiji.phtml');
                exit;
            }else if (strcmp($dir, 'zdyy') === 0) {
                $this->getView()->display('./article/zdyy.phtml');
                exit;
            }
        } else {
            misc::show404();
        }
    }
    
    /**
     * 获取调用
     * @param type $cond
     * @param type $start
     * @param type $offset
     * @return array
     */
    public function getListById($cond, $start, $offset)
    {
        $data = $this->article->articleList($cond, $start, $offset, 'id ASC');
        if(count($data) < $offset) {
            if(isset($cond['id[>]'])) {
                $cond['id[<]'] = $cond['id[>]'];
                unset($cond['id[>]']);
            }
            $dataL = $this->article->articleList($cond, $start, ($offset-count($data)), 'id DESC');
            $data = array_merge($data,$dataL);
        }
        return $data;
    }

   /**
     * 关键字添加链接
     * @param type $keywords
     * @return string
     */
    public function keywordsAddUrl($keywords, $title) {
        $tag = "";

        //多个关键字
        if (strpos($keywords, ' ') && $keywords = explode(' ', $keywords)) {
            $newTitle = explode(' ', $title);
            foreach ($keywords as $k=>$value) {
                if ($value) {
                    //keywords加链接，前面空格
                    $tag .= '   <a href="/' . SITEDIR . '/search/' . Pinyin::get($value) . '.html" >' . $newTitle[$k] . '</a>  ';
                }
            }
        } else {
            //一个关键字
            $tag = '';
        }
        return $tag;
    }

    

    /**
     * 获取文章的关键字并逐一替换（title keywords description 和 content）
     * @param $data array 要替换的数据
     */
    public function replace_keywords(&$data){
        //获取文章关键词
        $content_words = $this->article->get_keywords();

        //文章关键词逐个替换
        foreach ($content_words as $ckeys) {

            if (strpos($data['keywords'], $ckeys['keywords']) !== false ){
                //页面头文件中关键词替换
                $data['keywords'] = preg_replace_callback("/" . $ckeys['keywords'] . "/", function($str) {
                    return $this->article->replace_keywords($str);
                }, $data['keywords']);
            }
            if(strpos($data['title'], $ckeys['keywords']) !== false ){
                //各标题中关键词替换
                $data['title'] = preg_replace_callback("/" . $ckeys['keywords'] . "/", function($str) {
                    return $this->article->replace_keywords($str);
                }, $data['title']);
            }
            if(strpos($data['description'], $ckeys['keywords']) !== false ){
                //文章描述关键词替换
                $data['description'] = preg_replace_callback("/" . $ckeys['keywords'] . "/", function($str) {
                    return $this->article->replace_keywords($str);
                }, $data['description']);
            }
            if(strpos($data['content'], $ckeys['keywords']) !== false ) {
                //文章内容关键词替换
                $data['content'] = str_replace($ckeys['keywords'], $this->article->replace_keywords($ckeys['keywords']), $data['content']);
            }

        }
    }

    /**
     * 给关键字添加URL链接(加内链)
     * @param $data array 要处理的数据
     */
    public function keywords_add_url(&$data){

        //获取内链关键词
        $url_key = $this->article->get_key_datas();

        //对文章内容中的关键词追加链接
        foreach ($url_key as $nkeys) {

            if (strpos($data['content'], $nkeys['keywords']) !== false) {

                $data['content'] = preg_replace_callback("/" . $nkeys['keywords'] . "/", function($str) {
                    $url_href = $this->article->replace_key_url($str);
                    return "<a href=" . $url_href['href_url'] . "  target=_blank >" . $str[0] . "</a>";
                }, $data['content'], 1);
            }
        }
    }

    /**
     * 相关文章
     * @param string $enKeyword 关键字
     * @param int    $count     要去除的
     */
    public function relevant($enKeyword, $count = 20) {
        $enKeyword = strpos($enKeyword, ' ') ? explode(' ', $enKeyword)[0] : trim($enKeyword);
        $data = [];
        /* if ($enKeywords = strpos($enKeyword, ' ')) {
          foreach ($enKeywords as $key => $value) {
          $data = $this->article->articleList(['LIKE' => ["en_keywords" => $value]], 0, $count);
          }
          } else { */
        $data = $this->article->articleList(["en_keywords" => $enKeyword]);

        //如果相关文章不够，从最新文章中填充
        //        if (count($data) < $count) {
        //            $new = $this->article->articleList([], 0, $count - count($data));
        //            $data = array_merge_recursive($data, $new);
        //        }
        //  }
        return $data;
    }

}
