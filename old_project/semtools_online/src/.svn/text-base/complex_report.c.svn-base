
#include "complex_report.h"

complex_report_node_t *complex_report_pair_analysis(
	complex_report_node_t *head,
	baidu_pair_report_node_t *pair_node)
{

    complex_report_node_t *complex_node = NULL;
    for (complex_node = head;
	    complex_node;
	    complex_node = complex_node->next) {

	if (complex_node->keyword_ID == pair_node->keyword_ID) {

	    complex_node->account_ID = pair_node->account_ID;
	    memcpy(complex_node->account_str, pair_node->account_str, 256);
	    complex_node->plan_ID = pair_node->plan_ID;
	    memcpy(complex_node->plan_str, pair_node->plan_str, 256);
	    complex_node->unit_ID = pair_node->unit_ID;
	    memcpy(complex_node->unit_str, pair_node->unit_str, 256);
	    memcpy(complex_node->keyword_str, pair_node->keyword_str, 256);
	    complex_node->impression += pair_node->impression;
	    complex_node->click += pair_node->click;
	    complex_node->cost += pair_node->cost;
	    return head;
	}
    }

    if (!(complex_node = malloc(sizeof(complex_report_node_t))))
	return head;

    memset(complex_node, 0, sizeof(complex_report_node_t));
    complex_node->account_ID = pair_node->account_ID;
    memcpy(complex_node->account_str, pair_node->account_str, 256);
    complex_node->plan_ID = pair_node->plan_ID;
    memcpy(complex_node->plan_str, pair_node->plan_str, 256);
    complex_node->unit_ID = pair_node->unit_ID;
    memcpy(complex_node->unit_str, pair_node->unit_str, 256);
    complex_node->keyword_ID = pair_node->keyword_ID;
    memcpy(complex_node->keyword_str, pair_node->keyword_str, 256);
    complex_node->impression = pair_node->impression;
    complex_node->click = pair_node->click;
    complex_node->cost = pair_node->cost;
    complex_node->next = head;
    return complex_node;
}

complex_report_t *complex_report_pair_load(
	complex_report_t *complex_report, 
	baidu_pair_report_node_t *pair_report,
	char *pc_str, 
	char *mobile_str)
{

    if (complex_report == NULL) {

	if ((complex_report = malloc(sizeof(complex_report_t))))
	    memset(complex_report, 0, sizeof(complex_report_t));
	else
	    return NULL;
    }

    baidu_pair_report_node_t *pair_node = NULL;
    for (pair_node = pair_report;
	    pair_node;
	    pair_node = pair_node->next) {

#if 0
	if (pc_str && strstr(pair_node->show_url, pc_str))
	    complex_report->pc_head = complex_report_pair_analysis(complex_report->pc_head, pair_node);

	if (mobile_str && strstr(pair_node->show_url, mobile_str))
	    complex_report->mobile_head = complex_report_pair_analysis(complex_report->mobile_head, pair_node);
#endif

	complex_report->all_head = complex_report_pair_analysis(complex_report->all_head, pair_node);
    }

    return complex_report;
}

void _complex_report_search_analysis(
	complex_report_node_t *head,
	complex_report_node_t *complex_node,
	baidu_search_report_node_t *search_node)
{

    complex_report_node_t *temp = NULL;
    for (temp = head; temp; temp = temp->next) {
	
	if (temp->search_str[0]) {

	    if (temp->keyword_ID == search_node->keyword_ID) {

		if (strcmp(temp->search_str, search_node->search_str) == 0) {

		    temp->impression += search_node->impression;
		    temp->click += search_node->click;
		    return;
		}
	    }
	}
    }

    complex_report_node_t *complex_node_new = NULL;
    if (!(complex_node_new = malloc(sizeof(complex_report_node_t))))
	return;

    memcpy(complex_node_new, complex_node, sizeof(complex_report_node_t));
    memcpy(complex_node_new->search_str, search_node->search_str, 256);
    complex_node_new->impression = search_node->impression;
    complex_node_new->click = search_node->click;

    complex_node_new->next = complex_node->next;
    complex_node->next = complex_node_new;
    return;
}

void complex_report_search_analysis(
	complex_report_node_t *head,
	baidu_search_report_node_t *search_node)
{

    complex_report_node_t *complex_node = NULL;
    for (complex_node = head;
	    complex_node;
	    complex_node = complex_node->next) {

	if (!complex_node->search_str[0]) {

	    if (complex_node->keyword_ID == search_node->keyword_ID) {

		_complex_report_search_analysis(head, complex_node, search_node);
	    }
	}
    }
}

