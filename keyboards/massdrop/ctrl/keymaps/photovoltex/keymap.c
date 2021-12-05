#include QMK_KEYBOARD_H
// #include "raw_hid.h"

#include <string.h>

#include "hid_wrapper.h"
#include "eons.h"

// #include "print.h"

// requires https://github.com/samhocevar/wincompose for windows => unicode converter

bool caps = false;

enum ctrl_keycodes {
    U_T_AUTO = SAFE_RANGE, //USB Extra Port Toggle Auto Detect / Always Active
    U_T_AGCR,              //USB Toggle Automatic GCR control
    DBG_TOG,               //DEBUG Toggle On / Off
    DBG_MTRX,              //DEBUG Toggle Matrix Prints
    DBG_KBD,               //DEBUG Toggle Keyboard Prints
    DBG_MOU,               //DEBUG Toggle Mouse Prints
    MD_BOOT,               //Restart into bootloader after hold timeout

    // custom keycodes
    EONS, // TODO: play with rgb (maybe let the key blink while in use => execute everywhere => execute cmd or so)
};

/*
    https://www.fastemoji.com/
    https://apps.timwhitlock.info/unicode/inspect

    https://docs.qmk.fm/#/feature_rawhid
*/

#define TAP_CODE_DELAY 1

const uint8_t UNDEFINED[RAW_EPSIZE] = "undefined";
const uint8_t SEQ_S[RAW_EPSIZE] = "seq_started";
const uint8_t CTC[RAW_EPSIZE] = "copied_to_clip";
const uint8_t SEQ_E[RAW_EPSIZE] = "seq_ended";

bool cmp_payloads(const uint8_t* cmp1, uint8_t* cmp2) {
    int count = 0, i = 0;
    for (; i < RAW_EPSIZE; i++) {
        if(cmp1[i] == cmp2[i]) {
            count++;
        }
    }
    return count == RAW_EPSIZE;
}

void raw_hid_receive(uint8_t *data, uint8_t length) {
    if(cmp_payloads(UNDEFINED, data)){
        // eons_symbol_fallback();
        s_hid_send("hewwo world uwu");
    } else if(cmp_payloads(SEQ_S, data)) {

        tap_code_delay(KC_V, 10);

        // tap_code16(C(KC_V));
        // unregister_code16(C(KC_V));

        // eons_start();
        // i_hid_send(SEQ_S);

    } else if(cmp_payloads(CTC, data)) {

        i_hid_send(CTC);

    } else if(cmp_payloads(SEQ_E, data)) {

        i_hid_send(SEQ_E);

    } else {
        s_hid_send("close");
        // execute ctrl + v
        // send back to app that the proccess is done
    }
    // else eons_success(); // probably ignore anything on success

}

void (*const sus)(const char*) = &send_unicode_string;

const eons_symbol_t eons_symbol_table[] = EONS_TABLE(
    EONS_SYM("ae", sus, "ä"), // ä
    EONS_SYM("oe", sus, "ö"), // ö
    EONS_SYM("ue", sus, "ü"), // ü
    EONS_SYM("AE", sus, "Ä"), // Ä
    EONS_SYM("UE", sus, "Ü"), // Ü
    EONS_SYM("OE", sus, "Ö"), // Ö
    EONS_SYM("ss", sus, "ß"), // ß

    //EONS_SYM("uwu",     sus, "(′ꈍωꈍ‵)"),  // ꈍ doesnt work
    EONS_SYM("mshrug", sus, "¯\\\\_(ツ)\\_/¯"),
    EONS_SYM("shrug",   sus, "¯\\_(ツ)_/¯"),
    EONS_SYM("cat",     sus, "( ⓛ ω ⓛ * )"),
    EONS_SYM("give",    sus, "ლ(◕ω◕ლ)"),
    EONS_SYM("sweat",   sus, "(⌒_⌒;)"),
    EONS_SYM("zzz",     sus, "( ≚ᄌ≚)ƶƵ"),
    EONS_SYM("smile",   sus, "(´• ᴗ •̥`)"),
    EONS_SYM("eyes",    sus, "( ´◔ ω◔`)"),
    EONS_SYM("worries", sus, ":( ´◦ω◦｀):"),
    EONS_SYM("serious", sus, "ಠ_ಠ")
);

