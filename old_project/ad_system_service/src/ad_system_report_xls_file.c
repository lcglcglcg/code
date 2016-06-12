
#include "libxl.h"
#include "ad_system_report.h"

typedef struct {

    int row;
    int report_type;
    BookHandle book;
    SheetHandle sheet;
    FormatHandle format_cpc;
    FormatHandle format_rate;

} ad_system_report_xls_file_t;

void ad_system_report_xls_file_standard_title(SheetHandle sheet, int report_type)
{

    int col = 0;

    xlSheetWriteStr(sheet, 0, col++, "时间", 0);
    if (report_type > 1)xlSheetWriteStr(sheet, 0, col++, "推广计划", 0);
    if (report_type > 2)xlSheetWriteStr(sheet, 0, col++, "推广组", 0);
    if (report_type > 3)xlSheetWriteStr(sheet, 0, col++, "创意", 0);

    xlSheetWriteStr(sheet, 0, col++, "展现次数", 0);
    xlSheetWriteStr(sheet, 0, col++, "点击次数", 0);
    xlSheetWriteStr(sheet, 0, col++, "总消费", 0);

    xlSheetWriteStr(sheet, 0, col++, "点击率", 0);
    xlSheetWriteStr(sheet, 0, col++, "平均点击价格", 0);
    xlSheetWriteStr(sheet, 0, col++, "千次展现成本", 0);

    //xlSheetSetColA(sheet, 0, report_type - 2, 25, 0, 0);
    //xlSheetSetColA(sheet, report_type -1, report_type - 1, 50, 0, 0);
}

void ad_system_report_xls_file_standard_write(ad_system_report_xls_file_t *info,
	char *date,
	char *campaign,
	char *group,
	char *creative,
	int impression,
	int click,
	double cost,
	double ctr,
	double cpc,
	double cpm)
{

    int col = 0;

    xlSheetWriteStr(info->sheet, info->row, col++, date, 0);
    if (info->report_type > 1) xlSheetWriteStr(info->sheet, info->row, col++, campaign, 0); 
    if (info->report_type > 2) xlSheetWriteStr(info->sheet, info->row, col++, group, 0); 
    if (info->report_type > 3) xlSheetWriteStr(info->sheet, info->row, col++, creative, 0); 

    xlSheetWriteNum(info->sheet, info->row, col++, impression, 0);
    xlSheetWriteNum(info->sheet, info->row, col++, click, 0);
    xlSheetWriteNum(info->sheet, info->row, col++, cost, info->format_cpc);

    xlSheetWriteNum(info->sheet, info->row, col++, ctr, info->format_rate);
    xlSheetWriteNum(info->sheet, info->row, col++, cpc, info->format_cpc);
    xlSheetWriteNum(info->sheet, info->row, col++, cpm, info->format_cpc);

    info->row++;
}

void ad_system_report_xls_file_standard_parse_creative(
	char *date, char *campaign_name, char *group_name, lg_list_t *head, ad_system_report_xls_file_t *info)
{

    lg_list_t *p = NULL;
    ad_system_report_standard_creative_t *node = NULL;
    lg_list_for_each(p, head){

	node = (ad_system_report_standard_creative_t *)p;
	ad_system_report_xls_file_standard_write(info,
		date,
		campaign_name,
		group_name,
		node->creative_name,
		node->impression,
		node->click,
		node->cost,
		node->ctr,
		node->cpc,
		node->cpm);
    }
}

void ad_system_report_xls_file_standard_parse_group(
	char *date, char *campaign_name, lg_list_t *head, ad_system_report_xls_file_t *info)
{

    lg_list_t *p = NULL;
    ad_system_report_standard_group_t *node = NULL;
    lg_list_for_each(p, head){

	node = (ad_system_report_standard_group_t *)p;
	if (info->report_type == 3) {

	    ad_system_report_xls_file_standard_write(info,
		    date,
		    campaign_name,
		    node->group_name,
		    NULL,
		    node->impression,
		    node->click,
		    node->cost,
		    node->ctr,
		    node->cpc,
		    node->cpm);

	} else {

	    ad_system_report_xls_file_standard_parse_creative(date, campaign_name, node->group_name, &node->head, info);
	}
    }
}

