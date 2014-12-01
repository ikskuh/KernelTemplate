#include "stdlib.h"

static void reverse(char *str, int length)
{
    int start = 0;
    int end = length -1;
    while (start < end)
    {
		char tmp = *(str+start);
		*(str+start) = *(str+end);
		*(str+end) = tmp;
        start++;
        end--;
    }
}

char *itoa(int num, char *str, int base)
{
    int i = 0;
    int isNegative = 0;
 
    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
 
    // In standard itoa(), negative numbers are handled only with 
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = 1;
        num = -num;
    }
 
    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'A' : rem + '0';
        num = num/base;
    }
 
    // If number is negative, append '-'
    if (isNegative)
	{
		str[i++] = '-';
	}
    str[i] = '\0'; // Append string terminator
 
    // Reverse the string
    reverse(str, i);
 
    return str;	
}

int atoi(const char *str)
{
	int res = 0;
	for (int i = 0; str[i] != '\0'; ++i)
	{
		res = res * 10 + str[i] - '0';
	}
	return res;
}

void *memset(void *ptr, int value, size_t num)
{
	uint8_t *it = (uint8_t*)ptr;
	while((num--) > 0)
	{
		*(it++) = (uint8_t)(value & 0xFF);
	}
	return ptr;
}

void *memcpy(void *destination, const void *source, size_t num)
{
	uint8_t *to = (uint8_t*)destination;
	uint8_t *from = (uint8_t*)source;
	while((num--) > 0)
	{
		*(to++) = *(from++);
	}
	return destination;
}

void *memmove( void *destination, const void *source, size_t num)
{
	// TODO: Implement memmove
	return nullptr;
}

char *strcpy(char *destination, const char *source)
{
	while(*source)
	{
		*(destination++) = *(source++);
	}
	return destination;
}

char *strcat(char *destination, const char *source)
{
	// TODO: Implement strcat
	return nullptr;
}

size_t strlen(const char *str)
{
	size_t size = 0;
	while(*(str++) != 0) size++;
	return size;
}