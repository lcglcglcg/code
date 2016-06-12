<?php

if (!defined('APP_PATH'))
    exit('No direct script access allowed');

class ToolsController extends BaseController {

    function init() {
        parent::init();

        null != $this->getRequest()->getQuery('key') || header('Location:' . BASEURL . SITEDIR . '/');

    }

    /**
     * 文章列表页
     * dir----dirname
     */
    public function IndexAction() {
        $data = [];
        $page = $this->getRequest()->getQuery('page') ? intval($this->getRequest()->getQuery('page')) : 1;

        $url = '/tools/index?key=' . date('Y-m-d');

        //实例化文章数据库操作类
        $article = new ArticleModel();

        if($this->getRequest()->getQuery('key')){
            $data['starttime'] = $this->getRequest()->getQuery('key') . ' 00:00:00';
            $data['endtime'] = $this->getRequest()->getQuery('key') . ' 23:59:59';
        }else{
            $data['starttime'] = date('Y-m-d') . ' 00:00:00';
            $data['endtime'] = date('Y-m-d') . ' 23:59:59';
        }

        //全站共发布文章数
        $data['count'] = $article->count('news');

        //今天发布文章数
        $data['todayCount'] = $article->count(
                'news', ['inputtime[<>]' => [strtotime($data['starttime']), strtotime($data['endtime'])]]
        );

        //搜索的文章数
        if ($this->getRequest()->getQuery('starttime') && $this->getRequest()->getQuery('endtime')) {
            $data['starttime'] = $this->getRequest()->getQuery('starttime');
            $data['endtime'] = $this->getRequest()->getQuery('endtime');

            if (strtotime($data['starttime']) && strtotime($data['endtime'])) {
                $data['searchCount'] = $article->count(
                        'news', ['inputtime[<>]' => [strtotime($data['starttime']), strtotime($data['endtime'])]]
                );
            }
        } else {
            $data['searchCount'] = $data['todayCount'];
        }

        $offset = 100;
        $start = ($page - 1) * $offset;

        //查询的文章
        $data['data'] = $article->select(
                'news', ['id', 'catid', 'title', 'inputtime'], ['AND' =>
            ['inputtime[<>]' => [strtotime($data['starttime']), strtotime($data['endtime'])]],
            'LIMIT' => [$start, $offset],
            'ORDER' => 'inputtime DESC'
                ]
        );

        $totalpage = ceil($data['searchCount'] / $offset);
        $url .= '&starttime=' . $data['starttime'] . '&endtime=' . $data['endtime'];


        //超过一页显示分页
        if ($totalpage > 1) {
            $url .= '&page=';
            $data['page'] = (new multi())->getSubContent($url, '', $totalpage, $page, 20, ' target="_self" ', '', '', 'no');
        }

        $this->getView()->assign("data", $data);
    }

}
