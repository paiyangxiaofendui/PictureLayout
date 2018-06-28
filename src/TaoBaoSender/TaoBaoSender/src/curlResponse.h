#ifndef __TopSDKC__curlResponse__
#define __TopSDKC__curlResponse__

#include <stdio.h>

typedef struct curlResponse{
    char* result;
    size_t size;
    size_t len;
}curlResponse,*pcurlResponse;

pcurlResponse alloc_curl_response();

void destroy_curl_response(pcurlResponse response);

#endif /* defined(__TopSDKC__curlResponse__) */
