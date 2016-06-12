/*
* Name		: Glacier 1.0
* Author	: wangzheng
* QQ		: 113666386
* Date		: 2012-08-08
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
		if ($G.isFun(selector)){
			if(!dom)
				this.Loaded(selector);
			else
				this.LoadedDom(selector);
			return new Glacier();
		}
		if(!$G.isObj(dom)) dom=window.document;
		if(selector && (typeof selector=="object") && selector.nodeType==1){
			this[0]=selector;
			this.length=1;
			return this;
		}
		var listDoms=[],domsTag=[],doms,name=new Array();
		if(typeof selector=="string"){
			var str = (" "+selector+" ".replace(/\s{2}/," ")).match($G.fn.Exp.GlacierExpr);
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
						if(!$G.isNaN(findTag)){
							var tmpTag=(obj.getAttribute(findTag)+'').toLowerCase();
							if($G.isNaN(tmpTag)) continue;
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
			var isTag=($G.isIE || dom!=window.document);
			if(isTag)doms=dom.getElementsByTagName("*");
			for(var i=0,countName=name.length;i<countName;i++){
				if(!$G.isIE && dom==window.document) doms=dom.getElementsByName(name[i]);
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
	/***************************************************以上 为定义变量**********************************************************/
	Glacier.prototype.Exp={
		GlacierExpr:/[\.|#|\w-]+[:#\.\w-]*\s+/g,
		rlocalProtocol:'',
		CHARS : '0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz'.split('')
	};
	Glacier.prototype.Each=function(fun,obj){
		if($G.isNaN(obj)) obj=this;
		if(!$G.isArray(obj) && !$G.isNumber(obj.length)){
			var n=0;
			for(var i in obj){
				fun.call(obj[i],n,i,obj);
				n++;
			}
			return;
		}
		for(var i=0,l=obj.length; i<l; i++)
			fun.call(obj[i],i,obj);
	};
	Glacier.Each=function(o,fun){
		if (typeof fun == "function")
			Glacier.prototype.Each.call(o,fun);				
	};
	Glacier.prototype.InitObj=function(doms){
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
	Glacier.prototype.SetArray=function(arr){
		this.length=0;
		[].push.apply(this,arr);
		return this;
	};
	Glacier.prototype.getfirst=function(){
		return this[0];
	};
	Glacier.prototype.getlast=function(){
		return this[this.length-1];
	};
	Glacier.prototype.id=function(){
		var ids=new Array();
		this.Each(function(){
			ids.push(this.id ? this.id:'');
		});
		return ids;
	};
	Glacier.prototype.get=function(key){
		var getAttribute=new Array();
		this.Each(function(){
			if(this && this.getAttribute){
				var v=this.getAttribute(key);
				getAttribute.push(v ? v:'');
			}
		});
		return getAttribute.length==0?getAttribute[0]:getAttribute;
	};
	Glacier.prototype.radio=function(isChecked){
		if($G.isNaN(isChecked)){
			return this.val();;
		}
		var v=new Array()
		;
		this.Each(function(){
			if(this.checked==isChecked)
				v.push(this.value);
		});
		return v;
	};
	Glacier.prototype.html=function(newHtml){
		var htmls=new Array();
		this.Each(function(){
			if($G.isNaN(newHtml))
				htmls.push(this.innerHTML ? this.innerHTML:'');
			else{
				if($G.isIE)this.innerHTML=newHtml;
				else{
					var newObj=this.cloneNode(false);
					newObj.innerHTML=newHtml;
					this.parentNode.replaceChild(newObj,this);
				}
			}
		});
		return ($G.isNaN(newHtml))?htmls:this;
	};
	Glacier.prototype.val=function(newVal){
		var values=new Array();
		this.Each(function(){
			if($G.isNaN(newVal))
				values.push(this.value ? this.value:'');
			else
				this.value=newVal;
		});
		return ($G.isNaN(newVal))?values:this;
	};
	Glacier.prototype.test=function(newTest){
		var tests=new Array();
		this.Each(function(){
			if($G.isNaN(newTest))
				tests.push($G.isIE?(this.innerText ?this.innerText:''):(this.textContent ?this.textContent:(this.value?this.value:'')));
			else
				$G.isIE?(this.innerText=newTest):(this.textContent=newTest);
		});
		return ($G.isNaN(newTest))?tests:this;
	};
	Glacier.prototype.set=function(key,val){
		this.Each(function(){
			key = $G.isIE6 || $G.isIE7?(Glacier.dom.ATTRIBUTES[key] || key):key;
			this.setAttribute(key,val);
		});
		return this;
	};
	Glacier.prototype.style=function(c,type){
		var tmpdomtpe={};
		this.Each(function(){
			var tmpcss={}
			,	tmphtml=this.style.cssText?this.style.cssText+';'+c.style:c.style
			;
			for (k in c){
				var tpk=$G.dom.TYPENAME[k]||k;
				if(tpk=='style' && type=='update'){
					$G.Each(
						tmphtml.split(";"),
						function(){
							var stycss=this.split(":");
							if(!$G.isNaN(stycss[0]))
								tmpcss[$G.Trim(stycss[0])]=stycss[1];
							
						}
					);
					
				}
			}
			tmpdomtpe[tpk]=$G.arraytoString(tmpcss)||c[k];
			$G.copyobject(this,tmpdomtpe,'dom');
		});
	};
	/**
	* 获取SELECT被选种的值或文本
	o{obj,isTrue,isValueTrue,type}
	**/
	Glacier.getValueOrText=function(o){
		var v=new Array()
			,type=o.type||',';
			var obj=o.obj;
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
	Glacier.setValueOrText=function(o){
		var type=o.type||','
			,v=o.value.toString().split(type);
			var map={};
			for(var iv=0,nv=v.length;iv<nv;iv++){
				map[v[iv]]="true";
			}
			var obj=o.obj;;
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
	Glacier.delValueOrText=function(o){
		var type=o.type||','
			,v=o.value.toString().split(type);
			var map={};
			for(var iv=0,nv=v.length;iv<nv;iv++){
				map[v[iv]]="true";
			}
			var delv=new Array();
			var obj=o.obj;
			for(var i=0;i<obj.options.length;i++){
				var e=obj.options[i];
				if(o.isTrue){
					if(map[e.value]){
						obj.remove(i);
						i=i-1;
					}
				}else{
					if(map[e.text]){
						obj.remove(i);
						i=i-1;
					}
				}
			}
	};
	/**
	* 获取SELECT被选种的值
	**/
	Glacier.prototype.getselvalue=function(){
		var arr={};
		this.Each(function(i){
			var type=this.type?this.type.toLowerCase():"";
			if(type.indexOf('select')>=0){
				var str=$G.getValueOrText({
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
	Glacier.prototype.getseltext=function(){
		var arr={};
		this.Each(function(){
			var type=this.type?this.type.toLowerCase():"";
			if(type.indexOf('select')>=0){
				var str=$G.getValueOrText({
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
	Glacier.prototype.getallselvalue=function(){
		var arr={};
		this.Each(function(){
			var type=this.type?this.type.toLowerCase():"";
			if(type.indexOf('select')>=0){
				var str=$G.getValueOrText({
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
	Glacier.prototype.getallseltext=function(){
		var arr={};
		this.Each(function(){
			var type=this.type?this.type.toLowerCase():"";
			if(type.indexOf('select')>=0){
				var str=$G.getValueOrText({
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
	Glacier.prototype.setselvalue=function(value){
		this.Each(function(){
			var type=this.type?this.type.toLowerCase():"";
			if(type.indexOf('select')>=0){
				$G.setValueOrText({
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
	Glacier.prototype.setseltext=function(value){
		this.Each(function(){
			var type=this.type?this.type.toLowerCase():"";
			if(type.indexOf('select')>=0){
				$G.setValueOrText({
								  obj:this,
								  value:value,
								  isTrue:false
								 });
			}
		});
		return this;
	};
	/**
	* 跟据传入的VALUE删除相对应值
	**/
	Glacier.prototype.delselvalue=function(value){
		this.Each(function(){
			var type=this.type?this.type.toLowerCase():"";
			if(type.indexOf('select')>=0){
				$G.delValueOrText({
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
	Glacier.prototype.deldeltext=function(value){
		this.Each(function(){
			var type=this.type?this.type.toLowerCase():"";
			if(type.indexOf('select')>=0){
				$G.delValueOrText({
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
	Glacier.prototype.delallsel=function(){
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
	Glacier.prototype.addsel=function(text,value){
		this.Each(function(){
			var type=this.type?this.type.toLowerCase():"";
			if(type.indexOf('select')>=0){
				this.options.add(new Option(text,value));
			}
		});
		return this;
	};
	Glacier.prototype.Loaded=function(selector){
		$G.AttachEvent(window,'load',selector);
	};
	Glacier.prototype.LoadedDom=function(f){
		var ie = !!(window.attachEvent && !window.opera);    
	    var wk = /webkit\/(\d+)/i.test(navigator.userAgent) && (RegExp.$1 < 525);    
		var fn = [];    
		var run = function () {
			for (var i = 0; i < fn.length; i++) fn[i]();
		};
		var d = document;
		if (!ie && !wk && d.addEventListener)    
			return d.addEventListener('DOMContentLoaded', f, false);    
		if (fn.push(f) > 1) return;
		if (ie)
			(function () {
				try { d.documentElement.doScroll('left'); run(); }    
				catch (err) { setTimeout(arguments.callee, 0); }    
			})();    
		else if (wk)    
			var t = setInterval(function () {
			if (/^(loaded|complete)$/.test(d.readyState))    
			clearInterval(t), run();    
		}, 0); 
	};
	Glacier.prototype.bd=function(o){
		if($G.isNaN(o.on) || $G.isNaN(o.callback)) return;
		var arg=arguments;
		this.Each(function(){
			var gb=$G.closure({obj:this,callBack:o.callback,arg:arg});
			if(o.on.indexOf('click')==0 && this.style) this.style.cursor='pointer';
			if(!this.gbak_fn)this.gbak_fn=[];
			this.gbak_fn[o.callback]=gb;
			$G.AttachEvent(this,o.on,gb);
		});
		return this;
	};
	Glacier.prototype.delbd=function(o){
		if($G.isNaN(o.on) || $G.isNaN(o.callback)) return;
		this.Each(function(){
			var gb=this.gbak_fn[o.callback];
			$G.RemoveAttachEvent(this,o.on,gb);
		});
		return this;
	};
	Glacier.prototype._Zindex=999999;
	Glacier.fn=Glacier.prototype;
	Glacier.fn.MoveStart=function(obj,et,oEvent){
		this.style.zIndex=$G.Zindex();
		this.isMove=true;
		this.intervalid&&clearInterval(this.intervalid);
		var obj=$G.Obj(this),wsize=$G.windowSize();
		this._sw=obj.w;
		this._sh=obj.h;
		this._ww=wsize.sw;
		this._wh=wsize.sh;
		this._wl=wsize.scrollLeft;
		this._wt=wsize.scrollTop;
		this._x=oEvent.clientX-obj.x;
		this._y=oEvent.clientY-obj.y;
		if($G.isIE)obj.setCapture();
		$G.AttachEvent(document, "mousemove", obj.mousemove);
		$G.AttachEvent(document, "mouseup", obj.mouseup);
		oEvent.preventDefault?oEvent.preventDefault():oEvent.returnValue = false;
	};
	Glacier.fn.MoveYesWindowIng=function(obj,et,oEvent){
		var L=oEvent.clientX-this._x;
		var T=oEvent.clientY-this._y;
		this.style.left=L+"px";
		this.style.top=T+"px";
		if(!$G.isNaN(this.isMoveWindowReset)){
			this.isWindowReset=false;
		}		
	};
	Glacier.fn.MoveNotWindowIng=function(obj,et,oEvent){
		var bw=0,bh=0;
		if((!$G.compatMode && $G.isIE) || $G.isIE6){
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
		if(!$G.isNaN(this.isMoveWindowReset)){
			this.isWindowReset=false;
		}		
	};
	Glacier.fn.MoveStop=function(){
		$G.RemoveAttachEvent(document, "mousemove", this.mousemove);
		$G.RemoveAttachEvent(document, "mouseup", this.mouseup);
		if($G.isIE)this.releaseCapture();
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
				$G.fn.Gdiv(o);
			var _obj=o.obj.gobj,moveing=op.MoveYesWindowIng;
			_obj.mousedown=$G.closure({obj:_obj,callBack:op.MoveStart});
			if(!o.ismovewindow)moveing=op.MoveNotWindowIng;
			_obj.mousemove=$G.closure({obj:_obj,callBack:moveing});
			_obj.mouseup=$G.closure({obj:_obj,callBack:op.MoveStop});
			if(o.moveobj && o.moveobj[i] && !$G.isNaN(o.moveobj[i])) objMove=o.moveobj[i];
			objMove.style.cursor="move";
			$G.AttachEvent(objMove,"mousedown",_obj.mousedown);
		});
		return this;
	};
	Glacier.getMouseCoords=function(e) {
		return {
			x: $G.isIE ? e.clientX + Math.max(document.body.scrollLeft, document.documentElement.scrollLeft) : e.pageX,
			y: $G.isIE ? e.clientY + Math.max(document.body.scrollTop, document.documentElement.scrollTop) : e.pageY
		};
	};
	Glacier.MoveSize=function(o){
		if(!o) o={obj:'',sizeObj:''};
		if(!o.sizeObj) o.sizeObj=o.obj;
		if(!o.obj) return;
		if($G.isString(o.obj)) o.obj=$G('#'+o.obj)[0];
		if($G.isString(o.sizeObj)) o.sizeObj=$G('#'+o.sizeObj)[0];
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
		obj.style.zIndex = '30000';
		obj.onmousedown = function(e) {
			var d = document;
			if (!e) e = window.event;
			var x = e.layerX ? e.layerX: e.offsetX,
			y = e.layerY ? e.layerY: e.offsetY;
			var MCD = $G.getMouseCoords(e);
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
				var mus = $G.getMouseCoords(e);
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
			if($G.isNaN(this.move_o)) return;
			if((typeof this.move_o.n)=="number" && i!=this.move_omove_o.n || !this) return;
			var objMove=this;
			if(this.move_o.moveobj && !$G.isNaN(this.move_o.moveobj[i])) objMove=this.move_o.moveobj[i];
			objMove.style.cursor="";
			if(!$G.isNaN(this.gobj.mousedown))
				$G.RemoveAttachEvent(objMove, "mousedown",this.gobj.mousedown);
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
		if($G.isIE6){
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
			var wsize=$G.windowSize();
			lockDiv.setstyle({style:"z-index:"+$G.Zindex()+";position:absolute;top: 0px;left: 0px;height:"+wsize.h+"px;width:"+wsize.w+"px;background:"+(o.lock_back || "#000")+";filter:alpha(opacity="+lockOpa+");-moz-opacity: 0."+lockOpa+";opacity: 0."+lockOpa+";"});
			lockDiv.add();
			if($G.isIE6) lockDiv.dom.appendChild(this.ie6DivForIframe());
			$G.AttachEvent(window,'load',function(){
												    var wsize=$G.windowSize();
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
			var wsize=$G.windowSize();
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
		var _obj_init=$G.Obj(o.obj),winsize=$G.windowSize(),ieDiv=document.createElement('div'),ieDivCss="position:absolute;width:"+_obj_init.w+"px; height:"+_obj_init.h+"px;";//border:1px solid red;
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
			if($G.isIE6 || (!$G.compatMode && $G.isIE) || $G.mobile){
				ieDiv.style.position='absolute';
				_obj_init.x=ssw>ieDiv.initX?ieDiv.initX:ssw;
				_obj_init.y=ssh>ieDiv.initY?ieDiv.initY:ssh;
				ieDiv.initX=_obj_init.x;
				ieDiv.initY=_obj_init.y;
				ieDiv.intervalidgo=function(){
					ieDiv.intervalid=setInterval(function(){
						if(ieDiv.style.display!='none' && !ieDiv.isMove){
							var wsize=$G.windowSize();
							ieDiv.style.left=(ieDiv.initX+wsize.scrollLeft)+'px';
							ieDiv.style.top=(ieDiv.initY+wsize.scrollTop)+'px';
							if(!$G.mobile) clearInterval(ieDiv.intervalid);
						}
					}, 1000)
				};
				if($G.mobile)
					ieDiv.intervalidgo();
			}
		}
		ieDiv.style.left=_obj_init.x+'px';
		ieDiv.style.top=_obj_init.y+'px';
		if($G.isIE6){
			$G.fn.ie6More();
			ieDiv.style.position='absolute';
			ieDiv.appendChild($G.fn.ie6DivForIframe());
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
				$G.fn.Gdiv(o);
			o.obj.gobj.style.visibility="";
			o.obj.gobj.style.display="";
			o.obj.o=o;
			op.WinAltObj(o);
			os.push({obj:o.obj,o:o});
		});
		this.altScroll=function(){
			$G.Each(os,function(){
								var obj=this.obj,i=0,objo=this.o;
								if(!obj.gobj) return;
								obj.gobj.style.visibility="";
								if(o.position=='fixed' && obj.gobj.ieDivCss && ($G.isIE6 || (!$G.compatMode && $G.isIE))){
									obj.gobj.intervalidgo();
								}
							  });
			
		};
		this.altResize=function(){
			$G.Each(os,function(){
										var obj=this.obj,objo=this.o;
										if(!obj.gobj) return;
										objo.obj=obj;
										obj.gobj.style.visibility="";
										objo.isCallBack=true;
										if(obj.gobj.isWindowReset)
											$G.fn.WinAltObj(objo);
										$G.fn.AltDivInit(lockDiv);
								  });
			
		};
		$G.AttachEvent(window,"scroll",this.altScroll);
		$G.AttachEvent(window,"resize",this.altResize);
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
		var _zIndex=o.zIndex || $G.Zindex();					//对象图层轴上位置
		var _lock=o.lock || false;								//是否锁定浏览器
		var _callBack=o.callBack || function(){};				//是否回调方法
		_obj.isWindowReset=o.isWindowReset!=undefined?o.isWindowReset:true;			    	//是否在窗体发生变化时，调整层的相对位置
		_obj.isMoveWindowReset=o.isMoveWindowReset!=undefined?o.isMoveWindowReset:true;		//是否在窗体发生变化时，已拖动的层，位置是否还原初始值
		
		/**设置**/
		_obj.style.zIndex=_zIndex;
		var T=0,L=0;
		//var w = parseInt(_obj.offsetWidth||_obj.clientWidth);
		//var h = parseInt(_obj.offsetHeight||_obj.clientHeight);
		var show_obj_ini=$G.Obj(_obj);
		var w = show_obj_ini.w
		,	h = show_obj_ini.h
		;
		if(o.isCallBack)_callBack=function(){};

		/**************兼容性设置*******************/
		var wobj=$G.windowSize();
		if(_obj.style.position=='fixed'){
			wobj.scrollTop=0;
			wobj.scrollLeft=0;
		}
		
		if(_isCenter){
			L=wobj.scrollLeft+(wobj.sw/2)-(w/2);
			/******update by pablo****/
			if(wobj.sh>wobj.bh){
				T=wobj.scrollTop+(wobj.bh/2)-(h/2);
			}
			else{
				T=wobj.scrollTop+(wobj.sh/2)-(h/2);
			}
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
					if($G.isNaN(trueObj)) return;
					var g_objDom=$G(trueObj);
					g_objDom.style({style:'display:none;'},'update');
					trueObj.parentObj.appendChild(trueObj);
					trueObj.style.cursor="";
					if(!$G.isNaN(this.gobj.parentNode))this.gobj.parentNode.removeChild(this.gobj);
					if(!$G.isNaN(op.lockObj) && !$G.isNaN(op.lockObj.parentNode))op.lockObj.parentNode.removeChild(op.lockObj);
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
	Glacier.getQuery=function(name){
		var reg=new RegExp(String("(^|&|\\?)"+name+"=([^&]*)(&|$)")),r;
		if(r=this.toString().match(reg))
			return unescape(r[2]);
		return null;
	};
	Glacier.getid=function(name){
		return $G.getQuery.call(document.location,name);
	};
	Glacier.isNaN=function(obj){
		return obj=="undefined" || obj=="" || obj=="null" || obj == null;
	};
	/**
	* 去除全部script
	*/
	Glacier.dellscript=function(str){
		str=str+'';
		var reg =/((.|^)<script(.*)<\/script>)/ig;
		return str.replace(reg,"");
	};
	/**
	* 去除全部空格
	*/
	Glacier.Trim=function(obj){
		return obj.toString().replace(/\s/g,'');
	};
	/**
	* 去除双空格将所有的一个以上的空格替换成一个空格
	*/
	Glacier.TrimOne=function(obj){
		return obj.toString().replace(/\s{2,}/g," ");
	};
	/**
	* 去除前后二端空格
	*/
	Glacier.TrimTest=function(obj){
		return obj.toString().replace(/(^\s*)|(\s*$)/g,'');
	};
	/**
	* 去除左空格
	*/
	Glacier.TrimLeft=function(obj){
		return obj.toString().replace(/(^\s*)/g,'');
	};
	/**
	* 去除右空格
	*/
	Glacier.TrimRight=function(obj){
		return obj.toString().replace(/(\s*$)/g,'');
	};
	/**
	* 验证普通电话、传真号码：可以“+”开头，除数字外，可含有“-” 
	*/
	Glacier.isTel=function(str)
	{
		return (/^(([0\+]\d{2,4}-)?(0\d{2,3})-)(\d{7,8})(-(\d{3,}))?$/.test(str));
	};
	/**
	* 验证移动电话（手机）
	*/
	Glacier.isMobile=function(str) { 
		return (/^(?:13\d|1[3-9][0-9])-?\d{5}(\d{3}|\*{3})$/.test(str)); 
	};
	/**
	* 验证邮箱
	* 例：xxx@xxx.xxx
	*/
	Glacier.isMail=function(str)
	{
		return (/^\w+((-\w+)|(\.\w+))*\@[A-Za-z0-9]+((\.|-)[A-Za-z0-9]+)*\.[A-Za-z0-9]+$/.test(str));
	};
	Glacier.isNumber=function(i){
		return (/^[0-9]+.?[0-9]*$/.test(i))?true:false;
	};
	Glacier.type=function( obj ) {
		return obj == null ?String( obj ) : class2type[ toString.call(obj) ] || "object";
	},
	Glacier.isObj=function(obj){
		return Glacier.type(obj) === "object";
	};
	Glacier.isFun=function(obj){
		return Glacier.type(obj) === "function";
	};
	Glacier.isWindow=function( obj ) {
		return obj && typeof obj === "object" && "setInterval" in obj;
	},
	Glacier.isArray=function(obj){
		return Glacier.type(obj) === "array";
	};
	Glacier.isString=function(obj){
		return typeof obj=="string"||false;
	};
	/**
	* 查找字符串位置
	*/
	Glacier.findstring=function(str,fstr)
	{
		return str.indexOf(fstr||'');
	};
	/**
	* 返回中英文字符长度
	*/
	Glacier.len=function(str)
	{
		return str.toString().replace(/[^\x00-\xff]/g, "**").length;
	};
	/**
	* 截取中英文字符长度
	*/
	Glacier.lenSub=function(str,n,endStr)
	{
	  var r = /[^\x00-\xff]/g;
	  if($G.isNaN(str))return str;
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
	Glacier.filterUrl=function(url){
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
		$G.fn.ajaxSettings.isLocal=(/^(?:about|app|app\-storage|.+\-extension|file|res|widget):$/.test( ajaxLocParts[ 1 ] ));
		return ( ( url?url:ajaxLocation ) + "" ).replace( /#.*$/, "" ).replace( /^\/\//, ajaxLocParts[ 1 ] + "//" );
	};
	Glacier.copyobject=function(destination, source,type) {
		for (property in source) {
		   if(type=='dom'){
			   if (property == "style") {
					destination.style.cssText = source[property];
				} else {
					var key = $G.isIE6 || $G.isIE7?(this.dom.ATTRIBUTES[property] || property):property;
					destination.setAttribute(key, source[property]);
				}
		   }else
				destination[property] = source[property];
		}
		return destination;
	};
	Glacier.fn.copyobject = function(object) {
		return Glacier.copyobject.call(this, object);
	};
	Glacier.dom=function(o){
		o=o||{};
		var type		=	o.type
		,	attributes	=	o.att	||{}
		,	domtype		=	o.dom	||{}
		,	name		=	o.name||$G.uuidFast().replace(/-/ig,'')
		,	id			=	o.id||$G.uuidFast().replace(/-/ig,'')
		;
		if(!$G.isString(type) || !type){
			attributes=type;
			type='div';
		}
		var div					=		document.createElement(type)
		,	tmpdomtpe			=		{}
		,	retdom				=		function(){return $G.copyobject(div,attributes,'dom');}
		;
		div.id=id;
		div.setAttribute('name',name);
		for (k in domtype){
			var tpk=this.dom.TYPENAME[k]||k;
			tmpdomtpe[tpk]=domtype[k];
		}
		div=$G.copyobject(div,tmpdomtpe);
		return {
			dom:div,
			add:function(obj){
				obj=obj||document.body;
				obj.appendChild(this.dom);
				return this;
			},
			remove:function(){
				var delDom=$G('#'+(this.dom||div).id)
				;
				if(delDom.length>0)
					delDom[0].parentNode.removeChild(delDom[0]);
				return this;
			},
			setclass:function(name){
				this.dom.className=name;
			},
			setstyle:function(attributes){
				$G.copyobject(this.dom,attributes,'dom');
			},
			del:function(){
				//this.dom='';
				this.remove();
			},
			'':''};
	};
	Glacier.dom.TYPENAME={
			"html"			:	"innerHTML"	,
			"val"			:	"value"	,
			"test"			:	Glacier.isIE?"innerText":"textContent",
			"classname"		:	"className",
	"":""};
	Glacier.dom.ATTRIBUTES={
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
	Glacier.isPlainObject=function( obj ) {
		if ( !obj || $G.type(obj) !== "object" || obj.nodeType || $G.isWindow( obj ) ) {
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
	Glacier.buildParams=function( prefix, obj, add ) {
		if ( $G.isArray( obj ) ) {
			$G.Each( obj, function( i ) {
				if ( /\[\]$/.test( prefix ) ) {
					add( prefix, this );
				} else {
					$G.buildParams( prefix + "[" + ( typeof this === "object" || $G.isArray(this) ? i : "" ) + "]", this, add );
				}
			});
	
		} else if ( obj != null && typeof obj === "object") {
			for ( var name in obj ) {
				$G.buildParams( prefix + "[" + name + "]", obj[ name ], add );
			}
		} else {
			add( prefix, obj );
		}
	};
	Glacier.formatparam=function(o){
		o=o||{};
		var s = []
		,	add = function( key, value ) {
				value = $G.isFun( value ) ? value() : value;
				s[ s.length ] = o.Handle(key,value);
			}
		;
		if(!$G.isNaN(o.param)){
			if ( $G.isArray( o.param ) || !$G.isPlainObject( o.param )) {
				$G.Each( o.param, function() {
					add( this.name, this.value );
				});
	
			} else {
				for ( var prefix in o.param ) {
					$G.buildParams( prefix, o.param[ prefix ], add );
				}
			}
		}
		return s.join( o.separate );
	};
	Glacier.param=function(param){
		return $G.formatparam({
				param		:	param
			,	separate	:	"&"
			,	Handle		:	function(key,value){
					return encodeURIComponent( key ) + "=" + encodeURIComponent( value );
				}
		}).replace( /%20/g , "+" );
	};
	Glacier.arraytoString=function(param){
		return $G.formatparam({
				param		:	param
			,	separate	:	";"
			,	Handle		:	function(key,value){
					return key + ":" +  value;
				}
		});
	};
	Glacier.tojson=function(param){
		return $G.formatparam({
				param		:	param
			,	separate	:	","
			,	Handle		:	function(key,value){
					return '"'+encodeURIComponent( key )+'":"'+encodeURIComponent( value )+'"';
				}
		});
	};
	Glacier.Stringtojson=function(a){
		a=a+"";
		var s = {},
		add = function( key, value ) {
				s[key.replace(/"|'/g,'')] = value.replace(/"|'/g,'')||'';
		}
		;
		if ( $G.isString( a ) ) {
			$G.Each( a.split(","), function() {
				var tmpList=this.split(":");
				add( tmpList[0], tmpList[1] );
			});

		}
		return s;
	};
	Glacier.closure=function(o){
		return function(event){
			if($G.isNaN(o)) return;
			var e=(event || window.event);
			var ev=(e  ||  arguments.callee.caller.arguments[ 0 ]);
		    var et=ev.srcElement || ev.target;
			o.callBack.call(o.obj,o.arg,et,e);
		}
	};
	Glacier.Browser=function(){
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
		if(!Glacier.isNaN(Glacier.isIE) && Glacier.isIE.length>1){
			Glacier.isIE6=Glacier.isIE[1]=="6.0"?Glacier.isIE[1]:false;
			Glacier.isIE7=Glacier.isIE[1]=="7.0"?Glacier.isIE[1]:false;
			Glacier.isIE8=Glacier.isIE[1]=="8.0"?Glacier.isIE[1]:false;
			Glacier.isIE9=Glacier.isIE[1]=="9.0"?Glacier.isIE[1]:false;
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
	Glacier.AttachEvent=function(obj,fn,gb,useCapture){
		if(obj.addEventListener)
			obj.addEventListener(fn,gb,(useCapture||false));
		else if(obj.attachEvent)
			obj.attachEvent("on"+fn,gb);
		else
			obj["on"+fn]=gb;
	};
	Glacier.RemoveAttachEvent=function(obj,fn,gb,useCapture){
		if(obj.removeEventListener){
			obj.removeEventListener(fn,gb,(useCapture||false));
		} else if(obj.detachEvent){
			obj.detachEvent("on"+fn,gb);
		} else{ 
			obj["on"+fn]=null;
		}
	};
	Glacier.Zindex=function(){
		return this.fn._Zindex++;
	};
	Glacier.windowSize=function(){
		var o = {x:0,y:0},windowobj=$G.compatMode?document.documentElement:document.body; 
		o.w=Math.max(windowobj.scrollWidth,windowobj.clientWidth);
		o.h=Math.max(windowobj.scrollHeight,windowobj.clientHeight);
		o.sw=windowobj.clientWidth;
		o.sh=windowobj.clientHeight;
		o.scrollTop=windowobj.scrollTop || window.pageYOffset || document.body.scrollTop;
		o.scrollLeft=windowobj.scrollLeft || window.pageXOffset || document.body.scrollLeft;
		o.bh = document.body.scrollHeight||document.body.clientHeight;// 正文高度 add by pablo
		return o;
	};
	Glacier.Obj=function(obj){
		var o=obj;
		var pt = {x:0,y:0}; 
		do
		{
		   pt.x += obj.offsetLeft;
		   pt.y += obj.offsetTop;
		   obj = obj.offsetParent;
		}
		while(obj);
		o.x=pt.x;
		o.y=pt.y;
		o.w = parseInt(o.offsetWidth||o.clientWidth);
		o.h = parseInt(o.offsetHeight||o.clientHeight);
		return o;
	};
	Glacier.getPrevioussibling=function(obj,o){
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
	Glacier.getParent=function(obj,tagName){
	    if($G.isNaN(obj))return(null);
	    if(obj.tagName==tagName){
	        return(obj);
	    }else{
	        return($G.getParent(obj.parentNode,tagName));
	    }
	};
	Glacier.ie6alt=function(){
		var o=this.o;
		if(o.obj.gobj.isMove) return;
		o.obj.gobj=this;
		$G.fn.WinAltObj(o);
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
	Glacier.createStandardXHR=function(){
		try {
			return new window.XMLHttpRequest();
		} catch( e ) {}
	};
	Glacier.createActiveXHR=function() {
		try {
			return new window.ActiveXObject( "Microsoft.XMLHTTP" );
		} catch( e ) {}
	};
	Glacier.globalEval=function(data){
			if ( data && /\S/.test( data ) ) {
				( window.execScript || function( data ) {
					window[ "eval" ].call( window, data );
				} )( data );
			}
	};
	Glacier.parseXML=function( data ) {
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
	Glacier.parseHTML=function( data ) {
		return data;
	};
	Glacier.parseJSON=function( data ) {
		if ( typeof data !== "string" || !data ) {
			return null;
		}
		data = $G.TrimTest( data );
		if ( window.JSON && window.JSON.parse ) {
			return window.JSON.parse( data );
		}
		if ( /^[\],:{}\s]*$/.test( data.replace( /\\(?:["\\\/bfnrt]|u[0-9a-fA-F]{4})/g, "@" )
			.replace( /"[^"\\\n\r]*"|true|false|null|-?\d+(?:\.\d*)?(?:[eE][+\-]?\d+)?/g, "]" )
			.replace( /(?:^|:|,)(?:\s*\[)+/g, "")) ) {
			return (new Function( "return " + data ))();
		}
	};
	Glacier.parseSCRIPT=function( data ) {
		$G.globalEval(data);
		return '';
	};
	Glacier.getscript=function(s){
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
					if(!$G.isNaN(s.cache))
						script.src = s.url + ( /\?/.test( s.url ) ? "&" : "?" ) + "notcache=" + new Date().getTime();
					
					script.onload = script.onerror = script.onreadystatechange = function( _, isAbort) {
						if (!script.readyState || /loaded|complete/.test( script.readyState ) ) {
							script.onload = script.onreadystatechange = null;
							if ( head && script.parentNode ) {
								head.removeChild( script );
							}
							script = undefined;
							if ( !$G.isNaN(isAbort) ) {
								s.callback  && s.callback( 200, "abort" );
							}else
								s.callback  && s.callback();
						}
					};
					if($G.isIE9)script.onerror='';
					head.insertBefore( script, head.firstChild );
				},
				abort: function() {
					if ( script ) {
						script.onload( 0, 1 );
					}
				}
			};
	};
	Glacier.getjs=function(o){
		o=o||{};
		var stime=function(a,b){
			if(b=='abort'){
			}else{
				clearTimeout(st);
			}
			o.callback && o.callback(a,b);
		},	scp=$G.getscript({
			 url:o.url,
			 callback:stime,
			 scriptCharset:o.charset,
			 cache:o.cache
			 })
		,	st=$G.setTimeout(scp.abort,(o.time||15))
		;
		
		scp.send();
		
	};
	Glacier.readyState = function(o) {
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
	Glacier.ajax=function(url,o){
		if ( typeof url === "object" ) {
			o = url;
			url=o.url;
		}
		o = o || {};
		if($G.isNaN(o.success)) return;
				
		o.xhr=window.ActiveXObject ?function(){
			return !$G.fn.ajaxSettings.isLocal && Glacier.createStandardXHR() || Glacier.createActiveXHR();
		}():Glacier.createStandardXHR();

		o.type = o.type || this.fn.ajaxSettings.type;
		o.timeout = o.timeout || this.fn.ajaxSettings.timeout;
		o.dataType = o.dataType || this.fn.ajaxSettings.dataType;
		o.beforeSend = o.beforeSend || this.fn.ajaxSettings.beforeSend;
		o.error = o.error || this.fn.ajaxSettings.error;
		o.async = o.async || this.fn.ajaxSettings.async;
		o.cache = o.cache || this.fn.ajaxSettings.cache;
		o.headers = o.headers || this.fn.ajaxSettings.headers;
		
		o.url=$G.filterUrl(url);
		if(o.async && (o.async==true || o.async=='true')) o.async=true;
		else o.async=false;
		o.data=(typeof o.data !== "string"?$G.param(o.data):o.data);
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
				if (o.headers.hasOwnProperty(i)) {
					o.xhr.setRequestHeader(i, o.headers[i]);
				}
			}
			o.xhr.send(o.data);
			//if(!o.async)o.success(Glacier.ajax.contents(o),o);
		} catch (e) {}
		return o.xhr;
	};
	Glacier.get=function(url,success){
		return $G.ajax(url,{success:success});
	};
	Glacier.post=function(url,data,success){
		return $G.ajax(url,{type:"POST",data:data,success:success});
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
	Glacier.postfrom=function(url,o){
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
			forobj=$G(o.fromobj);
			forobj_n=forobj.length;
		}
		for(var i=0;i<forobj_n;i++){
			var tmp_subobj=$G('input textarea select',forobj[i]);
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
			var obj=subobj[k],val=$G(obj).val(),type=obj.type?obj.type.toLowerCase():"",isRadio=type.indexOf('radio'),isCheckbox=type.indexOf('checkbox');
			if(o.paramnullpass && $G.isNaN(val))continue;
			if(o.display){
				if(obj.style.display=='none' || $G.isNaN(obj.offsetWidth || o.clientWidth || o.offsetHeight || o.clientHeight))
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
				if($G.isNaN(val)){
					msgobj.push(obj.title?obj.title:o.msg);
					try{obj.focus();} catch (e) {}
					if(o.ckmsgnotall) break;

				}
			}else if($G.isFun(o.ck)){
				var isTrueNext=o.ck.call(o.ck,obj);
				if($G.isString(isTrueNext) && isTrueNext=='next'){
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
			if(o.paramnullpass && $G.isNaN(v)) return true;
			if(typeof o.ck=="boolean"){
				if($G.isNaN(v)){
					msgobj.push(obj && obj.length>0 &&obj[0].title?obj[0].title:o.msg);
					return false;
				}
			}else if($G.isFun(o.ck)){
				var isTrueNext=o.ck.call(o.ck,obj);
				if($G.isString(isTrueNext) && isTrueNext=='next'){
					return true;
				}
				if(typeof isTrueNext=="boolean" && !isTrueNext){
					dataArray.length=0;
					return false;
				}
			}
			if(o.checkboxarray && type=='checkbox'){
				$G.Each(v,function(v){
								   dataArray.push(encodeURIComponent(k)+'='+encodeURIComponent((this+'').toString()));
								   });
			}else{
					dataArray.push(encodeURIComponent(k)+'='+encodeURIComponent((v+'').toString()));
			}
			return true;
		};
		for(var k in radioarray){
			var v=radioarray[k];
			if(!ckrc(k,v,'radio') && o.ckmsgnotall) break;
		}
		if(o.ckmsgnotall && msgobj.length>0){}
		else{
		for(var k in checkboxarray){
			var v=checkboxarray[k];
			if(!ckrc(k,v,'checkbox') && o.ckmsgnotall) break;
		}}
		if($G.isFun(o.ck)){
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
				return subdata+param+(typeof data !== "string"?$G.param(data):data);
			else return '';
		};
		
		o.data=adddata(datatmp,o.data) || datatmp;
		o.url=url;
		o.type='post';
		$G.ajax(o);	
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
	Glacier.bdJsonByDom=function(o){
		o = o || {};
		o.type=o.type||'input textarea select';
		o.bdtype=o.bdtype||'name';
		if($G.isNaN(o.json)){
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
			bdDom=$G(o.bdDom);
			bdDom_n=bdDom.length;
		}
		
		for(var i=0;i<bdDom_n;i++){
			var tmp_domList=$G(o.type,bdDom[i]);
			for(var k=0,n=tmp_domList.length;k<n;k++){
				domList.push(tmp_domList[k]);
			}
		}
		if(o.bdtype=='index'){
			for(var a in o.json){
				jsonList.push(o.json[a]);
			}
		}
		for(var k=0,n=domList.length;k<n;k++){
			var obj=domList[k]
			,	g_obj=$G(obj)
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
			if(!$G.isNumber(fn_cb_this.val) && $G.isNaN(fn_cb_this.val)) fn_cb_this.val='';
			tmp_fn.call(fn_cb_this);
		}
		
		return ;
	};
	/***********************************AJAX的实现结束**************************************************/
	/***********************************js cookie的开始**************************************************/
	Glacier.jscookie=function(c){
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
	Glacier.thisMovie=function(movieName){
		if($G.isIE)
			return window[movieName];
		else
			return document[movieName];
	};
	Glacier.flashcookie=function(o){
		o = o || {};
		var cookieObj	  =  ''
		,	span=$G.dom({type:'span'})
		,	src			  =  o.src || "/cookie.swf"//flashCookie.swf路径
		,	callback	  =  o.callback || function(){}
		,	flashID	      = 'flashcookie'+span.dom.id
		;
			this.CookiesInit=function(){
				var flashCookieObj=($G.thisMovie(flashID))
				,	obj={
							type		:	'flash',
							id			:	flashID,
							obj			:	flashCookieObj,
							set			:	function(cookieName,cookieValue){
								this.obj.setCookies(escape(cookieName),escape(cookieValue));
							},
							get			:	function(cookieName){
								return unescape(this.obj.getCookies(escape(cookieName)))||'';
							},
							del			:	function(cookieName){
								this.obj.delCookies(escape(cookieName));
							},
							delAll		:	function(){
								this.obj.cls();
							},
							iscookies	:	function(cookieName){
								return this.obj.isCookies(escape(cookieName))||'';
							}
						}
				;
				callback && callback.call(this,obj);
				
			};
			var str='<OBJECT id="'+flashID+'" classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000" width="0px" height="0px" ><PARAM NAME="movie" VALUE="'+src+'">';
			if(!$G.isIE) str='<object width="0px" height="0px" data="'+src+'" id="'+flashID+'" type="application/x-shockwave-flash">';
			
			str+='<param value="sameDomain" name="allowscriptaccess"><param value="true" name="allowfullscreen"></object>';
			span.dom.innerHTML=str;
			span.add();
	};
	/***********************************flash cookie的结束**************************************************/
	/**************************************** cookie的开始**************************************************/
	Glacier.cookiesetup={'isSetup':false,'js':'','flash':''};
	Glacier.cookie=function(o){
		o = o || {};
		var 
		 jscookie=$G.jscookie(o)
		,jsgetvalue=''
		,ck_callbak=''
		,ck_cookieName=''
		,fn=function(){
			var s=jsgetvalue
			,flashcookie=$G.cookiesetup['flash'];
			if($G.isNaN(s) && flashcookie){
				s=flashcookie.get(ck_cookieName);
				if(!$G.isNaN(s))jscookie.set(s);
			}
			ck_callbak && ck_callbak.call(this,s);
		};
		
		return {
			ckget:function(cookieName,callbak){
				if($G.isNaN(callbak)) return;
				ck_callbak=callbak;
				ck_cookieName=cookieName;
				jsgetvalue=jscookie.get(cookieName);
				if($G.isNaN(jsgetvalue)){
					o.callbak=function(){
						var v='';
						try{
							v=$G.cookiesetup['flash'].get(cookieName)||'';
						}catch(e){}
						if(!$G.isNaN(v))jscookie.set(cookieName,v);
						fn(v);
					};
					$G.cookieinit(o);
				}else fn();
			},
			set:function(cookieName,cookieValue){
				if(jscookie)
					jscookie.set(cookieName,cookieValue);
				if($G.cookiesetup['flash'])
					$G.cookiesetup['flash'].set(cookieName,cookieValue);				
				return cookieValue;
			},
			get:function(cookieName){
				return jscookie.get(cookieName) || $G.cookiesetup['flash'].get(cookieName);
			},
			del:function(cookieName){
				if(jscookie)
					jscookie.del(cookieName);
				if($G.cookiesetup['flash'])
					$G.cookiesetup['flash'].del(cookieName);
			},
			delAll:function(){
				if(jscookie)
					jscookie.delAll();
				if($G.cookiesetup['flash'])
					$G.cookiesetup['flash'].delAll();
			},
			iscookies:function(cookieName){
				return jscookie.iscookies(cookieName) || $G.cookiesetup['flash'].iscookies(cookieName);
			}
		}
		
	};
	Glacier.cookieinit=function(o){
		o = o || {};
		var fn=function(k){
			if($G.isNaN(k) || $G.isNaN(k.obj.isCookies))
				return;
			fc=k;
			cls();
		}
		,fc=''
		,intervalid=setInterval(function(){
			fn();
		}, 100)
		,cls=function(){
			clearInterval(intervalid);
			if($G.cookiesetup['isSetup']) return;
			if(!$G.isNaN(fc) && !$G.isNaN(fc.obj) && !$G.isNaN(fc.obj.isCookies)) $G.cookiesetup['flash']=fc;
			$G.cookiesetup['isSetup']=true;
			o.callbak && o.callbak();
		};
		this.flashcookie({src:o.src,callback:fn});
		setTimeout(cls,3000);
	};
	
	/**************************************** cookie的结束**************************************************/
	/***********************************鼠标提示    开始**************************************************/
	Glacier.tip=function(o){
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
					this.mouseover=$G.closure({obj:this,callBack:this.start});
					$G.AttachEvent(document,"mouseover",this.mouseover);
					
					this.ouseout=$G.closure({obj:this,callBack:this.end});
					$G.AttachEvent(document,"mouseout",this.ouseout);
				},
			start:function(o,et,oEvent){
					var obj=$G.Obj(et)
					,_x=obj.x+obj.w
					,_y=obj.y+obj.h
					,wsize=$G.windowSize()
					,bw=wsize.scrollLeft
					,bh=wsize.scrollTop
					;
					obj.tip=obj.title||obj.alt||obj.tip;
					if($G.isNaN(obj.tip)) return;
					div.innerHTML=obj.tip;
					div.style.display=obj.title=obj.alt="";
					
					var tipobj=$G.Obj(div);
					var _x=bw+_x,ckL=bw+wsize.sw-tipobj.w;
					var _y=bh+_y,ckT=bh+wsize.sh-tipobj.h;
					if((!$G.compatMode && $G.isIE) || $G.isIE6){
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
					$G.RemoveAttachEvent(document, "mouseover",this.mouseover);
					$G.RemoveAttachEvent(document, "mouseout",this.ouseout);
				}
			};
	};
	/***********************************鼠标提示    结束**************************************************/
	Glacier.statistics=function(){
		var ref=function(){
			if(!$G.isNaN(document.referrer)) return document.referrer;
			return '';
		};
		return {
			fromPage:ref(),
			toPage:window.location,
			toPageTitle:encodeURIComponent(window.document.title) || this.toPage,
			//userAgent:navigator.userAgent,
			platform:navigator.platform,
			browser:Glacier.getBrowser,
			bVersion:Glacier.getBrowserVersion,
			ufcolor:navigator.appName=="Netscape"?ufcolor=screen.pixelDepth:ufcolor=screen.colorDepth,
			screensize:screen.width+'.'+screen.height,
			language:navigator.language?navigator.language:navigator.browserLanguage,
			timeZone:(new Date().getTimezoneOffset()/60)*(-1)
			};
	};
	Glacier.uuid=function(len, radix){
		var chars = $G.fn.Exp.CHARS, uuid = [], i;
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
	Glacier.uuidFast=function(){
		 var chars=$G.fn.Exp.CHARS, uuid = new Array(36), rnd=0, r;
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
	Glacier.uuidCompact=function(){
		return 'xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx'.replace(/[xy]/g, function(c) {
																				var r = Math.random()*16|0, v = c == 'x' ? r : (r&0x3|0x8);
																				return v.toString(16);
																				});
	};
	Glacier.setTimeout=function(fn,time){
		var tmpfn=fn;
		time=$G.isNumber(time)?time:0;
		if($G.isFun(fn)){
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
	Glacier.setInterval=function(fn,time){
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
															n=$G.isNumber(o.n)?parseInt(o.n):1;
														if(o.time){
															var callfn=function(){
																						this.obj.stop();
																					}
															,	t=$G.isNumber(o.time)?o.time:1
															;
															
															$G.setInterval(callfn,t).start();
														}
														return retfn;
													}
									}
		;
		time=$G.isNumber(time)?parseInt(time):0;
		if($G.isFun(fn)){
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
	Glacier.animation=function(o){
		o 					=		o || {};
		if($G.isNaN(o.obj)) return;
		var obj_init			=	$G.Obj(o.obj)
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
															$G.setTimeout(fn.callbck,fn.speed,s,fn,callbak);
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
											var obj_init = $G.Obj(obj.obj);
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
	Glacier.getColor=function(o){
		o 					=		o || {};
		o.obj = $G.isString(o.obj)?$G('#'+o.obj)[0]:o.obj;
		if(!$G.isObj(o.obj)) return;
		o.h = [];
		o.h[0] = "FF";
		o.h[1] = "CC";
		o.h[2] = "99";
		o.h[3] = "66";
		o.h[4] = "33";
		o.h[5] = "00";
		o.on=o.on||'click';
		function init(){
			o.showobj=$G('#'+o.id).length>0?$G('#'+o.id)[0]:'';
			if((!$G.isNaN(o.id) && !$G.isObj($G('#'+o.id)[0])) || $G.isNaN(o.id)){
				var wsize=$G.windowSize();
				var lockdiv=$G.dom({type:'div'});
				lockdiv.setstyle({style:"display:none;width:"+wsize.w+"px;height:"+wsize.h+"px;z-index:"+$G.Zindex()+";position:absolute;top: 0px;left: 0px;height:100%;width:100%;background:#ffffff;filter:alpha(opacity=0);-moz-opacity: 0;opacity: 0;"});
				lockdiv.add();
				
				var div=$G.dom({type:'div',id:(o.id||'')});
				div.setstyle({style:"display:none;z-index:"+$G.Zindex()+";position:absolute;width:235px; padding:0 0 1px 0; background:#fff; overflow:hidden; margin-bottom:30px;"});
				div.add();
				o.showobj=div.dom;
				o.showobj.lockobj=lockdiv.dom;
				
				$G(lockdiv.dom).bd({on:'click',callback:function(){
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
					var _ul=$G.dom({type:'ul'});
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
				var _li = $G.dom({type:'li'});
				var _a = $G.dom({type:'a'});
				_li.setstyle({title:("#"+ R + G + B),style:'float:left; display:inline; width:12px; height:12px; margin:1px 0 0 1px; background:#808080;'});
				_a.setstyle({style:'background:#'+(R + G + B)+';display:block; margin:1px 0 0 1px; width:11px; height:11px; overflow:hidden;'});
				_a.add(_li.dom);
				_li.add(_ul);
				$G(_li.dom).bd({on:'click',callback:function(){
					hide(_ul.parentNode);
					o.callback.call(this,R + G + B,o.on_obj)
				}});
			}
		;
		var tmpobj=$G(o.obj);
		tmpobj[0].showobj=o.showobj;
		tmpobj.bd({on:o.on,callback:function(){
			o.on_obj=this;
			var obj=$G.Obj(o.on_obj)
			,	_x=obj.x
			,	_y=obj.y+obj.h
			,	wsize=$G.windowSize()
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
	Glacier.setupGetColor=function(o){
		o 					=		o || {};
		o.isOne=o.isOne?$G.uuidFast().replace(/-/ig,''):'';
		$G.Each(
			o.ids.split(","),
			function(i){
				$G.getColor({
					 id:o.isOne,
					 obj:this.toString(),
					 on:o.on,
					 callback:o.callback
				 });
			}
		);
	};
	Glacier.Dropshadow=function(o){
		o 					=		o || {};
		var css_filter=new Array()
		,	css_textShadow=new Array()
		,	RGB=this.RGB
		;
		$G.Each(
			o,
			function(i){
				if($G.isNaN(RGB)) RGB=this[2];
				css_filter.push('Dropshadow(offx='+this[0]+',offy='+this[1]+',color='+RGB+')');
				css_textShadow.push(RGB+' '+this[0]+'px '+this[1]+'px 0 ');
			}
		);
		return 'filter:'+css_filter.join(' ')+';text-shadow:'+css_textShadow.join(',')+';';
	};
	Glacier.domload=function(o){
		o = o || {};
		var obj=$G(o.obj)[0]
		,	list=$G((o.find || 'img object iframe'),obj)
		,	n=list.length
		,	ckn=1
		,	onfn=function(cbing,cb){
				ckn>0&&ckn++;
				if($G.isFun(cbing)) cbing.call(this);
				if(n<ckn){
					ckn=n=-1;
					ckn--;
					this.onerror=this.onload='';
					cb && cb.call(obj);
				}
			}
		,	fn=function(i){
				var domObj=list[i];
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
			}
		;
		if(n==0) o.callbak && o.callbak();
		for(var i=0;i<n;i++){
			if($G.isIE) onfn.call(list[i],o.callbaking,o.callbak);
			else fn(i);
		}
	};
	Glacier.autoimg=function(maxWidth,maxHeight,objImg){
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
	Glacier.loadscroll=function(o){
		o=o||{};
		if($G.isNaN(o.obj)) return;
		var obj=o.obj
		,	loadh=o.loadh||20
		,	p=1
		,	objIni={}
		,	isOne=false
		,	ckLoad=function(){
				return (obj.scrollHeight-obj.scrollTop-objIni.h)<loadh;
			}
		,	fn=function(){
				if(ckLoad() && isOne){
					isOne=false;
					stop();
					o.callback && o.callback(p);
				}
			}
		,	start=function(){
				p=1;
				objIni=$G.Obj(obj);
				$G.AttachEvent(obj,"scroll",fn);
			}
		,	go=function(){
				if(ckLoad())
					obj.scrollTop=obj.scrollHeight-loadh-objIni.h-1;
					
				var th=obj.scrollHeight-objIni.h;
				if(th<loadh)
					loadh=th;
				p++;
				$G.AttachEvent(obj,"scroll",fn);
			}
		,	stop=function(){
				$G.RemoveAttachEvent(obj,"scroll",fn);
			}
		,	uptoph=0
		,	uptopfn=function(){
				if(uptoph<obj.scrollTop){
					$G.RemoveAttachEvent(obj,"scroll",uptopfn);
					isOne=true;
				}
				uptoph=obj.scrollTop;
			}
		;
		$G.AttachEvent(obj,"mousedown",function(){
			if(!ckLoad())
				isOne=true;
			else{
				uptoph=obj.scrollTop;
				$G.AttachEvent(obj,"scroll",uptopfn);
			}
		});
		return {
				start	:	start,
				go		:	go,
				stop	:	stop,
				''		:	''
			};
	};
	Glacier.png=function(obj){
		if(!$G.isIE6)return obj;
		obj=obj||window.document;
		$G.Each(
				obj.getElementsByTagName('*')
			,	function(i){
					var bg = this.currentStyle.backgroundImage;
					if (bg && bg.match(/.PNG|.png/i) != null){
						var mypng = bg.substring(5,bg.length-2);
						this.style.filter = "progid:DXImageTransform.Microsoft.AlphaImageLoader(src='"+mypng+"',enabled=true, sizingMethod=scale)";
						this.style.backgroundImage = "url('')";
						this.style.background = "none";
					}
				}
		);
		$G.Each(
				$G('img',obj)
			,	function(i){
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
		);
		return obj;
	};
	window.$G=Glacier;
	$G.Each(
		"Boolean Number String Function Array Date RegExp Object".split(" "),
		function(i){
			class2type[ "[object " + this + "]" ] = this.toLowerCase();
		}
	);
})();