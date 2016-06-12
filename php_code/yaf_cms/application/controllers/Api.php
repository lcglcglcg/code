<?php

if (!defined('APP_PATH'))
    exit('No direct script access allowed');

/**
 * Created by PhpStorm.
 * User: kele
 * Date: 11/21/14
 * Time: 9:31 AM
 * 全站api调用统一接口
 */
class ApiController extends Yaf_Controller_Abstract {

    protected $redisKey =  'tableinfo';

    function init() {
        $this->redis = Yaf_Registry::get('redis');
        //数据库
        $this->db = Yaf_Registry::get('db_read');
        $this->db_write = Yaf_Registry::get('db_write');

        /* 关闭自动渲染 */
        Yaf_Dispatcher::getInstance()->disableView();
    }

    function indexAction() {
        $client_data = file_get_contents("php://input");
        var_dump($client_data);
        exit(0);
    }

    function mailtestAction(){
        //邮件通知，提示数据分表
        $subject = "站点：".SITENAME ." news 表中数据已经超过您设置的".ARTNUM."条数据.<br/>请引起重视！";
        $message = $subject ." 请注意及时分表和导出数据<br/>站点地址:".IMGURL."<br/> By lianchuangcms <br/>@".date('Y-m-d H:i:s',time());
        misc::sendmail('联创CMS','1184237916@qq.com',$subject,$message);
    }

    /*
     * 文章的浏览量
     */

    public function clickAction() {
        if ('POST' == $this->getRequest()->method) {
            //文章id
            $id = $this->getRequest()->getPost('id', 0);
            //栏目id
            $catid = intval($this->getRequest()->getPost('catid', 0));
            if (is_numeric($id)) {
                $redisKey =  'article:hits' . $id;
                $views = intval($this->redis->get($redisKey));
                if ($views == false) {//memcache没有值或者失效
                    $views = intval($this->db->get('hits', 'views', ['aid' => $id]));
                    $views++;
                    echo json_encode(['hit' => $views]);
                    // fastcgi_finish_request();
                    if ($views == 1)
                        $this->db_write->insert('hits', ['aid' => $id, 'views' => $views, 'catid' => $catid]);
                    else
                        $this->db_write->update('hits', ['views' => $views], ['aid' => $id]);
                    $this->redis->set($redisKey, $views, 0, 3600 * 24 * 29);
                } else {
                    $views++;
                    echo json_encode(['hit' => $views]);
                    //  fastcgi_finish_request();
                    //20次入库一次
                    if ($views % 20 == 0)
                        $this->db_write->update('hits', ['views' => $views], ['aid' => $id]);
                    $this->redis->set($redisKey, $views, 0, 3600 * 24 * 29);
                }
                return $views;
            }
        }
    }

    //输出栏目列表
    public function categoryAction() {
        $redisKey =  'category';
        $data = $this->redis->get($redisKey);
        //$data = false;
        if ($data == false) {
            $data = [];
            $temp = $this->db->select('category', [
                'catid',
                'catname',
                'catpath',
                    ], [
                'parentid' => 7,
                "ORDER" => "orderid DESC",
                "LIMIT" => 200
                    ]
            );
            foreach ($temp as $v)
                $data[$v['catid']] = $v;
            //缓存一小时
            $data <> false && $this->redis->set($redisKey, $data,  3600 * 24);
        }
        return $data;
    }

    //输出栏目列表
    function topicAction() {
        $redisKey =  'topic';
        $data = $this->redis->get($redisKey);
        //  $data = false;
        if ($data == false) {
            $data = [];
            $temp = $this->db->select('topic', [
                'topic_id',
                'topic_name',
                'topic_path',
                    ], [
                'is_show' => 1,
                "LIMIT" => 200
                    ]
            );
            foreach ($temp as $v) {
                $data[$v['topic_id']] = $v;
            }
            //缓存一小时
            $data <> false && $this->redis->set($redisKey, $data,  3600 * 24);
        }
        return $data;
    }

