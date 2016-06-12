<?php if (!defined('APP_PATH')) exit('No direct script access allowed');

/**
 * Created by PhpStorm.
 * User: kele
 * Date: 14-3-14
 * Time: 上午11:44
 */
class UserModel extends BaseModel
{
    function __construct()
    {
        parent::__construct();
        $this->table['qq_login'] = 'user_qqlogin';
        $this->table['weibo_login'] = 'user_weibologin';
        $this->table['user_profile'] = 'user_profile';
        $this->table['user_profile2'] = 'user_profile2';
        $this->table['user_family'] = 'user_family';
        $this->table['user_friends'] = 'user_friends';
        $this->table['feedback'] = 'feedback';
        //用户经纬度表
        $this->table['user_lng_lat'] = 'user_lng_lat';
        //用户签到日志
        $this->table['user_sign_log'] = 'user_sign_log';
        //用户铃声
        $this->table['user_bell'] = 'user_bell';

    }

    //检测邮箱格式
    function check_email(&$email)
    {
        $emaildefault = array('admin', 'system');
        if (empty($email)) {
            return 'EMAIL 不能为空';
        } elseif (utf8::strlen($email) > 60) {
            return 'Email 长度不能大于 60 个字符。';
        } elseif (!preg_match("/^[\w\-\.]+@[\w\-\.]+(\.\w+)+$/", $email)) {
            return 'Email 格式不对';
        } elseif (utf8::strlen($email) < 6) {
            return 'Email 太短';
        } elseif (str_replace($emaildefault, '', $email) != $email) {
            return 'Email 含有非法关键词';
        }

        return false;
    }

    //检测邮箱是否被注册
    function check_email_exists($email)
    {
        $uid = $this->db->get($this->table['user'], 'uid', ['email' => $email]);
        if (!empty($uid))
            return $uid;

        return false;
    }

    //检测用户名是否注册
    function check_username_exists($username)
    {
        $uid = $this->db->get($this->table['user'], 'uid', ['username' => $username]);
        if (!empty($uid))
            return $uid;

        return false;
    }

    function qqlogin_get_user_by_openid($openid)
    {
        $uid = $this->db->get($this->table['qq_login'], 'uid', ['openid' => $openid]);
        if (!empty($uid))
            return $uid;

        return false;
    }

    function qqlogin_register($uid, $openid)
    {
        if ($uid <> false && $openid <> false)
            $this->db->insert($this->table['qq_login'], ['uid' => $uid, 'openid' => $openid]);

    }

    function weibologin_get_user_by_openid($openid)
    {
        $uid = $this->db->get($this->table['weibo_login'], 'uid', ['openid' => $openid]);
        if (!empty($uid))
            return $uid;

        return false;
    }

    function weibologin_register($uid, $openid)
    {
        if ($uid <> false && $openid <> false)
            $this->db->insert($this->table['weibo_login'], ['uid' => $uid, 'openid' => $openid]);


    }

    //检测用户名格式
    function check_username(&$username)
    {
        $username = trim($username);
        if (empty($username)) {
            return '用户名不能为空。';
        } elseif (utf8::strlen($username) > 8) {
            return '不超过8个字符';
        } elseif (str_replace(array("\t", "\r", "\n", ' ', '　', ',', '，', '-'), '', $username) != $username) {
            return '不能含有空格和 , - 等字符';
        } elseif (htmlspecialchars($username) != $username) {
            return '不能含有HTML字符';
        }
        return false;
    }

    function check_password(&$password)
    {
        if(empty($password)){
            return '请输入密码！';
        }elseif (utf8::strlen($password) > 32) {
            return '密码不能超过 32 个字符！';
        }
        return false;
    }

    function check_password2(&$password, &$password2)
    {
        if ($password != $password2) {
            return '两次输入的密码不符合';
        }
        return false;
    }

    function check_homepage(&$homepage)
    {
        if (!empty($homepage)) {
            core::htmlspecialchars($homepage);
            if (utf8::strlen($homepage) > 40) {
                return '网址不能超过 40 个字符';
            } elseif (!misc::is_url($homepage)) {
                return '网址格式不正确！';
            }
        }
        return false;
    }

    function md5_md5($s, $salt = '')
    {
        return md5(md5($s) . $salt);
    }

    // ----------------------> 加密解密
    function verify_password($password1, $password2, $salt)
    {
        return $this->md5_md5($password1, $salt) == $password2;
    }

