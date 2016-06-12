window.ul_show=function(obj){
	obj.className='hover_bg';
};
window.ul_hidden=function(obj){
	obj.className='brown_bg';
};
var change_pwd=''
,	isEditLoginIng=false
;
window.editloginpass=function(){
	change_pwd=$G('#change_pwd');
	if(change_pwd.length<1)return;
	change_pwd.WinAlt({
		position:'fixed',
		isCenter:true,
		lock:true,
		lock_back:'#BBBBBB',
		lock_opa:30
	});
};
window.colse_editloginpass=function(){
	change_pwd.display().none();
};
window.sub_editloginpass=function(){
	var inp=$G('input',change_pwd[0])
	,	pass=inp.val()
	,	ckfn=function(msg){
			var v=this.value
			,	l=$G.len(v)
			;
			if($G.isNaN(v)){
				alert(msg);
				this.focus();
				return false;
			}else if(!$G.cklogin(v,0,100)){
				alert('请勿使用除字母、数字、下划线、英文句点外的其他字符');
				this.focus();
				return false;
			}else if(l<6 || l>16 ){
				alert('最小6个字符长度，最大16个字符长度');
				this.focus();
				return false;
			}else
				return true;
		}
	;
	if(!ckfn.call(inp[1],'旧密码不能为空'))
		return
	if(!ckfn.call(inp[2],'新密码不能为空'))
		return
	if(!ckfn.call(inp[3],'确认密码不能为空'))
		return
	var	newpass=(pass.toString()).replace(new RegExp(pass[2], 'ig'),'').replace(/,/ig,'')
	;
	if(newpass!=(pass[1].replace(new RegExp(pass[2], 'ig'),''))){
		pass[3].value='';
		alert('确认密码与新密码不一致');
		pass[3].focus();
		return;
	}
	if(isEditLoginIng)return;
	isEditLoginIng=true;
	$G.ajax({
		type:'post',
		cache:true,
		url:'/editlogin/'+new Date().getTime(),
		data:{
			pass:pass[1]
		,	newpass:pass[2]
		},
		dataType:'html',
		beforeSend:function(){},
		error:function (XMLHttpRequest, textStatus, errorThrown) {isEditLoginIng=false;},
		success:function(html){
			if(html && Number(html)<1)
				alert('旧密码不正确');
			else
				change_pwd.display().none();
			isEditLoginIng=false;
		}
	});
	
};
var msgViews={};
msgViews[-8]='账户已冻结! 不可创建报告! 请联系服务商!';
msgViews[-7]='账户已过期! 不可创建报告! 请联系服务商!';
msgViews[-6]='账户验证失败! 不可创建报告! 请联系服务商!';
msgViews[-5]='报告生成异常! 请重新创建报告!';
msgViews[-4]='账户已过期! 请联系服务商!';
msgViews[-3]='账户验证失败! 请联系服务商!';
msgViews[-2]='服务器未能成功生成数据';
msgViews[-1]='报告生成超时! 请重新创建报告!';
msgViews[0]='未验证';
msgViews[1]='请正确输入验证码';
msgViews[2]='用户名或密码不正确';
msgViews[3]='用户名或密码不正确';
msgViews[800]='系统错误';
msgViews[801]='错误的用户名、密码或者token请求过于频繁';
msgViews[802]='错误的密码请求过于频繁';
msgViews[8001]='SOAP版本不合法，请使用SOAP/1.1';
msgViews[8002]='Jsonmessage不合法';
msgViews[8101]='缺少用户名信息';
msgViews[8102]='用户名不合法';
msgViews[8103]='用户名不合法';
msgViews[8104]='没有权限操作该用户';
msgViews[8201]='缺少密码信息';
msgViews[8202]='密码不合法';
msgViews[8301]='被操作的用户不合法';
msgViews[8303]='没有权限操作该用户';
msgViews[8401]='缺少token信息';
msgViews[8402]='token不合法';
msgViews[8403]='token已删除';
msgViews[8404]='没有申请搜索推广API服务';
msgViews[8405]='没有申请网盟推广API服务';
msgViews[8406]='没有权限使用搜索推广API服务';
msgViews[8407]='没有权限使用网盟推广API服务';
msgViews[8408]='没有权限使用该token';
msgViews[8409]='没有权限操作该用户';
msgViews[8410]='没有权限使用该token';
msgViews[8411]='没有权限操作其他用户';
msgViews[8412]='没有权限使用该token';
msgViews[8501]='配额不足';
msgViews[8601]='未知服务器错误，稍后再试';
msgViews[8602]='未知服务器错误，稍后再试';
msgViews[8603]='请求头信息不合法';
msgViews[8604]='未知服务器错误，稍后再试';
msgViews[8605]='未知服务器错误，稍后再试';
msgViews[8606]='未知服务器错误，稍后再试';
msgViews[8607]='请求的SOAP信息不合法';