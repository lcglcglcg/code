
#include "zoosnet_report.h"

void zoosnet_report_time_get(
	zoosnet_report_head_t *head,
	time_t *start_time, 
	time_t *end_time)
{

    time_t date_time;
    struct tm date_tm;
    zoosnet_report_head_t *temp = NULL;

    if (!head)return;
    memset(&date_tm, 0, sizeof(date_tm));
    strptime(head->date_str, "%Y-%m-%d", &date_tm);
    date_time = mktime(&date_tm) + 86400;
    *start_time = date_time;
    *end_time = date_time;

    for (temp = head; temp; temp = temp->next) {

	memset(&date_tm, 0, sizeof(date_tm));
	strptime(temp->date_str, "%Y-%m-%d", &date_tm);
	date_time = mktime(&date_tm) + 86400;

	if (date_time < *start_time)
	    *start_time = date_time;

	if (date_time > *end_time)
	    *end_time = date_time;
    }
}

char *zoosnet_report_time_analysis(char *str, char *date_str)
{

    struct tm date_tm = {0};

    if (str[4] == '-' || str[2] == '-')
	strptime(str, "%Y-%m-%d", &date_tm);
    else if (str[4] == '/' || str[2] == '/')
	strptime(str, "%Y/%m/%d", &date_tm);
    else
	return NULL;

    sprintf(date_str, "%04d-%02d-%02d",
	    date_tm.tm_year + 1900,
	    date_tm.tm_mon + 1,
	    date_tm.tm_mday);

    return date_str;
}

void zoosnet_report_baidu_url(char *buffer, zoosnet_report_baidu_url_t *baidu_url)
{

    char *baidu_ad_str = NULL;
    char *baidu_kw_str = NULL;
    char *baidu_net_str = NULL;
    //char *baidu_type_str = NULL;

    if ((baidu_ad_str = strstr(buffer, "baidu-ad="))) {

	baidu_ad_str += strlen("baidu-ad=");
	baidu_url->baidu_ad = atoll(baidu_ad_str);
    }

    if ((baidu_kw_str = strstr(buffer, "baidu-kw="))) {

	baidu_kw_str += strlen("baidu-kw=");
	baidu_url->baidu_kw = atoll(baidu_kw_str);
    }

    if ((baidu_net_str = strstr(buffer, "baidu-net="))) {

	baidu_net_str += strlen("baidu-net=");
	baidu_url->baidu_net = atoll(baidu_net_str);
    }

    if (strstr(buffer, "baidu-type=1"))
	baidu_url->baidu_type = 1;
}

void zoosnet_report_dialog_analysis(
	zoosnet_report_node_t *node,
	char *dialog_type,
	int service_count_col,
	int guest_count_col)
{

    if (strcmp(dialog_type, "仅访问") == 0) {

	node->dialog_type = 1;

    } else if (strcmp(dialog_type, "客服未接受") == 0) {

	node->dialog_type = 2;

    } else if (strcmp(dialog_type, "其它有效对话") == 0) {

	node->dialog_type = 3;

    } else if (strcmp(dialog_type, "客服无讯息") == 0) {

	node->dialog_type = 3;

    } else if (strcmp(dialog_type, "客人无讯息") == 0) {

	node->dialog_type = 4;

    } else if (strcmp(dialog_type, "一般对话") == 0) {

	node->dialog_type = 5;

    } else if (strcmp(dialog_type, "较好对话") == 0) {

	node->dialog_type = 6;

    } else if (strcmp(dialog_type, "极佳对话") == 0) {

	node->dialog_type = 7;
    }
}

extern void zonnsnet_reservation_free(reservation_node_t *head);
reservation_node_t *zonnsnet_reservation_init(char *buffer)
{

    char *str = NULL;
    char value[256] = {0};
    char *p = value;
    reservation_node_t *head = NULL;
    reservation_node_t *node = NULL;

    for (str = buffer; str && *str; str++) {

	*p++ = *str;
	if (*str == ',') {

	    *--p = 0;
	    p = value;
	    if(!(node = malloc(sizeof(reservation_node_t)))) {

		zonnsnet_reservation_free(head);
		return NULL;
	    }

	    strcpy(node->str, value);
	    node->next = head;
	    head = node;
	}
    }

    if (value[0]) {

	if (!(node = malloc(sizeof(reservation_node_t)))) {

	    zonnsnet_reservation_free(head);
	    return NULL;
	}

	strcpy(node->str, value);
	node->next = head;
	head = node;
    }

    return head;
}

