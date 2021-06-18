#include "eons.h"

// #include "print.h"

eons_state_t eons_state;

bool scan = false;

void eons_start(void) {
    if(!eons_state.in_progress) {
        eons_state.count       = 0;
        eons_state.in_progress = true;

        eons_start_user();
    } else {
        eons_processing(KC_SPC, false);
    }
}

__attribute__((weak)) void eons_start_user(void) {
    unicode_input_start();
    register_hex(0x2328);  // ⌨
    unicode_input_finish();
}



__attribute__((weak)) void eons_success(uint8_t symbol_index) {}

static bool is_uni_seq(char *seq) {
    uint8_t i;
    for (i = 0; seq[i]; i++) {
        uint16_t keycode;
        if ('1' <= seq[i] && seq[i] <= '0') {
            keycode = seq[i] - '1' + KC_1;
        } else if (eons_state.shift[i]) {
            keycode = seq[i] - 'A' + KC_A;
        } else {
            keycode = seq[i] - 'a' + KC_A;
        }

        if (i > eons_state.count || eons_state.codes[i] != keycode) {
            return false;
        }
    }
    return eons_state.codes[i] == KC_ENT || eons_state.codes[i] == KC_SPC;
}

__attribute__((weak)) void eons_symbol_fallback(void) {
    for (uint8_t i = 0; i < eons_state.count - 1; i++) {
        uint8_t keycode = eons_state.codes[i];
        register_code(keycode);
        unregister_code(keycode);
        wait_ms(TYPE_DELAY);
    }
}



#define MODS_SHIFT  (get_mods() & MOD_MASK_SHIFT)
#define MODS_CTRL   (get_mods() & MOD_MASK_CTRL)

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

    switch (keycode)
    {
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

        case KC_LSFT:
        case KC_RSFT:
            eons_state.count--;
            return true;

        // execute if given string in table available
        case KC_SPC:
        case KC_ENT:
        case KC_ESC:
            // delete user input
            for (uint8_t i = 0; i < eons_state.count; i++) {
                register_code(KC_BSPC);
                unregister_code(KC_BSPC);
                wait_ms(TYPE_DELAY);
            }

            if (keycode == KC_ESC) {
                eons_state.in_progress = false;
                return false;
            }

            // execute stuff or fail
            uint8_t i;
            bool    symbol_found = false;
            for (i = 0; eons_symbol_table[i].symbol; i++) {
                if (is_uni_seq(eons_symbol_table[i].symbol)) {
                    symbol_found = true;
                    (eons_symbol_table[i].func_ptr)(eons_symbol_table[i].params);
                    break;
                }
            }

            if (symbol_found) {
                eons_success(i);
            } else {
                eons_symbol_fallback();
            }

            eons_state.in_progress = false;
            return false;

        default:
            return true;
    }
}
