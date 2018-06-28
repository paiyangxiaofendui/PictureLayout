#ifndef TopSDKC_TopResponse_h
#define TopSDKC_TopResponse_h

#include "../topsdk.h"

void add_result_param(pTopResponse pt,char* key,char* value);

void set_msg(pTopResponse pt,char* value);

void set_submsg(pTopResponse pt,char* value);

void set_subcode(pTopResponse pt,char* value);

void set_request_id(pTopResponse pt,char* value);

#endif
