#ifndef __TopSDKC__WebUtils__
#define __TopSDKC__WebUtils__

#include <stdio.h>
#include <curl/curl.h>
#include "SingleMap.h"
#include "curlResponse.h"

typedef struct WebHandler {
    CURL *curl;
}WebHandler,*pWebHandler;

pWebHandler alloc_webHandler();
void destroy_webHandler(pWebHandler webHandler);
int doPost(pWebHandler webHandler,char* url,top_map* params,top_map* files,top_map* httpheader,pcurlResponse response);

#endif /* defined(__TopSDKC__WebUtils__) */
