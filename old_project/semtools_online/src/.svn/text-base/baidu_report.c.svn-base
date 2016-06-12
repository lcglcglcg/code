
#include "baidu_report.h"

void utf8_to_gbk(char *in_buff, size_t in_size, char *out_buff, size_t out_size)
{

    char *in = in_buff;
    char *out = out_buff;
    iconv_t cd = iconv_open("GBK", "UTF-8");
    iconv(cd, &in, &in_size, &out, &out_size);
    iconv_close(cd);
}

void gbk_to_utf8(char *in_buff, size_t in_size, char *out_buff, size_t out_size)
{

    char *in = in_buff;
    char *out = out_buff;
    iconv_t cd = iconv_open("UTF-8", "GBK");
    iconv(cd, &in, &in_size, &out, &out_size);
    iconv_close(cd);
}

baidu_pair_report_node_t *baidu_pair_report_analysis(char *buffer)
{

    char *csv_account_ID = NULL;//帐号ID
    char *csv_account_str = NULL;//帐号
    char *csv_plan_ID = NULL;//推广计划ID
    char *csv_plan_str = NULL;//推广计划
    char *csv_unit_ID = NULL;//推广单元ID
    char *csv_unit_str = NULL;//推广单元
    char *csv_keyword_ID = NULL;//关键词ID
    char *csv_keyword_str = NULL;//关键词
    char *csv_creat_ID = NULL; //创意ID
    char *csv_creat_title = NULL;//创意标题
    char *csv_creat_depiction_1 = NULL;//创意描述1
    char *csv_creat_depiction_2 = NULL;//创意描述2
    char *csv_show_url = NULL;//显示URL
    char *csv_method = NULL;//推广方式
    char *csv_impression = NULL;//展现量
    char *csv_click = NULL;//点击量
    char *csv_cost = NULL;//消费

    for (; *buffer; buffer++) {

	if (*buffer == '\t') {

	    *buffer = 0;
	    if (csv_account_ID == NULL)
		csv_account_ID = buffer + 1;
	    else if (csv_account_str == NULL)
		csv_account_str = buffer + 1;
	    else if (csv_plan_ID == NULL)
		csv_plan_ID = buffer + 1;
	    else if (csv_plan_str == NULL)
		csv_plan_str = buffer + 1;
	    else if (csv_unit_ID == NULL)
		csv_unit_ID = buffer + 1;
	    else if (csv_unit_str == NULL)
		csv_unit_str = buffer + 1;
	    else if (csv_keyword_ID == NULL)
		csv_keyword_ID = buffer + 1;
	    else if (csv_keyword_str == NULL)
		csv_keyword_str = buffer + 1;
	    else if (csv_creat_ID == NULL)
		csv_creat_ID = buffer + 1;
	    else if (csv_creat_title == NULL)
		csv_creat_title = buffer + 1;
	    else if (csv_creat_depiction_1 == NULL)
		csv_creat_depiction_1 = buffer + 1;
	    else if (csv_creat_depiction_2 == NULL)
		csv_creat_depiction_2 = buffer + 1;
	    else if (csv_show_url == NULL)
		csv_show_url = buffer + 1;
	    else if (csv_method == NULL)
		csv_method = buffer + 1;
	    else if (csv_impression == NULL)
		csv_impression = buffer + 1;
	    else if (csv_click == NULL)
		csv_click = buffer + 1;
	    else if (csv_cost == NULL)
		csv_cost = buffer + 1;

	} else if (*buffer == '\r' || *buffer == '\n') {

	    *buffer = 0;
	    if (csv_cost == NULL)
		return NULL;
	}
    }

    if (!csv_account_ID ||
	    !csv_account_str ||
	    !csv_plan_ID ||
	    !csv_plan_str ||
	    !csv_unit_ID ||
	    !csv_unit_str ||
	    !csv_keyword_ID ||
	    !csv_keyword_str ||
	    !csv_creat_ID ||
	    !csv_creat_title ||
	    !csv_creat_depiction_1 ||
	    !csv_creat_depiction_2 ||
	    !csv_show_url ||
	    !csv_method ||
	    !csv_impression ||
	    !csv_click ||
	    !csv_cost) {

	return NULL;
    }

    baidu_pair_report_node_t *node = NULL;
    if (!(node = malloc(sizeof(baidu_pair_report_node_t))))
	return NULL;

    memset(node, 0, sizeof(baidu_pair_report_node_t));
    node->account_ID = atoi(csv_account_ID);
    gbk_to_utf8(csv_account_str, strlen(csv_account_str), node->account_str, 256);
    node->plan_ID = atoi(csv_plan_ID);
    gbk_to_utf8(csv_plan_str, strlen(csv_plan_str), node->plan_str, 256);
    node->unit_ID = atoi(csv_unit_ID);
    gbk_to_utf8(csv_unit_str, strlen(csv_unit_str), node->unit_str, 256);
    node->keyword_ID = atoll(csv_keyword_ID);
    gbk_to_utf8(csv_keyword_str, strlen(csv_keyword_str), node->keyword_str, 256);
    gbk_to_utf8(csv_show_url, strlen(csv_show_url), node->show_url, 256);
    node->impression = atoi(csv_impression);
    node->click = atoi(csv_click);
    node->cost = atof(csv_cost);
    return node;
}