void ad_system_report_xls_file_standard_parse_campaign(
	char *date, lg_list_t *head, ad_system_report_xls_file_t *info)
{

    lg_list_t *p = NULL;
    ad_system_report_standard_campaign_t *node = NULL;
    lg_list_for_each(p, head){

	node = (ad_system_report_standard_campaign_t *)p;
	if (info->report_type == 2) {

	    ad_system_report_xls_file_standard_write(info,
		    date,
		    node->campaign_name,
		    NULL,
		    NULL,
		    node->impression,
		    node->click,
		    node->cost,
		    node->ctr,
		    node->cpc,
		    node->cpm);

	} else {

	    ad_system_report_xls_file_standard_parse_group(date, node->campaign_name, &node->head, info);
	}
    }
}

void ad_system_report_xls_file_standard_parse(lg_list_t *head, ad_system_report_xls_file_t *info)
{

    lg_list_t *p = NULL;
    ad_system_report_standard_t *node = NULL;
    lg_list_for_each(p, head){

	node = (ad_system_report_standard_t *)p;
	if (info->report_type == 1) {

	    ad_system_report_xls_file_standard_write(info,
		    node->date,
		    NULL,
		    NULL,
		    NULL,
		    node->impression,
		    node->click,
		    node->cost,
		    node->ctr,
		    node->cpc,
		    node->cpm);
	} else {

	    ad_system_report_xls_file_standard_parse_campaign(node->date, &node->head, info);
	}
    }
}

int ad_system_report_xls_file_standard(lg_list_t *cpc_head, lg_list_t *cpm_head, int report_type, const char *report_path)
{

    ad_system_report_xls_file_t info;
    memset(&info, 0, sizeof(ad_system_report_xls_file_t));
    info.report_type = report_type;

    if (!(info.book = xlCreateBook()))
	return -1;

    info.format_cpc = xlBookAddFormat(info.book, NULL);
    xlFormatSetNumFormat(info.format_cpc, NUMFORMAT_NUMBER_D2);

    info.format_rate = xlBookAddFormat(info.book, NULL);
    xlFormatSetNumFormat(info.format_rate, NUMFORMAT_PERCENT_D2);

    xlBookSetKeyA(info.book, "Tao Zhang", "linux-ebd9197b99a8a51e0b01243c4eh2dcm6");

    info.row = 1;
    if (!(info.sheet = xlBookAddSheet(info.book, "点击消费", 0))) {

	xlBookRelease(info.book);
	return -1;
    }

    ad_system_report_xls_file_standard_title(info.sheet, info.report_type);
    ad_system_report_xls_file_standard_parse(cpc_head, &info);

    if (cpm_head) {

	info.row = 1;
	if (!(info.sheet = xlBookAddSheet(info.book, "展现消费", 0))) {

	    xlBookRelease(info.book);
	    return -1;
	}

	ad_system_report_xls_file_standard_title(info.sheet, info.report_type);
	ad_system_report_xls_file_standard_parse(cpm_head, &info);
    }

    xlBookSave(info.book, report_path);
    xlBookRelease(info.book);
    return 0;
}

void ad_system_report_xls_file_complex_title(SheetHandle sheet, int report_type)
{

    int col = 0;

    xlSheetWriteStr(sheet, 0, col++, "时间", 0);
    xlSheetWriteStr(sheet, 0, col++, "省份", 0);
    if (report_type > 1)xlSheetWriteStr(sheet, 0, col++, "城市", 0);

    xlSheetWriteStr(sheet, 0, col++, "展现次数", 0);
    xlSheetWriteStr(sheet, 0, col++, "点击次数", 0);
    xlSheetWriteStr(sheet, 0, col++, "总消费", 0);

    xlSheetWriteStr(sheet, 0, col++, "点击率", 0);
    xlSheetWriteStr(sheet, 0, col++, "平均点击价格", 0);
    xlSheetWriteStr(sheet, 0, col++, "千次展现成本", 0);

    xlSheetSetColA(sheet, 0, 100, 15, 0, 0);
}

