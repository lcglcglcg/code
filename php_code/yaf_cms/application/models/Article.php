<?php

/**
 * 文章数据库操作类
 *
 * User: zhangxudong@lianchuangbrothers.com
 * Date: 14-11-14
 * Time: 11:12
 */
class ArticleModel extends BaseModel {

    /**
     * 初始化
     */
    function __construct() {
        parent::__construct();
        //导航
        $this->category = $this->category();
    }

    /**
     * 获取文章详情
     *
     * @param int $id
     * @return mixed;
     */
    public function detail($id) {
        $redisKey =  'article:detail' . $id;

        $news_table_name = $this->get_table_id($id);
        $news_data_table_name = $this->get_table_id($id,'news_data');
        $data = false;

        if (is_numeric($id)) {

            $data = $this->redis->get($redisKey);

            if ($data == false) {
                $detail = $this->db->get($news_table_name, [
                    'id',
                    'catid',
                    'title',
                    'keywords',
                    'description',
                    'listorder',
                    'uid',
                    'username',
                    'url',
                    'islink',
                    'inputtime',
                    'updatetime',
                    'en_keywords'
                        ], [
                    'AND' => [
                        'id' => $id,
                        'status' => 99
                    ],
                        ]
                );

                if ($detail <> false) {
                    $data = $this->db->get($news_data_table_name, [
                        'content',
                        'fromweb',
                            ], [
                        'id' => $id
                            ]
                    );
                    $data == false && $data = [];
                    $data = array_merge($detail, $data);
                    $data['description'] == false && $data['description'] = utf8::substr(strip_tags($data['content']), 0, 78) . '...';
                }

                $data <> false && $this->redis->set($redisKey, $data,  3603 );
            }
        }
        return $data;
    }

    /*
     * 获取文章列表
     * array $cond where条件 attribute=1 推荐 attribute=2 精华
     * int $start 分页起始页
     * int $offset 分页结束页
     * int $memtime 缓存时间
     * $order 排序
     * return array
     */
    public function articleList($cond, $start = 0, $offset = 10, $order = 'id DESC', $memtime = 300) {
        
        if(isset($cond['id[>]'])){
            $news_table_name = $this->get_table_id($cond['id[>]']);
        }elseif(isset($cond['id[<]'])){
            $news_table_name = $this->get_table_id($cond['id[<]']);
        }else{
            $news_table_name = 'news';
        }
        
        $redisKey =  'article:list';

        if (isset($cond['en_keywords'])) {
            $where['AND']['en_keywords'] = $cond['en_keywords'];
            $redisKey .= '.like' . $cond['en_keywords'];
        }
        if (isset($cond['inputtime[>]'])) {
            $where['AND']['inputtime[>]'] = $cond['inputtime[>]'];
            $redisKey .= '.inputtime' . $cond['inputtime[>]'];
        }
        if (isset($cond['catid'])) {
            $where['AND']['catid'] = $cond['catid'];
            $redisKey .= '.catid' . $cond['catid'];
        }
        if ($cond['id[>]']) {
            $where['AND']['id[>]'] = $cond['id[>]'];
            $redisKey .= 'id[>]' . $cond['id[>]'];
        }
        if ($cond['id[<]']) {
            $where['AND']['id[<]'] = $cond['id[<]'];
            $redisKey .= 'id[<]' . $cond['id[<]'];
        }
        if (isset($cond['attribute'])) {
            $where['AND']['attribute'] = intval($cond['attribute']);
            $redisKey .= '.attribute' . $cond['attribute'];
        }
        if (isset($cond['id'])) {
            $where['AND']['id'] = $cond['id'];
            if (is_array($cond['id']))
                $redisKey .= ' .id' . implode('', $cond['id']);
            else
                $redisKey .= ' .id' . $cond['id'];
        }

        if (isset($cond['startTime'])) {
            $where['AND']['inputtime[<>]'] = [$cond['startTime'], $cond['endTime']];
            $redisKey .= '.startTime' . $cond['startTime'];

            $news_table_name = $this->get_table_id_by_time($cond['startTime'], $cond['endTime']);

        } else {
            $time = time();
            $time = $time - $time % 100;
            //$where['AND']['inputtime[<]'] = $time;
        }

        $where['LIMIT'] = [$start, $offset];
        $where['ORDER'] = $order;
        $redisKey .= '.limit' . ($start + $offset) . $order;

        $data = false;

        $data = $this->redis->get(md5($redisKey));
        
        if ($data == false) {
            $data = $this->db->select($news_table_name, ['id', 'catid', 'title', 'keywords', 'description', 'inputtime'], $where
            );
            //echo $this->db->last_query();
            $data <> false && $this->redis->set(md5($redisKey), $data,  $memtime);
        }
        return $data;
    }

