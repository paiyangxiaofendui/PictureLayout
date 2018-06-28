#include <stdlib.h>
#include <string.h>
#include "SingleMap.h"
#include "../topsdk.h"

pTopResponse alloc_top_response(){
    pTopResponse response = (pTopResponse) malloc(sizeof(TopResponse));
    response->code = 0;
    response->msg = NULL;
    response->subMsg = NULL;
    response->subCode = NULL;
    response->results = NULL;
    response->requestId = NULL;
    response->bytes = NULL;
    response->len = 0;
    return response;
}

void destroy_top_response(pTopResponse pt){
    if(pt){
        if(pt->requestId)
            free(pt->requestId);
        if(pt->msg)
            free(pt->msg);
        if(pt->subMsg)
            free(pt->subMsg);
        if(pt->subCode)
            free(pt->subCode);
        if(pt->results)
            destrop_map(pt->results);
        if(pt->bytes)
            free(pt->bytes);
        
        free(pt);
    }
}

void add_result_param(pTopResponse pt,char* key,char* value){
    if(!pt)
        return;
    if(!pt->results)
        pt->results = alloc_map();
    insert_map(pt->results, key, value);
}

char * prepare_memory(char* value){
    char* result = NULL;
    long len ;
    if(!value)
        return NULL;
    len = strlen(value);
    result = (char*) malloc(sizeof(char)*(len+1));
    memcpy(result, value, strlen(value));
    result[len] = '\0';
    return result;
}

void set_msg(pTopResponse pt,char* value){
    char* result = NULL;
    if(!pt)
        return;
    result = prepare_memory(value);
    pt->msg = result;
}

void set_submsg(pTopResponse pt,char* value){
    char* result = NULL;
    if(!pt)
        return;
    result = prepare_memory(value);
    pt->subMsg = result;
}

void set_subcode(pTopResponse pt,char* value){
    char* result = NULL;
    if(!pt)
        return;
    result = prepare_memory(value);
    pt->subCode = result;
}

void set_request_id(pTopResponse pt,char* value){
    char* result = NULL;
    if(!pt)
        return;
    result = prepare_memory(value);
    pt->requestId = result;
}


pTopResponseIterator init_response_iterator(pTopResponse pResult){
    pTopResponseIterator pIte = (pTopResponseIterator) malloc(sizeof(TopResponseIterator));
    pIte->cur_index = 0;
    pIte->pResult = pResult;
    return pIte;
}

void destroy_response_iterator(pTopResponseIterator ite){
    /*do not free TopResponse */
    free(ite);
}

int hasMore(pTopResponseIterator ite){
    if(!ite->pResult)
        return 1;
    if(!ite->pResult->results)
        return 2;
    if(ite->cur_index < ite->pResult->results->ite){
        return 0;
    }
    return -1;
}

int parseNext(pTopResponseIterator ite,pResultItem resultItem){
    if(hasMore(ite) == 0){
        resultItem->key = ite->pResult->results->keys[ite->cur_index];
        resultItem->value = ite->pResult->results->values[ite->cur_index];
        ite->cur_index ++ ;
        return 0;
    }
    return 1;
}

pResultItem alloc_result_item(){
    return (pResultItem)malloc(sizeof(ResultItem));
}
void destroy_result_item(pResultItem pResult){
    /*do not free key and value ,it will free with TopResponse*/
    free(pResult);
}


