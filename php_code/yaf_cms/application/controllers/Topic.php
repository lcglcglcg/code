<?php

if (!defined('APP_PATH'))
    exit('No direct script access allowed');

/**
 * 前台专题模块
 */
class TopicController extends BaseController {

    public function init() {
        //实例化首页model层
        $this->topic = new TopicModel();
    }

    /**
     * 专题首页
     */
    public function index($dir = '') {
        if (!$dir) {
            misc::show404();
        }

        //获取专题信息
        $topic = $this->topic->get_topic_detail($dir);
        if (!$topic) {
            misc::show404();
        }

        $topic_id = (int) $topic['topic_id'];

        if ($topic['show_data']) {
            $show_data = json_decode($topic['show_data'], true);
        }

        $data = [];
        $field = ["id", "title", "content", "tid", "sid", "description", "keywords", "inputtime"];
        //获取子栏目数据
        $data = $this->getlistdata($show_data['data'], $topic_id);
        //获取基础信息
        $data['base'] = $this->getbasedata($show_data['base'], $topic_id);
        $data['title'] = $topic['topic_title'];
        $data['keywords'] = $topic['topic_keywords'];
        $data['description'] = $topic['topic_description'];

        //获取子类
        $subclass_list = $this->topic->get_subclass_list('topic_subclass', '*', ['tid' => $topic_id]);

        return $data;
    }

    /**
     * 专题列表页面
     */
    public function topiclist($dir = '', $subclass = '', $page = 1) {

        //判断参数是否正常
        if (!($dir && $subclass && $page)) {
            misc::show404();
        }

        //查询专题信息
        $topic_id = (int) $this->topic->get("topic", "topic_id", [
                    "AND" => ["is_show" => 1, "topic_path" => $dir],
                    "LIMIT" => 1
        ]);

        if (!$topic_id) {
            misc::show404();
        }

        //获取专题子类信息
        $subclass_data = $this->topic->get("topic_subclass", ["id", "parent_id", "title", "path", "keywords", "description", "show_data", "username"], [
            "AND" => ["tid" => $topic_id, "path" => $subclass],
            "LIMIT" => 1
        ]);

        if (!$subclass_data || $subclass_data['status' != 1]) {
            misc::show404();
        }



        //判断专题子类的show_data 是否为空 如果不为空，则本专题页显示的数据以show_data 为主
        //如果show_data为空，则以本专题的父级show_data为准
        if ($subclass_data['show_data']) {
            $show_data = $subclass_data['show_data'];
        } else {
            $show_data = $this->getshowdata($subclass_data["pid"], "show_data");
        }

        $show_data = json_decode($show_data, true);
        unset($subclass_data['show_data']);
        $data = [];
        $data['sub'] = $subclass_data;

        //载入配置 获取数据 如：相关文章/推荐文章/热门文章/精华文章
        $data['base'] = $this->getbasedata($show_data['base'], $topic_id);
        //获取要显示其他子类的文章列表
        $data['data'] = $this->getlistdata($show_data['data'], $topic_id);
        
        //注意要分页的问题
        $data['prepage'] = isset($show_data['setting']['prepage']) && $show_data['setting']['prepage'] ? $show_data['setting']['prepage'] : 10;

        //获取总的文章数量
        $data['total'] = $this->topic->count_subclass_posts($subclass_data['id']);

        //计算总的页码数量
        $data['totalpage'] = ceil($data['total'] / $data['prepage']);

        //判断page 是否合法
        if ($page < 1) {
            $page = 1;
        }

        if ($page > $data['totalpage']) {
            $page = $data['totalpage'];
        }

        //开始分页处理
        $start = ($page - 1) * $data['prepage'];

        //获取该专题子类下的文章列表
        $data['list'] = $this->topic->select("topic_data", ["id", "title", "content", "tid", "sid", "description", "keywords", "inputtime"], ["AND" => ["status" => 1, "sid" => $subclass_data['id'], "tid" => $topic_id], 'LIMIT' => [$start, $data['prepage']]]);
        return $data;
    }

