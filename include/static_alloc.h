#ifndef __STATIC_MALLOC_H__
#define __STATIC_MALLOC_H__

#include "common_defs.h"

/**
 * @brief set_alloc_buffer init the static allocator system.
 *        Must be called once.
 * @param buffer is The Buffer aka static heap
 * @param bufflen is buffer len
 */
void set_alloc_buffer(void* buffer, const alloc_size_t bufflen);


/**
 * @brief set_alloc_table set the table for static_xalloc
 * @param table is table ref
 * @param tablelen is table len
 */
void set_alloc_table(alloc_table_t* table, const alloc_size_t tablelen);


/**
 * @brief set_allocator_logic set the allocator logic [BEST_FIT, FIRST_FIT]
 * @param mode is [BEST_FIT, FIRST_FIT]
 */
void set_allocator_logic(const ALLOCATOR_MODE mode);


/**
 * @brief get_last_status return last status
 * @return
 */
ALLOCATOR_STATUS get_last_status(void);


/**
 * @brief get_allocator_logic return allocator logic
 * @return
 */
ALLOCATOR_MODE  get_allocator_logic(void);


/**
 * @brief get_free_space return available space left
 * @return
 */
alloc_size_t    get_free_space(void);


/**
 * @brief get_alloc_count return allocated chunks count
 * @return
 */
unsigned int    get_alloc_count(void);


/**
 * @brief start_static_heap enable static ops
*/
void start_static_heap(void);

/**
 * @brief start_static_heap stop static ops
*/
void stop_static_heap(void);


/**
 * @brief static_malloc allocate static N bytes of memory
 * @param size represents N bytes to be allocated
 * @return NULL or pointer
 */
void* static_malloc(alloc_size_t size);


/**
 * @brief static_calloc allocate static N bytes to 0 of memory
 * @param size represents N bytes to be allocated
 * @return NULL or pointer
 */
void* static_calloc(alloc_size_t size);


/**
 * @brief static_realloc reallocate ptr with N bytes of memory
 * @param size represents N bytes to be reallocated
 * @return NULL if N bytes are less or equal then ptr N bytes
 */
void* static_realloc(void* ptr, alloc_size_t size);


/**
 * @brief static_free deallocate static memory.
 * @param ptr to be freed
 */
int static_free(void* ptr);


#endif /* __STATIC_MALLOC_H__ */
