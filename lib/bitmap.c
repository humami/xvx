#include "bitmap.h"
#include "string.h"
#include "debug.h"

void bitmap_init(bitmap *btmp)
{
    memset(btmp->bits, 0, btmp->btmp_bytes_len);
}

int bitmap_get(bitmap *btmp, uint32_t bit_idx)
{
    uint32_t byte_idx = bit_idx / 8;
    uint32_t bit_odd = bit_idx % 8;

    return (btmp->bits[byte_idx] & (1 << bit_odd)); 
}

void bitmap_set(bitmap *btmp, uint32_t bit_idx, int8_t value)
{   
    ASSERT((value == 0) || (value == 1));
    uint32_t byte_idx = bit_idx / 8;
    uint32_t bit_odd = bit_idx % 8;

    if(value) {
        btmp->bits[byte_idx] |= (1 << bit_odd);
    } else {
        btmp->bits[byte_idx] &= ~(1 << bit_odd);
    }
}

int bitmap_scan(bitmap *btmp, uint32_t cnt)
{
    uint32_t idx_byte = 0;

    while(0xFF == btmp->bits[idx_byte] && (idx_byte < btmp->btmp_bytes_len))
        idx_byte++;
    
    ASSERT(idx_byte < btmp->btmp_bytes_len);
    if(idx_byte == btmp->btmp_bytes_len)
        return -1;

    uint32_t idx_bit = 0;

    while((uint8_t)((1 << idx_bit) & btmp->bits[idx_byte]))
        idx_bit++;

    uint32_t bit_idx_start = idx_byte * 8 + idx_bit;
    if(cnt == 1)
        return bit_idx_start;

    uint32_t bit_left = btmp->btmp_bytes_len * 8 - bit_idx_start - 1;
    uint32_t next_bit = bit_idx_start + 1;
    uint32_t count = 0;

    while(bit_left > 0)
    {
        if(!bitmap_get(btmp, next_bit))
            count++;
        else
            count = 0;

        if(count == cnt)
        {
            bit_idx_start = next_bit - cnt + 1;
            break;
        }

        next_bit++;
        bit_left--;
    }

    return bit_idx_start;
}