    //注册
    function register($user)
    {
        $uid = $this->db->insert($this->table['user'], ['username' => $user['username'], 'password' => $user['password'], 'email' => $user['email'], 'regip' => core::ip(), 'regdate' => $_SERVER['time'], 'salt' => $user['salt'], 'deviceid' => $user['deviceid']]);
        $this->db->insert($this->table['user_profile2'], ['uid' => $uid]);
        //性别初始化
        !isset($user['gender']) && $user['gender'] = 0;
        $this->db->insert($this->table['user_profile'], ['uid' => $uid, 'realname' => '', 'mobile' => '', 'birthday' => 0, 'idcard' => '', 'address' => '', 'zipcode' => '', 'gender' => $user['gender'], 'bio' => '']);

        return ['uid'=>$uid,'username'=>$user['username']];
    }

    //登录
    function  getuser($email)
    {
        $user = $this->db->get($this->table['user'], ['uid', 'password', 'username', 'salt'], ['OR' => ['username' => $email, 'email' => $email]]);
        return $user;

    }

    function  get_user_by_uid($uid)
    {
        $user = $this->db->get($this->table['user'], ['uid', 'password', 'username', 'salt', 'deviceid'], ['uid' => $uid]);
        return $user;

    }

    //更新
    function update($user)
    {
        $this->db->update($this->table['user'], ['password' => $user['password']], ['uid' => $user['uid']]);
    }

    //根据表名和字段更新
    function update_user($table = 'user', $value, $where)
    {
        return $this->db->update($this->table[$table], $value, $where);

    }

    function get_family_by_uid($uid = 0, $fid = 0)
    {
        if ($uid <> false) {
            if ($fid) {
                return $this->db->get($this->table['user_family'], ['fid', 'uid', 'type', 'username', 'mobile', 'realname', 'idcard'], ['AND' => array('uid' => $uid, 'fid' => $fid)]);
            } else {
                return $this->db->select($this->table['user_family'], ['fid', 'uid', 'type', 'username', 'mobile', 'realname', 'idcard'], ['uid' => $uid]);
            }
        }
        return false;

    }

    //新增我的家人
    function family_add($where)
    {
        return $this->db->insert($this->table['user_family'], $where);

    }

    //修改我的家人
    function family_change($value, $where)
    {
        return $this->db->update($this->table['user_family'], $value, ['AND' => $where]);
    }

    function family_delete($where)
    {
        return $this->db->delete($this->table['user_family'], ['AND' => $where]);
    }

    /**
     * 设置登陆cookie
     * @param $user
     * @param int $time
     */
    function set_login_cookie($user, $savetime)
    {
//         包含登录信息，重要。HTTP_ONLY
//        $savetime += 86400 * 7; // 默认设置为一周
//        $time += -1;
        core::setcookie($this->cookie['cookie_pre'] . 'user_auth', core::authcode($user['uid'] . "\t" . $user['username'], 'ENCODE'), $savetime, $this->cookie['cookie_path'], $this->cookie['cookie_domain'], false);
        $this->update_user('user', ['lastloginip' => ip2long(core::ip()), 'lastlogintime' => $_SERVER['time']], ['uid' => $user['uid']]);
    }

    //得到我的好友
    function get_friends_by_uid($uid)
    {
        if ($uid <> false) {
            $sql = 'SELECT  a.`friendid`, a.`direction`, a.`delstatus`, a.`comment` FROM `user_friends` a left join `user_profile` b on a.friendid=b.uid WHERE a.uid=' . $uid . ' limit 1000';
            return $this->db->query($sql)->fetchAll(PDO::FETCH_ASSOC);
        }
        return false;
    }

    function get_user($uid = 0)
    {
        if ($uid <> false) {
            $user = $this->db->get($this->table['user'], ['uid', 'username', 'email', 'groupid', 'avatar', 'regip', 'regdate'], ['uid' => intval($uid)]);
            if (!empty($user))
                $user_profile = $this->db->get($this->table['user_profile'], ['realname', 'gender', 'mobile', 'birthday', 'idcardtype', 'idcard', 'address', 'zipcode', 'bio'], ['uid' => intval($uid)]);
            // var_dump($user_profile);
            // 111
            // 111
            $user_profile =$user_profile == false ? ['realname' => '', 'gender' => '', 'mobile' => '', 'birthday' => '', 'idcardtype' => '', 'idcard' => '', 'address' => '', 'zipcode' => '', 'bio' => '']:$user_profile;
            $user_profile2 = $this->db->get($this->table['user_profile2'], ['threads', 'posts', 'newpms', 'creadits'], ['uid' => intval($uid)]);
            $user_profile2 == false && $user_profile2 = ['threads' => 0, 'posts' => 0, 'newpms' => 0, 'creadits' => 5];
            return array_merge($user, $user_profile, $user_profile2);
        }
        return false;
    }

