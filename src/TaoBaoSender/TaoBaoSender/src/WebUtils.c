//#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "WebUtils.h"

extern int retry_times;

extern int retry_sleep_times;

extern int http_time_out;

extern char* capath;

pWebHandler alloc_webHandler(){
    pWebHandler pwh = (pWebHandler)malloc(sizeof(WebHandler));
    pwh->curl = curl_easy_init();
    return pwh;
}

void destroy_webHandler(pWebHandler webHandler){
    if(webHandler){
        curl_easy_cleanup(webHandler->curl);
        free(webHandler);
    }
}

static size_t OnWriteData(void* buffer, size_t size, size_t nmemb, void* lpVoid)
{
    pcurlResponse response = (pcurlResponse)lpVoid;
    if(response->size < response->len + size*nmemb){
        response->size = (response->size + size*nmemb) * 2;
        response->result = (char*)realloc(response->result, response->size);
    }
    memcpy(response->result + response->len, buffer, size*nmemb);
    response->len += size*nmemb;
    return size*nmemb;
}

int startWith(char* src,char*mode){
    int i;
    int len = strlen(mode);
    for(i = 0; i < len; i++){
        if(src[i] != mode[i])
            break;
    }
    return i == len ? 1: 0;
}

int doPost(pWebHandler webHander,char* url,top_map* params,top_map* files,top_map* httpheader,pcurlResponse response){
    CURLcode res;
    int i =0 ;
    int cur_retry = 0;
    struct curl_slist *chunk = NULL;
    struct curl_httppost *post=NULL;
    struct curl_httppost *last=NULL;

    if(webHander->curl) {
        curl_easy_setopt(webHander->curl, CURLOPT_URL, url);

        if(startWith(url,"https")){
            if(capath != NULL){
                curl_easy_setopt(webHander->curl, CURLOPT_CAINFO,capath);
            }else{
                curl_easy_setopt(webHander->curl, CURLOPT_SSL_VERIFYPEER, 0);
                curl_easy_setopt(webHander->curl, CURLOPT_SSL_VERIFYHOST, 0);
            }
        }

        curl_easy_setopt(webHander->curl, CURLOPT_TIMEOUT,http_time_out);
        curl_easy_setopt(webHander->curl, CURLOPT_WRITEFUNCTION, OnWriteData);
        curl_easy_setopt(webHander->curl, CURLOPT_WRITEDATA, (void *)response);
        curl_easy_setopt(webHander->curl, CURLOPT_USERAGENT,"top-sdk-c");

        if(params != NULL){
            for( i = 0 ; i != params->ite; i++){
                if( curl_formadd(&post, &last, CURLFORM_COPYNAME, params->keys[i],
                                 CURLFORM_COPYCONTENTS, params->values[i], CURLFORM_END) != 0){
                    return CURLE_FAILED_INIT;
                }
            }
        }

        if(files != NULL){
            for( i = 0 ; i != files->ite; i++){
                if( curl_formadd(&post, &last, CURLFORM_COPYNAME, files->keys[i],
                                 CURLFORM_FILE, files->values[i], CURLFORM_END) != 0){
                    return CURLE_FAILED_INIT;
                }
            }
        }
        
        curl_easy_setopt(webHander->curl, CURLOPT_HTTPPOST, post);
        
        if(httpheader && httpheader->ite > 0){
            char temp[128];
            for(i =0; i< httpheader->ite; i++){
                memset(&temp, 0, 128);
                strcat(temp, httpheader->keys[i]);
                strcat(temp, ": ");
                strcat(temp, httpheader->values[i]);
                chunk = curl_slist_append(chunk,temp);
            
            }
            
            if(chunk != NULL)
                curl_easy_setopt(webHander->curl, CURLOPT_HTTPHEADER, chunk);
        }

        do{
            res = curl_easy_perform(webHander->curl);
            if(res != CURLE_OK){
                cur_retry ++;
            }
        }while(res != CURLE_OK && cur_retry <= retry_times);

        curl_formfree(post);
    }else{
        return -1;
    }
    if(chunk)
        curl_slist_free_all(chunk);
    
    return res;
}
