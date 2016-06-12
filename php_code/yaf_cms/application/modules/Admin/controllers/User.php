<?php

if (!defined('APP_PATH'))
    exit('No direct script access allowed');
/**
  用户类
  @author lucas lucasphper@gmail.com
  @access public
  @package base
  @version 1.0 2014/11/19
 */
include_once(PATH_APP . 'modules/Admin/controllers/Admin.php');
include_once(PATH_APP . 'modules/Admin/models/User.php');

class UserController extends AdminController {

    /**
      初始化
      @param void
      @return void
     */
    function init() {
        parent::init();

        //db 实例化
        $this->user = new UserModel();
    }

    /**
      登录页
      @param void
      @return void
     */
    function loginAction() {
        $this->getView();
    }

    /**
      处理登录
      @param void
      @return void
     */
    function dealLoginAction() {
        //暂不考虑过滤问题
        $username = trim($this->getRequest()->getPost('username', 0));
        if (!$username) {
            Yaf_Controller_Abstract::redirect("/admin/user/login");
            exit;
        }

        $password = trim($this->getRequest()->getPost('password', 0));
        if (!$password) {
            Yaf_Controller_Abstract::redirect("/admin/user/login/");
            exit;
        }

        $code_arr = (array) Yaf_Session::getInstance();
        foreach ($code_arr as $key => $value) {
            $code_arr_tmp = $value;
            break;
        }
        $code = strtolower(trim($this->getRequest()->getPost('code', 0)));
        if ($code_arr_tmp['vcode'] != $code) {
            Yaf_Controller_Abstract::redirect("/admin/user/login/");
            exit;
        }

       
        //获取用户详情
        $user_detail = $this->user->get_detail_by_username($username);

        //信息判断
        if (empty($user_detail)) {
            Yaf_Controller_Abstract::redirect("/admin/user/login/");
            exit;
        }
        if ($user_detail[0]['password'] != md5($password)) {
            Yaf_Controller_Abstract::redirect("/admin/user/login/");
            exit;
        }

        //账号是否被禁用
        if ($user_detail[0]['roleid'] == 1) {
            //超级管理员不能被禁用
        } else {
            if (!(int) $user_detail[0]['id']) {
                Yaf_Controller_Abstract::redirect("/admin/user/login/");
                exit;
            }
        }

        //设置cookie
        $user_auth_cleartext = $user_detail[0]['id'] . '\t' . $user_detail[0]['username'] . '\t' . $user_detail[0]['roleid'];
        $user_auth = core::authcode($user_auth_cleartext, 'ENCODE');
        $cookie_pre = Yaf_Registry::get('config')->get('cookie')->pre;
        $cookie_path = Yaf_Registry::get('config')->get('cookie')->path;
        $cookie_domain = Yaf_Registry::get('config')->get('cookie')->domain;
        $result = setcookie($cookie_pre . 'user_auth', $user_auth, time() + 3600 * 24, $cookie_path, $cookie_domain);
        if (!$result) {
            Yaf_Controller_Abstract::redirect("/admin/user/login/");
        }

        //本地保存本次登录时间
        setcookie($cookie_pre . 'last_login_date', time(), time() + 3600 * 24 * 7, $cookie_path, $cookie_domain);

        Yaf_Controller_Abstract::redirect("/admin/index/index/");
        exit;
    }