void zonnsnet_reservation_free(reservation_node_t *head)
{

    reservation_node_t *node = head;
    reservation_node_t *node_free = NULL;

    while(node) {

	node_free = node;
	node = node->next;
	free(node_free);
    }
}

zoosnet_report_head_t *zoosnet_report_analysis(
	zoosnet_report_head_t *head,
	SheetHandle sheet, int row,
	reservation_node_t *reservation_list,
	reservation_node_t *not_reservation_list,
	zoosnet_report_baidu_url_t *baidu_url_info,
	char *baidu_url_str,
	int date_col,
	int search_col,
	int dialog_type_col,
	int service_count_col,
	int guest_count_col,
	int reservation_col)
{

    char *str = NULL;
    char date_str[20] = {0};

    zoosnet_report_node_t *zoosnet_node = NULL;
    if (!(zoosnet_node = malloc(sizeof(zoosnet_report_node_t))))
	return head;

    memset(zoosnet_node, 0, sizeof(zoosnet_report_node_t));
    zoosnet_node->keyword_ID = baidu_url_info->baidu_kw;
    zoosnet_node->mobile_mark = baidu_url_info->baidu_type;
    strcpy(zoosnet_node->baidu_url_str, baidu_url_str);

    //开始时间
    if(!(str = (char *)xlSheetReadStr(sheet, row, date_col, 0))) {

	free(zoosnet_node);
	return head;
    }

    if (!(zoosnet_report_time_analysis(str, date_str))) {

	free(zoosnet_node);
	return head;
    }

    //搜索词
    if ((str = (char *)xlSheetReadStr(sheet, row, search_col, 0)) && str[0])
	strcpy(zoosnet_node->search_str, str);

    //对话类型
    if((str = (char *)xlSheetReadStr(sheet, row, dialog_type_col, 0)))
	zoosnet_report_dialog_analysis(
		zoosnet_node,
		str,
		service_count_col,
		guest_count_col);

    if (reservation_col != -1 && reservation_list) {

	if((str = (char *)xlSheetReadStr(sheet, row, reservation_col, 0))) {

	    reservation_node_t *reservation_node = NULL;
	    for (reservation_node = reservation_list;
		    reservation_node;
		    reservation_node = reservation_node->next) {

		if (strstr(str, reservation_node->str)) {

		    zoosnet_node->reservation = 1;
		   
		    fprintf(stdout, "%s:%p\n", str, not_reservation_list);
		    
		    reservation_node_t *not_reservation_node = NULL;
		    for (not_reservation_node = not_reservation_list;
			    not_reservation_node;
			    not_reservation_node = not_reservation_node->next) {
			
			fprintf(stdout, "%s==%s\n", str, not_reservation_node->str);
			if (strstr(str, not_reservation_node->str)) {

			    zoosnet_node->reservation = 0;
			    break;
			}
		    }

		    break;
		}
	    }
	}
    }

    zoosnet_report_head_t *date_node = NULL;
    for (date_node = head; date_node; date_node = date_node->next) {

	if (strcmp(date_node->date_str, date_str) == 0) {

	    zoosnet_node->next = date_node->head;
	    date_node->head = zoosnet_node;
	    return head;
	}
    }

    if (!(date_node = malloc(sizeof(zoosnet_report_head_t)))) {

	free(zoosnet_node);
	return head;
    }

    strcpy(date_node->date_str, date_str);
    date_node->next = head;
    date_node->head = zoosnet_node;
    return date_node;
}

