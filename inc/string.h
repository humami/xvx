#ifndef STRING_H
#define STRING_H

#include "types.h"

inline void memcpy(uint8_t *dest, uint8_t *src, uint32_t len);

inline void memset(void *dest, uint8_t val, uint32_t len);

inline void bzero(void *dest, uint32_t len);

inline int strcmp(const char *str1, const char *str2);

inline char* strcpy(char *dest, const char *src);

inline int strlen(const char *str);

#endif