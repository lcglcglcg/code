<?php if (!defined('APP_PATH')) exit('No direct script access allowed');

/**
 * Created by PhpStorm.
 * User: kele
 * Date: 14-4-12
 * Time: 上午10:16
 * sphinx调用接口
 * 帮助文档:http://www.coreseek.cn/docs/coreseek_4.1-sphinx_2.0.1-beta.html
 * 官网:http://sphinxsearch.com/
 */
class sphinxsearch
{
    /**
     * @var string $host 主机
     */
    private $host;

    /**
     * @var string $port 端口
     */
    private $port;

    /**
     * @var array $indexes 索引的名字
     *
     * $this->indexes should look like:
     *
     * $this->indexes = array(
     *   'IndexLabel' => 'Index name as defined in sphinxsearch.conf',
     *   ...,
     * );
     */
    private $indexes;

    /**
     * @var SphinxClient $sphinx
     */
    private $sphinx;

    /**
     * @var string $search_string 搜索的关键字
     */
    protected $search_string;

    /**
     * @var string $index_name 索引名字
     */
    protected $index_name;

    /**
     * @var string $total_count 总共多少条
     */
    protected $total_count;

    /**
     * @var string $time 耗时
     */
    protected $time;

    /**
     * @var string $httpcws_url 中文分词的url
     */
    protected $httpcws_url;

    /**
     * Constructor.
     *
     * @param string $host The server's host name/IP.
     * @param string $port The port that the server is listening on.
     * @param array $indexes The list of indexes that can be used.
     */
    function __construct()
    {
        //sphinx
        $sphinxconf = Yaf_Registry::get("config")->get('sphinx')->toArray();
        $this->host = $sphinxconf['host'];
        $this->port = $sphinxconf['port'];
        $this->indexes = $sphinxconf['indexes'];
        $this->sphinx = new SphinxClient();

        $this->sphinx->SetServer($this->host, $this->port);
        //sphinx连接超时时间，单位为秒
        $this->sphinx->SetConnectTimeout(5);
        //sphinx搜索结果集返回方式：TRUE为普通数组返回，FALSE为PHP hash格式返回
        $this->sphinx->SetArrayResult(false);
        //sphinx最大搜索时间，单位为毫秒
        $this->sphinx->SetMaxQueryTime(5000);
        //匹配模式
        $this->sphinx->SetMatchMode(SPH_MATCH_EXTENDED2);
        //设置权重评分模式，详见sphinx文档第6.3.2节：SetRankingMode
        $this->sphinx->SetRankingMode(SPH_RANK_PROXIMITY_BM25);
        //设置排序模式排序模式
        $this->sphinx->SetSortMode(SPH_SORT_EXTENDED, '@weight DESC,@id DESC');
        //中文分词配置
        $httpcwsconf = Yaf_Registry::get("config")->get('httpcws')->toArray();
        //中文分词HTTPCWS服务器接口地址
        $this->httpcws_url = 'http://'.$httpcwsconf['host'].':'.$httpcwsconf['port'].'/?w=';

    }

    /*
     * 设置搜索字和索引名字
     * $string 搜索的的关键字
     * $index_name sphinx里设置的index名  当是string的时候就是一个索引  数组的时候就是多个索引查询
     */
    function search($string, $index_name = NULL)
    {
        $string = urldecode($string);
        //用PHPCWS中文分词扩展对输入的关键字进行切分
        if (mb_strlen($string, "UTF-8") > 4) { //大于4个汉字的关键词才调用中文分词接口
            $string = mb_convert_encoding($string, "GBK", "UTF-8");
            $ch = curl_init();
            curl_setopt($ch, CURLOPT_URL, $this->httpcws_url . urlencode($string));
            curl_setopt($ch, CURLOPT_TIMEOUT, 3); //3秒超时时间
            curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1); //返回内容不是输出
            $string_tmp = curl_exec($ch);
            curl_close($ch);
            if ($string_tmp != "") {
                $string = mb_convert_encoding($string_tmp, "UTF-8", "GBK");
            }
        }

