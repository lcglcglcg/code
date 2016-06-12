
#include <stdlib.h>
#include "lg_oci.h"
#include "lg_list.h"
#include "lg_rbtree.h"
#include "lg_string.h"
#include "lg_config_file.h"
#include "ad_system_oci.h"
#include "ad_system_upload_image.h"

#define	AD_SYSTEM_SQL_LOGIN "select ACCOUNT_ID from T_ACCOUNT where LOGIN_NAME='%s' and LOGIN_PASSWORD_MD5='%s' and ROW_STATE=1"
#define	AD_SYSTEM_SQL_GET_TITLE_INFO "select \
    nvl(to_char(LOGIN_NAME), 'null'),\
nvl(to_char(COMPANY_NAME), 'null'),\
nvl(to_char(round(BALANCE, 2)), 'null'),\
nvl(to_char(round(INVEST, 2)), 'null'),\
nvl(to_char(round(INVEST - BALANCE, 2)), 'null'),\
to_char(CREATE_TIME, 'yyyy-mm-dd') \
from T_ACCOUNT where ACCOUNT_ID=%s"

#define	AD_SYSTEM_SQL_GET_USER_INFO "select \
    nvl(to_char(LOGIN_NAME), 'null'),\
nvl(to_char(COMPANY_NAME), 'null'),\
nvl(to_char(COMPANY_SITE), 'null'),\
nvl(to_char(INDUSTRY), 'null'),\
nvl(to_char(API_TOKEN), 'null'),\
nvl(to_char(PROVINCE_ID), 'null'),\
nvl(to_char(CITY_ID), 'null'),\
nvl(to_char(ADDRESS), 'null'),\
nvl(to_char(POSTCODE), 'null'),\
nvl(to_char(CONTACT), 'null'),\
nvl(to_char(TELEPHONE), 'null'),\
nvl(to_char(FAX), 'null'),\
nvl(to_char(MOBILE), 'null'),\
nvl(to_char(EMAIL), 'null'),\
nvl(to_char(WEBIM), 'null'),\
to_char(LATE_LOGIN_TIME, 'yyyy-mm-dd HH24:MI:SS'),\
nvl(to_char(LATE_LOGIN_IP), 'null'),\
nvl(to_char(LATE_LOGIN_ADDRESS), 'null') \
from T_ACCOUNT where ACCOUNT_ID=%s"

#define	AD_SYSTEM_SQL_SET_USER_INFO "update T_ACCOUNT set \
    LOGIN_NAME='%s',\
COMPANY_NAME='%s',\
COMPANY_SITE='%s',\
INDUSTRY='%s',\
API_TOKEN='%s',\
PROVINCE_ID='%s',\
CITY_ID='%s',\
ADDRESS='%s',\
POSTCODE='%s',\
CONTACT='%s',\
TELEPHONE='%s',\
FAX='%s',\
MOBILE='%s',\
EMAIL='%s',\
WEBIM='%s'\
where ACCOUNT_ID=%s"

#define	AD_SYSTEM_SQL_GET_USER_PASS_PROTECT "select \
    nvl(to_char(PROTECT_QUESTION1), 'null'),\
nvl(to_char(PROTECT_ANSWER1), 'null'),\
nvl(to_char(PROTECT_QUESTION2), 'null'),\
nvl(to_char(PROTECT_ANSWER2), 'null'),\
nvl(to_char(PROTECT_QUESTION3), 'null'),\
nvl(to_char(PROTECT_ANSWER3), 'null'),\
nvl(to_char(LOGIN_PASSWORD_MD5), 'null') \
from T_ACCOUNT where ACCOUNT_ID=%s"

#define	AD_SYSTEM_SQL_SET_USER_PASS_PROTECT "update T_ACCOUNT set \
    PROTECT_QUESTION1='%s',\
PROTECT_ANSWER1='%s',\
PROTECT_QUESTION2='%s',\
PROTECT_ANSWER2='%s',\
PROTECT_QUESTION3='%s',\
PROTECT_ANSWER3 ='%s'\
where ACCOUNT_ID=%s"

#define	AD_SYSTEM_SQL_SET_USER_PASS "update T_ACCOUNT set LOGIN_PASSWORD='%s', LOGIN_PASSWORD_MD5='%s' where ACCOUNT_ID=%s"
#define	AD_SYSTEM_SQL_GET_MONEY_RECHARGE "select \
    nvl(to_char(SWIFT_NUMBER), 'null'),\
nvl(to_char(round(MONEY, 2)), 'null'),\
nvl(to_char(PAYMENT_TYPE), 'null'),\
nvl(to_char(ACCOUNTING_DOCUMENT_FILE), 'null'),\
nvl(to_char(round(INVEST, 2)), 'null'),\
nvl(to_char(round(CURRENT_MONEY_BALANCE, 2)), 'null'),\
to_char(LAST_UPDATE_TIME, 'yyyy-mm-dd HH24:MI:SS') \
from T_RECHARGE where account_id = %s"

#define	AD_SYSTEM_SQL_GET_REGION_CODE "select NAME_CN, CODE from T_SYS_REGION"

#define	AD_SYSTEM_SQL_GET_SPERAD_LIST "select campaign.campaign_id,campaign.campaign_name,campaign.pause,nvl(adgroup.adgroup_id, 'null'),nvl(adgroup.adgroup_name, 'null'),nvl(to_char(adgroup.adgroup_state), 'null') from t_campaign campaign left join t_adgroup adgroup on campaign.campaign_id = adgroup.campaign_id where campaign.account_id=%s"

#define	AD_SYSTEM_SQL_GET_CAMPAIGN "select CAMPAIGN_ID,\
    CAMPAIGN_NAME,\
CAMPAIGN_STATE,\
PAUSE,\
CAMPAIGN_BUDGET,\
SCHEDULE_TYPE,\
SCHEDULE_SERIALIZE,\
DATE_SYSDATE,\
REGION_TYPE,\
REGION_SERIALIZE,\
BEGIN_TIME,\
END_TIME,\
DATE_SUB_BEGIN_TIME,\
DATE_SUB_END_TIME \
from (select rownum as PAGE_INDEX,\
	ACCOUNT_ID,\
	CAMPAIGN_ID,\
	nvl(to_char(CAMPAIGN_NAME), 'null') as CAMPAIGN_NAME,\
	CAMPAIGN_STATE,\
	PAUSE,\
	to_char(round(CAMPAIGN_BUDGET, 2)) as CAMPAIGN_BUDGET,\
	SCHEDULE_TYPE,\
	nvl(to_char(SCHEDULE_SERIALIZE), 'null') as SCHEDULE_SERIALIZE,\
	to_char(sysdate, 'DYHH24') as DATE_SYSDATE,\
	REGION_TYPE,\
	nvl(to_char(REGION_SERIALIZE), 'null') as REGION_SERIALIZE,\
	nvl(to_char(BEGIN_TIME, 'yyyy-mm-dd'), 'null') as BEGIN_TIME,\
	nvl(to_char(END_TIME, 'yyyy-mm-dd'), 'null') as END_TIME,\
	round(sysdate - nvl(BEGIN_TIME, sysdate), 2) as DATE_SUB_BEGIN_TIME,\
	round(sysdate - nvl(END_TIME, sysdate), 2) as DATE_SUB_END_TIME \
	from T_CAMPAIGN where account_id = %s) t \
where t.PAGE_INDEX BETWEEN %u and %u %s"

#define	AD_SYSTEM_SQL_GET_CAMPAIGN_COUNT "select count(*) from T_CAMPAIGN where account_id = %s %s"

#define	AD_SYSTEM_SQL_GET_GROUP "select t.group_id,\
    campaign.campaign_name,\
t.group_name,\
t.group_price,\
t.group_pause,\
campaign.CAMPAIGN_STATE as CAMPAIGN_STATE,\
campaign.PAUSE as CAMPAIGN_PAUSE,\
campaign.SCHEDULE_TYPE as CAMPAIGN_SCHEDULE_TYPE,\
nvl(to_char(campaign.SCHEDULE_SERIALIZE), 'null') as CAMPAIGN_SCHEDULE_SERIALIZE,\
to_char(sysdate, 'DYHH24') as CAMPAIGN_SCHEDULE_DATE,\
nvl(to_char(campaign.BEGIN_TIME, 'yyyy-mm-dd'), 'null') as CAMPAIGN_BEGIN_TIME,\
nvl(to_char(campaign.END_TIME, 'yyyy-mm-dd'), 'null') as CAMPAIGN_END_TIME,\
round(sysdate - nvl(campaign.BEGIN_TIME, sysdate), 2) as CAMPAIGN_DATE_SUB_BEGIN_TIME,\
round(sysdate - nvl(campaign.END_TIME, sysdate), 2) as CAMPAIGN_DATE_SUB_END_TIME,\
t.targeting_type,\
nvl(to_char(group_targeting.code), 'null') as disease_code \
from (select rownum as PAGE_INDEX,\
	adgroup.campaign_id as CAMPAIGN_ID,\
	adgroup.adgroup_id as GROUP_ID,\
	adgroup.adgroup_name as GROUP_NAME,\
	adgroup.cpc_price as GROUP_PRICE,\
	adgroup.ADGROUP_STATE as GROUP_PAUSE,\
	adgroup.targeting_type \
	from t_adgroup adgroup where adgroup.account_id = %s) t \
left join t_campaign campaign on t.CAMPAIGN_ID = campaign.campaign_id \
left join t_adgroup_targeting group_targeting on t.group_id = group_targeting.adgroup_id \
where t.PAGE_INDEX BETWEEN %u and %u %s"

#define	AD_SYSTEM_SQL_GET_GROUP_COUNT "select count(*) from t_adgroup adgroup left join t_campaign campaign on adgroup.CAMPAIGN_ID=campaign.campaign_id where adgroup.account_id = %s %s"

#define	AD_SYSTEM_SQL_GET_CREATIVE "select creative.creative_id,\
    campaign.campaign_name,\
adgroup.adgroup_name as group_name,\
creative.creative_name,\
nvl(to_char(creative_append.content), 'null') as creative_append,\
creative.creative_pause,\
creative.creative_state,\
nvl(to_char(creative.creative_refuse_reason), 'null') as creative_refuse_reason,\
nvl(to_char(creative.creative_description), 'null') as creative_description,\
nvl(to_char(creative.creative_destination_url), 'null') as creative_destination_url,\
nvl(to_char(creative.creative_display_url), 'null') as creative_display_url \
from (select rownum as PAGE_INDEX,\
	campaign_id,\
	adgroup_id,\
	creative_id,\
	append_id,\
	title as creative_name,\
	pause as creative_pause,\
	creative_state,\
	refuse_reason as creative_refuse_reason,\
	description1 as creative_description,\
	destination_url as creative_destination_url,\
	display_url as creative_display_url \
	from t_text_creative where account_id = %s) creative \
left join t_adgroup adgroup on creative.adgroup_id = adgroup.adgroup_id \
left join t_campaign campaign on creative.campaign_id =campaign.campaign_id \
left join t_text_creative_append creative_append on creative.append_id = creative_append.append_id \
where creative.page_index between %u and %u %s"

#define AD_SYSTEM_SQL_GET_CREATIVE_COUNT "select count(*) from t_text_creative creative left join t_adgroup adgroup on creative.adgroup_id = adgroup.adgroup_id left join t_campaign campaign on creative.campaign_id = campaign.campaign_id where creative.account_id = %s %s"

#define	AD_SYSTEM_SQL_CAMPAIGN_TO_DEL_TABLE "insert into t_deleted_campaign t_del (t_del.CAMPAIGN_ID,t_del.ACCOUNT_ID,t_del.CAMPAIGN_NAME,t_del.CAMPAIGN_BUDGET,t_del.REGION_TYPE,t_del.REGION_SERIALIZE,t_del.BEGIN_TIME,t_del.END_TIME,t_del.SCHEDULE_TYPE,t_del.SCHEDULE_SERIALIZE,t_del.MAXIMP,t_del.CAMPAIGN_STATE,t_del.PAUSE,t_del.IMPRESSION,t_del.CLICK,t_del.COST,t_del.DIALOG,t_del.APPOINTMENT,t_del.ARRIVAL,t_del.CREATE_TIME,t_del.LAST_UPDATE_TIME,t_del.DELETED_TIME,t_del.SYNCH_STATE,t_del.BIDDING_MODEL) select t.CAMPAIGN_ID,t.ACCOUNT_ID,t.CAMPAIGN_NAME,t.CAMPAIGN_BUDGET,t.REGION_TYPE,t.REGION_SERIALIZE,t.BEGIN_TIME,t.END_TIME,t.SCHEDULE_TYPE,t.SCHEDULE_SERIALIZE,t.MAXIMP,t.CAMPAIGN_STATE,t.PAUSE,t.IMPRESSION,t.CLICK,t.COST,t.DIALOG,t.APPOINTMENT,t.ARRIVAL,t.CREATE_TIME,t.LAST_UPDATE_TIME,sysdate,t.SYNCH_STATE,t.BIDDING_MODEL from t_campaign t where t.ACCOUNT_ID=%s and t.CAMPAIGN_ID='%s'"

#define	AD_SYSTEM_SQL_GROUP_TO_DEL_TABLE "insert into t_deleted_adgroup t_del (t_del.ADGROUP_ID,t_del.ACCOUNT_ID,t_del.CAMPAIGN_ID,t_del.ADGROUP_NAME,t_del.CPC_PRICE,t_del.CPM_PRICE,t_del.TARGETING_TYPE,t_del.ADGROUP_STATE,t_del.IMPRESSION,t_del.CLICK,t_del.COST,t_del.DIALOG,t_del.APPOINTMENT,t_del.ARRIVAL,t_del.CREATE_TIME,t_del.LAST_UPDATE_TIME,t_del.DELETED_TIME,t_del.SYNCH_STATE) select t.ADGROUP_ID,t.ACCOUNT_ID,t.CAMPAIGN_ID,t.ADGROUP_NAME,t.CPC_PRICE,t.CPM_PRICE,t.TARGETING_TYPE,t.ADGROUP_STATE,t.IMPRESSION,t.CLICK,t.COST,t.DIALOG,t.APPOINTMENT,t.ARRIVAL,t.CREATE_TIME,t.LAST_UPDATE_TIME,sysdate,t.SYNCH_STATE from t_adgroup t where t.account_id=%s "

#define	AD_SYSTEM_SQL_CREATIVE_TO_DEL_TABLE "insert into t_deleted_text_creative t_del (t_del.CREATIVE_ID,t_del.ACCOUNT_ID,t_del.CAMPAIGN_ID,t_del.ADGROUP_ID,t_del.TITLE,t_del.DESCRIPTION1,t_del.DESCRIPTION2,t_del.DESTINATION_URL,t_del.DISPLAY_URL,t_del.ECTR,t_del.CREATIVE_STATE,t_del.REFUSE_REASON,t_del.PAUSE,t_del.QUALITY,t_del.IMPRESSION,t_del.CLICK,t_del.COST,t_del.DIALOG,t_del.APPOINTMENT,t_del.ARRIVAL,t_del.CREATE_TIME,t_del.LAST_UPDATE_TIME,t_del.AUDITOR_ID,t_del.APPEND_ID,t_del.DELETED_TIME,t_del.SYNCH_STATE) select t.CREATIVE_ID,t.ACCOUNT_ID,t.CAMPAIGN_ID,t.ADGROUP_ID,t.TITLE,t.DESCRIPTION1,t.DESCRIPTION2,t.DESTINATION_URL,t.DISPLAY_URL,t.ECTR,t.CREATIVE_STATE,t.REFUSE_REASON,t.PAUSE,t.QUALITY,t.IMPRESSION,t.CLICK,t.COST,t.DIALOG,t.APPOINTMENT,t.ARRIVAL,t.CREATE_TIME,t.LAST_UPDATE_TIME,t.AUDITOR_ID,t.APPEND_ID,sysdate,t.SYNCH_STATE from t_text_creative t where t.account_id=%s "

