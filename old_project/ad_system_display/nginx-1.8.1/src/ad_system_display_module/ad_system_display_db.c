
#include "lg_string.h"
#include "ad_system_display_module.h"
#include "ad_system_display_packet.h"
#include "ad_system_display_db.h"

void ad_system_db_print(char *buffer, size_t size)
{

    size_t i;
    char *str = buffer;
    for (i = 0; i < size; i++) {

	if (buffer[i] == CH_COMM) {

	    buffer[i] = 0;
	    fprintf(stdout, "[%s]\n", str);
	    str = &buffer[i + 1];

	} else if (buffer[i] == CH_ROW) {

	    buffer[i] = 0;
	    fprintf(stdout, "[%s]\n", str);
	    fprintf(stdout, "============================\n");
	    str = &buffer[i + 1];
	}
    }

    if (str == buffer) {
	fprintf(stdout, "%s\n", buffer);
	fprintf(stdout, "============================\n");
    }
}

size_t ad_system_db_command(ngx_http_request_t *r, char *cmd, char **buffer)
{

    *buffer = NULL;
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCK_PATH);

    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) < 0) {
	close(sockfd);
	return 0;
    }

    if (write(sockfd, cmd, strlen(cmd)) <= 0) {
	close(sockfd);
	return 0;
    }

    size_t i, size = 0;
    if (read(sockfd, &size, sizeof(size_t)) <= 0) {
	close(sockfd);
	return 0;
    }

    char *p_buffer = ngx_pcalloc(r->pool, size);
    if (!p_buffer) {
	close(sockfd);
	return 0;
    }

    for (i = 0; i < size; i++) {

	if (read(sockfd, &p_buffer[i], 1) <= 0) {
	    close(sockfd);
	    return 0;
	}
    }

    p_buffer[size] = 0;
    *buffer = p_buffer;
    close(sockfd);
    return size;
}

ad_system_code_t *ad_system_db_get_code(ngx_http_request_t *r, char *cmd)
{

    char __cmd[1024];
    sprintf(__cmd, "GetCode %s", cmd);

    char *buffer = NULL;
    size_t size = ad_system_db_command(r, __cmd, &buffer);
    if (!buffer || !size) return NULL;
    if (strcmp(buffer, "not data") == 0)
	return NULL;

    char *code = lg_string_cutting(buffer, CH_COMM);
    if (!code) return NULL;

    if (lg_string_cutting(code, CH_ROW) == NULL)
	return NULL;

    ad_system_code_t *db_code = ngx_pcalloc(r->pool, sizeof(ad_system_code_t));
    db_code->type = atoi(buffer);
    db_code->code = code;
    return db_code;
}

ad_system_code_t *ad_system_db_get_place(ngx_http_request_t *r, char *cmd)
{

    char __cmd[1024];
    sprintf(__cmd, "GetPlace %s", cmd);

    char *buffer = NULL;
    size_t size = ad_system_db_command(r, __cmd, &buffer);
    if (!buffer || !size) return NULL;
    if (strcmp(buffer, "not data") == 0)
	return NULL;

    char *code = lg_string_cutting(buffer, CH_COMM);
    if (!code) return NULL;

    if (lg_string_cutting(code, CH_ROW) == NULL)
	return NULL;

    ad_system_code_t *db_code = ngx_pcalloc(r->pool, sizeof(ad_system_code_t));
    db_code->type = atoi(buffer);
    db_code->code = code;
    return db_code;
}

ad_system_info_img_t *__ad_system_db_get_info_img(ngx_http_request_t *r, char *str[], int index)
{

    fprintf(stdout, "index=%d\n", index);

    if (index != 10) return NULL;
    ad_system_info_img_t *info = ngx_pcalloc(r->pool, sizeof(ad_system_info_img_t));
    if (!info) return NULL;

    info->index			= str[0];
    info->account_id 		= str[1];
    info->campaign_id 		= str[2];
    info->adgroup_id 		= str[3];
    info->creative_id 		= str[4];
    info->img_size		= str[5];
    info->img_path 		= str[6];
    info->destination_url 	= str[7];
    info->display_url 		= str[8];
    info->webim 		= str[9];
    return info;
}

