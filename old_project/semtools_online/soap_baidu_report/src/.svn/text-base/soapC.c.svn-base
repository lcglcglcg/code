
#if defined(__BORLANDC__)
#pragma option push -w-8060
#pragma option push -w-8004
#endif

#include "soapH.h"

#ifdef __cplusplus
extern "C" {
#endif

    SOAP_SOURCE_STAMP("@(#) soapC.c ver 2.8.14 2013-04-08 08:00:15 GMT")

	SOAP_NMAC struct Namespace namespaces[] =
	{
	    {"SOAP-ENV", "http://schemas.xmlsoap.org/soap/envelope/", "http://www.w3.org/*/soap-envelope", NULL},
	    {"SOAP-ENC", "http://schemas.xmlsoap.org/soap/encoding/", "http://www.w3.org/*/soap-encoding", NULL},
	    {"xsi", "http://www.w3.org/2001/XMLSchema-instance", "http://www.w3.org/*/XMLSchema-instance", NULL},
	    {"xsd", "http://www.w3.org/2001/XMLSchema", "http://www.w3.org/*/XMLSchema", NULL},
	    {"ns2", "http://api.baidu.com/sem/common/v2", NULL, NULL},
	    {"ns1", "https://api.baidu.com/sem/sms/v2", NULL, NULL},
	    {NULL, NULL, NULL, NULL}
	};


#ifndef WITH_NOGLOBAL

    SOAP_FMAC3 int SOAP_FMAC4 soap_getheader(struct soap *soap)
    {
	soap->part = SOAP_IN_HEADER;
	soap->header = soap_in_SOAP_ENV__Header(soap, "SOAP-ENV:Header", soap->header, NULL);
	soap->part = SOAP_END_HEADER;

	return soap->header == NULL;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_putheader(struct soap *soap)
    {
	if (soap->version && soap->header)
	{	soap->part = SOAP_IN_HEADER;
	    if (soap_out_SOAP_ENV__Header(soap, "SOAP-ENV:Header", 0, soap->header, NULL))
		return soap->error;
	    soap->part = SOAP_END_HEADER;
	}
	return SOAP_OK;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serializeheader(struct soap *soap)
    {
	if (soap->version && soap->header)
	    soap_serialize_SOAP_ENV__Header(soap, soap->header);
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_header(struct soap *soap)
    {
	if (soap->header == NULL)
	{	if ((soap->header = (struct SOAP_ENV__Header*)soap_malloc(soap, sizeof(struct SOAP_ENV__Header))))
	    soap_default_SOAP_ENV__Header(soap, soap->header);
	}
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_fault(struct soap *soap)
    {
	if (soap->fault == NULL)
	{	soap->fault = (struct SOAP_ENV__Fault*)soap_malloc(soap, sizeof(struct SOAP_ENV__Fault));
	    if (soap->fault == NULL)
		return;
	    soap_default_SOAP_ENV__Fault(soap, soap->fault);
	}
	if (soap->version == 2 && !soap->fault->SOAP_ENV__Code)
	{	soap->fault->SOAP_ENV__Code = (struct SOAP_ENV__Code*)soap_malloc(soap, sizeof(struct SOAP_ENV__Code));
	    soap_default_SOAP_ENV__Code(soap, soap->fault->SOAP_ENV__Code);
	}
	if (soap->version == 2 && !soap->fault->SOAP_ENV__Reason)
	{	soap->fault->SOAP_ENV__Reason = (struct SOAP_ENV__Reason*)soap_malloc(soap, sizeof(struct SOAP_ENV__Reason));
	    soap_default_SOAP_ENV__Reason(soap, soap->fault->SOAP_ENV__Reason);
	}
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serializefault(struct soap *soap)
    {
	if (soap->fault)
	    soap_serialize_SOAP_ENV__Fault(soap, soap->fault);
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_putfault(struct soap *soap)
    {
	if (soap->fault)
	    return soap_put_SOAP_ENV__Fault(soap, soap->fault, "SOAP-ENV:Fault", NULL);
	return SOAP_OK;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_getfault(struct soap *soap)
    {
	return (soap->fault = soap_get_SOAP_ENV__Fault(soap, NULL, "SOAP-ENV:Fault", NULL)) == NULL;
    }

    SOAP_FMAC3 const char ** SOAP_FMAC4 soap_faultcode(struct soap *soap)
    {
	soap_fault(soap);
	if (soap->version == 2 && soap->fault->SOAP_ENV__Code)
	    return (const char**)&soap->fault->SOAP_ENV__Code->SOAP_ENV__Value;
	return (const char**)&soap->fault->faultcode;
    }

    SOAP_FMAC3 const char ** SOAP_FMAC4 soap_faultsubcode(struct soap *soap)
    {
	soap_fault(soap);
	if (soap->version == 2)
	{	if (soap->fault->SOAP_ENV__Code->SOAP_ENV__Subcode == NULL)
	    {	soap->fault->SOAP_ENV__Code->SOAP_ENV__Subcode = (struct SOAP_ENV__Code*)soap_malloc(soap, sizeof(struct SOAP_ENV__Code));
		soap_default_SOAP_ENV__Code(soap, soap->fault->SOAP_ENV__Code->SOAP_ENV__Subcode);
	    }
	    return (const char**)&soap->fault->SOAP_ENV__Code->SOAP_ENV__Subcode->SOAP_ENV__Value;
	}
	return (const char**)&soap->fault->faultcode;
    }

    SOAP_FMAC3 const char * SOAP_FMAC4 soap_check_faultsubcode(struct soap *soap)
    {
	soap_fault(soap);
	if (soap->version == 2)
	{	if (soap->fault->SOAP_ENV__Code && soap->fault->SOAP_ENV__Code->SOAP_ENV__Subcode && soap->fault->SOAP_ENV__Code->SOAP_ENV__Subcode)
	    return soap->fault->SOAP_ENV__Code->SOAP_ENV__Subcode->SOAP_ENV__Value;
	    return NULL;
	}
	return soap->fault->faultcode;
    }

    SOAP_FMAC3 const char ** SOAP_FMAC4 soap_faultstring(struct soap *soap)
    {
	soap_fault(soap);
	if (soap->version == 2)
	    return (const char**)&soap->fault->SOAP_ENV__Reason->SOAP_ENV__Text;
	return (const char**)&soap->fault->faultstring;
    }

    SOAP_FMAC3 const char ** SOAP_FMAC4 soap_faultdetail(struct soap *soap)
    {
	soap_fault(soap);
	if (soap->version == 2)
	{	if (soap->fault->SOAP_ENV__Detail == NULL)
	    {	soap->fault->SOAP_ENV__Detail = (struct SOAP_ENV__Detail*)soap_malloc(soap, sizeof(struct SOAP_ENV__Detail));
		soap_default_SOAP_ENV__Detail(soap, soap->fault->SOAP_ENV__Detail);
	    }
	    return (const char**)&soap->fault->SOAP_ENV__Detail->__any;
	}
	if (soap->fault->detail == NULL)
	{	soap->fault->detail = (struct SOAP_ENV__Detail*)soap_malloc(soap, sizeof(struct SOAP_ENV__Detail));
	    soap_default_SOAP_ENV__Detail(soap, soap->fault->detail);
	}
	return (const char**)&soap->fault->detail->__any;
    }

    SOAP_FMAC3 const char * SOAP_FMAC4 soap_check_faultdetail(struct soap *soap)
    {
	soap_fault(soap);
	if (soap->version == 2 && soap->fault->SOAP_ENV__Detail)
	    return soap->fault->SOAP_ENV__Detail->__any;
	if (soap->fault->detail)
	    return soap->fault->detail->__any;
	return NULL;
    }

#endif

#ifndef WITH_NOIDREF
    SOAP_FMAC3 int SOAP_FMAC4 soap_getindependent(struct soap *soap)
    {
	int t;
	if (soap->version == 1)
	{	for (;;)
	    {	if (!soap_getelement(soap, &t))
		if (soap->error || soap_ignore_element(soap))
		    break;
	    }
	}
	if (soap->error == SOAP_NO_TAG || soap->error == SOAP_EOF)
	    soap->error = SOAP_OK;
	return soap->error;
    }
#endif
    SOAP_FMAC3 void * SOAP_FMAC4 soap_getelement(struct soap *soap, int *type)
    {	(void)type;
	if (soap_peek_element(soap))
	    return NULL;
#ifndef WITH_NOIDREF
	if (!*soap->id || !(*type = soap_lookup_type(soap, soap->id)))
	    *type = soap_lookup_type(soap, soap->href);
	switch (*type)
	{
	    case SOAP_TYPE_byte:
		return soap_in_byte(soap, NULL, NULL, "xsd:byte");
	    case SOAP_TYPE_int:
		return soap_in_int(soap, NULL, NULL, "xsd:int");
	    case SOAP_TYPE_LONG64:
		return soap_in_LONG64(soap, NULL, NULL, "xsd:long");
	    case SOAP_TYPE_float:
		return soap_in_float(soap, NULL, NULL, "xsd:float");
	    case SOAP_TYPE_double:
		return soap_in_double(soap, NULL, NULL, "xsd:double");
	    case SOAP_TYPE_time:
		return soap_in_time(soap, NULL, NULL, "xsd:dateTime");
	    case SOAP_TYPE_xsd__boolean:
		return soap_in_xsd__boolean(soap, NULL, NULL, "xsd:boolean");
	    case SOAP_TYPE_ns2__Failure:
		return soap_in_ns2__Failure(soap, NULL, NULL, "ns2:Failure");
	    case SOAP_TYPE_ns2__ResHeader:
		return soap_in_ns2__ResHeader(soap, NULL, NULL, "ns2:ResHeader");
	    case SOAP_TYPE_ns2__AuthHeader:
		return soap_in_ns2__AuthHeader(soap, NULL, NULL, "ns2:AuthHeader");
	    case SOAP_TYPE_ns2__DoubleMapItemType:
		return soap_in_ns2__DoubleMapItemType(soap, NULL, NULL, "ns2:DoubleMapItemType");
	    case SOAP_TYPE_ns2__FloatMapItemType:
		return soap_in_ns2__FloatMapItemType(soap, NULL, NULL, "ns2:FloatMapItemType");
	    case SOAP_TYPE_ns2__LongMapItemType:
		return soap_in_ns2__LongMapItemType(soap, NULL, NULL, "ns2:LongMapItemType");
	    case SOAP_TYPE_ns2__IntMapItemType:
		return soap_in_ns2__IntMapItemType(soap, NULL, NULL, "ns2:IntMapItemType");
	    case SOAP_TYPE_ns2__StringMapItemType:
		return soap_in_ns2__StringMapItemType(soap, NULL, NULL, "ns2:StringMapItemType");
	    case SOAP_TYPE_ns2__OptType:
		return soap_in_ns2__OptType(soap, NULL, NULL, "ns2:OptType");
	    case SOAP_TYPE_ns1__AttributeType:
		return soap_in_ns1__AttributeType(soap, NULL, NULL, "ns1:AttributeType");
	    case SOAP_TYPE_ns1__ReportRequestType:
		return soap_in_ns1__ReportRequestType(soap, NULL, NULL, "ns1:ReportRequestType");
	    case SOAP_TYPE_PointerTo_ns1__getReportFileUrlResponse:
		return soap_in_PointerTo_ns1__getReportFileUrlResponse(soap, NULL, NULL, "ns1:getReportFileUrlResponse");
	    case SOAP_TYPE_PointerTo_ns1__getReportFileUrlRequest:
		return soap_in_PointerTo_ns1__getReportFileUrlRequest(soap, NULL, NULL, "ns1:getReportFileUrlRequest");
	    case SOAP_TYPE_PointerTo_ns1__getReportStateResponse:
		return soap_in_PointerTo_ns1__getReportStateResponse(soap, NULL, NULL, "ns1:getReportStateResponse");
	    case SOAP_TYPE_PointerTo_ns1__getReportStateRequest:
		return soap_in_PointerTo_ns1__getReportStateRequest(soap, NULL, NULL, "ns1:getReportStateRequest");
	    case SOAP_TYPE_PointerTo_ns1__getProfessionalReportIdResponse:
		return soap_in_PointerTo_ns1__getProfessionalReportIdResponse(soap, NULL, NULL, "ns1:getProfessionalReportIdResponse");
	    case SOAP_TYPE_PointerTo_ns1__getProfessionalReportIdRequest:
		return soap_in_PointerTo_ns1__getProfessionalReportIdRequest(soap, NULL, NULL, "ns1:getProfessionalReportIdRequest");
	    case SOAP_TYPE_PointerTons2__ResHeader:
		return soap_in_PointerTons2__ResHeader(soap, NULL, NULL, "ns2:ResHeader");
	    case SOAP_TYPE_PointerTons2__AuthHeader:
		return soap_in_PointerTons2__AuthHeader(soap, NULL, NULL, "ns2:AuthHeader");
	    case SOAP_TYPE_PointerTons2__Failure:
		return soap_in_PointerTons2__Failure(soap, NULL, NULL, "ns2:Failure");
	    case SOAP_TYPE_PointerTons2__DoubleMapItemType:
		return soap_in_PointerTons2__DoubleMapItemType(soap, NULL, NULL, "ns2:DoubleMapItemType");
	    case SOAP_TYPE_PointerTons2__FloatMapItemType:
		return soap_in_PointerTons2__FloatMapItemType(soap, NULL, NULL, "ns2:FloatMapItemType");
	    case SOAP_TYPE_PointerTons2__LongMapItemType:
		return soap_in_PointerTons2__LongMapItemType(soap, NULL, NULL, "ns2:LongMapItemType");
	    case SOAP_TYPE_PointerTons2__IntMapItemType:
		return soap_in_PointerTons2__IntMapItemType(soap, NULL, NULL, "ns2:IntMapItemType");
	    case SOAP_TYPE_PointerTons2__StringMapItemType:
		return soap_in_PointerTons2__StringMapItemType(soap, NULL, NULL, "ns2:StringMapItemType");
	    case SOAP_TYPE_PointerTons1__ReportRequestType:
		return soap_in_PointerTons1__ReportRequestType(soap, NULL, NULL, "ns1:ReportRequestType");
	    case SOAP_TYPE_PointerToLONG64:
		return soap_in_PointerToLONG64(soap, NULL, NULL, "xsd:long");
	    case SOAP_TYPE_PointerTons1__AttributeType:
		return soap_in_PointerTons1__AttributeType(soap, NULL, NULL, "ns1:AttributeType");
	    case SOAP_TYPE_PointerToint:
		return soap_in_PointerToint(soap, NULL, NULL, "xsd:int");
	    case SOAP_TYPE_PointerToxsd__boolean:
		return soap_in_PointerToxsd__boolean(soap, NULL, NULL, "xsd:boolean");
	    case SOAP_TYPE_PointerTostring:
		return soap_in_PointerTostring(soap, NULL, NULL, "xsd:string");
	    case SOAP_TYPE__QName:
		{	char **s;
		    s = soap_in__QName(soap, NULL, NULL, "xsd:QName");
		    return s ? *s : NULL;
		}
	    case SOAP_TYPE_string:
		{	char **s;
		    s = soap_in_string(soap, NULL, NULL, "xsd:string");
		    return s ? *s : NULL;
		}
	    default:
#endif
		{	const char *t = soap->type;
		    if (!*t)
			t = soap->tag;
		    if (!soap_match_tag(soap, t, "xsd:byte"))
		    {	*type = SOAP_TYPE_byte;
			return soap_in_byte(soap, NULL, NULL, NULL);
		    }
		    if (!soap_match_tag(soap, t, "xsd:int"))
		    {	*type = SOAP_TYPE_int;
			return soap_in_int(soap, NULL, NULL, NULL);
		    }
		    if (!soap_match_tag(soap, t, "xsd:long"))
		    {	*type = SOAP_TYPE_LONG64;
			return soap_in_LONG64(soap, NULL, NULL, NULL);
		    }
		    if (!soap_match_tag(soap, t, "xsd:float"))
		    {	*type = SOAP_TYPE_float;
			return soap_in_float(soap, NULL, NULL, NULL);
		    }
		    if (!soap_match_tag(soap, t, "xsd:double"))
		    {	*type = SOAP_TYPE_double;
			return soap_in_double(soap, NULL, NULL, NULL);
		    }
		    if (!soap_match_tag(soap, t, "xsd:dateTime"))
		    {	*type = SOAP_TYPE_time;
			return soap_in_time(soap, NULL, NULL, NULL);
		    }
		    if (!soap_match_tag(soap, t, "xsd:boolean"))
		    {	*type = SOAP_TYPE_xsd__boolean;
			return soap_in_xsd__boolean(soap, NULL, NULL, NULL);
		    }
		    if (!soap_match_tag(soap, t, "ns2:Failure"))
		    {	*type = SOAP_TYPE_ns2__Failure;
			return soap_in_ns2__Failure(soap, NULL, NULL, NULL);
		    }
		    if (!soap_match_tag(soap, t, "ns2:ResHeader"))
		    {	*type = SOAP_TYPE_ns2__ResHeader;
			return soap_in_ns2__ResHeader(soap, NULL, NULL, NULL);
		    }
		    if (!soap_match_tag(soap, t, "ns2:AuthHeader"))
		    {	*type = SOAP_TYPE_ns2__AuthHeader;
			return soap_in_ns2__AuthHeader(soap, NULL, NULL, NULL);
		    }
		    if (!soap_match_tag(soap, t, "ns2:DoubleMapItemType"))
		    {	*type = SOAP_TYPE_ns2__DoubleMapItemType;
			return soap_in_ns2__DoubleMapItemType(soap, NULL, NULL, NULL);
		    }
		    if (!soap_match_tag(soap, t, "ns2:FloatMapItemType"))
		    {	*type = SOAP_TYPE_ns2__FloatMapItemType;
			return soap_in_ns2__FloatMapItemType(soap, NULL, NULL, NULL);
		    }
		    if (!soap_match_tag(soap, t, "ns2:LongMapItemType"))
		    {	*type = SOAP_TYPE_ns2__LongMapItemType;
			return soap_in_ns2__LongMapItemType(soap, NULL, NULL, NULL);
		    }
		    if (!soap_match_tag(soap, t, "ns2:IntMapItemType"))
		    {	*type = SOAP_TYPE_ns2__IntMapItemType;
			return soap_in_ns2__IntMapItemType(soap, NULL, NULL, NULL);
		    }
		    if (!soap_match_tag(soap, t, "ns2:StringMapItemType"))
		    {	*type = SOAP_TYPE_ns2__StringMapItemType;
			return soap_in_ns2__StringMapItemType(soap, NULL, NULL, NULL);
		    }
		    if (!soap_match_tag(soap, t, "ns2:OptType"))
		    {	*type = SOAP_TYPE_ns2__OptType;
			return soap_in_ns2__OptType(soap, NULL, NULL, NULL);
		    }
		    if (!soap_match_tag(soap, t, "ns1:AttributeType"))
		    {	*type = SOAP_TYPE_ns1__AttributeType;
			return soap_in_ns1__AttributeType(soap, NULL, NULL, NULL);
		    }
		    if (!soap_match_tag(soap, t, "ns1:ReportRequestType"))
		    {	*type = SOAP_TYPE_ns1__ReportRequestType;
			return soap_in_ns1__ReportRequestType(soap, NULL, NULL, NULL);
		    }
		    if (!soap_match_tag(soap, t, "xsd:QName"))
		    {	char **s;
			*type = SOAP_TYPE__QName;
			s = soap_in__QName(soap, NULL, NULL, NULL);
			return s ? *s : NULL;
		    }
		    if (!soap_match_tag(soap, t, "xsd:string"))
		    {	char **s;
			*type = SOAP_TYPE_string;
			s = soap_in_string(soap, NULL, NULL, NULL);
			return s ? *s : NULL;
		    }
		    t = soap->tag;
		    if (!soap_match_tag(soap, t, "ns1:getReportFileUrlResponse"))
		    {	*type = SOAP_TYPE__ns1__getReportFileUrlResponse;
			return soap_in__ns1__getReportFileUrlResponse(soap, NULL, NULL, NULL);
		    }
		    if (!soap_match_tag(soap, t, "ns1:getReportFileUrlRequest"))
		    {	*type = SOAP_TYPE__ns1__getReportFileUrlRequest;
			return soap_in__ns1__getReportFileUrlRequest(soap, NULL, NULL, NULL);
		    }
		    if (!soap_match_tag(soap, t, "ns1:getReportStateResponse"))
		    {	*type = SOAP_TYPE__ns1__getReportStateResponse;
			return soap_in__ns1__getReportStateResponse(soap, NULL, NULL, NULL);
		    }
		    if (!soap_match_tag(soap, t, "ns1:getReportStateRequest"))
		    {	*type = SOAP_TYPE__ns1__getReportStateRequest;
			return soap_in__ns1__getReportStateRequest(soap, NULL, NULL, NULL);
		    }
		    if (!soap_match_tag(soap, t, "ns1:getProfessionalReportIdResponse"))
		    {	*type = SOAP_TYPE__ns1__getProfessionalReportIdResponse;
			return soap_in__ns1__getProfessionalReportIdResponse(soap, NULL, NULL, NULL);
		    }
		    if (!soap_match_tag(soap, t, "ns1:getProfessionalReportIdRequest"))
		    {	*type = SOAP_TYPE__ns1__getProfessionalReportIdRequest;
			return soap_in__ns1__getProfessionalReportIdRequest(soap, NULL, NULL, NULL);
		    }
#ifndef WITH_NOIDREF
		}
#endif
	}
	soap->error = SOAP_TAG_MISMATCH;
	return NULL;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_ignore_element(struct soap *soap)
    {
	if (!soap_peek_element(soap))
	{	int t;
	    DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Unexpected element '%s' in input (level=%u, %d)\n", soap->tag, soap->level, soap->body));
	    if (soap->mustUnderstand && !soap->other)
		return soap->error = SOAP_MUSTUNDERSTAND;
	    if (((soap->mode & SOAP_XML_STRICT) && soap->part != SOAP_IN_HEADER) || !soap_match_tag(soap, soap->tag, "SOAP-ENV:"))
	    {	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "REJECTING element '%s'\n", soap->tag));
		return soap->error = SOAP_TAG_MISMATCH;
	    }
	    if (!*soap->id || !soap_getelement(soap, &t))
	    {	soap->peeked = 0;
		if (soap->fignore)
		    soap->error = soap->fignore(soap, soap->tag);
		else
		    soap->error = SOAP_OK;
		DBGLOG(TEST, if (!soap->error) SOAP_MESSAGE(fdebug, "IGNORING element '%s'\n", soap->tag));
		if (!soap->error && soap->body)
		{	soap->level++;
		    while (!soap_ignore_element(soap))
			;
		    if (soap->error == SOAP_NO_TAG)
			soap->error = soap_element_end_in(soap, NULL);
		}
	    }
	}
	return soap->error;
    }

#ifndef WITH_NOIDREF
    SOAP_FMAC3 int SOAP_FMAC4 soap_putindependent(struct soap *soap)
    {
	int i;
	struct soap_plist *pp;
	if (soap->version == 1 && soap->encodingStyle && !(soap->mode & (SOAP_XML_TREE | SOAP_XML_GRAPH)))
	    for (i = 0; i < SOAP_PTRHASH; i++)
		for (pp = soap->pht[i]; pp; pp = pp->next)
		    if (pp->mark1 == 2 || pp->mark2 == 2)
			if (soap_putelement(soap, pp->ptr, "id", pp->id, pp->type))
			    return soap->error;
	return SOAP_OK;
    }
#endif
    SOAP_FMAC3 int SOAP_FMAC4 soap_putelement(struct soap *soap, const void *ptr, const char *tag, int id, int type)
    {	(void)tag;
	switch (type)
	{
	    case SOAP_TYPE_byte:
		return soap_out_byte(soap, tag, id, (const char *)ptr, "xsd:byte");
	    case SOAP_TYPE_int:
		return soap_out_int(soap, tag, id, (const int *)ptr, "xsd:int");
	    case SOAP_TYPE_LONG64:
		return soap_out_LONG64(soap, tag, id, (const LONG64 *)ptr, "xsd:long");
	    case SOAP_TYPE_float:
		return soap_out_float(soap, tag, id, (const float *)ptr, "xsd:float");
	    case SOAP_TYPE_double:
		return soap_out_double(soap, tag, id, (const double *)ptr, "xsd:double");
	    case SOAP_TYPE_time:
		return soap_out_time(soap, tag, id, (const time_t *)ptr, "xsd:dateTime");
	    case SOAP_TYPE_xsd__boolean:
		return soap_out_xsd__boolean(soap, tag, id, (const enum xsd__boolean *)ptr, "xsd:boolean");
	    case SOAP_TYPE_ns2__Failure:
		return soap_out_ns2__Failure(soap, tag, id, (const struct ns2__Failure *)ptr, "ns2:Failure");
	    case SOAP_TYPE_ns2__ResHeader:
		return soap_out_ns2__ResHeader(soap, tag, id, (const struct ns2__ResHeader *)ptr, "ns2:ResHeader");
	    case SOAP_TYPE_ns2__AuthHeader:
		return soap_out_ns2__AuthHeader(soap, tag, id, (const struct ns2__AuthHeader *)ptr, "ns2:AuthHeader");
	    case SOAP_TYPE_ns2__DoubleMapItemType:
		return soap_out_ns2__DoubleMapItemType(soap, tag, id, (const struct ns2__DoubleMapItemType *)ptr, "ns2:DoubleMapItemType");
	    case SOAP_TYPE_ns2__FloatMapItemType:
		return soap_out_ns2__FloatMapItemType(soap, tag, id, (const struct ns2__FloatMapItemType *)ptr, "ns2:FloatMapItemType");
	    case SOAP_TYPE_ns2__LongMapItemType:
		return soap_out_ns2__LongMapItemType(soap, tag, id, (const struct ns2__LongMapItemType *)ptr, "ns2:LongMapItemType");
	    case SOAP_TYPE_ns2__IntMapItemType:
		return soap_out_ns2__IntMapItemType(soap, tag, id, (const struct ns2__IntMapItemType *)ptr, "ns2:IntMapItemType");
	    case SOAP_TYPE_ns2__StringMapItemType:
		return soap_out_ns2__StringMapItemType(soap, tag, id, (const struct ns2__StringMapItemType *)ptr, "ns2:StringMapItemType");
	    case SOAP_TYPE_ns2__OptType:
		return soap_out_ns2__OptType(soap, tag, id, (const struct ns2__OptType *)ptr, "ns2:OptType");
	    case SOAP_TYPE__ns1__getReportFileUrlResponse:
		return soap_out__ns1__getReportFileUrlResponse(soap, "ns1:getReportFileUrlResponse", id, (const struct _ns1__getReportFileUrlResponse *)ptr, NULL);
	    case SOAP_TYPE__ns1__getReportFileUrlRequest:
		return soap_out__ns1__getReportFileUrlRequest(soap, "ns1:getReportFileUrlRequest", id, (const struct _ns1__getReportFileUrlRequest *)ptr, NULL);
	    case SOAP_TYPE__ns1__getReportStateResponse:
		return soap_out__ns1__getReportStateResponse(soap, "ns1:getReportStateResponse", id, (const struct _ns1__getReportStateResponse *)ptr, NULL);
	    case SOAP_TYPE__ns1__getReportStateRequest:
		return soap_out__ns1__getReportStateRequest(soap, "ns1:getReportStateRequest", id, (const struct _ns1__getReportStateRequest *)ptr, NULL);
	    case SOAP_TYPE__ns1__getProfessionalReportIdResponse:
		return soap_out__ns1__getProfessionalReportIdResponse(soap, "ns1:getProfessionalReportIdResponse", id, (const struct _ns1__getProfessionalReportIdResponse *)ptr, NULL);
	    case SOAP_TYPE__ns1__getProfessionalReportIdRequest:
		return soap_out__ns1__getProfessionalReportIdRequest(soap, "ns1:getProfessionalReportIdRequest", id, (const struct _ns1__getProfessionalReportIdRequest *)ptr, NULL);
	    case SOAP_TYPE_ns1__AttributeType:
		return soap_out_ns1__AttributeType(soap, tag, id, (const struct ns1__AttributeType *)ptr, "ns1:AttributeType");
	    case SOAP_TYPE_ns1__ReportRequestType:
		return soap_out_ns1__ReportRequestType(soap, tag, id, (const struct ns1__ReportRequestType *)ptr, "ns1:ReportRequestType");
	    case SOAP_TYPE_PointerTo_ns1__getReportFileUrlResponse:
		return soap_out_PointerTo_ns1__getReportFileUrlResponse(soap, tag, id, (struct _ns1__getReportFileUrlResponse *const*)ptr, "ns1:getReportFileUrlResponse");
	    case SOAP_TYPE_PointerTo_ns1__getReportFileUrlRequest:
		return soap_out_PointerTo_ns1__getReportFileUrlRequest(soap, tag, id, (struct _ns1__getReportFileUrlRequest *const*)ptr, "ns1:getReportFileUrlRequest");
	    case SOAP_TYPE_PointerTo_ns1__getReportStateResponse:
		return soap_out_PointerTo_ns1__getReportStateResponse(soap, tag, id, (struct _ns1__getReportStateResponse *const*)ptr, "ns1:getReportStateResponse");
	    case SOAP_TYPE_PointerTo_ns1__getReportStateRequest:
		return soap_out_PointerTo_ns1__getReportStateRequest(soap, tag, id, (struct _ns1__getReportStateRequest *const*)ptr, "ns1:getReportStateRequest");
	    case SOAP_TYPE_PointerTo_ns1__getProfessionalReportIdResponse:
		return soap_out_PointerTo_ns1__getProfessionalReportIdResponse(soap, tag, id, (struct _ns1__getProfessionalReportIdResponse *const*)ptr, "ns1:getProfessionalReportIdResponse");
	    case SOAP_TYPE_PointerTo_ns1__getProfessionalReportIdRequest:
		return soap_out_PointerTo_ns1__getProfessionalReportIdRequest(soap, tag, id, (struct _ns1__getProfessionalReportIdRequest *const*)ptr, "ns1:getProfessionalReportIdRequest");
	    case SOAP_TYPE_PointerTons2__ResHeader:
		return soap_out_PointerTons2__ResHeader(soap, tag, id, (struct ns2__ResHeader *const*)ptr, "ns2:ResHeader");
	    case SOAP_TYPE_PointerTons2__AuthHeader:
		return soap_out_PointerTons2__AuthHeader(soap, tag, id, (struct ns2__AuthHeader *const*)ptr, "ns2:AuthHeader");
	    case SOAP_TYPE_PointerTons2__Failure:
		return soap_out_PointerTons2__Failure(soap, tag, id, (struct ns2__Failure *const*)ptr, "ns2:Failure");
	    case SOAP_TYPE_PointerTons2__DoubleMapItemType:
		return soap_out_PointerTons2__DoubleMapItemType(soap, tag, id, (struct ns2__DoubleMapItemType *const*)ptr, "ns2:DoubleMapItemType");
	    case SOAP_TYPE_PointerTons2__FloatMapItemType:
		return soap_out_PointerTons2__FloatMapItemType(soap, tag, id, (struct ns2__FloatMapItemType *const*)ptr, "ns2:FloatMapItemType");
	    case SOAP_TYPE_PointerTons2__LongMapItemType:
		return soap_out_PointerTons2__LongMapItemType(soap, tag, id, (struct ns2__LongMapItemType *const*)ptr, "ns2:LongMapItemType");
	    case SOAP_TYPE_PointerTons2__IntMapItemType:
		return soap_out_PointerTons2__IntMapItemType(soap, tag, id, (struct ns2__IntMapItemType *const*)ptr, "ns2:IntMapItemType");
	    case SOAP_TYPE_PointerTons2__StringMapItemType:
		return soap_out_PointerTons2__StringMapItemType(soap, tag, id, (struct ns2__StringMapItemType *const*)ptr, "ns2:StringMapItemType");
	    case SOAP_TYPE_PointerTons1__ReportRequestType:
		return soap_out_PointerTons1__ReportRequestType(soap, tag, id, (struct ns1__ReportRequestType *const*)ptr, "ns1:ReportRequestType");
	    case SOAP_TYPE_PointerToLONG64:
		return soap_out_PointerToLONG64(soap, tag, id, (LONG64 *const*)ptr, "xsd:long");
	    case SOAP_TYPE_PointerTons1__AttributeType:
		return soap_out_PointerTons1__AttributeType(soap, tag, id, (struct ns1__AttributeType *const*)ptr, "ns1:AttributeType");
	    case SOAP_TYPE_PointerToint:
		return soap_out_PointerToint(soap, tag, id, (int *const*)ptr, "xsd:int");
	    case SOAP_TYPE_PointerToxsd__boolean:
		return soap_out_PointerToxsd__boolean(soap, tag, id, (enum xsd__boolean *const*)ptr, "xsd:boolean");
	    case SOAP_TYPE_PointerTostring:
		return soap_out_PointerTostring(soap, tag, id, (char **const*)ptr, "xsd:string");
	    case SOAP_TYPE__QName:
		return soap_out_string(soap, tag, id, (char*const*)&ptr, "xsd:QName");
	    case SOAP_TYPE_string:
		return soap_out_string(soap, tag, id, (char*const*)&ptr, "xsd:string");
	}
	return SOAP_OK;
    }

#ifndef WITH_NOIDREF
    SOAP_FMAC3 void SOAP_FMAC4 soap_markelement(struct soap *soap, const void *ptr, int type)
    {
	(void)soap; (void)ptr; (void)type; /* appease -Wall -Werror */
	switch (type)
	{
	    case SOAP_TYPE___ns1__getReportFileUrl:
		soap_serialize___ns1__getReportFileUrl(soap, (const struct __ns1__getReportFileUrl *)ptr);
		break;
	    case SOAP_TYPE___ns1__getReportState:
		soap_serialize___ns1__getReportState(soap, (const struct __ns1__getReportState *)ptr);
		break;
	    case SOAP_TYPE___ns1__getProfessionalReportId:
		soap_serialize___ns1__getProfessionalReportId(soap, (const struct __ns1__getProfessionalReportId *)ptr);
		break;
	    case SOAP_TYPE_ns2__Failure:
		soap_serialize_ns2__Failure(soap, (const struct ns2__Failure *)ptr);
		break;
	    case SOAP_TYPE_ns2__ResHeader:
		soap_serialize_ns2__ResHeader(soap, (const struct ns2__ResHeader *)ptr);
		break;
	    case SOAP_TYPE_ns2__AuthHeader:
		soap_serialize_ns2__AuthHeader(soap, (const struct ns2__AuthHeader *)ptr);
		break;
	    case SOAP_TYPE_ns2__DoubleMapItemType:
		soap_serialize_ns2__DoubleMapItemType(soap, (const struct ns2__DoubleMapItemType *)ptr);
		break;
	    case SOAP_TYPE_ns2__FloatMapItemType:
		soap_serialize_ns2__FloatMapItemType(soap, (const struct ns2__FloatMapItemType *)ptr);
		break;
	    case SOAP_TYPE_ns2__LongMapItemType:
		soap_serialize_ns2__LongMapItemType(soap, (const struct ns2__LongMapItemType *)ptr);
		break;
	    case SOAP_TYPE_ns2__IntMapItemType:
		soap_serialize_ns2__IntMapItemType(soap, (const struct ns2__IntMapItemType *)ptr);
		break;
	    case SOAP_TYPE_ns2__StringMapItemType:
		soap_serialize_ns2__StringMapItemType(soap, (const struct ns2__StringMapItemType *)ptr);
		break;
	    case SOAP_TYPE_ns2__OptType:
		soap_serialize_ns2__OptType(soap, (const struct ns2__OptType *)ptr);
		break;
	    case SOAP_TYPE__ns1__getReportFileUrlResponse:
		soap_serialize__ns1__getReportFileUrlResponse(soap, (const struct _ns1__getReportFileUrlResponse *)ptr);
		break;
	    case SOAP_TYPE__ns1__getReportFileUrlRequest:
		soap_serialize__ns1__getReportFileUrlRequest(soap, (const struct _ns1__getReportFileUrlRequest *)ptr);
		break;
	    case SOAP_TYPE__ns1__getReportStateResponse:
		soap_serialize__ns1__getReportStateResponse(soap, (const struct _ns1__getReportStateResponse *)ptr);
		break;
	    case SOAP_TYPE__ns1__getReportStateRequest:
		soap_serialize__ns1__getReportStateRequest(soap, (const struct _ns1__getReportStateRequest *)ptr);
		break;
	    case SOAP_TYPE__ns1__getProfessionalReportIdResponse:
		soap_serialize__ns1__getProfessionalReportIdResponse(soap, (const struct _ns1__getProfessionalReportIdResponse *)ptr);
		break;
	    case SOAP_TYPE__ns1__getProfessionalReportIdRequest:
		soap_serialize__ns1__getProfessionalReportIdRequest(soap, (const struct _ns1__getProfessionalReportIdRequest *)ptr);
		break;
	    case SOAP_TYPE_ns1__AttributeType:
		soap_serialize_ns1__AttributeType(soap, (const struct ns1__AttributeType *)ptr);
		break;
	    case SOAP_TYPE_ns1__ReportRequestType:
		soap_serialize_ns1__ReportRequestType(soap, (const struct ns1__ReportRequestType *)ptr);
		break;
	    case SOAP_TYPE_PointerTo_ns1__getReportFileUrlResponse:
		soap_serialize_PointerTo_ns1__getReportFileUrlResponse(soap, (struct _ns1__getReportFileUrlResponse *const*)ptr);
		break;
	    case SOAP_TYPE_PointerTo_ns1__getReportFileUrlRequest:
		soap_serialize_PointerTo_ns1__getReportFileUrlRequest(soap, (struct _ns1__getReportFileUrlRequest *const*)ptr);
		break;
	    case SOAP_TYPE_PointerTo_ns1__getReportStateResponse:
		soap_serialize_PointerTo_ns1__getReportStateResponse(soap, (struct _ns1__getReportStateResponse *const*)ptr);
		break;
	    case SOAP_TYPE_PointerTo_ns1__getReportStateRequest:
		soap_serialize_PointerTo_ns1__getReportStateRequest(soap, (struct _ns1__getReportStateRequest *const*)ptr);
		break;
	    case SOAP_TYPE_PointerTo_ns1__getProfessionalReportIdResponse:
		soap_serialize_PointerTo_ns1__getProfessionalReportIdResponse(soap, (struct _ns1__getProfessionalReportIdResponse *const*)ptr);
		break;
	    case SOAP_TYPE_PointerTo_ns1__getProfessionalReportIdRequest:
		soap_serialize_PointerTo_ns1__getProfessionalReportIdRequest(soap, (struct _ns1__getProfessionalReportIdRequest *const*)ptr);
		break;
	    case SOAP_TYPE_PointerTons2__ResHeader:
		soap_serialize_PointerTons2__ResHeader(soap, (struct ns2__ResHeader *const*)ptr);
		break;
	    case SOAP_TYPE_PointerTons2__AuthHeader:
		soap_serialize_PointerTons2__AuthHeader(soap, (struct ns2__AuthHeader *const*)ptr);
		break;
	    case SOAP_TYPE_PointerTons2__Failure:
		soap_serialize_PointerTons2__Failure(soap, (struct ns2__Failure *const*)ptr);
		break;
	    case SOAP_TYPE_PointerTons2__DoubleMapItemType:
		soap_serialize_PointerTons2__DoubleMapItemType(soap, (struct ns2__DoubleMapItemType *const*)ptr);
		break;
	    case SOAP_TYPE_PointerTons2__FloatMapItemType:
		soap_serialize_PointerTons2__FloatMapItemType(soap, (struct ns2__FloatMapItemType *const*)ptr);
		break;
	    case SOAP_TYPE_PointerTons2__LongMapItemType:
		soap_serialize_PointerTons2__LongMapItemType(soap, (struct ns2__LongMapItemType *const*)ptr);
		break;
	    case SOAP_TYPE_PointerTons2__IntMapItemType:
		soap_serialize_PointerTons2__IntMapItemType(soap, (struct ns2__IntMapItemType *const*)ptr);
		break;
	    case SOAP_TYPE_PointerTons2__StringMapItemType:
		soap_serialize_PointerTons2__StringMapItemType(soap, (struct ns2__StringMapItemType *const*)ptr);
		break;
	    case SOAP_TYPE_PointerTons1__ReportRequestType:
		soap_serialize_PointerTons1__ReportRequestType(soap, (struct ns1__ReportRequestType *const*)ptr);
		break;
	    case SOAP_TYPE_PointerToLONG64:
		soap_serialize_PointerToLONG64(soap, (LONG64 *const*)ptr);
		break;
	    case SOAP_TYPE_PointerTons1__AttributeType:
		soap_serialize_PointerTons1__AttributeType(soap, (struct ns1__AttributeType *const*)ptr);
		break;
	    case SOAP_TYPE_PointerToint:
		soap_serialize_PointerToint(soap, (int *const*)ptr);
		break;
	    case SOAP_TYPE_PointerToxsd__boolean:
		soap_serialize_PointerToxsd__boolean(soap, (enum xsd__boolean *const*)ptr);
		break;
	    case SOAP_TYPE_PointerTostring:
		soap_serialize_PointerTostring(soap, (char **const*)ptr);
		break;
	    case SOAP_TYPE__QName:
		soap_serialize_string(soap, (char*const*)&ptr);
		break;
	    case SOAP_TYPE_string:
		soap_serialize_string(soap, (char*const*)&ptr);
		break;
	}
    }
#endif

    SOAP_FMAC3 void SOAP_FMAC4 soap_default_byte(struct soap *soap, char *a)
    {
	(void)soap; /* appease -Wall -Werror */
#ifdef SOAP_DEFAULT_byte
	*a = SOAP_DEFAULT_byte;
#else
	*a = (char)0;
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_byte(struct soap *soap, const char *tag, int id, const char *a, const char *type)
    {	(void)soap; (void)type; (void)tag; (void)id;
	return soap_outbyte(soap, tag, id, a, type, SOAP_TYPE_byte);
    }

    SOAP_FMAC3 char * SOAP_FMAC4 soap_in_byte(struct soap *soap, const char *tag, char *a, const char *type)
    {	char *p;
	p = soap_inbyte(soap, tag, a, type, SOAP_TYPE_byte);
	return p;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_byte(struct soap *soap, const char *a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_byte);
	if (soap_out_byte(soap, tag?tag:"byte", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 char * SOAP_FMAC4 soap_get_byte(struct soap *soap, char *p, const char *tag, const char *type)
    {
	if ((p = soap_in_byte(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_default_int(struct soap *soap, int *a)
    {
	(void)soap; /* appease -Wall -Werror */
#ifdef SOAP_DEFAULT_int
	*a = SOAP_DEFAULT_int;
#else
	*a = (int)0;
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_int(struct soap *soap, const char *tag, int id, const int *a, const char *type)
    {	(void)soap; (void)type; (void)tag; (void)id;
	return soap_outint(soap, tag, id, a, type, SOAP_TYPE_int);
    }

    SOAP_FMAC3 int * SOAP_FMAC4 soap_in_int(struct soap *soap, const char *tag, int *a, const char *type)
    {	int *p;
	p = soap_inint(soap, tag, a, type, SOAP_TYPE_int);
	return p;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_int(struct soap *soap, const int *a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_int);
	if (soap_out_int(soap, tag?tag:"int", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 int * SOAP_FMAC4 soap_get_int(struct soap *soap, int *p, const char *tag, const char *type)
    {
	if ((p = soap_in_int(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_default_LONG64(struct soap *soap, LONG64 *a)
    {
	(void)soap; /* appease -Wall -Werror */
#ifdef SOAP_DEFAULT_LONG64
	*a = SOAP_DEFAULT_LONG64;
#else
	*a = (LONG64)0;
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_LONG64(struct soap *soap, const char *tag, int id, const LONG64 *a, const char *type)
    {	(void)soap; (void)type; (void)tag; (void)id;
	return soap_outLONG64(soap, tag, id, a, type, SOAP_TYPE_LONG64);
    }

    SOAP_FMAC3 LONG64 * SOAP_FMAC4 soap_in_LONG64(struct soap *soap, const char *tag, LONG64 *a, const char *type)
    {	LONG64 *p;
	p = soap_inLONG64(soap, tag, a, type, SOAP_TYPE_LONG64);
	return p;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_LONG64(struct soap *soap, const LONG64 *a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_LONG64);
	if (soap_out_LONG64(soap, tag?tag:"long", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 LONG64 * SOAP_FMAC4 soap_get_LONG64(struct soap *soap, LONG64 *p, const char *tag, const char *type)
    {
	if ((p = soap_in_LONG64(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_default_float(struct soap *soap, float *a)
    {
	(void)soap; /* appease -Wall -Werror */
#ifdef SOAP_DEFAULT_float
	*a = SOAP_DEFAULT_float;
#else
	*a = (float)0;
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_float(struct soap *soap, const char *tag, int id, const float *a, const char *type)
    {	(void)soap; (void)type; (void)tag; (void)id;
	return soap_outfloat(soap, tag, id, a, type, SOAP_TYPE_float);
    }

    SOAP_FMAC3 float * SOAP_FMAC4 soap_in_float(struct soap *soap, const char *tag, float *a, const char *type)
    {	float *p;
	p = soap_infloat(soap, tag, a, type, SOAP_TYPE_float);
	return p;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_float(struct soap *soap, const float *a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_float);
	if (soap_out_float(soap, tag?tag:"float", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 float * SOAP_FMAC4 soap_get_float(struct soap *soap, float *p, const char *tag, const char *type)
    {
	if ((p = soap_in_float(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_default_double(struct soap *soap, double *a)
    {
	(void)soap; /* appease -Wall -Werror */
#ifdef SOAP_DEFAULT_double
	*a = SOAP_DEFAULT_double;
#else
	*a = (double)0;
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_double(struct soap *soap, const char *tag, int id, const double *a, const char *type)
    {	(void)soap; (void)type; (void)tag; (void)id;
	return soap_outdouble(soap, tag, id, a, type, SOAP_TYPE_double);
    }

    SOAP_FMAC3 double * SOAP_FMAC4 soap_in_double(struct soap *soap, const char *tag, double *a, const char *type)
    {	double *p;
	p = soap_indouble(soap, tag, a, type, SOAP_TYPE_double);
	return p;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_double(struct soap *soap, const double *a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_double);
	if (soap_out_double(soap, tag?tag:"double", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 double * SOAP_FMAC4 soap_get_double(struct soap *soap, double *p, const char *tag, const char *type)
    {
	if ((p = soap_in_double(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_default_time(struct soap *soap, time_t *a)
    {
	(void)soap; /* appease -Wall -Werror */
#ifdef SOAP_DEFAULT_time
	*a = SOAP_DEFAULT_time;
#else
	*a = (time_t)0;
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_time(struct soap *soap, const char *tag, int id, const time_t *a, const char *type)
    {	(void)soap; (void)type; (void)tag; (void)id;
	return soap_outdateTime(soap, tag, id, a, type, SOAP_TYPE_time);
    }

    SOAP_FMAC3 time_t * SOAP_FMAC4 soap_in_time(struct soap *soap, const char *tag, time_t *a, const char *type)
    {	time_t *p;
	p = soap_indateTime(soap, tag, a, type, SOAP_TYPE_time);
	return p;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_time(struct soap *soap, const time_t *a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_time);
	if (soap_out_time(soap, tag?tag:"dateTime", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 time_t * SOAP_FMAC4 soap_get_time(struct soap *soap, time_t *p, const char *tag, const char *type)
    {
	if ((p = soap_in_time(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_default_xsd__boolean(struct soap *soap, enum xsd__boolean *a)
    {
	(void)soap; /* appease -Wall -Werror */
#ifdef SOAP_DEFAULT_xsd__boolean
	*a = SOAP_DEFAULT_xsd__boolean;
#else
	*a = (enum xsd__boolean)0;
#endif
    }

    static const struct soap_code_map soap_codes_xsd__boolean[] =
    {	{ (long)xsd__boolean__false_, "false" },
	{ (long)xsd__boolean__true_, "true" },
	{ 0, NULL }
    };

    SOAP_FMAC3S const char* SOAP_FMAC4S soap_xsd__boolean2s(struct soap *soap, enum xsd__boolean n)
    {
	(void)soap; /* appease -Wall -Werror */
	return soap_code_str(soap_codes_xsd__boolean, n!=0);
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_xsd__boolean(struct soap *soap, const char *tag, int id, const enum xsd__boolean *a, const char *type)
    {	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_xsd__boolean), type) || soap_send(soap, soap_xsd__boolean2s(soap, *a)))
	return soap->error;
	return soap_element_end_out(soap, tag);
    }

    SOAP_FMAC3S int SOAP_FMAC4S soap_s2xsd__boolean(struct soap *soap, const char *s, enum xsd__boolean *a)
    {
	const struct soap_code_map *map;
	if (!s)
	    return soap->error;
	map = soap_code(soap_codes_xsd__boolean, s);
	if (map)
	    *a = (enum xsd__boolean)(map->code != 0);
	else
	{	long n;
	    if (soap_s2long(soap, s, &n) || n < 0 || n > 1)
		return soap->error = SOAP_TYPE;
	    *a = (enum xsd__boolean)(n != 0);
	}
	return SOAP_OK;
    }

    SOAP_FMAC3 enum xsd__boolean * SOAP_FMAC4 soap_in_xsd__boolean(struct soap *soap, const char *tag, enum xsd__boolean *a, const char *type)
    {
	if (soap_element_begin_in(soap, tag, 0, NULL))
	    return NULL;
	if (*soap->type && soap_match_tag(soap, soap->type, type) && soap_match_tag(soap, soap->type, ":boolean"))
	{	soap->error = SOAP_TYPE;
	    return NULL;
	}
	a = (enum xsd__boolean *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_xsd__boolean, sizeof(enum xsd__boolean), 0, NULL, NULL, NULL);
	if (!a)
	    return NULL;
	if (soap->body && !*soap->href)
	{	if (!a || soap_s2xsd__boolean(soap, soap_value(soap), a) || soap_element_end_in(soap, tag))
	    return NULL;
	}
	else
	{	a = (enum xsd__boolean *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_xsd__boolean, 0, sizeof(enum xsd__boolean), 0, NULL);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_xsd__boolean(struct soap *soap, const enum xsd__boolean *a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_xsd__boolean);
	if (soap_out_xsd__boolean(soap, tag?tag:"xsd:boolean", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 enum xsd__boolean * SOAP_FMAC4 soap_get_xsd__boolean(struct soap *soap, enum xsd__boolean *p, const char *tag, const char *type)
    {
	if ((p = soap_in_xsd__boolean(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

#ifndef WITH_NOGLOBAL

    SOAP_FMAC3 void SOAP_FMAC4 soap_default_SOAP_ENV__Fault(struct soap *soap, struct SOAP_ENV__Fault *a)
    {
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default__QName(soap, &a->faultcode);
	soap_default_string(soap, &a->faultstring);
	soap_default_string(soap, &a->faultactor);
	a->detail = NULL;
	a->SOAP_ENV__Code = NULL;
	a->SOAP_ENV__Reason = NULL;
	soap_default_string(soap, &a->SOAP_ENV__Node);
	soap_default_string(soap, &a->SOAP_ENV__Role);
	a->SOAP_ENV__Detail = NULL;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_SOAP_ENV__Fault(struct soap *soap, const struct SOAP_ENV__Fault *a)
    {
#ifndef WITH_NOIDREF
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize__QName(soap, &a->faultcode);
	soap_serialize_string(soap, &a->faultstring);
	soap_serialize_string(soap, &a->faultactor);
	soap_serialize_PointerToSOAP_ENV__Detail(soap, &a->detail);
	soap_serialize_PointerToSOAP_ENV__Code(soap, &a->SOAP_ENV__Code);
	soap_serialize_PointerToSOAP_ENV__Reason(soap, &a->SOAP_ENV__Reason);
	soap_serialize_string(soap, &a->SOAP_ENV__Node);
	soap_serialize_string(soap, &a->SOAP_ENV__Role);
	soap_serialize_PointerToSOAP_ENV__Detail(soap, &a->SOAP_ENV__Detail);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_SOAP_ENV__Fault(struct soap *soap, const char *tag, int id, const struct SOAP_ENV__Fault *a, const char *type)
    {
	const char *soap_tmp_faultcode = soap_QName2s(soap, a->faultcode);
	(void)soap; (void)tag; (void)id; (void)type;
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_SOAP_ENV__Fault), type))
	    return soap->error;
	if (soap_out__QName(soap, "faultcode", -1, (char*const*)&soap_tmp_faultcode, ""))
	    return soap->error;
	if (soap_out_string(soap, "faultstring", -1, &a->faultstring, ""))
	    return soap->error;
	if (soap_out_string(soap, "faultactor", -1, &a->faultactor, ""))
	    return soap->error;
	if (soap_out_PointerToSOAP_ENV__Detail(soap, "detail", -1, &a->detail, ""))
	    return soap->error;
	if (soap_out_PointerToSOAP_ENV__Code(soap, "SOAP-ENV:Code", -1, &a->SOAP_ENV__Code, ""))
	    return soap->error;
	if (soap_out_PointerToSOAP_ENV__Reason(soap, "SOAP-ENV:Reason", -1, &a->SOAP_ENV__Reason, ""))
	    return soap->error;
	if (soap_out_string(soap, "SOAP-ENV:Node", -1, &a->SOAP_ENV__Node, ""))
	    return soap->error;
	if (soap_out_string(soap, "SOAP-ENV:Role", -1, &a->SOAP_ENV__Role, ""))
	    return soap->error;
	if (soap_out_PointerToSOAP_ENV__Detail(soap, "SOAP-ENV:Detail", -1, &a->SOAP_ENV__Detail, ""))
	    return soap->error;
	return soap_element_end_out(soap, tag);
    }

    SOAP_FMAC3 struct SOAP_ENV__Fault * SOAP_FMAC4 soap_in_SOAP_ENV__Fault(struct soap *soap, const char *tag, struct SOAP_ENV__Fault *a, const char *type)
    {
	size_t soap_flag_faultcode = 1;
	size_t soap_flag_faultstring = 1;
	size_t soap_flag_faultactor = 1;
	size_t soap_flag_detail = 1;
	size_t soap_flag_SOAP_ENV__Code = 1;
	size_t soap_flag_SOAP_ENV__Reason = 1;
	size_t soap_flag_SOAP_ENV__Node = 1;
	size_t soap_flag_SOAP_ENV__Role = 1;
	size_t soap_flag_SOAP_ENV__Detail = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
	    return NULL;
	a = (struct SOAP_ENV__Fault *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_SOAP_ENV__Fault, sizeof(struct SOAP_ENV__Fault), 0, NULL, NULL, NULL);
	if (!a)
	    return NULL;
	soap_default_SOAP_ENV__Fault(soap, a);
	if (soap->body && !*soap->href)
	{
	    for (;;)
	    {	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_faultcode && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
		    if (soap_in__QName(soap, "faultcode", &a->faultcode, ""))
		    {	soap_flag_faultcode--;
			continue;
		    }
		if (soap_flag_faultstring && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
		    if (soap_in_string(soap, "faultstring", &a->faultstring, "xsd:string"))
		    {	soap_flag_faultstring--;
			continue;
		    }
		if (soap_flag_faultactor && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
		    if (soap_in_string(soap, "faultactor", &a->faultactor, "xsd:string"))
		    {	soap_flag_faultactor--;
			continue;
		    }
		if (soap_flag_detail && soap->error == SOAP_TAG_MISMATCH)
		    if (soap_in_PointerToSOAP_ENV__Detail(soap, "detail", &a->detail, ""))
		    {	soap_flag_detail--;
			continue;
		    }
		if (soap_flag_SOAP_ENV__Code && soap->error == SOAP_TAG_MISMATCH)
		    if (soap_in_PointerToSOAP_ENV__Code(soap, "SOAP-ENV:Code", &a->SOAP_ENV__Code, ""))
		    {	soap_flag_SOAP_ENV__Code--;
			continue;
		    }
		if (soap_flag_SOAP_ENV__Reason && soap->error == SOAP_TAG_MISMATCH)
		    if (soap_in_PointerToSOAP_ENV__Reason(soap, "SOAP-ENV:Reason", &a->SOAP_ENV__Reason, ""))
		    {	soap_flag_SOAP_ENV__Reason--;
			continue;
		    }
		if (soap_flag_SOAP_ENV__Node && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
		    if (soap_in_string(soap, "SOAP-ENV:Node", &a->SOAP_ENV__Node, "xsd:string"))
		    {	soap_flag_SOAP_ENV__Node--;
			continue;
		    }
		if (soap_flag_SOAP_ENV__Role && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
		    if (soap_in_string(soap, "SOAP-ENV:Role", &a->SOAP_ENV__Role, "xsd:string"))
		    {	soap_flag_SOAP_ENV__Role--;
			continue;
		    }
		if (soap_flag_SOAP_ENV__Detail && soap->error == SOAP_TAG_MISMATCH)
		    if (soap_in_PointerToSOAP_ENV__Detail(soap, "SOAP-ENV:Detail", &a->SOAP_ENV__Detail, ""))
		    {	soap_flag_SOAP_ENV__Detail--;
			continue;
		    }
		if (soap->error == SOAP_TAG_MISMATCH)
		    soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
		    break;
		if (soap->error)
		    return NULL;
	    }
	    if (soap_element_end_in(soap, tag))
		return NULL;
	}
	else
	{	a = (struct SOAP_ENV__Fault *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_SOAP_ENV__Fault, 0, sizeof(struct SOAP_ENV__Fault), 0, NULL);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_SOAP_ENV__Fault(struct soap *soap, const struct SOAP_ENV__Fault *a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_SOAP_ENV__Fault);
	if (soap_out_SOAP_ENV__Fault(soap, tag?tag:"SOAP-ENV:Fault", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 struct SOAP_ENV__Fault * SOAP_FMAC4 soap_get_SOAP_ENV__Fault(struct soap *soap, struct SOAP_ENV__Fault *p, const char *tag, const char *type)
    {
	if ((p = soap_in_SOAP_ENV__Fault(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

#endif

#ifndef WITH_NOGLOBAL

    SOAP_FMAC3 void SOAP_FMAC4 soap_default_SOAP_ENV__Reason(struct soap *soap, struct SOAP_ENV__Reason *a)
    {
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_string(soap, &a->SOAP_ENV__Text);
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_SOAP_ENV__Reason(struct soap *soap, const struct SOAP_ENV__Reason *a)
    {
#ifndef WITH_NOIDREF
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_string(soap, &a->SOAP_ENV__Text);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_SOAP_ENV__Reason(struct soap *soap, const char *tag, int id, const struct SOAP_ENV__Reason *a, const char *type)
    {
	(void)soap; (void)tag; (void)id; (void)type;
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_SOAP_ENV__Reason), type))
	    return soap->error;
	if (soap->lang)
	    soap_set_attr(soap, "xml:lang", soap->lang, 1);
	if (soap_out_string(soap, "SOAP-ENV:Text", -1, &a->SOAP_ENV__Text, ""))
	    return soap->error;
	return soap_element_end_out(soap, tag);
    }

    SOAP_FMAC3 struct SOAP_ENV__Reason * SOAP_FMAC4 soap_in_SOAP_ENV__Reason(struct soap *soap, const char *tag, struct SOAP_ENV__Reason *a, const char *type)
    {
	size_t soap_flag_SOAP_ENV__Text = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
	    return NULL;
	a = (struct SOAP_ENV__Reason *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_SOAP_ENV__Reason, sizeof(struct SOAP_ENV__Reason), 0, NULL, NULL, NULL);
	if (!a)
	    return NULL;
	soap_default_SOAP_ENV__Reason(soap, a);
	if (soap->body && !*soap->href)
	{
	    for (;;)
	    {	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_SOAP_ENV__Text && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
		    if (soap_in_string(soap, "SOAP-ENV:Text", &a->SOAP_ENV__Text, "xsd:string"))
		    {	soap_flag_SOAP_ENV__Text--;
			continue;
		    }
		if (soap->error == SOAP_TAG_MISMATCH)
		    soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
		    break;
		if (soap->error)
		    return NULL;
	    }
	    if (soap_element_end_in(soap, tag))
		return NULL;
	}
	else
	{	a = (struct SOAP_ENV__Reason *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_SOAP_ENV__Reason, 0, sizeof(struct SOAP_ENV__Reason), 0, NULL);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_SOAP_ENV__Reason(struct soap *soap, const struct SOAP_ENV__Reason *a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_SOAP_ENV__Reason);
	if (soap_out_SOAP_ENV__Reason(soap, tag?tag:"SOAP-ENV:Reason", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 struct SOAP_ENV__Reason * SOAP_FMAC4 soap_get_SOAP_ENV__Reason(struct soap *soap, struct SOAP_ENV__Reason *p, const char *tag, const char *type)
    {
	if ((p = soap_in_SOAP_ENV__Reason(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

#endif

#ifndef WITH_NOGLOBAL

    SOAP_FMAC3 void SOAP_FMAC4 soap_default_SOAP_ENV__Detail(struct soap *soap, struct SOAP_ENV__Detail *a)
    {
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->__any = NULL;
	a->__type = 0;
	a->fault = NULL;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_SOAP_ENV__Detail(struct soap *soap, const struct SOAP_ENV__Detail *a)
    {
#ifndef WITH_NOIDREF
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_markelement(soap, a->fault, a->__type);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_SOAP_ENV__Detail(struct soap *soap, const char *tag, int id, const struct SOAP_ENV__Detail *a, const char *type)
    {
	(void)soap; (void)tag; (void)id; (void)type;
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_SOAP_ENV__Detail), type))
	    return soap->error;
	soap_outliteral(soap, "-any", &a->__any, NULL);
	if (soap_putelement(soap, a->fault, "fault", -1, a->__type))
	    return soap->error;
	return soap_element_end_out(soap, tag);
    }

    SOAP_FMAC3 struct SOAP_ENV__Detail * SOAP_FMAC4 soap_in_SOAP_ENV__Detail(struct soap *soap, const char *tag, struct SOAP_ENV__Detail *a, const char *type)
    {
	size_t soap_flag___any = 1;
	size_t soap_flag_fault = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
	    return NULL;
	a = (struct SOAP_ENV__Detail *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_SOAP_ENV__Detail, sizeof(struct SOAP_ENV__Detail), 0, NULL, NULL, NULL);
	if (!a)
	    return NULL;
	soap_default_SOAP_ENV__Detail(soap, a);
	if (soap->body && !*soap->href)
	{
	    for (;;)
	    {	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_fault && soap->error == SOAP_TAG_MISMATCH)
		    if ((a->fault = soap_getelement(soap, &a->__type)))
		    {	soap_flag_fault = 0;
			continue;
		    }
		if (soap_flag___any && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
		    if (soap_inliteral(soap, "-any", &a->__any))
		    {	soap_flag___any--;
			continue;
		    }
		if (soap->error == SOAP_TAG_MISMATCH)
		    soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
		    break;
		if (soap->error)
		    return NULL;
	    }
	    if (soap_element_end_in(soap, tag))
		return NULL;
	}
	else
	{	a = (struct SOAP_ENV__Detail *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_SOAP_ENV__Detail, 0, sizeof(struct SOAP_ENV__Detail), 0, NULL);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_SOAP_ENV__Detail(struct soap *soap, const struct SOAP_ENV__Detail *a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_SOAP_ENV__Detail);
	if (soap_out_SOAP_ENV__Detail(soap, tag?tag:"SOAP-ENV:Detail", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 struct SOAP_ENV__Detail * SOAP_FMAC4 soap_get_SOAP_ENV__Detail(struct soap *soap, struct SOAP_ENV__Detail *p, const char *tag, const char *type)
    {
	if ((p = soap_in_SOAP_ENV__Detail(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

#endif

#ifndef WITH_NOGLOBAL

    SOAP_FMAC3 void SOAP_FMAC4 soap_default_SOAP_ENV__Code(struct soap *soap, struct SOAP_ENV__Code *a)
    {
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default__QName(soap, &a->SOAP_ENV__Value);
	a->SOAP_ENV__Subcode = NULL;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_SOAP_ENV__Code(struct soap *soap, const struct SOAP_ENV__Code *a)
    {
#ifndef WITH_NOIDREF
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize__QName(soap, &a->SOAP_ENV__Value);
	soap_serialize_PointerToSOAP_ENV__Code(soap, &a->SOAP_ENV__Subcode);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_SOAP_ENV__Code(struct soap *soap, const char *tag, int id, const struct SOAP_ENV__Code *a, const char *type)
    {
	const char *soap_tmp_SOAP_ENV__Value = soap_QName2s(soap, a->SOAP_ENV__Value);
	(void)soap; (void)tag; (void)id; (void)type;
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_SOAP_ENV__Code), type))
	    return soap->error;
	if (soap_out__QName(soap, "SOAP-ENV:Value", -1, (char*const*)&soap_tmp_SOAP_ENV__Value, ""))
	    return soap->error;
	if (soap_out_PointerToSOAP_ENV__Code(soap, "SOAP-ENV:Subcode", -1, &a->SOAP_ENV__Subcode, ""))
	    return soap->error;
	return soap_element_end_out(soap, tag);
    }

    SOAP_FMAC3 struct SOAP_ENV__Code * SOAP_FMAC4 soap_in_SOAP_ENV__Code(struct soap *soap, const char *tag, struct SOAP_ENV__Code *a, const char *type)
    {
	size_t soap_flag_SOAP_ENV__Value = 1;
	size_t soap_flag_SOAP_ENV__Subcode = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
	    return NULL;
	a = (struct SOAP_ENV__Code *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_SOAP_ENV__Code, sizeof(struct SOAP_ENV__Code), 0, NULL, NULL, NULL);
	if (!a)
	    return NULL;
	soap_default_SOAP_ENV__Code(soap, a);
	if (soap->body && !*soap->href)
	{
	    for (;;)
	    {	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_SOAP_ENV__Value && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
		    if (soap_in__QName(soap, "SOAP-ENV:Value", &a->SOAP_ENV__Value, ""))
		    {	soap_flag_SOAP_ENV__Value--;
			continue;
		    }
		if (soap_flag_SOAP_ENV__Subcode && soap->error == SOAP_TAG_MISMATCH)
		    if (soap_in_PointerToSOAP_ENV__Code(soap, "SOAP-ENV:Subcode", &a->SOAP_ENV__Subcode, ""))
		    {	soap_flag_SOAP_ENV__Subcode--;
			continue;
		    }
		if (soap->error == SOAP_TAG_MISMATCH)
		    soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
		    break;
		if (soap->error)
		    return NULL;
	    }
	    if (soap_element_end_in(soap, tag))
		return NULL;
	}
	else
	{	a = (struct SOAP_ENV__Code *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_SOAP_ENV__Code, 0, sizeof(struct SOAP_ENV__Code), 0, NULL);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_SOAP_ENV__Code(struct soap *soap, const struct SOAP_ENV__Code *a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_SOAP_ENV__Code);
	if (soap_out_SOAP_ENV__Code(soap, tag?tag:"SOAP-ENV:Code", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 struct SOAP_ENV__Code * SOAP_FMAC4 soap_get_SOAP_ENV__Code(struct soap *soap, struct SOAP_ENV__Code *p, const char *tag, const char *type)
    {
	if ((p = soap_in_SOAP_ENV__Code(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

#endif

    SOAP_FMAC3 void SOAP_FMAC4 soap_default___ns1__getReportFileUrl(struct soap *soap, struct __ns1__getReportFileUrl *a)
    {
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->ns1__getReportFileUrlRequest = NULL;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___ns1__getReportFileUrl(struct soap *soap, const struct __ns1__getReportFileUrl *a)
    {
#ifndef WITH_NOIDREF
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTo_ns1__getReportFileUrlRequest(soap, &a->ns1__getReportFileUrlRequest);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out___ns1__getReportFileUrl(struct soap *soap, const char *tag, int id, const struct __ns1__getReportFileUrl *a, const char *type)
    {
	(void)soap; (void)tag; (void)id; (void)type;
	if (soap_out_PointerTo_ns1__getReportFileUrlRequest(soap, "ns1:getReportFileUrlRequest", -1, &a->ns1__getReportFileUrlRequest, ""))
	    return soap->error;
	return SOAP_OK;
    }

    SOAP_FMAC3 struct __ns1__getReportFileUrl * SOAP_FMAC4 soap_in___ns1__getReportFileUrl(struct soap *soap, const char *tag, struct __ns1__getReportFileUrl *a, const char *type)
    {
	size_t soap_flag_ns1__getReportFileUrlRequest = 1;
	short soap_flag;
	a = (struct __ns1__getReportFileUrl *)soap_id_enter(soap, "", a, SOAP_TYPE___ns1__getReportFileUrl, sizeof(struct __ns1__getReportFileUrl), 0, NULL, NULL, NULL);
	if (!a)
	    return NULL;
	soap_default___ns1__getReportFileUrl(soap, a);
	for (soap_flag = 0;; soap_flag = 1)
	{	soap->error = SOAP_TAG_MISMATCH;
	    if (soap_flag_ns1__getReportFileUrlRequest && soap->error == SOAP_TAG_MISMATCH)
		if (soap_in_PointerTo_ns1__getReportFileUrlRequest(soap, "ns1:getReportFileUrlRequest", &a->ns1__getReportFileUrlRequest, ""))
		{	soap_flag_ns1__getReportFileUrlRequest--;
		    continue;
		}
	    if (soap->error == SOAP_TAG_MISMATCH)
		if (soap_flag)
		{	soap->error = SOAP_OK;
		    break;
		}
	    if (soap_flag && soap->error == SOAP_NO_TAG)
		break;
	    if (soap->error)
		return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put___ns1__getReportFileUrl(struct soap *soap, const struct __ns1__getReportFileUrl *a, const char *tag, const char *type)
    {
	register int id = 0;
	if (soap_out___ns1__getReportFileUrl(soap, tag?tag:"-ns1:getReportFileUrl", id, a, type))
	    return soap->error;
	return SOAP_OK;
    }

    SOAP_FMAC3 struct __ns1__getReportFileUrl * SOAP_FMAC4 soap_get___ns1__getReportFileUrl(struct soap *soap, struct __ns1__getReportFileUrl *p, const char *tag, const char *type)
    {
	if ((p = soap_in___ns1__getReportFileUrl(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_default___ns1__getReportState(struct soap *soap, struct __ns1__getReportState *a)
    {
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->ns1__getReportStateRequest = NULL;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___ns1__getReportState(struct soap *soap, const struct __ns1__getReportState *a)
    {
#ifndef WITH_NOIDREF
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTo_ns1__getReportStateRequest(soap, &a->ns1__getReportStateRequest);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out___ns1__getReportState(struct soap *soap, const char *tag, int id, const struct __ns1__getReportState *a, const char *type)
    {
	(void)soap; (void)tag; (void)id; (void)type;
	if (soap_out_PointerTo_ns1__getReportStateRequest(soap, "ns1:getReportStateRequest", -1, &a->ns1__getReportStateRequest, ""))
	    return soap->error;
	return SOAP_OK;
    }

    SOAP_FMAC3 struct __ns1__getReportState * SOAP_FMAC4 soap_in___ns1__getReportState(struct soap *soap, const char *tag, struct __ns1__getReportState *a, const char *type)
    {
	size_t soap_flag_ns1__getReportStateRequest = 1;
	short soap_flag;
	a = (struct __ns1__getReportState *)soap_id_enter(soap, "", a, SOAP_TYPE___ns1__getReportState, sizeof(struct __ns1__getReportState), 0, NULL, NULL, NULL);
	if (!a)
	    return NULL;
	soap_default___ns1__getReportState(soap, a);
	for (soap_flag = 0;; soap_flag = 1)
	{	soap->error = SOAP_TAG_MISMATCH;
	    if (soap_flag_ns1__getReportStateRequest && soap->error == SOAP_TAG_MISMATCH)
		if (soap_in_PointerTo_ns1__getReportStateRequest(soap, "ns1:getReportStateRequest", &a->ns1__getReportStateRequest, ""))
		{	soap_flag_ns1__getReportStateRequest--;
		    continue;
		}
	    if (soap->error == SOAP_TAG_MISMATCH)
		if (soap_flag)
		{	soap->error = SOAP_OK;
		    break;
		}
	    if (soap_flag && soap->error == SOAP_NO_TAG)
		break;
	    if (soap->error)
		return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put___ns1__getReportState(struct soap *soap, const struct __ns1__getReportState *a, const char *tag, const char *type)
    {
	register int id = 0;
	if (soap_out___ns1__getReportState(soap, tag?tag:"-ns1:getReportState", id, a, type))
	    return soap->error;
	return SOAP_OK;
    }

    SOAP_FMAC3 struct __ns1__getReportState * SOAP_FMAC4 soap_get___ns1__getReportState(struct soap *soap, struct __ns1__getReportState *p, const char *tag, const char *type)
    {
	if ((p = soap_in___ns1__getReportState(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_default___ns1__getProfessionalReportId(struct soap *soap, struct __ns1__getProfessionalReportId *a)
    {
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->ns1__getProfessionalReportIdRequest = NULL;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___ns1__getProfessionalReportId(struct soap *soap, const struct __ns1__getProfessionalReportId *a)
    {
#ifndef WITH_NOIDREF
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTo_ns1__getProfessionalReportIdRequest(soap, &a->ns1__getProfessionalReportIdRequest);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out___ns1__getProfessionalReportId(struct soap *soap, const char *tag, int id, const struct __ns1__getProfessionalReportId *a, const char *type)
    {
	(void)soap; (void)tag; (void)id; (void)type;
	if (soap_out_PointerTo_ns1__getProfessionalReportIdRequest(soap, "ns1:getProfessionalReportIdRequest", -1, &a->ns1__getProfessionalReportIdRequest, ""))
	    return soap->error;
	return SOAP_OK;
    }

    SOAP_FMAC3 struct __ns1__getProfessionalReportId * SOAP_FMAC4 soap_in___ns1__getProfessionalReportId(struct soap *soap, const char *tag, struct __ns1__getProfessionalReportId *a, const char *type)
    {
	size_t soap_flag_ns1__getProfessionalReportIdRequest = 1;
	short soap_flag;
	a = (struct __ns1__getProfessionalReportId *)soap_id_enter(soap, "", a, SOAP_TYPE___ns1__getProfessionalReportId, sizeof(struct __ns1__getProfessionalReportId), 0, NULL, NULL, NULL);
	if (!a)
	    return NULL;
	soap_default___ns1__getProfessionalReportId(soap, a);
	for (soap_flag = 0;; soap_flag = 1)
	{	soap->error = SOAP_TAG_MISMATCH;
	    if (soap_flag_ns1__getProfessionalReportIdRequest && soap->error == SOAP_TAG_MISMATCH)
		if (soap_in_PointerTo_ns1__getProfessionalReportIdRequest(soap, "ns1:getProfessionalReportIdRequest", &a->ns1__getProfessionalReportIdRequest, ""))
		{	soap_flag_ns1__getProfessionalReportIdRequest--;
		    continue;
		}
	    if (soap->error == SOAP_TAG_MISMATCH)
		if (soap_flag)
		{	soap->error = SOAP_OK;
		    break;
		}
	    if (soap_flag && soap->error == SOAP_NO_TAG)
		break;
	    if (soap->error)
		return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put___ns1__getProfessionalReportId(struct soap *soap, const struct __ns1__getProfessionalReportId *a, const char *tag, const char *type)
    {
	register int id = 0;
	if (soap_out___ns1__getProfessionalReportId(soap, tag?tag:"-ns1:getProfessionalReportId", id, a, type))
	    return soap->error;
	return SOAP_OK;
    }

    SOAP_FMAC3 struct __ns1__getProfessionalReportId * SOAP_FMAC4 soap_get___ns1__getProfessionalReportId(struct soap *soap, struct __ns1__getProfessionalReportId *p, const char *tag, const char *type)
    {
	if ((p = soap_in___ns1__getProfessionalReportId(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

#ifndef WITH_NOGLOBAL

    SOAP_FMAC3 void SOAP_FMAC4 soap_default_SOAP_ENV__Header(struct soap *soap, struct SOAP_ENV__Header *a)
    {
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->ns2__AuthHeader = NULL;
	a->ns2__ResHeader = NULL;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_SOAP_ENV__Header(struct soap *soap, const struct SOAP_ENV__Header *a)
    {
#ifndef WITH_NOIDREF
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTons2__AuthHeader(soap, &a->ns2__AuthHeader);
	soap_serialize_PointerTons2__ResHeader(soap, &a->ns2__ResHeader);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_SOAP_ENV__Header(struct soap *soap, const char *tag, int id, const struct SOAP_ENV__Header *a, const char *type)
    {
	(void)soap; (void)tag; (void)id; (void)type;
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_SOAP_ENV__Header), type))
	    return soap->error;
	soap->mustUnderstand = 1;
	if (soap_out_PointerTons2__AuthHeader(soap, "ns2:AuthHeader", -1, &a->ns2__AuthHeader, ""))
	    return soap->error;
	if (soap_out_PointerTons2__ResHeader(soap, "ns2:ResHeader", -1, &a->ns2__ResHeader, ""))
	    return soap->error;
	return soap_element_end_out(soap, tag);
    }

    SOAP_FMAC3 struct SOAP_ENV__Header * SOAP_FMAC4 soap_in_SOAP_ENV__Header(struct soap *soap, const char *tag, struct SOAP_ENV__Header *a, const char *type)
    {
	size_t soap_flag_ns2__AuthHeader = 1;
	size_t soap_flag_ns2__ResHeader = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
	    return NULL;
	a = (struct SOAP_ENV__Header *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_SOAP_ENV__Header, sizeof(struct SOAP_ENV__Header), 0, NULL, NULL, NULL);
	if (!a)
	    return NULL;
	soap_default_SOAP_ENV__Header(soap, a);
	if (soap->body && !*soap->href)
	{
	    for (;;)
	    {	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_ns2__AuthHeader && soap->error == SOAP_TAG_MISMATCH)
		    if (soap_in_PointerTons2__AuthHeader(soap, "ns2:AuthHeader", &a->ns2__AuthHeader, "ns2:AuthHeader"))
		    {	soap_flag_ns2__AuthHeader--;
			continue;
		    }
		if (soap_flag_ns2__ResHeader && soap->error == SOAP_TAG_MISMATCH)
		    if (soap_in_PointerTons2__ResHeader(soap, "ns2:ResHeader", &a->ns2__ResHeader, "ns2:ResHeader"))
		    {	soap_flag_ns2__ResHeader--;
			continue;
		    }
		if (soap->error == SOAP_TAG_MISMATCH)
		    soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
		    break;
		if (soap->error)
		    return NULL;
	    }
	    if (soap_element_end_in(soap, tag))
		return NULL;
	}
	else
	{	a = (struct SOAP_ENV__Header *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_SOAP_ENV__Header, 0, sizeof(struct SOAP_ENV__Header), 0, NULL);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}

	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_SOAP_ENV__Header(struct soap *soap, const struct SOAP_ENV__Header *a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_SOAP_ENV__Header);
	if (soap_out_SOAP_ENV__Header(soap, tag?tag:"SOAP-ENV:Header", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 struct SOAP_ENV__Header * SOAP_FMAC4 soap_get_SOAP_ENV__Header(struct soap *soap, struct SOAP_ENV__Header *p, const char *tag, const char *type)
    {
	if ((p = soap_in_SOAP_ENV__Header(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

#endif

    SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns2__Failure(struct soap *soap, struct ns2__Failure *a)
    {
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_int(soap, &a->code);
	soap_default_string(soap, &a->message);
	soap_default_string(soap, &a->position);
	soap_default_string(soap, &a->content);
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns2__Failure(struct soap *soap, const struct ns2__Failure *a)
    {
#ifndef WITH_NOIDREF
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_embedded(soap, &a->code, SOAP_TYPE_int);
	soap_serialize_string(soap, &a->message);
	soap_serialize_string(soap, &a->position);
	soap_serialize_string(soap, &a->content);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns2__Failure(struct soap *soap, const char *tag, int id, const struct ns2__Failure *a, const char *type)
    {
	(void)soap; (void)tag; (void)id; (void)type;
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns2__Failure), type))
	    return soap->error;
	if (soap_out_int(soap, "ns2:code", -1, &a->code, "")) {

	    return soap->error;
	}

	if (soap_out_string(soap, "ns2:message", -1, &a->message, ""))
	    return soap->error;
	if (a->position)
	{	if (soap_out_string(soap, "ns2:position", -1, &a->position, ""))
	    return soap->error;
	}
	else if (soap_element_nil(soap, "ns2:position"))
	    return soap->error;
	if (soap_out_string(soap, "ns2:content", -1, &a->content, ""))
	    return soap->error;
	return soap_element_end_out(soap, tag);
    }

    SOAP_FMAC3 struct ns2__Failure * SOAP_FMAC4 soap_in_ns2__Failure(struct soap *soap, const char *tag, struct ns2__Failure *a, const char *type)
    {
	size_t soap_flag_code = 1;
	size_t soap_flag_message = 1;
	size_t soap_flag_position = 1;
	size_t soap_flag_content = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
	    return NULL;
	a = (struct ns2__Failure *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns2__Failure, sizeof(struct ns2__Failure), 0, NULL, NULL, NULL);
	if (!a)
	    return NULL;
	soap_default_ns2__Failure(soap, a);
	if (soap->body && !*soap->href)
	{
	    for (;;)
	    {	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_code && soap->error == SOAP_TAG_MISMATCH)
		    if (soap_in_int(soap, "ns2:code", &a->code, "xsd:int"))
		    {	soap_flag_code--;
			continue;
		    }
		if (soap_flag_message && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
		    if (soap_in_string(soap, "ns2:message", &a->message, "xsd:string"))
		    {	soap_flag_message--;
			continue;
		    }
		if (soap_flag_position && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
		    if (soap_in_string(soap, "ns2:position", &a->position, "xsd:string"))
		    {	soap_flag_position--;
			continue;
		    }
		if (soap_flag_content && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
		    if (soap_in_string(soap, "ns2:content", &a->content, "xsd:string"))
		    {	soap_flag_content--;
			continue;
		    }
		if (soap->error == SOAP_TAG_MISMATCH)
		    soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
		    break;
		if (soap->error)
		    return NULL;
	    }

	    if (soap_element_end_in(soap, tag))
		return NULL;
	}
	else
	{	a = (struct ns2__Failure *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_ns2__Failure, 0, sizeof(struct ns2__Failure), 0, NULL);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_code > 0 || soap_flag_position > 0))
	{	soap->error = SOAP_OCCURS;
	    return NULL;
	}

	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns2__Failure(struct soap *soap, const struct ns2__Failure *a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns2__Failure);
	if (soap_out_ns2__Failure(soap, tag?tag:"ns2:Failure", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 struct ns2__Failure * SOAP_FMAC4 soap_get_ns2__Failure(struct soap *soap, struct ns2__Failure *p, const char *tag, const char *type)
    {
	if ((p = soap_in_ns2__Failure(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns2__ResHeader(struct soap *soap, struct ns2__ResHeader *a)
    {
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_string(soap, &a->desc);
	a->__sizefailures = 0;
	a->failures = NULL;
	a->oprs = NULL;
	a->oprtime = NULL;
	a->quota = NULL;
	a->rquota = NULL;
	soap_default_int(soap, &a->status);
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns2__ResHeader(struct soap *soap, const struct ns2__ResHeader *a)
    {
#ifndef WITH_NOIDREF
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_string(soap, &a->desc);
	if (a->failures)
	{	int i;
	    for (i = 0; i < a->__sizefailures; i++)
	    {
		soap_embedded(soap, a->failures + i, SOAP_TYPE_ns2__Failure);
		soap_serialize_ns2__Failure(soap, a->failures + i);
	    }
	}
	soap_serialize_PointerToint(soap, &a->oprs);
	soap_serialize_PointerToint(soap, &a->oprtime);
	soap_serialize_PointerToint(soap, &a->quota);
	soap_serialize_PointerToint(soap, &a->rquota);
	soap_embedded(soap, &a->status, SOAP_TYPE_int);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns2__ResHeader(struct soap *soap, const char *tag, int id, const struct ns2__ResHeader *a, const char *type)
    {
	(void)soap; (void)tag; (void)id; (void)type;
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns2__ResHeader), type))
	    return soap->error;
	if (a->desc)
	{	if (soap_out_string(soap, "ns2:desc", -1, &a->desc, ""))
	    return soap->error;
	}
	else if (soap_element_nil(soap, "ns2:desc"))
	    return soap->error;
	if (a->failures)
	{	int i;
	    for (i = 0; i < a->__sizefailures; i++)
		if (soap_out_ns2__Failure(soap, "ns2:failures", -1, a->failures + i, ""))
		    return soap->error;
	}
	if (soap_out_PointerToint(soap, "ns2:oprs", -1, &a->oprs, ""))
	    return soap->error;
	if (soap_out_PointerToint(soap, "ns2:oprtime", -1, &a->oprtime, ""))
	    return soap->error;
	if (soap_out_PointerToint(soap, "ns2:quota", -1, &a->quota, ""))
	    return soap->error;
	if (soap_out_PointerToint(soap, "ns2:rquota", -1, &a->rquota, ""))
	    return soap->error;
	if (soap_out_int(soap, "ns2:status", -1, &a->status, ""))
	    return soap->error;
	return soap_element_end_out(soap, tag);
    }

    SOAP_FMAC3 struct ns2__ResHeader * SOAP_FMAC4 soap_in_ns2__ResHeader(struct soap *soap, const char *tag, struct ns2__ResHeader *a, const char *type)
    {
	size_t soap_flag_desc = 1;
	struct soap_blist *soap_blist_failures = NULL;
	size_t soap_flag_oprs = 1;
	size_t soap_flag_oprtime = 1;
	size_t soap_flag_quota = 1;
	size_t soap_flag_rquota = 1;
	size_t soap_flag_status = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
	    return NULL;
	a = (struct ns2__ResHeader *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns2__ResHeader, sizeof(struct ns2__ResHeader), 0, NULL, NULL, NULL);
	if (!a)
	    return NULL;
	soap_default_ns2__ResHeader(soap, a);
	if (soap->body && !*soap->href)
	{
	    for (;;)
	    {	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_desc && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
		    if (soap_in_string(soap, "ns2:desc", &a->desc, "xsd:string"))
		    {	soap_flag_desc--;
			continue;
		    }
		if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "ns2:failures", 1, NULL))
		{	if (a->failures == NULL)
		    {	if (soap_blist_failures == NULL)
			soap_blist_failures = soap_new_block(soap);
			a->failures = (struct ns2__Failure *)soap_push_block(soap, soap_blist_failures, sizeof(struct ns2__Failure));
			if (a->failures == NULL)
			    return NULL;
			soap_default_ns2__Failure(soap, a->failures);
		    }
		    soap_revert(soap);
		    if (soap_in_ns2__Failure(soap, "ns2:failures", a->failures, "ns2:Failure"))
		    {	a->__sizefailures++;
			a->failures = NULL;
			continue;
		    }
		}
		if (soap_flag_oprs && soap->error == SOAP_TAG_MISMATCH)
		    if (soap_in_PointerToint(soap, "ns2:oprs", &a->oprs, "xsd:int"))
		    {	soap_flag_oprs--;
			continue;
		    }
		if (soap_flag_oprtime && soap->error == SOAP_TAG_MISMATCH)
		    if (soap_in_PointerToint(soap, "ns2:oprtime", &a->oprtime, "xsd:int"))
		    {	soap_flag_oprtime--;
			continue;
		    }
		if (soap_flag_quota && soap->error == SOAP_TAG_MISMATCH)
		    if (soap_in_PointerToint(soap, "ns2:quota", &a->quota, "xsd:int"))
		    {	soap_flag_quota--;
			continue;
		    }
		if (soap_flag_rquota && soap->error == SOAP_TAG_MISMATCH)
		    if (soap_in_PointerToint(soap, "ns2:rquota", &a->rquota, "xsd:int"))
		    {	soap_flag_rquota--;
			continue;
		    }
		if (soap_flag_status && soap->error == SOAP_TAG_MISMATCH)
		    if (soap_in_int(soap, "ns2:status", &a->status, "xsd:int"))
		    {	soap_flag_status--;
			continue;
		    }
		if (soap->error == SOAP_TAG_MISMATCH)
		    soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
		    break;
		if (soap->error)
		    return NULL;
	    }
	    if (a->failures)
		soap_pop_block(soap, soap_blist_failures);
	    if (a->__sizefailures)
		a->failures = (struct ns2__Failure *)soap_save_block(soap, soap_blist_failures, NULL, 1);
	    else
	    {	a->failures = NULL;
		if (soap_blist_failures)
		    soap_end_block(soap, soap_blist_failures);
	    }
	    if (soap_element_end_in(soap, tag))
		return NULL;
	}
	else
	{	a = (struct ns2__ResHeader *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_ns2__ResHeader, 0, sizeof(struct ns2__ResHeader), 0, NULL);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_desc > 0 || soap_flag_status > 0))
	{	soap->error = SOAP_OCCURS;
	    return NULL;
	}

	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns2__ResHeader(struct soap *soap, const struct ns2__ResHeader *a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns2__ResHeader);
	if (soap_out_ns2__ResHeader(soap, tag?tag:"ns2:ResHeader", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 struct ns2__ResHeader * SOAP_FMAC4 soap_get_ns2__ResHeader(struct soap *soap, struct ns2__ResHeader *p, const char *tag, const char *type)
    {
	if ((p = soap_in_ns2__ResHeader(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns2__AuthHeader(struct soap *soap, struct ns2__AuthHeader *a)
    {
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_string(soap, &a->username);
	soap_default_string(soap, &a->password);
	soap_default_string(soap, &a->token);
	soap_default_string(soap, &a->target);
	soap_default_string(soap, &a->accessToken);
	a->account_USCOREtype = NULL;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns2__AuthHeader(struct soap *soap, const struct ns2__AuthHeader *a)
    {
#ifndef WITH_NOIDREF
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_string(soap, &a->username);
	soap_serialize_string(soap, &a->password);
	soap_serialize_string(soap, &a->token);
	soap_serialize_string(soap, &a->target);
	soap_serialize_string(soap, &a->accessToken);
	soap_serialize_PointerToint(soap, &a->account_USCOREtype);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns2__AuthHeader(struct soap *soap, const char *tag, int id, const struct ns2__AuthHeader *a, const char *type)
    {
	(void)soap; (void)tag; (void)id; (void)type;
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns2__AuthHeader), type))
	    return soap->error;
	if (soap_out_string(soap, "ns2:username", -1, &a->username, ""))
	    return soap->error;
	if (soap_out_string(soap, "ns2:password", -1, &a->password, ""))
	    return soap->error;
	if (a->token)
	{	if (soap_out_string(soap, "ns2:token", -1, &a->token, ""))
	    return soap->error;
	}
	else if (soap_element_nil(soap, "ns2:token"))
	    return soap->error;
	if (soap_out_string(soap, "ns2:target", -1, &a->target, ""))
	    return soap->error;
	if (soap_out_string(soap, "ns2:accessToken", -1, &a->accessToken, ""))
	    return soap->error;
	if (soap_out_PointerToint(soap, "ns2:account_type", -1, &a->account_USCOREtype, ""))
	    return soap->error;
	return soap_element_end_out(soap, tag);
    }

    SOAP_FMAC3 struct ns2__AuthHeader * SOAP_FMAC4 soap_in_ns2__AuthHeader(struct soap *soap, const char *tag, struct ns2__AuthHeader *a, const char *type)
    {
	size_t soap_flag_username = 1;
	size_t soap_flag_password = 1;
	size_t soap_flag_token = 1;
	size_t soap_flag_target = 1;
	size_t soap_flag_accessToken = 1;
	size_t soap_flag_account_USCOREtype = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
	    return NULL;
	a = (struct ns2__AuthHeader *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns2__AuthHeader, sizeof(struct ns2__AuthHeader), 0, NULL, NULL, NULL);
	if (!a)
	    return NULL;
	soap_default_ns2__AuthHeader(soap, a);
	if (soap->body && !*soap->href)
	{
	    for (;;)
	    {	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_username && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
		    if (soap_in_string(soap, "ns2:username", &a->username, "xsd:string"))
		    {	soap_flag_username--;
			continue;
		    }
		if (soap_flag_password && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
		    if (soap_in_string(soap, "ns2:password", &a->password, "xsd:string"))
		    {	soap_flag_password--;
			continue;
		    }
		if (soap_flag_token && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
		    if (soap_in_string(soap, "ns2:token", &a->token, "xsd:string"))
		    {	soap_flag_token--;
			continue;
		    }
		if (soap_flag_target && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
		    if (soap_in_string(soap, "ns2:target", &a->target, "xsd:string"))
		    {	soap_flag_target--;
			continue;
		    }
		if (soap_flag_accessToken && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
		    if (soap_in_string(soap, "ns2:accessToken", &a->accessToken, "xsd:string"))
		    {	soap_flag_accessToken--;
			continue;
		    }
		if (soap_flag_account_USCOREtype && soap->error == SOAP_TAG_MISMATCH)
		    if (soap_in_PointerToint(soap, "ns2:account_type", &a->account_USCOREtype, "xsd:int"))
		    {	soap_flag_account_USCOREtype--;
			continue;
		    }
		if (soap->error == SOAP_TAG_MISMATCH)
		    soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
		    break;
		if (soap->error)
		    return NULL;
	    }
	    if (soap_element_end_in(soap, tag))
		return NULL;
	}
	else
	{	a = (struct ns2__AuthHeader *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_ns2__AuthHeader, 0, sizeof(struct ns2__AuthHeader), 0, NULL);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_token > 0))
	{	soap->error = SOAP_OCCURS;
	    return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns2__AuthHeader(struct soap *soap, const struct ns2__AuthHeader *a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns2__AuthHeader);
	if (soap_out_ns2__AuthHeader(soap, tag?tag:"ns2:AuthHeader", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 struct ns2__AuthHeader * SOAP_FMAC4 soap_get_ns2__AuthHeader(struct soap *soap, struct ns2__AuthHeader *p, const char *tag, const char *type)
    {
	if ((p = soap_in_ns2__AuthHeader(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns2__DoubleMapItemType(struct soap *soap, struct ns2__DoubleMapItemType *a)
    {
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_string(soap, &a->key);
	soap_default_double(soap, &a->value);
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns2__DoubleMapItemType(struct soap *soap, const struct ns2__DoubleMapItemType *a)
    {
#ifndef WITH_NOIDREF
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_string(soap, &a->key);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns2__DoubleMapItemType(struct soap *soap, const char *tag, int id, const struct ns2__DoubleMapItemType *a, const char *type)
    {
	(void)soap; (void)tag; (void)id; (void)type;
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns2__DoubleMapItemType), type))
	    return soap->error;
	if (a->key)
	{	if (soap_out_string(soap, "ns2:key", -1, &a->key, ""))
	    return soap->error;
	}
	else if (soap_element_nil(soap, "ns2:key"))
	    return soap->error;
	if (soap_out_double(soap, "ns2:value", -1, &a->value, ""))
	    return soap->error;
	return soap_element_end_out(soap, tag);
    }

    SOAP_FMAC3 struct ns2__DoubleMapItemType * SOAP_FMAC4 soap_in_ns2__DoubleMapItemType(struct soap *soap, const char *tag, struct ns2__DoubleMapItemType *a, const char *type)
    {
	size_t soap_flag_key = 1;
	size_t soap_flag_value = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
	    return NULL;
	a = (struct ns2__DoubleMapItemType *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns2__DoubleMapItemType, sizeof(struct ns2__DoubleMapItemType), 0, NULL, NULL, NULL);
	if (!a)
	    return NULL;
	soap_default_ns2__DoubleMapItemType(soap, a);
	if (soap->body && !*soap->href)
	{
	    for (;;)
	    {	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_key && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
		    if (soap_in_string(soap, "ns2:key", &a->key, "xsd:string"))
		    {	soap_flag_key--;
			continue;
		    }
		if (soap_flag_value && soap->error == SOAP_TAG_MISMATCH)
		    if (soap_in_double(soap, "ns2:value", &a->value, "xsd:double"))
		    {	soap_flag_value--;
			continue;
		    }
		if (soap->error == SOAP_TAG_MISMATCH)
		    soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
		    break;
		if (soap->error)
		    return NULL;
	    }
	    if (soap_element_end_in(soap, tag))
		return NULL;
	}
	else
	{	a = (struct ns2__DoubleMapItemType *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_ns2__DoubleMapItemType, 0, sizeof(struct ns2__DoubleMapItemType), 0, NULL);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_key > 0 || soap_flag_value > 0))
	{	soap->error = SOAP_OCCURS;
	    return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns2__DoubleMapItemType(struct soap *soap, const struct ns2__DoubleMapItemType *a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns2__DoubleMapItemType);
	if (soap_out_ns2__DoubleMapItemType(soap, tag?tag:"ns2:DoubleMapItemType", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 struct ns2__DoubleMapItemType * SOAP_FMAC4 soap_get_ns2__DoubleMapItemType(struct soap *soap, struct ns2__DoubleMapItemType *p, const char *tag, const char *type)
    {
	if ((p = soap_in_ns2__DoubleMapItemType(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns2__FloatMapItemType(struct soap *soap, struct ns2__FloatMapItemType *a)
    {
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_string(soap, &a->key);
	soap_default_float(soap, &a->value);
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns2__FloatMapItemType(struct soap *soap, const struct ns2__FloatMapItemType *a)
    {
#ifndef WITH_NOIDREF
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_string(soap, &a->key);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns2__FloatMapItemType(struct soap *soap, const char *tag, int id, const struct ns2__FloatMapItemType *a, const char *type)
    {
	(void)soap; (void)tag; (void)id; (void)type;
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns2__FloatMapItemType), type))
	    return soap->error;
	if (a->key)
	{	if (soap_out_string(soap, "ns2:key", -1, &a->key, ""))
	    return soap->error;
	}
	else if (soap_element_nil(soap, "ns2:key"))
	    return soap->error;
	if (soap_out_float(soap, "ns2:value", -1, &a->value, ""))
	    return soap->error;
	return soap_element_end_out(soap, tag);
    }

    SOAP_FMAC3 struct ns2__FloatMapItemType * SOAP_FMAC4 soap_in_ns2__FloatMapItemType(struct soap *soap, const char *tag, struct ns2__FloatMapItemType *a, const char *type)
    {
	size_t soap_flag_key = 1;
	size_t soap_flag_value = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
	    return NULL;
	a = (struct ns2__FloatMapItemType *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns2__FloatMapItemType, sizeof(struct ns2__FloatMapItemType), 0, NULL, NULL, NULL);
	if (!a)
	    return NULL;
	soap_default_ns2__FloatMapItemType(soap, a);
	if (soap->body && !*soap->href)
	{
	    for (;;)
	    {	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_key && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
		    if (soap_in_string(soap, "ns2:key", &a->key, "xsd:string"))
		    {	soap_flag_key--;
			continue;
		    }
		if (soap_flag_value && soap->error == SOAP_TAG_MISMATCH)
		    if (soap_in_float(soap, "ns2:value", &a->value, "xsd:float"))
		    {	soap_flag_value--;
			continue;
		    }
		if (soap->error == SOAP_TAG_MISMATCH)
		    soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
		    break;
		if (soap->error)
		    return NULL;
	    }
	    if (soap_element_end_in(soap, tag))
		return NULL;
	}
	else
	{	a = (struct ns2__FloatMapItemType *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_ns2__FloatMapItemType, 0, sizeof(struct ns2__FloatMapItemType), 0, NULL);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_key > 0 || soap_flag_value > 0))
	{	soap->error = SOAP_OCCURS;
	    return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns2__FloatMapItemType(struct soap *soap, const struct ns2__FloatMapItemType *a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns2__FloatMapItemType);
	if (soap_out_ns2__FloatMapItemType(soap, tag?tag:"ns2:FloatMapItemType", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 struct ns2__FloatMapItemType * SOAP_FMAC4 soap_get_ns2__FloatMapItemType(struct soap *soap, struct ns2__FloatMapItemType *p, const char *tag, const char *type)
    {
	if ((p = soap_in_ns2__FloatMapItemType(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns2__LongMapItemType(struct soap *soap, struct ns2__LongMapItemType *a)
    {
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_string(soap, &a->key);
	soap_default_LONG64(soap, &a->value);
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns2__LongMapItemType(struct soap *soap, const struct ns2__LongMapItemType *a)
    {
#ifndef WITH_NOIDREF
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_string(soap, &a->key);
	soap_embedded(soap, &a->value, SOAP_TYPE_LONG64);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns2__LongMapItemType(struct soap *soap, const char *tag, int id, const struct ns2__LongMapItemType *a, const char *type)
    {
	(void)soap; (void)tag; (void)id; (void)type;
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns2__LongMapItemType), type))
	    return soap->error;
	if (a->key)
	{	if (soap_out_string(soap, "ns2:key", -1, &a->key, ""))
	    return soap->error;
	}
	else if (soap_element_nil(soap, "ns2:key"))
	    return soap->error;
	if (soap_out_LONG64(soap, "ns2:value", -1, &a->value, ""))
	    return soap->error;
	return soap_element_end_out(soap, tag);
    }

    SOAP_FMAC3 struct ns2__LongMapItemType * SOAP_FMAC4 soap_in_ns2__LongMapItemType(struct soap *soap, const char *tag, struct ns2__LongMapItemType *a, const char *type)
    {
	size_t soap_flag_key = 1;
	size_t soap_flag_value = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
	    return NULL;
	a = (struct ns2__LongMapItemType *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns2__LongMapItemType, sizeof(struct ns2__LongMapItemType), 0, NULL, NULL, NULL);
	if (!a)
	    return NULL;
	soap_default_ns2__LongMapItemType(soap, a);
	if (soap->body && !*soap->href)
	{
	    for (;;)
	    {	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_key && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
		    if (soap_in_string(soap, "ns2:key", &a->key, "xsd:string"))
		    {	soap_flag_key--;
			continue;
		    }
		if (soap_flag_value && soap->error == SOAP_TAG_MISMATCH)
		    if (soap_in_LONG64(soap, "ns2:value", &a->value, "xsd:long"))
		    {	soap_flag_value--;
			continue;
		    }
		if (soap->error == SOAP_TAG_MISMATCH)
		    soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
		    break;
		if (soap->error)
		    return NULL;
	    }
	    if (soap_element_end_in(soap, tag))
		return NULL;
	}
	else
	{	a = (struct ns2__LongMapItemType *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_ns2__LongMapItemType, 0, sizeof(struct ns2__LongMapItemType), 0, NULL);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_key > 0 || soap_flag_value > 0))
	{	soap->error = SOAP_OCCURS;
	    return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns2__LongMapItemType(struct soap *soap, const struct ns2__LongMapItemType *a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns2__LongMapItemType);
	if (soap_out_ns2__LongMapItemType(soap, tag?tag:"ns2:LongMapItemType", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 struct ns2__LongMapItemType * SOAP_FMAC4 soap_get_ns2__LongMapItemType(struct soap *soap, struct ns2__LongMapItemType *p, const char *tag, const char *type)
    {
	if ((p = soap_in_ns2__LongMapItemType(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns2__IntMapItemType(struct soap *soap, struct ns2__IntMapItemType *a)
    {
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_string(soap, &a->key);
	soap_default_int(soap, &a->value);
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns2__IntMapItemType(struct soap *soap, const struct ns2__IntMapItemType *a)
    {
#ifndef WITH_NOIDREF
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_string(soap, &a->key);
	soap_embedded(soap, &a->value, SOAP_TYPE_int);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns2__IntMapItemType(struct soap *soap, const char *tag, int id, const struct ns2__IntMapItemType *a, const char *type)
    {
	(void)soap; (void)tag; (void)id; (void)type;
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns2__IntMapItemType), type))
	    return soap->error;
	if (a->key)
	{	if (soap_out_string(soap, "ns2:key", -1, &a->key, ""))
	    return soap->error;
	}
	else if (soap_element_nil(soap, "ns2:key"))
	    return soap->error;
	if (soap_out_int(soap, "ns2:value", -1, &a->value, ""))
	    return soap->error;
	return soap_element_end_out(soap, tag);
    }

    SOAP_FMAC3 struct ns2__IntMapItemType * SOAP_FMAC4 soap_in_ns2__IntMapItemType(struct soap *soap, const char *tag, struct ns2__IntMapItemType *a, const char *type)
    {
	size_t soap_flag_key = 1;
	size_t soap_flag_value = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
	    return NULL;
	a = (struct ns2__IntMapItemType *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns2__IntMapItemType, sizeof(struct ns2__IntMapItemType), 0, NULL, NULL, NULL);
	if (!a)
	    return NULL;
	soap_default_ns2__IntMapItemType(soap, a);
	if (soap->body && !*soap->href)
	{
	    for (;;)
	    {	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_key && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
		    if (soap_in_string(soap, "ns2:key", &a->key, "xsd:string"))
		    {	soap_flag_key--;
			continue;
		    }
		if (soap_flag_value && soap->error == SOAP_TAG_MISMATCH)
		    if (soap_in_int(soap, "ns2:value", &a->value, "xsd:int"))
		    {	soap_flag_value--;
			continue;
		    }
		if (soap->error == SOAP_TAG_MISMATCH)
		    soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
		    break;
		if (soap->error)
		    return NULL;
	    }
	    if (soap_element_end_in(soap, tag))
		return NULL;
	}
	else
	{	a = (struct ns2__IntMapItemType *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_ns2__IntMapItemType, 0, sizeof(struct ns2__IntMapItemType), 0, NULL);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_key > 0 || soap_flag_value > 0))
	{	soap->error = SOAP_OCCURS;
	    return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns2__IntMapItemType(struct soap *soap, const struct ns2__IntMapItemType *a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns2__IntMapItemType);
	if (soap_out_ns2__IntMapItemType(soap, tag?tag:"ns2:IntMapItemType", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 struct ns2__IntMapItemType * SOAP_FMAC4 soap_get_ns2__IntMapItemType(struct soap *soap, struct ns2__IntMapItemType *p, const char *tag, const char *type)
    {
	if ((p = soap_in_ns2__IntMapItemType(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns2__StringMapItemType(struct soap *soap, struct ns2__StringMapItemType *a)
    {
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_string(soap, &a->key);
	soap_default_string(soap, &a->value);
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns2__StringMapItemType(struct soap *soap, const struct ns2__StringMapItemType *a)
    {
#ifndef WITH_NOIDREF
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_string(soap, &a->key);
	soap_serialize_string(soap, &a->value);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns2__StringMapItemType(struct soap *soap, const char *tag, int id, const struct ns2__StringMapItemType *a, const char *type)
    {
	(void)soap; (void)tag; (void)id; (void)type;
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns2__StringMapItemType), type))
	    return soap->error;
	if (a->key)
	{	if (soap_out_string(soap, "ns2:key", -1, &a->key, ""))
	    return soap->error;
	}
	else if (soap_element_nil(soap, "ns2:key"))
	    return soap->error;
	if (a->value)
	{	if (soap_out_string(soap, "ns2:value", -1, &a->value, ""))
	    return soap->error;
	}
	else if (soap_element_nil(soap, "ns2:value"))
	    return soap->error;
	return soap_element_end_out(soap, tag);
    }

    SOAP_FMAC3 struct ns2__StringMapItemType * SOAP_FMAC4 soap_in_ns2__StringMapItemType(struct soap *soap, const char *tag, struct ns2__StringMapItemType *a, const char *type)
    {
	size_t soap_flag_key = 1;
	size_t soap_flag_value = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
	    return NULL;
	a = (struct ns2__StringMapItemType *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns2__StringMapItemType, sizeof(struct ns2__StringMapItemType), 0, NULL, NULL, NULL);
	if (!a)
	    return NULL;
	soap_default_ns2__StringMapItemType(soap, a);
	if (soap->body && !*soap->href)
	{
	    for (;;)
	    {	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_key && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
		    if (soap_in_string(soap, "ns2:key", &a->key, "xsd:string"))
		    {	soap_flag_key--;
			continue;
		    }
		if (soap_flag_value && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
		    if (soap_in_string(soap, "ns2:value", &a->value, "xsd:string"))
		    {	soap_flag_value--;
			continue;
		    }
		if (soap->error == SOAP_TAG_MISMATCH)
		    soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
		    break;
		if (soap->error)
		    return NULL;
	    }
	    if (soap_element_end_in(soap, tag))
		return NULL;
	}
	else
	{	a = (struct ns2__StringMapItemType *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_ns2__StringMapItemType, 0, sizeof(struct ns2__StringMapItemType), 0, NULL);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_key > 0 || soap_flag_value > 0))
	{	soap->error = SOAP_OCCURS;
	    return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns2__StringMapItemType(struct soap *soap, const struct ns2__StringMapItemType *a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns2__StringMapItemType);
	if (soap_out_ns2__StringMapItemType(soap, tag?tag:"ns2:StringMapItemType", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 struct ns2__StringMapItemType * SOAP_FMAC4 soap_get_ns2__StringMapItemType(struct soap *soap, struct ns2__StringMapItemType *p, const char *tag, const char *type)
    {
	if ((p = soap_in_ns2__StringMapItemType(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns2__OptType(struct soap *soap, struct ns2__OptType *a)
    {
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->__sizeoptString = 0;
	a->optString = NULL;
	a->__sizeoptInt = 0;
	a->optInt = NULL;
	a->__sizeoptLong = 0;
	a->optLong = NULL;
	a->__sizeoptFloat = 0;
	a->optFloat = NULL;
	a->__sizeoptDouble = 0;
	a->optDouble = NULL;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns2__OptType(struct soap *soap, const struct ns2__OptType *a)
    {
#ifndef WITH_NOIDREF
	(void)soap; (void)a; /* appease -Wall -Werror */
	if (a->optString)
	{	int i;
	    for (i = 0; i < a->__sizeoptString; i++)
	    {
		soap_embedded(soap, a->optString + i, SOAP_TYPE_ns2__StringMapItemType);
		soap_serialize_ns2__StringMapItemType(soap, a->optString + i);
	    }
	}
	if (a->optInt)
	{	int i;
	    for (i = 0; i < a->__sizeoptInt; i++)
	    {
		soap_embedded(soap, a->optInt + i, SOAP_TYPE_ns2__IntMapItemType);
		soap_serialize_ns2__IntMapItemType(soap, a->optInt + i);
	    }
	}
	if (a->optLong)
	{	int i;
	    for (i = 0; i < a->__sizeoptLong; i++)
	    {
		soap_embedded(soap, a->optLong + i, SOAP_TYPE_ns2__LongMapItemType);
		soap_serialize_ns2__LongMapItemType(soap, a->optLong + i);
	    }
	}
	if (a->optFloat)
	{	int i;
	    for (i = 0; i < a->__sizeoptFloat; i++)
	    {
		soap_embedded(soap, a->optFloat + i, SOAP_TYPE_ns2__FloatMapItemType);
		soap_serialize_ns2__FloatMapItemType(soap, a->optFloat + i);
	    }
	}
	if (a->optDouble)
	{	int i;
	    for (i = 0; i < a->__sizeoptDouble; i++)
	    {
		soap_embedded(soap, a->optDouble + i, SOAP_TYPE_ns2__DoubleMapItemType);
		soap_serialize_ns2__DoubleMapItemType(soap, a->optDouble + i);
	    }
	}
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns2__OptType(struct soap *soap, const char *tag, int id, const struct ns2__OptType *a, const char *type)
    {
	(void)soap; (void)tag; (void)id; (void)type;
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns2__OptType), type))
	    return soap->error;
	if (a->optString)
	{	int i;
	    for (i = 0; i < a->__sizeoptString; i++)
		if (soap_out_ns2__StringMapItemType(soap, "ns2:optString", -1, a->optString + i, ""))
		    return soap->error;
	}
	if (a->optInt)
	{	int i;
	    for (i = 0; i < a->__sizeoptInt; i++)
		if (soap_out_ns2__IntMapItemType(soap, "ns2:optInt", -1, a->optInt + i, ""))
		    return soap->error;
	}
	if (a->optLong)
	{	int i;
	    for (i = 0; i < a->__sizeoptLong; i++)
		if (soap_out_ns2__LongMapItemType(soap, "ns2:optLong", -1, a->optLong + i, ""))
		    return soap->error;
	}
	if (a->optFloat)
	{	int i;
	    for (i = 0; i < a->__sizeoptFloat; i++)
		if (soap_out_ns2__FloatMapItemType(soap, "ns2:optFloat", -1, a->optFloat + i, ""))
		    return soap->error;
	}
	if (a->optDouble)
	{	int i;
	    for (i = 0; i < a->__sizeoptDouble; i++)
		if (soap_out_ns2__DoubleMapItemType(soap, "ns2:optDouble", -1, a->optDouble + i, ""))
		    return soap->error;
	}
	return soap_element_end_out(soap, tag);
    }

    SOAP_FMAC3 struct ns2__OptType * SOAP_FMAC4 soap_in_ns2__OptType(struct soap *soap, const char *tag, struct ns2__OptType *a, const char *type)
    {
	struct soap_blist *soap_blist_optString = NULL;
	struct soap_blist *soap_blist_optInt = NULL;
	struct soap_blist *soap_blist_optLong = NULL;
	struct soap_blist *soap_blist_optFloat = NULL;
	struct soap_blist *soap_blist_optDouble = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
	    return NULL;
	a = (struct ns2__OptType *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns2__OptType, sizeof(struct ns2__OptType), 0, NULL, NULL, NULL);
	if (!a)
	    return NULL;
	soap_default_ns2__OptType(soap, a);
	if (soap->body && !*soap->href)
	{
	    for (;;)
	    {	soap->error = SOAP_TAG_MISMATCH;
		if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "ns2:optString", 1, NULL))
		{	if (a->optString == NULL)
		    {	if (soap_blist_optString == NULL)
			soap_blist_optString = soap_new_block(soap);
			a->optString = (struct ns2__StringMapItemType *)soap_push_block(soap, soap_blist_optString, sizeof(struct ns2__StringMapItemType));
			if (a->optString == NULL)
			    return NULL;
			soap_default_ns2__StringMapItemType(soap, a->optString);
		    }
		    soap_revert(soap);
		    if (soap_in_ns2__StringMapItemType(soap, "ns2:optString", a->optString, "ns2:StringMapItemType"))
		    {	a->__sizeoptString++;
			a->optString = NULL;
			continue;
		    }
		}
		if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "ns2:optInt", 1, NULL))
		{	if (a->optInt == NULL)
		    {	if (soap_blist_optInt == NULL)
			soap_blist_optInt = soap_new_block(soap);
			a->optInt = (struct ns2__IntMapItemType *)soap_push_block(soap, soap_blist_optInt, sizeof(struct ns2__IntMapItemType));
			if (a->optInt == NULL)
			    return NULL;
			soap_default_ns2__IntMapItemType(soap, a->optInt);
		    }
		    soap_revert(soap);
		    if (soap_in_ns2__IntMapItemType(soap, "ns2:optInt", a->optInt, "ns2:IntMapItemType"))
		    {	a->__sizeoptInt++;
			a->optInt = NULL;
			continue;
		    }
		}
		if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "ns2:optLong", 1, NULL))
		{	if (a->optLong == NULL)
		    {	if (soap_blist_optLong == NULL)
			soap_blist_optLong = soap_new_block(soap);
			a->optLong = (struct ns2__LongMapItemType *)soap_push_block(soap, soap_blist_optLong, sizeof(struct ns2__LongMapItemType));
			if (a->optLong == NULL)
			    return NULL;
			soap_default_ns2__LongMapItemType(soap, a->optLong);
		    }
		    soap_revert(soap);
		    if (soap_in_ns2__LongMapItemType(soap, "ns2:optLong", a->optLong, "ns2:LongMapItemType"))
		    {	a->__sizeoptLong++;
			a->optLong = NULL;
			continue;
		    }
		}
		if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "ns2:optFloat", 1, NULL))
		{	if (a->optFloat == NULL)
		    {	if (soap_blist_optFloat == NULL)
			soap_blist_optFloat = soap_new_block(soap);
			a->optFloat = (struct ns2__FloatMapItemType *)soap_push_block(soap, soap_blist_optFloat, sizeof(struct ns2__FloatMapItemType));
			if (a->optFloat == NULL)
			    return NULL;
			soap_default_ns2__FloatMapItemType(soap, a->optFloat);
		    }
		    soap_revert(soap);
		    if (soap_in_ns2__FloatMapItemType(soap, "ns2:optFloat", a->optFloat, "ns2:FloatMapItemType"))
		    {	a->__sizeoptFloat++;
			a->optFloat = NULL;
			continue;
		    }
		}
		if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "ns2:optDouble", 1, NULL))
		{	if (a->optDouble == NULL)
		    {	if (soap_blist_optDouble == NULL)
			soap_blist_optDouble = soap_new_block(soap);
			a->optDouble = (struct ns2__DoubleMapItemType *)soap_push_block(soap, soap_blist_optDouble, sizeof(struct ns2__DoubleMapItemType));
			if (a->optDouble == NULL)
			    return NULL;
			soap_default_ns2__DoubleMapItemType(soap, a->optDouble);
		    }
		    soap_revert(soap);
		    if (soap_in_ns2__DoubleMapItemType(soap, "ns2:optDouble", a->optDouble, "ns2:DoubleMapItemType"))
		    {	a->__sizeoptDouble++;
			a->optDouble = NULL;
			continue;
		    }
		}
		if (soap->error == SOAP_TAG_MISMATCH)
		    soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
		    break;
		if (soap->error)
		    return NULL;
	    }
	    if (a->optString)
		soap_pop_block(soap, soap_blist_optString);
	    if (a->__sizeoptString)
		a->optString = (struct ns2__StringMapItemType *)soap_save_block(soap, soap_blist_optString, NULL, 1);
	    else
	    {	a->optString = NULL;
		if (soap_blist_optString)
		    soap_end_block(soap, soap_blist_optString);
	    }
	    if (a->optInt)
		soap_pop_block(soap, soap_blist_optInt);
	    if (a->__sizeoptInt)
		a->optInt = (struct ns2__IntMapItemType *)soap_save_block(soap, soap_blist_optInt, NULL, 1);
	    else
	    {	a->optInt = NULL;
		if (soap_blist_optInt)
		    soap_end_block(soap, soap_blist_optInt);
	    }
	    if (a->optLong)
		soap_pop_block(soap, soap_blist_optLong);
	    if (a->__sizeoptLong)
		a->optLong = (struct ns2__LongMapItemType *)soap_save_block(soap, soap_blist_optLong, NULL, 1);
	    else
	    {	a->optLong = NULL;
		if (soap_blist_optLong)
		    soap_end_block(soap, soap_blist_optLong);
	    }
	    if (a->optFloat)
		soap_pop_block(soap, soap_blist_optFloat);
	    if (a->__sizeoptFloat)
		a->optFloat = (struct ns2__FloatMapItemType *)soap_save_block(soap, soap_blist_optFloat, NULL, 1);
	    else
	    {	a->optFloat = NULL;
		if (soap_blist_optFloat)
		    soap_end_block(soap, soap_blist_optFloat);
	    }
	    if (a->optDouble)
		soap_pop_block(soap, soap_blist_optDouble);
	    if (a->__sizeoptDouble)
		a->optDouble = (struct ns2__DoubleMapItemType *)soap_save_block(soap, soap_blist_optDouble, NULL, 1);
	    else
	    {	a->optDouble = NULL;
		if (soap_blist_optDouble)
		    soap_end_block(soap, soap_blist_optDouble);
	    }
	    if (soap_element_end_in(soap, tag))
		return NULL;
	}
	else
	{	a = (struct ns2__OptType *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_ns2__OptType, 0, sizeof(struct ns2__OptType), 0, NULL);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns2__OptType(struct soap *soap, const struct ns2__OptType *a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns2__OptType);
	if (soap_out_ns2__OptType(soap, tag?tag:"ns2:OptType", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 struct ns2__OptType * SOAP_FMAC4 soap_get_ns2__OptType(struct soap *soap, struct ns2__OptType *p, const char *tag, const char *type)
    {
	if ((p = soap_in_ns2__OptType(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_default__ns1__getReportFileUrlResponse(struct soap *soap, struct _ns1__getReportFileUrlResponse *a)
    {
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_string(soap, &a->reportFilePath);
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__ns1__getReportFileUrlResponse(struct soap *soap, const struct _ns1__getReportFileUrlResponse *a)
    {
#ifndef WITH_NOIDREF
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_string(soap, &a->reportFilePath);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out__ns1__getReportFileUrlResponse(struct soap *soap, const char *tag, int id, const struct _ns1__getReportFileUrlResponse *a, const char *type)
    {
	(void)soap; (void)tag; (void)id; (void)type;
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__ns1__getReportFileUrlResponse), type))
	    return soap->error;
	if (soap_out_string(soap, "reportFilePath", -1, &a->reportFilePath, ""))
	    return soap->error;
	return soap_element_end_out(soap, tag);
    }

    SOAP_FMAC3 struct _ns1__getReportFileUrlResponse * SOAP_FMAC4 soap_in__ns1__getReportFileUrlResponse(struct soap *soap, const char *tag, struct _ns1__getReportFileUrlResponse *a, const char *type)
    {
	size_t soap_flag_reportFilePath = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
	    return NULL;
	a = (struct _ns1__getReportFileUrlResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__ns1__getReportFileUrlResponse, sizeof(struct _ns1__getReportFileUrlResponse), 0, NULL, NULL, NULL);
	if (!a)
	    return NULL;
	soap_default__ns1__getReportFileUrlResponse(soap, a);
	if (soap->body && !*soap->href)
	{
	    for (;;)
	    {	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_reportFilePath && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
		    if (soap_in_string(soap, "reportFilePath", &a->reportFilePath, "xsd:string"))
		    {	soap_flag_reportFilePath--;
			continue;
		    }
		if (soap->error == SOAP_TAG_MISMATCH)
		    soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
		    break;
		if (soap->error)
		    return NULL;
	    }
	    if (soap_element_end_in(soap, tag))
		return NULL;
	}
	else
	{	a = (struct _ns1__getReportFileUrlResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__ns1__getReportFileUrlResponse, 0, sizeof(struct _ns1__getReportFileUrlResponse), 0, NULL);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put__ns1__getReportFileUrlResponse(struct soap *soap, const struct _ns1__getReportFileUrlResponse *a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__ns1__getReportFileUrlResponse);
	if (soap_out__ns1__getReportFileUrlResponse(soap, tag?tag:"ns1:getReportFileUrlResponse", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 struct _ns1__getReportFileUrlResponse * SOAP_FMAC4 soap_get__ns1__getReportFileUrlResponse(struct soap *soap, struct _ns1__getReportFileUrlResponse *p, const char *tag, const char *type)
    {
	if ((p = soap_in__ns1__getReportFileUrlResponse(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_default__ns1__getReportFileUrlRequest(struct soap *soap, struct _ns1__getReportFileUrlRequest *a)
    {
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_string(soap, &a->reportId);
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__ns1__getReportFileUrlRequest(struct soap *soap, const struct _ns1__getReportFileUrlRequest *a)
    {
#ifndef WITH_NOIDREF
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_string(soap, &a->reportId);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out__ns1__getReportFileUrlRequest(struct soap *soap, const char *tag, int id, const struct _ns1__getReportFileUrlRequest *a, const char *type)
    {
	(void)soap; (void)tag; (void)id; (void)type;
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__ns1__getReportFileUrlRequest), type))
	    return soap->error;
	if (soap_out_string(soap, "reportId", -1, &a->reportId, ""))
	    return soap->error;
	return soap_element_end_out(soap, tag);
    }

    SOAP_FMAC3 struct _ns1__getReportFileUrlRequest * SOAP_FMAC4 soap_in__ns1__getReportFileUrlRequest(struct soap *soap, const char *tag, struct _ns1__getReportFileUrlRequest *a, const char *type)
    {
	size_t soap_flag_reportId = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
	    return NULL;
	a = (struct _ns1__getReportFileUrlRequest *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__ns1__getReportFileUrlRequest, sizeof(struct _ns1__getReportFileUrlRequest), 0, NULL, NULL, NULL);
	if (!a)
	    return NULL;
	soap_default__ns1__getReportFileUrlRequest(soap, a);
	if (soap->body && !*soap->href)
	{
	    for (;;)
	    {	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_reportId && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
		    if (soap_in_string(soap, "reportId", &a->reportId, "xsd:string"))
		    {	soap_flag_reportId--;
			continue;
		    }
		if (soap->error == SOAP_TAG_MISMATCH)
		    soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
		    break;
		if (soap->error)
		    return NULL;
	    }
	    if (soap_element_end_in(soap, tag))
		return NULL;
	}
	else
	{	a = (struct _ns1__getReportFileUrlRequest *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__ns1__getReportFileUrlRequest, 0, sizeof(struct _ns1__getReportFileUrlRequest), 0, NULL);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put__ns1__getReportFileUrlRequest(struct soap *soap, const struct _ns1__getReportFileUrlRequest *a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__ns1__getReportFileUrlRequest);
	if (soap_out__ns1__getReportFileUrlRequest(soap, tag?tag:"ns1:getReportFileUrlRequest", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 struct _ns1__getReportFileUrlRequest * SOAP_FMAC4 soap_get__ns1__getReportFileUrlRequest(struct soap *soap, struct _ns1__getReportFileUrlRequest *p, const char *tag, const char *type)
    {
	if ((p = soap_in__ns1__getReportFileUrlRequest(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_default__ns1__getReportStateResponse(struct soap *soap, struct _ns1__getReportStateResponse *a)
    {
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->isGenerated = NULL;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__ns1__getReportStateResponse(struct soap *soap, const struct _ns1__getReportStateResponse *a)
    {
#ifndef WITH_NOIDREF
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerToint(soap, &a->isGenerated);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out__ns1__getReportStateResponse(struct soap *soap, const char *tag, int id, const struct _ns1__getReportStateResponse *a, const char *type)
    {
	(void)soap; (void)tag; (void)id; (void)type;
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__ns1__getReportStateResponse), type))
	    return soap->error;
	if (soap_out_PointerToint(soap, "isGenerated", -1, &a->isGenerated, ""))
	    return soap->error;
	return soap_element_end_out(soap, tag);
    }

    SOAP_FMAC3 struct _ns1__getReportStateResponse * SOAP_FMAC4 soap_in__ns1__getReportStateResponse(struct soap *soap, const char *tag, struct _ns1__getReportStateResponse *a, const char *type)
    {
	size_t soap_flag_isGenerated = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
	    return NULL;
	a = (struct _ns1__getReportStateResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__ns1__getReportStateResponse, sizeof(struct _ns1__getReportStateResponse), 0, NULL, NULL, NULL);
	if (!a)
	    return NULL;
	soap_default__ns1__getReportStateResponse(soap, a);
	if (soap->body && !*soap->href)
	{
	    for (;;)
	    {	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_isGenerated && soap->error == SOAP_TAG_MISMATCH)
		    if (soap_in_PointerToint(soap, "isGenerated", &a->isGenerated, "xsd:int"))
		    {	soap_flag_isGenerated--;
			continue;
		    }
		if (soap->error == SOAP_TAG_MISMATCH)
		    soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
		    break;
		if (soap->error)
		    return NULL;
	    }
	    if (soap_element_end_in(soap, tag))
		return NULL;
	}
	else
	{	a = (struct _ns1__getReportStateResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__ns1__getReportStateResponse, 0, sizeof(struct _ns1__getReportStateResponse), 0, NULL);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put__ns1__getReportStateResponse(struct soap *soap, const struct _ns1__getReportStateResponse *a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__ns1__getReportStateResponse);
	if (soap_out__ns1__getReportStateResponse(soap, tag?tag:"ns1:getReportStateResponse", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 struct _ns1__getReportStateResponse * SOAP_FMAC4 soap_get__ns1__getReportStateResponse(struct soap *soap, struct _ns1__getReportStateResponse *p, const char *tag, const char *type)
    {
	if ((p = soap_in__ns1__getReportStateResponse(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_default__ns1__getReportStateRequest(struct soap *soap, struct _ns1__getReportStateRequest *a)
    {
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_string(soap, &a->reportId);
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__ns1__getReportStateRequest(struct soap *soap, const struct _ns1__getReportStateRequest *a)
    {
#ifndef WITH_NOIDREF
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_string(soap, &a->reportId);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out__ns1__getReportStateRequest(struct soap *soap, const char *tag, int id, const struct _ns1__getReportStateRequest *a, const char *type)
    {
	(void)soap; (void)tag; (void)id; (void)type;
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__ns1__getReportStateRequest), type))
	    return soap->error;
	if (a->reportId)
	{	if (soap_out_string(soap, "reportId", -1, &a->reportId, ""))
	    return soap->error;
	}
	else if (soap_element_nil(soap, "reportId"))
	    return soap->error;
	return soap_element_end_out(soap, tag);
    }

    SOAP_FMAC3 struct _ns1__getReportStateRequest * SOAP_FMAC4 soap_in__ns1__getReportStateRequest(struct soap *soap, const char *tag, struct _ns1__getReportStateRequest *a, const char *type)
    {
	size_t soap_flag_reportId = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
	    return NULL;
	a = (struct _ns1__getReportStateRequest *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__ns1__getReportStateRequest, sizeof(struct _ns1__getReportStateRequest), 0, NULL, NULL, NULL);
	if (!a)
	    return NULL;
	soap_default__ns1__getReportStateRequest(soap, a);
	if (soap->body && !*soap->href)
	{
	    for (;;)
	    {	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_reportId && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
		    if (soap_in_string(soap, "reportId", &a->reportId, "xsd:string"))
		    {	soap_flag_reportId--;
			continue;
		    }
		if (soap->error == SOAP_TAG_MISMATCH)
		    soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
		    break;
		if (soap->error)
		    return NULL;
	    }
	    if (soap_element_end_in(soap, tag))
		return NULL;
	}
	else
	{	a = (struct _ns1__getReportStateRequest *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__ns1__getReportStateRequest, 0, sizeof(struct _ns1__getReportStateRequest), 0, NULL);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_reportId > 0))
	{	soap->error = SOAP_OCCURS;
	    return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put__ns1__getReportStateRequest(struct soap *soap, const struct _ns1__getReportStateRequest *a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__ns1__getReportStateRequest);
	if (soap_out__ns1__getReportStateRequest(soap, tag?tag:"ns1:getReportStateRequest", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 struct _ns1__getReportStateRequest * SOAP_FMAC4 soap_get__ns1__getReportStateRequest(struct soap *soap, struct _ns1__getReportStateRequest *p, const char *tag, const char *type)
    {
	if ((p = soap_in__ns1__getReportStateRequest(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_default__ns1__getProfessionalReportIdResponse(struct soap *soap, struct _ns1__getProfessionalReportIdResponse *a)
    {
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_string(soap, &a->reportId);
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__ns1__getProfessionalReportIdResponse(struct soap *soap, const struct _ns1__getProfessionalReportIdResponse *a)
    {
#ifndef WITH_NOIDREF
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_string(soap, &a->reportId);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out__ns1__getProfessionalReportIdResponse(struct soap *soap, const char *tag, int id, const struct _ns1__getProfessionalReportIdResponse *a, const char *type)
    {
	(void)soap; (void)tag; (void)id; (void)type;
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__ns1__getProfessionalReportIdResponse), type))
	    return soap->error;
	if (soap_out_string(soap, "reportId", -1, &a->reportId, ""))
	    return soap->error;
	return soap_element_end_out(soap, tag);
    }

    SOAP_FMAC3 struct _ns1__getProfessionalReportIdResponse * SOAP_FMAC4 soap_in__ns1__getProfessionalReportIdResponse(struct soap *soap, const char *tag, struct _ns1__getProfessionalReportIdResponse *a, const char *type)
    {
	size_t soap_flag_reportId = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
	    return NULL;
	a = (struct _ns1__getProfessionalReportIdResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__ns1__getProfessionalReportIdResponse, sizeof(struct _ns1__getProfessionalReportIdResponse), 0, NULL, NULL, NULL);
	if (!a)
	    return NULL;
	soap_default__ns1__getProfessionalReportIdResponse(soap, a);
	if (soap->body && !*soap->href)
	{
	    for (;;)
	    {	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_reportId && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
		    if (soap_in_string(soap, "reportId", &a->reportId, "xsd:string"))
		    {	soap_flag_reportId--;
			continue;
		    }
		if (soap->error == SOAP_TAG_MISMATCH)
		    soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
		    break;
		if (soap->error)
		    return NULL;
	    }
	    if (soap_element_end_in(soap, tag))
		return NULL;
	}
	else
	{	a = (struct _ns1__getProfessionalReportIdResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__ns1__getProfessionalReportIdResponse, 0, sizeof(struct _ns1__getProfessionalReportIdResponse), 0, NULL);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put__ns1__getProfessionalReportIdResponse(struct soap *soap, const struct _ns1__getProfessionalReportIdResponse *a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__ns1__getProfessionalReportIdResponse);
	if (soap_out__ns1__getProfessionalReportIdResponse(soap, tag?tag:"ns1:getProfessionalReportIdResponse", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 struct _ns1__getProfessionalReportIdResponse * SOAP_FMAC4 soap_get__ns1__getProfessionalReportIdResponse(struct soap *soap, struct _ns1__getProfessionalReportIdResponse *p, const char *tag, const char *type)
    {
	if ((p = soap_in__ns1__getProfessionalReportIdResponse(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_default__ns1__getProfessionalReportIdRequest(struct soap *soap, struct _ns1__getProfessionalReportIdRequest *a)
    {
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->reportRequestType = NULL;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__ns1__getProfessionalReportIdRequest(struct soap *soap, const struct _ns1__getProfessionalReportIdRequest *a)
    {
#ifndef WITH_NOIDREF
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTons1__ReportRequestType(soap, &a->reportRequestType);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out__ns1__getProfessionalReportIdRequest(struct soap *soap, const char *tag, int id, const struct _ns1__getProfessionalReportIdRequest *a, const char *type)
    {
	(void)soap; (void)tag; (void)id; (void)type;
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__ns1__getProfessionalReportIdRequest), type))
	    return soap->error;
	if (a->reportRequestType)
	{	if (soap_out_PointerTons1__ReportRequestType(soap, "reportRequestType", -1, &a->reportRequestType, ""))
	    return soap->error;
	}
	else if (soap_element_nil(soap, "reportRequestType"))
	    return soap->error;
	return soap_element_end_out(soap, tag);
    }

    SOAP_FMAC3 struct _ns1__getProfessionalReportIdRequest * SOAP_FMAC4 soap_in__ns1__getProfessionalReportIdRequest(struct soap *soap, const char *tag, struct _ns1__getProfessionalReportIdRequest *a, const char *type)
    {
	size_t soap_flag_reportRequestType = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
	    return NULL;
	a = (struct _ns1__getProfessionalReportIdRequest *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__ns1__getProfessionalReportIdRequest, sizeof(struct _ns1__getProfessionalReportIdRequest), 0, NULL, NULL, NULL);
	if (!a)
	    return NULL;
	soap_default__ns1__getProfessionalReportIdRequest(soap, a);
	if (soap->body && !*soap->href)
	{
	    for (;;)
	    {	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_reportRequestType && soap->error == SOAP_TAG_MISMATCH)
		    if (soap_in_PointerTons1__ReportRequestType(soap, "reportRequestType", &a->reportRequestType, "ns1:ReportRequestType"))
		    {	soap_flag_reportRequestType--;
			continue;
		    }
		if (soap->error == SOAP_TAG_MISMATCH)
		    soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
		    break;
		if (soap->error)
		    return NULL;
	    }
	    if (soap_element_end_in(soap, tag))
		return NULL;
	}
	else
	{	a = (struct _ns1__getProfessionalReportIdRequest *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__ns1__getProfessionalReportIdRequest, 0, sizeof(struct _ns1__getProfessionalReportIdRequest), 0, NULL);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_reportRequestType > 0))
	{	soap->error = SOAP_OCCURS;
	    return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put__ns1__getProfessionalReportIdRequest(struct soap *soap, const struct _ns1__getProfessionalReportIdRequest *a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__ns1__getProfessionalReportIdRequest);
	if (soap_out__ns1__getProfessionalReportIdRequest(soap, tag?tag:"ns1:getProfessionalReportIdRequest", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 struct _ns1__getProfessionalReportIdRequest * SOAP_FMAC4 soap_get__ns1__getProfessionalReportIdRequest(struct soap *soap, struct _ns1__getProfessionalReportIdRequest *p, const char *tag, const char *type)
    {
	if ((p = soap_in__ns1__getProfessionalReportIdRequest(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns1__AttributeType(struct soap *soap, struct ns1__AttributeType *a)
    {
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_string(soap, &a->key);
	a->__sizevalue = 0;
	a->value = NULL;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns1__AttributeType(struct soap *soap, const struct ns1__AttributeType *a)
    {
#ifndef WITH_NOIDREF
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_string(soap, &a->key);
	if (a->value)
	{	int i;
	    for (i = 0; i < a->__sizevalue; i++)
	    {
		soap_embedded(soap, a->value + i, SOAP_TYPE_int);
	    }
	}
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns1__AttributeType(struct soap *soap, const char *tag, int id, const struct ns1__AttributeType *a, const char *type)
    {
	(void)soap; (void)tag; (void)id; (void)type;
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns1__AttributeType), type))
	    return soap->error;
	if (soap_out_string(soap, "key", -1, &a->key, ""))
	    return soap->error;
	if (a->value)
	{	int i;
	    for (i = 0; i < a->__sizevalue; i++)
		if (soap_out_int(soap, "value", -1, a->value + i, ""))
		    return soap->error;
	}
	return soap_element_end_out(soap, tag);
    }

    SOAP_FMAC3 struct ns1__AttributeType * SOAP_FMAC4 soap_in_ns1__AttributeType(struct soap *soap, const char *tag, struct ns1__AttributeType *a, const char *type)
    {
	size_t soap_flag_key = 1;
	struct soap_blist *soap_blist_value = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
	    return NULL;
	a = (struct ns1__AttributeType *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns1__AttributeType, sizeof(struct ns1__AttributeType), 0, NULL, NULL, NULL);
	if (!a)
	    return NULL;
	soap_default_ns1__AttributeType(soap, a);
	if (soap->body && !*soap->href)
	{
	    for (;;)
	    {	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_key && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
		    if (soap_in_string(soap, "key", &a->key, "xsd:string"))
		    {	soap_flag_key--;
			continue;
		    }
		if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "value", 1, NULL))
		{	if (a->value == NULL)
		    {	if (soap_blist_value == NULL)
			soap_blist_value = soap_new_block(soap);
			a->value = (int *)soap_push_block(soap, soap_blist_value, sizeof(int));
			if (a->value == NULL)
			    return NULL;
			soap_default_int(soap, a->value);
		    }
		    soap_revert(soap);
		    if (soap_in_int(soap, "value", a->value, "xsd:int"))
		    {	a->__sizevalue++;
			a->value = NULL;
			continue;
		    }
		}
		if (soap->error == SOAP_TAG_MISMATCH)
		    soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
		    break;
		if (soap->error)
		    return NULL;
	    }
	    if (a->value)
		soap_pop_block(soap, soap_blist_value);
	    if (a->__sizevalue)
		a->value = (int *)soap_save_block(soap, soap_blist_value, NULL, 1);
	    else
	    {	a->value = NULL;
		if (soap_blist_value)
		    soap_end_block(soap, soap_blist_value);
	    }
	    if (soap_element_end_in(soap, tag))
		return NULL;
	}
	else
	{	a = (struct ns1__AttributeType *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_ns1__AttributeType, 0, sizeof(struct ns1__AttributeType), 0, NULL);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns1__AttributeType(struct soap *soap, const struct ns1__AttributeType *a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns1__AttributeType);
	if (soap_out_ns1__AttributeType(soap, tag?tag:"ns1:AttributeType", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 struct ns1__AttributeType * SOAP_FMAC4 soap_get_ns1__AttributeType(struct soap *soap, struct ns1__AttributeType *p, const char *tag, const char *type)
    {
	if ((p = soap_in_ns1__AttributeType(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns1__ReportRequestType(struct soap *soap, struct ns1__ReportRequestType *a)
    {
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->__sizeperformanceData = 0;
	a->performanceData = NULL;
	soap_default_time(soap, &a->startDate);
	soap_default_time(soap, &a->endDate);
	a->idOnly = NULL;
	a->levelOfDetails = NULL;
	a->__sizeattributes = 0;
	a->attributes = NULL;
	a->format = NULL;
	soap_default_int(soap, &a->reportType);
	a->__sizestatIds = 0;
	a->statIds = NULL;
	a->statRange = NULL;
	a->unitOfTime = NULL;
	a->platform = NULL;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns1__ReportRequestType(struct soap *soap, const struct ns1__ReportRequestType *a)
    {
#ifndef WITH_NOIDREF
	(void)soap; (void)a; /* appease -Wall -Werror */
	if (a->performanceData)
	{	int i;
	    for (i = 0; i < a->__sizeperformanceData; i++)
	    {
		soap_serialize_string(soap, a->performanceData + i);
	    }
	}
	soap_serialize_PointerToxsd__boolean(soap, &a->idOnly);
	soap_serialize_PointerToint(soap, &a->levelOfDetails);
	if (a->attributes)
	{	int i;
	    for (i = 0; i < a->__sizeattributes; i++)
	    {
		soap_embedded(soap, a->attributes + i, SOAP_TYPE_ns1__AttributeType);
		soap_serialize_ns1__AttributeType(soap, a->attributes + i);
	    }
	}
	soap_serialize_PointerToint(soap, &a->format);
	soap_embedded(soap, &a->reportType, SOAP_TYPE_int);
	if (a->statIds)
	{	int i;
	    for (i = 0; i < a->__sizestatIds; i++)
	    {
		soap_embedded(soap, a->statIds + i, SOAP_TYPE_LONG64);
	    }
	}
	soap_serialize_PointerToint(soap, &a->statRange);
	soap_serialize_PointerToint(soap, &a->unitOfTime);
	soap_serialize_PointerToint(soap, &a->platform);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns1__ReportRequestType(struct soap *soap, const char *tag, int id, const struct ns1__ReportRequestType *a, const char *type)
    {
	(void)soap; (void)tag; (void)id; (void)type;
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_ns1__ReportRequestType), type))
	    return soap->error;
	if (a->performanceData)
	{	int i;
	    for (i = 0; i < a->__sizeperformanceData; i++) {

		if (soap_out_string(soap, "performanceData", -1, a->performanceData + i, ""))
		    return soap->error;
	    }
	}
	if (soap_out_time(soap, "startDate", -1, &a->startDate, ""))
	    return soap->error;
	if (soap_out_time(soap, "endDate", -1, &a->endDate, ""))
	    return soap->error;
	if (soap_out_PointerToxsd__boolean(soap, "idOnly", -1, &a->idOnly, ""))
	    return soap->error;
	if (soap_out_PointerToint(soap, "levelOfDetails", -1, &a->levelOfDetails, ""))
	    return soap->error;
	if (a->attributes)
	{	int i;
	    for (i = 0; i < a->__sizeattributes; i++)
		if (soap_out_ns1__AttributeType(soap, "attributes", -1, a->attributes + i, ""))
		    return soap->error;
	}
	if (soap_out_PointerToint(soap, "format", -1, &a->format, ""))
	    return soap->error;
	if (soap_out_int(soap, "reportType", -1, &a->reportType, ""))
	    return soap->error;
	if (a->statIds)
	{	int i;
	    for (i = 0; i < a->__sizestatIds; i++)
		if (soap_out_LONG64(soap, "statIds", -1, a->statIds + i, ""))
		    return soap->error;
	}
	if (soap_out_PointerToint(soap, "statRange", -1, &a->statRange, ""))
	    return soap->error;
	if (soap_out_PointerToint(soap, "unitOfTime", -1, &a->unitOfTime, ""))
	    return soap->error;
	if (soap_out_PointerToint(soap, "platform", -1, &a->platform, ""))
	    return soap->error;
	return soap_element_end_out(soap, tag);
    }

    SOAP_FMAC3 struct ns1__ReportRequestType * SOAP_FMAC4 soap_in_ns1__ReportRequestType(struct soap *soap, const char *tag, struct ns1__ReportRequestType *a, const char *type)
    {
	struct soap_blist *soap_blist_performanceData = NULL;
	size_t soap_flag_startDate = 1;
	size_t soap_flag_endDate = 1;
	size_t soap_flag_idOnly = 1;
	size_t soap_flag_levelOfDetails = 1;
	struct soap_blist *soap_blist_attributes = NULL;
	size_t soap_flag_format = 1;
	size_t soap_flag_reportType = 1;
	struct soap_blist *soap_blist_statIds = NULL;
	size_t soap_flag_statRange = 1;
	size_t soap_flag_unitOfTime = 1;
	size_t soap_flag_platform = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
	    return NULL;
	a = (struct ns1__ReportRequestType *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_ns1__ReportRequestType, sizeof(struct ns1__ReportRequestType), 0, NULL, NULL, NULL);
	if (!a)
	    return NULL;
	soap_default_ns1__ReportRequestType(soap, a);
	if (soap->body && !*soap->href)
	{
	    for (;;)
	    {	soap->error = SOAP_TAG_MISMATCH;
		if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "performanceData", 1, NULL))
		{	if (a->performanceData == NULL)
		    {	if (soap_blist_performanceData == NULL)
			soap_blist_performanceData = soap_new_block(soap);
			a->performanceData = (char **)soap_push_block(soap, soap_blist_performanceData, sizeof(char *));
			if (a->performanceData == NULL)
			    return NULL;
			*a->performanceData = NULL;
		    }
		    soap_revert(soap);
		    if (soap_in_string(soap, "performanceData", a->performanceData, "xsd:string"))
		    {	a->__sizeperformanceData++;
			a->performanceData = NULL;
			continue;
		    }
		}
		if (soap_flag_startDate && soap->error == SOAP_TAG_MISMATCH)
		    if (soap_in_time(soap, "startDate", &a->startDate, "xsd:dateTime"))
		    {	soap_flag_startDate--;
			continue;
		    }
		if (soap_flag_endDate && soap->error == SOAP_TAG_MISMATCH)
		    if (soap_in_time(soap, "endDate", &a->endDate, "xsd:dateTime"))
		    {	soap_flag_endDate--;
			continue;
		    }
		if (soap_flag_idOnly && soap->error == SOAP_TAG_MISMATCH)
		    if (soap_in_PointerToxsd__boolean(soap, "idOnly", &a->idOnly, "xsd:boolean"))
		    {	soap_flag_idOnly--;
			continue;
		    }
		if (soap_flag_levelOfDetails && soap->error == SOAP_TAG_MISMATCH)
		    if (soap_in_PointerToint(soap, "levelOfDetails", &a->levelOfDetails, "xsd:int"))
		    {	soap_flag_levelOfDetails--;
			continue;
		    }
		if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "attributes", 1, NULL))
		{	if (a->attributes == NULL)
		    {	if (soap_blist_attributes == NULL)
			soap_blist_attributes = soap_new_block(soap);
			a->attributes = (struct ns1__AttributeType *)soap_push_block(soap, soap_blist_attributes, sizeof(struct ns1__AttributeType));
			if (a->attributes == NULL)
			    return NULL;
			soap_default_ns1__AttributeType(soap, a->attributes);
		    }
		    soap_revert(soap);
		    if (soap_in_ns1__AttributeType(soap, "attributes", a->attributes, "ns1:AttributeType"))
		    {	a->__sizeattributes++;
			a->attributes = NULL;
			continue;
		    }
		}
		if (soap_flag_format && soap->error == SOAP_TAG_MISMATCH)
		    if (soap_in_PointerToint(soap, "format", &a->format, "xsd:int"))
		    {	soap_flag_format--;
			continue;
		    }
		if (soap_flag_reportType && soap->error == SOAP_TAG_MISMATCH)
		    if (soap_in_int(soap, "reportType", &a->reportType, "xsd:int"))
		    {	soap_flag_reportType--;
			continue;
		    }
		if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "statIds", 1, NULL))
		{	if (a->statIds == NULL)
		    {	if (soap_blist_statIds == NULL)
			soap_blist_statIds = soap_new_block(soap);
			a->statIds = (LONG64 *)soap_push_block(soap, soap_blist_statIds, sizeof(LONG64));
			if (a->statIds == NULL)
			    return NULL;
			soap_default_LONG64(soap, a->statIds);
		    }
		    soap_revert(soap);
		    if (soap_in_LONG64(soap, "statIds", a->statIds, "xsd:long"))
		    {	a->__sizestatIds++;
			a->statIds = NULL;
			continue;
		    }
		}
		if (soap_flag_statRange && soap->error == SOAP_TAG_MISMATCH)
		    if (soap_in_PointerToint(soap, "statRange", &a->statRange, "xsd:int"))
		    {	soap_flag_statRange--;
			continue;
		    }
		if (soap_flag_unitOfTime && soap->error == SOAP_TAG_MISMATCH)
		    if (soap_in_PointerToint(soap, "unitOfTime", &a->unitOfTime, "xsd:int"))
		    {	soap_flag_unitOfTime--;
			continue;
		    }
		if (soap_flag_platform && soap->error == SOAP_TAG_MISMATCH)
		    if (soap_in_PointerToint(soap, "platform", &a->platform, "xsd:int"))
		    {	soap_flag_platform--;
			continue;
		    }
		if (soap->error == SOAP_TAG_MISMATCH)
		    soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
		    break;
		if (soap->error)
		    return NULL;
	    }
	    if (a->performanceData)
		soap_pop_block(soap, soap_blist_performanceData);
	    if (a->__sizeperformanceData)
		a->performanceData = (char **)soap_save_block(soap, soap_blist_performanceData, NULL, 1);
	    else
	    {	a->performanceData = NULL;
		if (soap_blist_performanceData)
		    soap_end_block(soap, soap_blist_performanceData);
	    }
	    if (a->attributes)
		soap_pop_block(soap, soap_blist_attributes);
	    if (a->__sizeattributes)
		a->attributes = (struct ns1__AttributeType *)soap_save_block(soap, soap_blist_attributes, NULL, 1);
	    else
	    {	a->attributes = NULL;
		if (soap_blist_attributes)
		    soap_end_block(soap, soap_blist_attributes);
	    }
	    if (a->statIds)
		soap_pop_block(soap, soap_blist_statIds);
	    if (a->__sizestatIds)
		a->statIds = (LONG64 *)soap_save_block(soap, soap_blist_statIds, NULL, 1);
	    else
	    {	a->statIds = NULL;
		if (soap_blist_statIds)
		    soap_end_block(soap, soap_blist_statIds);
	    }
	    if (soap_element_end_in(soap, tag))
		return NULL;
	}
	else
	{	a = (struct ns1__ReportRequestType *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_ns1__ReportRequestType, 0, sizeof(struct ns1__ReportRequestType), 0, NULL);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (a->__sizeperformanceData < 1 || soap_flag_startDate > 0 || soap_flag_endDate > 0 || soap_flag_reportType > 0))
	{	soap->error = SOAP_OCCURS;
	    return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns1__ReportRequestType(struct soap *soap, const struct ns1__ReportRequestType *a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_ns1__ReportRequestType);
	if (soap_out_ns1__ReportRequestType(soap, tag?tag:"ns1:ReportRequestType", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 struct ns1__ReportRequestType * SOAP_FMAC4 soap_get_ns1__ReportRequestType(struct soap *soap, struct ns1__ReportRequestType *p, const char *tag, const char *type)
    {
	if ((p = soap_in_ns1__ReportRequestType(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

#ifndef WITH_NOGLOBAL

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerToSOAP_ENV__Reason(struct soap *soap, struct SOAP_ENV__Reason *const*a)
    {
#ifndef WITH_NOIDREF
	if (!soap_reference(soap, *a, SOAP_TYPE_SOAP_ENV__Reason))
	    soap_serialize_SOAP_ENV__Reason(soap, *a);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerToSOAP_ENV__Reason(struct soap *soap, const char *tag, int id, struct SOAP_ENV__Reason *const*a, const char *type)
    {
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_SOAP_ENV__Reason);
	if (id < 0)
	    return soap->error;
	return soap_out_SOAP_ENV__Reason(soap, tag, id, *a, type);
    }

    SOAP_FMAC3 struct SOAP_ENV__Reason ** SOAP_FMAC4 soap_in_PointerToSOAP_ENV__Reason(struct soap *soap, const char *tag, struct SOAP_ENV__Reason **a, const char *type)
    {
	if (soap_element_begin_in(soap, tag, 1, NULL))
	    return NULL;
	if (!a)
	    if (!(a = (struct SOAP_ENV__Reason **)soap_malloc(soap, sizeof(struct SOAP_ENV__Reason *))))
		return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	    if (!(*a = soap_in_SOAP_ENV__Reason(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct SOAP_ENV__Reason **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_SOAP_ENV__Reason, sizeof(struct SOAP_ENV__Reason), 0);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerToSOAP_ENV__Reason(struct soap *soap, struct SOAP_ENV__Reason *const*a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerToSOAP_ENV__Reason);
	if (soap_out_PointerToSOAP_ENV__Reason(soap, tag?tag:"SOAP-ENV:Reason", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 struct SOAP_ENV__Reason ** SOAP_FMAC4 soap_get_PointerToSOAP_ENV__Reason(struct soap *soap, struct SOAP_ENV__Reason **p, const char *tag, const char *type)
    {
	if ((p = soap_in_PointerToSOAP_ENV__Reason(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

#endif

#ifndef WITH_NOGLOBAL

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerToSOAP_ENV__Detail(struct soap *soap, struct SOAP_ENV__Detail *const*a)
    {
#ifndef WITH_NOIDREF
	if (!soap_reference(soap, *a, SOAP_TYPE_SOAP_ENV__Detail))
	    soap_serialize_SOAP_ENV__Detail(soap, *a);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerToSOAP_ENV__Detail(struct soap *soap, const char *tag, int id, struct SOAP_ENV__Detail *const*a, const char *type)
    {
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_SOAP_ENV__Detail);
	if (id < 0)
	    return soap->error;
	return soap_out_SOAP_ENV__Detail(soap, tag, id, *a, type);
    }

    SOAP_FMAC3 struct SOAP_ENV__Detail ** SOAP_FMAC4 soap_in_PointerToSOAP_ENV__Detail(struct soap *soap, const char *tag, struct SOAP_ENV__Detail **a, const char *type)
    {
	if (soap_element_begin_in(soap, tag, 1, NULL))
	    return NULL;
	if (!a)
	    if (!(a = (struct SOAP_ENV__Detail **)soap_malloc(soap, sizeof(struct SOAP_ENV__Detail *))))
		return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	    if (!(*a = soap_in_SOAP_ENV__Detail(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct SOAP_ENV__Detail **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_SOAP_ENV__Detail, sizeof(struct SOAP_ENV__Detail), 0);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerToSOAP_ENV__Detail(struct soap *soap, struct SOAP_ENV__Detail *const*a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerToSOAP_ENV__Detail);
	if (soap_out_PointerToSOAP_ENV__Detail(soap, tag?tag:"SOAP-ENV:Detail", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 struct SOAP_ENV__Detail ** SOAP_FMAC4 soap_get_PointerToSOAP_ENV__Detail(struct soap *soap, struct SOAP_ENV__Detail **p, const char *tag, const char *type)
    {
	if ((p = soap_in_PointerToSOAP_ENV__Detail(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

#endif

#ifndef WITH_NOGLOBAL

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerToSOAP_ENV__Code(struct soap *soap, struct SOAP_ENV__Code *const*a)
    {
#ifndef WITH_NOIDREF
	if (!soap_reference(soap, *a, SOAP_TYPE_SOAP_ENV__Code))
	    soap_serialize_SOAP_ENV__Code(soap, *a);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerToSOAP_ENV__Code(struct soap *soap, const char *tag, int id, struct SOAP_ENV__Code *const*a, const char *type)
    {
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_SOAP_ENV__Code);
	if (id < 0)
	    return soap->error;
	return soap_out_SOAP_ENV__Code(soap, tag, id, *a, type);
    }

    SOAP_FMAC3 struct SOAP_ENV__Code ** SOAP_FMAC4 soap_in_PointerToSOAP_ENV__Code(struct soap *soap, const char *tag, struct SOAP_ENV__Code **a, const char *type)
    {
	if (soap_element_begin_in(soap, tag, 1, NULL))
	    return NULL;
	if (!a)
	    if (!(a = (struct SOAP_ENV__Code **)soap_malloc(soap, sizeof(struct SOAP_ENV__Code *))))
		return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	    if (!(*a = soap_in_SOAP_ENV__Code(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct SOAP_ENV__Code **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_SOAP_ENV__Code, sizeof(struct SOAP_ENV__Code), 0);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerToSOAP_ENV__Code(struct soap *soap, struct SOAP_ENV__Code *const*a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerToSOAP_ENV__Code);
	if (soap_out_PointerToSOAP_ENV__Code(soap, tag?tag:"SOAP-ENV:Code", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 struct SOAP_ENV__Code ** SOAP_FMAC4 soap_get_PointerToSOAP_ENV__Code(struct soap *soap, struct SOAP_ENV__Code **p, const char *tag, const char *type)
    {
	if ((p = soap_in_PointerToSOAP_ENV__Code(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

#endif

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_ns1__getReportFileUrlResponse(struct soap *soap, struct _ns1__getReportFileUrlResponse *const*a)
    {
#ifndef WITH_NOIDREF
	if (!soap_reference(soap, *a, SOAP_TYPE__ns1__getReportFileUrlResponse))
	    soap_serialize__ns1__getReportFileUrlResponse(soap, *a);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_ns1__getReportFileUrlResponse(struct soap *soap, const char *tag, int id, struct _ns1__getReportFileUrlResponse *const*a, const char *type)
    {
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__ns1__getReportFileUrlResponse);
	if (id < 0)
	    return soap->error;
	return soap_out__ns1__getReportFileUrlResponse(soap, tag, id, *a, type);
    }

    SOAP_FMAC3 struct _ns1__getReportFileUrlResponse ** SOAP_FMAC4 soap_in_PointerTo_ns1__getReportFileUrlResponse(struct soap *soap, const char *tag, struct _ns1__getReportFileUrlResponse **a, const char *type)
    {
	if (soap_element_begin_in(soap, tag, 1, NULL))
	    return NULL;
	if (!a)
	    if (!(a = (struct _ns1__getReportFileUrlResponse **)soap_malloc(soap, sizeof(struct _ns1__getReportFileUrlResponse *))))
		return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	    if (!(*a = soap_in__ns1__getReportFileUrlResponse(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct _ns1__getReportFileUrlResponse **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__ns1__getReportFileUrlResponse, sizeof(struct _ns1__getReportFileUrlResponse), 0);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_ns1__getReportFileUrlResponse(struct soap *soap, struct _ns1__getReportFileUrlResponse *const*a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_ns1__getReportFileUrlResponse);
	if (soap_out_PointerTo_ns1__getReportFileUrlResponse(soap, tag?tag:"ns1:getReportFileUrlResponse", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 struct _ns1__getReportFileUrlResponse ** SOAP_FMAC4 soap_get_PointerTo_ns1__getReportFileUrlResponse(struct soap *soap, struct _ns1__getReportFileUrlResponse **p, const char *tag, const char *type)
    {
	if ((p = soap_in_PointerTo_ns1__getReportFileUrlResponse(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_ns1__getReportFileUrlRequest(struct soap *soap, struct _ns1__getReportFileUrlRequest *const*a)
    {
#ifndef WITH_NOIDREF
	if (!soap_reference(soap, *a, SOAP_TYPE__ns1__getReportFileUrlRequest))
	    soap_serialize__ns1__getReportFileUrlRequest(soap, *a);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_ns1__getReportFileUrlRequest(struct soap *soap, const char *tag, int id, struct _ns1__getReportFileUrlRequest *const*a, const char *type)
    {
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__ns1__getReportFileUrlRequest);
	if (id < 0)
	    return soap->error;
	return soap_out__ns1__getReportFileUrlRequest(soap, tag, id, *a, type);
    }

    SOAP_FMAC3 struct _ns1__getReportFileUrlRequest ** SOAP_FMAC4 soap_in_PointerTo_ns1__getReportFileUrlRequest(struct soap *soap, const char *tag, struct _ns1__getReportFileUrlRequest **a, const char *type)
    {
	if (soap_element_begin_in(soap, tag, 1, NULL))
	    return NULL;
	if (!a)
	    if (!(a = (struct _ns1__getReportFileUrlRequest **)soap_malloc(soap, sizeof(struct _ns1__getReportFileUrlRequest *))))
		return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	    if (!(*a = soap_in__ns1__getReportFileUrlRequest(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct _ns1__getReportFileUrlRequest **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__ns1__getReportFileUrlRequest, sizeof(struct _ns1__getReportFileUrlRequest), 0);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_ns1__getReportFileUrlRequest(struct soap *soap, struct _ns1__getReportFileUrlRequest *const*a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_ns1__getReportFileUrlRequest);
	if (soap_out_PointerTo_ns1__getReportFileUrlRequest(soap, tag?tag:"ns1:getReportFileUrlRequest", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 struct _ns1__getReportFileUrlRequest ** SOAP_FMAC4 soap_get_PointerTo_ns1__getReportFileUrlRequest(struct soap *soap, struct _ns1__getReportFileUrlRequest **p, const char *tag, const char *type)
    {
	if ((p = soap_in_PointerTo_ns1__getReportFileUrlRequest(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_ns1__getReportStateResponse(struct soap *soap, struct _ns1__getReportStateResponse *const*a)
    {
#ifndef WITH_NOIDREF
	if (!soap_reference(soap, *a, SOAP_TYPE__ns1__getReportStateResponse))
	    soap_serialize__ns1__getReportStateResponse(soap, *a);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_ns1__getReportStateResponse(struct soap *soap, const char *tag, int id, struct _ns1__getReportStateResponse *const*a, const char *type)
    {
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__ns1__getReportStateResponse);
	if (id < 0)
	    return soap->error;
	return soap_out__ns1__getReportStateResponse(soap, tag, id, *a, type);
    }

    SOAP_FMAC3 struct _ns1__getReportStateResponse ** SOAP_FMAC4 soap_in_PointerTo_ns1__getReportStateResponse(struct soap *soap, const char *tag, struct _ns1__getReportStateResponse **a, const char *type)
    {
	if (soap_element_begin_in(soap, tag, 1, NULL))
	    return NULL;
	if (!a)
	    if (!(a = (struct _ns1__getReportStateResponse **)soap_malloc(soap, sizeof(struct _ns1__getReportStateResponse *))))
		return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	    if (!(*a = soap_in__ns1__getReportStateResponse(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct _ns1__getReportStateResponse **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__ns1__getReportStateResponse, sizeof(struct _ns1__getReportStateResponse), 0);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_ns1__getReportStateResponse(struct soap *soap, struct _ns1__getReportStateResponse *const*a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_ns1__getReportStateResponse);
	if (soap_out_PointerTo_ns1__getReportStateResponse(soap, tag?tag:"ns1:getReportStateResponse", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 struct _ns1__getReportStateResponse ** SOAP_FMAC4 soap_get_PointerTo_ns1__getReportStateResponse(struct soap *soap, struct _ns1__getReportStateResponse **p, const char *tag, const char *type)
    {
	if ((p = soap_in_PointerTo_ns1__getReportStateResponse(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_ns1__getReportStateRequest(struct soap *soap, struct _ns1__getReportStateRequest *const*a)
    {
#ifndef WITH_NOIDREF
	if (!soap_reference(soap, *a, SOAP_TYPE__ns1__getReportStateRequest))
	    soap_serialize__ns1__getReportStateRequest(soap, *a);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_ns1__getReportStateRequest(struct soap *soap, const char *tag, int id, struct _ns1__getReportStateRequest *const*a, const char *type)
    {
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__ns1__getReportStateRequest);
	if (id < 0)
	    return soap->error;
	return soap_out__ns1__getReportStateRequest(soap, tag, id, *a, type);
    }

    SOAP_FMAC3 struct _ns1__getReportStateRequest ** SOAP_FMAC4 soap_in_PointerTo_ns1__getReportStateRequest(struct soap *soap, const char *tag, struct _ns1__getReportStateRequest **a, const char *type)
    {
	if (soap_element_begin_in(soap, tag, 1, NULL))
	    return NULL;
	if (!a)
	    if (!(a = (struct _ns1__getReportStateRequest **)soap_malloc(soap, sizeof(struct _ns1__getReportStateRequest *))))
		return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	    if (!(*a = soap_in__ns1__getReportStateRequest(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct _ns1__getReportStateRequest **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__ns1__getReportStateRequest, sizeof(struct _ns1__getReportStateRequest), 0);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_ns1__getReportStateRequest(struct soap *soap, struct _ns1__getReportStateRequest *const*a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_ns1__getReportStateRequest);
	if (soap_out_PointerTo_ns1__getReportStateRequest(soap, tag?tag:"ns1:getReportStateRequest", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 struct _ns1__getReportStateRequest ** SOAP_FMAC4 soap_get_PointerTo_ns1__getReportStateRequest(struct soap *soap, struct _ns1__getReportStateRequest **p, const char *tag, const char *type)
    {
	if ((p = soap_in_PointerTo_ns1__getReportStateRequest(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_ns1__getProfessionalReportIdResponse(struct soap *soap, struct _ns1__getProfessionalReportIdResponse *const*a)
    {
#ifndef WITH_NOIDREF
	if (!soap_reference(soap, *a, SOAP_TYPE__ns1__getProfessionalReportIdResponse))
	    soap_serialize__ns1__getProfessionalReportIdResponse(soap, *a);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_ns1__getProfessionalReportIdResponse(struct soap *soap, const char *tag, int id, struct _ns1__getProfessionalReportIdResponse *const*a, const char *type)
    {
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__ns1__getProfessionalReportIdResponse);
	if (id < 0)
	    return soap->error;
	return soap_out__ns1__getProfessionalReportIdResponse(soap, tag, id, *a, type);
    }

    SOAP_FMAC3 struct _ns1__getProfessionalReportIdResponse ** SOAP_FMAC4 soap_in_PointerTo_ns1__getProfessionalReportIdResponse(struct soap *soap, const char *tag, struct _ns1__getProfessionalReportIdResponse **a, const char *type)
    {
	if (soap_element_begin_in(soap, tag, 1, NULL))
	    return NULL;
	if (!a)
	    if (!(a = (struct _ns1__getProfessionalReportIdResponse **)soap_malloc(soap, sizeof(struct _ns1__getProfessionalReportIdResponse *))))
		return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	    if (!(*a = soap_in__ns1__getProfessionalReportIdResponse(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct _ns1__getProfessionalReportIdResponse **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__ns1__getProfessionalReportIdResponse, sizeof(struct _ns1__getProfessionalReportIdResponse), 0);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_ns1__getProfessionalReportIdResponse(struct soap *soap, struct _ns1__getProfessionalReportIdResponse *const*a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_ns1__getProfessionalReportIdResponse);
	if (soap_out_PointerTo_ns1__getProfessionalReportIdResponse(soap, tag?tag:"ns1:getProfessionalReportIdResponse", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 struct _ns1__getProfessionalReportIdResponse ** SOAP_FMAC4 soap_get_PointerTo_ns1__getProfessionalReportIdResponse(struct soap *soap, struct _ns1__getProfessionalReportIdResponse **p, const char *tag, const char *type)
    {
	if ((p = soap_in_PointerTo_ns1__getProfessionalReportIdResponse(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_ns1__getProfessionalReportIdRequest(struct soap *soap, struct _ns1__getProfessionalReportIdRequest *const*a)
    {
#ifndef WITH_NOIDREF
	if (!soap_reference(soap, *a, SOAP_TYPE__ns1__getProfessionalReportIdRequest))
	    soap_serialize__ns1__getProfessionalReportIdRequest(soap, *a);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_ns1__getProfessionalReportIdRequest(struct soap *soap, const char *tag, int id, struct _ns1__getProfessionalReportIdRequest *const*a, const char *type)
    {
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__ns1__getProfessionalReportIdRequest);
	if (id < 0)
	    return soap->error;
	return soap_out__ns1__getProfessionalReportIdRequest(soap, tag, id, *a, type);
    }

    SOAP_FMAC3 struct _ns1__getProfessionalReportIdRequest ** SOAP_FMAC4 soap_in_PointerTo_ns1__getProfessionalReportIdRequest(struct soap *soap, const char *tag, struct _ns1__getProfessionalReportIdRequest **a, const char *type)
    {
	if (soap_element_begin_in(soap, tag, 1, NULL))
	    return NULL;
	if (!a)
	    if (!(a = (struct _ns1__getProfessionalReportIdRequest **)soap_malloc(soap, sizeof(struct _ns1__getProfessionalReportIdRequest *))))
		return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	    if (!(*a = soap_in__ns1__getProfessionalReportIdRequest(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct _ns1__getProfessionalReportIdRequest **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__ns1__getProfessionalReportIdRequest, sizeof(struct _ns1__getProfessionalReportIdRequest), 0);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_ns1__getProfessionalReportIdRequest(struct soap *soap, struct _ns1__getProfessionalReportIdRequest *const*a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_ns1__getProfessionalReportIdRequest);
	if (soap_out_PointerTo_ns1__getProfessionalReportIdRequest(soap, tag?tag:"ns1:getProfessionalReportIdRequest", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 struct _ns1__getProfessionalReportIdRequest ** SOAP_FMAC4 soap_get_PointerTo_ns1__getProfessionalReportIdRequest(struct soap *soap, struct _ns1__getProfessionalReportIdRequest **p, const char *tag, const char *type)
    {
	if ((p = soap_in_PointerTo_ns1__getProfessionalReportIdRequest(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTons2__ResHeader(struct soap *soap, struct ns2__ResHeader *const*a)
    {
#ifndef WITH_NOIDREF
	if (!soap_reference(soap, *a, SOAP_TYPE_ns2__ResHeader))
	    soap_serialize_ns2__ResHeader(soap, *a);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTons2__ResHeader(struct soap *soap, const char *tag, int id, struct ns2__ResHeader *const*a, const char *type)
    {
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_ns2__ResHeader);
	if (id < 0)
	    return soap->error;
	return soap_out_ns2__ResHeader(soap, tag, id, *a, type);
    }

    SOAP_FMAC3 struct ns2__ResHeader ** SOAP_FMAC4 soap_in_PointerTons2__ResHeader(struct soap *soap, const char *tag, struct ns2__ResHeader **a, const char *type)
    {
	if (soap_element_begin_in(soap, tag, 1, NULL))
	    return NULL;
	if (!a)
	    if (!(a = (struct ns2__ResHeader **)soap_malloc(soap, sizeof(struct ns2__ResHeader *))))
		return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	    if (!(*a = soap_in_ns2__ResHeader(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct ns2__ResHeader **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_ns2__ResHeader, sizeof(struct ns2__ResHeader), 0);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}

	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTons2__ResHeader(struct soap *soap, struct ns2__ResHeader *const*a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTons2__ResHeader);
	if (soap_out_PointerTons2__ResHeader(soap, tag?tag:"ns2:ResHeader", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 struct ns2__ResHeader ** SOAP_FMAC4 soap_get_PointerTons2__ResHeader(struct soap *soap, struct ns2__ResHeader **p, const char *tag, const char *type)
    {
	if ((p = soap_in_PointerTons2__ResHeader(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTons2__AuthHeader(struct soap *soap, struct ns2__AuthHeader *const*a)
    {
#ifndef WITH_NOIDREF
	if (!soap_reference(soap, *a, SOAP_TYPE_ns2__AuthHeader))
	    soap_serialize_ns2__AuthHeader(soap, *a);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTons2__AuthHeader(struct soap *soap, const char *tag, int id, struct ns2__AuthHeader *const*a, const char *type)
    {
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_ns2__AuthHeader);
	if (id < 0)
	    return soap->error;
	return soap_out_ns2__AuthHeader(soap, tag, id, *a, type);
    }

    SOAP_FMAC3 struct ns2__AuthHeader ** SOAP_FMAC4 soap_in_PointerTons2__AuthHeader(struct soap *soap, const char *tag, struct ns2__AuthHeader **a, const char *type)
    {
	if (soap_element_begin_in(soap, tag, 1, NULL))
	    return NULL;
	if (!a)
	    if (!(a = (struct ns2__AuthHeader **)soap_malloc(soap, sizeof(struct ns2__AuthHeader *))))
		return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	    if (!(*a = soap_in_ns2__AuthHeader(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct ns2__AuthHeader **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_ns2__AuthHeader, sizeof(struct ns2__AuthHeader), 0);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTons2__AuthHeader(struct soap *soap, struct ns2__AuthHeader *const*a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTons2__AuthHeader);
	if (soap_out_PointerTons2__AuthHeader(soap, tag?tag:"ns2:AuthHeader", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 struct ns2__AuthHeader ** SOAP_FMAC4 soap_get_PointerTons2__AuthHeader(struct soap *soap, struct ns2__AuthHeader **p, const char *tag, const char *type)
    {
	if ((p = soap_in_PointerTons2__AuthHeader(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTons2__Failure(struct soap *soap, struct ns2__Failure *const*a)
    {
#ifndef WITH_NOIDREF
	if (!soap_reference(soap, *a, SOAP_TYPE_ns2__Failure))
	    soap_serialize_ns2__Failure(soap, *a);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTons2__Failure(struct soap *soap, const char *tag, int id, struct ns2__Failure *const*a, const char *type)
    {
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_ns2__Failure);
	if (id < 0)
	    return soap->error;
	return soap_out_ns2__Failure(soap, tag, id, *a, type);
    }

    SOAP_FMAC3 struct ns2__Failure ** SOAP_FMAC4 soap_in_PointerTons2__Failure(struct soap *soap, const char *tag, struct ns2__Failure **a, const char *type)
    {
	if (soap_element_begin_in(soap, tag, 1, NULL))
	    return NULL;
	if (!a)
	    if (!(a = (struct ns2__Failure **)soap_malloc(soap, sizeof(struct ns2__Failure *))))
		return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	    if (!(*a = soap_in_ns2__Failure(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct ns2__Failure **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_ns2__Failure, sizeof(struct ns2__Failure), 0);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTons2__Failure(struct soap *soap, struct ns2__Failure *const*a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTons2__Failure);
	if (soap_out_PointerTons2__Failure(soap, tag?tag:"ns2:Failure", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 struct ns2__Failure ** SOAP_FMAC4 soap_get_PointerTons2__Failure(struct soap *soap, struct ns2__Failure **p, const char *tag, const char *type)
    {
	if ((p = soap_in_PointerTons2__Failure(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTons2__DoubleMapItemType(struct soap *soap, struct ns2__DoubleMapItemType *const*a)
    {
#ifndef WITH_NOIDREF
	if (!soap_reference(soap, *a, SOAP_TYPE_ns2__DoubleMapItemType))
	    soap_serialize_ns2__DoubleMapItemType(soap, *a);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTons2__DoubleMapItemType(struct soap *soap, const char *tag, int id, struct ns2__DoubleMapItemType *const*a, const char *type)
    {
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_ns2__DoubleMapItemType);
	if (id < 0)
	    return soap->error;
	return soap_out_ns2__DoubleMapItemType(soap, tag, id, *a, type);
    }

    SOAP_FMAC3 struct ns2__DoubleMapItemType ** SOAP_FMAC4 soap_in_PointerTons2__DoubleMapItemType(struct soap *soap, const char *tag, struct ns2__DoubleMapItemType **a, const char *type)
    {
	if (soap_element_begin_in(soap, tag, 1, NULL))
	    return NULL;
	if (!a)
	    if (!(a = (struct ns2__DoubleMapItemType **)soap_malloc(soap, sizeof(struct ns2__DoubleMapItemType *))))
		return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	    if (!(*a = soap_in_ns2__DoubleMapItemType(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct ns2__DoubleMapItemType **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_ns2__DoubleMapItemType, sizeof(struct ns2__DoubleMapItemType), 0);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTons2__DoubleMapItemType(struct soap *soap, struct ns2__DoubleMapItemType *const*a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTons2__DoubleMapItemType);
	if (soap_out_PointerTons2__DoubleMapItemType(soap, tag?tag:"ns2:DoubleMapItemType", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 struct ns2__DoubleMapItemType ** SOAP_FMAC4 soap_get_PointerTons2__DoubleMapItemType(struct soap *soap, struct ns2__DoubleMapItemType **p, const char *tag, const char *type)
    {
	if ((p = soap_in_PointerTons2__DoubleMapItemType(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTons2__FloatMapItemType(struct soap *soap, struct ns2__FloatMapItemType *const*a)
    {
#ifndef WITH_NOIDREF
	if (!soap_reference(soap, *a, SOAP_TYPE_ns2__FloatMapItemType))
	    soap_serialize_ns2__FloatMapItemType(soap, *a);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTons2__FloatMapItemType(struct soap *soap, const char *tag, int id, struct ns2__FloatMapItemType *const*a, const char *type)
    {
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_ns2__FloatMapItemType);
	if (id < 0)
	    return soap->error;
	return soap_out_ns2__FloatMapItemType(soap, tag, id, *a, type);
    }

    SOAP_FMAC3 struct ns2__FloatMapItemType ** SOAP_FMAC4 soap_in_PointerTons2__FloatMapItemType(struct soap *soap, const char *tag, struct ns2__FloatMapItemType **a, const char *type)
    {
	if (soap_element_begin_in(soap, tag, 1, NULL))
	    return NULL;
	if (!a)
	    if (!(a = (struct ns2__FloatMapItemType **)soap_malloc(soap, sizeof(struct ns2__FloatMapItemType *))))
		return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	    if (!(*a = soap_in_ns2__FloatMapItemType(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct ns2__FloatMapItemType **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_ns2__FloatMapItemType, sizeof(struct ns2__FloatMapItemType), 0);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTons2__FloatMapItemType(struct soap *soap, struct ns2__FloatMapItemType *const*a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTons2__FloatMapItemType);
	if (soap_out_PointerTons2__FloatMapItemType(soap, tag?tag:"ns2:FloatMapItemType", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 struct ns2__FloatMapItemType ** SOAP_FMAC4 soap_get_PointerTons2__FloatMapItemType(struct soap *soap, struct ns2__FloatMapItemType **p, const char *tag, const char *type)
    {
	if ((p = soap_in_PointerTons2__FloatMapItemType(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTons2__LongMapItemType(struct soap *soap, struct ns2__LongMapItemType *const*a)
    {
#ifndef WITH_NOIDREF
	if (!soap_reference(soap, *a, SOAP_TYPE_ns2__LongMapItemType))
	    soap_serialize_ns2__LongMapItemType(soap, *a);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTons2__LongMapItemType(struct soap *soap, const char *tag, int id, struct ns2__LongMapItemType *const*a, const char *type)
    {
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_ns2__LongMapItemType);
	if (id < 0)
	    return soap->error;
	return soap_out_ns2__LongMapItemType(soap, tag, id, *a, type);
    }

    SOAP_FMAC3 struct ns2__LongMapItemType ** SOAP_FMAC4 soap_in_PointerTons2__LongMapItemType(struct soap *soap, const char *tag, struct ns2__LongMapItemType **a, const char *type)
    {
	if (soap_element_begin_in(soap, tag, 1, NULL))
	    return NULL;
	if (!a)
	    if (!(a = (struct ns2__LongMapItemType **)soap_malloc(soap, sizeof(struct ns2__LongMapItemType *))))
		return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	    if (!(*a = soap_in_ns2__LongMapItemType(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct ns2__LongMapItemType **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_ns2__LongMapItemType, sizeof(struct ns2__LongMapItemType), 0);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTons2__LongMapItemType(struct soap *soap, struct ns2__LongMapItemType *const*a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTons2__LongMapItemType);
	if (soap_out_PointerTons2__LongMapItemType(soap, tag?tag:"ns2:LongMapItemType", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 struct ns2__LongMapItemType ** SOAP_FMAC4 soap_get_PointerTons2__LongMapItemType(struct soap *soap, struct ns2__LongMapItemType **p, const char *tag, const char *type)
    {
	if ((p = soap_in_PointerTons2__LongMapItemType(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTons2__IntMapItemType(struct soap *soap, struct ns2__IntMapItemType *const*a)
    {
#ifndef WITH_NOIDREF
	if (!soap_reference(soap, *a, SOAP_TYPE_ns2__IntMapItemType))
	    soap_serialize_ns2__IntMapItemType(soap, *a);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTons2__IntMapItemType(struct soap *soap, const char *tag, int id, struct ns2__IntMapItemType *const*a, const char *type)
    {
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_ns2__IntMapItemType);
	if (id < 0)
	    return soap->error;
	return soap_out_ns2__IntMapItemType(soap, tag, id, *a, type);
    }

    SOAP_FMAC3 struct ns2__IntMapItemType ** SOAP_FMAC4 soap_in_PointerTons2__IntMapItemType(struct soap *soap, const char *tag, struct ns2__IntMapItemType **a, const char *type)
    {
	if (soap_element_begin_in(soap, tag, 1, NULL))
	    return NULL;
	if (!a)
	    if (!(a = (struct ns2__IntMapItemType **)soap_malloc(soap, sizeof(struct ns2__IntMapItemType *))))
		return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	    if (!(*a = soap_in_ns2__IntMapItemType(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct ns2__IntMapItemType **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_ns2__IntMapItemType, sizeof(struct ns2__IntMapItemType), 0);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTons2__IntMapItemType(struct soap *soap, struct ns2__IntMapItemType *const*a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTons2__IntMapItemType);
	if (soap_out_PointerTons2__IntMapItemType(soap, tag?tag:"ns2:IntMapItemType", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 struct ns2__IntMapItemType ** SOAP_FMAC4 soap_get_PointerTons2__IntMapItemType(struct soap *soap, struct ns2__IntMapItemType **p, const char *tag, const char *type)
    {
	if ((p = soap_in_PointerTons2__IntMapItemType(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTons2__StringMapItemType(struct soap *soap, struct ns2__StringMapItemType *const*a)
    {
#ifndef WITH_NOIDREF
	if (!soap_reference(soap, *a, SOAP_TYPE_ns2__StringMapItemType))
	    soap_serialize_ns2__StringMapItemType(soap, *a);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTons2__StringMapItemType(struct soap *soap, const char *tag, int id, struct ns2__StringMapItemType *const*a, const char *type)
    {
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_ns2__StringMapItemType);
	if (id < 0)
	    return soap->error;
	return soap_out_ns2__StringMapItemType(soap, tag, id, *a, type);
    }

    SOAP_FMAC3 struct ns2__StringMapItemType ** SOAP_FMAC4 soap_in_PointerTons2__StringMapItemType(struct soap *soap, const char *tag, struct ns2__StringMapItemType **a, const char *type)
    {
	if (soap_element_begin_in(soap, tag, 1, NULL))
	    return NULL;
	if (!a)
	    if (!(a = (struct ns2__StringMapItemType **)soap_malloc(soap, sizeof(struct ns2__StringMapItemType *))))
		return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	    if (!(*a = soap_in_ns2__StringMapItemType(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct ns2__StringMapItemType **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_ns2__StringMapItemType, sizeof(struct ns2__StringMapItemType), 0);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTons2__StringMapItemType(struct soap *soap, struct ns2__StringMapItemType *const*a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTons2__StringMapItemType);
	if (soap_out_PointerTons2__StringMapItemType(soap, tag?tag:"ns2:StringMapItemType", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 struct ns2__StringMapItemType ** SOAP_FMAC4 soap_get_PointerTons2__StringMapItemType(struct soap *soap, struct ns2__StringMapItemType **p, const char *tag, const char *type)
    {
	if ((p = soap_in_PointerTons2__StringMapItemType(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTons1__ReportRequestType(struct soap *soap, struct ns1__ReportRequestType *const*a)
    {
#ifndef WITH_NOIDREF
	if (!soap_reference(soap, *a, SOAP_TYPE_ns1__ReportRequestType))
	    soap_serialize_ns1__ReportRequestType(soap, *a);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTons1__ReportRequestType(struct soap *soap, const char *tag, int id, struct ns1__ReportRequestType *const*a, const char *type)
    {
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_ns1__ReportRequestType);
	if (id < 0)
	    return soap->error;
	return soap_out_ns1__ReportRequestType(soap, tag, id, *a, type);
    }

    SOAP_FMAC3 struct ns1__ReportRequestType ** SOAP_FMAC4 soap_in_PointerTons1__ReportRequestType(struct soap *soap, const char *tag, struct ns1__ReportRequestType **a, const char *type)
    {
	if (soap_element_begin_in(soap, tag, 1, NULL))
	    return NULL;
	if (!a)
	    if (!(a = (struct ns1__ReportRequestType **)soap_malloc(soap, sizeof(struct ns1__ReportRequestType *))))
		return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	    if (!(*a = soap_in_ns1__ReportRequestType(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct ns1__ReportRequestType **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_ns1__ReportRequestType, sizeof(struct ns1__ReportRequestType), 0);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTons1__ReportRequestType(struct soap *soap, struct ns1__ReportRequestType *const*a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTons1__ReportRequestType);
	if (soap_out_PointerTons1__ReportRequestType(soap, tag?tag:"ns1:ReportRequestType", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 struct ns1__ReportRequestType ** SOAP_FMAC4 soap_get_PointerTons1__ReportRequestType(struct soap *soap, struct ns1__ReportRequestType **p, const char *tag, const char *type)
    {
	if ((p = soap_in_PointerTons1__ReportRequestType(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerToLONG64(struct soap *soap, LONG64 *const*a)
    {
#ifndef WITH_NOIDREF
	soap_reference(soap, *a, SOAP_TYPE_LONG64);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerToLONG64(struct soap *soap, const char *tag, int id, LONG64 *const*a, const char *type)
    {
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_LONG64);
	if (id < 0)
	    return soap->error;
	return soap_out_LONG64(soap, tag, id, *a, type);
    }

    SOAP_FMAC3 LONG64 ** SOAP_FMAC4 soap_in_PointerToLONG64(struct soap *soap, const char *tag, LONG64 **a, const char *type)
    {
	if (soap_element_begin_in(soap, tag, 1, NULL))
	    return NULL;
	if (!a)
	    if (!(a = (LONG64 **)soap_malloc(soap, sizeof(LONG64 *))))
		return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	    if (!(*a = soap_in_LONG64(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (LONG64 **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_LONG64, sizeof(LONG64), 0);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerToLONG64(struct soap *soap, LONG64 *const*a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerToLONG64);
	if (soap_out_PointerToLONG64(soap, tag?tag:"long", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 LONG64 ** SOAP_FMAC4 soap_get_PointerToLONG64(struct soap *soap, LONG64 **p, const char *tag, const char *type)
    {
	if ((p = soap_in_PointerToLONG64(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTons1__AttributeType(struct soap *soap, struct ns1__AttributeType *const*a)
    {
#ifndef WITH_NOIDREF
	if (!soap_reference(soap, *a, SOAP_TYPE_ns1__AttributeType))
	    soap_serialize_ns1__AttributeType(soap, *a);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTons1__AttributeType(struct soap *soap, const char *tag, int id, struct ns1__AttributeType *const*a, const char *type)
    {
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_ns1__AttributeType);
	if (id < 0)
	    return soap->error;
	return soap_out_ns1__AttributeType(soap, tag, id, *a, type);
    }

    SOAP_FMAC3 struct ns1__AttributeType ** SOAP_FMAC4 soap_in_PointerTons1__AttributeType(struct soap *soap, const char *tag, struct ns1__AttributeType **a, const char *type)
    {
	if (soap_element_begin_in(soap, tag, 1, NULL))
	    return NULL;
	if (!a)
	    if (!(a = (struct ns1__AttributeType **)soap_malloc(soap, sizeof(struct ns1__AttributeType *))))
		return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	    if (!(*a = soap_in_ns1__AttributeType(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct ns1__AttributeType **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_ns1__AttributeType, sizeof(struct ns1__AttributeType), 0);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTons1__AttributeType(struct soap *soap, struct ns1__AttributeType *const*a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTons1__AttributeType);
	if (soap_out_PointerTons1__AttributeType(soap, tag?tag:"ns1:AttributeType", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 struct ns1__AttributeType ** SOAP_FMAC4 soap_get_PointerTons1__AttributeType(struct soap *soap, struct ns1__AttributeType **p, const char *tag, const char *type)
    {
	if ((p = soap_in_PointerTons1__AttributeType(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerToint(struct soap *soap, int *const*a)
    {
#ifndef WITH_NOIDREF
	soap_reference(soap, *a, SOAP_TYPE_int);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerToint(struct soap *soap, const char *tag, int id, int *const*a, const char *type)
    {
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_int);
	if (id < 0)
	    return soap->error;
	return soap_out_int(soap, tag, id, *a, type);
    }

    SOAP_FMAC3 int ** SOAP_FMAC4 soap_in_PointerToint(struct soap *soap, const char *tag, int **a, const char *type)
    {
	if (soap_element_begin_in(soap, tag, 1, NULL))
	    return NULL;
	if (!a)
	    if (!(a = (int **)soap_malloc(soap, sizeof(int *))))
		return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	    if (!(*a = soap_in_int(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (int **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_int, sizeof(int), 0);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerToint(struct soap *soap, int *const*a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerToint);
	if (soap_out_PointerToint(soap, tag?tag:"int", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 int ** SOAP_FMAC4 soap_get_PointerToint(struct soap *soap, int **p, const char *tag, const char *type)
    {
	if ((p = soap_in_PointerToint(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerToxsd__boolean(struct soap *soap, enum xsd__boolean *const*a)
    {
#ifndef WITH_NOIDREF
	soap_reference(soap, *a, SOAP_TYPE_xsd__boolean);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerToxsd__boolean(struct soap *soap, const char *tag, int id, enum xsd__boolean *const*a, const char *type)
    {
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_xsd__boolean);
	if (id < 0)
	    return soap->error;
	return soap_out_xsd__boolean(soap, tag, id, *a, type);
    }

    SOAP_FMAC3 enum xsd__boolean ** SOAP_FMAC4 soap_in_PointerToxsd__boolean(struct soap *soap, const char *tag, enum xsd__boolean **a, const char *type)
    {
	if (soap_element_begin_in(soap, tag, 1, NULL))
	    return NULL;
	if (!a)
	    if (!(a = (enum xsd__boolean **)soap_malloc(soap, sizeof(enum xsd__boolean *))))
		return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	    if (!(*a = soap_in_xsd__boolean(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (enum xsd__boolean **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_xsd__boolean, sizeof(enum xsd__boolean), 0);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerToxsd__boolean(struct soap *soap, enum xsd__boolean *const*a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerToxsd__boolean);
	if (soap_out_PointerToxsd__boolean(soap, tag?tag:"xsd:boolean", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 enum xsd__boolean ** SOAP_FMAC4 soap_get_PointerToxsd__boolean(struct soap *soap, enum xsd__boolean **p, const char *tag, const char *type)
    {
	if ((p = soap_in_PointerToxsd__boolean(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTostring(struct soap *soap, char **const*a)
    {
#ifndef WITH_NOIDREF
	if (!soap_reference(soap, *a, SOAP_TYPE_string))
	    soap_serialize_string(soap, *a);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTostring(struct soap *soap, const char *tag, int id, char **const*a, const char *type)
    {
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_string);
	if (id < 0)
	    return soap->error;
	return soap_out_string(soap, tag, id, *a, type);
    }

    SOAP_FMAC3 char *** SOAP_FMAC4 soap_in_PointerTostring(struct soap *soap, const char *tag, char ***a, const char *type)
    {
	if (soap_element_begin_in(soap, tag, 1, NULL))
	    return NULL;
	if (!a)
	    if (!(a = (char ***)soap_malloc(soap, sizeof(char **))))
		return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	    if (!(*a = soap_in_string(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (char ***)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_string, sizeof(char *), 1);
	    if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTostring(struct soap *soap, char **const*a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTostring);
	if (soap_out_PointerTostring(soap, tag?tag:"byte", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 char *** SOAP_FMAC4 soap_get_PointerTostring(struct soap *soap, char ***p, const char *tag, const char *type)
    {
	if ((p = soap_in_PointerTostring(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__QName(struct soap *soap, char *const*a)
    {
#ifndef WITH_NOIDREF
	soap_reference(soap, *a, SOAP_TYPE__QName);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out__QName(struct soap *soap, const char *tag, int id, char *const*a, const char *type)
    {
	return soap_outstring(soap, tag, id, a, type, SOAP_TYPE__QName);
    }

    SOAP_FMAC3 char * * SOAP_FMAC4 soap_in__QName(struct soap *soap, const char *tag, char **a, const char *type)
    {	char **p;
	p = soap_instring(soap, tag, a, type, SOAP_TYPE__QName, 2, 0, -1);
	return p;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put__QName(struct soap *soap, char *const*a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__QName);
	if (soap_out__QName(soap, tag?tag:"byte", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 char ** SOAP_FMAC4 soap_get__QName(struct soap *soap, char **p, const char *tag, const char *type)
    {
	if ((p = soap_in__QName(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_default_string(struct soap *soap, char **a)
    {
	(void)soap; /* appease -Wall -Werror */
#ifdef SOAP_DEFAULT_string
	*a = SOAP_DEFAULT_string;
#else
	*a = (char *)0;
#endif
    }

    SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_string(struct soap *soap, char *const*a)
    {
#ifndef WITH_NOIDREF
	soap_reference(soap, *a, SOAP_TYPE_string);
#endif
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_out_string(struct soap *soap, const char *tag, int id, char *const*a, const char *type)
    {
	return soap_outstring(soap, tag, id, a, type, SOAP_TYPE_string);
    }

    SOAP_FMAC3 char * * SOAP_FMAC4 soap_in_string(struct soap *soap, const char *tag, char **a, const char *type)
    {	char **p;
	p = soap_instring(soap, tag, a, type, SOAP_TYPE_string, 1, 0, -1);
	return p;
    }

    SOAP_FMAC3 int SOAP_FMAC4 soap_put_string(struct soap *soap, char *const*a, const char *tag, const char *type)
    {
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_string);
	if (soap_out_string(soap, tag?tag:"byte", id, a, type))
	    return soap->error;
	return soap_putindependent(soap);
    }

    SOAP_FMAC3 char ** SOAP_FMAC4 soap_get_string(struct soap *soap, char **p, const char *tag, const char *type)
    {
	if ((p = soap_in_string(soap, tag, p, type)))
	    if (soap_getindependent(soap))
		return NULL;
	return p;
    }

#ifdef __cplusplus
}
#endif

#if defined(__BORLANDC__)
#pragma option pop
#pragma option pop
#endif

