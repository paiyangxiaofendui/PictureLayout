#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "SingleMap.h"

top_map* alloc_map(){
    top_map* m = (top_map*)malloc(sizeof(top_map));
    m->ite = 0;
    m->total_length = 0;
    m->size = 32;
    m->keys = (char**)malloc(sizeof(char*) * m->size);
    m->values = (char**)malloc(sizeof(char*) * m->size);
    return m;
}

int insert_map(top_map* m,const char* key, const char* value){

    char * tk,*tv;
    size_t len;
    if(!key || !value || !m)
        return -1;
    
    len = strlen(key);
    tk = (char*) malloc(sizeof(char)*len+1);
    memmove(tk, key, len);
    tk[len] = '\0';
    m->total_length += len;
    
    len = strlen(value);
    tv = (char*) malloc(sizeof(char)*len+1);
    tv[len] = '\0';
    memmove(tv, value, len);
    m->total_length += len;
    
    if(m->ite == m->size){
        int i;
        char** tmp = NULL;
        m->size = m->size * 2;
        tmp = (char**)malloc(sizeof(char*)* m->size);
        assert(tmp != NULL);
        for(i = 0; i < m->ite ;i++){
            tmp[i] = m->keys[i];
        }
        free(m->keys);
        m->keys = tmp;
        
        tmp = (char**)malloc(sizeof(char*)* m->size);
        assert(tmp != NULL);
        for(i = 0; i < m->ite ;i++){
            tmp[i] = m->values[i];
        }
        free(m->keys);
        m->values = tmp;
        
        for(i = 0; i < m->ite; i++){
            printf("%s ",m->keys[i]);
        }
    }
    m->keys[m->ite] = tk;
    m->values[m->ite] = tv;
    m->ite = m->ite + 1;
    return m->ite;
}

void destrop_map(top_map* m){
    if(m){
        int i ;
        for(i = 0; i < m->ite; i++){
            free(m->keys[i]);
            free(m->values[i]);
        }
        free(m->keys);
        free(m->values);
        free(m);
    }
}

void quiksort_(top_map* m,int low,int high)
{
    int i = low;
    int j = high;
    char** keys = m->keys;
    char** values = m->values;
    char* tempKey = keys[i];
    char* tempValue = values[i];
    if( low < high)
    {
        while(i < j)
        {
            while((strcmp(keys[j], tempKey) >= 0) && (i < j))
            {
                j--;
            }
            keys[i] = keys[j];
            values[i] = values[j];
            while((strcmp(keys[i], tempKey) < 0) && (i < j))
            {
                i++;
            }
            keys[j]= keys[i];
            values[j] = values[i];
        }
        keys[i] = tempKey;
        values[i] =tempValue;
        quiksort_(m,low,i-1);
        quiksort_(m,j+1,high);
    }
    else
    {
        return;
    }
}

void quiksort_by_key(top_map* m){
    quiksort_(m,0,m->ite - 1);
}
