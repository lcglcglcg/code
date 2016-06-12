<?php

/**
 * Created by PhpStorm.
 * User: kele
 * Date: 13-11-18
 * Time: 上午12:59
 */
class misc {

    public static function show404() {
        header('HTTP/1.1 404 Not Found');
        header("status: 404 Not Found");
        exit;
    }

    static function tranTime($time) {
        $seconds = $_SERVER['time'] - $time;
        if ($seconds > 31536000) {
            return date('Y-n-j', $time);
        } elseif ($seconds > 2592000) {
            return floor($seconds / 2592000) . '月前';
        } elseif ($seconds > 86400) {
            return floor($seconds / 86400) . '天前';
        } elseif ($seconds > 3600) {
            return floor($seconds / 3600) . '小时前';
        } elseif ($seconds > 60) {
            return floor($seconds / 60) . '分钟前';
        } else {
            return '刚刚';
        }
    }

    // 返回格式：http://www.domain.com/blog/，这里不考虑 https://
    public static function get_url_path() {
        $port = core::gpc('SERVER_PORT', 'S');
        //$portadd = ($port == 80 ? '' : ':'.$port);
        $host = core::gpc('HTTP_HOST', 'S'); // host 里包含 port
        $path = substr(core::gpc('PHP_SELF', 'S'), 0, strrpos(core::gpc('PHP_SELF', 'S'), '/'));
        $http = (($port == 443) || (isset($_SERVER['HTTPS']) && strtolower($_SERVER['HTTPS']) != 'off')) ? 'https' : 'http';
        return "$http://$host$path/";
    }

    // 返回格式：http://www.domain.com/path/script.php?a=b&c=d
    public static function get_script_uri() {
        $port = core::gpc('SERVER_PORT', 'S');
        //$portadd = $port == 80 ? '' : ':80';
        $host = core::gpc('HTTP_HOST', 'S');
        //$schme = self::gpc('SERVER_PROTOCOL', 'S');
        // [SERVER_SOFTWARE] => Microsoft-IIS/6.0
        // [REQUEST_URI] => /index.php
        // [HTTP_X_REWRITE_URL] => /?a=b
        // iis
        if (isset($_SERVER['HTTP_X_REWRITE_URL'])) {
            $request_uri = $_SERVER['HTTP_X_REWRITE_URL'];
        } else {
            $request_uri = $_SERVER['REQUEST_URI'];
        }
        $http = (($port == 443) || (isset($_SERVER['HTTPS']) && strtolower($_SERVER['HTTPS']) != 'off')) ? 'https' : 'http';
        return "$http://$host" . $request_uri;
        //if(isset($_SERVER['SCRIPT_URI']) && 0) {
        //	return $_SERVER['SCRIPT_URI'];// 会漏掉 query_string, .core::gpc('QUERY_STRING', 'S');
        //}
    }

    // 转换空白字符, $onlytab 仅仅转换 \t
    static function html_space($s) {
        $s = str_replace('  ', ' &nbsp;', $s);
        $s = str_replace('  ', ' &nbsp;', $s);
        $s = str_replace('  ', ' &nbsp;', $s);
        $s = str_replace("\t", '&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; ', $s);
        $s = str_replace("\r\n", "\n", $s);
        $s = str_replace("\n", "<br />", $s);
        return $s;
    }

    static function encrypt($input) {
        $size = mcrypt_get_block_size(MCRYPT_3DES, 'ecb');
        $input = self::pkcs5_pad($input, $size);
        $key = str_pad(DESKEY, 24, '0');
        $td = mcrypt_module_open(MCRYPT_3DES, '', 'ecb', '');
        $iv = @mcrypt_create_iv(mcrypt_enc_get_iv_size($td), MCRYPT_RAND);
        @mcrypt_generic_init($td, $key, $iv);
        $data = mcrypt_generic($td, $input);
        mcrypt_generic_deinit($td);
        mcrypt_module_close($td);
        $data = base64_encode($data);
        return $data;
    }

    static function decrypt($encrypted) {
        $encrypted = base64_decode($encrypted);
        $key = str_pad(DESKEY, 24, '0');
        $td = mcrypt_module_open(MCRYPT_3DES, '', 'ecb', '');
        $iv = @mcrypt_create_iv(mcrypt_enc_get_iv_size($td), MCRYPT_RAND);
        $ks = mcrypt_enc_get_key_size($td);
        @mcrypt_generic_init($td, $key, $iv);
        $decrypted = mdecrypt_generic($td, $encrypted);
        mcrypt_generic_deinit($td);
        mcrypt_module_close($td);
        $y = self::pkcs5_unpad($decrypted);
        return $y;
    }

    static function pkcs5_pad($text, $blocksize) {
        $pad = $blocksize - (strlen($text) % $blocksize);
        return $text . str_repeat(chr($pad), $pad);
    }

    static function pkcs5_unpad($text) {
        $pad = ord($text{strlen($text) - 1});
        if ($pad > strlen($text)) {
            return false;
        }
        if (strspn($text, chr($pad), strlen($text) - $pad) != $pad) {
            return false;
        }
        return substr($text, 0, -1 * $pad);
    }

    static function PaddingPKCS7($data) {
        $block_size = mcrypt_get_block_size(MCRYPT_3DES, MCRYPT_MODE_CBC);
        $padding_char = $block_size - (strlen($data) % $block_size);
        $data .= str_repeat(chr($padding_char), $padding_char);
        return $data;
    }

    static function is_email($s) {
        return preg_match("/^[\w\-\.]+@[\w\-\.]+(\.\w+)+$/", $s);
    }

