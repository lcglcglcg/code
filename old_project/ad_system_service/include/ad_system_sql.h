
#ifndef	__AD_SYSTEM_SQL_H__
#define	__AD_SYSTEM_SQL_H__

#include "lg_list.h"
#include "ad_system_packet.h"

#ifdef __cplusplus
extern "C" { 
#endif

    /**************/
    /* oracle API */
    /**************/

    typedef struct {

	void *cn;
	void *st;
	void *rs;

    } ad_system_sql_t;

    extern int ad_system_sql_init(lg_list_t *config_file_head);
    extern int ad_system_sql_connect(ad_system_sql_t *p);

    extern int ad_system_sql_fetch_next(ad_system_sql_t *p);
    extern int ad_system_sql_get_number(ad_system_sql_t *p, char *name, int index);
    extern double ad_system_sql_get_double(ad_system_sql_t *p, char *name, int index);
    extern const char *ad_system_sql_get_string(ad_system_sql_t *p, char *name, int index);

    extern int ad_system_sql_execute(ad_system_sql_t *p, char *sql_str);
    extern void ad_system_sql_commit(ad_system_sql_t *p);
    extern void ad_system_sql_rollback(ad_system_sql_t *p);
    extern void ad_system_sql_free(ad_system_sql_t *p);

    /************/
    /* 业务逻辑 */
    /************/

    //登录验证
    extern int ad_system_sql_login(ad_system_sql_t *p, char *account, char *password);

    //更新登录地址
    extern int ad_system_sql_set_login(char *account_id, char *addr);

    //获取标题信息
    extern int ad_system_sql_get_title_info(ad_system_sql_t *p, char *account_id);

    //首页->获取公告
    extern int ad_system_sql_get_announcement(ad_system_sql_t *p, int page_index_s, int page_index_e, const char *title);

    //首页->获取公告->数量
    extern int ad_system_sql_get_announcement_count(ad_system_sql_t *p, const char *title);

    //首页->曲线图->出价
    extern int ad_system_sql_get_budget(ad_system_sql_t *p, char *account_id);

    //推广->获取科室病种CODE
    extern int ad_system_sql_get_disease_code(ad_system_sql_t *p);

    //推广->获取计划
    extern int ad_system_sql_get_campaign(ad_system_sql_t *p,
	    char *account_id, 
	    int display_status,
	    int find_type,
	    int find_name_type,
	    const char *find_name);

    //推广->设置计划
    extern int ad_system_sql_set_campaign(char *account_id,
	    const char *campaign_id,
	    int update_type,
	    int status,
	    const char *dudget,
	    int region_type,
	    const char *region_serialize,
	    int schedule_type,
	    const char *schedule_serialize,
	    int time_type,
	    const char *begin_time,
	    const char *end_time,
	    const char *campign_name);

    //推广->获取组
    int ad_system_sql_get_group(ad_system_sql_t *p,
	    const char *account_id, 
	    int display_status, 
	    int find_type,
	    const char *campaign_id, 
	    int group_name_type, 
	    const char *group_name);

    //推广->设置组
    extern int ad_system_sql_set_group(
	    const char *account_id,
	    const char *group_id,
	    int update_type,
	    int status,
	    const char *cpc_price,
	    const char *cpm_price,
	    int bidding_model,
	    int targeting_type,
	    const char *group_name);

    //推广->设置组附加属性
    extern int ad_system_sql_set_group_targeting(
	    const char *account_id,
	    const char *group_id,
	    int update_type,
	    int targeting_type,
	    const char *disease,
	    const char *keyword);


    //推广->获取文字创意
    extern int ad_system_sql_get_creative_text(ad_system_sql_t *p,
	    const char *account_id, 
	    int find_type,
	    const char *campaign_id, 
	    const char *group_id, 
	    int creative_name_type, 
	    const char *creative_name);

    //推广->获取文字创意已删除
    extern int ad_system_sql_get_creative_del_text(ad_system_sql_t *p,
	    const char *account_id, 
	    int find_type,
	    const char *campaign_id, 
	    const char *group_id, 
	    int find_name_type, 
	    const char *find_name);

    //推广->修改文字创意
    extern int ad_system_sql_set_creative_text(const char *creative_id, int status);
#if 0
    extern int ad_system_sql_set_creative_text(
	    const char *account_id,
	    const char *creative_id,
	    int update_type,
	    int status,
	    const char *creative_name,
	    const char *descriptionpriec,
	    const char *destination_url,
	    const char *creative_display_url,
	    int creative_append_type,
	    const char *creative_append_id,
	    const char *mobile_destination_url,
	    const char *mobile_display_url);
#endif

    //推广->复制文字创意
    extern int ad_system_sql_copy_creative_text(
	    int update_update,
	    const char *creative_id,
	    const char *creative_name,
	    const char *descriptionpriec,
	    const char *destination_url,
	    const char *creative_display_url,
	    int creative_append_type,
	    const char *creative_append_id,
	    const char *mobile_destination_url,
	    const char *mobile_display_url);

    //推广->获取图片创意
    extern int ad_system_sql_get_img_creative(ad_system_sql_t *p,
	    const char *account_id, 
	    int find_type,
	    const char *campaign_id, 
	    const char *group_id, 
	    int creative_name_type, 
	    const char *creative_name);

    //推广->获取图片创意(已删除)
    extern int ad_system_sql_get_creative_del_img(ad_system_sql_t *p,
	    const char *account_id,
	    int find_type,
	    const char *campaign_id, 
	    const char *group_id, 
	    int find_name_type,
	    const char *find_name);

    //推广->修改图片创意
    extern int ad_system_sql_set_creative_img(
	    const char *account_id,
	    const char *creative_id,
	    int creative_status);

    //推广->复制图片创意
    extern int ad_system_sql_copy_creative_img(ad_system_sql_t *p,
	    int update_type,
	    const char *creative_id,
	    const char *creative_name,
	    const char *creative_destination_url,
	    const char *creative_display_url);

    //推广->创建图片创意
    extern int ad_system_sql_add_creative_img(ad_system_sql_t *p,
	    const char *account_id,
	    const char *campaign_id,
	    const char *group_id,
	    const char *creative_name,
	    const char *img_path,
	    int img_width,
	    int img_height,
	    int img_length,
	    const char *creative_destination_url,
	    const char *creative_display_url);

    //推广->附加创意
    extern int ad_system_sql_get_creative_append(ad_system_sql_t *p,
	    const char *account_id, 
	    int find_type,
	    int sort_type,
	    int sort_column,
	    const char *find_name, 
	    int status);

    //推广->修改附加创意
    extern int ad_system_sql_set_creative_append(
	    const char *account_id,
	    const char *creative_append_id,
	    int update_type,
	    int status,
	    const char *contend);

    //获取创意ID 与组ID
    extern int ad_system_sql_get_campaign_group_from_group_or_creative_img(ad_system_sql_t *p, 
	    const char *group_id, 
	    const char *creative_id);

    //推广->添加文字创意
    extern int ad_system_sql_add_creative_text(const char *account_id,
	    const char *campaign_id,
	    const char *group_id,
	    const char *creative_name,
	    const char *descriptionpriec,
	    const char *destination_url,
	    const char *creative_display_url,
	    int creative_append_type,
	    const char *creative_append_id,
	    const char *mobile_destination_url,
	    const char *mobile_display_url);

    //推广->添加附加创意
    extern int ad_system_sql_add_creative_append(const char *account_id, const char *append_contend);

    //推广->验证组重命名
    extern int ad_system_sql_group_name_check(const char *campaign_id, const int display_status, const char *group_name);

    //推广->获取新的组ID
    extern int ad_system_sql_get_group_id(char *group_id);

    //推广->添加组
    extern int ad_system_sql_add_group(
	    const char *account_id,
	    int display_status,
	    const char *campaign_id,
	    const char *group_id,
	    const char *group_name,
	    int group_price_type,
	    const char *group_cpc_price,
	    const char *group_cpm_price,
	    int targeting_type);

    //推广->添加组附属属性
    extern int ad_system_sql_add_group_targeting(
	    const char *account_id,
	    const char *campaign_id,
	    const char *group_id,
	    int targeting_type,
	    const char *disease,
	    const char *keyword);

    //推广->获取新的计划ID
    extern int ad_system_sql_get_campaign_id(char *campaign_id);

    //推广->验证计划重命名
    extern int ad_system_sql_campaign_name_check(const char *account_id, const char *campaign_name, int display_style);

    //推广->添加计划
    extern int ad_system_sql_add_campaign(
	    const char *account_id,
	    int display_status,
	    const char *campaign_id,
	    const char *campaign_name,
	    const char *dudget,
	    int region_type,
	    const char *region_serialize,
	    int schedule_type,
	    const char *schedule_serialize,
	    int time_type,
	    const char *begin_time,
	    const char *end_time);

    //删除文字创意
    extern int ad_system_sql_del_creative_text(const char *creative_id);

    //删除图片创意
    extern int ad_system_sql_del_creative_img(const char *creative_id);

    //删除组
    extern int ad_system_sql_del_group(const char *group_id);

    //删除计划
    extern int ad_system_sql_del_campaign(const char *campaign_id);

    //删除附加创意
    extern int ad_system_sql_del_creative_append(const char *append_id);

    //财务->充值记录
    extern int ad_system_sql_get_money_recharge(ad_system_sql_t *p,
	    const char *account_id,
	    int find_type,
	    int money_type,
	    int find_name_type,
	    const char *find_name,
	    const char *time_start,
	    const char *time_end,
	    int sort_type,
	    int sort_column,
	    int page_index_s,
	    int page_index_e);

    //财务->充值记录数量
    extern int ad_system_sql_get_money_recharge_count(ad_system_sql_t *p,
	    const char *account_id,
	    int find_type,
	    int money_type,
	    int find_name_type,
	    const char *find_name,
	    const char *time_start,
	    const char *time_end);

    //账户信息->修改信息
    extern int ad_system_sql_set_user_info(ad_system_sql_t *p, char *account_id,
	    const char *company_name,
	    const char *company_site,
	    const char *industry,
	    const char *province_id,
	    const char *city_id,
	    const char *address,
	    const char *postcode,
	    const char *contact,
	    const char *telephone,
	    const char *fax,
	    const char *mobile,
	    const char *email,
	    const char *webim);

    //推广->获取预算与出价
    extern int ad_system_sql_get_budget_and_bidding(ad_system_sql_t *p, char *account_id, const char *group_id);

    //推广->计划(单个)
    extern int ad_system_sql_get_campaign_unique(ad_system_sql_t *p, const char *campaign_id);

    //推广->附加创意获取(单个)
    extern int ad_system_sql_get_creative_append_unique(ad_system_sql_t *p, const char *creative_append_id);

    //推广->获取组(单个)
    extern int ad_system_sql_get_group_unique(ad_system_sql_t *p,	const char *group_id);


    //账户信息->基本信息
    extern int ad_system_sql_get_user_info(ad_system_sql_t *p, char *account_id);

    //账户信息->获取密码
    extern int ad_system_sql_get_user_pass(ad_system_sql_t *p, char *account_id);

    //账户信息修改改改密码
    extern int ad_system_sql_set_user_pass(char *account_id, const char *pass_new);

    extern int ad_system_sql_get_group_count(ad_system_sql_t *p,
	    const char *account_id, 
	    int display_status, 
	    int find_type,
	    const char *campaign_id, 
	    int group_name_type, 
	    const char *group_name,
	    char *count);

    extern int ad_system_sql_get_group_set_keywoed(const char *group_id, const char *keyword);

    //推广->组->关键词
    extern int ad_system_sql_add_group_keyword(const char *account_id, const char *group_id, const char  *keyword);
    extern int ad_system_sql_get_group_keyword(ad_system_sql_t *p, const char *account_id, 
	    int page_index_s,
	    int page_index_e,
	    int display_status,
	    const char *campaign_id,
	    const char *group_id,
	    int find_type, 
	    int find_name_type,
	    const char *find_name);
    extern int ad_system_sql_get_group_keyword_count(ad_system_sql_t *p, const char *account_id, 
	    int display_status,
	    const char *campaign_id,
	    const char *group_id,
	    int find_type, 
	    int find_name_type,
	    const char *find_name);
    extern int ad_system_sql_del_group_keyword(const char  *keyword_id);

    extern int ad_system_sql_group_synch_state(const char *account_id, const char *group_id, const char *keyword_id);

#ifdef __cplusplus
}
#endif

#endif


