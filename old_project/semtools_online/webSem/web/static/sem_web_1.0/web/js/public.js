// Add By Wangzhe

// 控制导航下拉菜单-鼠标经过显示
var curr_page=0;
	function nav_show(i,t){
		if(t>0){
		curr_page=t;
		}
		document.getElementById('nav'+i).className="dh_hover";
		if(i!=1){
		document.getElementById('subnav'+i).style.display="block";
		var divheight=document.getElementById('subnav'+i).offsetHeight;
		document.getElementById('h_frame'+i).style.height=divheight;
		}
		}
// 控制导航下拉菜单-鼠标离开隐蔽
	function nav_hidden(j,t){
		if(t>0){
		curr_page=t;
		}
		if(curr_page!=j){
		document.getElementById('nav'+j).className="";
		}
		if(j!=1){
		document.getElementById('subnav'+j).style.display="none";
		}
		}


	function nav_hover(i,t){
		if(t>0){
		curr_page=t;
		}
		document.getElementById('nav1').className="";
		document.getElementById('nav'+i).className="dh_hover";
		}



// 控制ul的背景-鼠标经过显示
function ul_show(m){
	document.getElementById('ul_hover'+m).className="hover_bg";
}

// 控制ul的背景-鼠标离开隐蔽
function ul_hidden(n){
	document.getElementById('ul_hover'+n).className="";
	var uls=document.getElementById(n+''+1);
	if(uls!=null){
		if(uls.style.display=="block"){
			document.getElementById('ul_hover'+n).className="brown_bg";
		}
	}
}