        $sphinx_query_oldarr = explode(" ", $string);
        foreach ($sphinx_query_oldarr as $key => $value) {
            if (substr($value, 0, 1) == "@" || $value == "|") {
                $sphinx_query_newarr[$key] = $value; //不加引号
            } else {
                if ($this->is_chinese($value, "GBK") == 1 || $this->is_chinese($value, "GBK") == 2) { //如果该字符串全部是中文、中英文混合
                    $sphinx_query_newarr[$key] = "\"" . $value . "\""; //关键字加上引号
                } else {
                    $sphinx_query_newarr[$key] = $value; //如果不是汉字则不加引号
                }
            }
        }
        $this->search_string = implode(" ", $sphinx_query_newarr);
        unset($sphinx_query_oldarr, $sphinx_query_newarr);
        if (NULL !== $index_name) {
            //多个索引查询
            if (is_array($index_name)) {
                $index_names = '';
                foreach ($index_name as &$label) {
                    if (isset($this->indexes[$label]))
                        $index_names .= $this->indexes[$label] . ' ';
                }
                $this->index_name = $index_names;
            } else {
                if (isset($this->indexes[$index_name]))
                    $this->index_name = $this->indexes[$index_name];
            }

        }
        if (empty($this->index_name))
            exit('没有设置该' . $this->index_name . '索引' . PHP_EOL);
        //清除当前设置的过滤器
        $this->sphinx->ResetFilters();