    /**
      处理注销
      @param void
      @return void
     */
    function dealLogoutAction() {
        $cookie_path = Yaf_Registry::get('config')->get('cookie')->path;
        $cookie_domain = Yaf_Registry::get('config')->get('cookie')->domain;
        $result = setcookie(Yaf_Registry::get('config')->get('cookie')->pre . 'user_auth', '', time() - 3600 * 24, $cookie_path, $cookie_domain);

        if (isset($_SESSION['cms']['privilege_allowed_url']) && count($_SESSION['cms']['privilege_allowed_url'])) {
            unset($_SESSION['cms']['privilege_allowed_url']);
            unset($_SESSION['cms']['privilege_allowed_page']);
        }

        //更新上次登录时间
        if (isset($_COOKIE[$this->cookie['cookie_pre'] . 'last_login_date']) && $_COOKIE[$this->cookie['cookie_pre'] . 'last_login_date']) {
            $last_login_time = $_COOKIE[$this->cookie['cookie_pre'] . 'last_login_date'];
            $cookie_path = Yaf_Registry::get('config')->get('cookie')->path;
            $cookie_domain = Yaf_Registry::get('config')->get('cookie')->domain;
            setcookie($this->cookie['cookie_pre'] . 'last_login_date', time(), time() - 3600 * 24 * 7, $cookie_path, $cookie_domain);

            $user_detail['logindatetime'] = $_COOKIE[$this->cookie['cookie_pre'] . 'last_login_date'];
            $this->user->update_admin_user($user_detail, $this->_user['uid']);
        }
        unset($this->_user['uid']);

        Yaf_Controller_Abstract::redirect("/admin/user/logout/");
    }

    /**
      注销页
      $param void
      @return void
     */
    function logoutAction() {
        $this->getView();
    }

    /**
     * 修改密码页
     */
    function changePasswordAction() {
        $this->getView();
    }

    /**
     * 处理修改密码
     */
    function dealChangePasswordAction() {
        //获取旧密码
        $oldPassword = trim($this->getRequest()->getPost('oldpassword', 0));
        if (!$oldPassword) {
            Yaf_Controller_Abstract::redirect("/admin/user/index/");
        }

        //获取新密码和确认密码
        $newPassword = trim($this->getRequest()->getPost('newpassword', 0));
        $confirmPassword = trim($this->getRequest()->getPost('confirmpassword', 0));

        //判断新密码和确认密码是否相等,如果不等 跳转到默认页面
        if ($newPassword != $confirmPassword) {
            echo '<script>alert("两次输入的密码不相等！");window.history.go(-1);</script>';
            exit;
        }

        if (!$this->_user['uid']) {
            echo '<script>alert("请重新登录！");window.location.href="/admin/user/login/"</script>';
            exit;
        }

        //获取用户详情
        $user_detail = $this->user->get_detail_by_userid($this->_user['uid']);

        //信息判断
        if (empty($user_detail)) {
            echo '<script>alert("用户信息错误！");window.history.go(-1);</script>';
            exit;
        }

        if ($user_detail[0]['password'] != md5($oldPassword)) {
            echo '<script>alert("原密码错误！");window.history.go(-1);</script>';
            exit;
        }

        //执行修改密码操作
        if ($this->user->changePassword($this->_user['uid'], md5($newPassword))) {
            echo '<script>alert("密码修改成功！");</script>';
        } else {
            echo '<script>alert("密码修改失败！");</script>';
        }

        Yaf_Controller_Abstract::redirect("/admin/index/index/");
    }

    /**
     * 忘记密码页面
     */
    function forgetPasswordAction() {
        //查找出所有的问题
        $questionList = $this->user->getQuestionList();
        $this->getView()->assign('questionlist', $questionList);
    }

