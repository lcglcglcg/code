$G(
function(){
	window.insetTime=function(o){
		eval("var j="+o+";");
		if(j.code>0){
			alert('选择的开始时间要小于等于结束时间');
			return;
		}
		inp[1].value=j.val;
		time.none();
	};
	window.insetFindDate=function(o){
		if($G.isNaN(o))return;
		eval("var j="+o+";");
		if(j.code>0){
			alert('选择的开始时间要小于等于结束时间');
			return;
		}
		getReportObj.dhtime.value=j.val;
	};
	
	var inp=$G('input')
	,	servicetime=''
	,	searchinp=$G('#search')
	,	echolist=$G('#echolist')
	,	create=$G('#create')
	,	timeinp=$G('.calendar')
	,	pagelist=$G('#pagelist')
	,	message=$G('#message')
	,	pagefn=function(page,count,pagesize,n){
			var pagecount=count%pagesize==0?count/pagesize:parseInt((Number(count)+Number((pagesize-Number(count%pagesize))))/pagesize)
			,	selectPage=function(){
					var s='';
					for(var i=1;i<=pagecount;i++){
						if(page!=i)
							s+='<option value="'+i+'">第'+i+'页</option>';
						else
							s+='<option selected="selected" value="'+i+'">第'+i+'页</option>';
					}
					return s;
				}()
			;
			if(pagecount>1)pagelist[0].style.display='';
			else pagelist[0].style.display='none';
			var html=' 显示 <s class="red">';
			html+=(pagesize*page-pagesize+1)+'-'+(pagesize*page-pagesize+n)+'</s> 条 共 '+count+' 条 '+pagecount+' 页 <a onclick="getlist(1)" href="javascript:;">首页</a> <a  onclick="getlist('+(page>1?(page-1):1)+')"  href="javascript:;">上页</a> <a onclick="getlist('+(page<pagecount?(page+1):pagecount)+')" href="javascript:;">下页</a> <a onclick="getlist('+pagecount+')" href="javascript:;">末页</a>';
			html+='&nbsp;&nbsp;<select onchange="pagesize(this.value);" name="page" class="padt">\
				<option '+(pagesize==10?' selected="selected" ':'')+' value="10">每页10条</option>\
				<option '+(pagesize==30?' selected="selected" ':'')+' value="30">每页30条</option>\
				<option '+(pagesize==50?' selected="selected" ':'')+' value="50">每页50条</option>\
				<option '+(pagesize==100?' selected="selected" ':'')+' value="100">每页100条</option>\
			  </select>';
			html+='&nbsp;<select onchange="getlist(this.value);"  name="each" class="padt">'+selectPage+'</select>';
			pagelist.html(html);
		}
	,	listtitlie=$G('li',$G('#listtitlie')[0])
	,	create_report=$G('#create_report')
	,	create_a=$G('a', create_report[0])
	,	create_dd=$G('#d1 #d2',create_report[0])
	,	getReportObj=function(){
			var create_inp=$G('input', create_report[0])
			,	create_select=$G('select', create_report[0])
			;
			create_select[0].onchange=function(){
				//create_dd.style({style:"display:none;"},'update');
				create_dd[0].style.display=create_dd[1].style.display='none';
				create_dd[(this.value==1?0:1)].style.display='';
			};
			return {
				bgName:create_inp[1]
			,	bgClose:create_inp[0]
			,	dhconuttype:create_select[1]
			,	dhselect:create_select[0]
			,	dhtype:create_select[2]
			,	dhcount:create_inp[2]
			,	dhimg:create_inp[3]
			,	dhtypeName:create_inp[4]
			,	dhtime:create_inp[5]
			};
		}()
	,	timeinpObj=timeinp.Obj(timeinp.parentNode)
	,	time=timeinp.time({wholemonth:true,two:true,fnname:'insetTime',x:(timeinpObj.x+timeinpObj.w-402),y:(timeinpObj.y+timeinpObj.h)})
	,	isSub=false
	,	listorder='createtime'
	,	listsort='DESC'
	,	listsize=10
	,	listRow='<ul id="row_%id%" onMouseOver="ul_show(this)" onMouseOut="ul_hidden(this)">'
	,	listRowLi='<li class="w430" title="%name%">%name%</li>\
            <li class="w200 f_center">%createtime%</li>\
            <li class="w200 f_center">%mktime%</li>\
            <li class="w140 f_center">%download%<a href="javascript:delRow(%id%);">删除</a></li>'
	,	rowIng=new Array()
	,	rowLi=function(s,r){
			s=s.replace(/%id%/ig,r.id);
			s=s.replace(/%name%/ig,r.name);
			s=s.replace(/%createtime%/ig,r.createtime);
			var mktime=$G.isNaN(r.mktime)?'报告正在生成，请稍后下载':r.mktime;
			if(r.mktype>0){
				if(r.mktype==1)
					s=s.replace(/%download%/ig,'<a href="/download/'+r.downloadflie+'" target="_blank">下载</a>');
				else{
					mktime='<span class="red">报告生成失败</span>';
					s=s.replace(/%download%/ig,'<a href="javascript:alertMsg(\''+r.mktype+'\');">查看</a>');
				}
			}else{
				try{
					var t1=(new Date(Date.parse(r.createtime.replace(/-/g,   "/")))).getTime()
					,	t2=(new Date(Date.parse(servicetime.replace(/-/g,   "/")))).getTime()
					;
					if((t2-t1)>60000000){
						mktime='--';
						s=s.replace(/%download%/ig,'<a href="javascript:alertMsg(-1);">查看</a>');
					}else{
						rowIng.push(r.id);
						s=s.replace(/%download%/ig,'<span>下载</span>');
					}
				} catch (e) {
				}
			}
			return s=s.replace(/%mktime%/ig,mktime);
		}
	,	hearList=function(){
			if(rowIng.length<1) return;
			var ids=rowIng.join(',');
			$G.ajax({
				type:'get',
				cache:true,
				url:'/main/reportlist/'+new Date().getTime(),
				data:{
					ids:ids
				},
				dataType:'json',
				beforeSend:function(){},
				error:function (XMLHttpRequest, textStatus, errorThrown) {},
				success:function(json){
					rowIng=new Array();
					var list=json.list;
					servicetime=json.time;
					for(var i=0,n=list.length;i<n;i++){
						var r=list[i];
						//r.name=r.name+(new Date().getTime());
						$G('#row_'+r.id).html(rowLi(listRowLi,r));
					}
					$G.setTimeout(hearList,5);
				}
			});
		}
	,	getlist=function(o){
			if(o.isPage) o.date=listDate;
			listDate=o.date||{
					keysearch:o.keyword||''
				,	time1:o.time1||''
				,	time2:o.time2||''
			};
			listDate['order']=listorder;
			listDate['sorts']=listsort;
			listDate['page']=o.page||1;
			listDate['pagesize']=listsize;
			$G.ajax({
				type:'get',
				cache:true,
				url:'/main/reportlist/'+new Date().getTime(),
				data:listDate,
				dataType:'json',
				beforeSend:function(){},
				error:function (XMLHttpRequest, textStatus, errorThrown) {},
				success:function(json){
					rowIng=new Array();
					if(json){
						var list=json.list
						,	rownum=json.max
						,	html=''
						,	fn=function(r){
								return rowLi((listRow+listRowLi),r)+'</ul>';
							}
						;
						servicetime=json.time;
						var row=0;
						for(var i=0,n=list.length;i<n;i++){
							var str=fn(list[i]);
							html+=str;
							row++;
						}
						if($G.isNaN(html)){
							echolist.html('没有报告数据');
							pagelist[0].style.display='none';
						}else{
							echolist.html(html);
							pagefn(o.page,rownum,listsize,row);
							$G.setTimeout(hearList,5);
						}
					}else{
						
					}
				}
			});
		}
	,	mkxls=function(o){
			o=o||{};
			//if(o.path)o.path=o.path.replace(/\//ig,'-');
			$G.ajax({
				type:'post',
				cache:true,
				url:'/main/getmkxls',
				data:o,
				dataType:'TEXT',
				beforeSend:function(){},
				error:function (XMLHttpRequest, textStatus, errorThrown) {isSub=false;},
				success:function(code){
					
				}
			});
		}
	,	sub=function(file, serverData,isdh){
			var data={}
			,	urls='/main/createreport'
			;
			if($G.isNaN(serverData)){
				if(!$G.isNaN(getReportObj.dhtime.value)){
					var time=getReportObj.dhtime.value.split('/')
					,	time1=time[0]
					,	time2=time[1]
					,	urls='/main/recreate'
					;
				}
				data={
					type:getReportObj.dhconuttype.value
				,	name:getReportObj.bgName.value
				,	ftime_1:time1
				,	ftime_2:time2
				};
			}else{
				var json=$G.parseJSON(serverData);
				if(json.error){
					alert(json.error);
					isSub=false;
					return false;
				}
				data={
						type:getReportObj.dhconuttype.value
					,	path:json.path
					,	yycolumn:getReportObj.dhtype.value
					,	yy:getReportObj.dhtypeName.value
					,	name:getReportObj.bgName.value
				};
			}
			$G.ajax({
				type:'post',
				cache:true,
				url:urls,
				data:data,
				dataType:'json',
				beforeSend:function(){},
				error:function (XMLHttpRequest, textStatus, errorThrown) {isSub=false;},
				success:function(obj){
					isSub=false;
					if(!$G.isNaN(obj.id)){
						data['id']=obj.id;
						mkxls(data);
						window.document.title='定制报告';
						if(!isdh)return;
						progress.display().none();
						create_report.display().none();
						//$G('select', create_report[0]).setselvalue(1);
						//create_dd[0].style.display='';
						//create_dd[1].style.display='none';
						
						getReportObj.dhcount.value=getReportObj.bgName.value='';//getReportObj.dhtypeName.value=
						getlist({page:1});
					}else{
						alert('创建报告失败，请重新创建');
					}
				}
			});
		}
	,	createSub=function(){
			if(isSub) return;
			try {
				if($G.isNaN(getReportObj.bgName.value)){
					alert('请输入报告名称');
					getReportObj.bgName.focus();
					return;
				}
				if(getReportObj.dhselect.value==2){
					if($G.isNaN(getReportObj.dhtime.value)){
						alert('请选择时间范围');
						return;
					}
					getReportObj.dhselect.disabled=true;
					sub("","",true);
					return;
				}
				if($G.isNaN(getReportObj.dhcount.value)){
					alert('选择对话记录文件');
					return;
				}
				if(getReportObj.dhtype.value>1 && $G.isNaN(getReportObj.dhtypeName.value)){
					alert('请填写对话预约标识');
					getReportObj.dhtypeName.focus();
					return;
				}
				if(getReportObj.dhtype.value==1 && !$G.isNaN(getReportObj.dhtypeName.value)){
					alert('请选择预约标识列');
					return;
				}
				if($G.len(getReportObj.dhtypeName.value)>50){
					alert('您输入预约标识不超过50个字符（等同25个汉字）长度！');
					getReportObj.dhtypeName.focus();
					return;
				}
				if($G.len(getReportObj.bgName.value)>50){
					alert('您输入报告名称不超过50个字符（等同25个汉字）长度！');
					getReportObj.bgName.focus();
					return;
				}
				isSub=true;
				getReportObj.dhselect.disabled=getReportObj.dhtypeName.disabled=getReportObj.bgName.disabled=getReportObj.dhtype.disabled=true;
				create_report.display().none();
				progress.WinAlt({
					position:'fixed',
					isCenter:true,
					lock:true,
					lock_back:'#BBBBBB',
					lock_opa:30
				});
				upflie.startUpload();
			} catch (e) {
				alert(e);
			}
		}
	,	selectData=function(){
			var html=$G.mkflash(function(o){
				o=o||{};
				o.w=402;
				o.h=180;
				o.data={
					isTwos	:true
				,	wholemonth:true
				,	urls		:'/calendar/zoosnet_calendar|/calendar/baidu_calendar'
				,	fn		:'insetFindDate'
				};
				o.src=o.path+'time.swf';
				return o.fn(o);
			});
			$G('#show_date').html(html);
		}()
	;
	window.alertMsg=function(html){
		if (html == 2)
			html=msgViews[-5];
		else if (html == 3)
			html=msgViews[-4];
		else if (html == 4)
			html=msgViews[-3];
		else if(html<0)
			html=msgViews[html];
		else
			html=msgViews[-2];
		$G('#messageText').html(html);
		message.WinAlt({
			position:'fixed',
			isCenter:true,
			lock:true,
			lock_back:'#BBBBBB',
			lock_opa:30
		});		
	};
	$G('#idBtn_close',message[0]).click(function(){
		message.display().none();
	});
	window.getlist=function(page){
		getlist({page:page,isPage:true});
	};
	window.pagesize=function(c){
		listsize=c;
		getlist({page:1,isPage:true});
	};
	window.listDate={};
	var titlieId=$G.uuidFast().replace(/-/ig,'');
	listtitlie.Each(function(i){
		if(i<3)
			this.id=titlieId+'_'+i;
	});
	
	window.titleMap={};
	titleMap[titlieId+'_0']=['name','DESC'];
	titleMap[titlieId+'_1']=['createtime','DESC'];
	titleMap[titlieId+'_2']=['mktime','DESC'];
	listtitlie.click(function(i){
		var typename=this.id
		;
		if($G.isNaN(typename))return;
		listorder=titleMap[typename][0];
		titleMap[typename][1]=listsort=titleMap[typename][1]=='DESC'?'ASC':'DESC';
		getlist({page:listDate['page'],isPage:true});
	});
	searchinp.click(function(){
		var time=inp[1].value
		,	time1=''
		,	time2=''
		,	keyword=inp[0].value
		;
		if(!$G.isNaN(time)){
			time=time.split('/');
			time1=time[0];
			time2=time[1];
		}
		if(!$G.isNaN(keyword) && $G.len(keyword)>50){
			alert('请正确输入查询条件！不超过50个字符（等同25个汉字）长度！');
			return;
		}
		getlist({page:1,keyword:keyword,time1:time1,time2:time2});
	});
	pagelist[0].style.display='none';
	create_a[1].name=getReportObj.bgClose.name="create";
	var create_close=$G('.create');
	getReportObj.dhimg.id="upflie";
	getReportObj.dhcount.disabled=true;
	getReportObj.dhtime.disabled=true;
	var inpdisabled=function(){
		getReportObj.dhselect.disabled=getReportObj.bgName.disabled=getReportObj.dhtypeName.disabled=getReportObj.dhtype.disabled=false;
	};
	var	progress=$G('#progress')
	,	progress_name=$G('#progress_name')
	,	progress_colse=$G('#progress_colse')
	,	progress_ing=$G('#progress_ing')
	,	progress_size=$G('#progress_size')
	,	progress_sizeing=$G('#progress_sizeing')
	,	progress_kb=$G('#progress_kb')
	,	progress_time=$G('#progress_time')
	,	progress_bar=$G('#progress_bar')
	;
	var upflie=$G.upflie({
		success:function(file, serverData){
			var stats=upflie.getStats()
			,	isNext=stats.files_queued<1?true:false
			;
			if(!isNext){
				if(!window.postName) window.postName=getReportObj.bgName.value;
				getReportObj.bgName.value=window.postName+stats.successful_uploads;
			}else if(window.postName) getReportObj.bgName.value=window.postName;
			sub(file,serverData,isNext);
		}
	,	upload_start_handler:inpdisabled
	,	queued_handler:inpdisabled
	,	file_queue_error_handler:inpdisabled
	,	upload_progress_handlerIni:function(file){
			progress_name[0].innerHTML=file.name;
			progress_bar[0].style.width=progress_ing[0].innerHTML=file.percent+'%';
			progress_size.html(file.filesize+'MB');
			progress_sizeing.html(file.upsize+'MB');
			progress_kb.html(file.speeding+'KB/秒');
			progress_time.html(file.overtime);
		}
	,	queued_handler:function(file){
			try {
				getReportObj.dhcount.value = file.name;
				return true
			} catch (e) {
			}
		}
	,	upload_complete_handler:function(file, serverData){
			//alert(file.name);
		}
	});
	progress_colse.click(function(){
			if(confirm('您确定要停止上传数据吗')){
				upflie.cancelUpload();
				getReportObj.dhcount.value='';
				progress.display().none();
				isSub=false;
			}
	});
	window.delRow=function(id){
		if(confirm('您确定删除此条报告吗')){
			$G.ajax({
				type:'get',
				cache:true,
				url:'/main/reportdel/'+id,
				dataType:'json',
				beforeSend:function(){},
				error:function (XMLHttpRequest, textStatus, errorThrown) {},
				success:function(json){
					getlist({page:1});
				}
			});
		}
	};
	timeinp.click(function(){
			time.show();
	});
	create.click(function(){
		if(!$G.isNaN(login_code) && Number(login_code)<0){
			login_code=Number(login_code);
			var k=-6;
			if(login_code==(-2)) k=-7;
			if(login_code==(-3)) k=-8;
			if(login_code==(-4)) k=8605;
			window.alertMsg(k);
			return;
		}
		getReportObj.bgName.value=$G.formatdate(new Date(),'对话报告_YYYY年MM月dd日hh点mm分ss秒');
		//getReportObj.dhtime.value=$G.formatdate(new Date((new Date()).getTime()-7*1000*60*60*24),'YYYY-MM-dd')+'/'+$G.formatdate(new Date(),'YYYY-MM-dd');
		window.postName='';
		inpdisabled();
		create_report.WinAlt({
			position:'fixed',
			isCenter:true,
			lock:true,
			lock_back:'#BBBBBB',
			lock_opa:30
		});
	});
	create_close.click(function(){
		create_report.display().none();
	});
	create_a[0].onclick=createSub;
	
	getlist({page:1});
	window.extnew=function(){
		create_report.display().none();
	};
document.onkeydown=function(event){
	var e = event || window.event || arguments.callee.caller.arguments[0];            
	if(e && e.keyCode==27){
		extnew();
	}
	if(e && e.keyCode==13){
		if(create_report[0].style.display!='none')createSub();
	}
};

}
);