<?php if(!defined('APP_PATH')) exie('No direct script access allowed');?>

<?php
	/**
		用户类
		@author lucas lucasphper@gmail.com
		@access public 
		@package base
		@version 1.0 2014/11/19
	*/
	class UserController extends BaseController
	{
		/**
			登录页
			@param void
			@return void
		*/
		function loginAction()
		{
			$this->getView();
		}
		
		/**
			处理登录
			@param void
			@return void
		*/
		function dealLoginAction()
		{
			print_r($_POST);
			exit;
		}
		
		
		
	}
	