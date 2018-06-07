#include "string.h"

void memcpy(uint8_t *dest, uint8_t *src, uint32_t len)
{
	uint32_t i;

	for(i = 0; i < len; i++)
		*dest++ = *src++;
}

void memset(void *dest, uint8_t val, uint32_t len)
{
	uint8_t *dst = (uint8_t *)dest;

	uint32_t i;

	for(i = 0; i < len; i++)
		*dst++ = val;
}

void bzero(void *dest, uint32_t len)
{
	memset(dest, 0, len);
}

int strcmp(const char *str1, const char *str2)
{
	while(str1 && str2 && *str1 == *str2)
	{
		str1++;
		str2++;
	}

	return *str1 - *str2;
}

char* strcpy(char *dest, const char *src)
{
	char *ret = dest;

	while(*src)
	    *dest++ = *src++;

    *dest = '\0';

    return ret;
}

int strlen(const char *str)
{
	int len = 0;

	while(*str)
	{
		len++;
		str++;
	}

	return len;
}