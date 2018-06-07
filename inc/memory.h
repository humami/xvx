#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"
#include "bitmap.h"

enum pool_flags {
	PF_KERNEL = 1,
	PF_USER = 2
};

#define PG_P_1   1
#define PG_P_0   0
#define PG_RW_R  0
#define PG_RW_W  2
#define PG_US_S  0
#define PG_US_U  4

typedef struct virtual_addr {
	bitmap vaddr_bitmap;
	uint32_t vaddr_start;
}virtual_addr;

extern struct pool kernel_pool, user_pool;
void mem_init();
void *get_kernel_pages(uint32_t pg_cnt);

#endif