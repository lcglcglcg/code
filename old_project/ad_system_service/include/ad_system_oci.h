
#ifndef	__AD_SYSTEM_OCI_H__
#define	__AD_SYSTEM_OCI_H__

#ifdef __cplusplus
extern "C" { 
#endif

    typedef struct {

	char USER_LOGIN_NAME[128]; 	//用户名
	char COMPANY_NAME[128]; 	//公司名称
	double BALANCE; 		//账面余额
	double INVEST;	 		//投资总额
	double CONSUMPTION; 		//消费总额
	char CREATE_TIME[256];		//创建时间
	char DOMAIN[4096];		//公司域名
	char GUAHAO_TOKEN[4096];	//挂号网API
	char WEBIM[2048];		//WEBIM

    }ad_system_oci_title_info_t;

    typedef struct {

	char LOGIN_NAME[128];
	char COMPANY_NAME[256];
	char COMPANY_SITE[2048];
	char INDUSTRY[64];
	char API_TOKEN[128];
	char PROVINCE_ID[128];
	char CITY_ID[128];
	char ADDRESS[512];
	char POSTCODE[32];
	char CONTACT[64];
	char TELEPHONE[64];
	char FAX[64];
	char MOBILE[64];
	char EMAIL[128];
	char WEBIM[2048];
	char LATE_LOGIN_TIME[32];
	char LATE_LOGIN_IP[32];
	char LATE_LOGIN_ADDRESS[64];

    } ad_system_oci_user_info_t;

    typedef struct {

	char PROTECT_QUESTION1[128];
	char PROTECT_ANSWER1[128];
	char PROTECT_QUESTION2[128];
	char PROTECT_ANSWER2[128];
	char PROTECT_QUESTION3[128];
	char PROTECT_ANSWER3[128];
	char LOGIN_PASSWORD_MD5[128];

    } ad_system_oci_user_pass_protect_t;

    typedef struct {

	lg_list_t list;
	char SWIFT_NUMBER[1024];		//流水号(系统自动生成)
	char MONEY[1024];			//本次操作金额
	char PAYMENT_TYPE[1024];		//付款方式1：支票，2：汇款，3：现金
	char ACCOUNTING_DOCUMENT_NUMBER[128];	//凭证号码
	char ACCOUNTING_DOCUMENT_FILE[1024];	//凭证文件(相对路径)
	char INVEST[1024];			//投资总额
	char CURRENT_MONEY_BALANCE[1024];	//当前款项余额
	char CREATE_TIME[1024];		//最后修改时间

    } ad_system_oci_money_recharge_t;

    typedef struct {

	lg_list_t list;
	char DATE[32];
	char TITLE[512];
	char CONTENT[4096];

    } ad_system_oci_announcement_t; //公告

    typedef struct {

	lg_list_t list;
	lg_list_t head;
	char CAMPAIGN_ID[64];
	char CAMPAIGN_NAME[1024];
	int CAMPAIGN_STATUS;

    } ad_system_oci_sperad_list_campaign_t; //计划列表

    typedef struct {

	lg_list_t list;
	char GROUP_ID[64];
	char GROUP_NAME[1024];
	char GROUP_STATUS[64];

    } ad_system_oci_sperad_list_group_t; //组列表

    typedef struct {

	lg_list_t list;
	int page_index;
	int find_status;

	char CAMPAIGN_ID[64];
	char CAMPAIGN_NAME[1024];
	int CAMPAIGN_STATE;
	double CAMPAIGN_BUDGET;

	int SCHEDULE_TYPE;
	char SCHEDULE_SERIALIZE[256];

	int REGION_TYPE;
	char REGION_SERIALIZE[4096];

	int TIME_TYPE;
	char BEGIN_TIME[32];
	char END_TIME [32];

	int CAMPAIGN_IMPRESSION;//展现次数
	int CAMPAIGN_CLICK;//点击次数
	double CAMPAIGN_COST;//总消费  
	double CAMPAIGN_CTR;//点击率
	double CAMPAIGN_CPC;//平均点击价格
	double CAMPAIGN_CPM;//千次展现成本

    } ad_system_oci_campaign_t; //获取计划

    typedef struct {

	lg_list_t list;
	int page_index;
	int find_status;

	char CAMPAIGN_ID[64];
	char CAMPAIGN_NAME[1024];

	char GROUP_ID[64];
	char GROUP_NAME[1024];

	int GROUP_STATE;

	int BIDDING_MODEL;//出价状态
	double GROUP_CPC_PRICE;//CPC出价
	double GROUP_CPM_PRICE;//CPM出价

	int GROUP_TARGETING_TYPE; //1浏览定向 2搜索定向
	char GROUP_DISEASE[4096];//TARGETING_TYPE = 1 生效

	int GROUP_IMPRESSION;//展现次数
	int GROUP_CLICK;//点击次数
	double GROUP_COST;//总消费  
	double GROUP_CTR;//点击率
	double GROUP_CPC;//平均点击价格
	double GROUP_CPM;//千次展现成本

    } ad_system_oci_group_t; //获取组

    typedef struct {

	lg_list_t list;
	int page_index;
	int find_status;

	char CAMPAIGN_ID[64];//计划ID
	char CAMPAIGN_NAME[1024];//计划名字

	char GROUP_ID[64];//组ID
	char GROUP_NAME[1024];//组名字

	char CREATIVE_ID[128];//创意ID
	char CREATIVE_NAME[1024];//创意名称

	int CREATIVE_STATE;//1有效 2不宜推广 3暂停 4审核中 5审核拒绝
	char CREATIVE_REFUSE_REASON[1024];//拒绝理由

	char CREATIVE_DESCRIPTION[1024];//描述1
	char CREATIVE_DESTINATION_URL[1024];//访问URL
	char CREATIVE_DISPLAY_URL[1024];//显示URL
	int CREATIVE_APPEND_TYPE;
	char CREATIVE_APPEND_ID[128];//附加创意

	char MOBILE_DESTINATION_URL[1024];//移动访问URL
	char MOBILE_DISPLAY_URL[1024];//移动显示URL

	int CREATIVE_IMPRESSION;//展现次数
	int CREATIVE_CLICK;//点击次数
	double CREATIVE_COST;//总消费  
	double CREATIVE_CTR;//点击率
	double CREATIVE_CPC;//平均点击价格
	double CREATIVE_CPM;//千次展现成本

    } ad_system_oci_creative_t; //获取文字创意

    typedef struct {

	lg_list_t list;
	char APPEND_ID[60];
	int STATUS;
	char CONTENT[4096];
	char REFUSE_REASON[1024];
	int find_status;

    } ad_system_oci_creative_append_t; //获取文字创意->附加创意

    typedef struct {

	lg_list_t list;
	int page_index;
	int find_status;

	char CAMPAIGN_ID[64];//计划ID
	char CAMPAIGN_NAME[1024];//计划名字

	char GROUP_ID[64];//组ID
	char GROUP_NAME[1024];//组名字

	char CREATIVE_ID[128];//创意ID
	char CREATIVE_NAME[1024];//创意名称

	int CREATIVE_STATE;//1有效 2不宜推广 3暂停 4审核中 5审核拒绝
	char CREATIVE_REFUSE_REASON[1024];//拒绝理由

	char CREATIVE_IMG_SIZE[256];//图片尺寸
	char CREATIVE_IMG_SIZE_IMGLENGTH[128];//图片大小
	char CREATIVE_FILE_PATH[4096];//图片路径

	char CREATIVE_DESTINATION_URL[1024];//访问URL
	char CREATIVE_DISPLAY_URL[1024];//显示URL

	int CREATIVE_IMPRESSION;//展现次数
	int CREATIVE_CLICK;//点击次数
	double CREATIVE_COST;//总消费  
	double CREATIVE_CTR;//点击率
	double CREATIVE_CPC;//平均点击价格
	double CREATIVE_CPM;//千次展现成本

	int PAUSE;

    } ad_system_oci_img_creative_t; //获取图片创意

    typedef struct {

	lg_list_t list;

	int id;
	char code[128];
	char name[128];

    } ad_system_oci_disease_code_t;

    //初始化
    extern int ad_system_oci_init(lg_list_t *head);

    extern int ad_system_oci_login(const char *account, const char *password, char *account_id);
    extern int ad_system_oci_get_title_info(const char *account_id, ad_system_oci_title_info_t *p);

    extern int ad_system_oci_get_user_info(const char *account_id, ad_system_oci_user_info_t *p);
    extern int ad_system_oci_set_user_info(const char *account_id, ad_system_oci_user_info_t *p);

    extern int ad_system_oci_get_user_pass_protect(char *account_id, ad_system_oci_user_pass_protect_t *p);
    extern int ad_system_oci_set_user_pass_protect(char *account_id, ad_system_oci_user_pass_protect_t *p);
    extern int ad_system_oci_set_user_pass(const char *account_id, const char *password);

    //充值
    extern int ad_system_oci_get_ad_money_recharge(lg_list_t *head,
	    char *account_id,
	    int find_type,
	    int money_type,
	    int find_name_type,
	    char *find_name,
	    char *time_start,
	    char *time_end,
	    int sort_type,
	    int sort_column,
	    int page_index_s,
	    int page_index_e);
    extern int ad_system_oci_get_ad_money_recharge_count(char *count,
	    char *account_id,
	    int find_type,
	    int money_type,
	    int find_name_type,
	    char *find_name,
	    char *time_start,
	    char *time_end);
    extern void ad_system_oci_ad_money_recharge_free(lg_list_t *head);

    //公告
    extern int ad_system_oci_get_announcement_count(int find_type, char *find_name, char *count);
    extern int ad_system_oci_get_announcement(int find_type, char *find_name, int page_index_s, int page_index_e, lg_list_t *head);
    extern void ad_system_oci_get_announcement_free(lg_list_t *head);

    //获取物料列表
    extern int ad_system_oci_get_sperad_list(char *account_id, int display_status, lg_list_t *head);
    extern void ad_system_oci_get_sperad_list_free(lg_list_t *head);

    //重命名检测
    extern int ad_system_oci_campaign_name_check(char *account_id, int display_status, char *campaign_name, char *campaign_id);
    extern int ad_system_oci_group_name_check(char *account_id, int display_status, char *campaign_id, char *group_name, char *group_id);


    //获取 计划ID/组ID/创意ID/图片创意ID
    extern int ad_system_oci_get_campaign_id(char *campaign_id);
    extern int ad_system_oci_get_group_id(char *group_id);
    extern int ad_system_oci_get_campaign_from_group(char *account_id, char *campaign_id, char *group_id);
    extern int ad_system_oci_get_campaign_and_group_from_creative(char*account_id,char *campaign_id,char *group_id,char *creative_id);

    //获取计划
    extern int ad_system_oci_get_campaign(const char *account_id, int display_status, int find_type,
	    int campaign_name_type, char *campaign_name, lg_list_t *head);
    extern void ad_system_oci_get_campaign_free(lg_list_t *head);

    //获取组
    int ad_system_oci_get_group(const char *account_id, int display_status, int find_type,
	    char *campaign_id, int group_name_type, char *group_name, lg_list_t *head);
    extern void ad_system_oci_get_group_free(lg_list_t *head);

    //获取组搜索词
    extern int ad_system_oci_get_group_keyword(const char *account_id, char *group_id, char *keyword);

    //获取创意
    extern int ad_system_oci_get_creative(const char *account_id, int find_type,
	    char *campaign_id, char *group_id, int creative_name_type, char *creative_name, lg_list_t *head);
    extern void ad_system_oci_get_creative_free(lg_list_t *head);

    //获取附加创意
    extern int ad_system_oci_get_creative_append(const char *account_id, int find_type, 
	    int sort_type, int sort_column, char *find_name, int status, lg_list_t *head);
    extern void ad_system_oci_get_creative_append_free(lg_list_t *head);

    //获取图片创意
    extern int ad_system_oci_get_img_creative(const char *account_id, int find_type,
	    char *campaign_id, char *group_id, int creative_name_type, char *creative_name, lg_list_t *head);
    extern void ad_system_oci_get_img_creative_free(lg_list_t *head);

    //获取单个计划
    extern int ad_system_oci_get_campaign_unique(const char *account_id, char *campaign_id, ad_system_oci_campaign_t *temp);

    //获取单个组
    extern int ad_system_oci_get_group_unique(const char *account_id, char *group_id, ad_system_oci_group_t *temp);

    //获取单个文字附加创意
    extern int ad_system_oci_get_creative_append_unique(const char *account_id, char *creative_append_id,
	    ad_system_oci_creative_append_t *temp);

    //获取单个图片创意
    extern int ad_system_oci_get_img_creative_unique(char *account_id, char *creative_id, ad_system_oci_img_creative_t *temp);



    //添加计划
    extern int ad_system_oci_add_campaign(const char *account_id,
	    int display_status,
	    char *campaign_id,
	    char *campaign_name,
	    char *dudget,
	    int region_type,
	    char *region_serialize,
	    int schedule_type,
	    char *schedule_serialize,
	    int time_type,
	    char *begin_time,
	    char *end_time);

    //添加组
    extern int ad_system_oci_add_group(const char *account_id,
	    int display_status,
	    char *campaign_id,
	    char *group_id,
	    char *group_name,
	    int group_price_type,
	    char *group_cpc_price,
	    char *group_cpm_price,
	    int targeting_type);

    //添加组定向
    extern int ad_system_oci_add_group_targeting(const char *account_id,
	    char *campaign_id,
	    char *group_id,
	    int targeting_type,
	    char *disease,
	    char *keyword);

    //添加文字创意
    extern int ad_system_oci_add_creative(const char *account_id,
	    char *campaign_id,
	    char *group_id,
	    char *creative_name,
	    char *descriptionpriec,
	    char *destination_url,
	    char *creative_display_url,
	    int creative_append_type,
	    char *creative_append_id);

    //添加文字附加创意
    extern int ad_system_oci_add_creative_append(const char *account_id, char *append_contend);

    //添加图片创意
    extern int ad_system_oci_add_img_creative(const char *account_id,
	    char *campaign_id,
	    char *group_id,
	    char *creative_name,
	    char *creative_img_size,
	    int creative_img_size_imglength,
	    char *creative_file_path,
	    char *creative_destination_url,
	    char *creative_display_url);



    //修改计划
    extern int ad_system_oci_set_campaign(
	    const char *account_id,
	    char *campaign_id,
	    int update_type,
	    int status,
	    char *dudget,
	    int region_type,
	    char *region_serialize,
	    int schedule_type,
	    char *schedule_serialize,
	    int time_type,
	    char *begin_time,
	    char *end_time,
	    char *campign_name);

    //修改组
    extern int ad_system_oci_set_group(const char *account_id,
	    char *group_id,
	    int update_type,
	    int status,
	    char *cpc_price,
	    char *cpm_price,
	    int bidding_model,
	    int targeting_type,
	    char *group_name);

    //修改组定向
    extern int ad_system_oci_set_group_targeting(const char *account_id,
	    char *group_id,
	    int update_type,
	    int targeting_type,
	    char *disease,
	    char *keyword);

    //修改文字创意
    extern int ad_system_oci_set_creative(const char *account_id,
	    char *creative_id,
	    int update_type,
	    int status,
	    char *creative_name,
	    char *descriptionpriec,
	    char *destination_url,
	    char *creative_display_url,
	    char *creative_append_id,
	    char *mobile_destination_url,
	    char *mobile_display_url);

    //修改文字附加创意
    extern int ad_system_oci_set_creative_append(const char *account_id,
	    char *creative_append_id,
	    int update_type,
	    int status,
	    char *contend);

    //修改图片创意
    extern int ad_system_oci_set_img_creative(char *account_id,
	    char *creative_id,
	    int update_type,
	    int creative_status,
	    char *creative_name,
	    char *creative_destination_url,
	    char *creative_display_url);

    //删除计划
    extern int ad_system_oci_del_campaign(char *account_id, char *campaign_id);

    //删除组 
    extern int ad_system_oci_del_group(char *account_id, char *group_id);

    //删除文字创意
    extern int ad_system_oci_del_creative(char *account_id, char *creative_id);

    //删除文字附加创意
    extern int ad_system_oci_del_creative_append(char *account_id, char *append_id);

    //删除图片创意
    extern int ad_system_oci_del_img_creative(char *account_id, char *creative_id);

    //获取科室病种CODE
    extern int ad_system_oci_get_disease_code(lg_list_t *head);
    extern void ad_system_oci_get_disease_code_free(lg_list_t *head);

    extern int ad_system_oci_get_group_price_and_campaign_budget(const char *account_id, char *group_id,
	    double *CAMPAIGN_BUDGET,
	    int *GROUP_BIDDING_MODEL,
	    double *GROUP_CPC_PRICE,
	    double *GROUP_CPM_PRICE);

#ifdef __cplusplus
}
#endif

#endif


