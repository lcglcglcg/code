<?php

if (!defined('APP_PATH'))
    exit('No direct script access allowed');

// 加载 XS 入口文件
require_once '/Data/apps/xunsearch/sdk/php/lib/XS.php';

class SearchController extends BaseController {

    /**
     * 文章列表页
     * dir----dirname
     */
    public function IndexAction($keywords = '', $page = 1) {

        // 参数列表
        // q: 查询语句
        // m: 开启模糊搜索，其值为 yes/no
        // f: 只搜索某个字段，其值为字段名称，要求该字段的索引方式为 self/both
        // s: 排序字段名称及方式，其值形式为：xxx_ASC 或 xxx_DESC
        // p: 显示第几页，每页数量为 XSSearch::PAGE_SIZE 即 10 条
        // ie: 查询语句编码，默认为 UTF-8
        // oe: 输出编码，默认为 UTF-8
        // xml: 是否将搜索结果以 XML 格式输出，其值为 yes/no
        //
        // variables

        //实例化文章数据库操作类
        $article = new ArticleModel();
        //关键字
        $keywords = core::safe_str(rtrim($keywords));

        //拼接字段参数
        $eu = '';
        $syn = '';

        //因杭州站点特殊需求，以下为手动填写
        $q = $keywords;
        $f = 'en_keywords';
        $m = 'yes';
        $s = 'relevance';
        $cs = 'UTF-8';
        $ie = 'UTF-8';


        // url里的参数转utf8
        if (!empty($ie) && !empty($q) && strcasecmp($ie, 'UTF-8')) {
            $q = XS::convert($q, $cs, $ie);
            $eu .= '&ie=' . $ie;
        }

        // 输出字符集
        if (!empty($oe) && strcasecmp($oe, 'UTF-8')) {

            function xs_output_encoding($buf)
            {
                return XS::convert($buf, $GLOBALS['oe'], 'UTF-8');
            }
            ob_start('xs_output_encoding');
            $eu .= '&oe=' . $oe;
        } else {
            $oe = 'UTF-8';
        }

        // 杭州站点单独处理，没有用到上面的get参数，以下为手动填写
        $q = get_magic_quotes_gpc() ? stripslashes($q) : $q;
        ${'m_check'} = ($m == 'yes' ? ' checked' : '');
        ${'syn_check'} = ($syn == 'yes' ? ' checked' : '');
        ${'f_' . $f} = ' checked';
        ${'s_' . $s} = ' selected';

        // 基本url路径
        $bu = urlencode($q);

        // other variable maybe used in tpl
        $count = $total = $search_cost = 0;
        $docs = $related = $corrected = $hot = array();
        $error = $pager = '';


        // 搜索结果
        try {
            $xs = new XS('hangzhou');
            $search = $xs->search;
            $search->setCharset('UTF-8');

            if (empty($q)) {
                // 如果为空则显示最热搜索，杭州站点目前不做该展示
                $hot = $search->getHotQuery();
            } else {
                // 启动模糊搜索
                $search->setFuzzy($m === 'yes');

                // 同义词搜索
                $search->setAutoSynonyms($syn === 'yes');

                // 设定查询条件
                if (!empty($f) && $f != '_all') {
                    $search->setQuery($f . ':(' . $q . ')');
                } else {
                    $search->setQuery($q);
                }

                // 排序
                if (($pos = strrpos($s, '_')) !== false) {
                    $sf = substr($s, 0, $pos);
                    $st = substr($s, $pos + 1);
                    $search->setSort($sf, $st === 'ASC');
                }

                // 分页处理（杭州站点暂不做分页处理）
                $p=1;
                $p = max(1, intval($p));
                $n = 10;
                $search->setLimit($n, ($p - 1) * $n);

                // 获得查询结果
                $docs = $search->search();


                // 总数查询
                $total = $search->getDbTotal();

            }
        } catch (XSException $e) {
            //捕获错误信息
            $error = strval($e);
            //暂不打印错误信息。。。
        }


        //最新
        $data['new'] = $article->articleList([], 0, 30);

        //取tag关键字中文
        $tag = $docs[0]['keywords'];



        $this->getView()->assign("docs", $docs);
        $this->getView()->assign("totalpage", $totalpage);
        $this->getView()->assign("total", $total);
        $this->getView()->assign("page", $pager);
        $this->getView()->assign("title", '关键词“' . $tag . '”');
        $this->getView()->assign("keywords", $tag);
        $this->getView()->assign("description", '关键词“' . $tag . '”的相关文章');
    }

}