    static function is_url($s) {
        return preg_match('#^(https?://[^\'"\\\\<>:\s]+(:\d+)?)?([^\'"\\\\<>:\s]+?)*$#is', $s); //url已http://开头  i 忽略大小写
    }

    static function is_qq($s) {
        return preg_match('#^\d+$#', $s);
    }

    static function is_tel($s) {
        return preg_match('#^[\d\-]+$#', $s);
    }

    static function is_mobile($s) {
        return preg_match('#^\d{11}$#', $s);
    }

    static function is_version($s) {
        return preg_match('#^\d+(\.\d+)+$#', $s);
    }

    /**
     * 验证身份证号
     * @param $vStr
     * @return bool
     */
    static function isCreditNo($vStr) {
        $vCity = array(
            '11', '12', '13', '14', '15', '21', '22',
            '23', '31', '32', '33', '34', '35', '36',
            '37', '41', '42', '43', '44', '45', '46',
            '50', '51', '52', '53', '54', '61', '62',
            '63', '64', '65', '71', '81', '82', '91'
        );

        if (!preg_match('/^([\d]{17}[xX\d]|[\d]{15})$/', $vStr))
            return false;

        if (!in_array(substr($vStr, 0, 2), $vCity))
            return false;

        $vStr = preg_replace('/[xX]$/i', 'a', $vStr);
        $vLength = strlen($vStr);

        if ($vLength == 18) {
            $vBirthday = substr($vStr, 6, 4) . '-' . substr($vStr, 10, 2) . '-' . substr($vStr, 12, 2);
        } else {
            $vBirthday = '19' . substr($vStr, 6, 2) . '-' . substr($vStr, 8, 2) . '-' . substr($vStr, 10, 2);
        }

        if (date('Y-m-d', strtotime($vBirthday)) != $vBirthday)
            return false;
        if ($vLength == 18) {
            $vSum = 0;

            for ($i = 17; $i >= 0; $i--) {
                $vSubStr = substr($vStr, 17 - $i, 1);
                $vSum += (pow(2, $i) % 11) * (($vSubStr == 'a') ? 10 : intval($vSubStr, 11));
            }

            if ($vSum % 11 != 1)
                return false;
        }

        return true;
    }

    static function sendmail($username, $email, $subject, $message) {
        $mailconf['sendtype'] = 1;
        $mailconf['smtplist'][0] = [
            'email' => 'lianchuangcms@sina.com', 
            'host' => 'smtp.sina.com', 
            'port' => 25,
            'user' => 'lianchuangcms@sina.com', 
            'pass' => 'A111111'
        ];

        //$mailconf['smtplist'][0] = Yaf_Registry::get("config")->get('smtp')->toArray();
        //php自带函数发送
        if ($mailconf['sendtype'] == 0) {

            $subject = iconv('UTF-8', 'GBK', $subject);
            $message = iconv('UTF-8', 'GBK', $message);
            mail($email, $subject, $message, NULL, NULL);
            //smtp发送
        } elseif ($mailconf['sendtype'] == 1) {

            $smtp = $mailconf['smtplist'][array_rand($mailconf['smtplist'])];
            $message = str_replace("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />", "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=GBK\" />", $message);
            $subject = iconv('UTF-8', 'GBK', $subject);
            $message = iconv('UTF-8', 'GBK', $message);
            $username = iconv('UTF-8', 'GBK', $username);
            return self::send($smtp, $username, $email, $subject, $message);
        }
    }

    public static function send($smtp, $username, $email, $subject, $message, $charset = 'GBK') {
        // 部分 SMTP 不支持UTF-8
        /*
          if(in_array($smtp, array('smtp.126.com', 'smtp.163.com'))) {
          $charset = 'GBK';
          } else {
          $charset = 'UTF-8';
          }
          $charset = 'GBK';
         */
        $mail = new PHPMailer();
        //$mail->PluginDir = FRAMEWORK_PATH.'lib/';
        $mail->IsSMTP(); // telling the class to use SMTP
        $mail->IsHTML(TRUE); //$mail->ContentType= 'text/html';
        $mail->SMTPDebug = 0; // enables SMTP debug information (for testing)
        // 1 = errors and messages
        // 2 = messages only
        $mail->SMTPAuth = TRUE; // enable SMTP authentication
        $mail->Host = $smtp['host']; // sets the SMTP server
        $mail->Port = $smtp['port']; // set the SMTP port for the GMAIL server
        $mail->Username = $smtp['user']; // SMTP account username
        $mail->Password = $smtp['pass']; // SMTP account password
        $mail->Timeout = 5; //
        $mail->CharSet = $charset;

        $mail->Encoding = 'base64';

        //$subject = $charset == 'UTF-8' ? iconv('UTF-8', 'GBK', $subject) : $subject;
        //$message = $charset == 'UTF-8' ? iconv('UTF-8', 'GBK', $message) : $message;
        //$username = $charset == 'UTF-8' ? iconv('UTF-8', 'GBK', $username) : $username;
        //$fromemail = $this->conf['reg_email_user'].'@'.$this->conf['reg_email_host'];

        $mail->SetFrom($smtp['email'], $username);
        $mail->AddReplyTo($smtp['email'], $email);
        $mail->Subject = $subject;
        $mail->AltBody = $message; // optional, comment out and test
        $message = str_replace("\\", '', $message);
        $mail->MsgHTML($message);

        $mail->AddAddress($email, $username);

        //$mail->AddAttachment("images/phpmailer.gif");      // attachment
        //$mail->AddAttachment("images/phpmailer_mini.gif"); // attachment

        if (!$mail->Send()) {
            return "Mailer Error: " . $mail->ErrorInfo;
        } else {
            return '';
        }
    }