baidu_pair_report_node_t *baidu_pair_report_load(char *path)
{

    FILE *fp = NULL;
    char buffer[4096];
    baidu_pair_report_node_t *head = NULL;
    baidu_pair_report_node_t *node = NULL;

    if (!(fp = fopen(path, "rb")))
	return NULL;

    fgets(buffer, 4096, fp);
    while(fgets(buffer, 4096, fp)) {

	if ((node = baidu_pair_report_analysis(buffer))) {

	    if (!head) {

		head = node;

	    } else {

		node->next = head;
		head = node;
	    }
	}
    }

    fclose(fp);
    return head;
}

size_t baidu_pair_report_day_save(baidu_pair_report_node_t *head, char *path)
{

    int fd = -1;
    size_t size = 0, size_max = 0;
    if ((fd = open(path, O_WRONLY | O_TRUNC | O_CREAT, 0755)) == -1)
	return -1;

    baidu_pair_report_node_t *node = NULL;
    for (node = head; node; node = node->next) {

	if ((size = write(fd, node, sizeof(baidu_pair_report_node_t))) > 0) {

	    size_max += size;

	} else {

	    close(fd);
	    return -1;
	}
    }

    close(fd);
    return size_max;
}

baidu_pair_report_node_t *baidu_pair_report_day_load(baidu_pair_report_node_t *head, char *path)
{

    int fd = -1;
    if ((fd = open(path, O_RDONLY)) == -1)
	return head;

    baidu_pair_report_node_t baidu_pair_report_buffer = {0};
    while (read(fd, &baidu_pair_report_buffer, sizeof(baidu_pair_report_node_t)) > 0) {

	baidu_pair_report_node_t *node = NULL;
	if ((node = malloc(sizeof(baidu_pair_report_node_t)))) {

	    memcpy(node, &baidu_pair_report_buffer, sizeof(baidu_pair_report_node_t));
	    node->next = NULL;

	    if (head == NULL) {

		head = node;

	    } else {

		node->next = head;
		head = node;
	    }
	}
    }

    close(fd);
    return head;
}

void baidu_pair_report_print(baidu_pair_report_node_t *head)
{

    baidu_pair_report_node_t *node = NULL;
    for (node = head; node; node = node->next)
	fprintf(stdout, "%u,%s,%s\n",
		node->keyword_ID,
		node->keyword_str,
		node->show_url);
}

void baidu_pair_report_free(baidu_pair_report_node_t *head)
{

    baidu_pair_report_node_t *temp = head;
    baidu_pair_report_node_t *temp_free = NULL;

    while(temp) {

	temp_free = temp;
	temp = temp->next;
	free(temp_free);
    }
}

baidu_search_report_node_t *baidu_search_report_analysis(char *buffer)
{

    char *csv_account_ID = NULL;//帐号ID
    char *csv_account_str = NULL;//帐号
    char *csv_plan_ID = NULL;//推广计划ID
    char *csv_plan_str = NULL;//推广计划
    char *csv_unit_ID = NULL;//推广单元ID
    char *csv_unit_str = NULL;//推广单元
    char *csv_creat_ID = NULL; //创意ID
    char *csv_creat_title = NULL;//创意标题
    char *csv_creat_depiction_1 = NULL;//创意描述1
    char *csv_creat_depiction_2 = NULL;//创意描述2
    char *csv_show_url = NULL;//显示URL
    char *csv_keyword_ID = NULL;//关键词ID
    char *csv_keyword_str = NULL;//关键词
    char *csv_search_str = NULL;//搜索词
    char *csv_impression = NULL;//展现量
    char *csv_click = NULL;//点击量

    for (; *buffer; buffer++) {

	if (*buffer == '\t') {

	    *buffer = 0;
	    if (csv_account_ID == NULL)
		csv_account_ID = buffer + 1;
	    else if (csv_account_str == NULL)
		csv_account_str = buffer + 1;
	    else if (csv_plan_ID == NULL)
		csv_plan_ID = buffer + 1;
	    else if (csv_plan_str == NULL)
		csv_plan_str = buffer + 1;
	    else if (csv_unit_ID == NULL)
		csv_unit_ID = buffer + 1;
	    else if (csv_unit_str == NULL)
		csv_unit_str = buffer + 1;
	    else if (csv_creat_ID == NULL)
		csv_creat_ID = buffer + 1;
	    else if (csv_creat_title == NULL)
		csv_creat_title = buffer + 1;
	    else if (csv_creat_depiction_1 == NULL)
		csv_creat_depiction_1 = buffer + 1;
	    else if (csv_creat_depiction_2 == NULL)
		csv_creat_depiction_2 = buffer + 1;
	    else if (csv_show_url == NULL)
		csv_show_url = buffer + 1;
	    else if (csv_keyword_ID == NULL)
		csv_keyword_ID = buffer + 1;
	    else if (csv_keyword_str == NULL)
		csv_keyword_str = buffer + 1;
	    else if (csv_search_str == NULL)
		csv_search_str = buffer + 1;
	    else if (csv_impression == NULL)
		csv_impression = buffer + 1;
	    else if (csv_click == NULL)
		csv_click = buffer + 1;

	} else if (*buffer == '\r' || *buffer == '\n') {

	    *buffer = 0;
	    if (csv_click == NULL)
		return NULL;
	}
    }

    if (!csv_account_ID ||
	    !csv_account_str ||
	    !csv_plan_ID ||
	    !csv_plan_str ||
	    !csv_unit_ID ||
	    !csv_unit_str ||
	    !csv_creat_ID ||
	    !csv_creat_title ||
	    !csv_creat_depiction_1 ||
	    !csv_creat_depiction_2 ||
	    !csv_show_url ||
	    !csv_keyword_ID ||
	    !csv_keyword_str ||
	    !csv_search_str ||
	    !csv_impression ||
	    !csv_click) {

	return NULL;
    }

    baidu_search_report_node_t *node = NULL;
    if (!(node = malloc(sizeof(baidu_search_report_node_t))))
	return NULL;

    memset(node, 0, sizeof(baidu_search_report_node_t));
    node->keyword_ID = atoll(csv_keyword_ID);
    gbk_to_utf8(csv_search_str, strlen(csv_search_str), node->search_str, 256);
    gbk_to_utf8(csv_show_url, strlen(csv_show_url), node->show_url, 256);
    node->impression = atoi(csv_impression);
    node->click = atoi(csv_click);
    return node;
}

