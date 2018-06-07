#ifndef BITMAP_H
#define BITMAP_H

#include "types.h"

typedef struct bitmap {
	uint32_t btmp_bytes_len;
	uint8_t *bits;
}bitmap;

void bitmap_init(bitmap *btmp);
int bitmap_get(bitmap *btmp, uint32_t bit_idx);
void bitmap_set(bitmap *btmp, uint32_t bit_idx, int8_t value);
int bitmap_scan(bitmap *btmp, uint32_t cnt); 

#endif