    //获取文章条数
    public function getTitleCount($cond) {
        $redisKey =  'index:gettitlecount';
        
        $news_table_name = 'news';

        if (isset($cond['en_keywords'])) {
            $where['AND']['en_keywords'] = $cond['en_keywords'];
            $redisKey .= '.like' . $cond['en_keywords'];
        }

        if (isset($cond['catid'])) {
            $where['AND'] = ['catid' => $cond['catid']];
            $redisKey .= '.catid' . $cond['catid'];
        }
        
        if (isset($cond['startTime'])) {
            $where['AND']['inputtime[<>]'] = [$cond['startTime'], $cond['endTime']];
            $redisKey .= '.startTime' . $cond['startTime'];
            $news_table_name = $this->get_table_id_by_time($cond['startTime'], $cond['endTime']);
        } else {
            $time = time();
            $time = $time - $time % 100;
            //排除当前以后的数据
            //$where['AND']['inputtime[<]'] = $time;
        }

        $where['AND']['status'] = 99;
        $data = $this->redis->get($redisKey);

        if ($data == false) {
            $data = $this->db->count($news_table_name, $where);
            $data <> false && $this->redis->set($redisKey, $data,  600);
        }
        return $data;
    }

    /**
     * 根据文章id获得该文章的上一篇下一篇
     * @param $inputtime int 时间
     * @param $cond array  查询的条件
     * @param $id int 文章的id
     * @return array 返回上一篇和下一篇的文章
     */
    public function preNext($inputtime = 0, $cond,$id)
    {
        $news_table_name = $this->get_table_id($id);

        $data = false;
        if (is_numeric($inputtime)) {
            $redisKey =  'article:prenext' . $inputtime;

            if (isset($cond['catid']) && $cond['catid']) {
                $where['AND'] = ['catid' => $cond['catid']];
                $redisKey .= '.catid' . $cond['catid'];
            }

            $where['AND']['status'] = 99;

            $data = $this->redis->get($redisKey);

            if ($data == false) {
                //上一篇
                $where['AND']['inputtime[<>]'] = [($inputtime + 1), ($inputtime + 46400)];
                $where['ORDER'] = 'inputtime ASC';
                $previous = $this->db->get($news_table_name, ['id', 'catid', 'title'], $where);
                
                if ($previous == false){
                    $data['previous'] = '没有了';
                }else{
                    $data['previous'] = '上一篇：<a href="/' . SITEDIR . '/' . $this->category[$previous['catid']]['catpath'] . '/' . $previous['id'] . '.html" title="' . $previous['title'] . '">' . $previous['title'] . '</a>';
                }

                //下一篇
                unset($where['AND']['inputtime[>]']);
                $where['ORDER'] = 'inputtime DESC';
                $where['AND']['inputtime[<>]'] = [($inputtime - 46400), ($inputtime - 1)];
                $next = $this->db->get($news_table_name, ['id', 'catid', 'title'], $where);
                
                if ($next == false){
                    $data['next'] = '没有了';
                }else{
                    $data['next'] = '下一篇：<a href="/' . SITEDIR . '/' . $this->category[$next['catid']]['catpath'] . '/' . $next['id'] . '.html" title="' . $next['title'] . '">' . $next['title'] . '</a>';
                }

                $this->redis->set($redisKey, $data,  3600 * 2);
            }
        }

        return $data;
    }