void complex_report_search_load(
	complex_report_t *complex_report,
	baidu_search_report_node_t *search_report,
	char *pc_str,
	char *mobile_str)
{

    if (complex_report == NULL)
	return;

    baidu_search_report_node_t *search_node = NULL;
    for (search_node = search_report;
	    search_node;
	    search_node = search_node->next) {

#if 0
	if (pc_str && strstr(search_node->show_url, pc_str))
	    complex_report_search_analysis(complex_report->pc_head, search_node);

	if (mobile_str && strstr(search_node->show_url, mobile_str))
	    complex_report_search_analysis(complex_report->mobile_head, search_node);
#endif

	complex_report_search_analysis(complex_report->all_head, search_node);
    }
}

void complex_report_zoosnet_dialog(
	complex_report_node_t *complex_node,
	int dialog_type,
	int mobile_mark)
{

    switch(dialog_type){

	case 1:
	    if (mobile_mark)complex_node->mobile_visit_only++;
	    else complex_node->pc_visit_only++;
	    complex_node->visit_only++;
	    break;

	case 2:
	    if(mobile_mark)complex_node->mobile_no_service_connect++;
	    else complex_node->pc_no_service_connect++;
	    complex_node->no_service_connect++;
	    break;

	case 3:
	    if(mobile_mark)complex_node->mobile_no_service_message++;
	    else complex_node->pc_no_service_message++;
	    complex_node->no_service_message++;
	    break;

	case 4:
	    if(mobile_mark)complex_node->mobile_no_guest_message++;
	    else complex_node->pc_no_guest_message++;
	    complex_node->no_guest_message++;
	    break;

	case 5:
	    if(mobile_mark)complex_node->mobile_normal_dialog++;
	    else complex_node->pc_normal_dialog++;
	    complex_node->normal_dialog++;
	    break;

	case 6:
	    if(mobile_mark)complex_node->mobile_better_dialog++;
	    else complex_node->pc_better_dialog++;
	    complex_node->better_dialog++;
	    break;

	case 7:
	    if(mobile_mark)complex_node->mobile_best_dialog++;
	    else complex_node->pc_best_dialog++;
	    complex_node->best_dialog++;
	    break;
    }
}

void complex_report_zoosnet_search_insert(
	complex_report_node_t *head,
	complex_report_node_t *complex_node,
	zoosnet_report_node_t *zoosnet_node)
{

    if (!zoosnet_node->search_str[0])
	return;

    complex_report_node_t *node = NULL;
    for (node = head; node; node = node->next) {

	if (node->search_str[0])
	    if (strcmp(node->search_str, zoosnet_node->search_str) == 0)
		return;
    }

    if (!(node = malloc(sizeof(complex_report_node_t))))
	return;

    memset(node, 0, sizeof(complex_report_node_t));
    node->keyword_ID = complex_node->keyword_ID;
    //strcpy(node->account_str, "zoosnet");
    //strcpy(node->plan_str, "zoosnet");
    //strcpy(node->unit_str, "zoosnet");
    strcpy(node->account_str, complex_node->account_str);
    strcpy(node->plan_str, complex_node->plan_str);
    strcpy(node->unit_str, complex_node->unit_str);
    
    strcpy(node->keyword_str, complex_node->keyword_str);
    strcpy(node->search_str, zoosnet_node->search_str);

    node->next = complex_node->next;
    complex_node->next = node;
}

void complex_report_zoosnet_analysis(
	complex_report_node_t *head,
	zoosnet_report_node_t *zoosnet_node,
	char *domain_str,
	int search_mask)
{

    complex_report_node_t *complex_node = NULL;
    for (complex_node = head;
	    complex_node;
	    complex_node = complex_node->next) {

	if (complex_node->keyword_ID == zoosnet_node->keyword_ID) {

	    if (strstr(zoosnet_node->baidu_url_str, domain_str)) {

		if (search_mask && !complex_node->search_str[0])
		    complex_report_zoosnet_search_insert(head, complex_node, zoosnet_node);

		if (complex_node->search_str[0]) {

		    if (strcmp(complex_node->search_str, zoosnet_node->search_str) == 0) {

			complex_report_zoosnet_dialog(complex_node, zoosnet_node->dialog_type, zoosnet_node->mobile_mark);
			if (zoosnet_node->reservation)
			    complex_node->reservation++;
		    }

		} else {

		    complex_report_zoosnet_dialog(complex_node, zoosnet_node->dialog_type, zoosnet_node->mobile_mark);
		    if (zoosnet_node->reservation)
			complex_node->reservation++;
		}
	    }
	}
    }
}

