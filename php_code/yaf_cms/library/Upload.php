<?php
 
    class Upload
    {
        private $upload_name;                   //上传文件名
        private $upload_tmp_name;               //上传临时文件名
        public $upload_final_name;             //上传文件的最终文件名
        private $upload_target_dir;             //文件被上传到的目标目录
        private $upload_target_path;            //文件被上传到的最终路径
        private $upload_filetype ;              //上传文件类型
        private $allow_uploadedfile_type;       //允许的上传文件类型
        private $upload_file_size;              //上传文件的大小
        private $allow_uploaded_maxsize=5000000; //允许上传文件的最大值
        private $image_w=1900;                   //要显示图片的宽
        private $image_h=1280;                   //要显示图片的高
 
        function __construct($file,$upload_path)
        {
            $this->upload_name = $_FILES[$file]["name"]; //取得上传文件名
            $this->upload_filetype = $_FILES[$file]["type"];
            $this->upload_final_name = date("YmdHis").$this->upload_name;
            $this->upload_tmp_name = $_FILES[$file]["tmp_name"];
            $this->allow_uploadedfile_type = array("image/gif","image/jpeg");
            $this->upload_file_size = $_FILES[$file]["size"];
            $this->upload_target_dir= $upload_path;
        }
 
        function upload_file()
        {
            if(in_array($this->upload_filetype,$this->allow_uploadedfile_type))
            {
                if($this->upload_file_size < $this->allow_uploaded_maxsize)
                {
                    if(!is_dir($this->upload_target_dir))
                    {
                        mkdir($this->upload_target_dir);
                    }
                    $this->upload_target_path = $this->upload_target_dir."/".$this->upload_final_name;
                    if(move_uploaded_file($this->upload_tmp_name,$this->upload_target_path))
                        return $this->upload_final_name;
                }
                else
                {
                   return false;
                }
            }
            else
            {
                return false;
            }
        }
 
    }
 
?>