    /**
     * 验证用户提问
     */
    function dealForgetPasswordAction() {

        //获取用户名
        $username = trim($this->getRequest()->getPost('username', 0));
        if (!$username) {
            echo '<script>alert("用户名不得为空");</script>';
            Yaf_Controller_Abstract::redirect("/admin/user/forgetPassword/");
            exit;
        }

        //判断用户是否存在
        $user_detail = $this->user->get_detail_by_username($username);

        if (!$user_detail) {
            echo '<script>alert("用户信息不存在");</script>';
            Yaf_Controller_Abstract::redirect("/admin/user/forgetPassword/");
            exit;
        }


        //获取问题
        $question = trim($this->getRequest()->getPost('question', 0));
        if (!$question) {
            echo '<script>alert("密保问题不得为空");window.location.href=history.go(-1);</script>';
            exit;
        }

        //获取答案
        $answer = trim($this->getRequest()->getPost('answer', 0));
        if (!$answer) {
            echo '<script>alert("答案不得为空");window.location.href=history.go(-1);</script>';
            exit;
        }

        //验证问题和答案
        if ($question == $user_detail[0]['question'] && $answer == $user_detail[0]['answer']) {
            //验证通过显示重置密码页面
            $session = Yaf_Session::getInstance();
            $session->resetPassword = array(
                'key' => md5($user_detail[0]['username']),
                'userid' => $user_detail[0]['id']);

            $this->getView()->assign('key', md5($user_detail[0]['username']));
        } else {
            //未通过验证
            //Yaf_Controller_Abstract::redirect("/admin/user/login/");
            echo '<script>alert("答案错误");window.location.href=history.go(-1);</script>';
            exit;
        }
    }

    /**
     * 重置密码
     */
    function resetPasswordAction() {
        //判断来路域名是否合法
        if ($_SERVER["HTTP_REFERER"] != Yaf_Registry::get('config')->get('url') . 'admin/user/dealForgetPassword/') {
            echo '<script>alert("非法请求！");window.history.go(-1);</script>';
            exit;
        }

        //判断本次修改密码是否合法
        $session = Yaf_Session::getInstance();
        $resetPassword = $session->resetPassword;

        $key = trim($this->getRequest()->getPost('key', 0));

        if ($key != $resetPassword['key']) {
            echo '<script>alert("非法请求！");window.history.go(-1);</script>';
            exit;
        }

        //获取密码和确认密码
        $newPassword = trim($this->getRequest()->getPost('newpassword', 0));
        $confirmPassword = trim($this->getRequest()->getPost('confirmpassword', 0));

        //判断新的密码和确认密码是否有效
        if (!$newPassword || !$confirmPassword) {
            echo '<script>alert("密码不得为空！");window.history.go(-1);</script>';
            exit;
        }

        //判断新密码和确认密码是否相等,如果不等 后退一页
        if ($newPassword != $confirmPassword) {
            echo '<script>alert("两次输入的密码不相等！");window.history.go(-1);</script>';
            exit;
        }

        //判断重置密码结果
        if ($this->user->changePassword($resetPassword['userid'], md5($newPassword))) {
            echo '<script>alert("密码修改成功！");</script>';
        } else {
            echo '<script>alert("密码修改失败！");</script>';
        }

        //清除session
        unset($session->resetPassword);

        //跳转到登录页面
        Yaf_Controller_Abstract::redirect("/admin/user/login/");
    }

    /**
     * 验证码图片流
     * @param void
     * @return image obj
     */
    function codeAction() {
        $code = new Vcode();  //实例化一个对象
        $code->doimg();
    }

    /**
     * 后台用户管理列表页面
     */
    function AdminListAction() {
        $data = [];
        $page = $this->getRequest()->getQuery('page') ? $this->getRequest()->getQuery('page') : 1;

        //每页显示数
        $offset = 10;
        $start = ($page - 1) * $offset;
        $data['data'] = $adminList = $this->user->get_admin_list($start, $offset);

        //总页数
        $total = $this->user->get_count_admin();
        $totalpage = ceil($total / $offset);
        //超过一页显示分页
        if ($totalpage > 1) {
            $data['page'] = (new multi())->getSubContent('/admin/user/adminlist?page=', '', $totalpage, $page, 9, ' target="_self" ', '', '', 'no');
        }
        $this->getView()->assign('data', $data);

        $this->getView()->assign("adminlist", $adminList);
    }

    /**
     * 添加用户页面
     */
    function addAdminAction() {
        //获取用户角色列表
        $role_list = $this->user->get_role_list();

        $this->getView()->assign('role_list', $role_list);
    }

