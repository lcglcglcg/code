<?php if(!defined('APP_PATH')) exit('No direct script access allowed');
/**
权限类
@author lucas lucasphper@gmail.com
@access public
@package base
@version 1.0 2014/11/25
 */

Yaf_Loader::import(PATH_APP.'modules/Admin/controllers/Admin.php');
Yaf_Loader::import(PATH_APP.'modules/Admin/models/Role.php');
class PrivilegeController extends AdminController
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
        //获取所有权限
        $privilege_list = (array)$this->db_privilege->get_privilege_list();

        //权限列表格式化
        $privilege_format_list = array();
        $temp_list1 = $temp_list2 = array();
        $count = count($privilege_list) - 1;

        $menu_id_list = array();
        foreach($privilege_list as $key => $value){
            $menu_id_list[] = $value['menu_id'];
            $temp_list1[] = $value;
            if((isset($privilege_list[$key+1]['menu_id']) && $value['menu_id'] != $privilege_list[$key+1]['menu_id']) or ($count == $key)){
                $temp_list2['menu_id'] = $value['menu_id'];
                $temp_list2['data'] = $temp_list1;
                $temp_list1 = array();
                $privilege_format_list[] = $temp_list2;
                $temp_list2 = array();
            }
        }
        $this->getView()->assign('privilege_list', $privilege_format_list);

        //菜单map
        $menu_map_list = array();
        $menu_map_list_result = $this->db_menu->get_menu_detail($menu_id_list);
        foreach($menu_map_list_result as $key => $value){
            $menu_map_list[$value['menu_id']] = $value['menu_name'];
        }
        $this->getView()->assign('menu_map_list', $menu_map_list);


        //给角色添加权限
        if(isset($_GET['rid']) && (int)$_GET['rid']){

            //验证角色有效性
            $role_detail = $this->db_role->get_role_detail($_GET['rid']);
            if(!(isset($role_detail['role_id']) && (int)$role_detail['role_id'])){
                Yaf_Controller_Abstract::redirect("/admin/role/index/");
                exit;
            }

            $this->getView()->assign('role_id', $_GET['rid']);
            //获取当前角色的所有权限
            $current_role_privilege = (array)$this->db_privilege->get_role_privilege_list($_GET['rid']);
            $current_privilege_list = array();
            foreach($current_role_privilege as $key => $value){
                $current_privilege_list[] = $value['privilege_id'];
            }
            $this->getView()->assign("current_privilege_list", $current_privilege_list);
        }
    }

    /**
     * 给角色添加权限
     * @param void
     * @return void
    */
    public function addPrivilegeForRoleAction()
    {
        $role_id = intval($this->getRequest()->getPost("role_id", 0));
        if(!$role_id){
            Yaf_Controller_Abstract::redirect("/admin/role/index/");
        }

        $privilege_list = $this->getRequest()->getPost('privilege_item', 0);
        if(!(isset($privilege_list) && count($privilege_list))){
            Yaf_Controller_Abstract::redirect("/admin/privilege/index?rid=" . $role_id);
        }
        //先清空该角色所有权限
        $this->db_privilege->delete_privilege_for_role($role_id);
        $result = $this->db_privilege->add_privilege_for_role($role_id, $privilege_list);
        if((int)$result[0]){
            echo "<script>alert('保存成功');location.href='/admin/role/index/'</script>";
        }else{
            Yaf_Controller_Abstract::redirect("/admin/privilege/index?rid=" . $role_id);
        }
        exit;
    }

}