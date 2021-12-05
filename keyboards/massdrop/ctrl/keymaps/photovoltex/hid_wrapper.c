#include QMK_KEYBOARD_H
#include "hid_wrapper.h"

#include <string.h>

uint8_t payload[RAW_EPSIZE];

void reset_payload(void) {
    for(int i = 0; i < RAW_EPSIZE; i++) {
        payload[i] = 0;
    };
}

void create_payload(const char* string){
    for(int i = 0; string[i]; i++) {
        payload[i] = string[i];
    };
}

void s_hid_send(const char* string) {
    reset_payload();
    create_payload(string);
    raw_hid_send(payload, RAW_EPSIZE);
}

void i_hid_send(const uint8_t* integers) {
    reset_payload();
    memcpy(payload, integers, RAW_EPSIZE);
    raw_hid_send(payload, RAW_EPSIZE);
}
