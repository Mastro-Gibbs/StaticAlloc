#include "../include/static_alloc.h"

#include <stdio.h>
#include <string.h>

/*
 * DATA FOR STATIC ALLOC
 */
#define BUFFLEN 1024
static_heap_t buffer[BUFFLEN];

#define MAX_ALLOC 4
alloc_table_t table [MAX_ALLOC];


void begin_test(void)
{
    set_alloc_buffer(buffer, BUFFLEN);
    set_alloc_table (table,  MAX_ALLOC);

    start_static_heap();
}


void test(void)
{
    set_allocator_logic(BEST_FIT);

    char* str = static_malloc(sizeof(char) * 10);

    str = static_realloc(str, 15);

    static_free(str);
}


void test_fail(void)
{
    set_allocator_logic(BEST_FIT);

    char* str = static_malloc(sizeof(char) * 10);

    str = static_realloc(str, 5);

    if (get_last_status() == REALLOC_ERROR)
        printf("realloc error\n");

    static_free(str);
}

int main(void)
{    
    begin_test();

    test();

    test_fail();

    return 0;
}


