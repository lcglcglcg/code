var upimgfn=function(o){
	o=o||{};
	var progress_gettime;
	return settings = {
		flash_url : "/static/web/flash/swfupload.swf",
		upload_url: o.url||'/upflie',
		post_params: o.data||{},
		file_size_limit : "2000 MB",
		file_types : o.types||"*.xls",
		file_types_description : o.test||"浏览",
		file_upload_limit : 0,
		//select_mode:o.select_mode||'',
		//file_queue_limit : 1,
		debug: false,
		button_image_url :o.pic||'/static/sem_web_1.0/web/images/shangc.gif',
		button_placeholder_id : o.upid||'upflie',
		button_width: o.pic_w||48,
		button_height: o.pic_h||19,
		file_queue_error_handler : o.file_queue_error_handler||function(file, errorCode, message){
		},
		file_queued_handler:o.queued_handler||function(file) {    
			this.customSettings.queue = this.customSettings.queue || new Array();
			while (this.customSettings.queue.length > 0) {
				this.cancelUpload(this.customSettings.queue.pop(), false);
			}
			this.customSettings.queue.push(file.id);
			return true;
		},
		upload_start_handler :function(file){
			try {
				progress_gettime=new Date().getTime();
				if(o.upload_start_handler) return o.upload_start_handler();
				else
					return true;
			} catch (e) {
			}
		},
		upload_progress_handler : o.upload_progress_handler||function(file, bytesLoaded, bytesTotal){
			if(o.upload_progress_handlerIni){
				try {
					var percent = Math.ceil((bytesLoaded / bytesTotal) * 100)
					,	filesize=$G.toDecimal2((file.size / 1024)/1024)
					,	kbing=(bytesLoaded / 1024)/1024
					,	upsize=$G.toDecimal2(kbing)
					,	ctime=progress_gettime
					,	timeing=new Date().getTime()
					,	otime=timeing-ctime
					,	skb=(kbing*1024)/otime
					,	alltime=(file.size-bytesLoaded)/(skb*1024)
					,	formatalltime=new Date().setTime(alltime)
					,	speeding=$G.toDecimal2(skb*1000)
					,	overtime=$G.formatdate(new Date(formatalltime-(8*60*60*1000)),'hh:mm:ss')
					;
					o.upload_progress_handlerIni({
						name:file.name
					,	percent:percent
					,	filesize:filesize
					,	upsize:upsize
					,	speeding:speeding
					,	overtime:overtime
					});
				} catch (ex) {
					
				}
				
			}
		},
		upload_error_handler : o.upload_error_handler||function(file, errorCode, message){
		},
		upload_success_handler : o.success||function(file, serverData) {
		},
		upload_complete_handler : o.upload_complete_handler||function(file, serverData){
		}
	};
}
;
$G.upflie=function(o){
	return new SWFUpload(upimgfn(o));
	//upFileObj.startUpload();
};