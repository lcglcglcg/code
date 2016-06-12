$G(
function(){
	var listLi='<ul onMouseOver="ul_show(this)" onMouseOut="ul_hidden(this)">\
		<li class="w220 f_center" title="%token%">%token%</li>\
		<li class="w200 f_center" title="%account%">%account%</li>\
		<li class="w200 f_center" title="%password%">%password%</li>\
		<li class="w110 f_center" title="%date%">%date%</li>\
		<li class="w150 f_center %classStart%" title="%status%">%status%</li>\
		<li class="w100 f_center"><a onClick="editaccount(%id%,\'%account%\',\'%password%\',\'%token%\');">修改信息</a></li>\
      </ul>'
	,	listTitlieLi=$G('li',$G('#listTitlie')[0])
	,	echolist=$G('#echolist')
	,	listorder='id'
	,	listsort='DESC'
	,	rowLi=function(s,r){
			s=s.replace(/%id%/ig,r.id);
			s=s.replace(/%token%/ig,$G.isNaN(r.token)?'--':r.token);
			s=s.replace(/%account%/ig,r.account);
			s=s.replace(/%password%/ig,$G.isNaN(r.password)?'--':r.password);
			s=s.replace(/%date%/ig,r.date);
			var start=r.status==1?1:r.code;
			if(start==1){
				s=s.replace(/%classStart%/ig,'green');
				start='有效';
			}else{
				s=s.replace(/%classStart%/ig,'red');
				try{
					start=msgViews[start];
				} catch( e ) {
					start='未知错误';
				}
			}
			s=s.replace(/%status%/ig,start);
			return s;
		}
	,	getlist=function(o){
			o=o||{};
			var listDate={};
			listDate['order']=listorder;
			listDate['sorts']=listsort;
			$G.ajax({
				type:'get',
				cache:true,
				url:'/user/userlist/'+new Date().getTime(),
				data:listDate,
				dataType:'json',
				beforeSend:function(){},
				error:function (XMLHttpRequest, textStatus, errorThrown) {},
				success:function(json){
					if(json){
						var list=json.list
						,	html=''
						,	fn=function(r){
								return rowLi(listLi,r);
							}
						;
						for(var i=0,n=list.length;i<n;i++){
							var str=fn(list[i]);
							html+=str;
						}
						if($G.isNaN(html)){
							echolist.html('没有报告数据');
						}else{
							echolist.html(html);
						}
					}else{
						echolist.html('没有报告数据');
					}
				}
			});
		}
	;
	var titlieId=$G.uuidFast().replace(/-/ig,'');
	listTitlieLi.Each(function(i){
		if(i<5)
			this.id=titlieId+'_'+i;
	});
	window.titleMap={};
	titleMap[titlieId+'_0']=['token','DESC'];
	titleMap[titlieId+'_1']=['account','DESC'];
	titleMap[titlieId+'_2']=['password','DESC'];
	titleMap[titlieId+'_3']=['date','DESC'];
	titleMap[titlieId+'_4']=['code','DESC'];
	listTitlieLi.click(function(){
		var typename=this.id
		;
		if($G.isNaN(typename))return;
		listorder=titleMap[typename][0];
		titleMap[typename][1]=listsort=titleMap[typename][1]=='DESC'?'ASC':'DESC';
		getlist();
	});
	getlist();
	
	var account_alert=$G('#account_alert')
	,	close_bnt=$G('.close_bnt',account_alert[0])
	,	close_inp=$G('input',account_alert[0])
	,	editAccount=$G('#editAccount')
	,	editId=0
	,	ifSubIng=false
	,	editName=''
	,	editPass=''
	,	editToken=''
	,	subfn=function(o,url){
			o=o||{};
			$G.ajax({
				type:'post',
				cache:true,
				url:(url||'/user/ckaccount/')+new Date().getTime(),
				data:o,
				dataType:'html',
				beforeSend:function(){},
				error:function (XMLHttpRequest, textStatus, errorThrown) {ifSubIng=false;},
				success:function(html){
					if(url=='/user/editaccount/'){
						account_alert.display().none();
						getlist();
						if(html && Number(html)<1)$G('#menu_errMsg')[0].style.display='none';
						ifSubIng=false;
						return;
					}
					if(html && Number(html)==0){
						subfn(o,'/user/editaccount/');
					}else{
						var msg='';
						try{
							msg=msgViews[html||8605];
						} catch( e ) {
							msg='未知错误';
						}
						alert(msg);
						ifSubIng=false;
					}
				}
			});
		}
	;
	close_inp.click(function(){
		if(this.value=='不设置默认不修改') this.value='';
	});
	window.editSub=function(){
		if(ifSubIng)return;
		ifSubIng=true;
		var token=close_inp[1].value.replace(/不设置默认不修改/ig,'')
		,	pass=close_inp[2].value.replace(/不设置默认不修改/ig,'')
		;
		var o={
			token:token?token:editToken
		,	password:pass?pass:editPass
		,	id:editId
		,	account:editName
		};
		subfn(o);
	};
	window.editaccount=function(id,name,pass,token){
		editId=id;
		editName=name;
		editPass=pass;
		editToken=token;
		editAccount.html(name);
		close_inp.val('不设置默认不修改');
		account_alert.WinAlt({
			position:'fixed',
			isCenter:true,
			lock:true,
			lock_back:'#BBBBBB',
			lock_opa:30
		});
	};
	close_bnt.click(function(){
		account_alert.display().none();
	});
	document.onkeydown=function(event){
		var e = event || window.event || arguments.callee.caller.arguments[0];
		if(e && e.keyCode==13){
			if(account_alert[0].style.display!='none')editSub();
		}
	};
});