    //输出栏目列表
    function topicCategory($topicid) {
        $redisKey =  'topiccategory:topicid:' . $topicid;
        
        $data = false;
        
        $data = $this->redis->get($redisKey);
        
        if ($data == false) {
            $data = [];
            $temp = $this->db->select('topic_subclass', [
                'id',
                'title',
                'path',
                    ], [
                'tid' => $topicid,
                "LIMIT" => 200
                    ]
            );
            foreach ($temp as $v){
                $data[$v['id']] = $v;
            }
            //缓存一小时
            $data <> false && $this->redis->set($redisKey, $data,  3600 * 24);
        }
        return $data;
    }

    //article入库
    function articleAction() {
        //入库
        if ('POST' == $this->getRequest()->method) {
            //标题
            $title = $this->getRequest()->getPost('title', '');
            $title <> false && $title = trim(misc::remove_nbsp(mb_convert_encoding($title, "UTF-8")));
            
            //媒体id
            $mtidlist = $this->getRequest()->getPost('mtidlist', 0);
            $catid = $this->getRequest()->getPost('catid', 0);
            
            //关键词
            $keywords = $this->getRequest()->getPost('keyword', '');
            $keywords != false && $keywords = trim(misc::remove_nbsp(mb_convert_encoding($keywords, "UTF-8")));
            
            //关键字(如果没写关键字，在标题中取。如果没取到，标题作为关键词)
            $keywords = empty($this->getRequest()->getPost('keyword', '')) ?
                    implode('   ', misc::getKeywords($title)) :
                    $this->getRequest()->getPost('keyword', '');
            $keywords = empty($keywords) ? $title : $keywords;

            //创建时间
            $createdate = $this->getRequest()->getPost('createdate', 0);
            $topicdate = $_SERVER['time'];

            //时间类型1:unix time 2:timestamp(主要是针对news表)
            if(TIME_TYPE == 1){
                $createdate = $createdate ? strtotime($createdate) : $_SERVER['time'];
            }elseif(TIME_TYPE == 2){
                $createdate = $createdate ? $createdate : date('Y-m-d H:i:s',$_SERVER['time']);
            }

            //token
            $token = $this->getRequest()->getPost('token', '');
            /*
              if ($token == false) {
              echo json_encode(['msg' => '缺少参数', 'errcode' => -100]);
              exit;
              }
             */
            
            //内容
            $content = $this->getRequest()->getPost('content', '');
            if ($content <> false) {
                $content = mb_convert_encoding($this->getRequest()->getPost('content', ''), "UTF-8");
                $descripion = trim(misc::remove_nbsp(mb_substr(strip_tags($content), 0, 78, 'UTF-8')));
            }

            //判断是否存在id
            $id = $this->getRequest()->getPost('id', '');

            //explode('_', '123_456'),那么$catid[0]=123,$catid[1]=456。如果123，那么explode('_', '123')，那么$catid[0]=123
            
            //专题id_栏目id
            $catid = explode('_', $catid);

            if (isset($catid[1])) {//专题
                $this->topic_post($catid,$id,$title,$content,$$descripion,$keywords,$createdate);
            } else {//文章
                $this->news_post($catid,$id,$title,$content,$keywords,$descripion,$createdate);
            }

        } else {
            misc::show404();
        }
    }   

