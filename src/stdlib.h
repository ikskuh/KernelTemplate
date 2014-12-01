#pragma once

#include "inttypes.h"
#include "varargs.h"

char *itoa(int value, char *str, int base);
int atoi(const char *str);
float atof(const char *str);

void *memset(void *ptr, int value, size_t num);
void *memcpy(void *destination, const void *source, size_t num);
void *memmove(void *destination, const void *source, size_t num);

char *strcpy(char *destination, const char *source);
char *strcat(char *destination, const char *source);
size_t strlen(const char *str);
