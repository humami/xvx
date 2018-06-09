#include "string.h"
#include "debug.h"

void memcpy(uint8_t *dest, uint8_t *src, uint32_t len)
{
    ASSERT(dest != NULL && src != NULL);
    uint32_t i;

    for(i = 0; i < len; i++)
        *dest++ = *src++;
}

void memset(void *dest, uint8_t val, uint32_t len)
{
    ASSERT(dest != NULL);
    uint8_t *dst = (uint8_t *)dest;

    uint32_t i;

    for(i = 0; i < len; i++)
        *dst++ = val;
}

void bzero(void *dest, uint32_t len)
{
    ASSERT(dest != NULL);
    memset(dest, 0, len);
}

int strcmp(const char *str1, const char *str2)
{   
    ASSERT(str1 != NULL && str2 != NULL);
    while(str1 && str2 && *str1 == *str2)
    {
        str1++;
        str2++;
    }

    return *str1 - *str2;
}

char* strcpy(char *dest, const char *src)
{   
    ASSERT(dest != NULL && src != NULL);
    char *ret = dest;

    while(*src)
        *dest++ = *src++;

    *dest = '\0';

    return ret;
}

int strlen(const char *str)
{   
    ASSERT(str != NULL);
    int len = 0;

    while(*str)
    {
        len++;
        str++;
    }

    return len;
}