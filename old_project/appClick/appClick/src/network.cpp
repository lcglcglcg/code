
#include "network.h"

struct in_addr network_host_to_addr(char *host)
{

    struct sockaddr_in addr = {0};
    struct hostent *phostent = NULL;

    if((phostent = gethostbyname(host)) == NULL) {

	return addr.sin_addr;
    }

    if (phostent->h_addr_list[0]) {

	memcpy(&addr.sin_addr.s_addr, phostent->h_addr_list[0], phostent->h_length);
    }

    return addr.sin_addr;
}

SOCKET network_connect(char *ip_addr, int port)
{

    int retval;
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr = network_host_to_addr(ip_addr);
    if (!addr.sin_addr.s_addr) {

	return FALSE;
    }

    SOCKET sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET) {

	return FALSE;
    }

    struct timeval time_out;
    time_out.tv_sec = 10;
    time_out.tv_usec = 0;
    retval = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&time_out, sizeof(time_out));
    if (retval == -1) {

	closesocket(sockfd);
	return FALSE;
    }

    retval = connect(sockfd, (SOCKADDR*)&addr, sizeof(addr));
    if (retval == -1) {

	closesocket(sockfd);
	return FALSE;
    }

    return sockfd;
}

int network_send(SOCKET sockfd, void *buffer, int buffer_size)
{

    int size = send(sockfd, (char *)buffer, buffer_size, 0);
    if (size == -1 || size != buffer_size) {

	return FALSE;
    }

    return size;
}

int network_recv(SOCKET sockfd, void *buffer, int buffer_size)
{

    int size = recv(sockfd, (char *)buffer, buffer_size, 0);
    if (size == -1) {

	return FALSE;
    }

    return size;
}

int network_packet_click_send(SOCKET sockfd, task_info_t *task_info)
{

    char url_str[2048] = {0};
    char link_str[2048] = {0};
    char cookie_str[4096] = {0};

    if (*task_info->url_str) {

	if (WideCharToMultiByte(CP_ACP, 0,
		    task_info->url_str, wcslen(task_info->url_str),
		    url_str, 2048,
		    NULL, NULL) == FALSE) {

	    return FALSE;
	}
    }

    if (*task_info->link_str) {

	if (WideCharToMultiByte(CP_ACP, 0,
		    task_info->link_str, wcslen(task_info->link_str),
		    link_str, 2048,
		    NULL, NULL) == FALSE) {

	    return FALSE;
	}
    }

    if (*task_info->cookie_str) {

	if (WideCharToMultiByte(CP_ACP, 0,
		    task_info->cookie_str, wcslen(task_info->cookie_str),
		    cookie_str, 2048,
		    NULL, NULL) == FALSE) {

	    return FALSE;
	}
    }

    json_object *jroot = NULL;
    if (!(jroot = json_object_new_object()))
	return FALSE;

    json_object_object_add(jroot, "url", *url_str?json_object_new_string(url_str):NULL);
    json_object_object_add(jroot, "link", *link_str?json_object_new_string(link_str):NULL);
    json_object_object_add(jroot, "cookie", *cookie_str?json_object_new_string(cookie_str):NULL);

    char *gbk_buffer = (char *)json_object_to_json_string(jroot);
    fprintf(stdout, "[send]%s\n", gbk_buffer);

    wchar_t wbuffer[4096] = {0};
    if (MultiByteToWideChar(CP_ACP, 0,
		gbk_buffer, -1, 
		wbuffer, 4096) == FALSE) {

	json_object_put(jroot);
	return FALSE;
    }

    char buffer[4096] = {0};
    if (WideCharToMultiByte(CP_UTF8, 0,
		wbuffer, -1, 
		buffer, 4096, NULL, NULL) == FALSE) {

	json_object_put(jroot);
	return FALSE;
    }

    if (network_send(sockfd, buffer, strlen(buffer)) == FALSE) {

	json_object_put(jroot);
	return FALSE;
    }

    json_object_put(jroot);
    return TRUE;
}

int network_packet_click_recv(SOCKET sockfd, task_info_t *task_info)
{

    char buffer[4096] = {0};
    if (network_recv(sockfd, buffer, 4096) == FALSE)
	return FALSE;

    wchar_t wbuffer[4096] = {0};
    if (MultiByteToWideChar(CP_UTF8, 0, 
		buffer, -1, 
		wbuffer, 4096) == FALSE) {

	return FALSE;
    }

    memset(buffer, 0, 4096);
    if (WideCharToMultiByte(CP_ACP, 0, 
		wbuffer, -1, 
		buffer, 4096, NULL, NULL) == FALSE) {

	return FALSE;
    }

    fprintf(stdout, "[recv]%s\n", buffer);

    json_object *j_obj = NULL;
    if (!(j_obj = json_tokener_parse(buffer)))
	return FALSE;

    json_object *j_url = NULL;
    if (!(j_url = json_object_object_get(j_obj, "url"))) {

	json_object_put(j_obj);
	return FALSE;
    }

    json_object *j_link = NULL;
    if (!(j_link = json_object_object_get(j_obj, "link"))) {

	json_object_put(j_obj);
	return FALSE;
    }

    json_object *j_time = NULL;
    if (!(j_time = json_object_object_get(j_obj, "time"))) {

	json_object_put(j_obj);
	return FALSE;
    }

    char *url_str = (char *)json_object_get_string(j_url);
    char *link_str = (char *)json_object_get_string(j_link);
    char *time_str = (char *)json_object_get_string(j_time);
    if (!url_str || !link_str || !time_str) {

	json_object_put(j_obj);
	return FALSE;
    }

    memset(task_info, 0, sizeof(task_info_t));

    if (MultiByteToWideChar(CP_ACP, 0,
		url_str, strlen(url_str), 
		task_info->url_str, 2048) == FALSE) {

	json_object_put(j_obj);
	return FALSE;
    }

    if (MultiByteToWideChar(CP_ACP, 0,
		link_str, strlen(link_str), 
		task_info->link_str, 2048) == FALSE) {

	json_object_put(j_obj);
	return FALSE;
    }

    if (!(task_info->delay_time = atoi(time_str))) {

	json_object_put(j_obj);
	return FALSE;
    }

    json_object_put(j_obj);
    return TRUE;
}


