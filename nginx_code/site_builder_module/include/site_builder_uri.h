
#ifndef __SITE_BUILDER_URI_H__
#define __SITE_BUILDER_URI_H__

#include <nginx.h>
#include <ngx_core.h>
#include <ngx_http.h>

#define SITE_BUILDER_URI_KEY_SIZE		128

#define SITE_BUILDER_URI_TYPE_URL		0X01
#define SITE_BUILDER_URI_TYPE_MOBILE_URL	0X02

#define SITE_BUILDER_URI_HTML_STR (u_char *)"text/html"
#define SITE_BUILDER_URI_HTML_SIZE 9
#define SITE_BUILDER_URI_CSS_STR (u_char *)"text/css"
#define SITE_BUILDER_URI_CSS_SIZE 8
#define SITE_BUILDER_URI_JS_STR (u_char *)"application/x-javascript"
#define SITE_BUILDER_URI_JS_SIZE 24
#define SITE_BUILDER_URI_GIF_STR (u_char *)"image/gif"
#define SITE_BUILDER_URI_GIF_SIZE 9
#define SITE_BUILDER_URI_ICO_STR (u_char *)"image/x-icon"
#define SITE_BUILDER_URI_ICO_SIZE 12
#define SITE_BUILDER_URI_JPEG_STR (u_char *)"image/jpeg"
#define SITE_BUILDER_URI_JPEG_SIZE 10
#define SITE_BUILDER_URI_PNG_STR (u_char *)"image/png"
#define SITE_BUILDER_URI_PNG_SIZE 9
#define SITE_BUILDER_URI_BMP_STR (u_char *)"application/x-bmp"
#define SITE_BUILDER_URI_BMP_SIZE 17
#define SITE_BUILDER_URI_IMG_STR (u_char *)"application/x-img"
#define SITE_BUILDER_URI_IMG_SIZE 17
#define SITE_BUILDER_URI_TXT_STR (u_char *)"text/plain"
#define SITE_BUILDER_URI_TXT_SIZE 10

#define SITE_BUILDER_URI_TYPE_HTML		1 // .htm .html 	text/html
#define SITE_BUILDER_URI_TYPE_CSS		2 // .css		text/css
#define SITE_BUILDER_URI_TYPE_JS		3 // .js		application/x-javascript
#define SITE_BUILDER_URI_TYPE_GIF		4 // .gif 		image/gif
#define SITE_BUILDER_URI_TYPE_ICO		5 // .ico 		image/x-icon
#define SITE_BUILDER_URI_TYPE_JPEG		6 // .jpe .jpg .jpeg 	image/jpeg
#define SITE_BUILDER_URI_TYPE_PNG		7 // .png		image/png
#define SITE_BUILDER_URI_TYPE_BMP		8 // .bmp 		application/x-bmp
#define SITE_BUILDER_URI_TYPE_IMG		9 // .img		application/x-img
#define SITE_BUILDER_URI_TYPE_TXT               10 //.txt
#define SITE_BUILDER_URI_TYPE_BAIDU		11


#ifdef __cplusplus
extern "C" { 
#endif

    typedef struct {

	int uri_type;
	int mobile_type;

	char *key;
	int key_size;

	char *url;
	int url_size;

	char *domain;
	int domain_size;

    } site_builder_uri_t;

    extern int site_builder_uri_parse(ngx_http_request_t *r,
	    const char *location_name,
	    int location_name_len,
	    site_builder_uri_t *uri);

#ifdef __cplusplus
}
#endif

#endif