void complex_report_zoosnet_load(
	complex_report_t *complex_report,
	zoosnet_report_head_t *zoosnet_report,
	char *domain_str,
	int search_mask)
{

    zoosnet_report_node_t *zoosnet_node = NULL;
    zoosnet_report_head_t *zoosnet_date_node = NULL;

    for (zoosnet_date_node = zoosnet_report;
	    zoosnet_date_node;
	    zoosnet_date_node = zoosnet_date_node->next) {

	for (zoosnet_node = zoosnet_date_node->head;
		zoosnet_node;
		zoosnet_node = zoosnet_node->next) {

#if 0
	    if (zoosnet_node->mobile_mark)
		complex_report_zoosnet_analysis(complex_report->mobile_head, zoosnet_node, domain_str, search_mask);
	    else
		complex_report_zoosnet_analysis(complex_report->pc_head, zoosnet_node, domain_str, search_mask);
#endif

	    complex_report_zoosnet_analysis(complex_report->all_head, zoosnet_node, domain_str, search_mask);
	}
    }
}

void complex_report_zoosnet_day_load(
	complex_report_t *complex_report,
	zoosnet_report_node_t *zoosnet_node_head,
	char *domain_str,
	int search_mask)
{

    zoosnet_report_node_t *zoosnet_node = NULL;
    for (zoosnet_node = zoosnet_node_head;
	    zoosnet_node;
	    zoosnet_node = zoosnet_node->next) {

#if 0
	if (zoosnet_node->mobile_mark)
	    complex_report_zoosnet_analysis(complex_report->mobile_head, zoosnet_node, domain_str, search_mask);
	else
	    complex_report_zoosnet_analysis(complex_report->pc_head, zoosnet_node, domain_str, search_mask);
#endif

	complex_report_zoosnet_analysis(complex_report->all_head, zoosnet_node, domain_str, search_mask);
    }
}

void _complex_report_calculate(complex_report_node_t *node)
{

    double dialog_max = (double)(node->normal_dialog + node->better_dialog + node->best_dialog);

    if (!node->search_str[0]) {

	if (node->click && node->impression)
	    node->ctr = (double)node->click / (double)node->impression;

	if (node->cost && node->click)
	    node->cpc = (double)node->cost / (double)node->click;

	if (dialog_max && node->click)
	    node->all_dialog_rate = (double)dialog_max / (double)node->click;

	if (node->cost && dialog_max)
	    node->all_dialog_cpc = (double)node->cost / (double)dialog_max;

	if (node->best_dialog && node->click)
	    node->best_dialog_rete = (double)node->best_dialog / (double)node->click;

	if (node->cost && node->best_dialog)
	    node->best_dialog_cpc = (double)node->cost / (double)node->best_dialog;

	if (node->reservation && node->click)
	    node->reservation_rate = (double)node->reservation / (double)node->click;

	if (node->cost && node->reservation)
	    node->reservation_cpc = node->cost / node->reservation;

    } else {

	if (node->click && node->impression)
	    node->ctr = (double)node->click / (double)node->impression;

	if (dialog_max && node->click)
	    node->all_dialog_rate = (double)dialog_max / (double)node->click;

	if (node->best_dialog && node->click)
	    node->best_dialog_rete = (double)node->best_dialog / (double)node->click;

	if (node->reservation && node->click)
	    node->reservation_rate = (double)node->reservation / (double)node->click;
    }
}

void complex_report_calculate(complex_report_t *complex_report)
{

    complex_report_node_t *complex_node = NULL;
    for (complex_node = complex_report->pc_head; 
	    complex_node;
	    complex_node = complex_node->next) {

	_complex_report_calculate(complex_node);

    }

    for (complex_node = complex_report->mobile_head; 
	    complex_node; 
	    complex_node = complex_node->next) {

	_complex_report_calculate(complex_node);
    }

    for (complex_node = complex_report->all_head;
	    complex_node; 
	    complex_node = complex_node->next) {

	_complex_report_calculate(complex_node);
    }
}

void _complex_report_print(complex_report_node_t *head)
{

    complex_report_node_t *complex_node = NULL;
    for (complex_node = head;
	    complex_node;
	    complex_node = complex_node->next) {

	fprintf(stdout, "%u,%s,%s,%u,%u,%u,%u,%u,%u,%u\n",
		complex_node->keyword_ID,
		complex_node->keyword_str,
		complex_node->search_str,
		complex_node->visit_only,
		complex_node->no_service_connect,
		complex_node->no_service_message,
		complex_node->no_guest_message,
		complex_node->normal_dialog,
		complex_node->better_dialog,
		complex_node->best_dialog);
    }
}

