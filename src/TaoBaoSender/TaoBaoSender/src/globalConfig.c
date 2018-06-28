#include <string.h>
#include <stdlib.h>

int retry_times = 0;

int retry_sleep_times = 50;

int http_time_out = 15;

char* capath = NULL;


void set_retry_times(int retry){
    retry_times = retry;
}

void set_retry_sleep_times(int sleep_time){
    if(sleep_time >= 0){
        retry_sleep_times = sleep_time;
    }
}

void set_http_time_out(int timeout){
    if(timeout > 0){
        http_time_out = timeout;
    }
}

void set_capath(char* path){
    int len = -1;
    if(path == NULL)
        return;
    len = strlen(path);
    if(len > 0){
        capath = (char*)malloc(sizeof(char)*(len+1));
        memmove(capath, path, len);
        capath[len] = '\0';
    }
}