    /**
     * 文章的修改和发布
     * @param $catid    array       POST过来的catid信息经过explode后得到的数组
     * @param $id       int         POST过来的文章id
     * @param $title    string      标题
     * @param $content  string      内容
     * @param $keywords string      关键字
     * @param $descrption string    描述
     * @param $createdate int       文章创建的时间
     */
    public function news_post($catid,$id,$title,$content,$keywords,$descripion,$createdate){
        $catid = $catid[0];

        if ($title == false || $catid == false || $content == false) {
            echo json_encode(['msg' => '缺少参数', 'errcode' => -300], JSON_UNESCAPED_UNICODE);
            exit;
        }

        $category = $this->categoryAction();
        if (!isset($category[$catid])) {
            echo json_encode(['msg' => 'catid错误', 'errcode' => -300], JSON_UNESCAPED_UNICODE);
            exit;
        }
        if ($id <> false) {//修改
            //如果做了mysql分表，则此时需要根据id 来判断 需要更新哪一张表
            $news_table_name = $this->get_table_id($id);
            $news_data_table_name = $this->get_table_id($id,'news_data');

            $updateid = $this->db_write->update($news_table_name, [
                'title'     => $title,
                'catid'     => $catid,
                'keywords'  => $keywords,
                'description' => $descripion,
                'updatetime'=> $createdate,
                'status'    => 99
                    ], [
                'id' => $id,
                    ]
            );

            $news_data = $this->db_write->update($news_data_table_name, [
                'content' => addslashes(stripslashes($content))
                    ]
                    , [
                'id' => $id,
                    ]
            );

            if(!$news_data){
                $this->db_write->insert($news_data_table_name, [
                        'id' => $id,
                        'content' => addslashes(stripslashes($content))
                    ]
                );
            }

            if ($updateid >= 0) {
                $redisKey =  'article:detail' . $id;
                $this->redis->delete($redisKey);

                $url = misc::get_return_news_url($category[$catid]['catpath'],$createdate,$id);

                $data = [
                    'msg'       => '修改成功',
                    'errcode'   => 0,
                    'url'       => $url,
                    'classid'   => $catid,
                    'id'        => $id
                ];
                echo json_encode($data, JSON_UNESCAPED_UNICODE);
            } else
                echo json_encode(['msg' => '修改失败', 'errcode' => -300], JSON_UNESCAPED_UNICODE);
        } else {//增加

            //获取相应的表信息
            //这里的查询可以先走memcache
            $data = $this->get_table_info();
            
            $mem_count = count($data);

            //获取最后一张表的信息
            $table_id     = $data[$mem_count-1]['table_id'];
            $table_count  = $data[$mem_count-1]['table_count'];
            $table_total  = $data[$mem_count-1]['table_total'];
            $start_id     = $data[$mem_count-1]['start_id'];
            $end_id       = $data[$mem_count-1]['end_id'];
            unset($data);

            //执行新文章插入
            $insertid = $this->db_write->insert('news', [
                'title' => $title,
                'catid' => $catid,
                'keywords' => $keywords,
                'description' => $descripion,
                'inputtime' => $createdate,
                'status' => 99,
                    ]
            );
            
            //判断文章插入结果
            if ($insertid <> false) {
                $this->db_write->insert('news_data', [
                    'id' => $insertid,
                    'content' => addslashes(stripslashes($content))
                        ]
                );
                
                //更新一次table记录表(每20次更新一次数据库)
                $data = $this->get_table_info();
                
                //文章插入成功，$table_count 总数+1;
                $table_count ++;
                //echo $table_id.'<<'.$table_total.'<<'.$table_count;

                //每20次入库一次
                if($table_count % 20 == 0){
                    $this->db_write->update('table',[
                        'table_count[+]'=>20,
                        'table_total[+]'=>20,
                        'end_id'=>$insertid,
                        'endtime'=>$createdate
                        ],[
                        'ORDER'=>'id DESC',
                        'LIMIT'=>1
                        ]);

                    $this->db_write->update('table',[
                            'table_count[+]'=>20,
                            'table_total[+]'=>20,
                            'end_id'=>$insertid,
                            'endtime'=>$createdate
                        ],[
                        'id'=>1
                        ]);
                }

                //实时更新缓存中的信息
                $mem_count = count($data);
                ++$data[$mem_count-1]['table_count'];
                ++$data[$mem_count-1]['table_total'];
                $data[$mem_count-1]['end_id'] = $insertid;
                $data[$mem_count-1]['endtime'] = $createdate;
                
                ++$data[0]['table_count'];
                ++$data[0]['table_total'];
                $data[0]['end_id'] = $insertid;
                $data[0]['endtime'] = $insertid;

                $this->redis->set($this->redisKey, $data,  3600 * 12);
                unset($data);

                //判断是否需要新建数据库,如果需要新建数据库，则进行相应的操作
                if($table_count == ARTNUM){
                    $this->create_new_table($table_id,$insertid,$table_total,$start_id,$end_id);
                }

                //获取返回链接
                $url = misc::get_return_news_url($category[$catid]['catpath'],$createdate,$insertid);

                $data = [
                    'msg'       => '共发布成功1篇',
                    'errcode'   => 0,
                    'url'       => $url,
                    'id'        => $insertid
                ];
                echo json_encode($data, JSON_UNESCAPED_UNICODE);
            } else {
                echo json_encode(['msg' => '添加失败', 'errcode' => -300], JSON_UNESCAPED_UNICODE);
            }
        }
    }

