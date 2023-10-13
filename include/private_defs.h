#ifndef __PRIVATE_DEFS_H__
#define __PRIVATE_DEFS_H__



typedef enum
{
    MALLOC  = 0,
    CALLOC  = 1,
    REALLOC = 2,
    FREE    = 3

} OPERATION_CODE;



#ifndef DEBUG_ALLOCS
#define DEBUG_ALLOCS 0
#endif

#if     DEBUG_ALLOCS == 1
#include <stdio.h>
#endif




#endif  /* __PRIVATE_DEFS_H__ */
