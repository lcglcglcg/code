
#include "ad_system_display_db.h"

// char *host = "192.168.49.106/xe";
char *host = "188.188.3.152/dev";
char *user = "lcg";
char *pass = "lcg";

int ad_system_db_load_kv(lg_db_t *db, int type, char *sql_str)
{

    lg_ocilib_t conn;
    if (lg_ocilib_conn(&conn, host, user, pass))
	return -1;

    if (lg_ocilib_find(&conn, sql_str)) {

	lg_ocilib_free(&conn);
	return -1;
    }

    while(lg_ocilib_fetch_next(&conn)) {

	char *key	= lg_ocilib_get_string(&conn, 1);
	char *value	= lg_ocilib_get_string(&conn, 2);

	char buffer[1024];
	size_t size = sprintf(buffer, 
		"%u%c"
		"%s%c",
		type,	CH_COMM,
		value,  CH_ROW);

	// fprintf(stdout, "%s\n", buffer);
	ad_system_db_element_add_kv(db, key, buffer, size);
    }

    lg_ocilib_free(&conn);
    return 0;
}

int ad_system_db_load_info_region_img(lg_db_t *db)
{

    char buffer[409600];
    char *sql_str = "select "
	"creative.account_id,"
	"creative.campaign_id,"
	"creative.adgroup_id,"
	"creative.creative_id,"
	"campaign.region_type,"
	"campaign.region_serialize,"
	"adgroup_targeting.code,"
	"adgroup.cpc_price,"
	"adgroup.cpm_price,"
	"creative.img_size,"
	"creative.file_path,"
	"creative.destination_url,"
	"creative.display_url,"
	"account.webim"
	" from t_creative creative"
	" left join t_adgroup adgroup on creative.adgroup_id = adgroup.adgroup_id"
	" left join t_adgroup_targeting adgroup_targeting on creative.adgroup_id = adgroup_targeting.adgroup_id"
	" left join t_campaign campaign on creative.campaign_id = campaign.campaign_id"
	" left join t_account account on creative.account_id = account.account_id";
    // fprintf(stdout, "%s\n", sql_str);

    lg_ocilib_t conn;
    if (lg_ocilib_conn(&conn, host, user, pass))
	return -1;

    if (lg_ocilib_find(&conn, sql_str)) {

	lg_ocilib_free(&conn);
	return -1;
    }

    int index = 1;
    while(lg_ocilib_fetch_next(&conn)) {

	char *account_id		= lg_ocilib_get_string(&conn, 1);
	char *campaign_id		= lg_ocilib_get_string(&conn, 2);
	char *adgroup_id		= lg_ocilib_get_string(&conn, 3);
	char *creative_id		= lg_ocilib_get_string(&conn, 4);
	int region_type			= lg_ocilib_get_number(&conn, 5);
	char *region_code		= lg_ocilib_get_string(&conn, 6);
	char *disease_code		= lg_ocilib_get_string(&conn, 7);
	double cpc_price		= lg_ocilib_get_double(&conn, 8);
	double cpm_price		= lg_ocilib_get_double(&conn, 9);
	char *img_size			= lg_ocilib_get_string(&conn, 10);
	char *img_path			= lg_ocilib_get_string(&conn, 11);
	char *destination_url		= lg_ocilib_get_string(&conn, 12);
	char *display_url 		= lg_ocilib_get_string(&conn, 13);
	char *webim			= lg_ocilib_get_string(&conn, 14);

	size_t size = sprintf(buffer,
		"%u%c"
		"%s%c"
		"%s%c"
		"%s%c"
		"%s%c"
		"%s%c"
		"%s%c"
		"%s%c"
		"%s%c"
		"%s%c",
		index,			CH_COMM,
		account_id,		CH_COMM,
		campaign_id,		CH_COMM,
		adgroup_id,		CH_COMM,
		creative_id,		CH_COMM,
		img_size,		CH_COMM,
		img_path,		CH_COMM,
		destination_url,	CH_COMM,
		display_url, 		CH_COMM,
		webim,			CH_ROW);

	ad_system_db_element_add_list(db,
		region_type,
		region_code,
		0, //disease_type,
		disease_code,
		cpc_price,
		cpm_price, 
		buffer,
		size);
    }

    lg_ocilib_free(&conn);
    return 0;
}

