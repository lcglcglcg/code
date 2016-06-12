<?php

if (!defined('APP_PATH'))
    exit('No direct script access allowed');
/**
 * 前台信息类
 * @author wangzejun
 * @access public
 * @package base
 */
Yaf_Loader::import(PATH_APP . 'modules/Admin/controllers/Admin.php');
Yaf_Loader::import(PATH_APP . 'modules/Admin/models/Config.php');

class ConfigController extends AdminController {

    /**
     * 初始化
     * @param void
     * @return void
     */
    function init() {
        parent::init();
        $this->referer      = isset($_SERVER["HTTP_REFERER"]) ? str_replace('http://' . $_SERVER['HTTP_HOST'], '', $_SERVER["HTTP_REFERER"]) : '/admin/index/index';
        //db 实例化
        $this->db_config = new ConfigModel();
    }

    /**
     * 清除单个缓存
     */
    public function flushAction(){
       $id = $this->getRequest()->getQuery('id', 0);
       $type = $this->getRequest()->getPost('cachetype',0);

       if($id || $type ){
            if($id){
                 $cacheKey = 'article:detail' . $id;
            }else{
                 switch ($type) {
                     case 'option':
                         $cacheKey = 'option';
                         break;
                     case 'category':
                         $cacheKey = 'category12';
                         break;
                     case 'table':
                         $cacheKey = 'tableinfobytime';
                         $this->cache->delete($cacheKey);

                         $cacheKey = 'tableinfo';
                         break;
                 }
            }

            if($this->cache->delete($cacheKey)){
                Alert::success("缓存清除成功");
                Yaf_Controller_Abstract::redirect("/admin/config/flush");
                exit;
            }else{
                Alert::danger("缓存清除失败");
                Yaf_Controller_Abstract::redirect("/admin/config/flush");
                exit;
            }
       }
       
    }

    /**
     * 清除全部缓存缓存
     */
    public function flushallAction(){
        if(CACHE_TYPE == 'redis' ? $this->cache->flushdb() : $this->cache->flush()){
            Alert::success("缓存清除成功");
            Yaf_Controller_Abstract::redirect("/admin/config/flush");
            exit;
        }else{
            Alert::danger("缓存清除失败");
            Yaf_Controller_Abstract::redirect("/admin/config/flush");
            exit;
        }
    }


    /**
     * 获取cms版本信息
     */
    public function checkversionAction(){
        exit;
        $current_version = $this->db_config->get_config();
        $this->getView()->assign("data",$data);
    }

    public function settingAction(){
        if($this->getRequest()->getPost('dosubmit',0)){
            $data = $this->getRequest()->getPost('config');
            if($data){
                foreach($data as $key=>$value){
                    $this->db_config->update_option(['conf_value'=>$value],['conf_name'=>$key]);
                }
                //清除缓存
                $cacheKey = 'option';
                $this->cache->delete($cacheKey);

                Alert::success("操作成功");
                Yaf_Controller_Abstract::redirect($this->referer);
                exit;
            }else{
                Yaf_Controller_Abstract::redirect($this->referer);
                exit;
            }
            
        }else{
            $data = $this->db_config->get_config();
            $this->getView()->assign("data",$data);
        }
    }

    public function articlesettingAction(){
        exit;
        if($this->getRequest()->getPost('dosubmit',0)){
            $data = $this->getRequest()->getPost('config');
            if($data){
                foreach($data as $key=>$value){
                    $this->db_config->update_option(['conf_value'=>$value],['conf_name'=>$key]);
                }

                Alert::success("操作成功");
                Yaf_Controller_Abstract::redirect($this->referer);
                exit;
            }
        }else{
            $data = $this->db_config->get_config();
            $this->getView()->assign("data",$data);
        }
    }

    /**
     * 文章列表调用管理
     */
    public function ArticlescallAction(){
        if($this->getRequest()->getPost('dosubmit',0)){
            $type = $this->getRequest()->getQuery('type',0);
            if($type == 'content'){
                $data = $this->getRequest()->getPost('contentpage',0);
                if($data){
                    $result = $this->db_config->update_option(['conf_value'=>json_encode($data)],['conf_name'=>'contentpage']);
                } 
            }elseif($type == 'list'){
                $data = $this->getRequest()->getPost('listpage',0);
                if($data){
                    $result = $this->db_config->update_option(['conf_value'=>json_encode($data)],['conf_name'=>'listpage']);
                }   
            }

            if($result){
                $cacheKey = 'option';
                $this->cache->delete($cacheKey);

                Alert::success("保存成功");
                Yaf_Controller_Abstract::redirect($this->referer);
                exit;
            }else{
                Alert::danger("保存失败");
                Yaf_Controller_Abstract::redirect($this->referer);
                exit;
            }

        }else{
            $data = $this->db_config->get_articles_call();
            $this->getView()->assign('data',$data);
        }
        
    }
}