// layer
/*

        UCIS                => activates ucis -> convert text to unicode chars
        MD_BOOT             => custom restart -> hold 0.5 sec (B key on board)

        KC_P<0-9>           => numpad keys
        NK_TOGG             => toogle n-key rollover


    RGB LIGHTING see https://docs.qmk.fm/#/keycodes?id=rgb-lighting
        RGB_TOG             => toogles rgb -> toogles between bottom off,
        RGB_SPD / RGB_SPI   => decreases or increases the rgb speed
        RGB_SAD / RGB_SAI   => decreases or increases the saturation
        RGB_VAD / RGB_VAI   => decreases or increases the brightness
        RGB_HUD / RGB_HUI   => decreases or increases hue (seems to do nothing on this board TODO:  maybe config problem or so)
        RGB_RMOD / RGB_MOD  => switches between the rgb modes => TODO: find out where this is customizable

    BASIC KEYCODES https://docs.qmk.fm/#/keycodes?id=basic-keycodes
        KC_MPLY             => start / pause track
        KC_MUTE             => mute
        KC_MPRV / KC_MNXT   => next / prev track
        KC_VOLD / KC_VOLU   => decreases or increases volume

    UNICODE see https://docs.qmk.fm/#/feature_unicode
        UC_RMOD / UC_MOD    => switches between the unicode converter mods if multiple implemented => see config.h UNICODE_SELECTED_MODES

*/

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,             MO(2),   MO(3),   RESET, \
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,   KC_INS,  KC_HOME, KC_PGUP, \
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,   KC_DEL,  KC_END,  KC_PGDN, \
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT, \
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,                              KC_UP, \
        KC_LCTL, KC_LGUI, KC_LALT,                   KC_SPC,                             EONS,    MO(1),   KC_APP,  KC_RCTL,            KC_LEFT, KC_DOWN, KC_RGHT \
    ),
    [1] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,            KC_PSCR, KC_SLCK, KC_PAUS, \
        _______, KC_P1,   KC_P2,   KC_P3,   KC_P4,   KC_P5,   KC_P6,   KC_P7,   KC_P8,   KC_P9,   KC_P0,   _______, _______, _______,   KC_MPRV, KC_MPLY, KC_MNXT, \
        _______, RGB_SPD, RGB_VAI, RGB_SPI, RGB_HUI, RGB_SAI, _______, U_T_AUTO,U_T_AGCR,_______, _______, _______, _______, _______,   KC_VOLD, KC_MUTE, KC_VOLU, \
        _______, RGB_RMOD,RGB_VAD, RGB_MOD, RGB_HUD, RGB_SAD, _______, _______, _______, _______, _______, _______, _______, \
        _______, RGB_TOG, _______, _______, _______, MD_BOOT, NK_TOGG, _______, _______, _______, _______, _______,                              _______, \
        _______, _______, _______,                   _______,                            KC_RALT, _______, _______, _______,            UC_RMOD, _______, UC_MOD   \
    ),
    /*
    [X] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,            _______, _______, _______, \
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,   _______, _______, _______, \
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,   _______, _______, _______, \
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
        _______, _______, _______, _______, _______, _______, NK_TOGG, _______, _______, _______, _______, _______,                              _______, \
        _______, _______, _______,                   _______,                            _______, _______, _______, _______,            _______, _______, _______ \
    ),
    */
};

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {
    unicode_input_mode_init();
};

// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {
};


#define MODS_SHIFT  (get_mods() & MOD_MASK_SHIFT)
#define MODS_CTRL   (get_mods() & MOD_MASK_CTRL)
#define MODS_ALT    (get_mods() & MOD_MASK_ALT)

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint32_t key_timer;

    if(keycode == MD_BOOT){
        if (record->event.pressed) {
            key_timer = timer_read32();
        } else {
            if (timer_elapsed32(key_timer) >= 5) {
                reset_keyboard();
            }
        }
        return false;
    }


    // process_unicode_common(keycode, record)
    if(record->event.pressed)
        if(!eons_processing(keycode, caps))
            return false;


    switch (keycode) {
        case U_T_AUTO:
            if (record->event.pressed && MODS_SHIFT && MODS_CTRL) {
                TOGGLE_FLAG_AND_PRINT(usb_extra_manual, "USB extra port manual mode");
            }
            return false;
        case U_T_AGCR:
            if (record->event.pressed && MODS_SHIFT && MODS_CTRL) {
                TOGGLE_FLAG_AND_PRINT(usb_gcr_auto, "USB GCR auto mode");
            }
            return false;
        case DBG_TOG:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_enable, "Debug mode");
            }
            return false;
        case DBG_MTRX:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_matrix, "Debug matrix");
            }
            return false;
        case DBG_KBD:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_keyboard, "Debug keyboard");
            }
            return false;
        case DBG_MOU:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_mouse, "Debug mouse");
            }
            return false;
        case RGB_TOG:
            if (record->event.pressed) {
              switch (rgb_matrix_get_flags()) {
                case LED_FLAG_ALL: {
                    rgb_matrix_set_flags(LED_FLAG_KEYLIGHT | LED_FLAG_MODIFIER | LED_FLAG_INDICATOR);
                    rgb_matrix_set_color_all(0, 0, 0);
                  }
                  break;
                case (LED_FLAG_KEYLIGHT | LED_FLAG_MODIFIER | LED_FLAG_INDICATOR): {
                    rgb_matrix_set_flags(LED_FLAG_UNDERGLOW);
                    rgb_matrix_set_color_all(0, 0, 0);
                  }
                  break;
                case LED_FLAG_UNDERGLOW: {
                    rgb_matrix_set_flags(LED_FLAG_NONE);
                    rgb_matrix_disable_noeeprom();
                  }
                  break;
                default: {
                    rgb_matrix_set_flags(LED_FLAG_ALL);
                    rgb_matrix_enable_noeeprom();
                  }
                  break;
              }
            }
            return false;
        case KC_CAPS:
            if (record->event.pressed) {
                caps = !caps;
            }
            return true;

        case EONS:
            if (record->event.pressed) {
                eons_init();
            }
            return false;
        /*
        case some_case:
            if (record->event.pressed) {

            } else {

            }
            break;
        */
        default:
            return true; //Process all other keycodes normally
    }
}
