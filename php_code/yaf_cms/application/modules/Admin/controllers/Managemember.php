<?php

if (!defined('APP_PATH'))
    exit('No direct script access allowed');
/**
  用户管理
  @author yangguofeng
  @package admin
  @version 1.0 2014/11/25
 */
Yaf_Loader::import(PATH_APP . 'modules/Admin/controllers/Admin.php');
Yaf_Loader::import(PATH_APP . 'modules/Admin/models/Member.php');
Yaf_Loader::import(APP_PATH . '/library/Alert.php');

class ManagememberController extends AdminController {

    /**
      初始化
      @param void
      @return void
     */
    function init() {
        parent::init();
        //db 实例化
        $this->member = new MemberModel();
    }

    public function testAction() {
        echo 12;
        $menu = [];
        $ret = $this->member->getParentMenu(9);
        var_dump($ret);
        foreach ($ret as $key => $value) {
            $menu[$key]['name'] = $value['menu_name'];
            $child = $this->member->getChildMenu($value['menu_id'],9);
            $menu[$key]['child'] = $child;
        }
        var_dump($menu);
    }

    /**
      默认页（会员列表页）
      @param void
      @return void
     */
    public function indexAction() {
        $data = [];
        $page = $this->getRequest()->getQuery('page') ? $this->getRequest()->getQuery('page') : 1;

        $offset = 10;
        $start = ($page - 1) * $offset;
        $data['data'] = $this->member->memberList(NULL, $start, $offset);

        $total = $this->member->count();

        $totalpage = ceil($total / $offset);
        //超过一页显示分页
        if ($totalpage > 1) {
            $data['page'] = (new multi())->getSubContent('/admin/managemember/index?page=', '', $totalpage, $page, 9, ' target="_self" ', '', '', 'no');
        }
        $this->getView()->assign('data', $data);
    }

    public function addAction() {
        $data = [];
        if ($this->getRequest()->isPost()) {
            $data['username'] = core::safe_str($this->getRequest()->getPost('username', 123));
            $data['password'] = core::safe_str($this->getRequest()->getPost('password'));
            if ($this->member->save($data)) {
                //Alert::success('保存成功！');
                $this->redirect('/admin/managemember/index');
            } else {
                Alert::success('保存失败！');
            }
        }
    }

    public function editAction() {
        $id = intval($this->getRequest()->getQuery('id'));

        if ($this->getRequest()->isPost()) {
            $value = [
                'username' => core::safe_str($this->getRequest()->getPost('username')),
                'name' => core::safe_str($this->getRequest()->getPost('name')),
                'nickname' => core::safe_str($this->getRequest()->getPost('nickname')),
                'gender' => intval($this->getRequest()->getPost('gender')),
                'phone' => core::safe_str($this->getRequest()->getPost('phone')),
                'email' => core::is_email($this->getRequest()->getPost('email')) ? $this->getRequest()->getPost('email') : NULL,
                'status' => intval($this->getRequest()->getPost('status')),
                'birthday' => misc::is_date($this->getRequest()->getPost('birthday'),'-')?strtotime($this->getRequest()->getPost('birthday')):NULL,
            ];

            if ($this->member->update($value, ['id' => intval($this->getRequest()->getPost('id'))])) {
                Alert::success('更新成功！');
            } else {
                Alert::success('没有任何变化！');
            }
            $this->redirect($this->getRequest()->getPost('referer'));
        } else {
            if ($data = $this->member->getMemberByAttribute(['id' => $id])) {
                $data['referer'] = $_SERVER["HTTP_REFERER"];
                $this->getView()->assign('data', $data);
            } else {
                Alert::success('没有此用户！');
                $this->redirect('/admin/managemember/index');
            }
        }
    }

    public function deleteAction() {
        $id = intval($this->getRequest()->getQuery('id'));
        if ($this->member->has(['id' => $id])) {
            $this->member->delete(['id' => $id]);
            Alert::success('删除成功！');
        } else {
            Alert::success('没有此用户！');
        }
        $this->redirect($_SERVER["HTTP_REFERER"] ? $_SERVER["HTTP_REFERER"] : '/admin/managemember/index');
    }

}
