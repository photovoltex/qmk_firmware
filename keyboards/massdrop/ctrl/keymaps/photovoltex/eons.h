#pragma once

#include <stdint.h>
#include <stdbool.h>

//#include "action.h"
#include "quantum.h"
#include "process_unicode_common.h"

// #ifndef EONS_MAX_PARAMS
// #   define EONS_MAX_PARAMS          3
// #endif
#ifndef EONS_MAX_PARAMS_LENGTH
#   define EONS_MAX_PARAMS_LENGTH   32
#endif
#ifndef EONS_MAX_SYMBOL_LENGTH
#   define EONS_MAX_SYMBOL_LENGTH   10
#endif
#ifndef TYPE_DELAY
#   define TYPE_DELAY               5
#endif

typedef struct {
    char *  symbol;
    void (*const func_ptr)(const char*);
    char params/*[EONS_MAX_PARAMS]*/[EONS_MAX_PARAMS_LENGTH];
} eons_symbol_t;

typedef struct {
    uint8_t  count;
    uint16_t codes[EONS_MAX_SYMBOL_LENGTH];
    bool     shift[EONS_MAX_SYMBOL_LENGTH];
    bool     in_progress : 1;
} eons_state_t;

extern eons_state_t eons_state;

#define EONS_TABLE(...) \
    {                   \
        __VA_ARGS__,    \
        { NULL, NULL, {} }    \
    }
#define EONS_SYM(name, func_ptr, ...) \
    { name, func_ptr, {__VA_ARGS__} }

extern const eons_symbol_t eons_symbol_table[];

void eons_init(void);
void eons_start(void);
void eons_start_user(void);
void eons_symbol_fallback(void);
void eons_success(uint8_t symbol_index);

bool eons_processing(uint16_t keycode, bool caps);