baidu_search_report_node_t *baidu_search_report_load(char *path)
{

    FILE *fp = NULL;
    char buffer[40960];
    baidu_search_report_node_t *head = NULL;
    baidu_search_report_node_t *node = NULL;

    if (!(fp = fopen(path, "rb")))
	return NULL;

    fgets(buffer, 40960, fp);
    while(fgets(buffer, 40960, fp)) {

	if ((node = baidu_search_report_analysis(buffer))) {

	    if (!head) {

		head = node;

	    } else {

		node->next = head;
		head = node;
	    }
	}
    }

    fclose(fp);
    return head;
}

size_t baidu_search_report_day_save(baidu_search_report_node_t *head, char *path)
{

    int fd = -1;
    size_t size = 0, size_max = 0;
    if ((fd = open(path, O_WRONLY | O_TRUNC | O_CREAT, 0755)) == -1)
	return -1;

    baidu_search_report_node_t *node = NULL;
    for (node = head; node; node = node->next) {

	if ((size = write(fd, node, sizeof(baidu_search_report_node_t))) > 0) {

	    size_max += size;

	} else {

	    close(fd);
	    return -1;
	}
    }

    close(fd);
    return size_max;
}

baidu_search_report_node_t *baidu_search_report_day_load(baidu_search_report_node_t *head, char *path)
{

    int fd = -1;
    if ((fd = open(path, O_RDONLY)) == -1)
	return NULL;

    baidu_search_report_node_t baidu_search_report_buffer = {0};
    while (read(fd, &baidu_search_report_buffer, sizeof(baidu_search_report_node_t)) > 0) {

	baidu_search_report_node_t *node = NULL;
	if ((node = malloc(sizeof(baidu_search_report_node_t)))) {

	    memcpy(node, &baidu_search_report_buffer, sizeof(baidu_search_report_node_t));
	    node->next = NULL;

	    if (!head) {

		head = node;

	    } else {

		node->next = head;
		head = node;
	    }
	}
    }

    close(fd);
    return head;
}

void baidu_search_report_print(baidu_search_report_node_t *head)
{

    baidu_search_report_node_t *node = NULL;
    for (node = head; node; node = node->next)
	fprintf(stdout, "%u,%s,%s\n",
		node->keyword_ID,
		node->search_str,
		node->show_url);
}

void baidu_search_report_free(baidu_search_report_node_t *head)
{

    baidu_search_report_node_t *temp = head;
    baidu_search_report_node_t *temp_free = NULL;

    while(temp) {

	temp_free = temp;
	temp = temp->next;
	free(temp_free);
    }
}

int baidu_search_report_date(char *start_time_str, char *end_time_str)
{

    char path[1024];
    struct tm date_tm = {0};

    strptime(start_time_str, "%Y-%m-%d", &date_tm);
    time_t start_time = mktime(&date_tm) + 86400;

    strptime(end_time_str, "%Y-%m-%d", &date_tm);
    time_t end_time = mktime(&date_tm) + 86400;

    while(start_time <= end_time) {

	gmtime_r(&start_time, &date_tm);
	start_time += 86400;

	sprintf(path, "%04d_%02d_%02d",
		date_tm.tm_year + 1900,
		date_tm.tm_mon + 1,
		date_tm.tm_mday);

	fprintf(stdout, "%s\n", path);
    }

    return 0;
}


