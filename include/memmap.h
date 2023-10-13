#ifndef __MEM_MAP_H__
#define __MEM_MAP_H__

#include "common_defs.h"
#include "private_defs.h"


/**
 * @brief init_memmap init the table.
 * @param table_head is first ref to first table item ptr
 */
void init_memmap(alloc_table_t* alloc_table, const alloc_size_t tablelen, const alloc_size_t bufflen);


/**
 * @brief get_memory_fit get a memory chunk
 * @param table_head is first ref to first table item ptr
 * @param size represent N bytes to be allocated
 * @param mode is allocator logic
 * @return offset on success or -1 on error
 */
int get_memory_fit(alloc_size_t size, const ALLOCATOR_MODE mode);


/**
 * @brief deallocate_slot remove a table entry
 * @param table_head is first ref to first table item ptr
 * @param addr is ID of table entry to be removed.
 * @return size of memory freed
 */
alloc_size_t deallocate_slot(address_t addr);


/**
 * @brief get_slot_size get a table slot size
 * @param table_head is first ref to first table item ptr
 * @param addr is ID of table entry to be removed.
 * @return size of slot
 */
alloc_size_t get_slot_size(address_t addr);


alloc_size_t available_free_space(void);


unsigned int allocated_chunks(void);


void show_allocations(
                      const OPERATION_CODE code, 
                      void* ptr, 
                      address_t addr,
                      alloc_size_t size, 
                      const ALLOCATOR_MODE mode, 
                      int status,
                      unsigned int static_heap_status
                     );




#endif /* __MEM_MAP_H__ */