    /**
     * 处理添加用户
     */
    function dealAddAdminAction() {
        //获取用户名
        $username = trim($this->getRequest()->getPost('user_name', 0));
        if (!$username) {
            echo '<script>alert("请填写用户名");window.history.go(-1);</script>';
            exit;
        }

        //判断用户名是否存在
        if ($this->user->is_username_exists($username)) {
            echo '<script>alert("用户名已存在");window.history.go(-1);</script>';
            exit;
        }

        //获取用户密码
        $password = trim($this->getRequest()->getPost('user_password'), 0);
        if (!$password) {
            echo '<script>alert("请填写密码");window.history.go(-1);</script>';
            exit;
        }

        if (strlen($password) < 7) {
            echo '<script>alert("用户密码不少于8位");window.history.go(-1);</script>';
            exit;
        }

        //获取email并判断email
        $email = trim($this->getRequest()->getPost('user_email'), '');
        if ($email) {
            if (!preg_match('/\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*/', $email)) {
                echo '<script>alert("请输入正确的邮箱地址");window.history.go(-1);</script>';
                exit;
            }
        } else {
            $email = '';
        }

        //获取role_id
        $role_id = intval(trim($this->getRequest()->getPost('user_role', 0)));

        $status = intval(trim($this->getRequest()->getPost('user_status', 0)));

        // 判断email是否已存在
        if ($this->user->is_email_exists($email)) {
            echo '<script>alert("邮箱已存在");window.history.go(-1);</script>';
            exit;
        }

        //组合数据
        $data['username'] = $username;
        $data['password'] = md5($password);
        $data['email'] = $email;
        $data['createdate'] = $_SERVER['time'];
        $data['status'] = $status;
        $data['roleid'] = $role_id;

        if ($this->user->insert_admin_user($data)) {
            echo '<script>alert("添加用户成功");window.location.href="/admin/user/adminlist/";</script>';
        } else {
            echo '<script>alert("添加用户失败");window.location.href="/admin/user/adminlist/";</script>';
        }
    }

    /**
     * 修改用户信息页面
     */
    function editAdminAction() {
        //获取并判断用户id
        $userid = isset($_GET['uid']) ? $_GET['uid'] : 0;
        if (!$userid) {
            echo '<script>alert("非法请求");window.location.href="/admin/user/adminlist/";</script>';
            exit;
        }

        //根据用户id获取用户信息
        $user_detail = $this->user->get_detail_by_userid($userid);

        if (!$user_detail) {
            echo '<script>alert("用户信息不存在");window.location.href="/admin/user/adminlist/";</script>';
            exit;
        }

        //获取用户角色列表
        $role_list = $this->user->get_role_list();

        $this->getView()->assign('role_list', $role_list);
        $this->getView()->assign('user_detail', $user_detail[0]);
    }

    /**
     * 处理用户信息修改
     */
    function dealEditAdminAction() {
        //获取数据
        $userid = intval(trim($this->getRequest()->getPost('user_id', 0)));
        $password = trim($this->getRequest()->getPost('user_password', 0));
        $roleid = intval($this->getRequest()->getPost('user_role', 0));
        $email = trim($this->getRequest()->getPost('user_email', 0));
        $status = $status = intval(trim($this->getRequest()->getPost('user_status', 0)));

        //判断用户id是否合法
        if (!$userid || $userid < 1) {
            echo '<script>alert("信息错误");</script>';
            exit;
        }

        //判断要不要修改密码
        if ($password) {
            if (strlen($password) < 7) {
                echo '<script>alert("密码不得少于8位");window.history.go(-1);</script>';
                exit;
            } else {
                $data['password'] = md5($password);
            }
        }

        //判断用户角色
        if (!$roleid) {
            echo '<script>alert("请选择合适的角色");window.history.go(-1);</script>';
            exit;
        }

        //判断email格式是否正确
        $pattern = '/\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*/';
        if (!preg_match($pattern, $email)) {
            echo '<script>alert("请输入正确的邮箱地址");window.history.go(-1);</script>';
            exit;
        }

        //判断email是否存在
        if ($this->user->is_email_exists($email, true)) {
            echo '<script>alert("邮箱已存在");window.history.go(-1);</script>';
            exit;
        }

        $data['email'] = $email;
        $data['roleid'] = $roleid;
        $data['updatedate'] = $_SERVER['time'];
        $data['status'] = $status;

        //执行操作
        if ($this->user->update_admin_user($data, $userid)) {
            echo '<script>alert("操作成功");window.location.href="/admin/user/adminlist/";</script>';
        } else {
            echo '<script>alert("操作失败");window.location.href="/admin/user/adminlist/";</script>';
        }

        //Yaf_Controller_Abstract::redirect("/admin/user/adminlist/");
    }