    static function get_email_site($str) {
        $email = array('url' => '', 'name' => '');
        switch ($str) {
            case '163.com':
                $email['url'] = 'http://mail.163.com/';
                $email['name'] = '163';
                break;
            case '126.com':
                $email['url'] = 'http://mail.126.com/';
                $email['name'] = '126';
                break;
            case 'yeah.net':
                $email['url'] = 'http://mail.163.com/';
                $email['name'] = '163';
                break;
            case 'qq.com':
                $email['url'] = 'http://mail.qq.com/';
                $email['name'] = 'QQ';
                break;
            case 'yahoo.cn':
                $email['url'] = 'http://mail.cn.yahoo.com/';
                $email['name'] = 'Yahoo';
                break;
            case 'yahoo.com.cn':
                $email['url'] = 'http://mail.cn.yahoo.com/';
                $email['name'] = 'Yahoo';
                break;
            case 'sina.com':
                $email['url'] = 'http://mail.sina.com.cn/';
                $email['name'] = 'sina';
                break;
            case 'sina.cn':
                $email['url'] = 'http://mail.sina.com.cn/';
                $email['name'] = 'sina';
                break;
            case 'hotmail.com':
                $email['url'] = 'http://www.hotmail.com/';
                $email['name'] = 'Hotmail';
                break;
            case 'live.cn':
                $email['url'] = 'http://www.hotmail.com/';
                $email['name'] = 'Hotmail';
                break;
            case 'live.com':
                $email['url'] = 'http://www.hotmail.com/';
                $email['name'] = 'Hotmail';
                break;
            case 'gmail.com':
                $email['url'] = 'https://accounts.google.com/ServiceLogin?service=mail';
                $email['name'] = 'Gmail';
                break;
            case 'sohu.com':
                $email['url'] = 'http://mail.sohu.com/';
                $email['name'] = 'sohu';
                break;
            case '21cn.com':
                $email['url'] = 'http://mail.21cn.com/';
                $email['name'] = '21cn';
                break;
            case 'eyou.com':
                $email['url'] = 'http://www.eyou.com/';
                $email['name'] = 'eyou';
                break;
            case '188.com':
                $email['url'] = 'http://www.188.com/';
                $email['name'] = '188';
                break;
            case '263.net':
                $email['url'] = 'http://www.263.net/';
                $email['name'] = '263';
                break;
            case '139.com':
                $email['url'] = 'http://mail.10086.cn/';
                $email['name'] = '139';
                break;
            case 'tom.com':
                $email['url'] = 'http://mail.tom.com/';
                $email['name'] = 'Tom';
                break;
            case 'sogou.com':
                $email['url'] = 'http://mail.sogou.com/';
                $email['name'] = 'sogou';
                break;
            case 'foxmail.com':
                $email['url'] = 'http://www.foxmail.com/';
                $email['name'] = 'foxmail';
                break;
            case 'wo.com.cn':
                $email['url'] = 'http://mail.wo.com.cn/';
                $email['name'] = 'mail.wo.com.cn';
                break;
            default:
                $email['url'] = "http://www." . $str;
                $email['name'] = $str;
                break;
        }
        return $email;
    }

    static function form_hash($auth_key) {
        return substr(md5(substr($_SERVER['time'], 0, -5) . $auth_key), 16);
    }

    // 校验 formhash
    static function form_submit($auth_key) {
        return isset($_POST['FORM_HASH']) && $_POST['FORM_HASH'] == self::form_hash($auth_key);
    }

    // 过滤图片中的 <script
    static function is_safe_image($filename) {
        $s = file_get_contents($filename);
        if (strpos($s, '<script') !== FALSE) {
            unset($s);
            return FALSE;
        }
        unset($s);
        return TRUE;
    }

    /**
     * +----------------------------------------------------------
     * 将一个字符串部分字符用*替代隐藏
     * +----------------------------------------------------------
     * @param string $string 待转换的字符串
     * @param int $bengin 起始位置，从0开始计数，当$type=4时，表示左侧保留长度
     * @param int $len 需要转换成*的字符个数，当$type=4时，表示右侧保留长度
     * @param int $type 转换类型：0，从左向右隐藏；1，从右向左隐藏；2，从指定字符位置分割前由右向左隐藏；3，从指定字符位置分割后由左向右隐藏；4，保留首末指定字符串
     * @param string $glue 分割符
     * +----------------------------------------------------------
     * @return string   处理后的字符串
     * +----------------------------------------------------------
     */
    static function hideStr($string, $bengin = 0, $len = 4, $type = 0, $glue = "@") {
        if (empty($string))
            return false;
        $array = array();
        if ($type == 0 || $type == 1 || $type == 4) {
            $strlen = $length = mb_strlen($string);
            while ($strlen) {
                $array[] = mb_substr($string, 0, 1, "utf8");
                $string = mb_substr($string, 1, $strlen, "utf8");
                $strlen = mb_strlen($string);
            }
        }
        if ($type == 0) {
            for ($i = $bengin; $i < ($bengin + $len); $i++) {
                if (isset($array[$i]))
                    $array[$i] = "*";
            }
            $string = implode("", $array);
        } else if ($type == 1) {
            $array = array_reverse($array);
            for ($i = $bengin; $i < ($bengin + $len); $i++) {
                if (isset($array[$i]))
                    $array[$i] = "*";
            }
            $string = implode("", array_reverse($array));
        } else if ($type == 2) {
            $array = explode($glue, $string);
            $array[0] = hideStr($array[0], $bengin, $len, 1);
            $string = implode($glue, $array);
        } else if ($type == 3) {
            $array = explode($glue, $string);
            $array[1] = hideStr($array[1], $bengin, $len, 0);
            $string = implode($glue, $array);
        } else if ($type == 4) {
            $left = $bengin;
            $right = $len;
            $tem = array();
            for ($i = 0; $i < ($length - $right); $i++) {
                if (isset($array[$i]))
                    $tem[] = $i >= $left ? "*" : $array[$i];
            }
            $array = array_chunk(array_reverse($array), $right);
            $array = array_reverse($array[0]);
            for ($i = 0; $i < $right; $i++) {
                $tem[] = $array[$i];
            }
            $string = implode("", $tem);
        }
        return $string;
    }