    /**
     * 专题内容页面
     */
    public function content($dir = '', $subdir = '', $id = 0) {
        if (!($dir && $subdir && $id)) {
            misc::show404();
        }

        //根据$dir 获取 topic_id
        $topic_id = (int) $this->topic->get("topic", "topic_id", [
                    "AND" => ["topic_path" => $dir, "is_show" => 1],
                    "LIMIT" => 1
        ]);

        if (!$topic_id) {
            misc::show404();
        }
        $data['subclass_listinfo'] = $this->select_subclass_list($topic_id);
        //根据$subclass 获取 子类id
        $data = [];
        $data['sub'] = $subclass = $this->topic->get("topic_subclass", ["id", "parent_id", "title", "path", "keywords", "description", "show_data", "username"], [
            "AND" => ["path" => $subdir, "tid" => $topic_id],
            "LIMIT" => 1
        ]);

        //根据id 查找文章的所有的sid。并取出content_data（也就是内容页面需要展示的数据）
        //如果content_data 为空 则通过查询该sid 的parent_id，以此来获取父级的content_data
        $show_data = $subclass["content_data"] ? $subclass["content_data"] : $this->getshowdata($subclass["id"], "content_data");
        $show_data = json_decode($show_data, true);
        unset($show_data['content_data']);

        //载入当前专题内容页相关配置选项 并获取数据

        if (!empty($show_data['base'])) {
            $data['base'] = $this->getbasedata($show_data['base'], $topic_id);
        }

        //获取文章详情
        $data['data'] = $this->topic->get("topic_data", ["id", "title", "content", "keywords", "description", "inputtime", "username"], [
            "AND" => ["id" => $id, "tid" => $topic_id, "status" => 1, 'sid' => (int) $subclass["id"]],
            "LIMIT" => 1
        ]);

        if (!$data['data']) {
            misc::show404();
        }

        //获取上一页 和 下一页
        $data['prenext'] = $this->topic->preNext($data['data']['inputtime'], ['sid' => (int) $subclass['id'], 'tid' => $topic_id], $dir, $subdir);

        //获取模板信息
        $template = isset($show_data['setting']['template']) && $show_data['setting']['template'] ? $show_data['setting']['template'] : 'content.phtml';
        //获取数据
        return $data;
    }

    /**
     * 获取subclass的show_data
     */
    public function getshowdata($pid, $field) {
        if ($pid != 0) {
            $tmp = $this->topic->get("topic_subclass", ["parent_id", $field], ["id" => $pid]);
            if ($tmp[$field]) {
                return $tmp[$field];
            } else {
                $this->getshowdata($tmp['parent_id'], $field);
            }
        }
    }

    /**
     * 获取基础数据（相关文章/热门文章/精华文章/推荐文章）
     */
    public function getbasedata($data, $topic_id, $field = '') {
        $field = $field ? $field : ["id", "title", "content", "tid", "sid", "description", "keywords", "inputtime"];
        $arr = [];
        $where["AND"]["tid"] = $topic_id;
        $where["AND"]["status"] = 1;
        // 获取 基础信息
        foreach ($data as $k => $v) {
            $where["AND"]["tid"] = $topic_id;
            $where["AND"]["status"] = 1;
            if ($v['status'] == 1) {
                switch ($k) {
                    case 'zxwz'://最新文章	    				
                        $where["ORDER"] = "inputtime DESC";
                        break;
                    case 'tjwz'://推荐文章
                        $where["AND"]['attribute'] = 1;
                        $where["ORDER"] = "inputtime DESC";
                        break;
                    case 'jhwz'://精华文章
                        $where["AND"]["attribute"] = 2;
                        $where["ORDER"] = "inputtime DESC";
                        break;
                    case 'rdyd'://热点阅读
                        $where["AND"]["attribute"] = 1;
                        $where["ORDER"] = "inputtime DESC";
                        break;
                    case 'xgwz':
                        $where["ORDER"] = "inputtime DESC";
                        break;
                    default:
                        $where["AND"][$v[$field]] = $v["value"];
                        $where["ORDER"] = "inputtime DESC";
                        break;
                }

                $where["LIMIT"] = $v["num"];
                $arr[$k] = $this->topic->select("topic_data", $field, $where);
                //判断是否需要让数据随机？

                unset($where);
            }
        }

        return $arr;
    }

    /**
     * 获取列表数据
     */
    public function getlistdata($data, $topic_id, $field = '') {
        $field = $field ? $field : ["id", "title", "content", "tid", "sid", "description", "keywords", "inputtime"];
        $arr = [];

        //获取子栏目数据
        foreach ($data as $k => $v) {
            $arr[$v['path']] = $this->topic->select("topic_data", $field, [
                "AND" => ["tid" => $topic_id, "status" => 1, "sid" => $v["sid"]],
                "LIMIT" => $v["num"],
                "ORDER" => "inputtime DESC"
            ]);
        }

        return $arr;
    }

    /**
     * 查询所有的专题子栏目列表
     */
    public function select_subclass_list($topic_id) {
        $data = $this->topic->select_subclass_list($topic_id);
        $tmp = array();

        foreach ($data as $v) {
            $tmp[$v['id']] = $v;
        }

        return $tmp;
    }

}

?>