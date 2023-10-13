#include "../include/static_alloc.h"

#include <stdio.h>
#include <string.h>

/*
 * DATA FOR STATIC ALLOC
 */
#define BUFFLEN 1024
static_heap_t buffer[BUFFLEN];

#define MAX_ALLOC 10
alloc_table_t table [MAX_ALLOC];


typedef struct __CANDOIT__
{
    unsigned int unum;
    char         c;
    char*        str;
} cid_t;


void begin_test(void)
{
    set_alloc_buffer(buffer, BUFFLEN);
    set_alloc_table (table,  MAX_ALLOC);

    set_allocator_logic(BEST_FIT);

    start_static_heap();
}


void test(void)
{
    cid_t* cid = NULL;

    cid = static_malloc(sizeof(*cid));

    if (NULL != cid)
    {
        cid->c    = '?';
        cid->unum = 42;
        cid->str  = static_malloc(sizeof(char) * 45);

        if (NULL != cid->str)
        {
            strncpy(cid->str, "ciao ciao", 10);
            cid->str[9] = '\0';
        }
    }

    /* redundant checks */
    if (NULL != cid)
    {
        printf("cid obj\nnum:  %d\nchar: %c\n", cid->unum, cid->c);

        if (NULL != cid->str)
        {
            printf("str:  %s\n", cid->str);

            static_free(cid->str);
        }

        static_free(cid);
    }
}


int main(void)
{   
    begin_test();

    test();

    return 0;
}