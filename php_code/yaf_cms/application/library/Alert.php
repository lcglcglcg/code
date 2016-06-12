<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of Alert
 * 编辑成功后的提示 
 * @author yangguofeng
 */
class Alert {

    //put your code here
    public static function get() {
        $notice = Yaf_Session::getInstance()->get("alert");
        Yaf_Session::getInstance()->del('alert');
        return $notice;
    }

    public static function success($msg) {
        $notice['success'] = $msg;
        Yaf_Session::getInstance()->set('alert', $notice);

    }

    public static function danger($msg) {
        $notice['danger'] = $msg;
        Yaf_Session::getInstance()->set('alert', $notice);
    }

    public static function info($msg) {
        $notice['info'] = $msg;
        Yaf_Session::getInstance()->set('alert', $notice);
    }

    public static function warning($msg) {
        $notice['warning'] = $msg;
        Yaf_Session::getInstance()->set('alert', $notice);
    }

}
