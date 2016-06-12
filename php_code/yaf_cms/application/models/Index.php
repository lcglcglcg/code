<?php

/**
 * 首页数据库操作类
 *
 * User: zhangxudong@lianchuangbrothers.com
 * Date: 14-11-14
 * Time: 11:12
 */
class IndexModel extends BaseModel {

    /**
     * 初始化
     */
    function __construct() {
        parent::__construct();
    }

    /**
     * 按文章点击量
     *
     * @param int $limit 影响行数
     * @return array
     */
    public function get_article_by_views($limit = 7) {
        $result = false;

        $result = $this->redis->get('index:news:hits:limit:' . $limit);

        if ($result === false) {
            $hits = $this->db->select('hits', ['aid', 'views'], ['ORDER' => 'views DESC', 'LIMIT' => $limit]);
            $result = [];
            
            foreach ($hits as $v) {
                $result['aid'][] = $v['aid'];
                $result['hits'][$v['aid']] = $v['views'];
            }
            
            //缓存一小时
            $this->redis->set('index:news:hits:limit:' . $limit, $result, 3600);
        }

        $news = $this->db->select('news', ['id', 'catid', 'title', 'description'], ['AND' => ['status' => 99, 'id' => $result['aid']]]);

        foreach ($news as &$v) {
            $v['views'] = $result['hits'][$v['id']];
        }

        return $news;
    }

    /**
     * 获取文章列表
     * @param $cond array where条件 attribute=1 推荐 attribute=2 精华
     * @param $order string 排序条件 默认inputtimr DESC
     * @param $page int 页码
     * @param $limit 限制返回查询结果的条数
     * @param $memtime int 缓存时间
     * @param $mobile int 是否是手机端：1是手机端 ，0是pc
     * @return array 文章列表
     */
    public function articleList($cond, $order = 'inputtime DESC', $page = 1, $limit = 6, $memtime = 60, $mobile = 1) {
        
        $offset = max($page - 1, 0) * $limit;
        
        if ($mobile == 1) {
            $redisKey = 'index : mobile';
            $field = ['id', 'catid', 'title'];
        } else {
            $redisKey = 'index';
            $field = ['id', 'catid', 'title', 'description'];
        }
        
        if (isset($cond['catid'])) {
            $where['AND']['catid'] = $cond['catid'];
            $redisKey .= '.catid' . $cond['catid'];
        }
        
        if (isset($cond['attribute'])) {
            $where['AND']['attribute'] = intval($cond['attribute']);
            $redisKey .= '.attribute' . $cond['attribute'];
        }
        
        if (isset($cond['id'])) {
            $where['AND']['id'] = $cond['id'];
            if (is_array($cond['id'])){
                $redisKey .= ' .id' . implode('', $cond['id']);
            }else{
                $redisKey .= ' .id' . $cond['id'];
            }
        }

        $time = time();
        $time = $time - $time % 100;
        
        // $where['AND']['inputtime[<]'] = $time;
        $where['AND']['status'] = 99;
        $where['LIMIT'] = [$offset, $limit];
        $where['ORDER'] = $order;
        
        $redisKey .= '.limit' . ($offset + $limit) . $order;
        $data = $this->redis->get(md5($redisKey));
        
        if (!$data) {
            $data = $this->db->select('news', $field, $where);

            if ($data <> false) {
                // if ($mobile == 1) {//手机获取浏览量
                //     $data = $this->getView($data);
                // }
                $this->redis->set(md5($redisKey), $data,  $memtime);
            }
        }
        return $data;
    }

    /**
     * 获取文章列表（包括点击数）
     *
     * @param $cond array  where条件 attribute=1 推荐 attribute=2 精华
     * @param $order int 排序
     * @param $page int 页码
     * @param $limit  限制返回的查询结果数量
     * @param $memtime 缓存时间
     * @param int $mobile 是否手机：1是手机：0不是手机
     * @return array 文章列表
     */
    public function articleListDetail($cond, $order = 'inputtime DESC', $page = 1, $limit = 6, $memtime = 60, $mobile = 1) {

        $offset = max($page - 1, 0) * $limit;
        if ($mobile == 1) {
            $redisKey = 'index : mobile';
            $redisKey .= '.joinhits';
        } else {
            $redisKey = 'index';
        }
        
        if (isset($cond['catid'])) {
            $where['AND']['news.catid'] = $cond['catid'];
            $redisKey .= '.catid' . $cond['catid'];
        }
        
        if (isset($cond['attribute'])) {
            $where['AND']['attribute'] = intval($cond['attribute']);
            $redisKey .= '.attribute' . $cond['attribute'];
        }
        
        if (isset($cond['id'])) {
            $where['AND']['id'] = $cond['id'];
            if (is_array($cond['id'])){
                $redisKey .= ' .id' . implode('', $cond['id']);
            }else{
                $redisKey .= ' .id' . $cond['id'];
            }
        }

        $where['AND']['status'] = 99;
        $where['LIMIT'] = [$offset, $limit];
        $where['ORDER'] = $order;
        
        $redisKey .= '.limit' . ($offset + $limit) . $order;

        $data = false;

        $data = $this->redis->get(md5($redisKey));

        if (!$data) {
            // 手机获取浏览量
            // if ($mobile == 1) {
            //     $data = $this->db->select('news',  ['id', 'news.catid', 'title', 'description'], $where);
            // } else {
            //     $data = $this->db->select('news', ['id', 'catid', 'title'], $where);
            // }
            $data = $this->db->select('news', ['id', 'catid', 'title'], $where);

            $this->redis->set(md5($redisKey), $data,  $memtime);
        }

        return $data;
    }

