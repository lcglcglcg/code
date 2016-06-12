<?php if(!defined('APP_PATH')) exit('No direct script access allowed');
/**
角色类
@author lucas lucasphper@gmail.com
@access public
@package base
@version 1.0 2014/11/25
 */
Yaf_Loader::import(PATH_APP.'modules/Admin/controllers/Admin.php');
Yaf_Loader::import(PATH_APP.'modules/Admin/models/Role.php');
class RoleController extends AdminController
{

    /**
    初始化
    @param void
    @return void
     */
    function init(){
        parent::init();

        //db 实例化
        $this->db_role = new RoleModel();
    }

    /**
    默认页
    @param void
    @return void
     */
    public function indexAction()
    {
        //获取所有角色
        $role_list = (array)$this->db_role->get_role_list();
        $this->getView()->assign('role_list', $role_list);

    }

    /**
     * 添加角色页
     * @param void
     * @return void
    */
    public function addAction()
    {
        if(isset($_GET['id']) && (int)$_GET['id']){
              $role_detail = $this->db_role->get_role_detail($_GET['id']);
              $this->getView()->assign("role_detail", $role_detail);
        }
        $this->getView();
    }

    /**
     * 处理添加角色
     * @param void
     * @return void
    */
    public function dealAddAction()
    {
        $role_detail = array();

        //角色名
        $name = trim($this->getRequest()->getPost('name', 0));
        if(!$name){
            Yaf_Controller_Abstract::redirect("/admin/role/add/");
            exit;
        }
        $role_detail['role_name'] = $name;

        //角色状态
        $status = intval($this->getRequest()->getPost('status', 0));
        $role_detail['is_forbid'] = $status;

        //角色描述
        $description = trim($this->getRequest()->getPost('description', 0));
        if(!$description){
            Yaf_Controller_Abstract::redirect('/admin/role/add/');
            exit;
        }
        $role_detail['role_description'] = $description;

        //添加时间
        $role_detail['add_time'] = date('Y-m-d H:i:s');
        $role_id = $this->db_role->add_role($role_detail);
        if(!$role_id){
            Yaf_Controller_Abstract::redirect("/admin/role/add");
            exit;
        }else{
            echo "<script>alert('添加成功');location.href='/admin/role/index/'</script>";
        }
    }

    /**
     * 删除角色
     * @param void
     * @return void
    */
    public function deleteAction()
    {
        $role_id = (int)$_GET['id'];
        if(!$role_id){
            Yaf_Controller_Abstract::redirect("/admin/role/index/");
        }
        $result = $this->db_role->delete_role($role_id);
        if(!$result){
            Yaf_Controller_Abstract::redirect("/admin/role/index/");
        }else{
            echo "<script>alert('删除成功');location.href='/admin/role/index/'</script>";
        }
        exit;
    }

    /**
     * 编辑角色
     * @param void
     * @return void
    */
    public function editAction()
    {
        $role_detail = array();

        //角色ID
        $role_id = intval($this->getRequest()->getPost("rid", 0));
        if(!$role_id){
            Yaf_Controller_Abstract::redirect("/admin/role/index/");
            exit;
        }
        $role_detail['role_id'] = $role_id;

        //角色名
        $name = trim($this->getRequest()->getPost('name', 0));
        if(!$name){
            Yaf_Controller_Abstract::redirect("/admin/role/add?id=" . $role_id);
            exit;
        }
        $role_detail['role_name'] = $name;

        //角色状态
        $status = intval($this->getRequest()->getPost('status', 0));
        $role_detail['is_forbid'] = $status;

        //角色描述
        $description = trim($this->getRequest()->getPost('description', 0));
        if(!$description){
            Yaf_Controller_Abstract::redirect('/admin/role/add?id=' . $role_id);
            exit;
        }
        $role_detail['role_description'] = $description;

        //添加时间
        $role_detail['add_time'] = date('Y-m-d H:i:s');
        $result = $this->db_role->modify_role($role_detail);


        if(!$result){
            Yaf_Controller_Abstract::redirect("/admin/role/add?id=" . $role_id);
            exit;
        }else{
            echo "<script>alert('保存成功');location.href='/admin/role/index/'</script>";
        }
        exit;
    }
}