zoosnet_report_head_t *zoosnet_report_load(
	char *path,
	char *reservation_str,
	char *reservation_col_str,
	char *not_reservation_str)
{

    fprintf(stdout, "%s\n", path);

    BookHandle book = NULL;
    SheetHandle sheet = NULL;

    if (!(book = xlCreateBook()))
	return NULL;

    xlBookSetKeyA(book, "Tao Zhang",
	    "linux-ebd9197b99a8a51e0b01243c4eh2dcm6");

    if(!xlBookLoad(book, path))
	return NULL;

    if (!(sheet = xlBookGetSheet(book, 0)))
	return NULL;

    int row, col;
    int baidu_url_col = -1;
    int search_col = -1;
    int date_col = -1;
    int dialog_type_col = -1;
    int service_count_col = -1;
    int guest_count_col = -1;
    int reservation_col = -1;
    int row_max = xlSheetLastRow(sheet);
    int col_max = xlSheetLastCol(sheet);

    for (col = 0; col < col_max; col++) {

	char *str = NULL;
	if ((str = (char *)xlSheetReadStr(sheet, 2, col, 0))) {

	    if (strcmp("初次访问网址", str) == 0)
		baidu_url_col = col;
	    else if (strcmp("关键词", str) == 0)
		search_col = col;
	    else if (strcmp("开始访问时间", str) == 0)
		date_col = col;
	    else if (strcmp("对话类型", str) == 0)
		dialog_type_col = col;
	    else if (strcmp("客服讯息数", str) == 0)
		service_count_col = col;
	    else if (strcmp("客人讯息数", str) == 0)
		guest_count_col = col;
	    else if (reservation_col_str && strcmp(reservation_col_str, str) == 0)
		reservation_col = col;
	}
    }

    fprintf(stdout, "reservation_col_str=%s reservation_col=%d\n", reservation_col_str, reservation_col);

    if (date_col == -1
	    || baidu_url_col == -1
	    || search_col == -1
	    || dialog_type_col == -1
	    || service_count_col == -1
	    || guest_count_col == -1) {

	xlBookRelease(book);
	return NULL;
    }

    reservation_node_t *reservation_list = NULL;
    reservation_node_t *not_reservation_list = NULL;

    if (reservation_col != -1) {

	reservation_list = zonnsnet_reservation_init(reservation_str);
	not_reservation_list = zonnsnet_reservation_init(not_reservation_str);
    }

    reservation_node_t *reservation_node = NULL;
    for (reservation_node = reservation_list;
	    reservation_node;
	    reservation_node = reservation_node->next)
	fprintf(stdout, "[reservation_str][%s]\n", reservation_node->str);
    for (reservation_node = not_reservation_list;
	    reservation_node;
	    reservation_node = reservation_node->next)
	fprintf(stdout, "[not_reservation_str][%s]\n", reservation_node->str);

    zoosnet_report_head_t *head = NULL;
    for (row = 3; row < row_max; row++) {

	char *baidu_url_str = NULL;
	zoosnet_report_baidu_url_t baidu_url_info = {0};
	memset(&baidu_url_info, 0, sizeof(zoosnet_report_baidu_url_t));

	if ((baidu_url_str = (char *)xlSheetReadStr(sheet, row, baidu_url_col, 0)))
	    zoosnet_report_baidu_url(baidu_url_str, &baidu_url_info);

	if (baidu_url_info.baidu_kw) {

	    head = zoosnet_report_analysis(
		    head, sheet, row,
		    reservation_list,
		    not_reservation_list,
		    &baidu_url_info,
		    baidu_url_str,		    
		    date_col,
		    search_col,
		    dialog_type_col,
		    service_count_col,
		    guest_count_col,
		    reservation_col);
	}
    }

    xlBookRelease(book);
    zonnsnet_reservation_free(reservation_list);
    zonnsnet_reservation_free(not_reservation_list);
    return head;
}

int _zoosnet_report_day_save(zoosnet_report_node_t *head, char *path)
{

    int fd = -1;
    if ((fd = open(path, O_WRONLY | O_TRUNC | O_CREAT, 0755)) == -1)
	return -1;

    zoosnet_report_node_t *node = NULL;
    for (node = head; node; node = node->next) {

	if (write(fd, node, sizeof(zoosnet_report_node_t)) == -1) {

	    close(fd);
	    return -1;
	}
    }

    close(fd);
    return 0;
}

