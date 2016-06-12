<?php
/**
 * Created by PhpStorm.
 * User: kele
 * Date: 11/19/14
 * Time: 2:08 PM
 */
$memcache = new Memcache;  //initialised memcahe
$memcache->connect("127.0.0.1",11211);  // connect memcahe server
