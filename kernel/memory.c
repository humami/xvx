#include "memory.h"
#include "console.h"
#include "string.h"
#include "debug.h"

#define PG_SIZE 4096

#define MEM_BITMAP_BASE 0xc009a000

#define K_HEAP_START 0xc0200000

typedef struct pool {
    bitmap phy_bitmap;
    uint32_t phy_addr_start;
    uint32_t pool_size;
}pool;

pool kernel_pool, user_pool;
virtual_addr kernel_vaddr;

#define PDE_IDX(addr) ((addr & 0xffc00000) >> 22)
#define PTE_IDX(addr) ((addr & 0x003ff000) >> 12)

static uint32_t *pde_ptr(uint32_t vaddr) {
    uint32_t *pde = (uint32_t *)(0xfffff000 + PDE_IDX(vaddr) * 4);
    return pde;
}

static uint32_t *pte_ptr(uint32_t vaddr) {
    uint32_t *pte = (uint32_t *)(0xffc00000 + ((vaddr & 0xffc00000) >> 10) + PTE_IDX(vaddr) * 4);
    return pte;
}

static void *vaddr_get(enum pool_flags pf, uint32_t pg_cnt) {
    int vaddr_start = 0, bit_idx_start = -1;
    uint32_t cnt = 0;

    if(pf == PF_KERNEL) {
        bit_idx_start = bitmap_scan(&kernel_vaddr.vaddr_bitmap, pg_cnt);

        if(bit_idx_start == -1)
            return NULL;

        while(cnt < pg_cnt) {
            bitmap_set(&kernel_vaddr.vaddr_bitmap, bit_idx_start + cnt, 1);
            cnt++;
        }

        vaddr_start = kernel_vaddr.vaddr_start + bit_idx_start * PG_SIZE;
    } else {

    }

    return (void *)vaddr_start;
}

static void *paddr_get(pool *m_pool) {
    int bit_idx = -1;

    bit_idx = bitmap_scan(&m_pool->phy_bitmap, 1);

    if(bit_idx == -1)
        return NULL;

    bitmap_set(&m_pool->phy_bitmap, bit_idx, 1);

    uint32_t paddr = bit_idx * PG_SIZE + m_pool->phy_addr_start;

    return (void *)paddr;
}

static void page_table_add(void *_vaddr, void *_paddr) {
    uint32_t vaddr = (uint32_t)_vaddr, paddr = (uint32_t)_paddr;
    uint32_t *pde = pde_ptr(vaddr);
    uint32_t *pte = pte_ptr(vaddr);

    if(*pde & 0x00000001) {
        ASSERT(!(*pte & 0x00000001));
        *pte = (paddr | PG_US_U | PG_RW_W | PG_P_1);
    }
    else {
        uint32_t pde_phyaddr = (uint32_t)paddr_get(&kernel_pool);

        *pde = (pde_phyaddr | PG_US_U | PG_RW_W | PG_P_1);
        memset((void *)((int)pte & 0xfffff000), 0, PG_SIZE);
        
        ASSERT(!(*pte & 0x00000001));
        *pte = (paddr | PG_US_U | PG_RW_W | PG_P_1);
    }
}

static void *malloc_page(enum pool_flags pf, uint32_t pg_cnt) {
    void *vaddr_start = vaddr_get(pf, pg_cnt);

    if(!vaddr_start)
        return NULL;

    uint32_t vaddr = (uint32_t)vaddr_start, cnt = pg_cnt;
    pool *mem_pool;

    if(pf == PF_KERNEL)
        mem_pool = &kernel_pool;
    else 
        mem_pool = &user_pool;

    while(cnt > 0) {
        void *phy_addr = paddr_get(mem_pool);

        if(!phy_addr)
            return NULL;
        
        page_table_add((void *)vaddr, phy_addr);

        vaddr += PG_SIZE;
        cnt--;
    }

    return vaddr_start;
}

void *get_kernel_pages(uint32_t pg_cnt) {
    void *vaddr = malloc_page(PF_KERNEL, pg_cnt);
    if(vaddr != NULL)
        memset(vaddr, 0, pg_cnt * PG_SIZE);
    return vaddr;
}

static void mem_pool_init(uint32_t total_mem_size) {
    uint32_t page_table_size = PG_SIZE * 2;

    uint32_t used_mem = page_table_size + 0x200000;
    uint32_t free_mem = total_mem_size - used_mem;
    uint32_t free_pages = free_mem / PG_SIZE;

    uint16_t kernel_free_pages = free_pages / 2;
    uint16_t user_free_pages = free_pages - kernel_free_pages;

    uint32_t kbm_length = kernel_free_pages / 8;
    uint32_t ubm_length = user_free_pages / 8;

    uint32_t kp_start = used_mem;
    uint32_t up_start = kp_start + kernel_free_pages * PG_SIZE;

    kernel_pool.phy_addr_start = kp_start;
    user_pool.phy_addr_start = up_start;

    kernel_pool.pool_size = kernel_free_pages * PG_SIZE;
    user_pool.pool_size = user_free_pages * PG_SIZE;

    kernel_pool.phy_bitmap.btmp_bytes_len = kbm_length;
    user_pool.phy_bitmap.btmp_bytes_len = ubm_length;

    kernel_pool.phy_bitmap.bits = (void *)MEM_BITMAP_BASE;
    user_pool.phy_bitmap.bits = (void *)(MEM_BITMAP_BASE + kbm_length);

    console_write("\n");
    console_write_color("kernel_pool_phy_addr_start: ", rc_black, rc_green);
    console_write_hex(kernel_pool.phy_addr_start, rc_black, rc_green);
    console_write("\n");

    bitmap_init(&kernel_pool.phy_bitmap);
    bitmap_init(&user_pool.phy_bitmap);

    kernel_vaddr.vaddr_bitmap.btmp_bytes_len = kbm_length;
    kernel_vaddr.vaddr_bitmap.bits = (void *)MEM_BITMAP_BASE + kbm_length + ubm_length;

    kernel_vaddr.vaddr_start = K_HEAP_START;
    bitmap_init(&kernel_vaddr.vaddr_bitmap);
}

void mem_init() 
{
    console_write_color("Memory init\n", rc_black, rc_green);
    uint32_t total_men_size = (*(uint32_t *)0xb00);
    mem_pool_init(total_men_size);
}
