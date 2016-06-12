// JavaScript Document
	function clickTxt(i,linkid,contid,t,current){
			for(n=1;n<=t;n++){
				if(i==n){
			document.getElementById(linkid+n).className=current;
			document.getElementById(contid+n).style.display="block";
					}else{
			document.getElementById(linkid+n).className="";
			document.getElementById(contid+n).style.display="none";
						}
				}
		}

	function queryCreative(idClick1,idClick2,idHidden,idView){
		document.getElementById(idClick1).style.display="none";
		document.getElementById(idClick2).style.display="block";
		document.getElementById(idHidden).style.display="none";
		document.getElementById(idView).style.display="block";
	}


	// 切换原型显示效果
	function viewButton(o){
		if(o.innerHTML == '上传'){
			o.innerHTML = "修改";
			document.getElementById('adPicId').src='/static/ad_c_1.0/web/images/view_photo1.gif';
		}
		else{
			o.innerHTML = "上传";
			document.getElementById('adPicId').src='/static/ad_c_1.0/web/images/no_photo.gif';
		}
	}

