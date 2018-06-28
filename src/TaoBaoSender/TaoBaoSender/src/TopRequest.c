#include <stdlib.h>
#include <string.h>
#include "SingleMap.h"
#include "../topsdk.h"

pTopRequest alloc_top_request(){
    pTopRequest request = (pTopRequest)malloc(sizeof(TopRequest));
    request->httpHeaders = NULL;
    request->params = NULL;
    request->files = NULL;
    request->apiName = NULL;
    request->url = NULL;
    return request;
}

void destroy_top_request(pTopRequest pt){
    if(pt){
        if(pt->params)
            destrop_map(pt->params);
        if(pt->files)
            destrop_map(pt->files);
        if(pt->httpHeaders)
            destrop_map(pt->httpHeaders);
        if(pt->apiName)
            free(pt->apiName);
        free(pt);
    }
}


int add_httpheader_add_param(pTopRequest pt,const char* key, const char* value){
    if(!pt)
        return 1;
    if(!pt->httpHeaders)
        pt->httpHeaders = alloc_map();
    insert_map(pt->httpHeaders, key, value);
    return 0;
}

int add_param(pTopRequest pt,const char* key, const char* value){
    if(!pt)
        return 1;
    if(!pt->params)
        pt->params = alloc_map();
    insert_map(pt->params, key, value);
    return 0;
}

int add_file_param(pTopRequest pt,const char* key, const char* value){
    if(!pt)
        return 1;
    if(!pt->files)
        pt->files = alloc_map();
    insert_map(pt->files, key, value);
    return 0;
}

int set_api_name(pTopRequest pt,const char* name){
    char* apiname;
    long len ;
    if(!pt)
        return 1;
    if(!name)
        return 2;
    len = strlen(name);
    apiname = (char*)malloc(sizeof(char)*(len + 1));
    memcpy(apiname, name, strlen(name));
    apiname[len] = '\0';
    pt->apiName = apiname;
    return 0;
    
}
