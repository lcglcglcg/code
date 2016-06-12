
#ifndef	__AD_SYSTEM_DISPLAY_DB_H__
#define	__AD_SYSTEM_DISPLAY_DB_H__

#include <nginx.h>
#include <ngx_core.h>
#include <ngx_http.h>

#define CH_COMM	2	//分割符
#define CH_ROW	22	//结束符
#define NOT_DATA_STR "not data"
#define NOT_DATA_SIZE 8
#define SOCK_PATH "/tmp/ad_system.sock"

typedef struct {

    int type;
    char *code;

} ad_system_code_t;

typedef struct {

    ngx_queue_t next;
    char *index;
    char *account_id;
    char *campaign_id;
    char *adgroup_id;
    char *creative_id;
    char *img_size;
    char *img_path;
    char *destination_url;
    char *display_url;
    char *webim;

} ad_system_info_img_t;

typedef struct {

    ngx_queue_t next;
    char *index;
    char *account_id;
    char *campaign_id;
    char *adgroup_id;
    char *creative_id;
    char *title;
    char *description;
    char *destination_url;
    char *display_url;
    char *mobile_destination_url;
    char *mobile_display_url;
    char *webim;

} ad_system_info_txt_t;

extern size_t ad_system_db_command(ngx_http_request_t *r, char *str, char **buffer);
extern ad_system_code_t *ad_system_db_get_code(ngx_http_request_t *r, char *cmd);
extern ad_system_code_t *ad_system_db_get_place(ngx_http_request_t *r, char *cmd);
extern ngx_queue_t *ad_system_db_get_info_img(ngx_http_request_t *r, char *region_code, char *disease_code);
extern ngx_queue_t *ad_system_db_get_info_txt(ngx_http_request_t *r, char *region_code, char *disease_code);

#endif


