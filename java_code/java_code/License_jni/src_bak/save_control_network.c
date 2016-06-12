
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curl/curl.h>
#include "control_deivers.h"
#include "control_network.h"

static char post_url[] = "http://192.168.10.88:1234";
//static char post_url[] = "http://192.168.10.208:8090/security/info.asp";

void control_key_coding(char *buffer, int size)
{

    int i, key;
    for (i = 0; i < size; i++) {

	if (i % 2)
	    buffer[i] = ~buffer[i];

	if (!(i % 3))
	    key = ((i + 1) * 5);
	else if (!(i % 4))
	    key = ((i + 2) / 3);
	else if (i % 5)
	    key = ((i + 3) * 4);
	else
	    key = ((i + 4) / 2);

	if (key % 2)
	    buffer[i] = (buffer[i] + key);
	else
	    buffer[i] = (buffer[i] - key);
    }
}

void control_key_decoding(char *buffer, int size)
{

    int i, key;
    for (i = 0; i < size; i++) {

	if (!(i % 3)) 
	    key = ((i + 1) * 5);
	else if (!(i % 4))
	    key = ((i + 2) / 3);
	else if (i % 5)
	    key = ((i + 3) * 4);
	else
	    key = ((i + 4) / 2);

	if (key % 2)
	    buffer[i] = (buffer[i] - key);
	else
	    buffer[i] = (buffer[i] + key);

	if (i % 2)
	    buffer[i] = ~buffer[i];
    }
}


size_t control_network_recv(void *buffer, size_t size, size_t nmemb, char *recv_buffer) 
{

    memcpy(recv_buffer, buffer, nmemb);
    recv_buffer[nmemb] = 0;
    return nmemb;
}  

int control_network_post(char *send_buffer, char *recv_buffer)
{

#if 0
    struct curl_httppost *post = NULL;
    struct curl_httppost *last = NULL;

    if (curl_formadd(&post, &last,
		CURLFORM_COPYNAME, "License",
		CURLFORM_BUFFER, "License",
		CURLFORM_BUFFERPTR, buffer,
		CURLFORM_BUFFERLENGTH, size,
		CURLFORM_END))
	return -1;
    if (curl_formadd(&post, &last, 
		CURLFORM_COPYNAME, "License",
		CURLFORM_FILE, "./License",
		CURLFORM_END))
	return -1;
    curl_formadd(&post, &last,
	    CURLFORM_COPYNAME, "License",
	    CURLFORM_COPYCONTENTS, buffer,
	    CURLFORM_END);
#endif

    CURL *curl = curl_easy_init();
    if(curl == NULL)
	return -1;

    //curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_URL, post_url);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "License");
    //curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1);

    struct curl_slist *headers = NULL;
    curl_slist_append(headers, "Content-Type: application/octet-stream");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); 

    //curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, send_buffer);

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buffer);   
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, control_network_recv);   

    CURLcode ret = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    //curl_formfree(post);
    return ret;
}

int main(int argc, char *argv[])
{

    srand(time(NULL));
    int rand_num = rand();

    char mac_addr[128] = {0};
    get_deivers_mac_addr(mac_addr);

    char cpu_id[128] = {0};
    get_deivers_cpuid(cpu_id);

    char disk_id[128] = {0};
    get_deivers_disk(disk_id);

    char buffer[4096] = {0};
    int size = sprintf(buffer, "%u\n%s\n%s\n%s\n%s\n", rand_num, mac_addr, cpu_id, disk_id, "licence");
    control_key_coding(buffer, size);

    char recv_buffer[4096];
    int ret = control_network_post("data=%EF%BF%BD7%EF%BF%BD%06%EF%BF%BD%3E", recv_buffer);

    fprintf(stdout, "ret=%d\n",ret);
    fprintf(stdout, "%s\n", recv_buffer);
    return 0;
}


