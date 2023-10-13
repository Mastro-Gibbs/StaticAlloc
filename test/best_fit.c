#include "../include/static_alloc.h"

#include <stdio.h>

/*
 * DATA FOR STATIC ALLOC
 */
#define BUFFLEN 1024
static_heap_t buffer[BUFFLEN];

#define MAX_ALLOC 10
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

    char* string1 = static_calloc(10);
    char* string2 = static_calloc(20);
    char* string3 = static_calloc(30);
    char* string4 = static_calloc(10);
    char* string5 = static_calloc(50);

    static_free(string2);
    static_free(string4);

    string2 = static_calloc(9);
    string4 = static_calloc(1);

    char* string6 = static_calloc(20);

    static_free(string1);
    static_free(string2);
    static_free(string3);
    static_free(string4);
    static_free(string5);
    static_free(string6);

    int array[10];

    if (static_free(array) == FREE_ERROR)
        printf("%s\n", "free error");
}

int main(void)
{    
    begin_test();

    test();

    return 0;
}


