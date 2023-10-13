#include "../include/static_alloc.h"

#include <stdio.h>

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
    set_allocator_logic(FIRST_FIT);

    unsigned int size = sizeof(size);
    
    while (static_malloc(size+=size));
}

int main(void)
{    
    begin_test();

    test();

    return 0;
}


