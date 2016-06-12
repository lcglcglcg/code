
#ifndef	__AD_SYSTEM_REPORT_XLS_FILE_H__
#define	__AD_SYSTEM_REPORT_XLS_FILE_H__

#ifdef __cplusplus
extern "C" { 
#endif

    extern int ad_system_report_xls_file_standard(lg_list_t *cpc_head, lg_list_t *cpm_head, int report_type, const char *report_path);
    extern int ad_system_report_xls_file_complex(lg_list_t *cpc_head, lg_list_t *cpm_head, int report_type, const char *report_path);

#ifdef __cplusplus
}
#endif

#endif

