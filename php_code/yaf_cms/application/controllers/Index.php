<?php

/**
 * 首页控制类
 *
 * User: zhangxudong@lianchuangbrothers.com
 * Date: 14-11-14
 * Time: 10:02
 */
if (!defined('APP_PATH'))
    exit('No direct script access allowed');

class IndexController extends BaseController {

    function init() {
        parent::init();
        //实例化首页model层
        $this->index = new IndexModel();
    }

    function indexAction() {
        $data = [];
        //手机端
        if ($this->mobile) {
            if (!strpos($_SERVER['REQUEST_URI'], '/m/')) {
                header('Location:' . PHONEURL . SITEDIR . '/m/');
            }

            //精华文章
            $data['jhwz'] = $this->index->articleListDetail(['attribute' => 2], 'inputtime DESC', 0, 6, rand(300, 600),1);
            //推荐文章
            $data['tjwz'] = $this->index->articleListDetail(['attribute' => 1], 'inputtime DESC', 0, 6, rand(300, 600),1);
            //热点文章
            $data['rdwz'] = $this->index->articleListDetail([], 'inputtime DESC', 0, 6, rand(300, 600),1);
                
            //判断站点类型:1老站,2新站
            if(SITE_TYPE == 2){

                $i = 1;
                foreach($this->data['category'] as $k=>$v){
                    if($v['wap_show_limit'] > 0){
                        $data['cat'.$i] = $this->index->articleListDetail(['catid' => $v['catid']], 'inputtime DESC', 0, $v['wap_show_limit'], rand(300, 600),1);
                        $i++;
                    }
                }

            }elseif(SITE_TYPE == 1){
                //由于老站的栏目分类较多,所以要手动指定catid和catpath
                $category = [
                    'ssjf'=>5827117610,
                    'tszl'=>5827119910,
                    'bzbj'=>5827123346,
                    'cnly'=>5827125496,
                    'myqf'=>5827128362,
                    'yszd'=>5827134126,
                    'qsjt'=>5827139067,
                    'jgyx'=>5827196539,
                    'pybm'=>5827198771,
                    'jkyl'=>5827200903,
                    'jkbj'=>5827203744,
                    'jbzd'=>5827205724
                ];

                foreach($category as $k=>$v){
                    $data[$k] = $this->index->articleListDetail(['catid' => $v], 'inputtime DESC', 0, 6, rand(300, 600),1);
                }
            }

        //PC端
        } else {
            if (strrchr($_SERVER['PHP_SELF'], '/') <> '/') {
                header('Location:' . BASEURL . SITEDIR . '/');
            }

            //首页最新资讯
            $data['zxwz'] = $this->index->articleListDetail([], 'inputtime DESC', 0, 10, rand(300, 600), 0);
            $data['zxwz_h'] = $this->index->articleListDetail(['attribute' => 1], 'inputtime DESC', 0, 10, rand(300, 600), 0);

            //各个栏目最新文章
            foreach($this->data['category'] as $k=>$v){
                if($v['pc_show_limit'] > 0){
                    $data[$v['catpath'].'_h'] = $this->index->articleListDetail(['attribute' => 1,'catid' => $v['catid']], 'inputtime DESC', 0, $v['pc_show_limit'], rand(300, 600),0);
                }
            }
            
            
            //隐藏医疗的各个栏目文章
            foreach($this->data['category'] as $k=>$v){
                if($v['pc_show_limit'] > 0){
                    $data[$v['catpath']] = $this->index->articleList(['catid' => $v['catid']], 'inputtime DESC', 0, $v['pc_show_limit'], rand(300, 600),0);
                }
            }

            //首页调用专题文章列表
            $data['topiclist'] = $this->index->topiclist(['tid'=>1], 'inputtime DESC', 0, 13, rand(300, 600));
        }

        //渲染模板
        $this->getView()->assign("data", $data);

        if ($this->mobile) {
            $this->getView()->display('./index/mobile.phtml');
            exit;
        }
    }

}