        return $this;
    }

    /**
     * 设置字段权重.  $weights should look like:
     *
     * $weights = array(
     *   'Normal field name' => 1,
     *   'Important field name' => 10,
     * );
     *
     * @param array $weights Array of field weights.
     */
    function setFieldWeights($weights)
    {
        $this->sphinx->SetFieldWeights($weights);
        return $this;
    }

    /**
     * 设置全文查询的匹配模式
     * SPH_MATCH_ALL, 匹配所有查询词(默认模式);
     * SPH_MATCH_ANY, 匹配查询词中的任意一个;
     * SPH_MATCH_PHRASE, 将整个查询看作一个词组，要求按顺序完整匹配;
     * SPH_MATCH_BOOLEAN, 将查询看作一个布尔表达式 (参见 第 5.2 节 “布尔查询语法”);
     * SPH_MATCH_EXTENDED, 将查询看作一个CoreSeek/Sphinx内部查询语言的表达式 (参见 第 5.3 节 “扩展查询语法”). 从版本Coreseek 3/Sphinx 0.9.9开始, 这个选项被选项SPH_MATCH_EXTENDED2代替，它提供了更多功能和更佳的性能。保留这个选项是为了与遗留的旧代码兼容——这样即使Sphinx及其组件包括API升级的时候，旧的应用程序代码还能够继续工作。
     * SPH_MATCH_EXTENDED2, 使用第二版的“扩展匹配模式”对查询进行匹配.
     * SPH_MATCH_FULLSCAN, 强制使用下文所述的“完整扫描”模式来对查询进行匹配。注意，在此模式下，所有的查询词都被忽略，尽管过滤器、过滤器范围以及分组仍然起作用，但任何文本匹配都不会发生.
     *
     * @param int $mode The matching mode to be used.
     */
    function setMatchMode($mode)
    {
        $this->sphinx->SetMatchMode($mode);
        return $this;
    }

    //设置索引权重
    function setIndexWeights($weights)
    {
        $this->sphinx->SetIndexWeights($weights);
        return $this;

    }

    /*
     * 设置查询ID范围
     * 设置接受的文档ID范围。参数必须是整数。默认是0和0，意思是不限制范围。
       此调用执行后，只有ID在$min和$max（包括$min和$max）之间的文档会被匹配
     */
    function setIDRange($min, $max)
    {
        $this->sphinx->SetIDRange($min, $max);
        return $this;
    }

    /**
     * 转义特殊字符
     *
     * @param string $string The string to be escaped.
     *
     * @return string The escaped string.
     */
    function escapeString($string)
    {
        return $this->sphinx->EscapeString($string);
    }

    //设置评分模式
    function setRankingMode($mode)
    {
        $this->sphinx->SetRankingMode($mode);
        return $this;
    }

    /*设置排序模式
    SPH_SORT_RELEVANCE 模式, 按相关度降序排列（最好的匹配排在最前面）
    SPH_SORT_ATTR_DESC 模式, 按属性降序排列 （属性值越大的越是排在前面）
    SPH_SORT_ATTR_ASC 模式, 按属性升序排列（属性值越小的越是排在前面）
    SPH_SORT_TIME_SEGMENTS 模式, 先按时间段（最近一小时/天/周/月）降序，再按相关度降序
    SPH_SORT_EXTENDED 模式, 按一种类似SQL的方式将列组合起来，升序或降序排列。
    SPH_SORT_EXPR 模式，按某个算术表达式排序
    */
    function setSortMode($mode, $par = NULL)
    {
        $this->sphinx->SetSortMode($mode, $par);
        return $this;
    }


    /*
     * 设置地表距离锚点
     * $attrlat 和 $attrlong是字符串，分别指定了对应经度和纬度的属性名称。$lat 和 $long是浮点值，指定了锚点的经度和纬度值，以角度为单位。
     * 一旦设置了锚点，您就可以在您的过滤器和/或排序表达式中使用"@geodist"特殊属性。Sphinx将在每一次全文检索中计算给定经纬度与锚点之前的地表距离，并把此距离附加到匹配结果上去。SetGeoAnchor和索引属性数据中的经纬度值都是角度。而结果会以米为单位返回，因此地表距离1000.0代表1千米。一英里大约是1609.344米。
     */
    function setGeoAnchor($attrlat, $attrlong, $lat = null, $long = null)
    {
        $this->sphinx->SetGeoAnchor($attrlat, $attrlong, $lat, $long);
        return $this;
    }

    /**
     * 设置结果集偏移量
     * 给服务器端结果集设置一个偏移量（$offset）和从那个偏移量起向客户端返回的匹配项数目限制（$limit）。并且可以在服务器端设定当前查询的结果集大小（$max_matches），另有一个阈值（$cutoff），当找到的匹配项达到这个阀值时就停止搜索。全部这些参数都必须是非负整数。
     */
    function limit($limit, $offset = 0, $max_matches = 1000, $cutoff = 1000)
    {
        $this->sphinx->SetLimits($offset, $limit, $max_matches, $cutoff);
        return $this;
    }

    /**
     * 设置属性过滤
     * $attribute, $values, $exclude = FALSE
     * 此调用在已有的过滤器列表中添加新的过滤器。$attribute是属性名。$values是整数数组。$exclude是布尔值，它控制是接受匹配的文档（默认模式，即$exclude为false时）还是拒绝它们。
     * 只有当索引中$attribute列的值与$values中的任一值匹配时文档才会被匹配（或者拒绝，如果$exclude值为true）
     */
    function filter($filter)
    {
        if (is_array($filter) && count($filter) > 0) {
            foreach ($filter as $attribute => $v) {
                list($values, $exclude) = explode(',', $v);
                $sphinx_int_array = explode("_", $values);
                $this->sphinx->SetFilter($attribute, $sphinx_int_array, constant($exclude));
            }
        }

        return $this;
    }

    /*
     * 设置属性范围
     * 此调用在已有的过滤器列表中添加新的过滤器。$attribute是属性名, $min 、$max定义了一个整数闭区间，$exclude布尔值，它控制是接受匹配的文档（默认模式，即$exclude为false时）还是拒绝它们。

     * 只有当索引中$attribute列的值落在$min 和 $max之间（包括$min 和 $max），文档才会被匹配（或者拒绝，如果$exclude值为true）。
     */
    function range($attribute, $min, $max, $exclude = FALSE)
    {
        $this->sphinx->SetFilterRange($attribute, $min, $max, $exclude);
        return $this;
    }

    /**
     * 设置浮点数范围
     * 此调用在已有的过滤器列表中添加新的过滤器。$attribute是属性名, $min 、$max定义了一个浮点数闭区间，$exclude必须是布尔值，它控制是接受匹配的文档（默认模式，即$exclude为false时）还是拒绝它们。
     * 只有当索引中$attribute列的值落在$min 和 $max之间（包括$min 和 $max），文档才会被匹配（或者拒绝，如果$exclude值为true）。
     */
    function floatRange($attribute, $min, $max, $exclude = false)
    {
        $this->sphinx->SetFilterFloatRange($attribute, $min, $max, $exclude);
        return $this;
    }

    /*
     * 设置分组的属性
     * $attribute是字符串，为进行分组的属性名。$func为常数，它指定内建函数，该函数以前面所述的分组属性的值为输入，目前的可选的值为： SPH_GROUPBY_DAY、SPH_GROUPBY_WEEK、 SPH_GROUPBY_MONTH、 SPH_GROUPBY_YEAR、SPH_GROUPBY_ATTR 。 $groupsort 是控制分组如何排序的子句。其语法与第 5.6 节 “SPH_SORT_EXTENDED mode”中描述的相似。
     */
    function group($attribute, $func, $groupsort = "@group desc")
    {
        $this->sphinx->SetGroupBy($attribute, $func, $groupsort);
        return $this;
    }

    //清楚现有的分组设置
    function resetgroup()
    {
        $this->sphinx->ResetGroupBy();
    }

    /*
     * 设置返回信息的内容
     * SetSelect()于标准SQL查询中SELECT和FROM之间的部分非常相近。它允许你指定要取出哪些属性（列），以及在这些列上要计算和取出哪些表达式。与SQL语言的区别是，表达式必须用关键字AS给每个表达式取一个别名，别名必须是有效的标识符（由字母和数字组成）。在SQL里面可以这样做，但是不是强制的。Sphinx强制必须有别名，以便计算结果总是可以以一个“正常”的名字在结果集中返回，或者在其他子句中引用，等等。
        其他方面基本上等同于SQL。支持星号（“*”），支持函数，支持任意数目的表达式。计算出的表达式可以用于排序、过滤和分组，这与其他常规属性相同。
     */
    function select($clause)
    {
        $this->sphinx->SetSelect($clause);
        return $this;

    }

    function get()
    {
        $this->total_count = 0;
        $result = $this->sphinx->Query($this->search_string, $this->index_name);
        if ($result['status'] !== SEARCHD_OK)
            throw new Exception(sprintf('Searching index "%s" for "%s" failed with error "%s".', $this->index_name, $this->search_string, $this->getErrorMessage()));
        // Get total count of existing results.
        $this->total_count = (int)$result['total_found'];
        // Get time taken for search.
        $this->time = $result['time'];
        $data = [];
        if ($result['total'] > 0 && isset($result['matches'])) {
            $data['time'] = $result['time'];
            $data['total'] = $result['total_found'];
            $i=0;
            foreach ($result['matches'] as $k => $v) {
                $data['data'][$i++] = $v['attrs'];
            }
        }
        unset($result);
        return $data;
    }

    /**
     * 增加批量查询
     *
     * @param string $query The query string that we are searching for.
     * @param array $indexes The indexes to perform the search on.
     */
    function addQuery($query, array $indexes)
    {
        $indexNames = '';
        foreach ($indexes as &$label) {
            if (isset($this->indexes[$label]))
                $indexNames .= $this->indexes[$label] . ' ';
        }

        if (!empty($indexNames))
            $this->sphinx->AddQuery($query, $indexNames);
    }

    /**
     * 执行批量查询
     *
     * @return array The results of the queries.
     */
    function runQueries()
    {
        return $this->sphinx->RunQueries();
    }

    //总共多少条
    function getTotalCount()
    {
        return $this->total_count;
    }

    //执行时间
    function getTime()
    {
        return $this->time;
    }

    //错误信息
    function getErrorMessage()
    {
        return $this->sphinx->GetLastError();
    }

    //警告信息
    function getLastWarning()
    {
        return $this->sphinx->GetLastWarning();
    }

    /*     * *******************************************
     * 函数名：is_chinese($string, $coding)
     * 函数用途：判断一个字符串是否为中文、中英文混合、纯英文字符串
     * 参数说明：
     *    $string   字符串      $coding 字符集编码，例如GB2312、UTF-8
     * 返回值：
     *    0表示未知，1表示中文，2表示中英文混合，3纯英文
     * ******************************************* */
    function is_chinese($string, $coding)
    {
        $en_len = strlen($string);
        $mb_len = mb_strlen($string, $coding);
        if ($en_len != $mb_len && ($en_len % $mb_len == 0)) {
            $result = 1; //中文
        } else if ($en_len != $mb_len && ($en_len % $mb_len != 0)) {
            $result = 2; //中英文混合
        } else if ($en_len == $mb_len) {
            $result = 3; //纯英文
        } else {
            $result = 0; //未知
        }
        return $result;
    }
}
