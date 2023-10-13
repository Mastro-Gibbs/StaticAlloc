#include "../include/static_alloc.h"
#include "../include/memmap.h"

void*       s_heap    = NULL;
void*       s_tmppool = NULL;
address_t   s_heaplen = 0;

ALLOCATOR_MODE   alloc_mode   = FIRST_FIT;
ALLOCATOR_STATUS alloc_status = SUCCESS;

#if DEBUG_ALLOCS == 1
#define TO_DEBUG(code, ptr, addr, size, mode, status, mp_status) show_allocations(code, ptr, addr, size, mode, status, mp_status); getchar()
#else
#define TO_DEBUG(code, ptr, addr, size, mode, status, mp_status)
#endif


#ifndef NORMALIZE
#define NORMALIZE(pool) ((unsigned char*) pool)
#endif


/* SETTERS  */

void set_alloc_buffer(void* buffer, const alloc_size_t bufflen)
{
    if (!s_heap)
    {
        address_t i;
        for (i = 0; i < bufflen; ++i)
            NORMALIZE(buffer)[i] = 0;

        s_tmppool = buffer;
        s_heaplen = bufflen;
    }
}

void set_alloc_table(alloc_table_t* table, const alloc_size_t tablelen) { init_memmap(table, tablelen, s_heaplen); }

void set_allocator_logic(const ALLOCATOR_MODE mode) { alloc_mode = mode; }


/* GETTERS */

alloc_size_t get_free_space(void)        { return s_heap != NULL ? available_free_space() : 0; }

ALLOCATOR_STATUS get_last_status(void)   { return alloc_status; }

ALLOCATOR_MODE get_allocator_logic(void) { return alloc_mode; }

unsigned int get_alloc_count(void)       { return allocated_chunks(); }


/* MENAGMENT */

void start_static_heap(void) { s_heap = s_tmppool; }

void stop_static_heap(void)  { s_heap = NULL; }



/* HEAP OPERATIONS */

void* static_malloc(alloc_size_t size)
{
    void* ptr  = NULL;
    int   slot = HEAP_INACTIVE;

    alloc_status = SUCCESS;

    if (size != 0 && s_heap != NULL)
    {
        slot = get_memory_fit(size, alloc_mode);

        if (slot >= 0)
            ptr = NORMALIZE(s_heap) + slot;
        else
            alloc_status = slot;
    }

    TO_DEBUG(
             MALLOC, 
             ptr, 
             slot, 
             size, 
             alloc_mode, 
             slot,
             s_heap != NULL
            );

    return ptr;
}


void* static_calloc(alloc_size_t size)
{
    void* ptr  = NULL;
    int   slot = HEAP_INACTIVE;

    unsigned int iterator = 0;

    alloc_status = SUCCESS;

    if (size != 0 && s_heap != NULL)
    {
        slot = get_memory_fit(size, alloc_mode);

        if (slot >= 0)
        {
            ptr = NORMALIZE(s_heap) + slot;

            for ( ; iterator < size; ++iterator)
                ((unsigned char*) ptr)[iterator] = 0;
        }
        else
            alloc_status = slot;
    }

    TO_DEBUG(
             CALLOC, 
             ptr, 
             slot, 
             size, 
             alloc_mode, 
             slot,
             s_heap != NULL
            );

    return ptr;
}


void* static_realloc(void* ptr, alloc_size_t size)
{
    address_t         slot = 0;
    alloc_size_t prev_size = 0;

    int new_slot = HEAP_INACTIVE;

    void* res = ptr;

    unsigned int iterator = 0;

    alloc_status = SUCCESS;

    if (s_heap != NULL)
    {
        slot = (address_t) (NORMALIZE(ptr) - NORMALIZE(s_heap));
        prev_size = get_slot_size(slot);

        if (size != 0 && prev_size < size)
        {
            new_slot = get_memory_fit(size, alloc_mode);

            if (new_slot >= 0)
            {
                res = NORMALIZE(s_heap) + ((address_t) new_slot);

                for (iterator = 0; iterator < size; ++iterator)
                    NORMALIZE(res)[iterator] = NORMALIZE(ptr)[iterator];

                TO_DEBUG(
                         REALLOC, 
                         res, 
                         new_slot, 
                         size, 
                         alloc_mode,
                         new_slot,
                         s_heap != NULL
                        );

                static_free(ptr);
            }
            else
            {
                alloc_status = new_slot;

                TO_DEBUG(
                         REALLOC, 
                         res, 
                         new_slot, 
                         size, 
                         alloc_mode,
                         new_slot,
                         s_heap != NULL
                        );
            }
        
        }
        else
        {
            alloc_status = REALLOC_ERROR;

            TO_DEBUG(
                     REALLOC, 
                     res, 
                     prev_size, 
                     size, 
                     alloc_mode,
                     REALLOC_ERROR,
                     s_heap != NULL
                    );
        }
    }

    return res;
}


int static_free(void* ptr)
{
    address_t    slot = HEAP_INACTIVE;
    alloc_size_t size = 0;

    if (s_heap != NULL && ptr != NULL)
    {
        slot = (address_t) (NORMALIZE(ptr) - NORMALIZE(s_heap));
    
        if (NORMALIZE(ptr) >= NORMALIZE(s_heap) && NORMALIZE(ptr) < (NORMALIZE(s_heap) + s_heaplen))
            size = deallocate_slot(slot);
    }

    TO_DEBUG(
             FREE, 
             ptr, 
             slot, 
             size, 
             2,
             size != 0 ? SUCCESS : FREE_ERROR,
             s_heap != NULL
            );

    alloc_status = size != 0 ? SUCCESS : FREE_ERROR;

    return size != 0 ? SUCCESS : FREE_ERROR;
}