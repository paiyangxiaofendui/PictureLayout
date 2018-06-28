#ifndef __TopSDKC__StrMap__
#define __TopSDKC__StrMap__

#include <stdio.h>
#include "../topsdk.h"

top_map* alloc_map();
int insert_map(top_map* m,const char* key, const char* value);
void destrop_map(top_map* m);
void quiksort_by_key(top_map* m);

#endif /* defined(__TopSDKC__StrMap__) */