int ad_system_db_load_info_region_txt(lg_db_t *db)
{

    char buffer[409600];
    char *sql_str = "select creative.account_id,"
	"creative.campaign_id,"
	"creative.adgroup_id,"
	"creative.creative_id,"
	"campaign.region_type,"
	"campaign.region_serialize,"
	"adgroup_targeting.code,"
	"adgroup.cpc_price,"
	"adgroup.cpm_price,"
	"creative.title,"
	"creative.description1,"
	"creative.destination_url,"
	"creative.display_url,"
	"creative.mobile_destination_url,"
	"creative.mobile_display_url,"
	"account.webim"
	" from t_text_creative creative"
	" left join t_adgroup adgroup on creative.adgroup_id = adgroup.adgroup_id"
	" left join t_adgroup_targeting adgroup_targeting on creative.adgroup_id = adgroup_targeting.adgroup_id"
	" left join t_campaign campaign on creative.campaign_id = campaign.campaign_id"
	" left join t_account account on creative.account_id = account.account_id";
    // fprintf(stdout, "%s\n", sql_str);

    lg_ocilib_t conn;
    if (lg_ocilib_conn(&conn, host, user, pass))
	return -1;

    if (lg_ocilib_find(&conn, sql_str)) {

	lg_ocilib_free(&conn);
	return -1;
    }

    int index = 1;
    while(lg_ocilib_fetch_next(&conn)) {

	char *account_id		= lg_ocilib_get_string(&conn, 1);
	char *campaign_id		= lg_ocilib_get_string(&conn, 2);
	char *adgroup_id		= lg_ocilib_get_string(&conn, 3);
	char *creative_id		= lg_ocilib_get_string(&conn, 4);
	int region_type			= lg_ocilib_get_number(&conn, 5);
	char *region_code		= lg_ocilib_get_string(&conn, 6);
	char *disease_code		= lg_ocilib_get_string(&conn, 7);
	double cpc_price		= lg_ocilib_get_double(&conn, 8);
	double cpm_price		= lg_ocilib_get_double(&conn, 9);
	char *title			= lg_ocilib_get_string(&conn, 10);
	char *description               = lg_ocilib_get_string(&conn, 11);
	char *destination_url           = lg_ocilib_get_string(&conn, 12);
	char *display_url               = lg_ocilib_get_string(&conn, 13);
	char *mobile_destination_url    = lg_ocilib_get_string(&conn, 14);
	char *mobile_display_url        = lg_ocilib_get_string(&conn, 15);
	char *webim			= lg_ocilib_get_string(&conn, 16);

	size_t size = sprintf(buffer,
		"%u%c"
		"%s%c"
		"%s%c"
		"%s%c"
		"%s%c"
		"%s%c"
		"%s%c"
		"%s%c"
		"%s%c"
		"%s%c"
		"%s%c"
		"%s%c",
		index,			CH_COMM,
		account_id,		CH_COMM,
		campaign_id,		CH_COMM,
		adgroup_id,		CH_COMM,
		creative_id,		CH_COMM,
		title,			CH_COMM,
		description,		CH_COMM,
		destination_url,	CH_COMM,
		display_url,		CH_COMM,
		mobile_destination_url,	CH_COMM,
		mobile_display_url,	CH_COMM,
		webim,			CH_ROW);

	ad_system_db_element_add_list(db,
		region_type,
		region_code,
		0, //disease_type,
		disease_code,
		cpc_price,
		cpm_price, 
		buffer,
		size);
    }

    lg_ocilib_free(&conn);
    return 0;
}

int ad_system_db_load_info_disease_img(lg_db_t *db)
{
    return 0;
}

int ad_system_db_load_info_disease_txt(lg_db_t *db)
{

    return 0;
}

int ad_system_db_load_dict_place(lg_db_t *db)
{
    return ad_system_db_load_kv(db, 1, "select ad_place_id,ad_size from t_ad_place");
}

int ad_system_db_load_dict_region(lg_db_t *db)
{
    return ad_system_db_load_kv(db, 2, "select uri,code from t_code_39_region");
}

int ad_system_db_load_dict_disease(lg_db_t *db)
{
    return ad_system_db_load_kv(db, 3, "select uri,code from t_code_39_disease");
}

int ad_system_db_load_dict_dept(lg_db_t *db)
{
    return ad_system_db_load_kv(db, 3, "select uri,code from t_code_39_dept");
}





