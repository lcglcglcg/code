
#if defined(__BORLANDC__)
#pragma option push -w-8060
#pragma option push -w-8004
#endif
#include "soapH.h"
#ifdef __cplusplus
extern "C" {
#endif

    SOAP_SOURCE_STAMP("@(#) soapClient.c ver 2.8.14 2013-04-08 08:00:15 GMT")


	SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__getProfessionalReportId(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__getProfessionalReportIdRequest *ns1__getProfessionalReportIdRequest, struct _ns1__getProfessionalReportIdResponse *ns1__getProfessionalReportIdResponse)
	{	struct __ns1__getProfessionalReportId soap_tmp___ns1__getProfessionalReportId;
	    if (soap_endpoint == NULL)
		soap_endpoint = "https://api.baidu.com/sem/sms/v2/ReportService";
	    if (soap_action == NULL)
		soap_action = "https://api.baidu.com/sem/sms/v2/ReportService/getProfessionalReportId";
	    soap->encodingStyle = NULL;
	    soap_tmp___ns1__getProfessionalReportId.ns1__getProfessionalReportIdRequest = ns1__getProfessionalReportIdRequest;
	    soap_begin(soap);
	    soap_serializeheader(soap);
	    soap_serialize___ns1__getProfessionalReportId(soap, &soap_tmp___ns1__getProfessionalReportId);
	    if (soap_begin_count(soap))
		return soap->error;
	    if (soap->mode & SOAP_IO_LENGTH)
	    {	if (soap_envelope_begin_out(soap)
		    || soap_putheader(soap)
		    || soap_body_begin_out(soap)
		    || soap_put___ns1__getProfessionalReportId(soap, &soap_tmp___ns1__getProfessionalReportId, "-ns1:getProfessionalReportId", NULL)
		    || soap_body_end_out(soap)
		    || soap_envelope_end_out(soap))
		return soap->error;
	    }
	    if (soap_end_count(soap))
		return soap->error;
	    if (soap_connect(soap, soap_endpoint, soap_action)
		    || soap_envelope_begin_out(soap)
		    || soap_putheader(soap)
		    || soap_body_begin_out(soap)
		    || soap_put___ns1__getProfessionalReportId(soap, &soap_tmp___ns1__getProfessionalReportId, "-ns1:getProfessionalReportId", NULL)
		    || soap_body_end_out(soap)
		    || soap_envelope_end_out(soap)
		    || soap_end_send(soap))
		return soap_closesock(soap);
	    if (!ns1__getProfessionalReportIdResponse)
		return soap_closesock(soap);
	    soap_default__ns1__getProfessionalReportIdResponse(soap, ns1__getProfessionalReportIdResponse);
	    if (soap_begin_recv(soap)
		    || soap_envelope_begin_in(soap)
		    || soap_recv_header(soap)
		    || soap_body_begin_in(soap))
		return soap_closesock(soap);
	    soap_get__ns1__getProfessionalReportIdResponse(soap, ns1__getProfessionalReportIdResponse, "ns1:getProfessionalReportIdResponse", "");
	    if (soap->error) {

		return soap_recv_fault(soap, 0);
	    }

	    if (soap_body_end_in(soap)
		    || soap_envelope_end_in(soap)
		    || soap_end_recv(soap))
		return soap_closesock(soap);
	    return soap_closesock(soap);
	}

    SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__getReportState(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__getReportStateRequest *ns1__getReportStateRequest, struct _ns1__getReportStateResponse *ns1__getReportStateResponse)
    {	struct __ns1__getReportState soap_tmp___ns1__getReportState;
	if (soap_endpoint == NULL)
	    soap_endpoint = "https://api.baidu.com/sem/sms/v2/ReportService";
	if (soap_action == NULL)
	    soap_action = "https://api.baidu.com/sem/sms/v2/ReportService/getReportState";
	soap->encodingStyle = NULL;
	soap_tmp___ns1__getReportState.ns1__getReportStateRequest = ns1__getReportStateRequest;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___ns1__getReportState(soap, &soap_tmp___ns1__getReportState);
	if (soap_begin_count(soap))
	    return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		|| soap_putheader(soap)
		|| soap_body_begin_out(soap)
		|| soap_put___ns1__getReportState(soap, &soap_tmp___ns1__getReportState, "-ns1:getReportState", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap))
	    return soap->error;
	}
	if (soap_end_count(soap))
	    return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
		|| soap_envelope_begin_out(soap)
		|| soap_putheader(soap)
		|| soap_body_begin_out(soap)
		|| soap_put___ns1__getReportState(soap, &soap_tmp___ns1__getReportState, "-ns1:getReportState", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
	    return soap_closesock(soap);
	if (!ns1__getReportStateResponse)
	    return soap_closesock(soap);
	soap_default__ns1__getReportStateResponse(soap, ns1__getReportStateResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
	    return soap_closesock(soap);
	soap_get__ns1__getReportStateResponse(soap, ns1__getReportStateResponse, "ns1:getReportStateResponse", "");
	if (soap->error)
	    return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
	    return soap_closesock(soap);
	return soap_closesock(soap);
    }

    SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__getReportFileUrl(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__getReportFileUrlRequest *ns1__getReportFileUrlRequest, struct _ns1__getReportFileUrlResponse *ns1__getReportFileUrlResponse)
    {	struct __ns1__getReportFileUrl soap_tmp___ns1__getReportFileUrl;
	if (soap_endpoint == NULL)
	    soap_endpoint = "https://api.baidu.com/sem/sms/v2/ReportService";
	if (soap_action == NULL)
	    soap_action = "https://api.baidu.com/sem/sms/v2/ReportService/getReportFileUrl";
	soap->encodingStyle = NULL;
	soap_tmp___ns1__getReportFileUrl.ns1__getReportFileUrlRequest = ns1__getReportFileUrlRequest;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___ns1__getReportFileUrl(soap, &soap_tmp___ns1__getReportFileUrl);
	if (soap_begin_count(soap))
	    return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		|| soap_putheader(soap)
		|| soap_body_begin_out(soap)
		|| soap_put___ns1__getReportFileUrl(soap, &soap_tmp___ns1__getReportFileUrl, "-ns1:getReportFileUrl", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap))
	    return soap->error;
	}
	if (soap_end_count(soap))
	    return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
		|| soap_envelope_begin_out(soap)
		|| soap_putheader(soap)
		|| soap_body_begin_out(soap)
		|| soap_put___ns1__getReportFileUrl(soap, &soap_tmp___ns1__getReportFileUrl, "-ns1:getReportFileUrl", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
	    return soap_closesock(soap);
	if (!ns1__getReportFileUrlResponse)
	    return soap_closesock(soap);
	soap_default__ns1__getReportFileUrlResponse(soap, ns1__getReportFileUrlResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
	    return soap_closesock(soap);
	soap_get__ns1__getReportFileUrlResponse(soap, ns1__getReportFileUrlResponse, "ns1:getReportFileUrlResponse", "");
	if (soap->error)
	    return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
	    return soap_closesock(soap);
	return soap_closesock(soap);
    }

#ifdef __cplusplus
}
#endif

#if defined(__BORLANDC__)
#pragma option pop
#pragma option pop
#endif

