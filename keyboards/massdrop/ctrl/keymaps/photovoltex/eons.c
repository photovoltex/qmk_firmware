#include QMK_KEYBOARD_H
#include "eons.h"
#include "hid_wrapper.h"

// #include "print.h"

eons_state_t eons_state;

bool scan = false;

void eons_init(void) {
    s_hid_send("start");
}

void eons_start(void) {
    if (!eons_state.in_progress) {
        eons_state.count       = 0;
        // should only start after the strg + v event was send
        eons_state.in_progress = true;
    } else {
        eons_processing(KC_SPC, false);
    }
}

__attribute__((weak)) void eons_success(uint8_t symbol_index) {}

// static bool is_uni_seq(char *seq) {
//     uint8_t i;
//     for (i = 0; seq[i]; i++) {
//         uint16_t keycode;
//         if ('1' <= seq[i] && seq[i] <= '0') {
//             keycode = seq[i] - '1' + KC_1;
//         } else if (eons_state.shift[i]) {
//             keycode = seq[i] - 'A' + KC_A;
//         } else {
//             keycode = seq[i] - 'a' + KC_A;
//         }

//         if (i > eons_state.count || eons_state.codes[i] != keycode) {
//             return false;
//         }
//     }
//     return eons_state.codes[i] == KC_ENT || eons_state.codes[i] == KC_SPC;
// }

// __attribute__((weak))
void eons_symbol_fallback(void) {
    for (uint8_t i = 0; i < eons_state.count - 1; i++) {
        uint8_t keycode = eons_state.codes[i];
        register_code(keycode);
        unregister_code(keycode);
        wait_ms(TYPE_DELAY);
    }
}

#define MODS_SHIFT (get_mods() & MOD_MASK_SHIFT)
#define MODS_CTRL (get_mods() & MOD_MASK_CTRL)

bool eons_processing(uint16_t keycode, bool caps) {
    // ignore if not active
    if (!eons_state.in_progress) {
        return true;
    }

    bool special = keycode == KC_SPC || keycode == KC_ENT || keycode == KC_ESC || keycode == KC_BSPC;
    if (eons_state.count >= EONS_MAX_SYMBOL_LENGTH && !special) {
        return false;
    }

    eons_state.codes[eons_state.count] = keycode;
    eons_state.shift[eons_state.count] = MODS_SHIFT || caps;
    eons_state.count++;

    switch (keycode) {
        // backspace
        case KC_BSPC:
            // delete full word => also deletes unicode keyboard => counts as cancel
            if (MODS_CTRL) {
                eons_state.in_progress = false;
                return true;
            }

            if (eons_state.count >= 2) {
                eons_state.count -= 2;
                return true;
            } else {
                eons_state.count--;
                return false;
            }

        // shift
        case KC_LSFT:
        case KC_RSFT:
            eons_state.count--;
            return true;


        // execute on space, enter or esc
        case KC_SPC:
        case KC_ENT:
        case KC_ESC:
            // delete user input
            for (uint8_t i = 0; i < eons_state.count; i++) {
                register_code(KC_BSPC);
                unregister_code(KC_BSPC);
                wait_ms(TYPE_DELAY);
            }

            // exit the process and leave everything how it is
            if (keycode == KC_ESC) {
                eons_state.in_progress = false;
                return false;
            }

            // converts the saved input into a string and sends it to the system via hid api
            // ' ' is used as seperator and tell the app which of the buffer is the actual string
            uint8_t data[RAW_EPSIZE];
            data[0] = ' ';
            for (uint8_t i = 0; i < eons_state.count; i++) {
                if (eons_state.shift[i])
                    data[i + 1] = eons_state.codes[i] - KC_A + 'A';
                else
                    data[i + 1] = eons_state.codes[i] - KC_A + 'a';
            }
            data[eons_state.count] = ' ';
            i_hid_send(data);

            // if (symbol_found) {
            //     eons_success(i);
            // } else {
            //     eons_symbol_fallback();
            // }

            eons_state.in_progress = false;
            return false;

        default:
            return true;
    }
}
