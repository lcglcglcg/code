<?php

// $p = new site_builder();
//
// $id = $p->create_template_id();
// $id = $p->create_keyword_id();
// $id = $p->create_content_id();
// $id = $p->create_link_group_id();
// $id = $p->create_link_external_id();
// 
// $p->add_template_dir($id, $dir_path);
// $p->add_keyword_file($id, $file_path);
// $p->add_content_file($id, $file_path);
// $p->add_link_group_file($id, $file_path);
// $p->add_link_external_file($id, $file_path);
// 
// $ret_code = $p->set_config($xml_buf);
// echo $ret_code; //this ret_code != 0 is err!
// $xml_buf = $p->get_config();

$p = new site_builder();

$template_id = $p->create_template_id();
$p->add_template_dir($template_id, '/make/site_builder_dir/template');

$id = $p->create_keyword_id();
$p->add_keyword_file($id, "/make/site_builder_dir/keyword.txt");
$p->add_keyword_file($id, "/make/site_builder_dir/keyword.txt");
$p->add_keyword_file($id, "/make/site_builder_dir/keyword.txt");
$p->add_keyword_file($id, "/make/site_builder_dir/keyword.txt");


