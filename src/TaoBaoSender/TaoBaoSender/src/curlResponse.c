#include <stdlib.h>
#include <string.h>
#include "curlResponse.h"

pcurlResponse alloc_curl_response(){
    pcurlResponse response = (pcurlResponse) malloc(sizeof(curlResponse));
    response->size = 4096;
    response->result = (char*)malloc(sizeof(char)*4096);
    memset(response->result, 0, 4096);
    response->len = 0;
    return response;
}

void destroy_curl_response(pcurlResponse response){
    if(response){
        free(response->result);
        free(response);
    }
}