    /**
     * 添加用户反馈
     * @param $data
     * @return bool
     */
    function addFeedback($data)
    {

        if (empty($data))
            return false;
        else
            $data['created_at'] = time();
        return $this->db->insert($this->table['feedback'], $data);
    }

    /**
     * 添加用户经纬度信息
     * @param array $data
     */
    function addUserLngLat($data = array())
    {
        if (empty($data))
            return false;
        else
            $data['created_at'] = time();
        return $this->db->insert($this->table['user_lng_lat'], $data);
    }

    /**
     * 更新用户经纬度
     * @param array $condition
     * @param array $data
     */
    function updateUserLngLat($condition = array(), $data = array())
    {
        if (empty($data))
            return false;
        $where = array();
        if (isset($condition['uid'])) {
            $where['AND']['uid'] = $condition['uid'];
        }
        $data['created_at'] = time();
        return $this->db->update($this->table['user_lng_lat'], $data, $where);
    }

    /**
     * 获取用户经纬度数据总数
     * @param $condition
     */
    function getUserLngLatCount($condition = array())
    {
        $where = array();
        if (isset($condition['uid'])) {
            $where['AND']['uid'] = $condition['uid'];
        }

        return $this->db->count($this->table['user_lng_lat'], $where);
    }

    /**
     * 获取附近的人
     * @param $lng          经度
     * @param $lat          纬度
     * @param $distance     距离（km）
     * @param int $limit 每页显示数量
     * @param int $offset 从第几个开始
     */
    function getNearUsers($lng, $lat, $distance = 20, $limit = 20, $offset = 0)
    {
        //获取每段距离正方形四点
        $squarePoint = $this->returnSquarePoint($lng, $lat, $distance);
        $sql = 'SELECT uid,lng,lat,
                ROUND(' . EARTH_RADIUS . '*2*ASIN(SQRT(POW(SIN((' . $lat . '*PI()/180-lat*PI()/180)/2),2)+COS(' . $lat . '*PI()/180)*COS(lat*PI()/180)*POW(SIN((' . $lng . '*PI()/180-lng*PI()/180)/2),2)))*1000)
                AS distance FROM `' . $this->table['user_lng_lat'] . '`';

        if ($distance && !empty($squarePoint)) {
            $sql .= ' where lat > ' . $squarePoint['right-bottom']['lat'] . ' and lat < ' . $squarePoint['left-top']['lat'];
            $sql .= ' and lng >  ' . $squarePoint['left-top']['lng'] . ' and lng < ' . $squarePoint['right-bottom']['lng'];
        }
        $sql .= ' ORDER BY distance DESC ';
        $sql .= ' LIMIT ' . $offset . ',' . $limit;

        $query = $this->db->query($sql);
        return $query->fetchAll(PDO::FETCH_ASSOC);
    }

    /**
     *计算某个经纬度的周围某段距离的正方形的四个点
     *
     * @param lng float 经度
     * @param lat float 纬度
     * @param distance float 该点所在圆的半径，该圆与此正方形内切，默认值为0.5千米
     * @return array 正方形的四个点的经纬度坐标
     */
    function returnSquarePoint($lng, $lat, $distance = 0.5)
    {

        $dlng = 2 * asin(sin($distance / (2 * EARTH_RADIUS)) / cos(deg2rad($lat)));
        $dlng = rad2deg($dlng);

        $dlat = $distance / EARTH_RADIUS;
        $dlat = rad2deg($dlat);

        return array(
            'left-top' => array('lat' => round(($lat + $dlat), 6), 'lng' => round(($lng - $dlng), 6)),
            'right-top' => array('lat' => round(($lat + $dlat), 6), 'lng' => round(($lng + $dlng), 6)),
            'left-bottom' => array('lat' => round(($lat - $dlat), 6), 'lng' => round(($lng - $dlng), 6)),
            'right-bottom' => array('lat' => round(($lat - $dlat), 6), 'lng' => round(($lng + $dlng), 6))
        );
    }