    /**
     * 热点阅读
     * @param $cond array  查询的条件
     * @param $limit int 
     */
    public function hothit($cond, $limit) {
        $redisKey =  'article:hothit';

        if (isset($cond['catid'])) {
            $where['AND']['catid'] = $cond['catid'];
            $redisKey .= '.catid' . $cond['catid'];
        }

        $where['LIMIT'] = $limit;
        $where['ORDER'] = 'views DESC';

        $redisKey .='.limit' . $limit;
        $aid = $this->redis->get($redisKey);

        if ($aid == false) {
            $aid = $this->db->select('hits', ['catid', 'aid', 'views'], $where);
            $this->redis->set($redisKey, $aid,  3600);
            $data = [];
            $val = [];
            
            if (!empty($aid)) {
                foreach ($aid as $ids) {
                    $id[] = $ids['aid'];
                    $val[$ids['aid']] = $ids['views'];
                }
                $data = $this->articleList(['id' => $id]);
                foreach ($data as &$d) {
                    $d['views'] = $val[$d['id']];
                }
            }
            return $data;
        }
    }

    /**
     * 获取文章列表（包括点击数）
     *
     * @param $cond array where条件 attribute=1 推荐 attribute=2 精华
     * @param $memtime int  缓存时间
     * @param $order string 排序
     * @param $field array  查询字段
     * @param $mobile int  是否手机:1表示手机，0表示pc
     * @return array 返回查询的数据
     */
    public function articleListDetail($cond, $order = 'inputtime DESC', $page = 1, $limit = 6, $memtime = 60, $mobile = 1) {

        $offset = max($page - 1, 0) * $limit;
        if ($mobile == 1) {
            $redisKey =  'index : mobile';
            $redisKey .= '.joinhits';
        } else {
            $redisKey =  'index';
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
            if (is_array($cond['id']))
                $redisKey .= ' .id' . implode('', $cond['id']);
            else
                $redisKey .= ' .id' . $cond['id'];
        }
        $time = time();
        $time = $time - $time % 100;
        $where['AND']['inputtime[<]'] = $time;
        $where['AND']['status'] = 99;
        $where['LIMIT'] = [$offset, $limit];
        $where['ORDER'] = $order;
        $redisKey .= '.limit' . ($offset + $limit) . $order;
        $data = $this->redis->get(md5($redisKey));

        //$data = false
        if ($data == false) {
            if ($mobile == 1) {//手机获取浏览量
                $data = $this->db->select('news', ["[>]hits" => ["id" => "aid"]], ['id', 'news.catid', 'title', 'description', 'views'], $where);
            } else {
                $data = $this->db->select('news', ['id', 'catid', 'title'], $where);
            }
            $this->redis->set(md5($redisKey), $data,  $memtime);
        }
        return $data;
    }

    /**
     * 为data数组添加图片字段
     * @param $data array 文章列表
     * @return array
     */
    public function getPic($data) {
        foreach ($data as $val) {
            $arr[$val['id']] = $val;
            $pic = $this->redis->get( 'article:pic' . $val['id']);
            if ($pic == false)
                $id[] = $val['id'];
            else
                $arr[$val['id']]['pic'] = $pic;
        }

        if (!empty($id)) {
            $data = $this->db->select('uploadfile', ['filepath', 'aid'], ['aid' => $id]);
            foreach ($data as $k => $v) {
                if (isset($arr[$v['aid']])) {
                    $this->redis->set( 'article:pic' . $v['aid'], $v['pic'],  3600 * 24 );
                    $arr[$v['aid']]['pic'] = $v['filepath'];
                }
            }
        }
        return $arr;
    }

    /**
     * foreach 遍历获取获取data文章列表中的文章的分类
     * @param $data array 文章列表
     * @return array
     */
    public function getCatname($data) {
        foreach ($data as &$val) {
            $num = false;

            $num = $this->redis->get( 'article:catpath' . $val['catid']);
            
            if ($num == false) {
                $nums = $this->db->get('category', ['catpath'], ['catid' => $val['catid']]);
                $num = $nums == false ? 0 : $nums['catpath'];
                $this->redis->set( 'article:catpath' . $val['catid'], $num, 0, 3600);
            }
            $val['catpath'] = $num;
        }
        return $data;
    }