void ad_system_report_xls_file_complex_write(ad_system_report_xls_file_t *info,
	char *date,
	char *region_father,
	char *region_child,
	int impression,
	int click,
	double cost,
	double ctr,
	double cpc,
	double cpm)
{

    int col = 0;

    xlSheetWriteStr(info->sheet, info->row, col++, date, 0); 
    xlSheetWriteStr(info->sheet, info->row, col++, region_father, 0); 
    if (info->report_type > 1) xlSheetWriteStr(info->sheet, info->row, col++, region_child, 0); 

    xlSheetWriteNum(info->sheet, info->row, col++, impression, 0);
    xlSheetWriteNum(info->sheet, info->row, col++, click, 0);
    xlSheetWriteNum(info->sheet, info->row, col++, cost, info->format_cpc);

    xlSheetWriteNum(info->sheet, info->row, col++, ctr, info->format_rate);
    xlSheetWriteNum(info->sheet, info->row, col++, cpc, info->format_cpc);
    xlSheetWriteNum(info->sheet, info->row, col++, cpm, info->format_cpc);

    info->row++;
}

void ad_system_report_xls_file_complex_parse_child(
	char *date, char *region_father, lg_list_t *head, ad_system_report_xls_file_t *info)
{

    lg_list_t *p = NULL;
    ad_system_report_complex_child_t *node = NULL;
    lg_list_for_each(p, head){

	node = (ad_system_report_complex_child_t *)p;
	ad_system_report_xls_file_complex_write(info,
		date, 
		region_father,
		node->region_child,
		node->impression,
		node->click,
		node->cost,
		node->ctr,
		node->cpc,
		node->cpm);
    }
}

void ad_system_report_xls_file_complex_parse_father(
	char *date, lg_list_t *head, ad_system_report_xls_file_t *info)
{

    lg_list_t *p = NULL;
    ad_system_report_complex_father_t *node = NULL;
    lg_list_for_each(p, head){

	node = (ad_system_report_complex_father_t *)p;
	if (info->report_type == 1) {

	    ad_system_report_xls_file_complex_write(info,
		    date,
		    node->region_father,
		    NULL,
		    node->impression,
		    node->click,
		    node->cost,
		    node->ctr,
		    node->cpc,
		    node->cpm);

	} else {

	    ad_system_report_xls_file_complex_parse_child(date, node->region_father, &node->head, info);
	}
    }
}

void ad_system_report_xls_file_complex_parse(lg_list_t *head, ad_system_report_xls_file_t *info)
{

    lg_list_t *p = NULL;
    ad_system_report_complex_t *node = NULL;
    lg_list_for_each(p, head){

	node = (ad_system_report_complex_t *)p;
	ad_system_report_xls_file_complex_parse_father(node->date, &node->head, info);
    }
}

int ad_system_report_xls_file_complex(lg_list_t *cpc_head, lg_list_t *cpm_head, int report_type, const char *report_path)
{

    ad_system_report_xls_file_t info;
    memset(&info, 0, sizeof(ad_system_report_xls_file_t));

    info.row = 1;
    info.report_type = report_type;

    if (!(info.book = xlCreateBook()))
	return -1;

    info.format_cpc = xlBookAddFormat(info.book, NULL);
    xlFormatSetNumFormat(info.format_cpc, NUMFORMAT_NUMBER_D2);

    info.format_rate = xlBookAddFormat(info.book, NULL);
    xlFormatSetNumFormat(info.format_rate, NUMFORMAT_PERCENT_D2);

    xlBookSetKeyA(info.book, "Tao Zhang", "linux-ebd9197b99a8a51e0b01243c4eh2dcm6");

    if (!(info.sheet = xlBookAddSheet(info.book, "点击消费", 0))) {

	xlBookRelease(info.book);
	return -1;
    }

    ad_system_report_xls_file_complex_title(info.sheet, info.report_type);
    ad_system_report_xls_file_complex_parse(cpc_head, &info);

    if (cpm_head) {

	info.row = 1;
	if (!(info.sheet = xlBookAddSheet(info.book, "展现消费", 0))) {

	    xlBookRelease(info.book);
	    return -1;
	}

	ad_system_report_xls_file_complex_title(info.sheet, info.report_type);
	ad_system_report_xls_file_complex_parse(cpm_head, &info);
    }

    xlBookSave(info.book, report_path);
    xlBookRelease(info.book);
    return 0;
}

