
#ifndef	__BAIDU_REPORT_H__
#define	__BAIDU_REPORT_H__

#include "sem.h"

extern char *strptime(const char *s, const char *format, struct tm *tm);
typedef struct baidu_pair_report_node_t baidu_pair_report_node_t;
struct baidu_pair_report_node_t {

    unsigned int account_ID;//帐号ID
    char account_str[256];//帐号
    unsigned int plan_ID;//推广计划ID
    char plan_str[256];//推广计划
    unsigned int unit_ID;//推广单元ID
    char unit_str[256];//推广单元
    unsigned int keyword_ID;//关键词ID
    char keyword_str[256];//关键词
    char show_url[256];//显示URL
    unsigned int impression;//展现量
    unsigned int click;//点击量
    double cost;//消费

    baidu_pair_report_node_t *next;
};

typedef struct baidu_pair_report_head_t baidu_pair_report_head_t;
struct baidu_pair_report_head_t{

    char date_str[256];
    baidu_pair_report_node_t *head;
    baidu_pair_report_head_t *next;
};

extern baidu_pair_report_node_t *baidu_pair_report_load(char *path);
extern size_t baidu_pair_report_day_save(baidu_pair_report_node_t *head, char *path);
extern baidu_pair_report_node_t *baidu_pair_report_day_load(baidu_pair_report_node_t *head, char *path);
extern void baidu_pair_report_print(baidu_pair_report_node_t *head);
extern void baidu_pair_report_free(baidu_pair_report_node_t *head);

typedef struct baidu_search_report_node_t baidu_search_report_node_t;
struct baidu_search_report_node_t {

    unsigned int keyword_ID;//关键词ID
    char search_str[256];//搜索詞
    char show_url[256];//显示URL
    unsigned int impression;//展现量
    unsigned int click;//点击量

    baidu_search_report_node_t *next;
};

extern baidu_search_report_node_t *baidu_search_report_load(char *path);
extern size_t baidu_search_report_day_save(baidu_search_report_node_t *head, char *path);
extern baidu_search_report_node_t *baidu_search_report_day_load(baidu_search_report_node_t *head, char *path);
extern void baidu_search_report_print(baidu_search_report_node_t *head);
extern void baidu_search_report_free(baidu_search_report_node_t *head);

#endif
