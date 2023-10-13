#ifndef __COMMON_DEFS_H__
#define __COMMON_DEFS_H__


#ifndef NULL
#define NULL ((void*) 0)
#endif  /* NULL */


typedef unsigned char static_heap_t;

typedef unsigned int  alloc_size_t;

typedef unsigned int  address_t;


typedef enum
{
    BEST_FIT,
    FIRST_FIT

} ALLOCATOR_MODE;


typedef enum
{
    NOSPACE        = -1,
    ALREADY_EXISTS = -2,
    NO_NODES_LEFT  = -3,
    FREE_ERROR     = -4,
    REALLOC_ERROR  = -5,
    HEAP_INACTIVE  = -6,
    SUCCESS        = 0

} ALLOCATOR_STATUS;


/**
 * Track table of allocations.
 */
typedef struct __alloc_table__
{
    struct
    {
        address_t       __private_Addr;
        alloc_size_t __private_Size;

    } __private_Item;

    struct __alloc_table__* __private_Next;

} alloc_table_t;






#endif  /* __COMMON_DEFS_H__ */