ngx_queue_t *ad_system_db_get_info_img(ngx_http_request_t *r, char *region_code, char *disease_code)
{

    ngx_queue_t *head = ngx_pcalloc(r->pool, sizeof(ngx_queue_t));
    ngx_queue_init(head);

    // get buffer
    char __cmd[1024];
    sprintf(__cmd, "GetInfoImg %s %s", region_code, disease_code);

    char *buffer = NULL;
    size_t size = ad_system_db_command(r, __cmd, &buffer);
    if (!buffer || !size) return NULL;
    if (strcmp(buffer, "not data") == 0)
	return NULL;

    // ad_system_db_print(buffer, size);

    // parse
    char *str[128];
    char row_ch[2], *row_save = NULL;
    char comm_ch[2], *comm_save = NULL;
    sprintf(row_ch, "%c", CH_ROW);
    sprintf(comm_ch, "%c", CH_COMM);

    int index = 0;
    char *row_str = strtok_r(buffer, row_ch, &row_save);
    while(row_str) {

	index = 0;
	memset(str, 0, sizeof(char *));

	char *comm_str = strtok_r(row_str, comm_ch, &comm_save); 
	while(comm_str) {

	    if (strcmp(comm_str, "null") == 0)
		str[index++] = "";
	    else
		str[index++] = comm_str;

	    comm_str = strtok_r(NULL, comm_ch, &comm_save); 
	}

	ad_system_info_img_t *info = __ad_system_db_get_info_img(r, str, index);
	if (!info) return NULL;
	ngx_queue_insert_tail(head, &info->next);

	row_str = strtok_r(NULL, row_ch, &row_save);
    }

    if (index != 10) return NULL;
    return head;
}

ad_system_info_txt_t *__ad_system_db_get_info_txt(ngx_http_request_t *r, char *str[], int index)
{

    fprintf(stdout, "index=%d\n", index);

    if (index != 12) return NULL;
    ad_system_info_txt_t *info = ngx_pcalloc(r->pool, sizeof(ad_system_info_txt_t));
    if (!info) return NULL;

    info->index				= str[0];
    info->account_id			= str[1];
    info->campaign_id			= str[2];
    info->adgroup_id			= str[3];
    info->creative_id			= str[4];
    info->title				= str[5];
    info->description			= str[6];
    info->destination_url		= str[7];
    info->display_url			= str[8];
    info->mobile_destination_url	= str[9];
    info->mobile_display_url		= str[10];
    info->webim				= str[11];
    return info;
}

ngx_queue_t *ad_system_db_get_info_txt(ngx_http_request_t *r, char *region_code, char *disease_code)
{

    ngx_queue_t *head = ngx_pcalloc(r->pool, sizeof(ngx_queue_t));
    ngx_queue_init(head);

    // get buffer
    char __cmd[1024];
    sprintf(__cmd, "GetInfoTxt %s %s", region_code, disease_code);

    char *buffer = NULL;
    size_t size = ad_system_db_command(r, __cmd, &buffer);
    if (!buffer || !size) return NULL;
    if (strcmp(buffer, "not data") == 0)
	return NULL;

    // ad_system_db_print(buffer, size);

    // parse
    char *str[128];
    char row_ch[2], *row_save = NULL;
    char comm_ch[2], *comm_save = NULL;
    sprintf(row_ch, "%c", CH_ROW);
    sprintf(comm_ch, "%c", CH_COMM);

    int index = 0;
    char *row_str = strtok_r(buffer, row_ch, &row_save);
    while(row_str) {

	index = 0;
	char *comm_str = strtok_r(row_str, comm_ch, &comm_save); 
	while(comm_str) {

	    if (strcmp(comm_str, "null") == 0)
		str[index++] = "";
	    else
		str[index++] = comm_str;

	    comm_str = strtok_r(NULL, comm_ch, &comm_save); 
	}

	ad_system_info_txt_t *info = __ad_system_db_get_info_txt(r, str, index);
	if (!info) return NULL;
	ngx_queue_insert_tail(head, &info->next);

	row_str = strtok_r(NULL, row_ch, &row_save);
    }

    if (index != 12) return NULL;
    return head;
}



