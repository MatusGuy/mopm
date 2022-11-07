/*
 * Created on Sun Nov 06 2022
 * mopm Package Manger
 * https://github.com/Localtings/mopm
 * Licensed under MIT license
 * Copyright (c) 2022 Localtings
 */

#ifndef M_DEBUG_H_
#define M_DEBUG_H_

#include <stdlib.h>

#ifdef _DEBUG
#define m_malloc(x) _m_malloc(x, __FILE__, __LINE__, __FUNCTION__)
#define m_free(x) _m_free(x, __FILE__, __LINE__, __FUNCTION__)
#define m_strdup(x) _m_strdup(x, __FILE__, __LINE__, __FUNCTION__)
#else
#define m_malloc(x) malloc(x)
#define m_free(x) free(x)
#define m_strdup(x) strdup(x)
#endif

void* _m_malloc(size_t size, const char *file, const int line, const char *func);
void _m_free(void* block, const char *file, const int line, const char *func);
char *_m_strdup(char const* str, const char *file, const int line, const char *func);

#endif