void complex_report_print(complex_report_t *complex_report)
{

    fprintf(stdout, "pc\n");
    _complex_report_print(complex_report->pc_head);
    fprintf(stdout, "mobile\n");
    _complex_report_print(complex_report->mobile_head);
    fprintf(stdout, "all\n");
    _complex_report_print(complex_report->all_head);
}

void complex_report_to_xls_tile(SheetHandle sheet)
{

    xlSheetWriteStr(sheet, 0, 0, "帐号", 0);
    xlSheetWriteStr(sheet, 0, 1, "推广计划", 0);
    xlSheetWriteStr(sheet, 0, 2, "推广单元", 0);
    xlSheetWriteStr(sheet, 0, 3, "关键词", 0);
    xlSheetWriteStr(sheet, 0, 4, "点击搜索词", 0);
    xlSheetWriteStr(sheet, 0, 5, "展现量", 0);
    xlSheetWriteStr(sheet, 0, 6, "点击量", 0);
    xlSheetWriteStr(sheet, 0, 7, "消费", 0);
    xlSheetWriteStr(sheet, 0, 8, "点击率", 0);
    xlSheetWriteStr(sheet, 0, 9, "平均点击价格", 0);

    xlSheetWriteStr(sheet, 0, 10, "仅打开窗口", 0);
    xlSheetWriteStr(sheet, 0, 11, "客服未接待", 0);
    xlSheetWriteStr(sheet, 0, 12, "客服无讯息", 0);
    xlSheetWriteStr(sheet, 0, 13, "访客无讯息", 0);
    xlSheetWriteStr(sheet, 0, 14, "一般对话", 0);
    xlSheetWriteStr(sheet, 0, 15, "较好对话", 0);
    xlSheetWriteStr(sheet, 0, 16, "极佳对话", 0);

    xlSheetWriteStr(sheet, 0, 17, "pc_仅打开窗口", 0);
    xlSheetWriteStr(sheet, 0, 18, "pc_客服未接待", 0);
    xlSheetWriteStr(sheet, 0, 19, "pc_客服无讯息", 0);
    xlSheetWriteStr(sheet, 0, 20, "pc_访客无讯息", 0);
    xlSheetWriteStr(sheet, 0, 21, "pc_一般对话", 0);
    xlSheetWriteStr(sheet, 0, 22, "pc_较好对话", 0);
    xlSheetWriteStr(sheet, 0, 23, "pc_极佳对话", 0);

    xlSheetWriteStr(sheet, 0, 24, "mp_仅打开窗口", 0);
    xlSheetWriteStr(sheet, 0, 25, "mp_客服未接待", 0);
    xlSheetWriteStr(sheet, 0, 26, "mp_客服无讯息", 0);
    xlSheetWriteStr(sheet, 0, 27, "mp_访客无讯息", 0);
    xlSheetWriteStr(sheet, 0, 28, "mp_一般对话", 0);
    xlSheetWriteStr(sheet, 0, 29, "mp_较好对话", 0);
    xlSheetWriteStr(sheet, 0, 30, "mp_极佳对话", 0);

    xlSheetWriteStr(sheet, 0, 31, "有效对话转化率", 0);
    xlSheetWriteStr(sheet, 0, 32, "有效对话成本", 0);

    xlSheetWriteStr(sheet, 0, 33, "极佳对话转换率", 0);
    xlSheetWriteStr(sheet, 0, 34, "极佳对话成本", 0);

    xlSheetWriteStr(sheet, 0, 35, "预约量", 0);
    xlSheetWriteStr(sheet, 0, 36, "预约率", 0);
    xlSheetWriteStr(sheet, 0, 37, "预约成本", 0);
}

