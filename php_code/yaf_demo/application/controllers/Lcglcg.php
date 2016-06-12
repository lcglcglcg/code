
<?php

class LcglcgController extends Yaf_Controller_Abstract {

    public function indexAction() {

	echo "indexAction\r\n";
	$this->getView()->assign("content", "Hello World");
    }

    public function lcglcgAction() {

	echo "lcglcg\r\n";
	$this->getView()->assign("content", "lcglcg");
    }
}

?>



