<?php if (!defined('APP_PATH')) exit('No direct script access allowed');

/**
菜单类
@author lucas lucasphper@gmail.com
@package admin
@access public
@version 1.0 2014/11/25
 */
include_once(PATH_APP.'modules/Admin/models/Admin.php');
class MenuModel extends AdminModel
{
    function __construct()
    {
        parent::__construct();
    }

    /**
    获取后台菜单
    @param void
    @return array
     */
    function get_menu_list()
    {   $where['is_show'] = 0;
        $where['ORDER'] = 'menu_order DESC';
        return $this->db->select('menus', ['menu_id', 'menu_name', 'parent_id', 'menu_description', 'menu_order', 'is_show', 'url'], $where);
    }

    /**
     * 菜单详情
     * @param int $menu_id 菜单id
     * @return array
    */
    public function get_menu_detail($menu_id)
    {
        return $this->db->select('menus', ['menu_id', 'menu_name'], ['menu_id' => $menu_id]);
    }
}