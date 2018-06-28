#include <stdlib.h>
#include <string.h>
#include "../topsdk.h"
#include "TopResponse.h"
#include "WebUtils.h"
#include "Md5Util.h"
#include "json.h"
#include "../Misc/Misc.h"

static int init_curl_count = 0;

pTaobaoClient alloc_taobao_client(const char* url,const char* appkey,const char* secret){
    int ret = 0;
    size_t len = 0;
    pTaobaoClient pClient = (pTaobaoClient) malloc(sizeof(TaobaoClient));
    if(!pClient)
        return NULL;
    if(ret < 0)
        return NULL;
    
    len = strlen(url);
    pClient->url = (char*) malloc(sizeof(char)*len+1);
    strcpy(pClient->url,url);
    pClient->url[len] = '\0';
    
    len = strlen(appkey);
    pClient->appkey = (char*) malloc(sizeof(char)*len+1);
    strcpy(pClient->appkey, appkey);
    pClient->appkey[len] = '\0';
    
    len = strlen(secret);
    pClient->appsecret = (char*) malloc(sizeof(char)*len+1);
    strcpy(pClient->appsecret, secret);
    pClient->appsecret[len] = '\0';
    
    
    if(init_curl_count == 0)
        curl_global_init(CURL_GLOBAL_ALL);
    init_curl_count ++;
    
    return pClient;
}

void destroy_taobao_client(pTaobaoClient pClient){
    if(!pClient)
        return;
    if(pClient->url)
        free(pClient->url);
    if(pClient->appkey)
        free(pClient->appkey);
    if(pClient->appsecret)
        free(pClient->appsecret);

    free(pClient);
    init_curl_count --;
    
    if(init_curl_count == 0)
        curl_global_cleanup();
    
}

void compeleteSystemParams(pTopRequest pRequest,char* appkey,char* appsecret,char* session){
    char* buffer;
    char sign[33];
    time_t result = time(NULL);
    char timestamps[32];
    int i ;
    unsigned long total_length = 0;
    sprintf(timestamps,"%ld",result);
    add_param(pRequest, "format", "json");
   
    //add_param(pRequest, "session", session);
    add_param(pRequest, "method", pRequest->apiName);
    add_param(pRequest, "v", "2.0");
    add_param(pRequest, "app_key", appkey);
    add_param(pRequest, "partner_id", "topsdkcpp");
    add_param(pRequest, "timestamp", timestamps/*"2017-04-15 19:53:00"*/);
	add_param(pRequest, "sign_method", "md5");
    
    quiksort_by_key(pRequest->params);
    total_length = pRequest->params->total_length+ 2* strlen(appsecret) + 1;
    buffer = (char*)malloc(sizeof(char)* total_length+1);
    memset(buffer, 0, total_length);
    strcat(buffer, appsecret);

    for(i = 0; i != pRequest->params->ite ; i ++ ){
        strcat(buffer,pRequest->params->keys[i]);
        strcat(buffer,pRequest->params->values[i]);
    }
    strcat(buffer, appsecret);
	//printf("\n%s\n",buffer); 
	OutputDebugString(buffer);

	//std::wstring wstringTmp = AnsiToUnicode(buffer);
	//std::string stringTmp = WStringToUTF8(wstringTmp.c_str());
	//char* bufferUTF8;
	//bufferUTF8 = (char*)malloc(sizeof(char)* stringTmp.length()+1);
	//memset(bufferUTF8, 0, stringTmp.length()+1);
	//strcat(bufferUTF8, stringTmp.c_str());

    md5(buffer,sign);
	//md5(bufferUTF8,sign);

	//printf("\n%s\n",sign);
	OutputDebugString(sign);
    add_param(pRequest, "sign",sign);
	

    free(buffer);
}

int convert_json_to_response(pcurlResponse pCurlResponse,pTopResponse pResponse){
    
    cJSON *root_json = cJSON_Parse(pCurlResponse->result);
    cJSON *result;
    char* out=0;
    if (NULL == root_json)
    {
        size_t len = pCurlResponse->len;
        cJSON_Delete(root_json);
        pResponse->bytes = (char*)malloc(sizeof(char)*len);
        memcpy(pResponse->bytes, pCurlResponse->result, len);
        pResponse->len = len ;
        return 0;
    }
    result =root_json->child->child;
    
    pResponse->code = 0;
    
    while(result){
        if(!strcmp(result->string,"code")){
            pResponse->code = result->valueint;
        }else if(!strcmp(result->string,"msg")){
            set_msg(pResponse,result->valuestring);
        }else if(!strcmp(result->string,"sub_code")){
            set_subcode(pResponse,result->valuestring);
        }else if(!strcmp(result->string,"sub_msg")){
            set_submsg(pResponse,result->valuestring);
        }else if(!strcmp(result->string,"request_id")){
            set_request_id(pResponse,result->valuestring);
        }else{
            out = NULL;
            out = print_value(result,0,0);
            if(out){
                add_result_param(pResponse,result->string,out);
                free(out);
            }
        }
        result = result ->next;
    }
    cJSON_Delete(root_json);
    return 0;
}

TopResponse* top_execute(pTaobaoClient pClient,pTopRequest pRequest,char* session){
    int retCode = -1;
    pWebHandler pHandler = alloc_webHandler();
    pcurlResponse curlResponse = alloc_curl_response();
    pTopResponse pResponse = alloc_top_response();
    pRequest->url = pClient->url;

    compeleteSystemParams(pRequest,pClient->appkey,pClient->appsecret,session);

    retCode = doPost(pHandler, pRequest->url, pRequest->params,pRequest->files,pRequest->httpHeaders, curlResponse);
    if(retCode == 0)
        convert_json_to_response(curlResponse,pResponse);
    else
        pResponse->code = retCode + 10000;
    
    destroy_curl_response(curlResponse);
    destroy_webHandler(pHandler);
    return pResponse;
}
