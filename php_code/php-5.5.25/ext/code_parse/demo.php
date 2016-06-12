<?php

$path="img/3117.jpg";

$p = new code_parse();
$code = $p->jpeg_path($path);
echo $code;

// $fp = fopen($path, 'rb');
// $buffer = fread($fp, filesize($path));
// $code = $p->jpeg_buffer($buffer);
// echo $code;

