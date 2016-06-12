/*
* Name		: Glacier 1.0
* Author	: wangzheng
* QQ		: 113666386
* Date		: 201302011339
* desc		: JS公用类
*/
(function(){
	/*
	* $G类
	* selector参数 selector=function时，为页面初始化后调用此方法
	*			  selector=String时，
					1、#为查找ID
					2、.为查找名称
					3、li:open为查找DOM（LI）对象class名称为open的对象
					4、li:gjson:names为查找DOM（LI）对象gjson属性的值为names的对象
					5、LI为直接输入对象类型查找
	* dom,当selector为方法时，dom如果不为空，则为页面DOM加载完毕后运行，如果为空，则为页面初始化后运行
	*	  当selector为字符时，dom为被查找对象的范围，如果为空，范围则为全部
	*/
	var Glacier=(!window.$G || window.$G.Version!='1.0')?function(selector,dom){
		if(window==this) return new Glacier(selector,dom);
		if (Glacier.isFun(selector)){
			if(!dom)
				this.Loaded(selector);
			else
				this.LoadedDom(selector);
			return new Glacier();
		}
		if(!Glacier.isObj(dom)) dom=window.document;
		if(selector && (typeof selector=="object") && selector.nodeType==1){
			this[0]=selector;
			this.length=1;
			return this;
		}
		var listDoms=[],domsTag=[],doms,name=new Array();
		if(typeof selector=="string"){
			var str = (" "+selector+" ".replace(/\s{2}/," ")).match(Glacier.fn.Exp.GlacierExpr);
			str=str||[];
			for(var i=0,n=str.length;i<n;i++){
				var s=str[i];
				if(s.indexOf(".")>=0){
					name.push([s.replace(/^\s*\.|\s/ig,"")]);
				}else if(s.indexOf("#")>=0){
					var obj=window.document.getElementById(s.replace(/^\s*#|\s/ig,""));
					obj && listDoms.push(obj);
				}else{
					var re=/[a-zA-Z:]/g,tag=s.replace(/\s/ig,"");
					if(re.test(tag)){
					}else
						continue;
					re.lastIndex=0;//lastIndex属性，返回匹配内容的最后一个索引位置，也就是下一个匹配的开始位置，注意字符串索引从0开始
					
					var tagarray=tag.split(":")
					,	findStr=tagarray[0]
					,	findTag=''
					,	findVal=''
					;
					if(tagarray.length>2){
						findTag=(tagarray[1]+'').toLowerCase();
						findVal=tagarray[2];
					}else if(tagarray.length>1){
						findTag='class';
						findVal=(tagarray[1]+'').toLowerCase();
					}
					doms=dom.getElementsByTagName(findStr);
					for(var iobj=0,div_n=doms.length;iobj<div_n;iobj++){
						var obj=doms[iobj];
						if(!Glacier.isNaN(findTag)){
							var tmpTag=(obj.getAttribute(findTag)+'').toLowerCase();
							if(Glacier.isNaN(tmpTag)) continue;
							if(tmpTag==findVal){
								obj && listDoms.push(obj);
							}
						}else{
							obj && listDoms.push(obj);
						}
						
					}
				}				
			}
		}
		if(name.length>0){
			var isTag=(Glacier.isIE || dom!=window.document);
			try{
				if(isTag)doms=dom.getElementsByTagName("*");
			}catch(e){
				//alert(dom+'|'+selector+'='+e);
				return;
			}
			
			for(var i=0,countName=name.length;i<countName;i++){
				if(!Glacier.isIE && dom==window.document) doms=dom.getElementsByName(name[i]);
				var tag=(name[i]+'').toLowerCase();
				for(var iobj=0,n=doms.length;iobj<n;iobj++){
					var obj=doms[iobj];
					if(isTag && obj){
						try{
							if(!obj.getAttribute('name')) continue;
							if((obj.getAttribute('name').toLowerCase())!=tag){
								domsTag.push(obj);
								continue;
							}
						}catch(e){continue};
					}
					obj && listDoms.push(obj);
				}
				doms=domsTag;
				domsTag=[];
			}
		}
		this.InitObj(listDoms);
	}:"",
	Version 				=			'1.0',
	toString				=			Object.prototype.toString,
	Timeout					=			window.setTimeout,
	Interval				=			window.setInterval,
	hasOwn					=			Object.prototype.hasOwnProperty,
	class2type				=			{};
	if(!Glacier)return;
	Glacier.fn=Glacier.prototype;
	/***************************************************以上 为定义变量**********************************************************/
	/*
	* 公用正则
	*/
	Glacier.fn.Exp={
		GlacierExpr:/[\.|#|\w-]+[:#\.\w-]*\s+/g,
		rlocalProtocol:'',
		CHARS : '0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz'.split('')
	};
	/*
	* 快捷遍历
	*/
	Glacier.fn.Each=function(fun,obj){
		if(Glacier.fn.isNaN(obj)) obj=this;
		if(!Glacier.fn.isArray(obj) && !Glacier.fn.isNumber(obj.length)){
			var n=0;
			for(var i in obj){
				if(!obj.hasOwnProperty(i))continue;
				fun.call(obj[i],n,i,obj);
				n++;
			}
			return;
		}
		for(var i=0,l=obj.length; i<l; i++)
			fun.call(obj[i],i,obj);
	};
	/*
	* 元素检索后对象整理
	*/
	Glacier.fn.InitObj=function(doms){
		var arr=[];
		if(doms && doms[0]!="null"){
			for(var i=0,n=doms.length;i<n;i++){
				var obj=doms[i];
				if(obj && (typeof obj=="object") && obj.nodeType==1)
					arr[i]=obj;
			}
		}
		return this.SetArray(arr);
	};
	/*
	* 将检索后的结果，添加到G中
	*/
	Glacier.fn.SetArray=function(arr){
		this.length=0;
		[].push.apply(this,arr);
		return this;
	};
	/*
	* 获取对象中的第一个元素
	*/
	Glacier.fn.getfirst=function(){
		return this[0];
	};
	/*
	* 获取对象中的最后一个元素
	*/
	Glacier.fn.getlast=function(){
		return this[this.length-1];
	};
	/*
	* 获取对像的ID
	*/
	Glacier.fn.id=function(){
		var ids=new Array();
		this.Each(function(){
			ids.push(this.id ? this.id:'');
		});
		return ids.length==1?ids[0]:ids;
	};
	Glacier.fn.getAttribute=function(key){
		var getAttribute=new Array();
		this.Each(function(){
			if(this && this.getAttribute){
				var v=this.getAttribute(key);
				getAttribute.push(v ? v:'');
			}
		});
		return getAttribute.length==1?getAttribute[0]:getAttribute;
	};
	Glacier.fn.radio=function(isChecked){
		if(Glacier.isNaN(isChecked)){
			return this.val();
		}
		var v=new Array()
		;
		this.Each(function(){
			if(this.checked==isChecked)
				v.push(this.value);
		});
		return v;
	};
	Glacier.fn.html=function(newHtml){
		var htmls=new Array();
		this.Each(function(){
			if(Glacier.isNaN(newHtml))
				htmls.push(this.innerHTML ? this.innerHTML:'');
			else{
				this.innerHTML=newHtml;
				/*if(Glacier.isIE)this.innerHTML=newHtml;
				else{
					var newObj=this.cloneNode(false);
					newObj.innerHTML=newHtml;
					this.parentNode.replaceChild(newObj,this);
				}
				*/
			}
		});
		return (Glacier.isNaN(newHtml))?htmls:this;
	};
	Glacier.fn.val=function(newVal){
		var values=new Array();
		this.Each(function(){
			if(Glacier.isNaN(newVal))
				values.push(this.value ? this.value:'');
			else
				this.value=newVal;
		});
		return (Glacier.isNaN(newVal))?values:this;
	};
	Glacier.fn.test=function(newTest){
		var tests=new Array();
		this.Each(function(){
			if(Glacier.isNaN(newTest))
				tests.push(Glacier.isIE?(this.innerText ?this.innerText:''):(this.textContent ?this.textContent:(this.value?this.value:'')));
			else
				Glacier.isIE?(this.innerText=newTest):(this.textContent=newTest);
		});
		return (Glacier.isNaN(newTest))?tests:this;
	};
	Glacier.fn.setAttribute=function(key,val){
		this.Each(function(){
			key = Glacier.isIE6 || Glacier.isIE7?(Glacier.dom.ATTRIBUTES[key] || key):key;
			this.setAttribute(key,val);
		});
		return this;
	};
	Glacier.fn.style=function(c,type){
		var tmpdomtpe={};
		this.Each(function(){
			var tmpcss={}
			,	tmphtml=this.style.cssText?this.style.cssText+';'+c.style:c.style
			;
			for (k in c){
				if(!c.hasOwnProperty(k))continue;
				var tpk=Glacier.dom.TYPENAME[k]||k;
				if(tpk=='style' && type=='update'){
					Glacier.Each(
						tmphtml.split(";"),
						function(){
							var stycss=this.split(":");
							if(!Glacier.isNaN(stycss[0]))
								tmpcss[Glacier.Trim(stycss[0])]=stycss[1];
							
						}
					);
					
				}
			}
			tmpdomtpe[tpk]=Glacier.arraytoString(tmpcss)||c[k];
			Glacier.copyobject(this,tmpdomtpe,'dom');
		});
	};
	/**
	* 获取SELECT被选种的值或文本
	o{obj,isTrue,isValueTrue,type}
	**/
	Glacier.fn.getValueOrText=function(o){
		var v=new Array()
			,type=o.type||',';
			var obj=o.obj||this;
			if(obj.multiple){
				for(var i=0,n=obj.options.length;i<n;i++){
					if(obj.options[i].selected || o.isValueTrue){
						var e=obj.options[i];
						var vl="";
						if(o.isTrue) vl=e.value;
						else vl=e.text;
						v.push(vl);
					}
				}
			}else{
				v.push(o.isTrue?obj.value:obj.options[obj.selectedIndex].text);
			}
			return v.join(type);
	};
	/**
	* 跟据传进来的值或内容，设置被选种的值
	o{obj,value,isTrue,type}
	**/
	Glacier.fn.setValueOrText=function(o){
		var type=o.type||','
			,v=o.value.toString().split(type);
			var map={};
			for(var iv=0,nv=v.length;iv<nv;iv++){
				map[v[iv]]="true";
			}
			var obj=o.obj||this;
			for(var i=0,n=obj.options.length;i<n;i++){
				var e=obj.options[i];
				if(o.isTrue){
					if(map[e.value])
						e.selected=true;
					else
						e.selected=false;
				}else{
					if(map[e.text])
						e.selected=true;
					else
						e.selected=false;
				}
			}
	};
	/**
	* 跟据传进来的值或内容，删除值
	o{obj,value,isTrue,type}
	**/
	Glacier.fn.delValueOrText=function(o){
		var type=o.type||','
		,	v=o.value.toString().split(type)
		,	map={}
		,	fn=o.fn||function(i){
				this.remove(i);
			}
		;
			for(var iv=0,nv=v.length;iv<nv;iv++){
				map[v[iv]]="true";
			}
			var delv=new Array();
			var obj=o.obj||this;
			for(var i=0;i<obj.options.length;i++){
				var e=obj.options[i];
				if(o.isTrue){
					if(map[e.value]){
						fn.call(obj,i);
						i=i-1;
					}
				}else{
					if(map[e.text]){
						fn.call(obj,i);
						i=i-1;
					}
				}
			}
	};
	/**
	* 跟据传进来的值或内容，修改值
	o{obj,value,isTrue,type}
	**/
	Glacier.fn.editValueOrText=function(o){
		var type=o.type||','
		,	v=o.value.toString().split(type)
		,	map={}
		,	fn=o.fn||function(i){
				
			}
		;
			for(var iv=0,nv=v.length;iv<nv;iv++){
				map[v[iv]]="true";
			}
			var delv=new Array();
			var obj=o.obj||this;
			for(var i=0,n=obj.options.length;i<n;i++){
				var e=obj.options[i];
				if(o.isTrue){
					if(map[e.value]){
						fn.call(obj,i);
					}
				}else{
					if(map[e.text]){
						fn.call(obj,i);
					}
				}
			}
	};
	/**
	* 获取SELECT被选种的值
	**/
	Glacier.fn.getselvalue=function(){
		var arr={};
		this.Each(function(i){
			var type=this.type?this.type.toLowerCase():"";
			if(type.indexOf('select')>=0){
				var str=Glacier.getValueOrText({
								  obj:this,
								  isTrue:true
								 });
				var k=this.id||this.name||i;
				arr[k]=str;
			}
		});
		return arr;
	};
	/**
	* 获取SELECT被选种的文本
	**/
	Glacier.fn.getseltext=function(){
		var arr={};
		this.Each(function(){
			var type=this.type?this.type.toLowerCase():"";
			if(type.indexOf('select')>=0){
				var str=Glacier.getValueOrText({
								  obj:this,
								  isTrue:false
								 });
				var k=this.id||this.name||i;
				arr[k]=str;
			}
		});
		return arr;
	};
	/**
	* 获取SELECT所有值
	**/
	Glacier.fn.getallselvalue=function(){
		var arr={};
		this.Each(function(){
			var type=this.type?this.type.toLowerCase():"";
			if(type.indexOf('select')>=0){
				var str=Glacier.getValueOrText({
								  obj:this,
								  isTrue:true,
								  isValueTrue:true
								 });
				var k=this.id||this.name||i;
				arr[k]=str;
			}
		});
		return arr;
	};
	/**
	* 获取SELECT所有文本
	**/
	Glacier.fn.getallseltext=function(){
		var arr={};
		this.Each(function(){
			var type=this.type?this.type.toLowerCase():"";
			if(type.indexOf('select')>=0){
				var str=Glacier.getValueOrText({
								  obj:this,
								  isTrue:false,
								  isValueTrue:true
								 });
				var k=this.id||this.name||i;
				arr[k]=str;
			}
		});
		return arr;
	};
	/**
	* 跟据传入的VALUE设置相对应值的选种
	**/
	Glacier.fn.setselvalue=function(value){
		this.Each(function(){
			var type=this.type?this.type.toLowerCase():"";
			if(type.indexOf('select')>=0){
				Glacier.setValueOrText({
								  obj:this,
								  value:value,
								  isTrue:true
								 });
			}
		});
		return this;
	};
	/**
	* 跟据传入的Text设置相对应值的选种
	**/
	Glacier.fn.setseltext=function(value){
		this.Each(function(){
			var type=this.type?this.type.toLowerCase():"";
			if(type.indexOf('select')>=0){
				Glacier.setValueOrText({
								  obj:this,
								  value:value,
								  isTrue:false
								 });
			}
		});
		return this;
	};
	/**
	* 跟据传入的VALUE修改option
	**/
	Glacier.fn.editselbyvalue=function(value,fn){
		this.Each(function(){
			var type=this.type?this.type.toLowerCase():"";
			if(type.indexOf('select')>=0){
				Glacier.editValueOrText({
								  obj:this,
								  value:value,
								  isTrue:true,
								  fn:fn
								 });
			}
		});
		return this;
	};
	/**
	* 跟据传入的TEST修改option
	**/
	Glacier.fn.editselbytest=function(value,fn){
		this.Each(function(){
			var type=this.type?this.type.toLowerCase():"";
			if(type.indexOf('select')>=0){
				Glacier.editValueOrText({
								  obj:this,
								  value:value,
								  isTrue:false,
								  fn:fn
								 });
			}
		});
		return this;
	};
	/**
	* 跟据传入的VALUE删除相对应值
	**/
	Glacier.fn.delselvalue=function(value){
		this.Each(function(){
			var type=this.type?this.type.toLowerCase():"";
			if(type.indexOf('select')>=0){
				Glacier.delValueOrText({
								  obj:this,
								  value:value,
								  isTrue:true
								 });
			}
		});
		return this;
	};
	/**
	* 跟据传入的Text删除相对应值
	**/
	Glacier.fn.deldeltext=function(value){
		this.Each(function(){
			var type=this.type?this.type.toLowerCase():"";
			if(type.indexOf('select')>=0){
				Glacier.delValueOrText({
								  obj:this,
								  value:value,
								  isTrue:false
								 });
			}
		});
		return this;
	};
	/**
	* 跟据传入的Text删除相对应值
	**/
	Glacier.fn.delallsel=function(){
		this.Each(function(){
			var type=this.type?this.type.toLowerCase():"";
			if(type.indexOf('select')>=0){
				this.length=0;
			}
		});
		return this;
	};
	/**
	* 跟据传入的Text删除相对应值
	**/
	Glacier.fn.addsel=function(text,value){
		this.Each(function(){
			var type=this.type?this.type.toLowerCase():"";
			if(type.indexOf('select')>=0){
				this.options.add(new Option(text,value));
			}
		});
		return this;
	};
	Glacier.fn.Loaded=function(selector){
		Glacier.AttachEvent(window,'load',selector);
	};
	Glacier.fn.LoadedDom=function(f){
		var ie = !!(window.attachEvent && !window.opera);    
	    var wk = /webkit\/(\d+)/i.test(navigator.userAgent) && (RegExp.$1 < 525);    
		//var fn = [];    
		//var run = function () {
			//for (var i = 0; i < fn.length; i++) fn[i]();
		//};
		var d = document;
		if (!ie && !wk && d.addEventListener)    
			return d.addEventListener('DOMContentLoaded', f, false);
		//Glacier.fn.Loaded.call(this,f);    
		/*if (fn.push(f) > 1) return;*/
		if (ie)
			(function () {
				try { d.documentElement.doScroll('left'); f(); }    
				catch (err) { setTimeout(arguments.callee, 0); }    
			})();    
		else if (wk)    
			var t = setInterval(function () {
			if (/^(loaded|complete)$/.test(d.readyState))    
			clearInterval(t), f();    
		}, 0); 
	};
	Glacier.fn.bd=function(o){
		if(Glacier.isNaN(o.on) || Glacier.isNaN(o.callback)) return;
		var arg=arguments;
		this.Each(function(){
			var gb=Glacier.closure({obj:this,callBack:o.callback,arg:arg});
			if(o.on.indexOf('click')==0 && this.style) this.style.cursor='pointer';
			if(!this.gbak_fn)this.gbak_fn=[];
			this.gbak_fn[o.callback]=gb;
			Glacier.AttachEvent(this,o.on,gb);
		});
		return this;
	};
	Glacier.fn.delbd=function(o){
		if(Glacier.isNaN(o.on) || Glacier.isNaN(o.callback)) return;
		this.Each(function(){
			var gb=this.gbak_fn[o.callback];
			Glacier.RemoveAttachEvent(this,o.on,gb);
		});
		return this;
	};
	Glacier.fn.click=function(fn){
		this.bd.call(this,{on:'click',callback:fn},arguments);
	};
	Glacier.fn._Zindex=999999;
	Glacier.fn.MoveStart=function(obj,et,oEvent){
		this.style.zIndex=Glacier.Zindex();
		this.isMove=true;
		this.intervalid&&clearInterval(this.intervalid);
		var obj=Glacier.Obj(this),wsize=Glacier.windowSize();
		this._sw=obj.w;
		this._sh=obj.h;
		this._ww=wsize.sw;
		this._wh=wsize.sh;
		this._wl=wsize.scrollLeft;
		this._wt=wsize.scrollTop;
		this._x=oEvent.clientX-obj.x;
		this._y=oEvent.clientY-obj.y;
		if(Glacier.isIE)obj.setCapture();
		Glacier.AttachEvent(document, "mousemove", obj.mousemove);
		Glacier.AttachEvent(document, "mouseup", obj.mouseup);
		oEvent.preventDefault?oEvent.preventDefault():oEvent.returnValue = false;
	};
	Glacier.fn.MoveYesWindowIng=function(obj,et,oEvent){
		var L=oEvent.clientX-this._x;
		var T=oEvent.clientY-this._y;
		this.style.left=L+"px";
		this.style.top=T+"px";
		if(!Glacier.isNaN(this.isMoveWindowReset)){
			this.isWindowReset=false;
		}		
	};
	Glacier.fn.MoveNotWindowIng=function(obj,et,oEvent){
		var bw=0,bh=0;
		if((!Glacier.compatMode && Glacier.isIE) || Glacier.isIE6){
			bw=this._wl;
			bh=this._wt;
		}
		var L=oEvent.clientX-this._x,ckL=bw+this._ww-this._sw;
		var T=oEvent.clientY-this._y,ckT=bh+this._wh-this._sh;
		if(L>ckL) L=ckL;
		if(T>ckT) T=ckT;
		if(L<(bw+0)) L=bw+0;
		if(T<(bh+0)) T=bh+0;
		this.style.left=L+"px";
		this.style.top=T+"px";
		if(!Glacier.isNaN(this.isMoveWindowReset)){
			this.isWindowReset=false;
		}		
	};
	Glacier.fn.MoveStop=function(){
		Glacier.RemoveAttachEvent(document, "mousemove", this.mousemove);
		Glacier.RemoveAttachEvent(document, "mouseup", this.mouseup);
		if(Glacier.isIE)this.releaseCapture();
		this.initX=parseInt(this.style.left)-this._wl;
		this.initY=parseInt(this.style.top)-this._wt;
		this.isMove=false;
	};
	/*
	* Glacier的移动事件
	* moveobj:移动对象
	* ismovewindow : false 是否可以移出窗外
	* position:窗体是否悬浮absolute  fixed|absolute
	*/
	Glacier.fn.Move=function(o){
		if(!o) o={n:''};
		this.Each(function(i,op){
			if((typeof o.n)=="number" && i!=o.n || !this) return;			
			var obj=this,objMove=obj;
			o.obj=obj;
			obj.move_o=o;
			if(!this.gobj)
				Glacier.fn.Gdiv(o);
			var _obj=o.obj.gobj,moveing=op.MoveYesWindowIng;
			_obj.mousedown=Glacier.closure({obj:_obj,callBack:op.MoveStart});
			if(!o.ismovewindow)moveing=op.MoveNotWindowIng;
			_obj.mousemove=Glacier.closure({obj:_obj,callBack:moveing});
			_obj.mouseup=Glacier.closure({obj:_obj,callBack:op.MoveStop});
			if(o.moveobj && o.moveobj[i] && !Glacier.isNaN(o.moveobj[i])) objMove=o.moveobj[i];
			objMove.style.cursor="move";
			Glacier.AttachEvent(objMove,"mousedown",_obj.mousedown);
		});
		return this;
	};
	Glacier.fn.getMouseCoords=function(e) {
		return {
			x: Glacier.isIE ? e.clientX + Math.max(document.body.scrollLeft, document.documentElement.scrollLeft) : e.pageX,
			y: Glacier.isIE ? e.clientY + Math.max(document.body.scrollTop, document.documentElement.scrollTop) : e.pageY
		};
	};
	Glacier.fn.MoveSize=function(o){
		if(!o) o={obj:'',sizeObj:''};
		if(!o.obj)o.obj=this;
		if(!o.sizeObj) o.sizeObj=o.obj;
		if(!o.obj) return;
		if(Glacier.isString(o.obj)) o.obj=Glacier('#'+o.obj)[0];
		if(Glacier.isString(o.sizeObj)) o.sizeObj=Glacier('#'+o.sizeObj)[0];
		var sobj=o.sizeObj
		,	obj=o.obj
		,	minW = o.minw||1
		,	minH = o.minh||1
		,	wX = sobj.offsetWidth - sobj.offsetWidth
		,	wH = sobj.offsetHeight - sobj.offsetHeight
		,	isLeft	=	o.isleft||true
		,	isRight	=	o.isRight||true
		,	isTop	=	o.isTop||true
		,	isBottom	=	o.isBottom||true
		;
		obj.style.zIndex = o.zindex||Glacier.Zindex();
		obj.onmousedown = function(e) {
			var d = document;
			if (!e) e = window.event;
			var x = e.layerX ? e.layerX: e.offsetX,
			y = e.layerY ? e.layerY: e.offsetY;
			var MCD = Glacier.getMouseCoords(e);
				sobj.startX = MCD.x;
				sobj.startY = MCD.y;
				sobj.startW = sobj.offsetWidth;
				sobj.startH = sobj.offsetHeight;
				
			
			if(o.callbak_mousedown)o.callbak_mousedown(this,e);
			
			if (obj.setCapture)
			 obj.setCapture();
			else if (window.captureEvents)
			 window.captureEvents(Event.MOUSEMOVE | Event.MOUSEUP);
			d.onmousemove = function(e) {
				if (!e) e = window.event;
				var mus = Glacier.getMouseCoords(e);
				var newW = (sobj.startW + (mus.x - sobj.startX));
				var newH = (sobj.startH + (mus.y - sobj.startY));
				newW = newW < minW ? minW: newW;
				newH = newH < minH ? minH: newH;
				var mw=(newW - wX);
				var mh=(newH - wH);
				sobj.style.width = mw + "px";
				sobj.style.height = mh + "px";
				if(o.callbak)o.callbak(sobj,e,mw,mh);
			};
			d.onmouseup = function() {
				if (obj.releaseCapture)
				 obj.releaseCapture();
				else if (window.captureEvents)
				 window.captureEvents(Event.MOUSEMOVE | Event.MOUSEUP);
				d.onmousemove = null;
				d.onmouseup = null;
				if(o.callbak_mouseup)o.callbak_mouseup(sobj);
			};
		};
	};
	/*
	* 去除Glacier的移动事件
	*/
	Glacier.fn.RemoveMove=function(){
		this.Each(function(i){
			if(Glacier.isNaN(this.move_o)) return;
			if((typeof this.move_o.n)=="number" && i!=this.move_omove_o.n || !this) return;
			var objMove=this;
			if(this.move_o.moveobj && !Glacier.isNaN(this.move_o.moveobj[i])) objMove=this.move_o.moveobj[i];
			objMove.style.cursor="";
			if(!Glacier.isNaN(this.gobj.mousedown))
				Glacier.RemoveAttachEvent(objMove, "mousedown",this.gobj.mousedown);
		});
		return this;
	};
	/*
	* IE6中，增加遮挡select等的IFRMAE
	*/
	Glacier.fn.ie6DivForIframe=function(){
		var iframeIe6=document.createElement('iframe');
		iframeIe6.style.cssText="position:absolute; z-index: -1; width: 100%; height: 100%;top: 0;left: 0; scrolling: no;filter:alpha(opacity=0);-moz-opacity: 0;opacity: 0;";
		iframeIe6.setAttribute("hspace","0");
		iframeIe6.setAttribute("frameborder","0");
		iframeIe6.src="about:blank";
		return iframeIe6;
	};
	/*
	* IE6中，层悬浮时，屏不抖动
	*/
	Glacier.fn.ie6More=function(){
		if(Glacier.isIE6){
			var imgSrc = document.body.currentStyle.backgroundImage;
			imgSrc = imgSrc.slice(5,imgSrc.length-2);
			if(!imgSrc){
				document.body.style.backgroundAttachment="fixed";
				document.body.style.backgroundImage="url(nothing)";
			}
		}
	};
	/*
	* 锁屏时的全屏背景层
	*/
	Glacier.fn.AltDiv=function(o){
		if(!o) o={};
		var lockDiv="";
		if(o.lock){
			var lockOpa=(o.lock_opa || 75);
			lockDiv=Glacier.dom({type:'div'});
			var wsize=Glacier.windowSize();
			lockDiv.setstyle({style:"z-index:"+Glacier.Zindex()+";position:absolute;top: 0px;left: 0px;height:"+wsize.h+"px;width:"+wsize.w+"px;background:"+(o.lock_back || "#000")+";filter:alpha(opacity="+lockOpa+");-moz-opacity: 0."+lockOpa+";opacity: 0."+lockOpa+";"});
			lockDiv.add();
			if(o.lockfn){
				lockDiv.dom.onclick=o.lockfn;
			}
			if(Glacier.isIE6) lockDiv.dom.appendChild(this.ie6DivForIframe());
			this.AttachEvent(window,'load',function(){
												    var wsize=this.windowSize();
													document.getElementById(lockDiv.dom.id).style.width=wsize.w+"px";
													document.getElementById(lockDiv.dom.id).style.height=wsize.h+"px";
												  });
		}
		return lockDiv.dom;
	};
	/*
	* 对象尺寸调整为全屏状态
	*/
	Glacier.fn.AltDivInit=function(o){
		if(o){
			var wsize=Glacier.windowSize();
			o.style.width=wsize.w+"px";
			o.style.height=wsize.h+"px";
		}
	};
	/**
		给要操作的层加上一个隐形的DIV框架，DIV取被操作对像的TOP、LEFT、WIDTH、HEIGHT，确保对实现功能的同时不对被操作对像进行修改
	**/
	Glacier.fn.Gdiv=function(o){
		o.obj.style.visibility="";
		o.obj.style.display="";
		var _obj_init=Glacier.Obj(o.obj),winsize=Glacier.windowSize(),ieDiv=document.createElement('div'),ieDivCss="position:absolute;width:"+_obj_init.w+"px; height:"+_obj_init.h+"px;";//border:1px solid red;
		ieDiv.ieDivCss=ieDiv.ieDivcss_bak=ieDiv.style.cssText=ieDivCss;
		o.obj.style.position='static';
		if(o.position) ieDiv.style.position=o.position;
		o.obj.parentObj=o.obj.parentNode;
		ieDiv.appendChild(o.obj);
		ieDiv.initX=_obj_init.x-winsize.scrollLeft;
		ieDiv.initY=_obj_init.y-winsize.scrollTop;
		if(o.position=='fixed'){
			var ssw=winsize.sw-_obj_init.w,ssh=winsize.sh-_obj_init.h;
			_obj_init.x=ssw>ieDiv.initX?ieDiv.initX:ssw;
			_obj_init.y=ssh>ieDiv.initY?ieDiv.initY:ssh;
			if(Glacier.isIE6 || (!Glacier.compatMode && Glacier.isIE) || Glacier.mobile){
				ieDiv.style.position='absolute';
				_obj_init.x=ssw>ieDiv.initX?ieDiv.initX:ssw;
				_obj_init.y=ssh>ieDiv.initY?ieDiv.initY:ssh;
				ieDiv.initX=_obj_init.x;
				ieDiv.initY=_obj_init.y;
				ieDiv.intervalidgo=function(){
					ieDiv.intervalid=setInterval(function(){
						if(ieDiv.style.display!='none' && !ieDiv.isMove){
							var wsize=Glacier.windowSize();
							ieDiv.style.left=(ieDiv.initX+wsize.scrollLeft)+'px';
							ieDiv.style.top=(ieDiv.initY+wsize.scrollTop)+'px';
							if(!Glacier.mobile) clearInterval(ieDiv.intervalid);
						}
					}, 1000)
				};
				if(Glacier.mobile)
					ieDiv.intervalidgo();
			}
		}
		ieDiv.style.left=_obj_init.x+'px';
		ieDiv.style.top=_obj_init.y+'px';
		if(Glacier.isIE6){
			Glacier.fn.ie6More();
			ieDiv.style.position='absolute';
			ieDiv.appendChild(Glacier.fn.ie6DivForIframe());
		}
		
		if(o.index)ieDiv.style.zIndex=o.index;
		document.body.appendChild(ieDiv);
		o.obj.gobj=ieDiv;
	};
	Glacier.fn.WinAlt=function(o){
		if(!o) o={};
		var lockDiv=''
		,	os=new Array()
		;
		if(!this.lockObj)lockDiv=this.AltDiv(o);
		else lockDiv=this.lockObj;
		if(lockDiv){
			lockDiv.style.display='';
			this.lockObj=lockDiv;
		}
		this.Each(function(i,op){
			if((typeof o.index)=="number" && i!=o.index || !this) return;
			o.obj=this;
			this.style.visibility="";
			this.style.display="";
			if(!o.obj.gobj)
				Glacier.fn.Gdiv(o);
			o.obj.gobj.style.visibility="";
			o.obj.gobj.style.display="";
			o.obj.o=o;
			op.WinAltObj(o);
			os.push({obj:o.obj,o:o});
		});
		this.altScroll=function(){
			Glacier.Each(function(){
								var obj=this.obj,i=0,objo=this.o;
								if(!obj.gobj) return;
								obj.gobj.style.visibility="";
								if(o.position=='fixed' && obj.gobj.ieDivCss && (Glacier.isIE6 || (!Glacier.compatMode && Glacier.isIE))){
									obj.gobj.intervalidgo();
								}
							  },os);
			
		};
		this.altResize=function(){
			Glacier.Each(function(){
										var obj=this.obj,objo=this.o;
										if(!obj.gobj) return;
										objo.obj=obj;
										obj.gobj.style.visibility="";
										objo.isCallBack=true;
										if(obj.gobj.isWindowReset)
											Glacier.fn.WinAltObj(objo);
										Glacier.fn.AltDivInit(lockDiv);
								  },os);
			
		};
		Glacier.AttachEvent(window,"scroll",this.altScroll);
		Glacier.AttachEvent(window,"resize",this.altResize);
		this.AltDivInit(lockDiv);
		return this;
	};
	Glacier.fn.WinAltObj=function(o){
		if(!o || !o.obj || !o.obj.gobj) return;
		/**参数与默认值**/
		
		var _obj=o.obj.gobj;
		if(!_obj.style.width || !_obj.style.height) return;
		var _isTop=o.isTop || false; 							//顶
		var _isBottom=o.isBottom || false;						//底
		var _isLeft=o.isLeft || false;							//左
		var _isRight=o.isRight || false;						//右
		var _isCenter=o.isCenter || false;						//居中
		var _isXCenter=o.isXCenter || false;					//X轴居中
		var _isYCenter=o.isYCenter || false;					//Y轴居中
		var _yLenPx=parseInt(o.yLenPx || "0");					//距Y轴边距
		var _xLenPx=parseInt(o.xLenPx || "0");					//距X轴边距
		var _zIndex=o.zIndex || Glacier.Zindex();					//对象图层轴上位置
		var _lock=o.lock || false;								//是否锁定浏览器
		var _callBack=o.callBack || function(){};				//是否回调方法
		_obj.isWindowReset=o.isWindowReset!=undefined?o.isWindowReset:true;			    	//是否在窗体发生变化时，调整层的相对位置
		_obj.isMoveWindowReset=o.isMoveWindowReset!=undefined?o.isMoveWindowReset:true;		//是否在窗体发生变化时，已拖动的层，位置是否还原初始值
		
		/**设置**/
		if(o.top || o.lock)
			_obj.style.zIndex=_zIndex;
		else {
			_obj.style.zIndex=o.zIndex||(Glacier.Zindex()-333333);
		}
		var T=0,L=0;
		//var w = parseInt(_obj.offsetWidth||_obj.clientWidth);
		//var h = parseInt(_obj.offsetHeight||_obj.clientHeight);
		var show_obj_ini=Glacier.Obj(_obj);
		var w = show_obj_ini.w
		,	h = show_obj_ini.h
		;
		if(o.isCallBack)_callBack=function(){};

		/**************兼容性设置*******************/
		var wobj=Glacier.windowSize();
		if(_obj.style.position=='fixed'){
			wobj.scrollTop=0;
			wobj.scrollLeft=0;
		}
		
		if(_isCenter){
			L=wobj.scrollLeft+(wobj.sw/2)-(w/2);
			T=wobj.scrollTop+(wobj.sh/2)-(h/2);
			if(T<0) T=0;
			if(L<0) L=0;
		}else{
			if(_isYCenter){
				T=wobj.scrollTop+(wobj.sh/2)-(h/2);
				if(T<0) T=0;
			}else{
				T=_yLenPx;
				if(!_isTop && _isBottom)
					T=wobj.scrollTop+wobj.sh-parseInt(_obj.style.height.replace('px',''))-_yLenPx;
			}
			if(_isXCenter){
				L=wobj.scrollLeft+(wobj.sw/2)-(w/2);
				if(L<0) L=0;
			}else{
				L=_xLenPx;
				if(!_isLeft && _isRight)
					L=wobj.scrollLeft+wobj.sw-parseInt(_obj.style.width.replace('px',''))-_xLenPx;
			}
		}
		_obj.style.top=T+"px";
		_obj.style.left=L+"px";
		_obj.initX=L-wobj.scrollLeft;
		_obj.initY=T-wobj.scrollTop;
		//alert(_obj.style.position+'\r\ntop:'+_obj.style.top+'\r\nleft:'+_obj.style.left);
		_callBack();
	};
	Glacier.fn.display=function(o){
		o=o||{};
		index=o.i;
		var list=this;
		var value=function(v){
			list.Each(function(i,op){
				if(i==0 && ((typeof index)!="number" || index==-1) && this.o && this.o.lock && op.lockObj){
					op.lockObj.style.display=(v=='dell'?'none':v);
				}
				if((typeof index)=="number" && i!=index || !this || !this.gobj) return;
				if(v=="dell"){
					var trueObj=this.gobj.firstChild;
					if(Glacier.isNaN(trueObj)) return;
					var g_objDom=Glacier(trueObj);
					g_objDom.style({style:'display:none;'},'update');
					trueObj.parentObj.appendChild(trueObj);
					trueObj.style.cursor="";
					if(!Glacier.isNaN(this.gobj.parentNode))this.gobj.parentNode.removeChild(this.gobj);
					if(!Glacier.isNaN(op.lockObj) && !Glacier.isNaN(op.lockObj.parentNode))op.lockObj.parentNode.removeChild(op.lockObj);
					trueObj.gobj='';
					trueObj.move_o='';
					trueObj.o='';
					op.lockObj='';
				}else{
					this.style.display=v;
					this.gobj.style.display=v;
				}
				o.fn && o.fn();
			});
		};
		var lockDisplay=function(v){
			list.Each(function(i,op){
				op.lockObj&&function(){op.lockObj.style.display=(v=='dell'?'none':v)}();
			});
		};
		this.none=function(){value("none");};
		this.block=function(){value("block");};
		this.show=function(){value("");};
		this.dell=function(){value("dell");};
		this.lock=function(){
			return {
						none:function(){
								lockDisplay("none");
								value("none");
							}
					,	block:function(){
								lockDisplay("block");
								value("block");
							}
					,	show:function(){
								lockDisplay("");
								value("");
							}
					,	dell:function(){
								lockDisplay("dell");
								value("dell");
							}
				};
		};
		return this;
	};
	//*****************************************************插件*******************************************************//
	Glacier.fn.getQuery=function(name){
		var reg=new RegExp(String("(^|&|\\?)"+name+"=([^&]*)(&|$)")),r;
		if(r=this.toString().match(reg))
			return unescape(r[2]);
		return null;
	};
	Glacier.fn.getid=function(name){
		return Glacier.getQuery.call(document.location,name);
	};
	Glacier.fn.isNaN=function(obj){
		return obj=="undefined" || obj==undefined || obj=="" || obj=="null" || obj == null;
	};
	/**
	* 去除全部script
	*/
	Glacier.fn.dellscript=function(str){
		str=str+'';
		var reg =/((.|^)<script(.*)<\/script>)/ig;
		return str.replace(reg,"");
	};
	Glacier.fn.cklogin=function(obj,i,x){
		return (new RegExp("^[a-zA-Z0-9]{1}([a-zA-Z0-9]|[._-]){"+(i>0?(i-1):0)+","+(x>0?(x-1):0)+"}$").test(Glacier.fn.Trim(obj)));
	};
	/**
	* 去除全部空格
	*/
	Glacier.fn.Trim=function(obj){
		return obj.toString().replace(/\s/g,'');
	};
	/**
	* 去除双空格将所有的一个以上的空格替换成一个空格
	*/
	Glacier.fn.TrimOne=function(obj){
		return obj.toString().replace(/\s{2,}/g," ");
	};
	/**
	* 去除前后二端空格
	*/
	Glacier.fn.TrimTest=function(obj){
		return obj.toString().replace(/(^\s*)|(\s*$)/g,'');
	};
	/**
	* 去除左空格
	*/
	Glacier.fn.TrimLeft=function(obj){
		return obj.toString().replace(/(^\s*)/g,'');
	};
	/**
	* 去除右空格
	*/
	Glacier.fn.TrimRight=function(obj){
		return obj.toString().replace(/(\s*$)/g,'');
	};
	/**
	* 验证普通电话、传真号码：可以“+”开头，除数字外，可含有“-” 
	*/
	Glacier.fn.isTel=function(str)
	{
		return (/^(([0\+]\d{2,4}-)?(0\d{2,3})-)(\d{7,8})(-(\d{3,}))?$/.test(str));
	};
	/**
	* 验证移动电话（手机）
	*/
	Glacier.fn.isMobile=function(str) { 
		return (/^(?:13\d|1[3-9][0-9])-?\d{5}(\d{3}|\*{3})$/.test(str)); 
	};
	/**
	* 验证邮箱
	* 例：xxx@xxx.xxx
	*/
	Glacier.fn.isMail=function(str)
	{
		return (/^\w+((-\w+)|(\.\w+))*\@[A-Za-z0-9]+((\.|-)[A-Za-z0-9]+)*\.[A-Za-z0-9]+$/.test(str));
	};
	Glacier.fn.isNumber=function(i){
		return (/^[0-9]+.?[0-9]*$/.test(i))?true:false;
	};
	Glacier.fn.type=function( obj ) {
		return obj == null ?String( obj ) : class2type[ toString.call(obj) ] || "object";
	},
	Glacier.fn.isObj=function(obj){
		return Glacier.type(obj) === "object";
	};
	Glacier.fn.isFun=function(obj){
		return Glacier.type(obj) === "function";
	};
	Glacier.fn.isWindow=function( obj ) {
		return obj && typeof obj === "object" && "setInterval" in obj;
	},
	Glacier.fn.isArray=function(obj){
		return Glacier.fn.type(obj) === "array";
	};
	Glacier.fn.isString=function(obj){
		return typeof obj=="string"||false;
	};
	/**
	* 查找字符串位置
	*/
	Glacier.fn.findstring=function(str,fstr)
	{
		return str.indexOf(fstr||'');
	};
	/**
	* 返回中英文字符长度
	*/
	Glacier.fn.len=function(str)
	{
		return str.toString().replace(/[^\x00-\xff]/g, "**").length;
	};
	Glacier.fn.toDecimal2=function(x){
		var f = parseFloat(x);  
		if (isNaN(f)) {  
			return false;  
		}  
		var f = Math.round(x*100)/100;  
		var s = f.toString();  
		var rs = s.indexOf('.');  
		if (rs < 0) {  
			rs = s.length;  
			s += '.';  
		}  
		while (s.length <= rs + 2) {  
			s += '0';  
		}  
		return s;
	};
	/**
	*
	*/
	Glacier.fn.formatdate=function(date,format){
		var o = {
			"M+" : date.getMonth()+1,
			"d+" : date.getDate(),
			"h+" : date.getHours(),
			"m+" : date.getMinutes(),
			"s+" : date.getSeconds(),
			"q+" : Math.floor((date.getMonth()+3)/3),
			"S" : date.getMilliseconds()
		};
		if(/(y+|Y+)/.test(format)) {
			format = format.replace(RegExp.$1, (date.getFullYear()+"").substr(4 - RegExp.$1.length));
		}
		
		for(var k in o) {
			if(new RegExp("("+ k +")").test(format)) {
				format = format.replace(RegExp.$1, RegExp.$1.length==1 ? o[k] : ("00"+ o[k]).substr((""+ o[k]).length));
			}
		}
		return format;
	};
	/**
	* 截取中英文字符长度
	*/
	Glacier.fn.lenSub=function(str,n,endStr)
	{
	  var r = /[^\x00-\xff]/g;
	  if(Glacier.isNaN(str))return str;
	  if(str.replace(r, "mm").length <= n) return str;
	  var m = Math.floor(n/2);
	  for(var i=m,ni=str.length; i<ni; i++)
	  {
		if(str.substr(0, i).replace(r, "mm").length>=n)
		{
		  return str.substr(0, i)+(endStr?endStr:'');
		}
	  }
	  return str;
	};
	Glacier.fn.filterUrl=function(url){
		var ajaxLocParts,ajaxLocation,
		rurl = /^([\w\+\.\-]+:)(?:\/\/([^\/?#:]*)(?::(\d+))?)?/;
		try {
			ajaxLocation = location.href;
		} catch( e ) {
			ajaxLocation = document.createElement( "a" );
			ajaxLocation.href = "";
			ajaxLocation = ajaxLocation.href;
		}
		ajaxLocParts = rurl.exec( ajaxLocation.toLowerCase() ) || [];
		Glacier.fn.ajaxSettings.isLocal=(/^(?:about|app|app\-storage|.+\-extension|file|res|widget):$/.test( ajaxLocParts[ 1 ] ));
		return ( ( url?url:ajaxLocation ) + "" ).replace( /#.*$/, "" ).replace( /^\/\//, ajaxLocParts[ 1 ] + "//" );
	};
	Glacier.fn.copyobject=function(destination, source,type) {
		for (property in source) {
		   if(!source.hasOwnProperty(property))continue;
		   if(type=='dom'){
			   if (property == "style") {
				   	try{
					destination.style.cssText = source[property];
					} catch( e ) {}
				} else {
					var key = Glacier.isIE6 || Glacier.isIE7?(this.dom.ATTRIBUTES[property] || property):property;
					destination.setAttribute(key, source[property]);
				}
		   }else
				destination[property] = source[property];
		}
		return destination;
	};
	Glacier.fn.dom=function(o){
		o=o||{};
		var type		=	o.type
		,	attributes	=	o.att	||{}
		,	domtype		=	o.dom	||{}
		,	name		=	o.name||Glacier.uuidFast().replace(/-/ig,'')
		,	id			=	o.id||Glacier.uuidFast().replace(/-/ig,'')
		;
		if(!Glacier.isString(type) || !type){
			attributes=type;
			type='div';
		}
		var div					=		document.createElement(type)
		,	tmpdomtpe			=		{}
		,	retdom				=		function(){return Glacier.copyobject(div,attributes,'dom');}
		;
		div.id=id;
		div.setAttribute('name',name);
		for (k in domtype){
			if(!domtype.hasOwnProperty(k))continue;
			var tpk=this.dom.TYPENAME[k]||k;
			tmpdomtpe[tpk]=domtype[k];
		}
		div=Glacier.copyobject(div,tmpdomtpe);
		return {
			dom:div,
			add:function(obj){
				obj=obj||document.body;
				obj.appendChild(this.dom);
				return this;
			},
			remove:function(){
				var delDom=Glacier('#'+(this.dom||div).id)
				;
				if(delDom.length>0)
					delDom[0].parentNode.removeChild(delDom[0]);
				return this;
			},
			setclass:function(name){
				this.dom.className=name;
			},
			setstyle:function(attributes){
				Glacier.copyobject(this.dom,attributes,'dom');
			},
			del:function(){
				//this.dom='';
				this.remove();
			},
			'':''};
	};
	Glacier.fn.dom.TYPENAME={
			"html"			:	"innerHTML"	,
			"val"			:	"value"	,
			"test"			:	Glacier.isIE?"innerText":"textContent",
			"classname"		:	"className",
	"":""};
	Glacier.fn.dom.ATTRIBUTES={
			"acceptcharset"			:	"acceptCharset"			,
			"accesskey"				:	"accessKey"				,
			"allowtransparency"		:	"allowTransparency"		,
			"bgcolor"				:	"bgColor"				,
			"cellpadding"			:	"cellSpacing"			,
			"cellspacing"			:	"cellPadding"			,
			"class"					:	"className"				,
			"colspan"				:	"colSpan"				,
			"checked"				:	"defaultChecked"		,
			"selected"				:	"defaultSelected"		,
			"for"					:	"htmlFor"				,
			"frameborder"			:	"frameBorder"			,
			"hspace"				:	"hSpace"				,
			"longdesc"				:	"longDesc"				,
			"maxlength"				:	"maxLength"				,
			"marginwidth"			:	"marginWidth"			,
			"marginheight"			:	"marginHeight"			,
			"noresize"				:	"noResize"				,
			"noshade"				:	"noShade"				,
			"readonly"				:	"readOnly"				,
			"rowspan"				:	"rowSpan"				,
			"tabindex"				:	"tabIndex"				,
			"valign"				:	"vAlign"				,
			"vspace"				:	"vSpace"				,
	"":""};
	Glacier.fn.isPlainObject=function( obj ) {
		if ( !obj || Glacier.type(obj) !== "object" || obj.nodeType || Glacier.isWindow( obj ) ) {
			return false;
		}

		try {
			if ( obj.constructor &&
				!hasOwn.call(obj, "constructor") &&
				!hasOwn.call(obj.constructor.prototype, "isPrototypeOf") ) {
				return false;
			}
		} catch ( e ) {
			return false;
		}
		var key;
		for ( key in obj ) {}

		return key === undefined || hasOwn.call( obj, key );
	};
	Glacier.fn.buildParams=function( prefix, obj, add ) {
		if ( Glacier.isArray( obj ) ) {
			Glacier.Each( function( i ) {
				if ( /\[\]$/.test( prefix ) ) {
					add( prefix, this );
				} else {
					Glacier.buildParams( prefix + "[" + ( typeof this === "object" || Glacier.isArray(this) ? i : "" ) + "]", this, add );
				}
			},obj);
	
		} else if ( obj != null && typeof obj === "object") {
			for ( var name in obj ) {
				try{
				if(!obj.hasOwnProperty(name))continue;
				Glacier.buildParams( prefix + "[" + name + "]", obj[ name ], add );
				} catch( e ) {continue;}
			}
		} else {
			add( prefix, obj );
		}
	};
	Glacier.fn.formatparam=function(o){
		o=o||{};
		var s = []
		,	add = function( key, value ) {
				value = Glacier.isFun( value ) ? value() : value;
				s[ s.length ] = o.Handle(key,value);
			}
		;
		if(!Glacier.isNaN(o.param)){
			if ( Glacier.isArray( o.param )) {
				Glacier.Each( function() {
					add( this.name, this.value );
				},o.param);
	
			} else {
				for ( var prefix in o.param ) {
					if(!o.param.hasOwnProperty(prefix))continue;
					Glacier.buildParams( prefix, o.param[ prefix ], add );
				}
			}
		}
		return s.join( o.separate );
	};
	Glacier.fn.param=function(param){
		return Glacier.formatparam({
				param		:	param
			,	separate	:	"&"
			,	Handle		:	function(key,value){
					return encodeURIComponent( key ) + "=" + encodeURIComponent( value );
				}
		}).replace( /%20/g , "+" );
	};
	Glacier.fn.arraytoString=function(param){
		return Glacier.formatparam({
				param		:	param
			,	separate	:	";"
			,	Handle		:	function(key,value){
					return key + ":" +  value;
				}
		});
	};
	Glacier.fn.tojson=function(param){
		return Glacier.formatparam({
				param		:	param
			,	separate	:	","
			,	Handle		:	function(key,value){
					return '"'+encodeURIComponent( key )+'":"'+encodeURIComponent( value )+'"';
				}
		});
	};
	Glacier.fn.Stringtojson=function(a){
		a=a+"";
		var s = {},
		add = function( key, value ) {
				s[key.replace(/"|'/g,'')] = value.replace(/"|'/g,'')||'';
		}
		;
		if ( Glacier.isString( a ) ) {
			Glacier.Each( function() {
				var tmpList=this.split(":");
				add( tmpList[0], tmpList[1] );
			},a.split(","));

		}
		return s;
	};
	Glacier.fn.closure=function(o){
		return function(event){
			if(Glacier.isNaN(o)) return;
			var e=(event || window.event);
			var ev=(e  ||  arguments.callee.caller.arguments[ 0 ]);
		    var et=ev.srcElement || ev.target;
			o.callBack.call(o.obj,o.arg,et,e);
		}
	};
	Glacier.fn.Browser=function(){
		var ua=navigator.userAgent.toLowerCase()
		,	check = function(reg){
				var str=ua.match(reg);
				if(str){
					Glacier.getBrowser=brow[reg];
					Glacier.getBrowserVersion=str[1];
				}
				return str;
			}
		,	brow={}
		;
		brow[/msie ([\d.]+)/]='ie';
		brow[/firefox\/([\d.]+)/]='firefox';
		brow[/chrome\/([\d.]+)/]='chrome';
		brow[/opera.([\d.]+)/]='opera';
		brow[/version\/([\d.]+).*safari/]='safari';
		brow[/maxthon\/([\d.]+)/]='maxthon';
		brow[/mobile/]='mobile';
		
		Glacier.isIE=check(/msie ([\d.]+)/) || false;
		if(!Glacier.fn.isNaN(Glacier.isIE) && Glacier.isIE.length>1){
			Glacier.isIE6=Glacier.isIE[1]=="6.0"?Glacier.isIE[1]:false;
			Glacier.isIE7=Glacier.isIE[1]=="7.0"?Glacier.isIE[1]:false;
			Glacier.isIE8=Glacier.isIE[1]=="8.0"?Glacier.isIE[1]:false;
			Glacier.isIE9=Glacier.isIE[1]=="9.0"?Glacier.isIE[1]:false;
			Glacier.isIE10=Glacier.isIE[1]=="10.0"?Glacier.isIE[1]:false;
		}else{
			Glacier.isIE6=false;
			Glacier.isIE7=false;
			Glacier.isIE8=false;
			Glacier.isIE9=false;
		}
		Glacier.firefox=check(/firefox\/([\d.]+)/) || false;
		Glacier.chrome=check(/chrome\/([\d.]+)/) || false;
		Glacier.opera=check(/opera.([\d.]+)/) || false;
		Glacier.safari=check(/version\/([\d.]+).*safari/) || false;
		Glacier.maxthon=check(/maxthon\/([\d.]+)/) || false;
		Glacier.compatMode=document.compatMode=="CSS1Compat"?true:false;
		Glacier.mobile=check(/mobile/) || false;;
	}();
	Glacier.fn.AttachEvent=function(obj,fn,gb,useCapture){
		if(obj.addEventListener)
			obj.addEventListener(fn,gb,(useCapture||false));
		else if(obj.attachEvent)
			obj.attachEvent("on"+fn,gb);
		else
			obj["on"+fn]=gb;
	};
	Glacier.fn.RemoveAttachEvent=function(obj,fn,gb,useCapture){
		if(obj.removeEventListener){
			obj.removeEventListener(fn,gb,(useCapture||false));
		} else if(obj.detachEvent){
			obj.detachEvent("on"+fn,gb);
		} else{ 
			obj["on"+fn]=null;



		}
	};
	Glacier.fn.Zindex=function(){
		return this.fn._Zindex++;
	};
	Glacier.fn.windowSize=function(){
		var o = {x:0,y:0},windowobj=Glacier.compatMode?document.documentElement:document.body; 
		o.w=Glacier.mobile?Math.max(windowobj.scrollWidth,windowobj.clientWidth,screen.width):Math.max(windowobj.scrollWidth,windowobj.clientWidth);
		o.h=Glacier.mobile?Math.max(windowobj.scrollHeight,windowobj.clientHeight,screen.height):Math.max(windowobj.scrollHeight,windowobj.clientHeight);
		o.sw=Glacier.mobile?Math.max(windowobj.clientWidth,screen.width):windowobj.clientWidth;
		o.sh=Glacier.mobile?Math.max(windowobj.clientHeight,screen.height):windowobj.clientHeight;
		o.scrollTop=windowobj.scrollTop || window.pageYOffset || document.body.scrollTop;
		o.scrollLeft=windowobj.scrollLeft || window.pageXOffset || document.body.scrollLeft;
		return o;
	};
	Glacier.fn.Obj=function(obj){
		var o=obj;
		if(!obj)
			o=obj=this[0];
		var pt = {x:0,y:0}; 
		do
		{
		   try{
		   pt.x += obj.offsetLeft;
		   pt.y += obj.offsetTop;
		   obj = obj.offsetParent;
		   } catch( e ) {}
		}
		while(obj);
		o.x=pt.x;
		o.y=pt.y;
		o.w = parseInt(o.offsetWidth||o.clientWidth);
		o.h = parseInt(o.offsetHeight||o.clientHeight);
		return o;
	};
	Glacier.fn.getPrevioussibling=function(obj,o){
		o=o||{};
		var x=obj.previousSibling
		,	ck=function(x){
				var isTrue=(x.nodeType!=1);
				if(!isTrue) return false;
				if(o.name && !obj.getAttribute('name')!=o.name)
					return false;
				if(o.id && !obj.getAttribute('id')!=o.id)
					return false;
				return true;
			}
		;
		while (ck(x))
		{
			x=x.previousSibling;
		}
		return x;
	};
	Glacier.fn.getParent=function(obj,tagName){
	    if(Glacier.isNaN(obj))return(null);
	    if(obj.tagName==tagName){
	        return(obj);
	    }else{
	        return(Glacier.getParent(obj.parentNode,tagName));
	    }
	};
	Glacier.fn.ie6alt=function(){
		var o=this.o;
		if(o.obj.gobj.isMove) return;
		o.obj.gobj=this;
		Glacier.fn.WinAltObj(o);
	};
	/***********************************AJAX的实现开始**************************************************/
	Glacier.fn.ajaxErr=function(XMLHttpRequest, textStatus, errorThrown){
		
	};
	Glacier.fn.ajaxSettings={
		url: '',
		isLocal: '',
		type: "GET",
		contentType: "application/x-www-form-urlencoded",
		dataType: "text",
		async: true,
		cache : false,
		timeout: 0,
		scriptCharset : 'utf-8',
		username : '',
		password : '',
		headers: {"X-Requested-With": "XMLHttpRequest"},
		beforeSend : function(){},
		error : Glacier.fn.ajaxErr
	};
	Glacier.fn.createStandardXHR=function(){
		try {
			return new window.XMLHttpRequest();
		} catch( e ) {}
	};
	Glacier.fn.createActiveXHR=function() {
		try {
			return new window.ActiveXObject( "Microsoft.XMLHTTP" );
		} catch( e ) {}
	};
	Glacier.fn.globalEval=function(data){
			if ( data && /\S/.test( data ) ) {
				( window.execScript || function( data ) {
					window[ "eval" ].call( window, data );
				} )( data );
			}
	};
	Glacier.fn.parseXML=function( data ) {
		var xml, tmp;
		try {
			if ( window.DOMParser ) {
				tmp = new DOMParser();
				xml = tmp.parseFromString( data , "text/xml" );
			} else {
				xml = new ActiveXObject( "Microsoft.XMLDOM" );
				xml.async = "false";
				xml.loadXML( data );
			}
		} catch( e ) {
			xml = undefined;
		}
		return xml;
	};
	Glacier.fn.parseHTML=function( data ) {
		return data;
	};
	Glacier.fn.parseJSON=function( data ) {
		try{
			if ( typeof data !== "string" || !data ) {
				return null;
			}
			data = Glacier.TrimTest( data );
			if ( window.JSON && window.JSON.parse ) {
				return window.JSON.parse( data );
			}
			if ( /^[\],:{}\s]*$/.test( data.replace( /\\(?:["\\\/bfnrt]|u[0-9a-fA-F]{4})/g, "@" )
				.replace( /"[^"\\\n\r]*"|true|false|null|-?\d+(?:\.\d*)?(?:[eE][+\-]?\d+)?/g, "]" )
				.replace( /(?:^|:|,)(?:\s*\[)+/g, "")) ) {
				return (new Function( "return " + data ))();
			}
		}catch(e){
		}
	};
	Glacier.fn.parseSCRIPT=function( data ) {
		Glacier.globalEval(data);
		return '';
	};
	Glacier.fn.getscript=function(s){
		var script,
				head = document.head || document.getElementsByTagName( "head" )[0] || document.documentElement;
		return {
				send: function( ) {
					script = document.createElement( "script" );
					script.async = "async";
					if ( s.scriptCharset ) {
						script.charset = s.scriptCharset;
					}
					script.src = s.url;
					if(!Glacier.isNaN(s.cache))
						script.src = s.url + ( /\?/.test( s.url ) ? "&" : "?" ) + "notcache=" + new Date().getTime();
					
					script.onload = script.onerror = script.onreadystatechange = function( _, isAbort) {
						if (!script.readyState || /loaded|complete/.test( script.readyState ) ) {
							script.onload = script.onreadystatechange = null;
							if ( head && script.parentNode ) {
								head.removeChild( script );
							}
							script = undefined;
							try{
								if ( !Glacier.isNaN(isAbort) ) {
									s.callback  && s.callback( 200, "abort" );
								}else
									s.callback  && s.callback();
							} catch (e) {s.callback  && s.callback();}
						}
					};
					if(Glacier.isIE9)script.onerror='';
					head.insertBefore( script, head.firstChild );
				},
				abort: function() {
					if ( script ) {
						script.onload( 0, 1 );
					}
				}
			};
	};
	Glacier.fn.getjs=function(o){
		o=o||{};
		var stime=function(a,b){
			if(b=='abort'){
			}else{
				clearTimeout(st);
			}
			o.callback && o.callback(a,b);
		},	scp=Glacier.getscript({
			 url:o.url,
			 callback:stime,
			 scriptCharset:o.charset,
			 cache:o.cache
			 })
		,	st=Glacier.setTimeout(scp.abort,(o.time||15))
		;
		
		scp.send();
		
	};
	Glacier.fn.readyState = function(o) {
		var readystate=o.xhr.readyState;
		if (readystate == 4) {	
			var status=o.xhr.status;
			if ( o.xhr.status === 1223 ) status = 204;
			if (status >= 200 && status < 300 || status == 304) {
				dataTypeFn={
					"XML"		:	Glacier.parseXML,
					"JSON"		:	Glacier.parseJSON,
					"TEXT"		:	window.String,
					"HTML"		:	Glacier.parseHTML,
					"SCRIPT"	:	Glacier.parseSCRIPT
				};
				o.success.call(o.xhr, dataTypeFn[o.dataType.toUpperCase()](o.xhr.responseText));
			} else {
				o.error.call(o.xhr);
			}
		} else if(readystate == 1){
			o.beforeSend && o.beforeSend.call(o.xhr);
			o.beforeSend='';
		}
	};
	Glacier.fn.ajax=function(url,o){
		if ( typeof url === "object" ) {
			o = url;
			url=o.url;
		}
		o = o || {};
		if(Glacier.isNaN(o.success)) return;
				
		o.xhr=window.ActiveXObject ?function(){
			return !Glacier.fn.ajaxSettings.isLocal && Glacier.createStandardXHR() || Glacier.createActiveXHR();
		}():Glacier.createStandardXHR();

		o.type = o.type || this.fn.ajaxSettings.type;
		o.timeout = o.timeout || this.fn.ajaxSettings.timeout;
		o.dataType = o.dataType || this.fn.ajaxSettings.dataType;
		o.beforeSend = o.beforeSend || this.fn.ajaxSettings.beforeSend;
		o.error = o.error || this.fn.ajaxSettings.error;
		o.async = o.async || this.fn.ajaxSettings.async;
		o.cache = o.cache || this.fn.ajaxSettings.cache;
		o.headers = o.headers || this.fn.ajaxSettings.headers;
		
		o.url=Glacier.filterUrl(url);
		if(o.async && (o.async==true || o.async=='true')) o.async=true;
		else o.async=false;
		o.data=(typeof o.data !== "string"?Glacier.param(o.data):o.data);
		if ( o.data && o.type.toUpperCase()==="GET")
			o.url += ( /\?/.test( o.url ) ? "&" : "?" ) + o.data;
		
		try {
			if(o.async){
				o.xhr.onreadystatechange = function() {
					Glacier.readyState(o);
				};
			}
			if(o.timeout>0){
				o.timeoutTimer = setTimeout( function(){
						o.xhr.abort();
						clearTimeout( o.timeoutTimer );
					}, o.timeout );
			}
			if (!o.cache) {
				o.url += ( /\?/.test( o.url ) ? "&" : "?" ) + "notcache=" + new Date().getTime(); 
			}
			o.xhr.open(o.type, o.url, o.async, o.username, o.password);
			if (o.type.toUpperCase() == "POST") {
				o.xhr.setRequestHeader("Content-Type",
					(o.headers["Content-Type"] || this.fn.ajaxSettings.contentType));
			}
			for (i in o.headers) {
				if(!o.headers.hasOwnProperty(i))continue;
				if (o.headers.hasOwnProperty(i)) {
					o.xhr.setRequestHeader(i, o.headers[i]);
				}
			}
			o.xhr.send(o.data);
			//if(!o.async)o.success(Glacier.ajax.contents(o),o);
		} catch (e) {}
		return o.xhr;
	};
	Glacier.fn.gethtml=function(url,success){
		if(Glacier.length<1)return;
		var obj=this;
		Glacier.ajax(url,{success:function(html){
			var gethtml=success?success(html):html;
			try{
				obj.html(gethtml);
				obj.val(gethtml);
			} catch (e) {}
		}});
		return this;
	};
	Glacier.fn.postdata=function(url,success){
		if(Glacier.length<1)return;
		var obj=this;
		Glacier.ajax(url,{success:function(html){
			var gethtml=success?success(html):html;
			try{
				obj.html(gethtml);
				obj.val(gethtml);
			} catch (e) {}
		}});
		return this;
	};
	Glacier.fn.get=function(url,success){
		return Glacier.ajax(url,{success:success});
	};
	Glacier.fn.post=function(url,data,success){
		return Glacier.ajax(url,{type:"POST",data:data,success:success});
	};
	/**
	* postfrom 将范围内的input textarea radio checkbox 以POST表单形式提交url
	* url:                      if(url) url=url else if(o.action) url=o.action else{/}
	* o.fromobj:window.document 表单内容取值范围
	* o.data:{}                 表单追加参数
	* o.i						限定要提交表单元素索引
	* o.notname					限定不要提交表单元素name
	* o.name					设定限定要提交表单元素名称
	* o.paramname				设定限定要提交表单元素名称(设置此处将会在强行将表单名称更改为paramname+i)
	* o.ck						默认不验证，如果等于TRUE为验证不为空，也可以传FUNCTION :当CK=true时，只对input和textarea这些有VALUE的验证为空
	* o.paramnullpass			是否开启无值不加入提交表单
	* o.display					是否开启不提交display=none的表单数据
	* o.msg			    		当ck=true时启用，默认值为TITLE,当TITLE为空时为"您输入的内容不能为空"
	* o.ckmsgnotall				当ck=true时启用，是否是全部为空内容一起提示
	* o.ckmsgfn					当ck=true时启用，为空默认为ALERT弹出所有错误提示，如果不为空，则将错误数据传入此方法
	* o.checkboxarray			表单中的checkbox是否已数组方式提交
	**/
	Glacier.fn.postfrom=function(url,o){
		if ( typeof url === "object" ) {
			o = url;
			url=o.action;
		}
		o = o || {};
		o.msg=o.msg || '您输入的内容不能为空';
		var msgobj=new Array(),
			dataArray=new Array(), 
			forobj = o.fromobj = o.fromobj || window.document,
			forobj_n=0,
			index_i = o.i?((','+o.i).toString()+','):false,
			index_name=o.notname?((','+o.notname).toString()+','):false,
			radioarray={},
			checkboxarray={},
			subobj=new Array();
			rcocarray={};
		if(o.fromobj && (typeof o.fromobj!="object") && o.fromobj.nodeType==1){
			return;
		}else{
			forobj=Glacier(o.fromobj);
			forobj_n=forobj.length;
		}
		for(var i=0;i<forobj_n;i++){
			var tmp_subobj=Glacier('input textarea select',forobj[i]);
			for(var k=0,n=tmp_subobj.length;k<n;k++){
				subobj.push(tmp_subobj[k]);
			}
		}
		o.ck = o.ck || 'ck';
		o.ck.msg=new Array();
		o.ck.ckend=false;
		var i=-1;
		for(var k=0,n=subobj.length;k<n;k++){
			i++;
			if(index_i && index_i.indexOf((','+k).toString()+',')<0)continue;
			var obj=subobj[k],val=Glacier(obj).val(),type=obj.type?obj.type.toLowerCase():"",isRadio=type.indexOf('radio'),isCheckbox=type.indexOf('checkbox');
			if(o.paramnullpass && Glacier.isNaN(val))continue;
			if(o.display){
				if(obj.style.display=='none' || Glacier.isNaN(obj.offsetWidth || o.clientWidth || o.offsetHeight || o.clientHeight))
				continue;
			}
			/**********************************************************************/
			if(!obj)continue;
			var objname=(o.name && o.name[i]?o.name[i]:'') || (o.paramname?(o.paramname+i):'') || (obj.name?obj.name:'');
			if(index_name && index_name.indexOf((','+objname).toString()+',')>=0)continue;
			if(!objname){alert('请设置'+i+'元素对象的NAME');return;}
			if((isRadio+isCheckbox)>=-1){
				var nametype='';
				if(isRadio>=0){
					if(!radioarray[objname]) radioarray[objname]='';
					if(obj.checked)	radioarray[objname]=val;
					nametype='radio';
				}else if(isCheckbox>=0){
					if(!checkboxarray[objname]) checkboxarray[objname]=new Array();
					if(obj.checked)	checkboxarray[objname].push(val);
					nametype='checkbox';
				}
				if(!rcocarray[objname+nametype]) rcocarray[objname+nametype]=new Array();
				rcocarray[objname+nametype].push(obj);
				continue;
			}
			/****************************CK********************************************/
			if(typeof o.ck=="boolean"){
				if(Glacier.isNaN(val)){
					msgobj.push(obj.title?obj.title:o.msg);
					try{obj.focus();} catch (e) {}
					if(o.ckmsgnotall) break;

				}
			}else if(Glacier.isFun(o.ck)){
				var isTrueNext=o.ck.call(o.ck,obj);
				if(Glacier.isString(isTrueNext) && isTrueNext=='next'){
					continue;
				}
				if(typeof isTrueNext=="boolean" && !isTrueNext){
					dataArray.length=0;
					break;
				}
			}
			/****************************注意后面加单选与多选******************************************/
			dataArray.push(encodeURIComponent(objname)+'='+encodeURIComponent(val));
		}
		var ckrc=function(k,v,type){
			var obj=rcocarray[k+type];
			if(o.paramnullpass && Glacier.isNaN(v)) return true;
			if(typeof o.ck=="boolean"){
				if(Glacier.isNaN(v)){
					msgobj.push(obj && obj.length>0 &&obj[0].title?obj[0].title:o.msg);
					return false;
				}
			}else if(Glacier.isFun(o.ck)){
				var isTrueNext=o.ck.call(o.ck,obj);
				if(Glacier.isString(isTrueNext) && isTrueNext=='next'){
					return true;
				}
				if(typeof isTrueNext=="boolean" && !isTrueNext){
					dataArray.length=0;
					return false;
				}
			}
			if(o.checkboxarray && type=='checkbox'){
				Glacier.Each(function(v){
								   dataArray.push(encodeURIComponent(k)+'='+encodeURIComponent((this+'').toString()));
								   },v);
			}else{
					dataArray.push(encodeURIComponent(k)+'='+encodeURIComponent((v+'').toString()));
			}
			return true;
		};
		for(var k in radioarray){
			if(!radioarray.hasOwnProperty(k))continue;
			var v=radioarray[k];
			if(!ckrc(k,v,'radio') && o.ckmsgnotall) break;
		}
		if(o.ckmsgnotall && msgobj.length>0){}
		else{
		for(var k in checkboxarray){
			if(!checkboxarray.hasOwnProperty(k))continue;
			var v=checkboxarray[k];
			if(!ckrc(k,v,'checkbox') && o.ckmsgnotall) break;
		}}
		if(Glacier.isFun(o.ck)){
			o.ck.ckend=true;
			if(!o.ck.call(o.ck)) dataArray.length=0;
		}
		if(msgobj.length>0){
			o.ckmsgfn?o.ckmsgfn(msgobj):alert(msgobj.join('\r\n'));
			return;
		}else if(dataArray.length==0) return;
		var datatmp=dataArray.join( "&" ).replace( /%20/g , "+" );
		var adddata=function(subdata,data){
			var param='';
			if(subdata) param='&';
			if(data)
				return subdata+param+(typeof data !== "string"?Glacier.param(data):data);
			else return '';
		};
		
		o.data=adddata(datatmp,o.data) || datatmp;
		o.url=url;
		o.type='post';
		Glacier.ajax(o);	
		return o.data;
	};
	/**
	* bdDom 					要绑定的对象
	* type 						选择的元素范围，默认为input textarea select
	* o.bdDom:window.document   表单内容取值范围
	* o.ischecked				当ischecked=true时，集合对象中的所有radio、checkbox、select将不会被赋值，而是设置 默认为false
	* o.bdtype				    表单内容绑定标准 默认NAME
	* o.json				    表单要附值的JSON数据
	* o.i						限定要提交表单元素索引
	* o.notname					限定不要提交表单元素name
	* o.notid					限定不要提交表单元素id
	* o.callbck					执行附值前回调的方法
	**/
	Glacier.fn.bdJsonByDom=function(o){
		o = o || {};
		o.type=o.type||'input textarea select';
		o.bdtype=o.bdtype||'name';
		if(Glacier.isNaN(o.json)){
			alert('请提供要绑定的JSON数据');
			return false;
		}
		var bdDom = o.bdDom = o.bdDom || window.document
		,	index_i = o.i?((','+o.i).toString()+','):false
		,	index_name=o.notname?((','+o.notname).toString()+','):false
		,	index_id=o.notid?((','+o.notid).toString()+','):false
		,	bdDom_n=0
		,	domList=new Array()
		,	jsonList=new Array()
		;
		if(o.bdDom && (typeof o.bdDom!="object") && o.bdDom.nodeType==1){
			return;
		}else{
			bdDom=Glacier(o.bdDom);
			bdDom_n=bdDom.length;
		}
		
		for(var i=0;i<bdDom_n;i++){
			var tmp_domList=Glacier(o.type,bdDom[i]);
			for(var k=0,n=tmp_domList.length;k<n;k++){
				domList.push(tmp_domList[k]);
			}
		}
		if(o.bdtype=='index'){
			for(var a in o.json){
				if(!o.json.hasOwnProperty(a))continue;
				jsonList.push(o.json[a]);
			}
		}
		for(var k=0,n=domList.length;k<n;k++){
			var obj=domList[k]
			,	g_obj=Glacier(obj)
			,	type=obj.type?obj.type.toLowerCase():""
			,	tagName=obj.tagName?obj.tagName.toLowerCase():""
			,	objname=(obj.name||'')
			,	objid=(obj.id||'')
			,	getName=function(){
					return objname;
				}
			,	getId=function(){
					return objid;
				}
			,	getI=function(){
					return k;
				}
			,	bdVal={
						"input"					:	function(){obj.value=this.val},
						"select"				:	function(){g_obj.addsel((this.text||this.val),this.val)},
						"textarea"				:	function(){g_obj.test(this.val)},
						""						:	""
					}




			,	checkedType={
						"radio"					:	function(){
														if(obj.value==this.val)
															obj.checked=true;
														else
															obj.checked=false;
													},
						"checkbox"				:	function(){
														if(((','+this.val).toString().split(',')+',').indexOf((','+obj.value).toString()+',')>=0)
															obj.checked=true;
														else
															obj.checked=false;
													},
						"select-one"			:	function(){
														g_obj.setselvalue(this.val);
													},
						"select-multiple"		:	function(){
														g_obj.setselvalue(this.val);
													},


						""						:	""
					}
			,	jsonVal={
						"name"			:	o.json[getName()],
						"id"			:	o.json[getId()],
						"index"			:	jsonList[getI()],
						""				:	""
					}
			,	json=jsonVal[o.bdtype]
			,	fn_cb_this={
						obj:obj,
						isNext:true,
						type:type,
						tagName:tagName,
						i:k,
						json:o.json,
						val:json
					}
			;
			if(index_i && index_i.indexOf((','+k).toString()+',')<0)continue;
			if(index_name && index_name.indexOf((','+objname).toString()+',')>=0)continue;
			if(index_id && index_id.indexOf((','+objid).toString()+',')>=0)continue;
			o.callbak && o.callbak.call(fn_cb_this,o.json);
			if(!fn_cb_this.isNext)continue;
			var tmp_fn=bdVal[tagName] || function(){g_obj.html(this.val)};
			if(o.ischecked) tmp_fn=checkedType[type]||tmp_fn;
			if(!Glacier.isNumber(fn_cb_this.val) && Glacier.isNaN(fn_cb_this.val)) fn_cb_this.val='';
			tmp_fn.call(fn_cb_this);
		}
		
		return ;
	};
	/***********************************AJAX的实现结束**************************************************/
	/***********************************js cookie的开始**************************************************/
	Glacier.fn.jscookie=function(c){
		c = c || {};
		var cookieDomain  =  c.domain || window.document.domain,
			cookieDays    =  c.day || 30,//此 cookie 将被保存 30 天
			cookieDir	  =  c.path || "/";//Cookie路径
		return {
			type:'script',
			obj:this,
			set:function(cookieName,cookieValue){
				var expires = new Date(); //new Date("December 31, 9998");
				expires.setTime(expires.getTime() + cookieDays*24*60*60*1000);
				this.setCookie(cookieName,cookieValue,expires);
			},
			setCookie:function(cookieName,cookieValue,timeExpires){
				document.cookie = escape(cookieName)+"="+escape(cookieValue)+" ;domain="+cookieDomain+" ;path="+cookieDir+" ;expires=" + timeExpires.toGMTString();
			},
			get:function(cookieName){
				var cookieValue = document.cookie.match(new RegExp("(^| )"+escape(cookieName)+"=([^;]*)(;|$)"));
				if(cookieValue != null)
					return unescape(cookieValue[2]);
				return null;
			},
			del:function(cookieName){
				var cookieValue=this.get(cookieName);
				var expires = new Date();
				expires.setTime(expires.getTime() - 1);
				this.setCookie(cookieName,cookieValue,expires);
			},
			delAll:function(){
				var expires = new Date();
				expires.setTime(expires.getTime() - 1);
				var cookieString = document.cookie.split('; ');
				for(var i=0,n=cookieString.length;i<n;i++){
					var tmp=cookieString[i].split("=");
					this.setCookie(tmp[0].toString().replace(/\s/g,''),tmp[1].toString().replace(/\s/g,''),expires);
				}
			},
			iscookies:function(cookieName){
				return this.get(cookieName)?true:false;
			}
		}
	};
	/***********************************js cookie的结束**************************************************/
	/***********************************flash cookie的开始**************************************************/
	Glacier.fn.thisMovie=function(movieName){
		if(Glacier.isIE)
			return window[movieName];
		else
			return document[movieName];
	};
	Glacier.fn.flashcookie=function(o){
		o = o || {};
		var cookieObj	  =  ''
		,	span=Glacier.dom({type:'span'})
		//,	src			  =  o.src || "/cookie.swf"//flashCookie.swf路径
		,	name		  =  o.name || ""
		,	day			  =  o.day  || ""
		,	callback	  =  o.callback || function(){}
		,	flashID	      = 'flashcookie'+span.dom.id
		,	flashCo=''
		,	isCallback	  =  false
		;
			this.CookiesInit=function(fck){
				isCallback=true;
				Glacier.CookiesInit=function(){
					flashCo=Glacier.thisMovie(flashID);
					if(!Glacier.isNaN(name))
						callback && callback.call(obj,'',2);
				};
				var obj={
							type		:	'flash',
							id			:	flashID,
							obj			:	function(){
												return flashCo;
											},
							set			:	function(cookieName,cookieValue){
								this.obj().setCookies(escape(cookieName),escape(cookieValue));
							},
							get			:	function(cookieName){
								return unescape(this.obj().getCookies(escape(cookieName)))||'';
							},
							del			:	function(cookieName){
								this.obj().delCookies(escape(cookieName));
							},
							uuid		:	function(cookieName){
								return this.obj().getOnlyID(cookieName);
							},
							delAll		:	function(){
								this.obj().cls();
							},
							iscookies	:	function(cookieName){
								return this.obj().isCookies(escape(cookieName))||'';
							}
						}
				;
				if(Glacier.isNaN(name)){
					callback && callback.call(obj,fck.fcookie,1);
				}
				
			};
			span.add();
			span.dom.innerHTML=Glacier.getFlash.cookies({
				id:flashID
			,	data:{
					name:name
				,	key:'QQ113666386'
				}
			});
			setTimeout(function(){
				if(!isCallback)callback && callback.call(this,'',2);
			},2000);
	};
	/***********************************flash cookie的结束**************************************************/
	/**************************************** cookie的开始**************************************************/
	Glacier.fn.cookiesetup={'isSetup':false,'js':'','flash':''};
	Glacier.fn.cookie=function(o){
		o = o || {};
		var jscookie=Glacier.jscookie(o)
		,ck_callbak=''
		,ck_cookieName=''
		,fn=function(s){
			try{
				if(!Glacier.isNaN(o.name))
					s=Glacier.cookiesetup['flash'].get(ck_cookieName)||'';
				if(!Glacier.isNaN(s))jscookie.set(ck_cookieName,s);
			}catch(e){}
			ck_callbak && ck_callbak.call(this,s);
		};
		
		return {
			ckget:function(cookieName,callbak){
				if(Glacier.isNaN(callbak)) return;
				ck_callbak=callbak;
				ck_cookieName=cookieName;
				var jsgetvalue=jscookie.get(cookieName);
				if(Glacier.isNaN(jsgetvalue)){
					o.callbak=fn;
					Glacier.cookieinit(o,cookieName);
				}else fn(jsgetvalue);
			},
			set:function(cookieName,cookieValue){
				if(jscookie)
					jscookie.set(cookieName,cookieValue);
				if(Glacier.cookiesetup['flash'])
					Glacier.cookiesetup['flash'].set(cookieName,cookieValue);				
				return cookieValue;
			},
			get:function(cookieName){
				return jscookie.get(cookieName) || Glacier.cookiesetup['flash'].get(cookieName);
			},
			del:function(cookieName){
				if(jscookie)
					jscookie.del(cookieName);
				if(Glacier.cookiesetup['flash'])
					Glacier.cookiesetup['flash'].del(cookieName);
			},
			delAll:function(){
				if(jscookie)
					jscookie.delAll();
				if(Glacier.cookiesetup['flash'])
					Glacier.cookiesetup['flash'].delAll();
			},
			iscookies:function(cookieName){
				return jscookie.iscookies(cookieName) || Glacier.cookiesetup['flash'].iscookies(cookieName);
			}
		}
		
	};
	Glacier.fn.cookieinit=function(o){
		o = o || {};
		var fn=function(fCookieVal,k){
			if((k==1 && Glacier.isNaN(fCookieVal)))
				return;
			if(Glacier.cookiesetup['isSetup']) return;
			if(!Glacier.isNaN(this)) Glacier.cookiesetup['flash']=this;
			Glacier.cookiesetup['isSetup']=true;
			o.callbak && o.callbak(fCookieVal);
		};
		this.flashcookie({name:o.name,src:o.src,callback:fn});
	};
	
	/**************************************** cookie的结束**************************************************/
	/***********************************鼠标提示    开始**************************************************/
	Glacier.fn.tip=function(o){
		var div = document.createElement("span");
		div.style.border="2px outset #ddd";
		div.style.padding="2px";
		div.style.backgroundColor="#eee";
		div.style.color="#000";
		o = o || {};
		if(o.css) div.style.cssText=o.css;
		div.style.position="absolute";
		div.style.display="none";
		div.style.width="auto";
		div.style.height="auto";
		div.style.zIndex=90000;
		div.style.left="-1000px";
		div.style.top="-1000px";
		document.body.appendChild(div);
		return {
			mouseover:'',
			ouseout:'',
			open:function(){
					this.mouseover=Glacier.closure({obj:this,callBack:this.start});
					Glacier.AttachEvent(document,"mouseover",this.mouseover);
					

					this.ouseout=Glacier.closure({obj:this,callBack:this.end});
					Glacier.AttachEvent(document,"mouseout",this.ouseout);
				},
			start:function(o,et,oEvent){
					var obj=Glacier.Obj(et)
					,_x=obj.x+obj.w
					,_y=obj.y+obj.h
					,wsize=Glacier.windowSize()
					,bw=wsize.scrollLeft
					,bh=wsize.scrollTop
					;
					obj.tip=obj.title||obj.alt||obj.tip;
					if(Glacier.isNaN(obj.tip)) return;
					div.innerHTML=obj.tip;
					div.style.display=obj.title=obj.alt="";
					
					var tipobj=Glacier.Obj(div);
					var _x=bw+_x,ckL=bw+wsize.sw-tipobj.w;
					var _y=bh+_y,ckT=bh+wsize.sh-tipobj.h;
					if((!Glacier.compatMode && Glacier.isIE) || Glacier.isIE6){
						_x=obj.x+obj.w;
						_y=obj.y+obj.h;
					}
					if(_x>ckL) _x=_y>ckT?(_x-obj.w-tipobj.w):ckL;
					if(_y>ckT) _y=ckT;
					if(_x<(bw+0)) _x=bw+0;
					if(_y<(bh+0)) _y=bh+0;
					
					div.style.left=_x+"px";
					div.style.top=_y+"px";
				},
			end:function(o,et,oEvent){
					div.style.display="none";
					div.style.left="-1000px";
					div.style.top="-1000px";
				},
			close:function(){
					Glacier.RemoveAttachEvent(document, "mouseover",this.mouseover);
					Glacier.RemoveAttachEvent(document, "mouseout",this.ouseout);
				}
			};
	};
	/***********************************鼠标提示    结束**************************************************/
	Glacier.fn.statistics=function(){
		var ref=function(){
			try{
				if(document.referrer) return document.referrer+'';
				else if(window.opener) return window.opener.location+'';
			}catch(e){}
			return '';
		}();
		if(Glacier.isNaN(ref))ref='';
		return {
			fromPage:ref,
			toPage:window.location+'',
			toPageTitle:encodeURIComponent(window.document.title) || this.toPage,
			//userAgent:navigator.userAgent,
			platform:navigator.platform+'',
			browser:Glacier.getBrowser,
			bVersion:Glacier.getBrowserVersion,
			ufcolor:(navigator.appName=="Netscape"?ufcolor=screen.pixelDepth:ufcolor=screen.colorDepth)+'',
			screensize:screen.width+'.'+screen.height,
			language:(navigator.language?navigator.language:navigator.browserLanguage)+'',
			timeZone:((new Date().getTimezoneOffset()/60)*(-1))+''
			};
	};
	Glacier.fn.uuid=function(len, radix){
		var chars = Glacier.fn.Exp.CHARS, uuid = [], i;
		radix = radix || chars.length;
		if(len){
			for (i = 0; i < len; i++) uuid[i] = chars[0 | Math.random()*radix];
		}else{
			var r;
			uuid[8] = uuid[13] = uuid[18] = uuid[23] = '-';
			uuid[14] = '4';
			for(i=0;i<36;i++){
				if(!uuid[i]){
					r = 0 | Math.random()*16;
					uuid[i] = chars[(i == 19) ? (r & 0x3) | 0x8 : r];
				}
			}
		}
		return uuid.join('');
	};
	Glacier.fn.uuidFast=function(){
		 var chars=Glacier.fn.Exp.CHARS, uuid = new Array(36), rnd=0, r;
		 for (var i = 0; i < 36; i++) {
			 if (i==8 || i==13 ||  i==18 || i==23) {
				 uuid[i] = '-';
			} else if (i==14) {
				uuid[i] = '4';
			} else {
				if (rnd <= 0x02) rnd = 0x2000000 + (Math.random()*0x1000000)|0;
				r = rnd & 0xf;
				rnd = rnd >> 4;
				uuid[i] = chars[(i == 19) ? (r & 0x3) | 0x8 : r];
			}
		}
		return uuid.join('');   
	};
	Glacier.fn.uuidCompact=function(){
		return 'xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx'.replace(/[xy]/g, function(c) {
																				var r = Math.random()*16|0, v = c == 'x' ? r : (r&0x3|0x8);
																				return v.toString(16);
																				});
	};
	Glacier.fn.setTimeout=function(fn,time){
		var tmpfn=fn;
		time=Glacier.isNumber(time)?time:0;
		if(Glacier.isFun(fn)){
		  var argu = Array.prototype.slice.call(arguments,2);
		  tmpfn = function(){
			  try{
				fn.apply(null, argu);
			  }catch(e){}
		  };
		}
		if(parseInt(time)==0) time=0.001;
		return Timeout(tmpfn,(time*1000));
	};
	Glacier.fn.setInterval=function(fn,time){
		time = parseInt(time)<=0?0.001:time;
		var clsInterval		=	''
		,	tmpfn			=	fn
		,	tmpstop			=	false
		,	i				=	0
		,	n				=	0
		,	nfn				=	function(){
									if(n>0 && i>n) tmpstop=true;
								}
		,	retfn			=	{
										start	:	function(){
														clearInterval(clsInterval);
														clsInterval=tmpfn?Interval(tmpfn,(time*1000)):'';
														return retfn;
													},
										stop	:	function(o){
														o = o || {n:1};
														if(o.n)
															n=Glacier.isNumber(o.n)?parseInt(o.n):1;
														if(o.time){
															var callfn=function(){
																						this.obj.stop();
																					}
															,	t=Glacier.isNumber(o.time)?o.time:1
															;
															
															Glacier.setInterval(callfn,t).start();
														}
														return retfn;
													}
									}
		;
		time=Glacier.isNumber(time)?parseInt(time):0;
		if(Glacier.isFun(fn)){
			  var argu = Array.prototype.slice.call(arguments,2);
			  tmpfn = function(){
				  try{
					fn.apply({obj:retfn,i:i}, argu);
					i++; nfn();
					tmpstop && clearInterval(clsInterval);
				  }catch(e){
					  tmpfn='';
					  clearInterval(clsInterval);
				  }
			  };
		}
		
		return retfn;
	};
	Glacier.fn.animation=function(o){
		o 					=		o || {};
		if(Glacier.isNaN(o.obj)) return;
		var obj_init			=	Glacier.Obj(o.obj)
		,	obj					=	o.obj
		,	init				=	{
										obj				:	obj,
										len				:	1,
										speed			:	0,
										callbck			:	function(s,fn,callbak){
															if(!s || !s.x) return;
															var n=s.x.length;
															n--;
															if(n<0){
																callbak && callbak(fn);
																return;
															}
															fn.obj.style.left=(fn.obj.x+s.x[n])+'px';
															fn.obj.style.top =(fn.obj.y+s.y[n])+'px';
															s.x.length=s.y.length=n;
															Glacier.setTimeout(fn.callbck,fn.speed,s,fn,callbak);
										},
										sline			:	function(p){
																p				=		p		||		{};
																p.callbak		=		p.callbak	||		'';
																p.x				=		p.x;
																p.y				=		p.y;
																p.obj			=		p.obj	||		'';
																var x			=		p.x-this.obj.x
																,	y			=		p.y-this.obj.y
																,	init		=		straightline(x,y,this.len)
																;
																this.callbck(init,this,p.callbak);
															
															
										},
										shake			:	function(p){
																p				=		p			||		{};
																p.callbak		=		p.callbak	||		'';
																p.l				=		p.l			||		6;
																p.n				=		p.n			||		3;
																p.isx			=		p.isx				;
																var init		=		vibration(p,this)
																;
																this.speed=0.02;
																this.callbck(init,this,p.callbak);
										},
									'':''}
		,	straightline			=	function(x,y,l){
											var _x=Math.abs(x)
											,	_y=Math.abs(y)
											,	arrayX=new Array()
											,	arrayY=new Array()
											,	L=l>0?l:1
											,	n=0
											;
											if(_x>_y){
												n=_x/L;
												
											}else{
												n=_y/L;
											}
											
											var	x_l=x/n
											,	y_l=y/n
											;
											for(var i=0;i<n;i++){
												arrayX.push(x_l*(n-i));
												arrayY.push(y_l*(n-i));
											}
											return {
												x:arrayX,
												y:arrayY,
											'':''};
		}
		,	vibration				=	function(p,obj){
											var obj_init = Glacier.Obj(obj.obj);
											obj.obj.x=obj_init.x;
											obj.obj.y=obj_init.y;
											var arrayX=new Array()
											,	arrayY=new Array();
											for(var i=0,nc=(p.n*2);i<nc;i++){
												if(p.isx){
													arrayX.push(0);
													arrayY.push(p.l/(nc-i));
													arrayX.push(0);
													arrayY.push(-(p.l/(nc-i)));
												}else{
													arrayX.push(p.l/(nc-i));
													arrayY.push(0);
													arrayX.push(-(p.l/(nc-i)));
													arrayY.push(0);
												}
											}
											arrayX.push(0);
											arrayY.push(0);
											return {
												x:arrayX,
												y:arrayY,
											'':''};
		}
		;
		obj.x=obj_init.x;
		obj.y=obj_init.y;
		obj.w=obj_init.w;
		obj.h=obj_init.h;
		return init;
		
	};
	Glacier.fn.getColor=function(o){
		o 					=		o || {};
		o.obj = Glacier.isString(o.obj)?Glacier('#'+o.obj)[0]:o.obj;
		if(!Glacier.isObj(o.obj)) return;
		o.h = [];
		o.h[0] = "FF";
		o.h[1] = "CC";
		o.h[2] = "99";

		o.h[3] = "66";
		o.h[4] = "33";
		o.h[5] = "00";
		o.on=o.on||'click';
		function init(){
			o.showobj=Glacier('#'+o.id).length>0?Glacier('#'+o.id)[0]:'';
			if((!Glacier.isNaN(o.id) && !Glacier.isObj(Glacier('#'+o.id)[0])) || Glacier.isNaN(o.id)){
				var wsize=Glacier.windowSize();
				var lockdiv=Glacier.dom({type:'div'});
				lockdiv.setstyle({style:"display:none;width:"+wsize.w+"px;height:"+wsize.h+"px;z-index:"+Glacier.Zindex()+";position:absolute;top: 0px;left: 0px;height:100%;width:100%;background:#ffffff;filter:alpha(opacity=0);-moz-opacity: 0;opacity: 0;"});
				lockdiv.add();
				
				var div=Glacier.dom({type:'div',id:(o.id||'')});
				div.setstyle({style:"display:none;z-index:"+Glacier.Zindex()+";position:absolute;width:235px; padding:0 0 1px 0; background:#fff; overflow:hidden; margin-bottom:30px;"});
				div.add();
				o.showobj=div.dom;
				o.showobj.lockobj=lockdiv.dom;
				
				Glacier(lockdiv.dom).bd({on:'click',callback:function(){
					if(!o.on_obj){
						o.on_obj={};
						o.on_obj.showobj=div.dom;
						o.on_obj.showobj.lockobj=lockdiv.dom;
					}
					hide(o.on_obj.showobj);
				}});
			}
		}
		init();
		if(!o.showobj) return;
		var	show=function(obj){
					obj.style.display = "";
					obj.lockobj.style.display='';
					obj.innerHTML = "";
				}
		,	hide=function(obj){
					obj.style.display = "none";
					obj.lockobj.style.display='none';
				}
		,	getRank=function(){// 组合出216种不同的颜色参数
				for(var r=0; r<6; r++){
					var _ul=Glacier.dom({type:'ul'});
					_ul.setstyle({style:'width:78px; float:left; display:inline; background:#fff; overflow:hidden; margin: 0px; padding: 0px;'});
					for(var g=0; g<6; g++){
						for(var b=0; b<6; b++){
							getCube(o.h[r], o.h[g], o.h[b], _ul.dom);
						};
					};
					_ul.add(o.showobj);
				};
			}
		,	getCube=function(R, G, B, _ul){// 创建颜色小方块
				var _li = Glacier.dom({type:'li'});
				var _a = Glacier.dom({type:'a'});
				_li.setstyle({title:("#"+ R + G + B),style:'float:left; display:inline; width:12px; height:12px; margin:1px 0 0 1px; background:#808080;'});
				_a.setstyle({style:'background:#'+(R + G + B)+';display:block; margin:1px 0 0 1px; width:11px; height:11px; overflow:hidden;'});
				_a.add(_li.dom);
				_li.add(_ul);
				Glacier(_li.dom).bd({on:'click',callback:function(){
					hide(_ul.parentNode);
					o.callback.call(this,R + G + B,o.on_obj)
				}});
			}
		;
		var tmpobj=Glacier(o.obj);
		tmpobj[0].showobj=o.showobj;
		tmpobj.bd({on:o.on,callback:function(){
			o.on_obj=this;
			var obj=Glacier.Obj(o.on_obj)
			,	_x=obj.x
			,	_y=obj.y+obj.h
			,	wsize=Glacier.windowSize()
			,	p_w=235
			,	p_h=165
			;
			if((wsize.sh-_y)<p_h) _y=wsize.sh-p_h;
			if((wsize.sw-_x)<p_w) _x=wsize.sw-p_w;
			if(_y<1)_y=0;
			if(_x<1)_x=0;
			this.showobj.style.top=_y+'px';
			this.showobj.style.left=_x+'px';
			show(this.showobj);
         	getRank();
		}});
	};
	Glacier.fn.setupGetColor=function(o){
		o 					=		o || {};
		o.isOne=o.isOne?Glacier.uuidFast().replace(/-/ig,''):'';
		Glacier.Each(
			function(i){
				Glacier.getColor({
					 id:o.isOne,
					 obj:this.toString(),
					 on:o.on,
					 callback:o.callback
				 });
			}
			,o.ids.split(",")
		);
	};
	Glacier.fn.Dropshadow=function(o){
		o 					=		o || {};
		var css_filter=new Array()
		,	css_textShadow=new Array()
		,	RGB=this.RGB
		;
		Glacier.Each(
			function(i){
				if(Glacier.isNaN(RGB)) RGB=this[2];
				css_filter.push('Dropshadow(offx='+this[0]+',offy='+this[1]+',color='+RGB+')');
				css_textShadow.push(RGB+' '+this[0]+'px '+this[1]+'px 0 ');
			}
			,o
		);
		return 'filter:'+css_filter.join(' ')+';text-shadow:'+css_textShadow.join(',')+';';
	};
	Glacier.fn.domload=function(o){
		o = o || {};
		var obj=Glacier(o.obj)[0]
		,	list=Glacier((o.find || 'img object iframe'),obj)
		,	n=list.length
		,	ckn=1
		,	onfn=function(cbing,cb){
				ckn>0&&ckn++;
				if(Glacier.isFun(cbing)) cbing.call(this,ckn,n,n<ckn);
				if(n<ckn){
					ckn=n=-1;
					ckn--;
					this.onerror=this.onload='';
					cb && cb.call(obj);
				}
			}
		,	fn=function(i){
				var domObj=list[i];
				try{
					if(domObj.readyState=='complete' || domObj.complete){
						onfn.call(domObj,o.callbaking,o.callbak);
					}else{
						domObj.onload=function(){
							onfn.call(domObj,o.callbaking,o.callbak);
						};
						domObj.onerror=function(){
							onfn.call(domObj,o.callbakErring,o.callbak);
						}
					}
				}catch(e){
					onfn.call(domObj,o.callbaking,o.callbak);
				}
			}
		,	start=function(){
				if(n==0){
					o.callbak && o.callbak();
				}
				for(var i=0;i<n;i++)fn(i);
			}()
		;
		
	};
	Glacier.fn.autoimg=function(maxWidth,maxHeight,objImg){
		var img = new Image();
		img.src = objImg.src;
		var hRatio;
		var wRatio;
		var Ratio = 1;
		var w = img.width;
		var h = img.height;
		wRatio = maxWidth / w;
		hRatio = maxHeight / h;
		if (maxWidth ==0 && maxHeight==0){
		Ratio = 1;
		}else if (maxWidth==0){//
		if (hRatio<1) Ratio = hRatio;
		}else if (maxHeight==0){
		if (wRatio<1) Ratio = wRatio;
		}else if (wRatio<1 || hRatio<1){
		Ratio = (wRatio<=hRatio?wRatio:hRatio);
		}
		if (Ratio<1){
		w = w * Ratio;
		h = h * Ratio;
		}
		objImg.height = h;
		objImg.width = w;
	};
	Glacier.fn.loadscroll=function(o){
		o=o||{};
		if(Glacier.isNaN(o.obj)) return;
		var obj=o.obj
		,	mouseScroll=Glacier.isIE?'mousewheel':'DOMMouseScroll'
		,	loadh=o.loadh||10
		,	p=1
		,	objIni={}
		,	isOne=false
		,	ckLoad=function(){
				window.document.title=obj.scrollHeight+'='+obj.scrollTop+'|'+((obj.scrollHeight-obj.scrollTop-objIni.h)<loadh);
				return (obj.scrollHeight-obj.scrollTop-objIni.h)<loadh;
			}
		,	bdOn=function(){
				if(!ckLoad())
					isOne=true;
				Glacier.AttachEvent(obj,"scroll",fn);
				Glacier.AttachEvent(obj,mouseScroll,fn);
			}
		,	delBdOn=function(){
				Glacier.RemoveAttachEvent(obj,"scroll",fn);
				Glacier.RemoveAttachEvent(obj,mouseScroll,fn);
			}
		,	fn=function(){
				if(ckLoad() && isOne){
					isOne=false;
					delBdOn();
					o.callback && o.callback(p);
				}
			}
		,	start=function(){
				p=1;
				objIni=Glacier.Obj(obj);
				Glacier.AttachEvent(obj,"mouseover",bdOn);
				Glacier.AttachEvent(obj,"mouseout",delBdOn);
			}
		,	go=function(){
				var th=obj.scrollHeight-objIni.h;
				if(th<loadh)
					loadh=th;
				if(ckLoad())
					obj.scrollTop=obj.scrollHeight-loadh-objIni.h-1;
				p++;
				bdOn();
			}
		,	stop=function(){
				delBdOn();
				Glacier.RemoveAttachEvent(obj,"mouseover",bdOn);
				Glacier.RemoveAttachEvent(obj,"mouseout",delBdOn);
			}
		;
		
		return {
				start	:	start,
				go		:	go,
				stop	:	stop,
				''		:	''
			};
	};
	Glacier.fn.png=function(obj){
		if(!Glacier.isIE6)return obj;
		obj=obj||window.document;
		Glacier.Each(
				function(i){
					var bg = this.currentStyle.backgroundImage;
					if (bg && bg.match(/.PNG|.png/i) != null){
						var mypng = bg.substring(5,bg.length-2);
						this.style.filter = "progid:DXImageTransform.Microsoft.AlphaImageLoader(src='"+mypng+"',enabled=true, sizingMethod=scale)";
						this.style.backgroundImage = "url('')";
						this.style.background = "none";
					}
				}
				,obj.getElementsByTagName('*')
		);
		Glacier.Each(function(i){
					var src = (this.src||'').toUpperCase();
					if (src.substring(src.length-3, src.length) == "PNG")
					{
						var imgID = (this.id) ? "id='" + this.id + "' " : "";
						var imgClass = (this.className) ? "class='" + this.className + "' " : "";
						var imgTitle = (this.title) ? "title='" + this.title + "' " : "title='" + this.alt + "' ";
						var imgStyle = "display:inline-block;" + this.style.cssText;
						if (this.align == "left") imgStyle = "float:left;" + imgStyle;
						if (this.align == "right") imgStyle = "float:right;" + imgStyle;
						if (this.parentElement.href) imgStyle = "cursor:hand;" + imgStyle;
						this.outerHTML = "<span " + imgID + imgClass + imgTitle
						+ " style=\"" + "width:" + this.width + "px; height:" + this.height + "px;" + imgStyle + ";"
						+ "filter:progid:DXImageTransform.Microsoft.AlphaImageLoader"
						+ "(src=\'" + this.src + "\', sizingMethod='scale');\"></span>";
					}
				}
				,Glacier('img',obj)
		);
		return obj;
	};
	Glacier.fn.mkflash=function(cabfn){
		var fn=function(o){
				var str='<OBJECT id="'+(o.id||Glacier.uuidFast().replace(/-/ig,''))+'" classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000" width="'+(o.w||0)+'px" height="'+(o.h||0)+'px" ><PARAM NAME="movie" VALUE="'+o.src+'">';
				if(!Glacier.isIE) str='<object width="'+(o.w||0)+'px" height="'+(o.h||0)+'px" data="'+o.src+'" id="'+(o.id||Glacier.uuidFast().replace(/-/ig,''))+'" type="application/x-shockwave-flash">';
				o.data=(typeof o.data !== "string"?function(){
						return Glacier.formatparam({
								param		:	o.data
							,	separate	:	"&amp;"
							,	Handle		:	function(key,value){
									return value?key + "=" + value:'';
								}
						}).replace( /%20/g , "+" );
					}():o.data);
				str+='<param value="'+o.data+'" name="flashvars">';
				str+='<param value="always" name="allowScriptAccess">';
				str+='<param value="true" name="allowfullscreen"></object>';
				return str;
			}
		,	path='/static/web/flash/'
		;
		if(cabfn) return cabfn.call(this,{fn:fn,path:path});
	};
	Glacier.fn.getFlash=function(){
		var fn=function(){}
		,	path=''
		;
		Glacier.fn.mkflash(function(o){
			o=o||{};
			fn=o.fn;
			path=o.path;
		});
		return {
			time:function(ini){
				ini = ini||{};
				var o={};
				o.w=ini.two?402:200;
				o.h=180;
				o.data={
					isTwos	:ini.two
				,	time1	:ini.time1
				,	time2	:ini.time2
				,	mindate	:ini.mindate
				,	maxdate	:ini.maxdate
				,	fn		:ini.fnname
				,	wholemonth:ini.wholemonth
				};
				o.src=(ini.path||path)+'time.swf';
				return fn(o);
			}
		,	cookies:function(o){
				o=o||{};
				o.src=path+'cookie.swf';
				return fn(o);
			}
		};
	}();
	Glacier.fn.time=function(o){
		o=o||{};
		var fn=function(html){return html;}
		,	G_Obj=this
		,	objIni=''
		,	flashDiv=Glacier.dom({type:'div'})
		,	GDiv=Glacier(flashDiv.dom)
		,	w=o.two?402:200
		,	h=180
		,	isTrue=false
		;
		if(Glacier.isNaN(o.obj)){
			o.obj=G_Obj[0];
			fn=function(str){
				Glacier.insert.call(G_Obj,{fn:function(){
					flashDiv.setstyle({style:'display:none;width:'+w+'px;height:'+h+'px;'});
					flashDiv.add();
					Glacier.domload({
						obj:flashDiv.dom
					,	callbak:function(){
							objIni=Glacier.Obj(G_Obj[0]);
							isTrue=true;
						}
					});
					flashDiv.dom.innerHTML=str;
				}});
				return {
						show:function(){
							if(!isTrue)return false;
							GDiv.WinAlt({
								lock:true
							,	lock_back:'#fff'
							,	lock_opa:1
							,	lockfn:o.lockfn||function(){GDiv.display().none();}
							,	xLenPx:o.x||(objIni.x+objIni.w)
							,	yLenPx:o.y||(objIni.y+objIni.h)
							});
						}
					,	none:function(){
							GDiv.display().none();
						}
					};
			};
		}
		return fn(Glacier.getFlash.time(o));
	};
	Glacier.fn.insert=function(o){
		var insobj=this;
		o = o || {};
		if(o.obj){
			insobj[0]=o.obj;
		}
		Glacier.Each(function(){
			if(o.fn){
				o.fn.call(this);
			}else{
				if('input'==(this.nodeName.toLowerCase())){
					this.value=o.str;
				}else{
					this.innerHTML=o.str;
				}
			}
		}
		,insobj);
	};
	
	Glacier.fn.Each.call(Glacier.fn,function(i,fnname){
		Glacier[fnname]=Glacier.prototype[fnname];
	});	
	Glacier.Each(
		function(i){
			class2type[ "[object " + this + "]" ] = this.toLowerCase();
		}
		,"Boolean Number String Function Array Date RegExp Object".split(" ")
	);
	window.$G=Glacier;
	
})();