char *oracle_name = NULL;
char *oracle_pass = NULL;
char *oracle_host = NULL;

int ad_system_oci_init(lg_list_t *config_file_head)
{

    if (!(oracle_name = lg_config_file_find(config_file_head, "oracle_name")))
	return -1;

    if (!(oracle_pass = lg_config_file_find(config_file_head, "oracle_pass")))
	return -1;

    if (!(oracle_host = lg_config_file_find(config_file_head, "oracle_host")))
	return -1;

    //fprintf(stdout, "ORACLE_NAME=%s\n", oracle_name);
    //fprintf(stdout, "ORACLE_PASS=%s\n", oracle_pass);
    //fprintf(stdout, "ORACLE_HOST=%s\n", oracle_host);

    setenv("NLS_LANG", "AMERICAN_AMERICA.UTF8", 1);
    return OCIInitialize(OCI_DEFAULT, NULL, NULL, NULL, NULL);
}

int ad_system_oci_login(const char *account, const char *password, char *account_id)
{

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    if (SQLOCI_Bind_Ouput(&conn, "%s", account_id) != 0) {

	fprintf(stdout, "SQLOCI_Bind_Ouput::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    char sql_str[1024] = {0};
    size_t sql_size = sprintf(sql_str, AD_SYSTEM_SQL_LOGIN, account, password);
    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    int ret = SQLOCI_Execute(&conn);
    if (!(ret == 0 || ret == 100)) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    SQLOCI_Close(&conn);
    return ret;
}

int ad_system_oci_get_title_info(const char *account_id, ad_system_oci_title_info_t *p)
{

    char sql_str[1024] = {0};
    size_t sql_size = sprintf(sql_str, "select LOGIN_NAME,COMPANY_NAME,BALANCE,INVEST,INVEST - BALANCE, to_char(CREATE_TIME, 'yyyy-mm-dd'), nvl(domain, 'null'), nvl(GUAHAO_TOKEN, 'null'),nvl(WEBIM,'null') from T_ACCOUNT where ACCOUNT_ID=%s", account_id);
    fprintf(stdout, "%s\n", sql_str);


    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    if (SQLOCI_Bind_Ouput(&conn, "1%s2%s3%f4%f5%f6%s7%s8%s9%s",
		p->USER_LOGIN_NAME,
		p->COMPANY_NAME,
		&p->BALANCE,
		&p->INVEST,
		&p->CONSUMPTION,
		p->CREATE_TIME,
		p->DOMAIN,
		p->GUAHAO_TOKEN,
		p->WEBIM) != 0) {

	fprintf(stdout, "SQLOCI_Bind_Ouput::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Execute(&conn) != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    SQLOCI_Close(&conn);
    return 0;
}

int ad_system_oci_get_user_info(const char *account_id, ad_system_oci_user_info_t *p)
{

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    if (SQLOCI_Bind_Ouput(&conn, "1%s2%s3%s4%s5%s6%s7%s8%s9%s10%s11%s12%s13%s14%s15%s16%s17%s18%s",
		p->LOGIN_NAME,
		p->COMPANY_NAME,
		p->COMPANY_SITE,
		p->INDUSTRY,
		p->API_TOKEN,
		p->PROVINCE_ID,
		p->CITY_ID,
		p->ADDRESS,
		p->POSTCODE,
		p->CONTACT,
		p->TELEPHONE,
		p->FAX,
		p->MOBILE,
		p->EMAIL,
		p->WEBIM,
		p->LATE_LOGIN_TIME,
		p->LATE_LOGIN_IP,
		p->LATE_LOGIN_ADDRESS) != 0) {

	fprintf(stdout, "SQLOCI_Bind_Ouput::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    char sql_str[1024] = {0};
    size_t sql_size = sprintf(sql_str, AD_SYSTEM_SQL_GET_USER_INFO, account_id);
    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Execute(&conn) != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    SQLOCI_Close(&conn);
    return 0;
}

int ad_system_oci_set_user_info(const char *account_id, ad_system_oci_user_info_t *p)
{

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    char sql_str[1024] = {0};
    size_t sql_size = sprintf(sql_str, AD_SYSTEM_SQL_SET_USER_INFO,
	    p->LOGIN_NAME,
	    p->COMPANY_NAME,
	    p->COMPANY_SITE,
	    p->INDUSTRY,
	    p->API_TOKEN,
	    p->PROVINCE_ID,
	    p->CITY_ID,
	    p->ADDRESS,
	    p->POSTCODE,
	    p->CONTACT,
	    p->TELEPHONE,
	    p->FAX,
	    p->MOBILE,
	    p->EMAIL,
	    p->WEBIM,
	    account_id);

    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Execute(&conn) != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Commit(&conn) != 0) {

	fprintf(stdout, "SQLOCI_Commit::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    SQLOCI_Close(&conn);
    return 0;
}

int ad_system_oci_get_user_pass_protect(char *account_id, ad_system_oci_user_pass_protect_t *p)
{

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    if (SQLOCI_Bind_Ouput(&conn, "1%s2%s3%s4%s5%s6%s7%s",
		p->PROTECT_QUESTION1,
		p->PROTECT_ANSWER1,
		p->PROTECT_QUESTION2,
		p->PROTECT_ANSWER2,
		p->PROTECT_QUESTION3,
		p->PROTECT_ANSWER3,
		p->LOGIN_PASSWORD_MD5,
		account_id) != 0) {

	fprintf(stdout, "SQLOCI_Bind_Ouput::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    char sql_str[1024] = {0};
    size_t sql_size = sprintf(sql_str, AD_SYSTEM_SQL_GET_USER_PASS_PROTECT, account_id);
    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    int ret = SQLOCI_Execute(&conn);
    if (!(ret == 0 || ret == 100)) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    SQLOCI_Close(&conn);
    return 0;
}

int ad_system_oci_set_user_pass_protect(char *account_id, ad_system_oci_user_pass_protect_t *p)
{

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    char sql_str[1024] = {0};
    size_t sql_size = sprintf(sql_str, AD_SYSTEM_SQL_SET_USER_PASS_PROTECT,
	    p->PROTECT_QUESTION1,
	    p->PROTECT_ANSWER1,
	    p->PROTECT_QUESTION2,
	    p->PROTECT_ANSWER2,
	    p->PROTECT_QUESTION3,
	    p->PROTECT_ANSWER3,
	    account_id);

    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Execute(&conn) != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Commit(&conn) != 0) {

	fprintf(stdout, "SQLOCI_Commit::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    SQLOCI_Close(&conn);
    return 0;
}

int ad_system_oci_set_user_pass(const char *account_id, const char *password)
{

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    char sql_str[1024] = {0};
    char md5_buffer[1024] = {0};
    size_t sql_size = sprintf(sql_str, AD_SYSTEM_SQL_SET_USER_PASS, password, lg_string_md5(password, md5_buffer), account_id);
    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Execute(&conn) != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Commit(&conn) != 0) {

	fprintf(stdout, "SQLOCI_Commit::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    SQLOCI_Close(&conn);
    return 0;
}

void ad_system_oci_ad_money_recharge_free(lg_list_t *head)
{

    lg_list_t *p = head->next;
    ad_system_oci_money_recharge_t *node = NULL;

    while(p != head) {

	node = (ad_system_oci_money_recharge_t *)p;
	p = p->next;

	free(node);
    }

    lg_list_init(head);
}

void ad_system_oci_get_sperad_list_add(lg_list_t *head, 
	char *CAMPAIGN_ID, 
	char *CAMPAIGN_NAME, 
	int CAMPAIGN_STATUS, 
	char *GROUP_ID,
	char *GROUP_NAME,
	char *GROUP_STATUS)
{

#if 0
    fprintf(stdout, "[%s][%s][%d][%s][%s][%d]\n",
	    CAMPAIGN_ID,
	    CAMPAIGN_NAME,
	    CAMPAIGN_STATUS,
	    GROUP_ID,
	    GROUP_NAME,
	    GROUP_STATUS);
#endif

    lg_list_t *p = NULL;
    ad_system_oci_sperad_list_campaign_t *campaign = NULL;
    lg_list_for_each(p, head) {

	campaign = (ad_system_oci_sperad_list_campaign_t *)p;
	if (strcmp(campaign->CAMPAIGN_ID, CAMPAIGN_ID) == 0) {

	    if (strcmp(GROUP_ID, "null") == 0)
		return;

	    ad_system_oci_sperad_list_group_t *group = NULL;
	    if ((group = malloc(sizeof(ad_system_oci_sperad_list_group_t)))) {

		strcpy(group->GROUP_ID, GROUP_ID);
		strcpy(group->GROUP_NAME, GROUP_NAME);
		strcpy(group->GROUP_STATUS, GROUP_STATUS);
		lg_list_add(&campaign->head, &group->list);
		return;
	    }
	}
    }

    if ((campaign = malloc(sizeof(ad_system_oci_sperad_list_campaign_t)))) {

	strcpy(campaign->CAMPAIGN_ID, CAMPAIGN_ID);
	strcpy(campaign->CAMPAIGN_NAME, CAMPAIGN_NAME);
	campaign->CAMPAIGN_STATUS = CAMPAIGN_STATUS;
	lg_list_init(&campaign->head);
	lg_list_add(head, &campaign->list);

	if (strcmp(GROUP_ID, "null") == 0)
	    return;

	ad_system_oci_sperad_list_group_t *group = NULL;
	if ((group = malloc(sizeof(ad_system_oci_sperad_list_group_t)))) {

	    strcpy(group->GROUP_ID, GROUP_ID);
	    strcpy(group->GROUP_NAME, GROUP_NAME);
	    strcpy(group->GROUP_STATUS, GROUP_STATUS);
	    lg_list_add(&campaign->head, &group->list);
	}
    }
}

int ad_system_oci_get_sperad_list(char *account_id, int display_status, lg_list_t *head)
{

    lg_list_init(head);

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    char CAMPAIGN_ID[64] = {0};
    char CAMPAIGN_NAME[1024] = {0};
    int CAMPAIGN_STATUS = 0;
    char GROUP_ID[64] = {0};
    char GROUP_NAME[1024] = {0};
    char GROUP_STATUS[64] = {0};
    if (SQLOCI_Bind_Ouput(&conn, "1%s2%s3%d4%s5%s6%s",
		CAMPAIGN_ID,
		CAMPAIGN_NAME,
		&CAMPAIGN_STATUS,
		GROUP_ID,
		GROUP_NAME,
		GROUP_STATUS) != 0) {

	fprintf(stdout, "SQLOCI_Bind_Ouput::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    char sql_str[1024] = {0};
    size_t sql_size = sprintf(sql_str, "select campaign.campaign_id,campaign.campaign_name,campaign.pause,nvl(adgroup.adgroup_id, 'null'),nvl(adgroup.adgroup_name, 'null'),nvl(to_char(adgroup.adgroup_state), 'null') from t_campaign campaign left join t_adgroup adgroup on campaign.campaign_id = adgroup.campaign_id where campaign.account_id=%s and campaign.DISPLAY_STYLE=%u order by NLSSORT(campaign.campaign_name, 'NLS_SORT=SCHINESE_PINYIN_M')",
	    account_id, display_status);

    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    int ret = SQLOCI_Execute(&conn);
    if (ret != 0) {

	SQLOCI_Close(&conn);
	return -1;
    }

    while(ret == 0) {

	ad_system_oci_get_sperad_list_add(head, CAMPAIGN_ID, CAMPAIGN_NAME, CAMPAIGN_STATUS, GROUP_ID, GROUP_NAME, GROUP_STATUS);
	ret = SQLOCI_Next_Data(&conn);
    }

    SQLOCI_Close(&conn);
    return 0;
}

void ad_system_oci_get_sperad_list_free(lg_list_t *head)
{

    lg_list_t *campaign_p = head->next;
    while(campaign_p != head) {

	ad_system_oci_sperad_list_campaign_t *campaign_node = NULL;
	campaign_node = (ad_system_oci_sperad_list_campaign_t *)campaign_p;
	lg_list_t *group_p = campaign_node->head.next;
	while(group_p != &campaign_node->head) {

	    ad_system_oci_sperad_list_group_t *group_node = NULL;
	    group_node = (ad_system_oci_sperad_list_group_t *)group_p;

	    group_p = group_p->next;
	    free(group_node);
	}

	campaign_p = campaign_p->next;
	free(campaign_node);
    }

    lg_list_init(head);
}

int ad_system_oci_campaign_name_check(char *account_id, int display_status, char *campaign_name, char *campaign_id)
{

    char sql_str[1024] = {0};
    size_t sql_size = sprintf(sql_str,
	    "select CAMPAIGN_ID from T_CAMPAIGN where ACCOUNT_ID=%s and DISPLAY_STYLE=%u and CAMPAIGN_NAME='%s'",
	    account_id, 
	    display_status, 
	    campaign_name);

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    if (SQLOCI_Bind_Ouput(&conn, "%s", campaign_id) != 0) {

	fprintf(stdout, "SQLOCI_Bind_Ouput::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    int ret = SQLOCI_Execute(&conn);
    if (ret == 100) {

	SQLOCI_Close(&conn);
	return 100;

    } else if (ret != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    SQLOCI_Close(&conn);
    return 0;
}

int ad_system_oci_group_name_check(char *account_id, int display_status, char *campaign_id, char *group_name, char *group_id)
{

    char sql_str[1024] = {0};
    size_t sql_size = sprintf(sql_str, 
	    "select ADGROUP_ID from T_ADGROUP where ACCOUNT_ID=%s and DISPLAY_STYLE=%u and CAMPAIGN_ID='%s'and ADGROUP_NAME='%s'",
	    account_id,
	    display_status,
	    campaign_id,
	    group_name);

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    if (SQLOCI_Bind_Ouput(&conn, "%s", group_id) != 0) {

	fprintf(stdout, "SQLOCI_Bind_Ouput::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    int ret = SQLOCI_Execute(&conn);
    if (ret == 100) {

	SQLOCI_Close(&conn);
	return 100;

    } else if (ret != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    SQLOCI_Close(&conn);
    return 0;
}

int ad_system_oci_get_campaign_id(char *campaign_id)
{

    char sql_str[1024] = {0};
    size_t sql_size = sprintf(sql_str, "select SEQ_T_CAMPAIGN.NEXTVAL from dual");

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    if (SQLOCI_Bind_Ouput(&conn, "%s", campaign_id) != 0) {

	fprintf(stdout, "SQLOCI_Bind_Ouput::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Execute(&conn) != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    SQLOCI_Close(&conn);
    return 0;

}

int ad_system_oci_get_campaign_schedule_check(char *sysdate, char *schedule)
{

    int week = 0;
    if (strncmp(sysdate, "MON", 3) == 0) {
	week = 0;
    } else if (strncmp(sysdate, "TUE", 3) == 0) {
	week = 1;
    } else if (strncmp(sysdate, "WED", 3) == 0) {
	week = 2;
    } else if (strncmp(sysdate, "THU", 3) == 0) {
	week = 3;
    } else if (strncmp(sysdate, "FRI", 3) == 0) {
	week = 4;
    } else if (strncmp(sysdate, "SAT", 3) == 0) {
	week = 5;
    } else if (strncmp(sysdate, "SUN", 3) == 0) {
	week = 6;
    } else {
	return -1;
    }

    int hour = atoi(sysdate + 3);
    if (hour < 0 && hour > 23)
	return -1;

    if (schedule[week * 24 + hour] == '1')
	return 1;
    else if (schedule[week * 24 + hour] == '0')
	return 0;

    return -1;
}

int ad_system_oci_get_campaign(const char *account_id, int display_status, int find_type,
	int campaign_name_type, char *campaign_name, lg_list_t *head)
{

    lg_list_init(head);
    char sql_str[4096] = {0};
    size_t sql_size = sprintf(sql_str, 
	    "select CAMPAIGN_ID,nvl(to_char(CAMPAIGN_NAME), 'null') as CAMPAIGN_NAME,CAMPAIGN_STATE,decode(PAUSE, 0, 1, 1, 2, 1) as PAUSE,CAMPAIGN_BUDGET,decode(SCHEDULE_TYPE, 0, 1, 1, 2, 1) as SCHEDULE_TYPE,nvl(to_char(SCHEDULE_SERIALIZE), 'null') as SCHEDULE_SERIALIZE,to_char(sysdate, 'DYHH24') as SCHEDULE_TIME,decode(REGION_TYPE, 0, 1, 1, 2, 1) as REGION_TYPE,nvl(to_char(REGION_SERIALIZE), 'null') as REGION_SERIALIZE,nvl(to_char(BEGIN_TIME, 'yyyy-mm-dd HH24:MI:SS'), 'null') as BEGIN_TIME,nvl(to_char(END_TIME, 'yyyy-mm-dd'), 'null') as END_TIME,round(sysdate - nvl(BEGIN_TIME, sysdate), 2) as DATE_SUB_BEGIN_TIME,round(sysdate - nvl(END_TIME, sysdate), 2) as DATE_SUB_END_TIME from T_CAMPAIGN where ACCOUNT_ID=%s and DISPLAY_STYLE=%d ", 
	    account_id, 
	    display_status);

    if (find_type & 2) {

	if (campaign_name_type == 1)
	    sql_size += sprintf(&sql_str[sql_size], " and CAMPAIGN_NAME='%s'", campaign_name);

	if (campaign_name_type == 2)
	    sql_size += sprintf(&sql_str[sql_size], " and CAMPAIGN_NAME like '%%%s%%'", campaign_name);
    }

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    int CAMPAIGN_STATE = 0;
    int PAUSE = 0;
    char SCHEDULE_TIME[128] = {0};
    double DATE_SUB_BEGIN_TIME = 0;
    double DATE_SUB_END_TIME = 0;
    ad_system_oci_campaign_t temp;
    memset(&temp, 0, sizeof(ad_system_oci_campaign_t));
    if (SQLOCI_Bind_Ouput(&conn, "1%s2%s3%d4%d5%f6%d7%s8%s9%d10%s11%s12%s13%f14%f",
		temp.CAMPAIGN_ID,
		temp.CAMPAIGN_NAME,
		&CAMPAIGN_STATE,
		&PAUSE,
		&temp.CAMPAIGN_BUDGET,
		&temp.SCHEDULE_TYPE,
		temp.SCHEDULE_SERIALIZE,
		SCHEDULE_TIME,
		&temp.REGION_TYPE,
		temp.REGION_SERIALIZE,
		temp.BEGIN_TIME,
		temp.END_TIME,
		&DATE_SUB_BEGIN_TIME,
		&DATE_SUB_END_TIME) != 0) {

	fprintf(stdout, "SQLOCI_Bind_Ouput::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    int ret = SQLOCI_Execute(&conn);
    if (ret == 100) {

	SQLOCI_Close(&conn);
	return 100;

    } else if (ret != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    while(ret == 0) {

	if (PAUSE == 2) {
	    temp.CAMPAIGN_STATE = 5;//暂停

	} else if (DATE_SUB_BEGIN_TIME < 0) {
	    temp.CAMPAIGN_STATE = 4;//未开始

	} else if (DATE_SUB_END_TIME < 0) {
	    temp.CAMPAIGN_STATE = 6;//下线

	} else if (CAMPAIGN_STATE == 13) {
	    temp.CAMPAIGN_STATE = 3;//预算不足

	} else if (temp.SCHEDULE_TYPE == 2 
		&& ad_system_oci_get_campaign_schedule_check(SCHEDULE_TIME, temp.SCHEDULE_SERIALIZE) == 0) {
	    temp.CAMPAIGN_STATE = 2;//暂停时段

	    //	} else if (DATE_SUB_BEGIN_TIME < 0) {
	    //	    temp.CAMPAIGN_STATE = 6;//下线

    } else {
	temp.CAMPAIGN_STATE = 1;//有效
    }

    if (strcmp(temp.END_TIME, "null") == 0)
	temp.TIME_TYPE = 1;
    else
	temp.TIME_TYPE = 2;

    ad_system_oci_campaign_t *node = NULL;
    if ((node = malloc(sizeof(ad_system_oci_campaign_t)))) {

	memcpy(node, &temp, sizeof(ad_system_oci_campaign_t));
	lg_list_add(head, &node->list);
    }

    memset(&temp, 0, sizeof(ad_system_oci_campaign_t));
    ret = SQLOCI_Next_Data(&conn);
    }

    SQLOCI_Close(&conn);
    return 0;
}

void ad_system_oci_get_campaign_free(lg_list_t *head)
{

    lg_list_t *p = head->next;
    ad_system_oci_campaign_t *node = NULL;

    while(p != head) {

	node = (ad_system_oci_campaign_t *)p;
	p = p->next;
	free(node);
    }

    lg_list_init(head);
}

int ad_system_oci_get_group(const char *account_id, int display_status, int find_type,
	char *campaign_id, int group_name_type, char *group_name, lg_list_t *head)
{

    lg_list_init(head);
    char sql_str[4096] = {0};
    size_t sql_size = sprintf(sql_str, 
	    "select campaign.campaign_id,campaign.campaign_name,adgroup.adgroup_id,adgroup.adgroup_name,nvl(adgroup.cpc_price,0),nvl(adgroup.cpm_price,0),decode(adgroup.adgroup_state, 21, 1, 22, 2, 1) as group_state,decode(campaign.pause, 0, 1, 1, 2, 1) as CAMPAIGN_PAUSE,campaign.campaign_state,decode(campaign.SCHEDULE_TYPE, 0, 1, 1, 2, 1) as CAMPAIGN_SCHEDULE_TYPE,nvl(to_char(campaign.SCHEDULE_SERIALIZE), 'null') as CAMPAIGN_SCHEDULE_SERIALIZE,to_char(sysdate, 'DYHH24') as CAMPAIGN_SCHEDULE_TIME,round(sysdate - nvl(campaign.BEGIN_TIME, sysdate), 2) as CAMPAIGN_DATE_SUB_BEGIN_TIME,round(sysdate - nvl(campaign.END_TIME, sysdate), 2) as CAMPAIGN_DATE_SUB_END_TIME,decode(adgroup.targeting_type, 2, 1, 3, 2, 1) as targeting_type,nvl(to_char(group_targeting.code), 'null') as disease_code from t_adgroup adgroup left join t_campaign campaign on adgroup.CAMPAIGN_ID=campaign.campaign_id left join t_adgroup_targeting group_targeting on adgroup.adgroup_id=group_targeting.adgroup_id where adgroup.account_id=%s and adgroup.DISPLAY_STYLE=%u",
	    account_id, 
	    display_status);

    if (find_type & 2) {

	if (group_name_type == 1)
	    sql_size += sprintf(&sql_str[sql_size], " and adgroup.ADGROUP_NAME='%s'", group_name);

	else if (group_name_type == 2)
	    sql_size += sprintf(&sql_str[sql_size], " and adgroup.ADGROUP_NAME like '%%%s%%'", group_name);
    }

    if (find_type & 8) {

	sql_size += sprintf(&sql_str[sql_size], " and adgroup.CAMPAIGN_ID='%s'", campaign_id);
    }

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    int GROUP_PAUSE = 0;
    int CAMPAIGN_PAUSE = 0;
    int CAMPAIGN_STATE = 0;

    int CAMPAIGN_SCHEDULE_TYPE;
    char CAMPAIGN_SCHEDULE_SERIALIZE[1024] = {0};
    char CAMPAIGN_SCHEDULE_TIME[128] = {0};

    double CAMPAIGN_DATE_SUB_BEGIN_TIME = 0;
    double CAMPAIGN_DATE_SUB_END_TIME = 0;

    ad_system_oci_group_t temp;
    memset(&temp, 0, sizeof(ad_system_oci_group_t));
    if (SQLOCI_Bind_Ouput(&conn, "1%s2%s3%s4%s5%f6%f7%d8%d9%d10%d11%s12%s13%f14%f15%d16%s",
		temp.CAMPAIGN_ID,
		temp.CAMPAIGN_NAME,
		temp.GROUP_ID,
		temp.GROUP_NAME,
		&temp.GROUP_CPC_PRICE,
		&temp.GROUP_CPM_PRICE,
		&GROUP_PAUSE,
		&CAMPAIGN_PAUSE,
		&CAMPAIGN_STATE,
		&CAMPAIGN_SCHEDULE_TYPE,
		CAMPAIGN_SCHEDULE_SERIALIZE,
		CAMPAIGN_SCHEDULE_TIME,
		&CAMPAIGN_DATE_SUB_BEGIN_TIME,
		&CAMPAIGN_DATE_SUB_END_TIME,
		&temp.GROUP_TARGETING_TYPE,
		temp.GROUP_DISEASE) != 0) {


	fprintf(stdout, "SQLOCI_Bind_Ouput::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    int ret = SQLOCI_Execute(&conn);
    if (ret == 100) {

	SQLOCI_Close(&conn);
	return 100;

    } else if (ret != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    while(ret == 0) {

	if (GROUP_PAUSE == 2) {
	    temp.GROUP_STATE = 8;//暂停

	} else if (CAMPAIGN_PAUSE == 2) {
	    temp.GROUP_STATE = 5;//暂停

	} else if (CAMPAIGN_DATE_SUB_BEGIN_TIME < 0) {
	    temp.GROUP_STATE = 4;//未开始

	} else if (CAMPAIGN_STATE == 13) {
	    temp.GROUP_STATE = 3;//预算不足

	} else if (CAMPAIGN_SCHEDULE_TYPE == 2 
		&& ad_system_oci_get_campaign_schedule_check(CAMPAIGN_SCHEDULE_TIME, CAMPAIGN_SCHEDULE_SERIALIZE) == 0) {
	    temp.GROUP_STATE = 2;//暂停时段

	} else if (CAMPAIGN_DATE_SUB_BEGIN_TIME < 0) {
	    temp.GROUP_STATE = 6;//下线

#if 0
	    MYCODE
	} else if (temp.GROUP_CPC_PRICE <= 0.6) {
	    temp.GROUP_STATE = 7;//CPC出价低

	} else if (temp.GROUP_CPC_PRICE <= 0.6) {
	    temp.GROUP_STATE = 7;//CPC出价低
#endif

	} else {
	    temp.GROUP_STATE = 1;//有效
	}

	ad_system_oci_group_t *node = NULL;
	if ((node = malloc(sizeof(ad_system_oci_group_t)))) {

	    memcpy(node, &temp, sizeof(ad_system_oci_group_t));
	    lg_list_add(head, &node->list);
	}

	memset(&temp, 0, sizeof(ad_system_oci_group_t));
	ret = SQLOCI_Next_Data(&conn);
    }

    SQLOCI_Close(&conn);
    return 0;
}

void ad_system_oci_get_group_free(lg_list_t *head)
{

    lg_list_t *p = head->next;
    ad_system_oci_group_t *node = NULL;

    while(p != head) {

	node = (ad_system_oci_group_t *)p;
	p = p->next;
	free(node);
    }

    lg_list_init(head);
}

int ad_system_oci_get_creative(const char *account_id, int find_type,
	char *campaign_id, char *group_id, int creative_name_type, char *creative_name,	lg_list_t *head)
{

    lg_list_init(head);
    char sql_str[4096] = {0};

    size_t sql_size = sprintf(sql_str, "select "
	    "campaign.campaign_id,"
	    "campaign.campaign_name,"
	    "adgroup.adgroup_id as group_id,"
	    "adgroup.adgroup_name as group_name,"
	    "creative.creative_id,"
	    "creative.title as creative_name,"
	    "decode(creative.pause, 0, 1, 1, 2, 1) as creative_pause,"
	    "decode(creative.creative_state, 31, 1, 32, 2, 33, 3, 34, 4, 35, 5, 1) as creative_state,"
	    "nvl(to_char(creative.refuse_reason), 'null') as creative_refuse_reason,"
	    "nvl(to_char(creative.description1), 'null') as creative_description,"
	    "nvl(to_char(creative.destination_url), 'null') as creative_destination_url,"
	    "nvl(to_char(creative.display_url), 'null') as creative_display_url,"
	    "nvl(to_char(creative.append_id), 'null') as creative_append_id,"
	    "nvl(to_char(creative.MOBILE_DESTINATION_URL), 'null') as MOBILE_DESTINATION_URL,"
	    "nvl(to_char(creative.MOBILE_DISPLAY_URL), 'null') as MOBILE_DISPLAY_URL "
	    "from t_text_creative creative "
	    "left join t_adgroup adgroup on creative.adgroup_id = adgroup.adgroup_id "
	    "left join t_campaign campaign on creative.campaign_id = campaign.campaign_id "
	    "where creative.account_id = %s", account_id);

    if (find_type & 2) {

	if (creative_name_type == 1)
	    sql_size += sprintf(&sql_str[sql_size], " and creative.TITLE='%s'", creative_name);
	else if (creative_name_type == 2)
	    sql_size += sprintf(&sql_str[sql_size], " and creative.TITLE like '%s'", creative_name);
    }

    if (find_type & 8) {

	sql_size += sprintf(&sql_str[sql_size], " and creative.CAMPAIGN_ID='%s'", campaign_id);
    }

    if (find_type & 16) {

	sql_size += sprintf(&sql_str[sql_size], " and creative.ADGROUP_ID='%s'", group_id);
    }

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    int PAUSE = 0;
    ad_system_oci_creative_t temp;
    memset(&temp, 0, sizeof(ad_system_oci_creative_t));
    if (SQLOCI_Bind_Ouput(&conn, "%s%s1%s2%s3%s4%s5%d6%d7%s8%s9%s10%s11%s12%s13%s",
		temp.CAMPAIGN_ID,
		temp.CAMPAIGN_NAME,

		temp.GROUP_ID,
		temp.GROUP_NAME,

		temp.CREATIVE_ID,
		temp.CREATIVE_NAME,

		&PAUSE,
		&temp.CREATIVE_STATE,
		temp.CREATIVE_REFUSE_REASON,
		temp.CREATIVE_DESCRIPTION,
		temp.CREATIVE_DESTINATION_URL,
		temp.CREATIVE_DISPLAY_URL,
		temp.CREATIVE_APPEND_ID,
		temp.MOBILE_DESTINATION_URL,
		temp.MOBILE_DISPLAY_URL) != 0) {

	fprintf(stdout, "SQLOCI_Bind_Ouput::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    int ret = SQLOCI_Execute(&conn);
    if (ret == 100) {

	SQLOCI_Close(&conn);
	return 100;

    } else if (ret != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    while(ret == 0) {

	if (PAUSE == 2)
	    temp.CREATIVE_STATE = 3;//暂停

	if (strcmp(temp.CREATIVE_APPEND_ID, "null") == 0)
	    temp.CREATIVE_APPEND_TYPE = 1;
	else
	    temp.CREATIVE_APPEND_TYPE = 2;

	if (strcmp(temp.MOBILE_DESTINATION_URL, "null") == 0)
	    temp.MOBILE_DESTINATION_URL[0] = 0;

	if (strcmp(temp.MOBILE_DISPLAY_URL, "null") == 0)
	    temp.MOBILE_DISPLAY_URL[0] = 0;

	ad_system_oci_creative_t *node = NULL;
	if ((node = malloc(sizeof(ad_system_oci_creative_t)))) {

	    memcpy(node, &temp, sizeof(ad_system_oci_creative_t));
	    lg_list_add(head, &node->list);
	}

	memset(&temp, 0, sizeof(ad_system_oci_creative_t));
	ret = SQLOCI_Next_Data(&conn);
    }

    SQLOCI_Close(&conn);
    return 0;
}

void ad_system_oci_get_creative_free(lg_list_t *head)
{

    lg_list_t *p = head->next;
    ad_system_oci_creative_t *node = NULL;

    while(p != head) {

	node = (ad_system_oci_creative_t *)p;
	p = p->next;
	free(node);
    }

    lg_list_init(head);
}

int ad_system_oci_set_campaign(const char *account_id,
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
	char *campign_name)
{

    char comma[4] = " ";
    char sql_str[4096] = {0};
    size_t sql_size = sprintf(sql_str, "update T_CAMPAIGN set ");

    if (update_type & 1) {

	if (status == 1) {

	    sql_size += sprintf(&sql_str[sql_size], "PAUSE=0");
	    comma[0] = ',';

	} else if (status == 2) {

	    sql_size += sprintf(&sql_str[sql_size], "PAUSE=1");
	    comma[0] = ',';
	}
    }

    if (update_type & 2) {

	sql_size += sprintf(&sql_str[sql_size], "%s CAMPAIGN_BUDGET=%s, campaign_state = decode(sign(%s - campaign_budget), -1, campaign_state, decode(campaign_state, 13, 11, campaign_state)) ",
		comma, dudget, dudget);
	comma[0] = ',';
    }

    if (update_type & 4) {

	if (region_type == 1)
	    sql_size += sprintf(&sql_str[sql_size], "%sREGION_TYPE=0", comma);
	else if (region_type == 2)
	    sql_size += sprintf(&sql_str[sql_size], "%sREGION_TYPE=1,REGION_SERIALIZE='%s'", comma, region_serialize);

	comma[0] = ',';
    }

    if (update_type & 8) {

	if (schedule_type == 1)
	    sql_size += sprintf(&sql_str[sql_size], "%sSCHEDULE_TYPE=0", comma);
	else if (schedule_type == 2)
	    sql_size += sprintf(&sql_str[sql_size], "%sSCHEDULE_TYPE=1,SCHEDULE_SERIALIZE='%s'", comma, schedule_serialize);

	comma[0] = ',';
    }

    if (update_type & 16) {

	if (time_type & 1) {

	    sql_size += sprintf(&sql_str[sql_size], "%sEND_TIME=null", comma);
	    comma[0] = ',';

	} else if (time_type & 2) {

	    sql_size += sprintf(&sql_str[sql_size], "%sEND_TIME=to_date('%s 23:59:59', 'YYYY-MM-DD HH24:MI:SS')", comma, end_time);
	    comma[0] = ',';
	}

	if (time_type & 4) {

	    sql_size += sprintf(&sql_str[sql_size], "%sBEGIN_TIME=to_date('%s', 'yyyy-mm-dd')", comma, begin_time);
	    comma[0] = ',';
	}
    }

    if (update_type & 32) {

	sql_size += sprintf(&sql_str[sql_size], "%sCAMPAIGN_NAME='%s'", comma, campign_name);
	comma[0] = ',';
    }

    if (comma[0] == ',')
	sql_size += sprintf(&sql_str[sql_size], ",LAST_UPDATE_TIME=sysdate,SYNCH_STATE=0 where ACCOUNT_ID=%s and CAMPAIGN_ID='%s'",
		account_id, campaign_id);
    else
	return -1;

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Execute(&conn) != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Commit(&conn) != 0) {

	fprintf(stdout, "SQLOCI_Commit::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    SQLOCI_Close(&conn);
    return 0;
}

int ad_system_oci_add_campaign(const char *account_id,
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
	char *end_time)
{

    char begin_time_sql[256] = "null";
    sprintf(begin_time_sql, "to_date('%s', 'YYYY-MM-DD')", begin_time);

    char end_time_sql[256] = "null";
    if (time_type == 2)
	sprintf(end_time_sql, "to_date('%s 23:59:59', 'YYYY-MM-DD HH24:MI:SS')", end_time);

    char sql_str[4096] = {0};
    size_t sql_size = sprintf(sql_str, "INSERT INTO T_CAMPAIGN (CAMPAIGN_ID,ACCOUNT_ID,CAMPAIGN_NAME,CAMPAIGN_BUDGET,REGION_TYPE,REGION_SERIALIZE,BEGIN_TIME,END_TIME,SCHEDULE_TYPE,SCHEDULE_SERIALIZE,CAMPAIGN_STATE,PAUSE,CREATE_TIME,LAST_UPDATE_TIME,SYNCH_STATE,SYNCN_INITIALIZED,ROW_STATE,BIDDING_MODEL,DISPLAY_STYLE) VALUES ('%s', %s, '%s', %s, %u, '%s', %s, %s, %u, '%s', 11, 0, sysdate, sysdate, 0, 1, 1, 1,%u)",
	    campaign_id,
	    account_id,
	    campaign_name,
	    dudget,
	    region_type&1?0:1,
	    region_type&2?region_serialize:"null",
	    begin_time_sql,
	    end_time_sql,
	    schedule_type&1?0:1,
	    schedule_type&2?schedule_serialize:"null",
	    display_status);

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Execute(&conn) != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Commit(&conn) != 0) {

	fprintf(stdout, "SQLOCI_Commit::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    SQLOCI_Close(&conn);
    return 0;
}

int ad_system_oci_set_group(const char *account_id,
	char *group_id,
	int update_type,
	int status,
	char *cpc_price,
	char *cpm_price,
	int bidding_model,
	int targeting_type,
	char *group_name)
{

    char comma[4] = {0};
    char sql_str[4096] = {0};
    size_t sql_size = sprintf(sql_str, "update T_ADGROUP set ");

    if (update_type & 1) {

	if (status == 1) {

	    sql_size += sprintf(&sql_str[sql_size], "ADGROUP_STATE=21");
	    comma[0] = ',';

	} else if (status == 2) {

	    sql_size += sprintf(&sql_str[sql_size], "ADGROUP_STATE=22");
	    comma[0] = ',';
	}
    }

    if (update_type & 2) {

	sql_size += sprintf(&sql_str[sql_size], "%sCPC_PRICE=%s", comma, cpc_price);
	comma[0] = ',';
    }

    if (update_type & 4) {

	if (targeting_type == 1) {

	    sql_size += sprintf(&sql_str[sql_size], "%sTARGETING_TYPE=2", comma);
	    comma[0] = ',';

	} else if (targeting_type == 2) {

	    sql_size += sprintf(&sql_str[sql_size], "%sTARGETING_TYPE=3", comma);
	    comma[0] = ',';
	}
    }

    if (update_type & 8) {

	sql_size += sprintf(&sql_str[sql_size], "%sADGROUP_NAME='%s'", comma, group_name);
	comma[0] = ',';
    }

    if (update_type & 16) {

	sql_size += sprintf(&sql_str[sql_size], "%sCPM_PRICE=%s", comma, cpm_price);
	comma[0] = ',';
    }

    fprintf(stdout, "bidding_model=%d\n", bidding_model);

    if (update_type & 2 || update_type & 16) {

	if (update_type & 16) {

	    if (strcmp(cpm_price, "null") == 0)
		bidding_model &= ~1;
	    else
		bidding_model |= 1;
	}

	if (update_type & 2) {

	    if (strcmp(cpc_price, "null") == 0)
		bidding_model &= ~2;
	    else
		bidding_model |= 2;
	}

	sql_size += sprintf(&sql_str[sql_size], ",bidding_model=%u", bidding_model);

#if 0
	int GROUP_PRICE_TYPE = 0;
	if (update_type & 2)
	    GROUP_PRICE_TYPE = 1;

	if (update_type & 16)
	    GROUP_PRICE_TYPE += 2;

	if (GROUP_PRICE_TYPE == 1) {

	    if (strcmp(cpm_price, "null") == 0)
		sql_size += sprintf(&sql_str[sql_size], ",bidding_model = BITAND(bidding_model, (0 - 1) - 1)");
	    else
		sql_size += sprintf(&sql_str[sql_size], ",bidding_model = (bidding_model + 1) - BITAND(bidding_model, 1)");

	} else if (GROUP_PRICE_TYPE == 2) {

	    if (strcmp(cpc_price, "null") == 0) 
		sql_size += sprintf(&sql_str[sql_size], ",bidding_model = BITAND(bidding_model, (0 - 2) - 1)");
	    else
		sql_size += sprintf(&sql_str[sql_size], ",bidding_model = (bidding_model + 2) - BITAND(bidding_model, 2)");

	} else if (GROUP_PRICE_TYPE == 3) {

	    if (strcmp(cpm_price, "null") == 0) 
		sql_size += sprintf(&sql_str[sql_size], 
			",bidding_model = BITAND((bidding_model + 2) - BITAND(bidding_model, 2),(0 - 1) - 1)");
	    else if (strcmp(cpc_price, "null") == 0)
		sql_size += sprintf(&sql_str[sql_size], 
			",bidding_model = BITAND((bidding_model + 1) - BITAND(bidding_model, 1),(0 - 2) - 1)");
	    else
		sql_size += sprintf(&sql_str[sql_size], ",bidding_model = 3");
	}
#endif
    }

    if (comma[0] == ',')
	sql_size += sprintf(&sql_str[sql_size], ",LAST_UPDATE_TIME=sysdate,SYNCH_STATE=0 where ACCOUNT_ID=%s and ADGROUP_ID='%s'",
		account_id, group_id);
    else
	return -1;

    fprintf(stdout, "%s\n", sql_str);

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Execute(&conn) != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Commit(&conn) != 0) {

	fprintf(stdout, "SQLOCI_Commit::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    SQLOCI_Close(&conn);
    return 0;
}

int ad_system_oci_set_group_targeting(const char *account_id,
	char *group_id,
	int update_type,
	int targeting_type,
	char *disease,
	char *keyword)
{

    char comma[4] = " ";
    char sql_str[409600] = {0};
    size_t sql_size = sprintf(sql_str, "update T_ADGROUP_TARGETING set ");

    if (update_type & 4 && targeting_type == 1) {

	sql_size += sprintf(&sql_str[sql_size], "CODE='%s'", disease);
	comma[0] = ',';

    } else if (update_type & 4 && targeting_type == 2) {

	sql_size += sprintf(&sql_str[sql_size], "KEYWORD='%s'", keyword);
	comma[0] = ',';

    } else {

	return -1;
    }

    if (comma[0] == ',')
	sql_size += sprintf(&sql_str[sql_size], ",LAST_UPDATE_TIME=sysdate where ACCOUNT_ID=%s and ADGROUP_ID='%s'", 
		account_id, group_id);
    else
	return -1;

    fprintf(stdout, "%s\n", sql_str);

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Execute(&conn) != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Commit(&conn) != 0) {

	fprintf(stdout, "SQLOCI_Commit::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    SQLOCI_Close(&conn);
    return 0;
}

int ad_system_oci_get_group_id(char *group_id)
{

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    if (SQLOCI_Bind_Ouput(&conn, "%s", group_id) != 0) {

	fprintf(stdout, "SQLOCI_Bind_Ouput::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    char sql_str[1024] = {0};
    size_t sql_size = sprintf(sql_str, "select SEQ_T_ADGROUP.NEXTVAL from dual");
    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Execute(&conn) != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    SQLOCI_Close(&conn);
    return 0;

}

int ad_system_oci_add_group(const char *account_id,
	int display_status,
	char *campaign_id,
	char *group_id,
	char *group_name,
	int group_price_type,
	char *group_cpc_price,
	char *group_cpm_price,
	int targeting_type)
{

    if (targeting_type == 1)
	targeting_type = 2;
    else if (targeting_type == 2)
	targeting_type = 3;

    char sql_str[4096] = {0};
    size_t sql_size = sprintf(sql_str, "INSERT INTO T_ADGROUP(ADGROUP_ID,ACCOUNT_ID,CAMPAIGN_ID,ADGROUP_NAME,CPC_PRICE,CPM_PRICE,TARGETING_TYPE,ADGROUP_STATE,CREATE_TIME,LAST_UPDATE_TIME,SYNCH_STATE,SYNCN_INITIALIZED,ROW_STATE,BIDDING_MODEL,DISPLAY_STYLE) VALUES ('%s',%s,'%s','%s',%s,%s,%u,21,sysdate,sysdate,0,1,1,%u,%u)",
	    group_id,
	    account_id,
	    campaign_id,
	    group_name,
	    group_price_type&2?group_cpc_price:"null",
	    group_price_type&1?group_cpm_price:"null",
	    targeting_type,
	    group_price_type,
	    display_status);

    fprintf(stdout, "%s\n", sql_str);

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Execute(&conn) != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Commit(&conn) != 0) {

	fprintf(stdout, "SQLOCI_Commit::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    SQLOCI_Close(&conn);
    return 0;
}

int ad_system_oci_add_group_targeting(const char *account_id,
	char *campaign_id,
	char *group_id,
	int targeting_type,
	char *disease,
	char *keyword)
{

    char sql_str[409600] = {0};
    size_t sql_size = sprintf(sql_str, "INSERT INTO T_ADGROUP_TARGETING (ADGROUP_TARGETING_ID,ACCOUNT_ID,CAMPAIGN_ID,ADGROUP_ID,CREATE_TIME,LAST_UPDATE_TIME,CODE,KEYWORD) VALUES (SEQ_T_ADGROUP_TARGETING.nextval,%s,'%s','%s',sysdate,sysdate,%s%s%s,%s%s%s)",
	    account_id,
	    campaign_id,
	    group_id,
	    targeting_type == 1?"'":"",
	    targeting_type == 1?disease:"null",
	    targeting_type == 1?"'":"",
	    targeting_type == 2?"'":"",
	    targeting_type == 2?keyword:"null",
	    targeting_type == 2?"'":"");

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Execute(&conn) != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Commit(&conn) != 0) {

	fprintf(stdout, "SQLOCI_Commit::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    SQLOCI_Close(&conn);
    return 0;
}

int ad_system_oci_get_campaign_from_group(char *account_id, char *campaign_id, char *group_id)
{

    char sql_str[1024] = {0};
    size_t sql_size = sprintf(sql_str, "select nvl(campaign_id,'null') from t_adgroup where account_id=%s and adgroup_id='%s'",
	    account_id, group_id);

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    if (SQLOCI_Bind_Ouput(&conn, "%s", campaign_id) != 0) {

	fprintf(stdout, "SQLOCI_Bind_Ouput::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    int ret = SQLOCI_Execute(&conn);
    if (ret == 100) {

	SQLOCI_Close(&conn);
	return 100;

    } else if (ret != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    SQLOCI_Close(&conn);
    return 0;
}

int ad_system_oci_get_campaign_and_group_from_creative(char *account_id, char *campaign_id, char *group_id, char *creative_id)
{

    char sql_str[1024] = {0};
    size_t sql_size = sprintf(sql_str, 
	    "select nvl(campaign_id,'null'),nvl(adgroup_id,'null') from t_creative where account_id=%s and creative_id='%s'",
	    account_id, creative_id);
    fprintf(stdout, "%s\n",sql_str);

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    if (SQLOCI_Bind_Ouput(&conn, "%s%s", campaign_id, group_id) != 0) {

	fprintf(stdout, "SQLOCI_Bind_Ouput::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    int ret = SQLOCI_Execute(&conn);
    if (ret == 100) {

	SQLOCI_Close(&conn);
	return 100;

    } else if (ret != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    SQLOCI_Close(&conn);
    return 0;
}

int ad_system_oci_get_group_keyword(const char *account_id, char *group_id, char *keyword)
{

    char sql_str[4096] = {0};
    size_t sql_size = sprintf(sql_str, "select keyword from t_adgroup_targeting where account_id=%s and adgroup_id='%s'",
	    account_id, group_id);

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    if (SQLOCI_Bind_Ouput(&conn, "%s", keyword) != 0) {

	fprintf(stdout, "SQLOCI_Bind_Ouput::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Execute(&conn) != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    SQLOCI_Close(&conn);
    return 0;
}

int ad_system_oci_set_creative(const char *account_id,
	char *creative_id,
	int update_type,
	int status,
	char *creative_name,
	char *descriptionpriec,
	char *destination_url,
	char *creative_display_url,
	char *creative_append_id,
	char *mobile_destination_url,
	char *mobile_display_url)
{

    char comma[4] = " ";
    char sql_str[4096] = {0};
    size_t sql_size = sprintf(sql_str, "update T_TEXT_CREATIVE set ");

    if (update_type & 1) {

	if (status == 1) {

	    sql_size += sprintf(&sql_str[sql_size], "PAUSE=0");
	    comma[0] = ',';

	} else if (status == 2) {

	    sql_size += sprintf(&sql_str[sql_size], "PAUSE=1");
	    comma[0] = ',';
	}
    }

    int CREATIVE_STATE = 0;
    if (update_type & 2) {

	sql_size += sprintf(&sql_str[sql_size], "%sTITLE='%s'", comma, creative_name);
	comma[0] = ',';
	CREATIVE_STATE = 1;
    }

    if (update_type & 4) {

	sql_size += sprintf(&sql_str[sql_size], "%sDESCRIPTION1='%s'", comma, descriptionpriec);
	comma[0] = ',';
	CREATIVE_STATE = 1;
    }

    if (update_type & 8) {

	sql_size += sprintf(&sql_str[sql_size], "%sDESTINATION_URL='%s'", comma, destination_url);
	comma[0] = ',';
	CREATIVE_STATE = 1;
    }

    if (update_type & 16) {

	sql_size += sprintf(&sql_str[sql_size], "%sDISPLAY_URL='%s'", comma, creative_display_url);
	comma[0] = ',';
	CREATIVE_STATE = 1;
    }

    if (update_type & 32) {

	sql_size += sprintf(&sql_str[sql_size], "%sAPPEND_ID='%s'", comma, creative_append_id);
	comma[0] = ',';
	CREATIVE_STATE = 1;
    }

    if (update_type & 64) {

	sql_size += sprintf(&sql_str[sql_size], "%sMOBILE_DESTINATION_URL='%s'", comma, mobile_destination_url);
	comma[0] = ',';
	CREATIVE_STATE = 1;
    }

    if (update_type & 128) {

	sql_size += sprintf(&sql_str[sql_size], "%sMOBILE_DISPLAY_URL='%s'", comma, mobile_display_url);
	comma[0] = ',';
	CREATIVE_STATE = 1;
    }

    if (comma[0] == ',') {

	if (CREATIVE_STATE)
	    sql_size += sprintf(&sql_str[sql_size], ",CREATIVE_STATE=34,LAST_UPDATE_TIME=sysdate,SYNCH_STATE=0 where ACCOUNT_ID=%s and CREATIVE_ID='%s'", account_id, creative_id);
	else
	    sql_size += sprintf(&sql_str[sql_size], ",LAST_UPDATE_TIME=sysdate,SYNCH_STATE=0 where ACCOUNT_ID=%s and CREATIVE_ID='%s'", account_id, creative_id);

    } else
	return -1;

    fprintf(stdout, "%s\n", sql_str);

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Execute(&conn) != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Commit(&conn) != 0) {

	fprintf(stdout, "SQLOCI_Commit::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    SQLOCI_Close(&conn);
    return 0;
}

int ad_system_oci_add_creative(const char *account_id,
	char *campaign_id,
	char *group_id,
	char *creative_name,
	char *descriptionpriec,
	char *destination_url,
	char *creative_display_url,
	int creative_append_type,
	char *creative_append_id)
{
    char sql_str[4096] = {0};
    size_t sql_size = sprintf(sql_str, "INSERT INTO T_TEXT_CREATIVE (CREATIVE_ID,ACCOUNT_ID,CAMPAIGN_ID,ADGROUP_ID,TITLE,DESCRIPTION1,DESTINATION_URL,DISPLAY_URL,CREATIVE_STATE,PAUSE,CREATE_TIME,LAST_UPDATE_TIME,SYNCH_STATE,SYNCN_INITIALIZED,ROW_STATE,APPEND_ID) VALUES (SEQ_T_TEXT_CREATIVE.Nextval,%s,'%s','%s','%s','%s','%s','%s',34,0,sysdate,sysdate,0,0,1,%s%s%s)",
	    account_id,
	    campaign_id,
	    group_id,
	    creative_name,
	    descriptionpriec,
	    destination_url,
	    creative_display_url,
	    creative_append_type==2?"'":"",
	    creative_append_type==2?creative_append_id:"null",
	    creative_append_type==2?"'":"");

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Execute(&conn) != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Commit(&conn) != 0) {

	fprintf(stdout, "SQLOCI_Commit::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    SQLOCI_Close(&conn);
    return 0;
}

int ad_system_oci_get_creative_append(const char *account_id, int find_type,
	int sort_type,
	int sort_column,
	char *find_name, 
	int status, 
	lg_list_t *head)
{

    lg_list_init(head);
    char sql_str[4096] = {0};
    size_t sql_size = sprintf(sql_str, "select append_id,decode(pause,1,3,decode(creative_state, 31, 1, 32, 2, 33, 3, 34, 4, 35, 5, 1)) as creative_append_status,nvl(content, 'null'),nvl(refuse_reason, 'null') from t_text_creative_append where account_id=%s", account_id);

    if (find_type & 2)
	sql_size += sprintf(&sql_str[sql_size], " and content like '%%%s%%'", find_name);

    if (find_type & 4) {

	if (status != 100) {

	    if (status == 1) {
		sql_size += sprintf(&sql_str[sql_size], " and CREATIVE_STATE=31 and PAUSE=0");

	    } else if (status == 2) {
		sql_size += sprintf(&sql_str[sql_size], " and CREATIVE_STATE=32 and PAUSE=0");

	    } else if (status == 3) {
		sql_size += sprintf(&sql_str[sql_size], " and PAUSE=1");

	    } else if (status == 4) {
		sql_size += sprintf(&sql_str[sql_size], " and CREATIVE_STATE=34 and PAUSE=0");

	    } else if (status == 5) {
		sql_size += sprintf(&sql_str[sql_size], " and CREATIVE_STATE=35 and PAUSE=0");

	    } else {

		return -1;
	    }
	}
    }

    if (sort_column == 1) {

	if (sort_type == 1) {
	    sql_size += sprintf(&sql_str[sql_size], " order by content");

	} else if (sort_type == 2) {
	    sql_size += sprintf(&sql_str[sql_size], " order by content desc");
	}

    } else if (sort_column == 2) {

	if (sort_type == 1) {
	    sql_size += sprintf(&sql_str[sql_size], " order by creative_append_status");

	} else if (sort_type == 2) {
	    sql_size += sprintf(&sql_str[sql_size], " order by creative_append_status desc");
	}
    }

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    ad_system_oci_creative_append_t temp;
    memset(&temp, 0, sizeof(ad_system_oci_creative_append_t));
    if (SQLOCI_Bind_Ouput(&conn, "1%s2%d3%s4%s",
		temp.APPEND_ID,
		&temp.STATUS,
		temp.CONTENT,
		temp.REFUSE_REASON) != 0) {

	fprintf(stdout, "SQLOCI_Bind_Ouput::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    int ret = SQLOCI_Execute(&conn);
    if (ret == 100) {

	SQLOCI_Close(&conn);
	return 100;

    } else if (ret != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    while (ret == 0) {

	//fprintf(stdout, "%s:%u\n",temp.APPEND_ID, temp.STATUS);

	ad_system_oci_creative_append_t *node = NULL;
	if ((node = malloc(sizeof(ad_system_oci_creative_append_t)))) {

	    memcpy(node, &temp, sizeof(ad_system_oci_creative_append_t));
	    lg_list_add(head, &node->list);
	}

	ret = SQLOCI_Next_Data(&conn);
    }

    SQLOCI_Close(&conn);
    return 0;
}

void ad_system_oci_get_creative_append_free(lg_list_t *head)
{

    lg_list_t *p = head->next;
    ad_system_oci_creative_append_t *node = NULL;

    while(p != head) {

	node = (ad_system_oci_creative_append_t *)p;
	p = p->next;

	//fprintf(stdout, "%s:%u\n",node->APPEND_ID,node->STATUS);
	free(node);
    }

    lg_list_init(head);
}

int ad_system_oci_set_creative_append(const char *account_id,
	char *creative_append_id,
	int update_type,
	int status,
	char *contend)
{

    char comma[4] = " ";
    char sql_str[4096] = {0};
    size_t sql_size = sprintf(sql_str, "update T_TEXT_CREATIVE_APPEND set ");

    if (update_type & 1) {

	if (status == 1) {

	    sql_size += sprintf(&sql_str[sql_size], "PAUSE=0");
	    comma[0] = ',';

	} else if (status == 2) {

	    sql_size += sprintf(&sql_str[sql_size], "PAUSE=1");
	    comma[0] = ',';
	}
    }

    if (update_type & 2) {

	sql_size += sprintf(&sql_str[sql_size], "%sCONTENT='%s',CREATIVE_STATE=34", comma, contend);
	comma[0] = ',';
    }

    if (comma[0] == ',')
	sql_size += sprintf(&sql_str[sql_size], ",LAST_UPDATE_TIME=sysdate,SYNCH_STATE=0 where ACCOUNT_ID=%s and APPEND_ID='%s'", 
		account_id, creative_append_id);
    else
	return -1;

    fprintf(stdout, "%s\n", sql_str);

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Execute(&conn) != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Commit(&conn) != 0) {

	fprintf(stdout, "SQLOCI_Commit::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    SQLOCI_Close(&conn);
    return 0;
}

int ad_system_oci_add_creative_append(const char *account_id, char *append_contend)
{

    char sql_str[4096] = {0};
    size_t sql_size = sprintf(sql_str,"INSERT INTO T_TEXT_CREATIVE_APPEND (APPEND_ID,ACCOUNT_ID,CONTENT,CREATIVE_STATE,PAUSE,CREATE_TIME,LAST_UPDATE_TIME,SYNCH_STATE,SYNCN_INITIALIZED,ROW_STATE) VALUES (seq_t_text_creative_append.nextval,%s,'%s', 34, 0, sysdate, sysdate, 0, 0, 1)", account_id, append_contend);

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Execute(&conn) != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Commit(&conn) != 0) {

	fprintf(stdout, "SQLOCI_Commit::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    SQLOCI_Close(&conn);
    return 0;
}

int ad_system_oci_del_creative_append(char *account_id, char *append_id)
{

    char sql_str[4096] = {0};
    size_t sql_size = sprintf(sql_str, "delete t_text_creative_append t where t.account_id=%s and t.append_id='%s'",
	    account_id, append_id);

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Execute(&conn) != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Commit(&conn) != 0) {

	fprintf(stdout, "SQLOCI_Commit::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    SQLOCI_Close(&conn);
    return 0;
}

int ad_system_oci_get_announcement_count(int find_type, char *find_name, char *count)
{

    size_t sql_size = 0;
    char sql_str[4096] = {0};

    if (find_type == 1)
	sql_size = sprintf(sql_str, "select count(*) from t_announcement");
    else if (find_type == 2)
	sql_size = sprintf(sql_str, "select count(*) from t_announcement where announcement_title like '%%%s%%'", find_name);

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    ad_system_oci_announcement_t temp;
    memset(&temp, 0, sizeof(ad_system_oci_announcement_t));
    if (SQLOCI_Bind_Ouput(&conn, "%s", count) != 0) {

	fprintf(stdout, "SQLOCI_Bind_Ouput::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    int ret = SQLOCI_Execute(&conn);
    if (ret == 100) {

	SQLOCI_Close(&conn);
	return 100;

    } else if (ret != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    SQLOCI_Close(&conn);
    return 0;
}

int ad_system_oci_get_announcement(int find_type, char *find_name, int page_index_s, int page_index_e, lg_list_t *head)
{

    lg_list_init(head);

    size_t where_size = 0;
    char where_str[1024] = "";
    if (find_type == 2)
	where_size = sprintf(where_str, " and announcement_title like '%%%s%%' ", find_name);

    size_t sql_size = 0;
    char sql_str[4096] = "";
    sql_size = sprintf(sql_str, "select to_char(send_time, 'yyyy-mm-dd HH24:MI:SS'),announcement_title, content from (select rownum as page_index, send_time, announcement_title, content from (select send_time, announcement_title, content from t_announcement where send_time <= sysdate %s order by send_time desc)) where page_index between %u and %u", where_str, page_index_s, page_index_e);

    fprintf(stdout, "%s\n", sql_str);

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    ad_system_oci_announcement_t temp;
    memset(&temp, 0, sizeof(ad_system_oci_announcement_t));
    if (SQLOCI_Bind_Ouput(&conn, "1%s2%s3%s",
		temp.DATE,
		temp.TITLE,
		temp.CONTENT) != 0) {

	fprintf(stdout, "SQLOCI_Bind_Ouput::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    int ret = SQLOCI_Execute(&conn);
    if (ret == 100) {

	SQLOCI_Close(&conn);
	return 100;

    } else if (ret != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    while(ret == 0) {

	ad_system_oci_announcement_t *node = NULL;

	if ((node = malloc(sizeof(ad_system_oci_announcement_t)))){

	    memcpy(node, &temp, sizeof(ad_system_oci_announcement_t)); 
	    lg_list_add(head, &node->list);
	}

	memset(&temp, 0, sizeof(ad_system_oci_announcement_t));
	ret = SQLOCI_Next_Data(&conn);
    }

    SQLOCI_Close(&conn);
    return 0;
}

void ad_system_oci_get_announcement_free(lg_list_t *head)
{

    lg_list_t *p = head->next;
    ad_system_oci_announcement_t *node = NULL;

    while(p != head) {

	node = (ad_system_oci_announcement_t *)p;
	p = p->next;

	//fprintf(stdout, "[%s][%s][%s]\n", node->DATE, node->TITLE, node->CONTENT);
	free(node);
    }

    lg_list_init(head);
}

int ad_system_oci_get_campaign_unique(const char *account_id, char *campaign_id, ad_system_oci_campaign_t *temp)
{

    char sql_str[4096] = {0};
    size_t sql_size = sprintf(sql_str, "select CAMPAIGN_ID,nvl(to_char(CAMPAIGN_NAME), 'null') as CAMPAIGN_NAME,CAMPAIGN_STATE,decode(PAUSE, 0, 1, 1, 2, 1) as PAUSE,CAMPAIGN_BUDGET,decode(SCHEDULE_TYPE, 0, 1, 1, 2, 1) as SCHEDULE_TYPE,nvl(to_char(SCHEDULE_SERIALIZE), 'null') as SCHEDULE_SERIALIZE,to_char(sysdate, 'DYHH24') as SCHEDULE_TIME,decode(REGION_TYPE, 0, 1, 1, 2, 1) as REGION_TYPE,nvl(to_char(REGION_SERIALIZE), 'null') as REGION_SERIALIZE,nvl(to_char(BEGIN_TIME, 'yyyy-mm-dd'), 'null') as BEGIN_TIME,nvl(to_char(END_TIME, 'yyyy-mm-dd'), 'null') as END_TIME,round(sysdate - nvl(BEGIN_TIME, sysdate), 2) as DATE_SUB_BEGIN_TIME,round(sysdate - nvl(END_TIME, sysdate), 2) as DATE_SUB_END_TIME from T_CAMPAIGN where ACCOUNT_ID=%s and CAMPAIGN_ID='%s'", account_id, campaign_id);

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    int CAMPAIGN_STATE = 0;
    int PAUSE = 0;
    char SCHEDULE_TIME[128] = {0};
    double DATE_SUB_BEGIN_TIME = 0;
    double DATE_SUB_END_TIME = 0;

    memset(temp, 0, sizeof(ad_system_oci_campaign_t));
    if (SQLOCI_Bind_Ouput(&conn, "1%s2%s3%d4%d5%f6%d7%s8%s9%d10%s11%s12%s13%f14%f",
		temp->CAMPAIGN_ID,
		temp->CAMPAIGN_NAME,
		&CAMPAIGN_STATE,
		&PAUSE,
		&temp->CAMPAIGN_BUDGET,
		&temp->SCHEDULE_TYPE,
		temp->SCHEDULE_SERIALIZE,
		SCHEDULE_TIME,
		&temp->REGION_TYPE,
		temp->REGION_SERIALIZE,
		temp->BEGIN_TIME,
		temp->END_TIME,
		&DATE_SUB_BEGIN_TIME,
		&DATE_SUB_END_TIME) != 0) {

	fprintf(stdout, "SQLOCI_Bind_Ouput::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    int ret = SQLOCI_Execute(&conn);
    if (ret == 100) {

	SQLOCI_Close(&conn);
	return 100;

    } else if (ret != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (PAUSE == 2) {
	temp->CAMPAIGN_STATE = 5;//暂停

    } else if (DATE_SUB_BEGIN_TIME < 0) {
	temp->CAMPAIGN_STATE = 4;//未开始

    } else if (CAMPAIGN_STATE == 13) {
	temp->CAMPAIGN_STATE = 3;//预算不足

    } else if (temp->SCHEDULE_TYPE == 2 && ad_system_oci_get_campaign_schedule_check(SCHEDULE_TIME, temp->SCHEDULE_SERIALIZE) == 0) {
	temp->CAMPAIGN_STATE = 2;//暂停时段

    } else if (DATE_SUB_BEGIN_TIME < 0) {
	temp->CAMPAIGN_STATE = 6;//下线

    } else {
	temp->CAMPAIGN_STATE = 1;//有效
    }

    if (strcmp(temp->END_TIME, "null") == 0)
	temp->TIME_TYPE = 1;
    else
	temp->TIME_TYPE = 2;

    SQLOCI_Close(&conn);
    return 0;
}

int ad_system_oci_get_group_unique(const char *account_id, char *group_id, ad_system_oci_group_t *temp)
{

    char sql_str[4096] = {0};
    size_t sql_size = sprintf(sql_str, "select "
	    "campaign.campaign_id,"
	    "campaign.campaign_name,"
	    "adgroup.adgroup_id,"
	    "adgroup.adgroup_name,"
	    "nvl(adgroup.bidding_model, 0),"
	    "nvl(adgroup.cpc_price,0),"
	    "nvl(adgroup.cpm_price,0),"
	    "decode(adgroup.adgroup_state, 21, 1, 22, 2, 1) as group_state,"
	    "decode(campaign.pause, 0, 1, 1, 2, 1) as CAMPAIGN_PAUSE,"
	    "campaign.campaign_state,"
	    "decode(campaign.SCHEDULE_TYPE, 0, 1, 1, 2, 1) as CAMPAIGN_SCHEDULE_TYPE,"
	    "nvl(to_char(campaign.SCHEDULE_SERIALIZE), 'null') as CAMPAIGN_SCHEDULE_SERIALIZE,"
	    "to_char(sysdate, 'DYHH24') as CAMPAIGN_SCHEDULE_TIME,"
	    "round(sysdate - nvl(campaign.BEGIN_TIME, sysdate), 2) as CAMPAIGN_DATE_SUB_BEGIN_TIME,"
	    "round(sysdate - nvl(campaign.END_TIME, sysdate), 2) as CAMPAIGN_DATE_SUB_END_TIME,"
	    "decode(adgroup.targeting_type, 2, 1, 3, 2, 1) as targeting_type,"
	    "nvl(to_char(group_targeting.code), 'null') as disease_code "
	    "from t_adgroup adgroup "
	    "left join t_campaign campaign on adgroup.CAMPAIGN_ID=campaign.campaign_id "
	    "left join t_adgroup_targeting group_targeting on adgroup.adgroup_id=group_targeting.adgroup_id "
	    "where adgroup.account_id=%s and adgroup.adgroup_id='%s'", account_id, group_id);

    fprintf(stdout, "%s\n", sql_str);

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    int GROUP_PAUSE = 0;
    int CAMPAIGN_PAUSE = 0;
    int CAMPAIGN_STATE = 0;

    int CAMPAIGN_SCHEDULE_TYPE;
    char CAMPAIGN_SCHEDULE_SERIALIZE[1024] = {0};
    char CAMPAIGN_SCHEDULE_TIME[128] = {0};

    double CAMPAIGN_DATE_SUB_BEGIN_TIME = 0;
    double CAMPAIGN_DATE_SUB_END_TIME = 0;

    memset(temp, 0, sizeof(ad_system_oci_group_t));
    if (SQLOCI_Bind_Ouput(&conn, "1%s2%s3%s4%sBIDDING_MODEL%d5%f6%f7%d8%d9%d10%d11%s12%s13%f14%f15%d16%s",
		temp->CAMPAIGN_ID,
		temp->CAMPAIGN_NAME,
		temp->GROUP_ID,
		temp->GROUP_NAME,
		&temp->BIDDING_MODEL,
		&temp->GROUP_CPC_PRICE,
		&temp->GROUP_CPM_PRICE,
		&GROUP_PAUSE,
		&CAMPAIGN_PAUSE,
		&CAMPAIGN_STATE,
		&CAMPAIGN_SCHEDULE_TYPE,
		CAMPAIGN_SCHEDULE_SERIALIZE,
		CAMPAIGN_SCHEDULE_TIME,
		&CAMPAIGN_DATE_SUB_BEGIN_TIME,
		&CAMPAIGN_DATE_SUB_END_TIME,
		&temp->GROUP_TARGETING_TYPE,
		temp->GROUP_DISEASE) != 0) {

	fprintf(stdout, "SQLOCI_Bind_Ouput::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    int ret = SQLOCI_Execute(&conn);
    if (ret == 100) {

	SQLOCI_Close(&conn);
	return 100;

    } else if (ret != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (GROUP_PAUSE == 2) {
	temp->GROUP_STATE = 8;//暂停

    } else if (CAMPAIGN_PAUSE == 2) {
	temp->GROUP_STATE = 5;//暂停

    } else if (CAMPAIGN_DATE_SUB_BEGIN_TIME < 0) {
	temp->GROUP_STATE = 4;//未开始

    } else if (CAMPAIGN_STATE == 13) {
	temp->GROUP_STATE = 3;//预算不足

    } else if (CAMPAIGN_SCHEDULE_TYPE == 2 
	    && ad_system_oci_get_campaign_schedule_check(CAMPAIGN_SCHEDULE_TIME, CAMPAIGN_SCHEDULE_SERIALIZE) == 0) {
	temp->GROUP_STATE = 2;//暂停时段

    } else if (CAMPAIGN_DATE_SUB_BEGIN_TIME < 0) {
	temp->GROUP_STATE = 6;//下线

#if 0
	MYCODE
    } else if (temp->GROUP_PRICE <= 0.6) {
	temp->GROUP_STATE = 7;//出价低
#endif

    } else {
	temp->GROUP_STATE = 1;//有效
    }

    SQLOCI_Close(&conn);
    return 0;
}

int ad_system_oci_get_img_creative_unique(char *account_id, char *creative_id, ad_system_oci_img_creative_t *temp)
{

    char sql_str[4096] = {0};
    size_t sql_size = sprintf(sql_str, "select t.campaign_id,t.adgroup_id,t.creative_id,t.creative_name,t.pause,t.creative_state,nvl(t.refuse_reason,'null'),t.img_size,t.img_size_imglength,t.file_path,t.destination_url,t.display_url from t_creative t where t.account_id=%s and t.creative_id='%s'", account_id, creative_id);

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    memset(temp, 0, sizeof(ad_system_oci_img_creative_t));
    if (SQLOCI_Bind_Ouput(&conn, "1%s2%s3%s4%s5%d6%d7%s8%s9%s10%s11%s12%s",
		temp->CAMPAIGN_ID,
		temp->GROUP_ID,
		temp->CREATIVE_ID,
		temp->CREATIVE_NAME,
		&temp->PAUSE,	
		&temp->CREATIVE_STATE,
		temp->CREATIVE_REFUSE_REASON,
		temp->CREATIVE_IMG_SIZE,
		temp->CREATIVE_IMG_SIZE_IMGLENGTH,
		temp->CREATIVE_FILE_PATH,
		temp->CREATIVE_DESTINATION_URL,
		temp->CREATIVE_DISPLAY_URL) != 0) {

	fprintf(stdout, "SQLOCI_Bind_Ouput::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    int ret = SQLOCI_Execute(&conn);
    if (ret == 100) {

	SQLOCI_Close(&conn);
	return 100;

    } else if (ret != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    SQLOCI_Close(&conn);
    if (!(temp->PAUSE == 0 || temp->PAUSE == 1))
	return -1;

    return 0;
}

int ad_system_oci_get_creative_append_unique(const char *account_id, char *creative_append_id, ad_system_oci_creative_append_t *temp)
{

    char sql_str[4096] = {0};
    size_t sql_size = sprintf(sql_str, "select append_id,decode(pause,1,3,decode(creative_state, 31, 1, 32, 2, 33, 3, 34, 4, 35, 5, 1)) as creative_append_status,nvl(content, 'null'),nvl(refuse_reason, 'null') from t_text_creative_append where account_id=%s and append_id='%s'",
	    account_id, creative_append_id);

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    memset(temp, 0, sizeof(ad_system_oci_creative_append_t));
    if (SQLOCI_Bind_Ouput(&conn, "1%s2%d3%s4%s",
		temp->APPEND_ID,
		&temp->STATUS,
		temp->CONTENT,
		temp->REFUSE_REASON) != 0) {

	fprintf(stdout, "SQLOCI_Bind_Ouput::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    int ret = SQLOCI_Execute(&conn);
    if (ret == 100) {

	SQLOCI_Close(&conn);
	return 100;

    } else if (ret != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    SQLOCI_Close(&conn);
    return 0;
}

int ad_system_oci_get_img_creative(const char *account_id, int find_type,
	char *campaign_id, char *group_id, int creative_name_type, char *creative_name,	lg_list_t *head)
{

    lg_list_init(head);
    char sql_str[4096] = {0};
    size_t sql_size = sprintf(sql_str, "select campaign.campaign_id,campaign.campaign_name,adgroup.adgroup_id,adgroup.adgroup_name,creative.creative_id,creative.creative_name,decode(creative.pause,0,1,1,2,1) as creative_pause,decode(creative.creative_state,31,1,32,2,33,3,34,4,35,5,1) as creative_state,nvl(to_char(creative.refuse_reason),'null') as creative_refuse_reason,nvl(creative.img_size,'null') as creative_img_size,nvl(creative.img_size_imglength,0) as creative_img_size_imglength,nvl(creative.file_path,'null') as creative_file_path,nvl(to_char(creative.destination_url),'null') as creative_destination_url,nvl(to_char(creative.display_url),'null') as creative_display_url from t_creative creative left join t_adgroup adgroup on creative.adgroup_id=adgroup.adgroup_id left join t_campaign campaign on creative.campaign_id= campaign.campaign_id where creative.account_id=%s", account_id);

    if (find_type & 2) {

	if (creative_name_type == 1)
	    sql_size += sprintf(&sql_str[sql_size], " and creative.TITLE='%s'", creative_name);
	else if (creative_name_type == 2)
	    sql_size += sprintf(&sql_str[sql_size], " and creative.TITLE like '%s'", creative_name);
    }

    if (find_type & 8) {

	sql_size += sprintf(&sql_str[sql_size], " and creative.CAMPAIGN_ID='%s'", campaign_id);
    }

    if (find_type & 16) {

	sql_size += sprintf(&sql_str[sql_size], " and creative.ADGROUP_ID='%s'", group_id);
    }

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    int PAUSE = 0;
    ad_system_oci_img_creative_t temp;
    memset(&temp, 0, sizeof(ad_system_oci_img_creative_t));
    if (SQLOCI_Bind_Ouput(&conn, "1%s2%s3%s4%s5%s6%s7%d8%d9%s10%s11%s12%s13%s14%s",
		temp.CAMPAIGN_ID,
		temp.CAMPAIGN_NAME,
		temp.GROUP_ID,
		temp.GROUP_NAME,
		temp.CREATIVE_ID,
		temp.CREATIVE_NAME,
		&PAUSE,
		&temp.CREATIVE_STATE,
		temp.CREATIVE_REFUSE_REASON,
		temp.CREATIVE_IMG_SIZE,
		temp.CREATIVE_IMG_SIZE_IMGLENGTH,
		temp.CREATIVE_FILE_PATH,
		temp.CREATIVE_DESTINATION_URL,
		temp.CREATIVE_DISPLAY_URL) != 0) {

	fprintf(stdout, "SQLOCI_Bind_Ouput::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    int ret = SQLOCI_Execute(&conn);
    if (ret == 100) {

	SQLOCI_Close(&conn);
	return 100;

    } else if (ret != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    while(ret == 0) {

	if (PAUSE == 2)
	    temp.CREATIVE_STATE = 3;//暂停

	ad_system_oci_img_creative_t *node = NULL;
	if ((node = malloc(sizeof(ad_system_oci_img_creative_t)))) {

	    memcpy(node, &temp, sizeof(ad_system_oci_img_creative_t));
	    lg_list_add(head, &node->list);
	}

	memset(&temp, 0, sizeof(ad_system_oci_img_creative_t));
	ret = SQLOCI_Next_Data(&conn);
    }

    SQLOCI_Close(&conn);
    return 0;
}

void ad_system_oci_get_img_creative_free(lg_list_t *head)
{

    lg_list_t *p = head->next;
    ad_system_oci_img_creative_t *node = NULL;

    while(p != head) {

	node = (ad_system_oci_img_creative_t *)p;
	p = p->next;
	free(node);
    }

    lg_list_init(head);
}

int ad_system_oci_add_img_creative(const char *account_id,
	char *campaign_id,
	char *group_id,
	char *creative_name,
	char *creative_img_size,
	int creative_img_size_imglength,
	char *creative_file_path,
	char *creative_destination_url,
	char *creative_display_url)
{

    char sql_str[4096] = {0};
    size_t sql_size = sprintf(sql_str, "INSERT INTO T_CREATIVE(CREATIVE_ID,ACCOUNT_ID,CAMPAIGN_ID,ADGROUP_ID,CREATIVE_NAME,IMG_SIZE,IMG_SIZE_IMGLENGTH,FILE_PATH,DESTINATION_URL,DISPLAY_URL,CREATIVE_STATE,PAUSE,CREATE_TIME,LAST_UPDATE_TIME,SYNCH_STATE,SYNCN_INITIALIZED,ROW_STATE) VALUES (SEQ_T_CREATIVE.nextval, %s, '%s', '%s', '%s', '%s', %u, '%s', '%s', '%s', 34, 0, sysdate, sysdate, 0, 0, 1)",
	    account_id,
	    campaign_id,
	    group_id,
	    creative_name,
	    creative_img_size,
	    creative_img_size_imglength,
	    creative_file_path,
	    creative_destination_url,
	    creative_display_url);

    fprintf(stdout, "%s\n", sql_str);

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Execute(&conn) != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Commit(&conn) != 0) {

	fprintf(stdout, "SQLOCI_Commit::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    SQLOCI_Close(&conn);
    return 0;
}

int ad_system_oci_set_img_creative(char *account_id,
	char *creative_id,
	int update_type,
	int creative_status,
	char *creative_name,
	char *creative_destination_url,
	char *creative_display_url)
{

    char comma[4] = "";
    char sql_str[4096] = {0};
    size_t sql_size = sprintf(sql_str, "update T_CREATIVE set ");

    if (update_type & 1) {

	if (creative_status == 1) {

	    sql_size += sprintf(&sql_str[sql_size], "PAUSE=0");
	    comma[0] = ',';

	} else if (creative_status == 2) {

	    sql_size += sprintf(&sql_str[sql_size], "PAUSE=1");
	    comma[0] = ',';
	}
    }

    if (update_type & 2) {

	sql_size += sprintf(&sql_str[sql_size], "%sCREATIVE_NAME='%s'", comma, creative_name);
	comma[0] = ',';
    }

    int CREATIVE_STATE = 0;

    if (update_type & 4) {

	sql_size += sprintf(&sql_str[sql_size], "%sDESTINATION_URL='%s'", comma, creative_destination_url);
	comma[0] = ',';
	CREATIVE_STATE = 1;
    }

    if (update_type & 8) {

	sql_size += sprintf(&sql_str[sql_size], "%sDISPLAY_URL='%s'", comma, creative_display_url);
	comma[0] = ',';
	CREATIVE_STATE = 1;
    }

    if (comma[0] == ',') {
	if (CREATIVE_STATE)
	    sql_size += sprintf(&sql_str[sql_size], ",CREATIVE_STATE=34,LAST_UPDATE_TIME=sysdate,SYNCH_STATE=0 where ACCOUNT_ID=%s and CREATIVE_ID='%s'", 
		    account_id, creative_id);
	else
	    sql_size += sprintf(&sql_str[sql_size], ",LAST_UPDATE_TIME=sysdate,SYNCH_STATE=0 where ACCOUNT_ID=%s and CREATIVE_ID='%s'", 
		    account_id, creative_id);
    }else
	return -1;

    fprintf(stdout, "%s\n",sql_str);

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Execute(&conn) != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Commit(&conn) != 0) {

	fprintf(stdout, "SQLOCI_Commit::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    SQLOCI_Close(&conn);
    return 0;

}

int ad_system_oci_add_upload_img_info(
	char *img_id,
	char *img_name,
	char *img_size,
	int img_size_imglength,
	char *img_file_path,
	char *group_id)
{

    char sql_str[4096] = {0};
    size_t sql_size = sprintf(sql_str, "INSERT INTO T_CREATIVE_IMG_INFO(IMG_ID,ACCOUNT_ID,CAMPAIGN_ID,ADGROUP_ID,IMG_NAME,IMG_SIZE,IMG_SIZE_IMGLENGTH,IMG_FILE_PATH) VALUES ('%s',null,null,'%s','%s','%s',%u,'%s')",
	    img_id,
	    group_id,
	    img_name,
	    img_size,
	    img_size_imglength,
	    img_file_path);

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Execute(&conn) != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Commit(&conn) != 0) {

	fprintf(stdout, "SQLOCI_Commit::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    SQLOCI_Close(&conn);
    return 0;
}

int ad_system_oci_del_campaign_move(SQLOCI_CONN_T *conn, char *account_id, char *campaign_id)
{

    char sql_str[4096] = {0};
    size_t sql_size = sprintf(sql_str, "insert into t_deleted_campaign t_del (t_del.CAMPAIGN_ID,t_del.ACCOUNT_ID,t_del.CAMPAIGN_NAME,t_del.CAMPAIGN_BUDGET,t_del.REGION_TYPE,t_del.REGION_SERIALIZE,t_del.BEGIN_TIME,t_del.END_TIME,t_del.SCHEDULE_TYPE,t_del.SCHEDULE_SERIALIZE,t_del.MAXIMP,t_del.CAMPAIGN_STATE,t_del.PAUSE,t_del.IMPRESSION,t_del.CLICK,t_del.COST,t_del.DIALOG,t_del.APPOINTMENT,t_del.ARRIVAL,t_del.CREATE_TIME,t_del.LAST_UPDATE_TIME,t_del.DELETED_TIME,t_del.SYNCH_STATE,t_del.BIDDING_MODEL) select t.CAMPAIGN_ID,t.ACCOUNT_ID,t.CAMPAIGN_NAME,t.CAMPAIGN_BUDGET,t.REGION_TYPE,t.REGION_SERIALIZE,t.BEGIN_TIME,t.END_TIME,t.SCHEDULE_TYPE,t.SCHEDULE_SERIALIZE,t.MAXIMP,t.CAMPAIGN_STATE,t.PAUSE,t.IMPRESSION,t.CLICK,t.COST,t.DIALOG,t.APPOINTMENT,t.ARRIVAL,t.CREATE_TIME,t.LAST_UPDATE_TIME,sysdate,0,t.BIDDING_MODEL from t_campaign t where t.ACCOUNT_ID=%s and t.CAMPAIGN_ID='%s'", account_id, campaign_id);

    if (SQLOCI_Prepare(conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(conn));
	return -1;
    }

    if (SQLOCI_Execute(conn) != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(conn));
	return -1;
    }

    return 0;
}

int ad_system_oci_del_group_move(SQLOCI_CONN_T *conn, char *account_id, char *campaign_id, char *group_id)
{

    char sql_str[4096] = {0};
    size_t sql_size = sprintf(sql_str, "insert into t_deleted_adgroup t_del (t_del.ADGROUP_ID,t_del.ACCOUNT_ID,t_del.CAMPAIGN_ID,t_del.ADGROUP_NAME,t_del.CPC_PRICE,t_del.CPM_PRICE,t_del.TARGETING_TYPE,t_del.ADGROUP_STATE,t_del.IMPRESSION,t_del.CLICK,t_del.COST,t_del.DIALOG,t_del.APPOINTMENT,t_del.ARRIVAL,t_del.CREATE_TIME,t_del.LAST_UPDATE_TIME,t_del.DELETED_TIME,t_del.SYNCH_STATE) select t.ADGROUP_ID,t.ACCOUNT_ID,t.CAMPAIGN_ID,t.ADGROUP_NAME,t.CPC_PRICE,t.CPM_PRICE,t.TARGETING_TYPE,t.ADGROUP_STATE,t.IMPRESSION,t.CLICK,t.COST,t.DIALOG,t.APPOINTMENT,t.ARRIVAL,t.CREATE_TIME,t.LAST_UPDATE_TIME,sysdate,0 from t_adgroup t where t.account_id=%s ", account_id);

    if (campaign_id)
	sql_size += sprintf(&sql_str[sql_size], " and t.CAMPAIGN_ID='%s'", campaign_id);
    else if (group_id)
	sql_size += sprintf(&sql_str[sql_size], " and t.ADGROUP_ID='%s'", group_id);
    else
	return -1;

    if (SQLOCI_Prepare(conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(conn));
	return -1;
    }

    if (SQLOCI_Execute(conn) != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(conn));
	return -1;
    }

    return 0;
}

int ad_system_oci_del_text_creative_move(SQLOCI_CONN_T *conn, char *account_id, char *campaign_id, char *group_id, char *creative_id)
{

    char sql_str[4096] = {0};
    size_t sql_size = sprintf(sql_str, "insert into t_deleted_text_creative t_del (t_del.CREATIVE_ID,t_del.ACCOUNT_ID,t_del.CAMPAIGN_ID,t_del.ADGROUP_ID,t_del.TITLE,t_del.DESCRIPTION1,t_del.DESCRIPTION2,t_del.DESTINATION_URL,t_del.DISPLAY_URL,t_del.ECTR,t_del.CREATIVE_STATE,t_del.REFUSE_REASON,t_del.PAUSE,t_del.QUALITY,t_del.IMPRESSION,t_del.CLICK,t_del.COST,t_del.DIALOG,t_del.APPOINTMENT,t_del.ARRIVAL,t_del.CREATE_TIME,t_del.LAST_UPDATE_TIME,t_del.AUDITOR_ID,t_del.APPEND_ID,t_del.DELETED_TIME,t_del.SYNCH_STATE) select t.CREATIVE_ID,t.ACCOUNT_ID,t.CAMPAIGN_ID,t.ADGROUP_ID,t.TITLE,t.DESCRIPTION1,t.DESCRIPTION2,t.DESTINATION_URL,t.DISPLAY_URL,t.ECTR,t.CREATIVE_STATE,t.REFUSE_REASON,t.PAUSE,t.QUALITY,t.IMPRESSION,t.CLICK,t.COST,t.DIALOG,t.APPOINTMENT,t.ARRIVAL,t.CREATE_TIME,t.LAST_UPDATE_TIME,t.AUDITOR_ID,t.APPEND_ID,sysdate,0 from t_text_creative t where t.account_id=%s ", account_id);

    if (campaign_id)
	sql_size += sprintf(&sql_str[sql_size], " and t.CAMPAIGN_ID='%s'", campaign_id);
    else if (group_id)
	sql_size += sprintf(&sql_str[sql_size], " and t.ADGROUP_ID='%s'", group_id);
    else if (creative_id)
	sql_size += sprintf(&sql_str[sql_size], " and t.CREATIVE_ID='%s'", creative_id);
    else
	return -1;

    if (SQLOCI_Prepare(conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(conn));
	return -1;
    }

    if (SQLOCI_Execute(conn) != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(conn));
	return -1;
    }

    return 0;
}

int ad_system_oci_del_img_creative_move(SQLOCI_CONN_T *conn, char *account_id, char *campaign_id, char *group_id, char *creative_id)
{

    char sql_str[4096] = {0};
    size_t sql_size = sprintf(sql_str, "INSERT INTO T_DELETED_CREATIVE(CREATIVE_ID,ACCOUNT_ID,CAMPAIGN_ID,ADGROUP_ID,CREATIVE_NAME,IMG_SIZE,IMG_SIZE_IMGLENGTH,FILE_PATH,FILE_STORE_ID,DESTINATION_URL,DISPLAY_URL,ECTR,CREATIVE_STATE,REFUSE_REASON,PAUSE,HAS_UPDATE_TEMP,QUALITY,IMPRESSION,CLICK,COST,DIALOG,APPOINTMENT,ARRIVAL,CREATE_TIME,LAST_UPDATE_TIME,AUDITOR_ID,DELETED_TIME,SYNCH_STATE) SELECT CREATIVE_ID,ACCOUNT_ID,CAMPAIGN_ID,ADGROUP_ID,CREATIVE_NAME,IMG_SIZE,IMG_SIZE_IMGLENGTH,FILE_PATH,FILE_STORE_ID,DESTINATION_URL,DISPLAY_URL,ECTR,CREATIVE_STATE,REFUSE_REASON,PAUSE,HAS_UPDATE_TEMP,QUALITY,IMPRESSION,CLICK,COST,DIALOG,APPOINTMENT,ARRIVAL,CREATE_TIME,LAST_UPDATE_TIME,AUDITOR_ID,sysdate,0 FROM T_CREATIVE t WHERE t.ACCOUNT_ID=%s", account_id);

    if (campaign_id)
	sql_size += sprintf(&sql_str[sql_size], " and t.CAMPAIGN_ID='%s'", campaign_id);
    else if (group_id)
	sql_size += sprintf(&sql_str[sql_size], " and t.ADGROUP_ID='%s'", group_id);
    else if (creative_id)
	sql_size += sprintf(&sql_str[sql_size], " and t.CREATIVE_ID='%s'", creative_id);
    else
	return -1;

    if (SQLOCI_Prepare(conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(conn));
	return -1;
    }

    if (SQLOCI_Execute(conn) != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(conn));
	return -1;
    }

    return 0;
}

int ad_system_oci_del_campaign_data(SQLOCI_CONN_T *conn, char *account_id, char *campaign_id)
{

    char sql_str[4096] = {0};
    size_t sql_size = sprintf(sql_str, "delete from t_campaign t where t.account_id=%s and t.campaign_id=%s",
	    account_id, campaign_id);

    if (SQLOCI_Prepare(conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(conn));
	return -1;
    }

    if (SQLOCI_Execute(conn) != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(conn));
	return -1;
    }

    return 0;
}

int ad_system_oci_del_group_data(SQLOCI_CONN_T *conn, char *account_id, char *campaign_id, char *group_id)
{

    size_t sql_size = 0;
    char sql_str[4096] = {0};

    if (campaign_id)
	sql_size = sprintf(sql_str, "delete from t_adgroup t where t.account_id=%s and t.campaign_id=%s",
		account_id, campaign_id);
    else if (group_id)
	sql_size = sprintf(sql_str, "delete from t_adgroup t where t.account_id=%s and t.adgroup_id=%s",
		account_id, group_id);
    else
	return -1;

    if (SQLOCI_Prepare(conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(conn));
	return -1;
    }

    if (SQLOCI_Execute(conn) != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(conn));
	return -1;
    }

    return 0;
}

int ad_system_oci_del_text_creative_data(SQLOCI_CONN_T *conn, char *account_id, char *campaign_id, char *group_id, char *creative_id)
{

    size_t sql_size = 0;
    char sql_str[4096] = {0};

    if (campaign_id)
	sql_size = sprintf(sql_str, "delete from t_text_creative t where t.account_id=%s and t.campaign_id=%s",
		account_id, campaign_id);
    else if (group_id)
	sql_size = sprintf(sql_str, "delete from t_text_creative t where t.account_id=%s and t.adgroup_id=%s",
		account_id, group_id);
    else if (creative_id)
	sql_size = sprintf(sql_str, "delete from t_text_creative t where t.account_id=%s and t.creative_id=%s",
		account_id, creative_id);
    else
	return -1;

    if (SQLOCI_Prepare(conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(conn));
	return -1;
    }

    if (SQLOCI_Execute(conn) != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(conn));
	return -1;
    }

    return 0;
}

int ad_system_oci_del_img_creative_data(SQLOCI_CONN_T *conn, char *account_id, char *campaign_id, char *group_id, char *creative_id)
{

    size_t sql_size = 0;
    char sql_str[4096] = {0};

    if (campaign_id)
	sql_size = sprintf(sql_str, "delete from t_creative t where t.account_id=%s and t.campaign_id=%s",
		account_id, campaign_id);
    else if (group_id)
	sql_size = sprintf(sql_str, "delete from t_creative t where t.account_id=%s and t.adgroup_id=%s",
		account_id, group_id);
    else if (creative_id)
	sql_size = sprintf(sql_str, "delete from t_creative t where t.account_id=%s and t.creative_id=%s",
		account_id, creative_id);

    if (SQLOCI_Prepare(conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(conn));
	return -1;
    }

    if (SQLOCI_Execute(conn) != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(conn));
	return -1;
    }

    return 0;
}

int ad_system_oci_del_campaign(char *account_id, char *campaign_id)
{

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    if (ad_system_oci_del_text_creative_move(&conn, account_id, campaign_id, NULL, NULL) != 0) {

	SQLOCI_RollBack(&conn);
	SQLOCI_Close(&conn);
	return -1;
    }

    if (ad_system_oci_del_img_creative_move(&conn, account_id, campaign_id, NULL, NULL) != 0) {

	SQLOCI_RollBack(&conn);
	SQLOCI_Close(&conn);
	return -1;
    }

    if (ad_system_oci_del_group_move(&conn, account_id, campaign_id, NULL) != 0) {

	SQLOCI_RollBack(&conn);
	SQLOCI_Close(&conn);
	return -1;
    }

    if (ad_system_oci_del_campaign_move(&conn, account_id, campaign_id) != 0) {

	SQLOCI_RollBack(&conn);
	SQLOCI_Close(&conn);
	return -1;
    }

    if (ad_system_oci_del_text_creative_data(&conn, account_id, campaign_id, NULL, NULL) != 0) {

	SQLOCI_RollBack(&conn);
	SQLOCI_Close(&conn);
	return -1;
    }

    if (ad_system_oci_del_img_creative_data(&conn, account_id, campaign_id, NULL, NULL) != 0) {

	SQLOCI_RollBack(&conn);
	SQLOCI_Close(&conn);
	return -1;
    }

    if (ad_system_oci_del_group_data(&conn, account_id, campaign_id, NULL) != 0) {

	SQLOCI_RollBack(&conn);
	SQLOCI_Close(&conn);
	return -1;
    }

    if (ad_system_oci_del_campaign_data(&conn, account_id, campaign_id) != 0) {

	SQLOCI_RollBack(&conn);
	SQLOCI_Close(&conn);
	return -1;
    }

    SQLOCI_Commit(&conn);
    SQLOCI_Close(&conn);
    return 0;
}

int ad_system_oci_del_group(char *account_id, char *group_id)
{

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    if (ad_system_oci_del_text_creative_move(&conn, account_id, NULL, group_id, NULL) != 0) {

	SQLOCI_RollBack(&conn);
	SQLOCI_Close(&conn);
	return -1;
    }

    if (ad_system_oci_del_img_creative_move(&conn, account_id, NULL, group_id, NULL) != 0) {

	SQLOCI_RollBack(&conn);
	SQLOCI_Close(&conn);
	return -1;
    }

    if (ad_system_oci_del_group_move(&conn, account_id, NULL, group_id) != 0) {

	SQLOCI_RollBack(&conn);
	SQLOCI_Close(&conn);
	return -1;
    }

    if (ad_system_oci_del_text_creative_data(&conn, account_id, NULL, group_id, NULL) != 0) {

	SQLOCI_RollBack(&conn);
	SQLOCI_Close(&conn);
	return -1;
    }

    if (ad_system_oci_del_img_creative_data(&conn, account_id, NULL, group_id, NULL) != 0) {

	SQLOCI_RollBack(&conn);
	SQLOCI_Close(&conn);
	return -1;
    }

    if (ad_system_oci_del_group_data(&conn, account_id, NULL, group_id) != 0) {

	SQLOCI_RollBack(&conn);
	SQLOCI_Close(&conn);
	return -1;
    }

    SQLOCI_Commit(&conn);
    SQLOCI_Close(&conn);
    return 0;
}

int ad_system_oci_del_creative(char *account_id, char *creative_id)
{

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    if (ad_system_oci_del_text_creative_move(&conn, account_id, NULL, NULL, creative_id) != 0) {

	SQLOCI_RollBack(&conn);
	SQLOCI_Close(&conn);
	return -1;
    }

    if (ad_system_oci_del_text_creative_data(&conn, account_id, NULL, NULL, creative_id) != 0) {

	SQLOCI_RollBack(&conn);
	SQLOCI_Close(&conn);
	return -1;
    }

    SQLOCI_Commit(&conn);
    SQLOCI_Close(&conn);
    return 0;
}

int ad_system_oci_del_img_creative(char *account_id, char *creative_id)
{

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    if (ad_system_oci_del_img_creative_move(&conn, account_id, NULL, NULL, creative_id) != 0) {

	SQLOCI_RollBack(&conn);
	SQLOCI_Close(&conn);
	return -1;
    }

    if (ad_system_oci_del_img_creative_data(&conn, account_id, NULL, NULL, creative_id) != 0) {

	SQLOCI_RollBack(&conn);
	SQLOCI_Close(&conn);
	return -1;
    }

    SQLOCI_Commit(&conn);
    SQLOCI_Close(&conn);
    return 0;
}








int ad_system_oci_get_ad_money_recharge(lg_list_t *head,
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
	int page_index_e)
{

    lg_list_init(head);

    size_t sort_size = 0;
    char sort_str[1024] = {0};
    switch(sort_column){

	case 1:
	    sort_size = sprintf(sort_str, "SWIFT_NUMBER");
	    break;

	case 2:
	    sort_size = sprintf(sort_str, "MONEY");
	    break;

	case 3:
	    sort_size = sprintf(sort_str, "PAYMENT_TYPE");
	    break;

	case 4:
	    sort_size = sprintf(sort_str, "INVEST");
	    break;

	case 5:
	    sort_size = sprintf(sort_str, "CURRENT_MONEY_BALANCE");
	    break;

	case 6:
	    sort_size = sprintf(sort_str, "CREATE_TIME");
	    break;
    }

    if (sort_type == 2)
	sprintf(&sort_str[sort_size], " desc");

    size_t size = 0;
    char where_str[1024] = {0};
    if (find_type & 1) {

	switch(find_name_type) {

	    case 1:
		size += sprintf(&where_str[size], "and RECHARGE.SWIFT_NUMBER like '%%%s%%' ", find_name);
		break;

	    case 2:
		size += sprintf(&where_str[size], "and %s = ACCOUNTING_DOCUMENT_NUMBER", find_name);
		break;
	}
    }

    if (money_type != 100)
	size += sprintf(&where_str[size], "and PAYMENT_TYPE=%u", money_type);

    if (find_type & 2) {

	size += sprintf(&where_str[size], "and CREATE_TIME > to_date('%s', 'yyyy-mm-dd') ", time_start);
	size += sprintf(&where_str[size], "and CREATE_TIME < to_date('%s 23:59:59', 'yyyy-mm-dd HH24:MI:SS') ", time_end);
    }

    char sql_str[4096] = {0};
    size_t sql_size = sprintf(sql_str, "select "
	    "SWIFT_NUMBER,"
	    "MONEY,"
	    "PAYMENT_TYPE,"
	    "ACCOUNTING_DOCUMENT_NUMBER,"
	    "ACCOUNTING_DOCUMENT_FILE,"
	    "INVEST,"
	    "CURRENT_MONEY_BALANCE,"
	    "CREATE_TIME "
	    "from (select "
	    "rownum as page_index,"
	    "nvl(to_char(SWIFT_NUMBER), 'null') as SWIFT_NUMBER,"
	    "nvl(to_char(MONEY), 'null') as MONEY,"
	    "nvl(to_char(CURRENT_MONEY_BALANCE), 'null') as CURRENT_MONEY_BALANCE,"
	    "nvl(to_char(INVEST), 'null') as INVEST,"
	    "nvl(to_char(PAYMENT_TYPE), 'null') as PAYMENT_TYPE,"
	    "nvl(to_char(ACCOUNTING_DOCUMENT_NUMBER), 'null') as ACCOUNTING_DOCUMENT_NUMBER,"
	    "nvl(to_char(ACCOUNTING_DOCUMENT_FILE), 'null') as ACCOUNTING_DOCUMENT_FILE,"
	    "nvl(to_char(ACCOUNTING_DOCUMENT_REMARK), 'null') as ACCOUNTING_DOCUMENT_REMARK,"
	    "nvl(to_char(CREATE_TIME, 'yyyy-mm-dd'), 'null') as CREATE_TIME "
	    "from T_RECHARGE "
	    "where account_id=%s  %s order by %s) t "
	    "where t.page_index between %u and %u",

	account_id,
	where_str,
	sort_str,
	page_index_s,
	page_index_e);

    fprintf(stdout, "=============\n");
    fprintf(stdout, "%s\n", sql_str);
    fprintf(stdout, "=============\n");

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    ad_system_oci_money_recharge_t temp;
    memset(&temp, 0, sizeof(ad_system_oci_money_recharge_t));
    if (SQLOCI_Bind_Ouput(&conn, "1%s2%s3%s4%s5%s6%s7%s8%s",
		temp.SWIFT_NUMBER,				
		temp.MONEY,                                     
		temp.PAYMENT_TYPE,                              
		temp.ACCOUNTING_DOCUMENT_NUMBER,
		temp.ACCOUNTING_DOCUMENT_FILE,                  
		temp.INVEST,                                    
		temp.CURRENT_MONEY_BALANCE,                     
		temp.CREATE_TIME) != 0) {                  

	fprintf(stdout, "SQLOCI_Bind_Ouput::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    int ret = SQLOCI_Execute(&conn);
    if (ret == 100) {

	SQLOCI_Close(&conn);
	return 100;

    } else if (ret != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    while(ret == 0) {

	ad_system_oci_money_recharge_t *node = NULL;
	if ((node = malloc(sizeof(ad_system_oci_money_recharge_t)))) {

	    memcpy(node, &temp, sizeof(ad_system_oci_money_recharge_t));
	    lg_list_add(head, &node->list);
	}

	memset(&temp, 0, sizeof(ad_system_oci_money_recharge_t));
	ret = SQLOCI_Next_Data(&conn);
    }

    SQLOCI_Close(&conn);
    return 0;
}

int ad_system_oci_get_ad_money_recharge_count(char *count,
	char *account_id,
	int find_type,
	int money_type,
	int find_name_type,
	char *find_name,
	char *time_start,
	char *time_end)
{


    size_t size = 0;
    char where_str[1024] = {0};
    if (find_type & 1) {

	switch(find_name_type) {

	    case 1:
		size += sprintf(&where_str[size], "and RECHARGE.SWIFT_NUMBER like '%%%s%%' ", find_name);
		break;

	    case 2:
		size += sprintf(&where_str[size], "and ACCOUNTING_DOCUMENT_NUMBER like '%%%s%%' ", find_name);
		break;
	}
    }

    if (money_type != 100)
	size += sprintf(&where_str[size], "and PAYMENT_TYPE=%u", money_type);

    if (find_type & 2) {

	size += sprintf(&where_str[size], "and CREATE_TIME > to_date('%s', 'yyyy-mm-dd') ", time_start);
	size += sprintf(&where_str[size], "and CREATE_TIME < to_date('%s 23:59:59', 'yyyy-mm-dd HH24:MI:SS') ", time_end);
    }

    char sql_str[4096] = {0};
    size_t sql_size = sprintf(sql_str, "select count(*) from T_RECHARGE	where account_id = %s %s", account_id, where_str);

    fprintf(stdout, "%s\n", sql_str);

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    if (SQLOCI_Bind_Ouput(&conn, "%s", count) != 0) {

	fprintf(stdout, "SQLOCI_Bind_Ouput::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    int ret = SQLOCI_Execute(&conn);
    if (ret != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    SQLOCI_Close(&conn);
    return 0;
}

int ad_system_oci_get_group_price_and_campaign_budget(const char *account_id, char *group_id, 
	double *CAMPAIGN_BUDGET,
	int *GROUP_BIDDING_MODEL,
	double *GROUP_CPC_PRICE,
	double *GROUP_CPM_PRICE)
{

    char sql_str[4096] = {0};
    size_t sql_size = sprintf(sql_str,
	    "select nvl(campaign.campaign_budget, 0),"
	    "nvl(adgroup.bidding_model, 0),"
	    "nvl(adgroup.cpc_price, 0),"
	    "nvl(adgroup.cpm_price, 0) "
	    "from t_adgroup adgroup "
	    "left join t_campaign campaign on adgroup.CAMPAIGN_ID = campaign.campaign_id "
	    "left join t_adgroup_targeting group_targeting on adgroup.adgroup_id = group_targeting.adgroup_id "
	    "where adgroup.account_id = %s "
	    "and adgroup.adgroup_id = '%s'",
	    account_id, group_id);

    fprintf(stdout, "%s\n", sql_str);

    SQLOCI_CONN_T conn;
    SQLOCI_Init(&conn);
    if (SQLOCI_Connect(&conn, oracle_name, oracle_pass, oracle_host) != 0) {

	fprintf(stdout, "SQLOCI_Connect::%s\n", SQLOCI_Error(&conn));
	return -1;
    }

    if (SQLOCI_Bind_Ouput(&conn, "1%f2%d3%f4%f",
		CAMPAIGN_BUDGET,
		GROUP_BIDDING_MODEL,
		GROUP_CPC_PRICE,
		GROUP_CPM_PRICE
		) != 0) {

	fprintf(stdout, "SQLOCI_Bind_Ouput::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    if (SQLOCI_Prepare(&conn, sql_str, sql_size) != 0) {

	fprintf(stdout, "SQLOCI_Prepare::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    int ret = SQLOCI_Execute(&conn);
    if (ret == 100) {

	SQLOCI_Close(&conn);
	return 100;

    } else if (ret != 0) {

	fprintf(stdout, "SQLOCI_Execute::%s\n", SQLOCI_Error(&conn));
	SQLOCI_Close(&conn);
	return -1;
    }

    SQLOCI_Close(&conn);
    return 0;
}

