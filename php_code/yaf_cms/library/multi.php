<?php
/**
 * 分页类
 * User: kele
 * Date: 13-10-22
 * Time: 上午9:30
 * To change this template use File | Settings | File Templates.
 */
//分页类
class multi
{
//分页
    /**
     *
     * @param String $url
     * @param 后缀 $end_fix
     * @param 总页数 Integer $totalPages
     * @param 当前页 Integer $currentPage
     * @param 最大显示页 Integer $maxPage
     * @param 附加 [optional] string $descriptions
     * @param 附加一 [optional] string $desp1
     * @param 附加二 [optional] string $desp2
     * @param 附加三  如果=no 不显示文本框 [optional] string $desp3
     * @return string
     */
    public function getSubContent($url, $end_fix, $totalPages, $currentPage, $maxPage, $descriptions = '', $desp1 = '', $desp2 = '', $desp3 = FALSE)
    {
        $temp_str = "";
        $lastPage = $currentPage - 1;
        $nextPage = $currentPage + 1;
        $page_href = "<a href=\"" . $url;
        $descriptions = ' ' . $descriptions . ' ';
        $index_fix_str = $final_fix_str = $last_fix_str = $next_fix_str = '';
        if (!empty($desp1) || !empty($desp2)) {
            $index_fix_str = $desp1 . "1" . $desp2;
            $final_fix_str = $desp1 . $totalPages . $desp2;
            $last_fix_str = $desp1 . $lastPage . $desp2;
            $next_fix_str = $desp1 . $nextPage . $desp2;
        }
        $firstPage ='<li>'. $page_href . "1" . $end_fix . "\"" . $descriptions . $index_fix_str . " target='_self' rel='nofollow'>首页</a></li>";
        //$firstPage='<a href="/">首页</a>';
        $finalPage ='<li>'. $page_href . $totalPages . $end_fix . "\"" . $descriptions . $final_fix_str . " rel='nofollow'>尾页</a></li>";
        $allPage = "<li>&nbsp;共" . $total . "记录&nbsp;{$currentPage}/{$totalPages}</li>";
//        $allPage = '';
        if ($lastPage > 0) {
            $temp_str ='<li>'. $page_href . $lastPage . $end_fix . "\"" . $descriptions . $last_fix_str . " class=\"pre\" rel='nofollow'><</a></li>";
        }
        $temp_str .= $this->getHref($page_href, $end_fix, $totalPages, $currentPage, $maxPage, '', '', $descriptions, $desp1, $desp2);
        if ($nextPage <= $totalPages) {
            $temp_str .='<li>'. $page_href . $nextPage . $end_fix . "\"" . $descriptions . $next_fix_str . " class=\"pagenext\" rel='nofollow'>></a></li>";
        }
        $input_str = "&nbsp;到第<input type=\"text\" size=\"3\" onkeydown=\"javascript:if(event.keyCode==13) return false;\" style=\"text-align:center;margin:0px 3px;border:1px solid #999;\" id=\"goPageText\" value=\"" . $currentPage . "\">页&nbsp;<input id=\"goPageButton\" onclick=\"javascript:var goP = document.getElementById('goPageText'); if(isNaN(goP.value)||goP.value<1||goP.value>" . $totalPages . "){alert('请正确输入页数!');goP.select();return false;}  window.location.href='" . $url . "'+goP.value+'" . $end_fix . "';\" type=\"button\" value=\"确定\">";
        if ($desp3 == 'no') {
            $input_str = '';
        }
        return'<ul class="pagination">'. $firstPage . $temp_str . $finalPage . $allPage . $input_str.'</ul>';
    }

    private function getHref($page_href, $end_fix, $totalPages, $currentPage, $maxPage, $title = '', $target = '', $descriptions = '', $desp1, $desp2)
    {
        $href_str = "";
        $midPage = ceil($maxPage / 2);
        if ($totalPages <= $maxPage) { //如果总页数小于要显示的最多页数
            $href_str .= $this->getHrefStr($end_fix, 1, $totalPages, $page_href, $currentPage, $descriptions, $desp1, $desp2);
        } else {
            if ($currentPage > $midPage) {
                $toEndPage = $currentPage + $midPage;
                $toStartPage = $currentPage - $midPage;
                if ($toEndPage <= $totalPages) {
                    $href_str .= $this->getHrefStr($end_fix, $toStartPage, $toEndPage, $page_href, $currentPage, $descriptions, $desp1, $desp2);
                } else {
                    $href_str .= $this->getHrefStr($end_fix, $totalPages - $maxPage + 1, $totalPages, $page_href, $currentPage, $descriptions, $desp1, $desp2);
                }
            } else {
                $href_str .= $this->getHrefStr($end_fix, 1, $maxPage, $page_href, $currentPage, $descriptions, $desp1, $desp2);
            }
        }
        return $href_str;
    }

    private function getHrefStr($end_fix, $k, $totalPages, $page_href, $currentPage, $descriptions, $desp1, $desp2)
    {
        $href_str = '';
        for ($i = $k; $i <= $totalPages; $i++) {
            $page_fix_str = '';
            if ($i == $currentPage || ($i==1 && $i ==! $currentPage)) {
                $href_str .= '<li  class="active">'.$page_href . $i . $end_fix . "\" " . $descriptions . $page_fix_str . ">$i</a></li>";
                continue;
            }

            if (!empty($desp1) || !empty($desp2)) {
                $page_fix_str = $desp1 . $i . $desp2;
            }
            $href_str .= '<li>'.$page_href . $i . $end_fix . "\" " . $descriptions . $page_fix_str . ">$i</a></li>";
        }
        return $href_str;
    }
}