    /**
     * 更新文章点击量
     * @param $id int 文章的id
     * @param $cid int 文章分类的id
     * @return int 本操作更新所影响的行数
     */
    public function updateViews($id, $cid) {
        if ($this->db->get('hits', ['views'], ['aid' => $id])) {
            return $this->db_write->update("hits", ["views[+]" => 1], ["aid" => $id]);
        } else {
            return $this->db_write->insert("hits", ["aid" => $id, "views" => 1, "catid" => $cid]);
        }
    }

    /**
     * 获取关键词相关列表
     * @param $key string 关键词key
     * @param $start int 分页开始id
     * @param $offset int 分页的偏移量
     * @param $order string where条件中ORDER BY 排序的条件,默认根据inputtime DESC排序
     * @return array 和关键字相关的文章列表
     */
    public function getKeyList($key, $start = 0, $offset = 20, $order = 'inputtime DESC') {
        $where['AND']['keyword_path'] = $key;
        $where['LIMIT'] = [$start, $offset];
        $where['ORDER'] = $order;
        if ($key == false) {
            $data = $this->db->select('keywords_data', ['id', 'catid', 'title', 'keywords', 'description', 'inputtime'], $where
            );
        }
        return $data;
    }

    /**
     * 获得文章关键词中的数据
     */
    public function get_keywords() {

        return $this->select('keywords', ['keywords']);
    }

    /**
     * 替换文章关键词敏感字
     * @param $words string 要替换的词语
     * @return string 返回替换之后的字符串
     */
    public function replace_keywords($words) {
        $replacewords = $this->get('keywords', ['replace_words'], ['keywords' => $words]);
        return $replacewords['replace_words'];
    }

    /**
     * 获得内链关键词表中的数据
     * @return 返回关键词数据
     */
    public function get_key_datas() {
        return $this->select('content_keywords', ['keywords']);
    }

    /**
     * 替换关键词的链接
     * @param $key string 要替换关键词连接
     * @return string 返回替换好的关键词链接
     */
    public function replace_key_url($key) {
        return $this->get('content_keywords', ['href_url'], ['keywords' => $key]);
    }

    /**
     * TAG功能
     * @param $data array 要处理的数据
     * @param $keywords string 关键字
     * @return 
     */
    public function tag($data, $keywords) {
        $redisKey =  'tag' . $keywords;
        $tag = $this->redis->get($redisKey);
        
        if ($tag) {
            return $tag;
        }
        
        foreach ($data as $v) {
            if (strpos($v['keywords'], ' ')) {
                $_keywords = explode(' ', $v['keywords']);
                foreach ($_keywords as $value) {
                    if (strcmp(Pinyin::get($value), $keywords) === 0) {
                        $tag = $value;
                        $this->redis->set($redisKey, $tag,  3600);
                        return $tag;
                    }
                }
            } else if (strcmp(Pinyin::get($v['keywords']), $keywords) === 0) {
                $tag = trim($v['keywords']);
                $this->redis->set($redisKey, $tag,  3600);
                return $tag;
            } else if (strpos(Pinyin::get($v['keywords']), $keywords)) {
                $tag = misc::getKeywords($v['keywords']);
                $this->redis->set($redisKey, $tag[0],  3600);
                return $tag[0];
            }
        }
    }
    
    //获取search页数据
    public function searchList($cond, $start = 0, $offset = 10, $memtime = 300) {
        
        $redisKey =  'article:searchlist';

        if (isset($cond['LIKE'])) {
            $where['LIKE']['en_keywords'] = $cond['LIKE'];
            $redisKey .= '.like' . $cond['LIKE']['en_keywords'];
        }
        if (isset($cond['inputtime[>]'])) {
            $where['AND']['inputtime[>]'] = $cond['inputtime[>]'];
            $redisKey .= '.inputtime' . $cond['inputtime[>]'];
        }

        $where['LIMIT'] = [$start, $offset];
        $redisKey .= '.limit' . ($start + $offset) . $order;

        $data = false;
        $data = $this->redis->get(md5($redisKey));
        if ($data == false) {
            $data = $this->db->select('keywords_data',['id', 'catpath', 'title', 'keywords', 'inputtime','en_keywords'],$where);
            $data <> false && $this->redis->set(md5($redisKey), $data,  $memtime);
        }
        return $data;
    }

}