    /**
     * 停用用户账户
     */
    function disableAdminAction() {
        //获取用户的id 和要处理的类型type
        $userid = isset($_GET['uid']) ? intval($_GET['uid']) : 0;

        //判断用户id是否合法的
        if (!$userid || $userid < 1) {
            echo '<script>alert("参数错误");</script>';
            exit;
        }

        if ($this->user->disable_admin_user($userid)) {
            echo '<script>alert("操作成功");window.location.href="/admin/user/adminlist/";</script>';
        } else {
            echo '<script>alert("操作失败");window.location.href="/admin/user/adminlist/";</script>';
        }
    }

    /**
     * 启用用户账户
     */
    function enableAdminAction() {
        //获取用户的id 和要处理的类型type
        $userid = isset($_GET['uid']) ? intval($_GET['uid']) : 0;

        //判断用户id是否合法的
        if (!$userid || $userid < 1) {
            echo '<script>alert("参数错误");</script>';
            exit;
        }

        if ($this->user->enable_admin_user($userid)) {
            echo '<script>alert("操作成功");window.location.href="/admin/user/adminlist/";</script>';
        } else {
            echo '<script>alert("操作失败");window.location.href="/admin/user/adminlist/";</script>';
        }
    }

    /**
     * 删除用户账户
     */
    function deleteAdminAction() {
        //获取用户的id 和要处理的类型type
        $userid = isset($_GET['uid']) ? intval($_GET['uid']) : 0;

        //判断用户id是否合法的
        if (!$userid || $userid < 1) {
            echo '<script>alert("参数错误");</script>';
            exit;
        }

        if ($this->user->delete_admin_user($userid)) {
            echo '<script>alert("操作成功");window.location.href="/admin/user/adminlist/";</script>';
        } else {
            echo '<script>alert("操作失败");window.location.href="/admin/user/adminlist/";</script>';
        }
    }

    /**
     * 设置密保页
     * @param void
     * @return void
     */
    public function setPasswordProtectAction() {
        //获取管理员个人信息
        $user_profile = $this->db_user->get_detail_by_userid($this->_user['uid']);
        $this->getView()->assign('user_profile', $user_profile[0]);

        //获取问题列表
        $question_list = $this->user->getQuestionList();
        $this->getView()->assign('question_list', $question_list);
    }

    /**
     * 处理保存密码
     * @param void
     * @return void
     */
    public function dealAddProtectAction() {
        $question = intval($this->getRequest()->getPost("question", 0));
        $user_detail['question'] = $question;

        $answer = trim($this->getRequest()->getPost('answer', 0));
        $user_detail['answer'] = $answer;

        if (!($question && $answer)) {
            if (!$question && !$answer) {
                
            } else {
                Yaf_Controller_Abstract::redirect("/admin/user/setPasswordProtect");
                exit;
            }
        }

        $result = $this->user->update_admin_user($user_detail, $this->_user['uid']);
        if ($result) {
            echo "<script>alert('保存成功');location.href='/admin/user/setPasswordProtect'</script>";
        } else {
            Yaf_Controller_Abstract::redirect("/admin/user/setPasswordProtect");
            exit;
        }
        exit;
    }

}