    /**
     * 为data数组添加浏览量字段
     * @param $data array 带查询的数据
     * @return array 查询号浏览量的数据
     */
    public function getView($data) {
        foreach ($data as &$val) {
            $num = false;

            $num = $this->redis->get('article:hits' . $val['id']);
            
            if ($num == false) {
                $nums = $this->db->get('hits', ['views'], ['aid' => $val['id']]);
                $num = $nums == false ? 0 : $nums['views'];
                $this->redis->set('article:hits' . $val['id'], $num, 0, 3600);
            }

            $val['views'] = $num;
        }

        return $data;
    }

    /**
     * 为data数组添加图片字段
     * @param $data array 待处理的数据
     * @return array 处理好图片的数据
     */
    public function getPic($data) {

        foreach ($data as $val) {
            $arr[$val['id']] = $val;
            $pic = $this->redis->get('article:pic' . $val['id']);
            
            if ($pic == false){
                $id[] = $val['id'];
            }else{
                $arr[$val['id']]['pic'] = $pic;
            }
        }

        if (!empty($id)) {
            $data = $this->db->select('uploadfile', ['filepath', 'aid'], ['aid' => $id]);
            foreach ($data as $k => $v) {
                if (isset($arr[$v['aid']])) {
                    $this->redis->set('article:pic' . $v['aid'], $v['pic'],  3600 * 24 * 30);
                    $arr[$v['aid']]['pic'] = $v['filepath'];
                }
            }
        }
        return $arr;
    }

    /**
     * 根据data 获取数据中每条数据对应的分类id
     * @param $data array 要查询的数据
     * @return array
     */
    public function getCatname($data) {
        foreach ($data as &$val) {
            $num = false;

            $num = $this->redis->get('article:catpath' . $val['catid']);
            
            if ($num == false) {
                $nums = $this->db->get('category', ['catpath'], ['catid' => $val['catid']]);
                $num = $nums == false ? 0 : $nums['catpath'];
                $this->redis->set('article:catpath' . $val['catid'], $num, 0, 3600);
            }
            $val['catpath'] = $num;
        }
        return $data;
    }

    /**
     * 获取专题文章列表
     * @param $cond array 查询的条件
     * @param $order string 排序条件 默认 inputtime desc 
     * @param $page int 页码
     * @param $limit int 限制查询的条件 
     * @param $memtime int 缓存时间
     * @return array 返回专题文章列表
     */
    public function topiclist($cond, $order = 'inputtime DESC', $page = 1, $limit = 6, $memtime = 60) {
        $offset = max($page - 1, 0) * $limit;
        $redisKey = 'index';
        $field = ['id', 'tid', 'sid','title', 'description'];
        
        if (isset($cond['tid'])) {
            $where['AND']['tid'] = $cond['tid'];
            $redisKey .= '.tid' . $cond['tid'];
        }

        if (isset($cond['sid'])) {
            $where['AND']['sid'] = $cond['sid'];
            $redisKey .= '.sid' . $cond['sid'];
        }

        if (isset($cond['attribute'])) {
            $where['AND']['attribute'] = intval($cond['attribute']);
            $redisKey .= '.attribute' . $cond['attribute'];
        }

        if (isset($cond['id'])) {
            $where['AND']['id'] = $cond['id'];
            if (is_array($cond['id'])){
                $redisKey .= ' .id' . implode('', $cond['id']);
            }else{
                $redisKey .= ' .id' . $cond['id'];
            }
        }

        $time = time();
        $where['AND']['inputtime[<]'] = $time;
        $where['AND']['status'] = 1;
        $where['LIMIT'] = [$offset, $limit];
        $where['ORDER'] = $order;
        
        $redisKey .= '.limit' . ($offset + $limit) . $order;
        $data = $this->redis->get(md5($redisKey));
        
        if (!$data) {
            $data = $this->db->select('topic_data', $field, $where
            );

            if ($data <> false) {
                // if ($mobile == 1) {//手机获取浏览量
                //     $data = $this->getView($data);
                // }
                $this->redis->set(md5($redisKey), $data,  $memtime);
            }
        }
        
        return $data;
    }

}