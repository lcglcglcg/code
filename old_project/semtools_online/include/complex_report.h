
#ifndef	__COMPLEX_REPORT_H__
#define	__COMPLEX_REPORT_H__

#include "sem.h"
#include "baidu_report.h"
#include "zoosnet_report.h"

typedef struct complex_report_t complex_report_t;
typedef struct complex_report_node_t complex_report_node_t;

struct complex_report_node_t {

    unsigned int account_ID;//帐号ID
    char account_str[256];//帐号
    unsigned int plan_ID;//推广计划ID
    char plan_str[256];//推广计划
    unsigned int unit_ID;//推广单元ID
    char unit_str[256];//推广单元
    unsigned int keyword_ID;//关键词ID
    char keyword_str[256];//关键词
    unsigned int impression;//展现量
    unsigned int click;//点击量
    double cost;//消费
    char search_str[256];//搜索詞

    double ctr;
    double cpc;

    unsigned int visit_only;//仅打开窗口
    unsigned int no_service_connect;//客服未接待
    unsigned int no_service_message;//客服无讯息
    unsigned int no_guest_message;//访客无讯息
    unsigned int normal_dialog;//一般对话
    unsigned int better_dialog;//较好对话
    unsigned int best_dialog;//极佳对话
    
    unsigned int mobile_visit_only;//仅打开窗口
    unsigned int mobile_no_service_connect;//客服未接待
    unsigned int mobile_no_service_message;//客服无讯息
    unsigned int mobile_no_guest_message;//访客无讯息
    unsigned int mobile_normal_dialog;//一般对话
    unsigned int mobile_better_dialog;//较好对话
    unsigned int mobile_best_dialog;//极佳对话
    
    unsigned int pc_visit_only;//pc仅打开窗口
    unsigned int pc_no_service_connect;//pc客服未接待
    unsigned int pc_no_service_message;//pc客服无讯息
    unsigned int pc_no_guest_message;//pc访客无讯息
    unsigned int pc_normal_dialog;//pc一般对话
    unsigned int pc_better_dialog;//pc较好对话
    unsigned int pc_best_dialog;//pc极佳对话

    double all_dialog_rate;//有效对话转化率
    double all_dialog_cpc;//有效对话成本
    double best_dialog_rete;//极佳度花转化率
    double best_dialog_cpc;//极佳对话成本

    unsigned int reservation;//预约量
    double reservation_rate;//预约转化率
    double reservation_cpc;//预约成本

    complex_report_node_t *next;
};

struct complex_report_t {

    complex_report_node_t *pc_head;
    complex_report_node_t *mobile_head;
    complex_report_node_t *all_head;
};

extern complex_report_t *complex_report_pair_load(
	complex_report_t *complex_report, 
	baidu_pair_report_node_t *pair_report,
	char *pc_str, 
	char *mobile_str);

extern void complex_report_search_load(
	complex_report_t *complex_report,
	baidu_search_report_node_t *search_report,
	char *pc_str,
	char *mobile_str);

extern void complex_report_zoosnet_load(
	complex_report_t *complex_report,
	zoosnet_report_head_t *zoosnet_report,
	char *domain_str,
	int search_mask);

extern void complex_report_zoosnet_day_load(
	complex_report_t *complex_report,
	zoosnet_report_node_t *zoosnet_node_head,
	char *domain_str,
	int search_mask);

extern void complex_report_calculate(complex_report_t *complex_report);
extern void complex_report_to_xls(complex_report_node_t *head, char *path);
extern void complex_report_free(complex_report_t *complex_report);

#endif