void zoosnet_report_day_save(zoosnet_report_head_t *head, int sem_id, zoosnet_report_day_save_backcall *backcall)
{

    char path[1024] = {0};
    sprintf(path, "%s/%u", ZOOSNET_REPORT_PATH, sem_id);
    if (access(path, 0)) {

	fprintf(stdout, "[mkdir][%s]\n", path);
	if (mkdir(path, 0755) == -1)
	    return;
    }

    zoosnet_report_head_t *date_node = NULL;
    for (date_node = head; date_node; date_node = date_node->next) {

	struct tm date_tm = {0};
	strptime(date_node->date_str, "%Y-%m-%d", &date_tm);

	sprintf(path, "%s/%u/zoosnet_%04d_%02d_%02d",
		ZOOSNET_REPORT_PATH,
		sem_id,
		date_tm.tm_year + 1900,
		date_tm.tm_mon + 1,
		date_tm.tm_mday);

	_zoosnet_report_day_save(date_node->head, path);
    }
}

zoosnet_report_head_t *zoosnet_report_day_load(
	int sem_id, 
	char *start_date, 
	char *end_date)
{

    char path[1024];
    struct tm date_tm = {0};

    strptime(start_date, "%Y-%m-%d", &date_tm);
    time_t start_time = mktime(&date_tm) + 86400;

    strptime(end_date, "%Y-%m-%d", &date_tm);
    time_t end_time = mktime(&date_tm) + 86400;

    zoosnet_report_head_t *head_head = NULL;
    zoosnet_report_head_t *head_temp = NULL;
    zoosnet_report_head_t *head_node = NULL;
    zoosnet_report_node_t *node_head = NULL;
    zoosnet_report_node_t *node_temp = NULL;

    while(start_time <= end_time) {

	struct tm *tm = gmtime(&start_time);
	start_time += 86400;

	sprintf(path, "%s/%u/zoosnet_%04d_%02d_%02d",
		ZOOSNET_REPORT_PATH,
		sem_id,
		tm->tm_year + 1900,
		tm->tm_mon + 1,
		tm->tm_mday);

	int fd = -1;
	node_head = NULL;
	node_temp = NULL;
	if ((fd = open(path, O_RDONLY)) != -1) {

	    zoosnet_report_node_t zoosnet_buffer = {0};
	    while (read(fd, &zoosnet_buffer, sizeof(zoosnet_report_node_t)) > 0) {

		zoosnet_report_node_t *node = NULL;
		if ((node = malloc(sizeof(zoosnet_report_node_t)))) {

		    memcpy(node, &zoosnet_buffer, sizeof(zoosnet_report_node_t));
		    node->next = NULL;

		    if (!node_head)
			node_head = node;

		    if (node_temp) {

			node_temp->next = node;
			node_temp = node;

		    } else {

			node_temp = node;
		    }
		}
	    }

	    close(fd);
	}

	if (node_head) {

	    if ((head_node = malloc(sizeof(zoosnet_report_head_t)))) {

		memset(head_node, 0, sizeof(zoosnet_report_head_t));
		sprintf(head_node->date_str, "%04d-%02d-%02d",
			tm->tm_year + 1900,
			tm->tm_mon + 1,
			tm->tm_mday);

		head_node->head = node_head;
		head_node->next = NULL;

		if (!head_head)
		    head_head = head_node;

		if (head_temp) {

		    head_temp->next = head_node;
		    head_temp = head_node;

		} else {

		    head_temp = head_node;
		}
	    }
	}
    }

    return head_head;
}

void zoosnet_report_print(zoosnet_report_head_t *head)
{

    zoosnet_report_node_t *node = NULL;
    zoosnet_report_head_t *day_node = NULL;

    for (day_node = head; day_node; day_node = day_node->next) {

	for (node = day_node->head; node; node = node->next) {

	    fprintf(stdout, "%s,%u,%s,%u,%u,%u\n",
		    day_node->date_str,
		    node->keyword_ID,
		    node->search_str,
		    node->mobile_mark,
		    node->dialog_type,
		    node->reservation);
	}
    }
}

void zoosnet_report_free(zoosnet_report_head_t *head)
{

    zoosnet_report_head_t *temp = head;
    zoosnet_report_node_t *node = NULL;
    zoosnet_report_head_t *temp_free = NULL;
    zoosnet_report_node_t *node_free = NULL;

    while(temp) {

	temp_free = temp;
	temp = temp->next;

	node = temp_free->head;
	while (node) {

	    node_free = node;
	    node = node->next;
	    free(node_free);
	}

	free(temp_free);
    }
}