    /**
     * 专题的插入和修改
     * @param $catid    array       POST过来的catid信息经过explode后得到的数组
     * @param $id       int         POST过来的文章id
     * @param $title    string      标题
     * @param $content  string      内容
     * @param $keywords string      关键字
     * @param $descrption string    描述
     * @param $createdate int       文章创建的时间
     */
    public function topic_post($catid,$id,$title,$content,$descripion,$keywords,$topicdate){

        $topicid = $catid[0];
        $catid = $catid[1];

        $topic = $this->topicAction();
        if (!isset($topic[$topicid])) {
            echo json_encode(['msg' => 'topicid错误', 'errcode' => -300], JSON_UNESCAPED_UNICODE);
            exit;
        }

        $topicCategory = $this->topicCategory($topicid);
        if (!isset($topicCategory[$catid])) {
            echo json_encode(['msg' => 'topiccatid错误', 'errcode' => -300], JSON_UNESCAPED_UNICODE);
            exit;
        }

        if ($id <> false) {//修改
            $updateid = $this->db_write->update('topic_data', [
                'title' => $title,
                'content' => $content,
                'tid' => $topicid,
                'sid' => $catid,
                'status' => 1,
                'description' => $descripion,
                'keywords' => $keywords,
                'updatetime' => $topicdate,
                    ], [
                'id' => $id
                    ]
            );
            if ($updateid >= 0) {
                $redisKey =  'key:topic_detail.id' . $id;
                $this->redis->delete($redisKey);
                $data = [
                    'msg' => '修改成功',
                    'errcode' => 0,
                    'url' => BASEURL . SITEDIR . '/' . $topic[$topicid]['topic_path'] . '/' . $topicCategory[$catid]['path'] . '/' . $id . '.html',
                    'classid' => $catid,
                    'id' => $id
                ];
                echo json_encode($data, JSON_UNESCAPED_UNICODE);
            } else
                echo json_encode(['msg' => '修改失败', 'errcode' => -300], JSON_UNESCAPED_UNICODE);
        } else {//增加
            $insertid = $this->db_write->insert('topic_data', [
                'title' => $title,
                'content' => $content,
                'tid' => $topicid,
                'sid' => $catid,
                'status' => 1,
                'description' => $descripion,
                'keywords' => $keywords,
                'inputtime' => $topicdate,
                    ]
            );
            if ($insertid) {
                $data = [
                    'msg' => '共发布成功1篇',
                    'errcode' => 0,
                    'url' => BASEURL . SITEDIR . '/' . $topic[$topicid]['topic_path'] . '/' . $topicCategory[$catid]['path'] . '/' . $insertid . '.html',
                    'id' => $insertid
                ];
                echo json_encode($data, JSON_UNESCAPED_UNICODE);
            } else {
                echo json_encode(['msg' => '添加失败', 'errcode' => -300], JSON_UNESCAPED_UNICODE);
            }
        }
    }

    /**
     * 根据文章id  返回改id所在的 表id;
     * @param int $id 文章的id
     * @return int 返回表的id
     */
    public function get_table_id($id,$type = 'news'){

        $data = $this->get_table_info();

        $table_id = 0;

        foreach($data as $k=>$v){
            if($id >= $v['start_id'] && $id <= $v['end_id']){
                $table_id = $v['table_id'];
            }
        }

        // 需要判断该id是否存在于news_xx表中,如果不存在，则在news表中查找，news_0表不存在。
        if( $table_id && !$this->db->get('news_'.$table_id,'catid',['id'=>$id]) ){
            $table_id = 0;
        }

        return $table_id == 0 ? $type : $type.'_'.$table_id;

    }