    /**
     * 获取用户信息及介绍
     * @param $uid
     */
    function getUserProfileInfo($uid)
    {
        $join['[>]' . $this->table['user_profile']] = array('uid' => 'uid');

        $data = $this->db->select($this->table['user'], $join,
            [
                $this->table['user'] . '.uid',
                $this->table['user'] . '.username',
                $this->table['user'] . '.avatar',
                $this->table['user_profile'] . '.bio'
            ],
            [$this->table['user'] . '.uid' => $uid]);
        if (isset($data[0])) {
            return $data[0];
        } else {
            return false;
        }
    }

    /**
     * 通过用户uid获取用户信息及介绍
     * @param array $uids
     * @return bool
     */
    function getUserProfileInfoByUids($uids = array())
    {
        $join['[>]' . $this->table['user_profile']] = array('uid' => 'uid');

        $data = $this->db->select($this->table['user'], $join,
            [
                $this->table['user'] . '.uid',
                $this->table['user'] . '.username',
                $this->table['user'] . '.avatar',
                $this->table['user_profile'] . '.bio'
            ],
            [$this->table['user'] . '.uid' => $uids]);

        return $data;

    }
    /**
     * @desc 根据两点间的经纬度计算距离
     * @param float $lat 纬度值
     * @param float $lng 经度值
     */
    function getDistance($lat1, $lng1, $lat2, $lng2)
    {
        //地球半径
        $earthRadius = 6367000;
        /*
          Convert these degrees to radians
          to work with the formula
        */

        $lat1 = ($lat1 * pi()) / 180;
        $lng1 = ($lng1 * pi()) / 180;

        $lat2 = ($lat2 * pi()) / 180;
        $lng2 = ($lng2 * pi()) / 180;


        $calcLongitude = $lng2 - $lng1;
        $calcLatitude = $lat2 - $lat1;
        $stepOne = pow(sin($calcLatitude / 2), 2) + cos($lat1) * cos($lat2) * pow(sin($calcLongitude / 2), 2);
        $stepTwo = 2 * asin(min(1, sqrt($stepOne)));
        //获取2经纬度之间距离
        $calculatedDistance = $earthRadius * $stepTwo;

        return round($calculatedDistance);
    }

    /**
     * 获取签到日志数量
     * @param $condition
     */
    function getSignLogCount($condition)
    {
        $where = array();
        //uid
        if (isset($condition['uid'])) {
            $where['AND']['uid'] = $condition['uid'];
        }
        //created_day 当天
        if (isset($condition['created_day'])) {
            $where['AND']['created_day'] = $condition['created_day'];
        }
        return $this->db->count($this->table['user_sign_log'], $where);
    }
    /*
     * 添加用户铃声
     */
    function addbell($data)
    {
        if (empty($data))
            return false;
        return $this->db->insert($this->table['user_bell'], $data);
    }
    /**
     * 获取用户铃声总数
     * @param $condition
     */
    function getBellCount($condition = array())
    {
        $where = array();
        if (isset($condition['uid'])) {
            $where['AND']['uid'] = $condition['uid'];
        }

        return $this->db->count($this->table['user_bell'], $where);
    }