void complex_report_to_xls(complex_report_node_t *head, char *path)
{

    int sheet_count = 0;
    char sheet_str[128] = {0};
    BookHandle book = NULL;
    SheetHandle sheet = NULL;

    if (!head || !path)return;
    if (!(book = xlCreateBook()))
	return;

    xlBookSetKeyA(book, "Tao Zhang", 
	    "linux-ebd9197b99a8a51e0b01243c4eh2dcm6");

    FormatHandle format_cpc = xlBookAddFormat(book, NULL);
    xlFormatSetNumFormat(format_cpc, NUMFORMAT_NUMBER_D2);

    FormatHandle format_rate = xlBookAddFormat(book, NULL);
    xlFormatSetNumFormat(format_rate, NUMFORMAT_PERCENT_D2);

    int row_count = 60000;
    complex_report_node_t *node = NULL;
    for (node = head; node; node = node->next, row_count++) {

	if (row_count >= 60000) {

	    sprintf(sheet_str, "Sheet%d", ++sheet_count);
	    if (!(sheet = xlBookAddSheet(book, sheet_str, 0))) {

		xlBookRelease(book);
		return;
	    }

	    complex_report_to_xls_tile(sheet);
	    row_count = 1;
	}

	xlSheetWriteStr(sheet, row_count, 0, node->account_str, 0);
	xlSheetWriteStr(sheet, row_count, 1, node->plan_str, 0);
	xlSheetWriteNum(sheet, row_count, 2, node->keyword_ID, 0);
	//xlSheetWriteStr(sheet, row_count, 2, node->unit_str, 0);
	xlSheetWriteStr(sheet, row_count, 3, node->keyword_str, 0);
	xlSheetWriteStr(sheet, row_count, 4, node->search_str, 0);
	xlSheetWriteNum(sheet, row_count, 5, node->impression, 0);
	xlSheetWriteNum(sheet, row_count, 6, node->click, 0);
	if(!node->search_str[0])
	    xlSheetWriteNum(sheet, row_count, 7, node->cost, format_cpc);
	xlSheetWriteNum(sheet, row_count, 8, node->ctr, format_rate);
	if(!node->search_str[0])
	    xlSheetWriteNum(sheet, row_count, 9, node->cpc, format_cpc);

	xlSheetWriteNum(sheet, row_count, 10, node->visit_only, 0);
	xlSheetWriteNum(sheet, row_count, 11, node->no_service_connect, 0);
	xlSheetWriteNum(sheet, row_count, 12, node->no_service_message, 0);
	xlSheetWriteNum(sheet, row_count, 13, node->no_guest_message, 0);
	xlSheetWriteNum(sheet, row_count, 14, node->normal_dialog, 0);
	xlSheetWriteNum(sheet, row_count, 15, node->better_dialog, 0);
	xlSheetWriteNum(sheet, row_count, 16, node->best_dialog, 0);

	xlSheetWriteNum(sheet, row_count, 17, node->pc_visit_only, 0);
	xlSheetWriteNum(sheet, row_count, 18, node->pc_no_service_connect, 0);
	xlSheetWriteNum(sheet, row_count, 19, node->pc_no_service_message, 0);
	xlSheetWriteNum(sheet, row_count, 20, node->pc_no_guest_message, 0);
	xlSheetWriteNum(sheet, row_count, 21, node->pc_normal_dialog, 0);
	xlSheetWriteNum(sheet, row_count, 22, node->pc_better_dialog, 0);
	xlSheetWriteNum(sheet, row_count, 23, node->pc_best_dialog, 0);

	xlSheetWriteNum(sheet, row_count, 24, node->mobile_visit_only, 0);
	xlSheetWriteNum(sheet, row_count, 25, node->mobile_no_service_connect, 0);
	xlSheetWriteNum(sheet, row_count, 26, node->mobile_no_service_message, 0);
	xlSheetWriteNum(sheet, row_count, 27, node->mobile_no_guest_message, 0);
	xlSheetWriteNum(sheet, row_count, 28, node->mobile_normal_dialog, 0);
	xlSheetWriteNum(sheet, row_count, 29, node->mobile_better_dialog, 0);
	xlSheetWriteNum(sheet, row_count, 30, node->mobile_best_dialog, 0);

	xlSheetWriteNum(sheet, row_count, 31, node->all_dialog_rate, format_rate);
	if(!node->search_str[0])
	    xlSheetWriteNum(sheet, row_count, 32, node->all_dialog_cpc, format_cpc);

	xlSheetWriteNum(sheet, row_count, 33, node->best_dialog_rete, format_rate);
	if(!node->search_str[0])
	    xlSheetWriteNum(sheet, row_count, 34, node->best_dialog_cpc, format_cpc);

	xlSheetWriteNum(sheet, row_count, 35, node->reservation, 0);
	xlSheetWriteNum(sheet, row_count, 36, node->reservation_rate, format_rate);
	if(!node->search_str[0])
	    xlSheetWriteNum(sheet, row_count, 37, node->reservation_cpc, format_cpc);
    }

    xlBookSave(book, path);
    xlBookRelease(book);
}

void complex_report_free(complex_report_t *complex_report)
{

    complex_report_node_t *node = NULL;
    complex_report_node_t *node_free = NULL;
    if (!complex_report)
	return;

    node = complex_report->pc_head;
    while(node){

	node_free = node;
	node = node->next;
	free(node_free);
    }

    node = complex_report->mobile_head;
    while(node){

	node_free = node;
	node = node->next;
	free(node_free);
    }

    node = complex_report->all_head;
    while(node){

	node_free = node;
	node = node->next;
	free(node_free);
    }

    free(complex_report);
}