    /**
     * 创建新的news 和 news_data 的分表
     * @param $table_id int 新表的id
     * @param $insertid int 当前插入新文章的id
     * @param $start_id int 异步导数据开始的id
     * @param $end_id int 异步导数据的结束的id
     */
    public function create_new_table($table_id,$insertid,$table_total,$start_id,$end_id){
        $table_id   += 1;
        $news_table = 'news_'.$table_id;
        $news_data_table = 'news_data_'.$table_id;
        $insertid++;//下一条记录开始的id ,所以要+1
        
        //判断当'news_'.($table_id-1) 表的数据积累满200w时，调用php脚本进行mysql数据分表，起始和结束位置为：$start_id 和 $end_id
        //echo 'start_id:'.$start_id.'>>end_id:'.$end_id;
        //echo '正在传输数据到news_'.($table_id-1);

        //邮件通知，提示数据分表
        $this->notice_mail();

        if(TIME_TYPE == 1){
            //执行sql语句，创建新的数据表
            $sql_news = "CREATE TABLE `$news_table` (
                  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT COMMENT '主键',
                  `catid` bigint(20) unsigned NOT NULL COMMENT '栏目id',
                  `title` varchar(80) NOT NULL COMMENT '标题',
                  `keywords` char(40) DEFAULT NULL COMMENT '关键词',
                  `description` varchar(240) NOT NULL COMMENT '描述',
                  `attribute` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '1:推荐, 2:精华',
                  `listorder` int(10) NOT NULL DEFAULT '0' COMMENT '排序',
                  `uid` bigint(20) unsigned NOT NULL COMMENT '发布人id',
                  `username` char(20) DEFAULT NULL COMMENT '用户名',
                  `url` char(100) DEFAULT NULL COMMENT '链接地址',
                  `islink` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '外部链接',
                  `inputtime` int(10) unsigned NOT NULL COMMENT '添加时间',
                  `updatetime` int(10) unsigned NOT NULL COMMENT '更新时间',
                  `status` tinyint(2) unsigned NOT NULL DEFAULT '1' COMMENT '状态：0退稿，1~4审核状态 99通过 5回收站',
                  `en_keywords` varchar(255) DEFAULT '',
                  PRIMARY KEY (`id`),
                  UNIQUE KEY `id` (`id`) USING BTREE,
                  KEY `cas` (`catid`,`attribute`,`status`) USING BTREE,
                  KEY `inputtime` (`inputtime`) USING BTREE
                ) ENGINE=InnoDB AUTO_INCREMENT=$insertid DEFAULT CHARSET=utf8 COMMENT='文章主表';";
            
        }else{
            $sql_news = "CREATE TABLE `$news_table` (
                  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT COMMENT '主键',
                  `catid` bigint(20) unsigned NOT NULL COMMENT '栏目id',
                  `title` varchar(80) NOT NULL COMMENT '标题',
                  `keywords` varchar(80) DEFAULT NULL COMMENT '关键词',
                  `description` varchar(240) NOT NULL DEFAULT '''''' COMMENT '描述',
                  `attribute` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '1:推荐, 2:精华',
                  `listorder` int(10) NOT NULL DEFAULT '0' COMMENT '排序',
                  `uid` bigint(20) unsigned NOT NULL COMMENT '发布人id',
                  `username` char(20) DEFAULT NULL COMMENT '用户名',
                  `url` char(100) DEFAULT NULL COMMENT '链接地址',
                  `islink` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '外部链接',
                  `inputtime` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00' COMMENT '添加时间',
                  `updatetime` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00' COMMENT '更新时间',
                  `status` tinyint(2) unsigned NOT NULL DEFAULT '99' COMMENT '状态：0退稿，1~4审核状态 99通过 5回收站',
                  `en_keywords` varchar(255) DEFAULT '',
                  PRIMARY KEY (`id`),
                  KEY `time` (`inputtime`) USING BTREE,
                  KEY `cas` (`catid`,`attribute`,`status`) USING BTREE
                ) ENGINE=InnoDB AUTO_INCREMENT=$insertid DEFAULT CHARSET=utf8 COMMENT='文章主表';";
        }

        $sql_newsdata = "CREATE TABLE `$news_data_table` (
              `id` bigint(20) unsigned NOT NULL COMMENT '文章id',
              `content` text COMMENT '文章内容',
              `defaultpic` varchar(256) DEFAULT NULL,
              `fromweb` varchar(48) NOT NULL COMMENT '文章来源',
              `filename` varchar(256) DEFAULT NULL,
              UNIQUE KEY `id` (`id`) USING BTREE
            ) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='文章从表';";
        
        //执行建表语句
        $this->db_write->query($sql_news);
        $this->db_write->query($sql_newsdata);

        //table 记录表新插入一个表记录,并将新的表数据追加写入memcache缓存中
        $tmp_data = [
            'table_id'      =>  $table_id,
            'table_count'   =>  0,
            'table_total'   =>  $table_total,
            'start_id'      =>  $insertid,
            'end_id'        =>  $insertid
            ];

        $this->db_write->insert('table',$tmp_data);

        $data = $this->get_table_info();

        $data[] = $tmp_data;
        
        $this->redis->set($this->redisKey, $data,  3600 * 12);

        unset($data);
    }

    /**
     * 读取table表的缓存，如果缓存失效，则重新查询
     * @return $data array table信息
     */
    public function get_table_info(){

        $data = false;

        $data = $this->redis->get($this->redisKey);

        if($data == false){
            $data = $this->db->select('table',['id','table_id','table_count','table_total','start_id','end_id','starttime','endtime'],['id[>]'=>0]);

            $this->redis->set($this->redisKey, $data,  3600 * 12);
        }

        return $data;
    }

    //敏感词文章删除
    public function deleteAction()
    {
        if ('POST' == $this->getRequest()->method) {
            $id = $this->getRequest()->getPost('id', '');
            if ($id <> false) {//修改

                
                if (count($id) > 0) {
                    foreach($id as $v){

                        $news_table_name = $this->get_table_id($v);

                        $this->db_write->update($news_table_name, [
                                'status' => 5
                            ], [
                                'id' => $v,
                            ]
                        );
                        
                        $redisKey = 'article:detail' . $v;
                        $this->redis->delete($redisKey);
                        $data = [
                            'msg' => '删除成功',
                            'errcode' => 0,
                            'url' => BASEURL . SITEDIR . '/' . $v . '.html',
                        ];
                        echo json_encode($data, JSON_UNESCAPED_UNICODE);
                    }
                } else{
                    echo json_encode(['msg' => '删除失败', 'errcode' => -381], JSON_UNESCAPED_UNICODE);
                }
            }
        }
    }

    /**
     * 发送邮件提醒
     */
    public function notice_mail(){
        //邮件通知，提示数据分表
        $subject = "站点：".SITENAME ." news 表中数据已经超过您设置的".ARTNUM."条数据.<br/>请引起重视！";
        
        $message = $subject ." 请注意及时分表和导出数据<br/>站点地址:".IMGURL."<br/> By lianchuangcms <br/>@".date('Y-m-d H:i:s',time());

        $mail_list = array(
            '1184237916@qq.com',
            '2503865566@qq.com',
            '1012714893@qq.com',
            '593504602@qq.com'
            );
        foreach($mail_list as $mail){
            misc::sendmail('联创CMS',$mail,$subject,$message);
        }
    }

    /**
     * 对POST过来的文章内容和标题进行过滤：
     * 如果含有需要过滤的关键字和词，则该文章发布失败，并返回相关的错误信息
     */
    public function keyword_filter($title,$keywords,$content){
        $data = false;

        $redisKey = 'keywordlist:';
        
        $data = $this->redis->get($redisKey);

       

        if(!$data){
            $data = $this->db->select('badwords',['id','keywords']);
            $this->redis->set($redisKey, $data,  3600 * 12);
        }

        if(true){
            echo json_encode(['msg' => '文章含有不和谐的东西', 'errcode' => -400], JSON_UNESCAPED_UNICODE);
        }

        exit;
    }

}