    /**
     * 更新用户铃声
     * @param $value
     * @param $where
     * @return mixed
     */
    function updateBell($data, $where)
    {
        return $this->db->update($this->table['user_bell'], $data, ['AND' => $where]);
    }
    /*
     * 获取右侧活跃安友
     */
    function getFaviorateUser($limit = 6){
        $data = $this->redis->get('user:getfaviorate'.$limit);
        $data =false;
        if($data === false){
            $where['ORDER'] = 'creadits DESC';
            $where['LIMIT'] = $limit;
            $uid = $this->db->select('user_profile2',['uid'],$where);
            $user = $_u = array();
            foreach($uid as $u){
                $_u[] = $u['uid'];
            }
            if(!empty($_u))
                $user = $this->getUserByUid($_u);
            $this->redis->set('user:getfaviorate'.$limit,$user,3600*24);
        }
        return $user;
    }
    //根据uid获得user信息
    function getUserByUid($uid){
        $join['[>]' . $this->table['user_profile']] = array('uid' => 'uid');

        $data = $this->db->select($this->table['user'], $join,
            [
                $this->table['user'] . '.uid',
                $this->table['user'] . '.username',
                $this->table['user'] . '.avatar',
                $this->table['user_profile'] . '.bio'
            ],
            [$this->table['user'] . '.uid' => $uid]);
        return $data;
    }
    /**
     * 添加签到
     * @param $data
     */
    function addSignLog($data)
    {
        if (empty($data))
            return false;
        else
            $data['created_at'] = time();
        $lastid= $this->db->insert($this->table['user_sign_log'], $data);
        if($lastid)
            $this->db->update($this->table['user_profile2'],['creadits[+]'=>1],['uid'=>$data['uid']]);
        return $lastid;
    }
    /**
     * 判断用户今天是否签到
     */
    function isSign($uid){
        $data = $this->redis->get('User:issign:uid'.$uid);
        if($data==false)
            return false;
        else
            return true;
    }
    /**
     * 添加签到
     */
    function addSign($uid){
        $data['created_at'] = time();
        $data['created_day'] = date('Y-m-d',$data['created_at']);
        $data['uid'] = $uid;
        //插入签到表
        $this->db->insert($this->table['user_sign_log'], $data);
        //更新用户扩展表
        $this->db->update($this->table['user_profile2'],['creadits[+]'=>1],['uid'=>$data['uid']]);
        //当天剩余时间
        $time = 86400-(time()-strtotime(date('Y-m-d')));
        //将今天是否签到信息存入redis
        $this->redis->set('User:issign:uid'.$uid,1,$time);
        return true;
    }
    //根据省份获取城市
    function getCity($provinceid){
        $data = $this->redis->get('User:getCity:provinceid'.$provinceid);
        if($data==false){
            $data = $this->db->select('area',['id','name'],['AND'=>['pid'=>$provinceid]]);
            $this->redis->set('User:getCity:provinceid'.$provinceid,$data,REDISTIME);
        }
        return $data;
    }
    //获取他人信息
    function getUserData($uid){
        $data = $this->redis->get('User:getUserData:uid'.$uid);
        if($data==false){
            $join['[>]user_profile'] = ['uid'=>'uid'];
            $where['AND'] = ['user.uid'=>$uid];
            $data = $this->db->select('user',$join,['user.uid','user.username','user_profile.provinceid','user_profile.bio','user_profile.gender'],$where);
            if(empty($data[0]['provinceid']))
                $data[0]['name']='';
            else
                $data[0]['name'] = $this->db->get('area','name',['id'=>$data[0]['provinceid']]);
            switch($data[0]['gender']){
                case 0:
                    $data[0]['gender'] = '保密';
                    break;
                case 1:
                    $data[0]['gender'] = '男';
                    break;
                case 2:
                    $data[0]['gender'] = '女';
                    break;
                default:
                    $data[0]['gender'] = '保密';
            }
            $this->redis->set('User:getUserData:uid'.$uid,$data,REDISTIME);
        }
        return $data[0];
    }
    //获取他人的话题
    function getUserThread($uid,$limit){
        $data = $this->redis->get('User:getUserThread:uid'.$uid.':limit'.$limit);
        if($data==false){
            $join['[>]thread_views'] = ['tid'=>'tid'];
            $join['[>]group'] = ['gid'=>'gid'];
            $where['AND'] = ['thread.authorid'=>$uid,'group.version'=>'1.1','thread.closed'=>0,'thread.status'=>0];
            $where['LIMIT'] = [$limit,10];
            $data = $this->db->select('thread',$join,['thread.tid','thread.gid','thread.subject','thread.dateline','thread_views.replies','thread_views.views'],$where);
            if(!empty($data)){
                foreach($data as $k=>$v){
                    $data[$k]['dateline'] = misc::tranTime($v['dateline']);
                }
            }
            $this->redis->set('User:getUserThread:uid'.$uid,$data,REDISTIME);
        }
        return $data;
    }
    //获取他人的话题总数
    function getUserThreadCount($uid){
        $data = $this->redis->get('User:getUserThreadCount:uid'.$uid);
        if($data==false){
            $join['[>]group'] = ['gid'=>'gid'];
            $where = ['AND'=>['thread.authorid'=>$uid,'group.version'=>'1.1','thread.closed'=>0,'thread.status'=>0]];
            $arr = $this->db->select('thread',$join,['thread.tid'],$where);
            $this->redis->set('User:getUserThreadCount:uid'.$uid,count($arr),REDISTIME);
        }
        return $data;
    }
    //获取活跃安友
    function getAnyou($gid,$flag,$limit,$isloginindex=0){
        $data = $this->redis->get('User:getAnyou:gid'.$gid.':flag'.$flag.':limit'.$limit);
        $data=false;
        if($data==false){
            //第一个用户使用时安好小秘书
            $data = ($isloginindex==0) ? $this->db->select('user','uid',['uid'=>448]) : [];
//            $time1 = strtotime('last Monday')-7*86400;
//            $time2 = strtotime('last Sunday');
            $time1 = strtotime('today')-86400;
            $time2 = strtotime('today')+86400;
            switch($flag)
            {
                case 'index':
                    $cond = 1;
                    break;
//                case 'group':
//                    $arr = $this->getGroupId($gid);
//                    $str = implode(',',$arr);
//                    $cond = 'gid in('.$str.')';
//                    break;
//                case 'grouplist':
//                    $cond = 'gid='.$gid;
//                    break;
                default:
                    $cond = 1;
            }
            $between = ' AND creatat BETWEEN '.$time1.' AND '.$time2;
            $where = $cond;
            $sql = 'SELECT uid FROM active_user WHERE '.$where.'  group by uid order by sum(number) desc limit 0,'.$limit;
            $data1 = $this->db->query($sql)->fetchAll(PDO::FETCH_COLUMN);
            $data = array_merge($data,$data1);
            //缓存时间从当天到下周一0点
            $redistime = strtotime('Monday')-time();
            $this->redis->set('User:getAnyou:gid'.$gid,$data,$redistime);
        }
        if(!empty($data)){
            foreach($data as $v){
                $res[] = $this->getUserInfo($v);
            }
        }
        return isset($res) ? $res : '';
    }
    //获取用户信息
    function getUserInfo($uid){
        $data = $this->redis->get('User:getUserInfo:uid'.$uid);
        if($data==false){
            $join['[>]user_profile'] =['uid'=>'uid'];
            $where['AND'] = ['user.uid'=>$uid];
            $data = $this->db->select('user',$join,['user.uid','user.username','user_profile.bio','user_profile.gender'],$where);
            $this->redis->set('User:getUserInfo:uid'.$uid,$data,REDISTIME);
        }
        return isset($data[0]) ? $data[0] : [];
    }
    //用户注册时关注的安友
    function getAnyouToRegister(){
        $data = $this->redis->get('User:getAnyouToRegister');
        if($data==false){

        }
        $join['[>]user_profile'] = ['uid'=>'uid'];
        $where['LIMIT'] = [0,15];
        return $this->db->select('user',$join,['user.uid','user.username','user_profile.bio'],$where);
    }
    //获取一级圈子下的所有二级gid
    function getGroupId($gid){
        $data = $this->redis->get('User:getGroupId:gid'.$gid);
        if($data==false){
            $data = $this->db->select('group','gid',['AND'=>['pid'=>'gid','status'=>0]]);
            $this->redis->set('User:getGroupId:gid'.$gid,$data,REDISTIME);
        }
        return $data;
    }
    //获取欢迎页中间的用户及他发表的帖子
    function getMiddle($uid){
        $data = $this->redis->get('User:getMiddle:uid'.$uid);
        $data=false;
        if($data==false){
            $join['[>]user'] = ['authorid'=>'uid'];
            $join['[>]group'] = ['gid'=>'gid'];
            $where['AND'] = ['user.uid'=>$uid,'thread.closed'=>0,'thread.status'=>0,'group.version'=>'1.1','group.status'=>0,'group.pid[!]'=>0];
            $where['ORDER'] = 'thread.dateline DESC';
            $where['LIMIT'] = [0,6];
            $data['thread'] = $this->db->select('thread',$join,['thread.tid','thread.subject','thread.authorid'],$where);
            $join2['[>]user_profile'] = ['uid'=>'uid'];
            $cond = ['user.uid'=>$uid];
            $arr = $this->db->select('user',$join2,['user.uid','user.username','user_profile.bio'],$cond);
            $data['user'] = $arr[0];
            $data['user']['threadcount'] = $this->db->count('thread',['AND'=>['authorid'=>$data['user']['uid']]]);
            $this->redis->set('User:getMiddle:uid'.$uid,$data,REDISTIME);
        }
        return $data;
    }

}