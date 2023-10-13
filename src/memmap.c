#include "../include/memmap.h"

alloc_size_t table_len = 0;
alloc_size_t pool_len  = 0;

/* static node pool */
alloc_table_t*  node_pool = NULL;

/* track space left */
alloc_size_t available_space = 0;

alloc_table_t* memmap = NULL;


static unsigned int alloc_chunks = 0;
static unsigned int admission_threshold = 75;


/**
 * @brief new_node take a node from node_pool
 * @return node pointer or null
 */
alloc_table_t* new_node(void)
{
    address_t t = 0;

    while (t < table_len)
    {
        if (node_pool[t].__private_Item.__private_Size == 0)
            return &node_pool[t];
        
        ++t;
    }

    return NULL;
}


/**
 * @brief mm_add add new node into the table
 * @param addr
 * @param size
 * @return
 */
ALLOCATOR_STATUS mm_add(address_t addr, alloc_size_t size)
{
    alloc_table_t* newnode = new_node();
    ALLOCATOR_STATUS err    = SUCCESS;

    if (newnode != NULL) 
    {
        newnode->__private_Item.__private_Addr = addr;
        newnode->__private_Item.__private_Size = size;

        /* insert in front */
        if (memmap == NULL || addr < memmap->__private_Item.__private_Addr)
        {
            newnode->__private_Next = memmap;
            memmap = newnode;
            available_space -= size;
            alloc_chunks++;
        } 
        else  /* find best point to keep natural sort */
        {
            /* move ptr until addr is quite bigger than current->next->Item.addr */
            alloc_table_t* current = memmap;

            while (current->__private_Next != NULL && current->__private_Next->__private_Item.__private_Addr < addr)  
                current = current->__private_Next;

            /* cannot insert two nodes with same addr */
            if (current->__private_Next != NULL && current->__private_Next->__private_Item.__private_Addr == addr)    
            {
                newnode->__private_Item.__private_Addr = 0;
                newnode->__private_Item.__private_Size = 0;

                err = ALREADY_EXISTS;
            }
            else /* let's insert */
            {
                newnode->__private_Next = current->__private_Next;
                current->__private_Next = newnode;
                available_space -= size;
                alloc_chunks++;
            }
        }
    }
    else
        err = NO_NODES_LEFT;

    return err;
}



/**
 * @brief mm_rem delete a node from the table
 * @param table_head
 * @param addr
 */
alloc_size_t mm_rem(address_t addr)
{
    alloc_table_t* current = memmap;
    alloc_table_t* prev    = NULL;

    alloc_size_t size   = 0;

    while (current != NULL) 
    {
        if (current->__private_Item.__private_Addr == addr)
        {
            if (prev == NULL) 
                memmap = current->__private_Next;
            else
                prev->__private_Next = current->__private_Next;

            available_space += current->__private_Item.__private_Size;
            alloc_chunks--;

            size = current->__private_Item.__private_Size;

            current->__private_Item.__private_Addr = 0;
            current->__private_Item.__private_Size = 0;
            current->__private_Next = NULL;
            break;
        }

        prev = current;
        current = current->__private_Next;
    }

    return size;
}


/**
 * @brief is_best_fit retrives the best fit chunck, if the chunk space is bigger than 60% of the wanted space it is valid.
 * @param curr_fit
 * @param size
 * @return boolean
 */
unsigned int is_best_fit(const alloc_size_t curr_fit, const alloc_size_t size)
{
    return ((size * 100) / curr_fit) >= admission_threshold;
}





void init_memmap(alloc_table_t* alloc_table, const alloc_size_t tablelen, const alloc_size_t bufflen)
{
    if (node_pool == NULL)
    {
        memmap = NULL;

        node_pool = alloc_table;

        table_len = tablelen;

        available_space = bufflen;
        pool_len        = bufflen;
    }
}


void get_first_fit(alloc_size_t size, int* slot)
{
    alloc_table_t* curr = memmap;

    *slot = 0;

    while (curr)
    {
        if (curr->__private_Item.__private_Addr > ((address_t) *slot) && (curr->__private_Item.__private_Addr - *slot) >= size) break;  /* if the current slot is big enough stop searching */
        
        *slot = curr->__private_Item.__private_Addr + curr->__private_Item.__private_Size;                                           /* set next slot */
        curr  = curr->__private_Next;                                                                  /* skip to next table item */
    }
    
}


void get_best_fit(alloc_size_t size, int* slot)
{
    alloc_table_t* curr = memmap;
    
    alloc_size_t best_fit  = pool_len - 1;                              /* init to the max    */
    alloc_size_t curr_diff = 0;                                         /* local best_fit     */

    address_t best_slot = 0;                                            /* best slot found    */
    address_t curr_slot = 0;                                            /* current slot found */

    unsigned char bs_found = 0;                                         /* tracks slot lookup */

    while (curr)
    {
        curr_diff = (curr->__private_Item.__private_Addr - curr_slot);             /* get the chunk               */

        if (curr->__private_Item.__private_Addr > curr_slot && curr_diff >= size)  /* if the chunk is big enough  */
        {
            if (best_fit > curr_diff && is_best_fit(curr_diff, size))   /* check if it is the best fit */
            {
                best_fit  = curr_diff;
                best_slot = curr_slot;

                bs_found = 1;
            }
        }

        curr_slot = curr->__private_Item.__private_Addr + curr->__private_Item.__private_Size; /* set next offsets        */
        curr = curr->__private_Next;                                     /* skip to next table item */
    }

    *slot = bs_found ? best_slot : curr_slot;                           /* if best slot was found, set it else get new slot */
}


