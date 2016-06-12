<?php

$p = new site_builder();
$p->ngx_reload();

// php.ini
// [site_builder]
// extension=site_builder.so

// Examples of this class
// add template need nginx -s reload
// $p = new site_builder();

// Get the ID for binding media form nginx.conf
// $p->create_template_id();
// $p->create_affiliated_file_id();
// $p->create_keyword_id();
// $p->create_content_id();
// $p->create_link_group_id();
// $p->create_link_external_id(external_id, '163', 'www.163.com');
// $p->create_link_external_id(external_id, 'sina', 'www.sina.com');

// create_id and save this ID to mysql associatemedia
// template_name=index,list,content
// $p->add_template(template_id, template_name, template_buffer);

// css,js,img... this buffer is affiliated file
// $p->add_affiliated_file(affiliated_file_id, create_affiliated_file_name, file_buffer);
// $p->add_affiliated_file(affiliated_file_id, 'css/xxx.css', file_buffer);

// for create rand title, link ...
// $p->add_keyword(keyword_id, keyword_buffer);

// rand create 
// $p->add_content(content_id, 'C1');
// $p->add_content(content_id, 'C2');

// rand doamin group 
// $p->add_link_group(group_id, 'aaa.com');
// $p->add_link_group(group_id, 'ccc.com');

// rand link custom link
// $p->add_link_external();

// No realization
// $p->add_template_directory(template_id, affiliated_file, 'dir_path');

// $p->add_keyword_file(keyword_id, 'file_path')
// $p->add_content_file(group_id, file_path)
// $p->add_link_group_file(group_id, 'file_path')
// $p->add_link_external_file(link_external_id, 'file_path')

//demo 
// $affiliated_file = get is mysql
// $p = new site_builder()
// $filename = "/xxx/temp.css";
// $handle = fopen($filename, "rb");
// $buffer = fread($handle, filesize ($filename));
// $p->add_affiliated_file($affiliated_file_id, $buffer);
// fclose($handle);

// $conf_file = $p->get_conf_file()	// this is XML nginx.conf

// $ret = $p->conf_flush($new_conf); 		// this is XML nginx.conf 
if ($ret == 0)
    else 
    ...