    public static function get($url = '', $fields = []) {
        $fields = http_build_query($fields);
        $header = [
            'Accept:text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8',
            'Accept-Encoding:gzip,deflate,sdch',
            'Accept-Language:zh-CN,zh;q=0.8',
            'Connection:keep-alive',
            'Cache-Control:max-age=0',
            'Host:baby.guahao.com',
            'Content-Length:' . strlen($fields), //设置发送的内容长度
            'Origin:http://baby.guahao.com',
            'Referer:http://baby.guahao.com',
            'User-Agent:Mozilla/5.0 (Macintosh; Intel Mac OS X 10_9_0) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/31.0.1650.57 Safari/537.36'
        ];
        $ch = curl_init();
        curl_setopt($ch, CURLOPT_URL, $url);
        curl_setopt($ch, CURLOPT_TIMEOUT, 10); //5秒超时时间
        //curl_setopt($ch, CURLOPT_HTTPHEADER, $header);
        curl_setopt($ch, CURLOPT_POST, true);
        curl_setopt($ch, CURLOPT_POSTFIELDS, $fields);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1); //返回内容不是输出
        $result = curl_exec($ch);
        curl_close($ch);
        return $result;
    }

    public static function is_writable($file) {
        // 主要是兼容 windows
        try {
            if (is_file($file)) {
                if (strpos(strtoupper(PHP_OS), 'WIN') !== FALSE) {
                    $fp = @fopen($file, 'rb+');
                    @fclose($fp);
                    return (bool) $fp;
                } else {
                    return is_writable($file);
                }
            } elseif (is_dir($file)) {
                $tmpfile = $file . '/____tmp.tmp';
                $n = @file_put_contents($tmpfile, 'a');
                if ($n > 0) {
                    unlink($tmpfile);
                    return TRUE;
                } else {
                    return FALSE;
                }
            }
        } catch (Exception $e) {
            return false;
        }
    }

    // https request
    public static function https_fetch_url($url, $timeout = 30, $post = '', $cookie = '', $deep = 0) {
        if (substr($url, 0, 5) == 'http:') {
            return self::fetch_url($url, $timeout, $post, $cookie, $deep);
        }
        $w = stream_get_wrappers();
        $allow_url_fopen = strtolower(ini_get('allow_url_fopen'));
        $allow_url_fopen = (empty($allow_url_fopen) || $allow_url_fopen == 'off') ? 0 : 1;
        if (extension_loaded('openssl') && in_array('https', $w) && $allow_url_fopen) {
            return file_get_contents($url);
        } elseif (!function_exists('curl_init')) {
            throw new Exception('server not installed curl.');
        }
        $ch = curl_init();
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
        curl_setopt($ch, CURLOPT_HEADER, 1);
        curl_setopt($ch, CURLOPT_URL, $url);
        curl_setopt($ch, CURLOPT_USERAGENT, core::gpc('HTTP_USER_AGENT', 'S'));
        curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0); // 对认证证书来源的检查
        curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 1); // 从证书中检查SSL加密算法是否存在
        if ($post) {
            curl_setopt($ch, CURLOPT_POST, 1);
            curl_setopt($ch, CURLOPT_POSTFIELDS, $post);
        }
        if ($cookie) {
            curl_setopt($ch, CURLOPT_HTTPHEADER, array("Cookie: $cookie"));
        }
        (!ini_get('safe_mode') && !ini_get('open_basedir')) && curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 1); // 使用自动跳转, 安全模式不允许
        curl_setopt($ch, CURLOPT_TIMEOUT, $timeout);
        $data = curl_exec($ch);
        if (curl_errno($ch)) {
            throw new Exception('Errno' . curl_error($ch)); //捕抓异常
        }
        if (!$data) {
            curl_close($ch);
            return '';
        }

        list($header, $data) = explode("\r\n\r\n", $data);
        $http_code = curl_getinfo($ch, CURLINFO_HTTP_CODE);
        if ($http_code == 301 || $http_code == 302) {
            $matches = array();
            preg_match('/Location:(.*?)\n/', $header, $matches);
            $url = trim(array_pop($matches));
            curl_setopt($ch, CURLOPT_URL, $url);
            curl_setopt($ch, CURLOPT_HEADER, false);
            $data = curl_exec($ch);
        }
        curl_close($ch);
        return $data;
    }

    // SAE 重载了 file_get_contents()
    public static function fetch_url($url, $timeout = 10, $post = '', $cookie = '', $deep = 0) {
        if ($deep > 5)
            throw new Exception('超出 fetch_url() 最大递归深度！');
        if (substr($url, 0, 5) == 'https') {
            return self::https_fetch_url($url, $timeout, $post, $cookie, $deep);
        }

        $w = stream_get_wrappers();
        $allow_url_fopen = strtolower(ini_get('allow_url_fopen'));
        $allow_url_fopen = (empty($allow_url_fopen) || $allow_url_fopen == 'off') ? 0 : 1;
        if (function_exists('fsockopen')) {
            $limit = 2000000;
            $ip = '';
            $return = '';
            $matches = parse_url($url);
            $host = $matches['host'];
            $path = $matches['path'] ? $matches['path'] . (!empty($matches['query']) ? '?' . $matches['query'] : '') : '/';
            $port = !empty($matches['port']) ? $matches['port'] : 80;

            $HTTP_USER_AGENT = core::gpc('$HTTP_USER_AGENT', 'S');
            empty($HTTP_USER_AGENT) && $HTTP_USER_AGENT = 'Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0)';
            if (empty($post)) {
                $out = "GET $path HTTP/1.0\r\n";
                $out .= "Accept: */*\r\n";
                $out .= "Accept-Language: zh-cn\r\n";
                $out .= "User-Agent: $HTTP_USER_AGENT\r\n";
                $out .= "Host: $host\r\n";
                $out .= "Connection: Close\r\n";
                $out .= "Cookie:$cookie\r\n\r\n";
            } else {
                $out = "POST $path HTTP/1.0\r\n";
                $out .= "Accept: */*\r\n";
                $out .= "Accept-Language: zh-cn\r\n";
                $out .= "User-Agent: $HTTP_USER_AGENT\r\n";
                $out .= "Host: $host\r\n";
                $out .= 'Content-Length: ' . strlen($post) . "\r\n";
                $out .= "Content-Type: application/x-www-form-urlencoded\r\n";
                $out .= "Connection: Close\r\n";
                $out .= "Cache-Control: no-cache\r\n";
                $out .= "Cookie:$cookie\r\n\r\n";
                $out .= $post;
            }
            $host == 'localhost' && $ip = '127.0.0.1';
            $fp = @fsockopen(($ip ? $ip : $host), $port, $errno, $errstr, $timeout);
            if (!$fp) {
                return FALSE;
            } else {
                stream_set_blocking($fp, TRUE);
                stream_set_timeout($fp, $timeout);
                @fwrite($fp, $out);
                $status = stream_get_meta_data($fp);
                if (!$status['timed_out']) {
                    $starttime = time();
                    while (!feof($fp)) {
                        if (($header = @fgets($fp)) && ($header == "\r\n" || $header == "\n")) {
                            break;
                            //Location: http://plugin.xiuno.net/upload/plugin/66/b0c35647c63b8b880766b50c06586c13.zip
                        } else {
                            if (strtolower(substr($header, 0, 9)) == 'location:') {
                                $location = trim(substr($header, 9));
                                return self::fetch_url($location, $timeout, $post, $cookie, $deep + 1);
                            }
                        }
                    }

                    $stop = false;
                    while (!feof($fp) && !$stop) {
                        $data = fread($fp, ($limit == 0 || $limit > 8192 ? 8192 : $limit));
                        $return .= $data;
                        if ($limit) {
                            $limit -= strlen($data);
                            $stop = $limit <= 0;
                        }
                        if (time() - $starttime > $timeout)
                            break;
                    }
                }
                @fclose($fp);
                return $return;
            }
        } elseif (function_exists('curl_init')) {
            $ch = curl_init();
            curl_setopt($ch, CURLOPT_URL, $url);
            curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
            curl_setopt($ch, CURLOPT_HEADER, 1);
            if ($post) {
                curl_setopt($ch, CURLOPT_POST, 1);
                curl_setopt($ch, CURLOPT_POSTFIELDS, $post);
            }
            if ($cookie) {
                curl_setopt($ch, CURLOPT_HTTPHEADER, array('Cookie', $cookie));
            }
            $data = curl_exec($ch);

            if (curl_errno($ch)) {
                throw new Exception('Errno' . curl_error($ch)); //捕抓异常
            }
            if (!$data) {
                curl_close($ch);
                return '';
            }

            list($header, $data) = explode("\r\n\r\n", $data);
            $http_code = curl_getinfo($ch, CURLINFO_HTTP_CODE);
            if ($http_code == 301 || $http_code == 302) {
                $matches = array();
                preg_match('/Location:(.*?)\n/', $header, $matches);
                $url = trim(array_pop($matches));
                curl_setopt($ch, CURLOPT_URL, $url);
                curl_setopt($ch, CURLOPT_HEADER, false);
                $data = curl_exec($ch);
            }
            curl_close($ch);
            return $data;
        } elseif ($allow_url_fopen && empty($post) && empty($cookie) && in_array('http', $w)) {
            // 尝试连接
            $opts = array('http' => array('method' => 'GET', 'timeout' => $timeout));
            $context = stream_context_create($opts);
            $html = file_get_contents($url, false, $context);
            return $html;
        } else {
            //log::write('fetch_url() failed: ' . $url);
            return FALSE;
        }
    }

    // 多线程抓取数据，需要CURL支持，一般在命令行下执行，此函数收集互联网，由 xiuno 整理。
    public static function multi_fetch_url($urls) {
        if (!function_exists('curl_multi_init')) {
            $data = array();
            foreach ($urls as $k => $url) {
                $data[$k] = self::fetch_url($url);
            }
            return $data;
        }

        $multi_handle = curl_multi_init();
        foreach ($urls as $i => $url) {
            $conn[$i] = curl_init($url);
            curl_setopt($conn[$i], CURLOPT_RETURNTRANSFER, 1);
            $timeout = 3;
            curl_setopt($conn[$i], CURLOPT_CONNECTTIMEOUT, $timeout); // 超时 seconds
            curl_setopt($conn[$i], CURLOPT_FOLLOWLOCATION, 1);
            //curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
            curl_multi_add_handle($multi_handle, $conn[$i]);
        }
        do {
            $mrc = curl_multi_exec($multi_handle, $active);
        } while ($mrc == CURLM_CALL_MULTI_PERFORM);
        while ($active and $mrc == CURLM_OK) {
            if (curl_multi_select($multi_handle) != -1) {
                do {
                    $mrc = curl_multi_exec($multi_handle, $active);
                } while ($mrc == CURLM_CALL_MULTI_PERFORM);
            }
        }
        foreach ($urls as $i => $url) {
            $data[$i] = curl_multi_getcontent($conn[$i]);
            curl_multi_remove_handle($multi_handle, $conn[$i]);
            curl_close($conn[$i]);
        }
        return $data;
    }

    // 替代 scandir, safe_mode
    public static function scandir($dir) {
        if (function_exists('scan_dir'))
            return scandir($dir);
        $df = opendir($dir);
        $arr = array();
        while ($file = readdir($df)) {
            if ($file == '.' || $file == '..')
                continue;
            $arr[] = $file;
        }
        closedir($df);
        return $arr;
    }

    // 递归删除目录，这个函数比较危险，传参一定要小心
    public static function rmdir($dir, $keepdir = 0) {
        if ($dir == '/' || $dir == '../')
            return FALSE; // 不允许删除根目录，避免程序意外删除数据。
        if (!is_dir($dir))
            return FALSE;
        substr($dir, -1, 1) != '/' && $dir .= '/';
        $files = self::scandir($dir);
        foreach ($files as $file) {
            if ($file == '.' || $file == '..')
                continue;
            $filepath = $dir . $file;
            if (!is_dir($filepath)) {
                try {
                    unlink($filepath);
                } catch (Exception $e) {
                    
                }
            } else {
                self::rmdir($filepath . '/');
            }
        }
        try {
            if (!$keepdir)
                rmdir($dir);
        } catch (Exception $e) {
            
        }
        return TRUE;
    }

    // 对 key-value 数组进行组合 ('a'=>'a1', 'b'=>'b1')
    /*
      implode('=', '&', ('a'=>'a1', 'b'=>'b1'))
      a=a1&b=b1
     */
    public static function implode($glue1, $glue2, $arr) {
        $s = '';
        foreach ($arr as $k => $v) {
            $s .= ($s ? $glue2 : '') . $k . ($v ? $glue1 . $v : '');
        }
        return $s;
    }

    // 对 key-value 数组进行组合
    public static function explode($sep1, $sep2, $s) {
        $arr = $arr2 = $arr3 = array();
        $arr = explode($sep2, $s);
        foreach ($arr as $v) {
            $arr2 = explode($sep1, $v);
            $arr3[$arr2[0]] = (isset($arr2[1]) ? $arr2[1] : '');
        }
        return $arr3;
    }

    public static function is_robot() {
        $robots = array('robot', 'spider', 'slurp');
        foreach ($robots as $robot) {
            if (strpos(core::gpc('HTTP_USER_AGENT', 'S'), $robot) !== FALSE) {
                return TRUE;
            }
        }
        return FALSE;
    }

    /**
     * 截取字符串
     * @param string $str
     * @paran int $len
     * @return string
     */
    public static function getSubStr($str = '', $len = 3, $sign = '...') {
        if (empty($str) || $len <= 0) {
            return FALSE;
        } else {
            $str = (mb_strlen($str, 'utf-8') <= $len) ? $str : mb_substr($str, 0, $len, 'utf-8') . $sign;
        }
        return $str;
    }

    /**
     * 二维索引数组去重
     * @param array
     */
    public static function array_unique_td($arr) {
        for ($i = 0; $i <= count($arr); $i++) {
            if (($i + 1) == count($arr)) {
                break;
            } elseif ($arr[$i] == $arr[$i + 1]) {
                unset($arr[$i]);
            }
        }
        return $arr;
    }

    /*
     * 高亮
     */

    public static function hightLight($s, $str) {
        $message = str_replace($s, '<span style="color:red">' . $s . '</span>', $str);
        return $message;
    }

    /*
     * 去空格
     */

    public static function remove_nbsp($str) {
        $str = mb_ereg_replace('^(　| )+', '', $str);
        $str = mb_ereg_replace('(　| )+$', '', $str);
        $str = mb_ereg_replace('　　', "\n　　", $str);
        return $str;
    }

    static function removeslashes($string) {
        $string = implode("", explode("\\", $string));
        return stripslashes(trim($string));
    }

    /*
     * 去\r\n
     */

    static function replacebr($str) {
        //echo $str;
        $order = array('\r\n', '\n', '\r');
        //$order = array("\r\n","\n","\n");
        $replace = '<br/>';

        $newstr = str_replace($order, $replace, $str);
        return $newstr;
    }

    //判断是否是手机访问
    static function checkmobile() {
        $touchbrowser_list = array('iphone', 'android', 'phone', 'mobile', 'wap', 'netfront', 'java', 'opera mobi', 'opera mini',
            'ucweb', 'windows ce', 'symbian', 'series', 'webos', 'sony', 'blackberry', 'dopod', 'nokia', 'samsung',
            'palmsource', 'xda', 'pieplus', 'meizu', 'midp', 'cldc', 'motorola', 'foma', 'docomo', 'up.browser',
            'up.link', 'blazer', 'helio', 'hosin', 'huawei', 'novarra', 'coolpad', 'webos', 'techfaith', 'palmsource',
            'alcatel', 'amoi', 'ktouch', 'nexian', 'ericsson', 'philips', 'sagem', 'wellcom', 'bunjalloo', 'maui', 'smartphone',
            'iemobile', 'spice', 'bird', 'zte-', 'longcos', 'pantech', 'gionee', 'portalmmm', 'jig browser', 'hiptop',
            'benq', 'haier', '^lct', '320x320', '240x320', '176x220');
        $mobilebrowser_list = array('windows phone');
        $wmlbrowser_list = array('cect', 'compal', 'ctl', 'lg', 'nec', 'tcl', 'alcatel', 'ericsson', 'bird', 'daxian', 'dbtel', 'eastcom',
            'pantech', 'dopod', 'philips', 'haier', 'konka', 'kejian', 'lenovo', 'benq', 'mot', 'soutec', 'nokia', 'sagem', 'sgh',
            'sed', 'capitel', 'panasonic', 'sonyericsson', 'sharp', 'amoi', 'panda', 'zte');

        $pad_list = array('pad', 'gt-p1000');

        $useragent = strtolower($_SERVER['HTTP_USER_AGENT']);

        if (misc::dstrpos($useragent, $pad_list)) {
            return false;
        }
        if (($v = misc::dstrpos($useragent, $mobilebrowser_list, true))) {
            return '1';
        }
        if (($v = misc::dstrpos($useragent, $touchbrowser_list, true))) {
            return '2';
        }
        if (($v = misc::dstrpos($useragent, $wmlbrowser_list))) {
            return '3'; //wml版
        }
        $brower = array('mozilla', 'chrome', 'safari', 'opera', 'm3gate', 'winwap', 'openwave', 'myop');
        if (misc::dstrpos($useragent, $brower))
            return false;


        return false;
    }

    static function dstrpos($string, $arr, $returnvalue = false) {
        if (empty($string))
            return false;
        foreach ((array) $arr as $v) {
            if (strpos($string, $v) !== false) {
                $return = $returnvalue ? $v : true;
                return $return;
            }
        }
        return false;
    }

    /**
     * 验证日期2014-2-21是否正确
     * @param type $string
     * @param type $dilimiter 默认 - 分割
     * @return  boolean
     */
    public static function is_date($string, $dilimiter = '-') {
        $arr = explode($dilimiter, $string);
        return count($arr) === 3 ? checkdate($arr[1], $arr[2], $arr[0]) : false;
    }
    
    //获取汉字拼音首字母
    private static function getfirstchar($s0){
        $fchar = ord($s0{0});
        if($fchar >= ord("A") and $fchar <= ord("z") ){
            return strtoupper($s0{0});
        }

        $s1 = mb_convert_encoding($s0,"gb2312","UTF-8");
        $s2 = mb_convert_encoding($s1,"gb2312","gb2312");

        if($s2 == $s0){
            $s = $s1;
        }else{
            $s = $s0;
        }

        $asc = ord($s{0}) * 256 + ord($s{1}) - 65536;
        if($asc >= -20319 and $asc <= -20284) return "A";
        if($asc >= -20283 and $asc <= -19776) return "B";
        if($asc >= -19775 and $asc <= -19219) return "C";
        if($asc >= -19218 and $asc <= -18711) return "D";
        if($asc >= -18710 and $asc <= -18527) return "E";
        if($asc >= -18526 and $asc <= -18240) return "F";
        if($asc >= -18239 and $asc <= -17923) return "G";
        if($asc >= -17922 and $asc <= -17418) return "I";
        if($asc >= -17417 and $asc <= -16475) return "J";
        if($asc >= -16474 and $asc <= -16213) return "K";
        if($asc >= -16212 and $asc <= -15641) return "L";
        if($asc >= -15640 and $asc <= -15166) return "M";
        if($asc >= -15165 and $asc <= -14923) return "N";
        if($asc >= -14922 and $asc <= -14915) return "O";
        if($asc >= -14914 and $asc <= -14631) return "P";
        if($asc >= -14630 and $asc <= -14150) return "Q";
        if($asc >= -14149 and $asc <= -14091) return "R";
        if($asc >= -14090 and $asc <= -13319) return "S";
        if($asc >= -13318 and $asc <= -12839) return "T";
        if($asc >= -12838 and $asc <= -12557) return "W";
        if($asc >= -12556 and $asc <= -11848) return "X";
        if($asc >= -11847 and $asc <= -11056) return "Y";
        if($asc >= -11055 and $asc <= -10247) return "Z";

        return null;
    }

    /**
     * 去除掉字符串中的中英文标点符号
     * @param string $text  要处理额字符串
     * @return string 处理后的字符串
     */
    public static function filter_mark($text){
        if(trim($text)=='') {
            return '';
        }
        $text=preg_replace("/[[:punct:]\s]/",'',$text);
        $text=urlencode($text);
        $text=preg_replace("/(%7E|%60|%21|%40|%23|%24|%25|%5E|%26|%27|%2A|%28|%29|%2B|%7C|%5C|%3D|\-|_|%5B|%5D|%7D|%7B|%3B|%22|%3A|%3F|%3E|%3C|%2C|\.|%2F|%A3%BF|%A1%B7|%A1%B6|%A1%A2|%A1%A3|%A3%AC|%7D|%A1%B0|%A3%BA|%A3%BB|%A1%AE|%A1%AF|%A1%B1|%A3%FC|%A3%BD|%A1%AA|%A3%A9|%A3%A8|%A1%AD|%A3%A4|%A1%A4|%A3%A1|%E3%80%82|%EF%BC%81|%EF%BC%8C|%EF%BC%9B|%EF%BC%9F|%EF%BC%9A|%E3%80%81|%E2%80%A6%E2%80%A6|%E2%80%9D|%E2%80%9C|%E2%80%98|%E2%80%99|%EF%BD%9E|%EF%BC%8E|%EF%BC%88)+/",'',$text);

        $text=urldecode($text);

        return trim($text);
    }

    /**
     * 获取中文字符中的每个中文拼音的首个字母
     * @param string $zh 中文字符串
     * @return string 由每个中文拼音的首个字母构成的字符串
     */
    public static function get_first_pinyin($zh,$upper = false){
        $ret = "";
        $s1 = mb_convert_encoding($zh,"gb2312","UTF-8");
        $s2 = mb_convert_encoding($s1,"UTF-8","gb2312");

        if($s2 == $zh){
            $zh = $s1;
        }

        for($i = 0; $i < strlen($zh); $i++){
            $s1 = substr($zh,$i,1);
            $p = ord($s1);
            if($p > 160){
                $s2 = substr($zh,$i++,2);
                $ret .= self::getfirstchar($s2);
            }else{
                $ret .= $s1;
            }
        }

        if($upper){
            return self::filter_mark($ret);
        }else{
            return  strtolower(self::filter_mark($ret));
        }

    }
    
    /**
     * 分词，返回关键字
     * @param type $content
     * @return type
     * @author yangguofeng
     */
    public static function getKeywords($content) {
        $words = [];
        $scws = scws_new();
        $scws->set_charset('utf8');
        // 这里没有调用 set_dict 和 set_rule 系统会自动试调用 ini 中指定路径下的词典和规则文件
        $scws->send_text($content);
        $scws->add_dict(ini_get("scws.default.fpath") . "/lianchuang.txt", SCWS_XDICT_TXT);
        $scws->set_ignore(TRUE);
        //是否复式分割，如“中国人”返回“中国＋人＋中国人”三个词。
        $scws->set_multi(false);
        //设定将文字自动以二字分词法聚合
        // $scws->set_duality(true);

        $result = $scws->get_words('@');
        if (is_array($result) && count($result) > 0) {
            foreach ($result as $key => $value) {
                $words[] = $value['word'];
            }
        }
        $scws->close();
        return $words;
    }

    /**
     * keywords输入中文
     * @param type $keywords
     * @return 
     * keywords：中国 return:zhongguo
     * keywords：中国 中国 return:zhongguo zhongguo
     */
    public static function getEnKeywords($keywords) {
        $enKeywords = '';
        if (strpos($keywords, ' ') && $keywords = explode(' ', $keywords)) {
            foreach ($keywords as $value) {
                if ($value) {
                    $enKeywords .= Pinyin::get($value) . ' ';
                }
            }
            $enKeywords = rtrim($enKeywords, ' ');
        } else {
            $enKeywords = Pinyin::get($keywords);
        }
        return $enKeywords;
    }

    /**
     * 专题url分析
     * @param string $url url连接
     * @return maxed
     */
    public static function URLAnalysis($url) {
        $url = trim(parse_url($url, PHP_URL_PATH), "/");
        $urlArr = explode("/", $url);
        if(count($urlArr) == 2) {
            return array('topic'=>$urlArr[1]);
        }else if(count($urlArr) == 3) {
            return array('topic'=>$urlArr[1], 'subtopic'=>$urlArr[2]);
        }else if(count($urlArr) == 4) {
            if(preg_match('#(\w+)/(\w+)/default_(\d+)\.html$#', $url, $match))
                return array('topic'=>$match[1], 'subtopic'=>$match[2], 'page'=>$match[3]);
            else if(preg_match('#(\w+)/(\w+)/(\d+)\.html$#', $url, $match))
                return array('topic'=>$match[1], 'subtopic'=>$match[2], 'id'=>$match[3]);
        }
        
        return false;
    }

    /**
     * 字符串格式化
     * @param string $string 要处理的字符串
     * @param int   $start 字符串开始截取的位置
     * @param int   $end 字符截取结束位置
     * @return string 
     */
    public static function stringFormat($string,$start = 0,$end = 10){
        $string = strip_tags($string);
        $string = misc::remove_nbsp($string);
        return mb_substr($string,$start,$end);
    }

    /**
     * 组合文章返回链接
     * @param $catpath  string  文章的分类path
     * @param $time     maxed   
     * @param $id       文章id
     * @return string   组合好的文章链接
     */
    public static function get_return_news_url($catpath,$time,$id){
        //判断返回的url类型:1=>/hyzx/catpath/id.html 2:/hyzx/catpath/date/id.html
        $url = BASEURL . SITEDIR . '/' . $catpath . '/' ;
        
        if(URL_TYPE == 1){
            $url .= $id . '.html';
        }elseif(URL_TYPE == 2){

            if(TIME_TYPE == 1){
                $url .= date('Ymd',$time) . '/' . $id . '.html';
            }else{
                $url .= date('Ymd',strtotime($time)) . '/' . $id . '.html';
            }
            
        }

        return $url;
    }
}