int get_memory_fit(alloc_size_t size, const ALLOCATOR_MODE mode)
{
    alloc_table_t* head = memmap;

    int slot = NOSPACE;

    ALLOCATOR_STATUS err;

    if (size <= available_space)
    {
        if (!head)
            slot = 0;
        else switch (mode)
        {
            case FIRST_FIT:
                get_first_fit(size, &slot);
                break;

            case BEST_FIT:
                get_best_fit(size, &slot);
                break;
        }

        if ((err = mm_add(slot, size)) != SUCCESS)
            slot = err;
    }

    return slot;
}


alloc_size_t deallocate_slot(address_t addr)
{
    return mm_rem(addr);
}


alloc_size_t get_slot_size(address_t addr)
{
    alloc_table_t* current = memmap;
    alloc_size_t size   = 0;

    while (current)
    {
        if (current->__private_Item.__private_Addr == addr)
        {
            size = current->__private_Item.__private_Size;
            break;
        }

        current = current->__private_Next;
    }

    return size;
}


alloc_size_t available_free_space(void)
{
    return available_space;
}


unsigned int allocated_chunks(void)
{
    return alloc_chunks;
}


void show_allocations(
                      const OPERATION_CODE code, 
                      void* ptr, 
                      address_t addr,
                      alloc_size_t size, 
                      const ALLOCATOR_MODE mode,
                      int status,
                      unsigned int static_heap_status
                     )
{
#if DEBUG_ALLOCS == 1
    alloc_table_t* current = memmap;

    char* operation;
    char* logic;
    char* result;
    char* message;

    char* mpool_status = static_heap_status ? "Active" : "Inactive";

    switch (mode)
    {
    case 0:
        logic = "Best Fit";
        break;

    case 1:
        logic = "First Fit";
        break;

    default:
        break;
    }
    
    switch (code)
    {
    case MALLOC:
        operation = "Static Malloc";
        message   = "Mallocated";
        break;

    case CALLOC:
        operation = "Static Calloc";
        message   = "Callocated";
        break;

    case REALLOC:
        operation = "Static Realloc";
        message   = "Reallocated";
        break;

    case FREE:
        operation = "Static Free";
        message   = "Freed";
        break;
    }

    switch (status)
    {
    case NOSPACE:
        result  = "FAIL";
        message = "Not Enough Space";
        break;

    case NO_NODES_LEFT:
        result  = "FAIL";
        message = "Max Allocs Reached";
        break;

    case ALREADY_EXISTS:
        result  = "FAIL";
        message = "Addr Already Exists";
        break;

    case FREE_ERROR:
        result  = "FAIL";
        message = "Deallocation Error";
        break;

    case REALLOC_ERROR:
        result  = "FAIL";
        message = "Realloc Size Error";
        break;

    case HEAP_INACTIVE:
        result  = "FAIL";
        message = "Heap Not Active";
        break;

    default:
        result  = "SUCCESS";
        break;
    }

    if (!static_heap_status)
        message = "Heap Not Active";


    printf("\n|```````````` Allocation Table `````````````|\n");
    printf("|___________________________________________|\n");

    if (!current)
        printf("|              No Allocations               |\n");
    else 
    {
        printf("|      Pointer     |  Address  |   Extent   |\n");
        printf("|___________________________________________|\n");
    }

    while (current)
    {
        printf("|  %p  |  %7u  |  %8u  |\n", (void*) current, current->__private_Item.__private_Addr, current->__private_Item.__private_Size);
        current = current->__private_Next;
    }
    printf("|___________________________________________|\n");

    printf("\n|``````````````` Heap Report ```````````````|\n");
    printf("|___________________________________________|\n");
    printf("| Heap Status:         %-20s |\n", mpool_status);
    printf("| Available Space (B): %-20u |\n", available_space);
    printf("| Total Space (B):     %-20u |\n", pool_len);
    printf("| Heap Usage (%%):      %-20u |\n", ((pool_len - available_space) * 100) / pool_len);
    printf("|___________________________________________|\n");

    printf("\n|```````````` Allocations Report ```````````|\n");
    printf("|___________________________________________|\n");
    printf("| Allocated (blocks):  %-20u |\n", alloc_chunks);
    printf("| Available (blocks):  %-20u |\n", table_len - alloc_chunks);
    printf("| Maximum   (blocks):  %-20u |\n", table_len);
    printf("|___________________________________________|\n");

    printf("\n|```````````` Operation Report `````````````|\n");
    printf("|___________________________________________|\n");
    printf("| Operation:           %-20s |\n", operation);
    
    if (code != FREE)
        printf("| Allocator Logic:     %-20s |\n", logic);
    
    
    printf("| Pointer:             %-20p |\n", ptr);

    if (code == REALLOC) 
    {
        printf("| Previous Size:       %-20d |\n", (int) addr);
        printf("| New Size:            %-20u |\n", size);
    }
    else if (status != FREE_ERROR)
    {
        printf("| Address:             %-20d |\n", (int) addr);
        printf("| Extend:              %-20u |\n", size);
    }

    printf("| Operation Status:    %-20s |\n", result);
    printf("| Message:             %-20s |\n", message);
    printf("|___________________________________________|\n\n");
#endif
}
