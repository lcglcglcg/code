$G(
function(){
	var ul=$G('li:name:msg')
	,	inp=$G('input img')
	,	fn={}
	,	msg=function(k,msg){
			if(msg){
				ul[k].style.display='';
				ul[k].innerHTML='<font color="red">'+msg+'</font>';
			}else
				ul[k].style.display='none';
		}
	;
	window.subCk=function(){
		fn[3]();
	};
	fn[0]=function(){
		var v=this.value.replace(/[^\x00-\xff]/g, "11")
		,	l=$G.len(v)
		,	istrue=false
		;
		if($G.isNaN(v)){
			msg(0,'请输入用户名');
			this.focus();
		}else if(!$G.cklogin(v,0,100)){
			msg(0,'请勿使用除字母(大小写等价)、数字、下划线、中文外的其他字符');
			this.focus();
		}else if(l<4 || l>16 ){
			msg(0,'最小4个字符(等同2个汉字)长度，最大16个字符(等同8个汉字)长度');
			this.focus();
		}else{
			msg(0);
			istrue=true;
		}
		this.onkeydown=function(event){
			var e = event || window.event || arguments.callee.caller.arguments[0];            
			if(e && e.keyCode==13){
				e.keyCode=27;
			}
		};
		return istrue;
	};
	fn[1]=function(){
		var v=this.value
		,	l=$G.len(v)
		,	istrue=false
		;
		if($G.isNaN(v)){
			msg(1,'请输入密码');
			this.focus();
		}else if(!$G.cklogin(v,0,100)){
			msg(1,'请勿使用除字母、数字、下划线、英文句点外的其他字符');
			this.focus();
		}else if(l<6 || l>16 ){
			msg(1,'最小6个字符长度，最大16个字符长度');
			this.focus();
		}else{
			msg(1);
			istrue=true;
		}
		return istrue;
	};
	fn[2]=function(){
		var v=this.value
		,	l=$G.len(v)
		,	istrue=false
		;
		if(l==4){
			msg(2);
			istrue=true;
		}else{
			msg(2,'请正确输入验证码');
			this.focus();
		}
		return istrue;
	};
	fn[3]=function(){
		var isTrue=true
		,	obj=this
		;
		this.onclick=null;
		inp.Each(function(i){
			if(i<3 && isTrue)isTrue=fn[i].call(this);
		});
		if(isTrue){
			$G.ajax({
				type:'post',
				cache:true,
				url:'/login/ck/',
				data:{
					username:inp[0].value
				,	password:inp[1].value
				,	ckcode  :inp[2].value.toUpperCase()
				},
				dataType:'json',
				beforeSend:function(){},
				error:function (XMLHttpRequest, textStatus, errorThrown) {obj.onclick=fn[3];},
				success:function(json){
					if(json.err>0){
						inp[4].src=inp[4].src.replace(/\/\w+$/ ,'/')+new Date().getTime();
						alert(msgViews[json.err]);
						if(json.err==1)inp[2].focus();
						else inp[0].focus();
					}else{
						document.location='/main/';
					}
					obj.onclick=fn[3];
				}
			});	
		}else{
			obj.onclick=fn[3];
		}
		return false;
	};
	inp.Each(function(i){
		if(i==1){
			//$G(this).setAttribute('Type','PassWord');
		}else if(i==4){
			this.alt=this.title='点击刷新验证码';
			this.style.cursor='pointer';
			this.onclick=function(){
				this.src=this.src.replace(/\/\w+$/ ,'/')+new Date().getTime();
			};
		}else if(i==3){
			this.style.cursor='pointer';
			this.onclick=fn[i];
			var G_obj=$G(this);
			this.onmousedown=function(){
				G_obj.style({'class':'inputactive'});
			};
			this.onmouseup=function(){
				G_obj.style({'class':''});
			};
		}
		if(i<2) this.onblur=this.onchange=fn[i];
		else if(i<3)
			this.onchange=fn[i];
	});

document.onkeydown=function(event){
	var e = event || window.event || arguments.callee.caller.arguments[0];            
	if(e && e.keyCode==13){
		subCk();
	}
}; 

}
);