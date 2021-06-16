/* Copyright 2020-2021 doodboard
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H

//function that turnes off our oled when the pc sleeps to prevent burn in
void suspend_power_down_user(void) {
    oled_off();
}


enum custom_keycodes {
    COPY_ALL = SAFE_RANGE, // Copy all text using ctrl(a+c)
    SYS_MIC_M,             // CTRL+SHIFT+ALT+M
    SEL_CPY,               // Select word under cursor and copy. Double mouse click then ctrl+c
    FILE_RENAME_S,         // small macro to help with renaming stuff  f2 + ctrl+v
    FILE_RENAME_M,          // another rename macro   enter and down arrow
    LOCK_PC,                // left gui + L
    KC_00                  // double zero
};


enum tapdance_keycodes {
    TD_CUST_MUTE = 0,     // Tap dance key to mute sound and mic on double press
    TD_MEDIA,     // same controls as wired earbuds
};

// fearherbs1 custom music controls like earbuds 
void dance_media (qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        tap_code(KC_MPLY);
    } else if (state->count == 2) {
        tap_code (KC_MNXT);
    } else if (state->count == 3) {
        tap_code(KC_MPRV);
    } else if (state->count == 4) {
        tap_code(KC_MUTE);
    } else {
        reset_tap_dance (state);
    }
}

//Associate our tap dance key with its functionality
qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_CUST_MUTE] = ACTION_TAP_DANCE_DOUBLE(KC_MUTE, KC_MPLY),
    [TD_MEDIA] = ACTION_TAP_DANCE_FN (dance_media),
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
                 TG(1),   KC_PSLS, KC_PAST, SYS_MIC_M,
		         KC_7,    KC_8,    KC_9,    KC_PMNS,
		         KC_4,    KC_5,    KC_6,    KC_PPLS,
		TD(TD_MEDIA), KC_1,    KC_2,    KC_3,    KC_ENT,
		LT(3,KC_BSPC),    KC_00,    KC_0,    KC_DOT,  KC_ENT),

    [1] = LAYOUT(
                 TG(1),   KC_TRNS, KC_TRNS, KC_TRNS,
		         KC_HOME, KC_UP,   KC_PGUP, KC_TRNS,
		         KC_LEFT, KC_TRNS, KC_RGHT, KC_TRNS,
		KC_TRNS, KC_END,  KC_DOWN, KC_PGDN, KC_TRNS,
		TG(2),   KC_TRNS, KC_INS,  KC_DEL,  KC_TRNS),

    [2] = LAYOUT(
                 KC_TRNS, RGB_TOG, RGB_MOD, KC_TRNS,
		         RGB_HUI, RGB_SAI, RGB_VAI, KC_TRNS,
		         RGB_HUD, RGB_SAD, RGB_VAD, KC_TRNS,
		RESET,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		TG(2),   KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS),

    [3] = LAYOUT(
                 KC_TRNS, KC_TRNS, KC_TRNS, TG(4),
		         KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		         KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		RESET,   COPY_ALL, SEL_CPY, KC_TRNS, LOCK_PC,
		KC_TRNS, FILE_RENAME_S, FILE_RENAME_M, KC_CALC, LOCK_PC),

    [4] = LAYOUT(
                 KC_ESC, KC_TILD, XXXXXXX, TG(4),
		         KC_F8, KC_F10, KC_F12, KC_DOWN,
		         XXXXXXX, XXXXXXX, XXXXXXX, KC_UP,
		RESET,   XXXXXXX, XXXXXXX, XXXXXXX, KC_ENT,
		XXXXXXX, KC_Z, KC_X, KC_C, KC_ENT),
};


//custom keycodes
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case COPY_ALL:
            // Selects all and text and copy
            if (record->event.pressed) {
                SEND_STRING(SS_LCTRL("ac"));
            }
            return false;
        case SYS_MIC_M:
            // Sends ctrl + shift + alt + M to use micswitch to mute mic
            if (record->event.pressed) {
                SEND_STRING(SS_LCTRL(SS_LSFT(SS_LALT("m"))));
            }
            return false;
        case SEL_CPY:
            // Select word under cursor and copy. Double mouse click then ctrl+c
            if (record->event.pressed) {
                tap_code16(KC_BTN1);
                tap_code16(KC_BTN1);
                tap_code16(C(KC_C));
            }
            return false;
        case FILE_RENAME_S:
            if (record->event.pressed) {
                tap_code16(KC_F2);
                SEND_STRING(SS_LCTRL("v"));
            }
            return false;
        case FILE_RENAME_M:
            if (record->event.pressed) {
                tap_code16(KC_ENT);
                tap_code16(KC_DOWN);
            }
            return false;
        case LOCK_PC:
            if (record->event.pressed) {
                tap_code16(LGUI(KC_L));
            }
            return false;
        case KC_00:
            if (record->event.pressed) {
                tap_code16(KC_0);
                tap_code16(KC_0);
            }
            return false;
        default:
            return true; // Process all other keycodes normally
    }
}

// encoder stuff
bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) { /* First encoder */
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    }
    return true;
}


#ifdef OLED_DRIVER_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_270; }


// WPM-responsive animation stuff here
#define IDLE_FRAMES 2
#define IDLE_SPEED 40 // below this wpm value your animation will idle

#define ANIM_FRAME_DURATION 200 // how long each frame lasts in ms
// #define SLEEP_TIMER 60000 // should sleep after this period of 0 wpm, needs fixing
#define ANIM_SIZE 636 // number of bytes in array, minimize for adequate firmware size, max is 1024

uint32_t anim_timer = 0;
uint32_t anim_sleep = 0;
uint8_t current_idle_frame = 0;

// Credit to u/Pop-X- for the initial code. You can find his commit here https://github.com/qmk/qmk_firmware/pull/9264/files#diff-303f6e3a7a5ee54be0a9a13630842956R196-R333.
static void render_anim(void) {
    static const char PROGMEM idle[IDLE_FRAMES][ANIM_SIZE] = {
        {
        0,  0,192,192,192,192,192,192,192,248,248, 30, 30,254,254,248,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  3,  3,  3,  3,  3,255,255,255,255,255,255,255,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,127,127,255,255,255,255,255,159,159,135,135,129,129,129, 97, 97, 25, 25,  7,  7,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1, 97, 97,127,  1,  1, 97, 97,127,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0
        },
        {
        0,  0,128,128,128,128,128,128,128,240,240, 60, 60,252,252,240,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  7,  7,  7,  7,  7,255,255,254,254,255,255,255,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,255,255,255,255,255,255,255, 63, 63, 15, 15,  3,  3,  3,195,195, 51, 51, 15, 15,  3,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  3,  3, 99, 99,127,  3,  3, 99, 99,127,  3,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0
        }
    };

    //assumes 1 frame prep stage
    void animation_phase(void) {
            current_idle_frame = (current_idle_frame + 1) % IDLE_FRAMES;
            oled_write_raw_P(idle[abs((IDLE_FRAMES-1)-current_idle_frame)], ANIM_SIZE);
    }

        if(timer_elapsed32(anim_timer) > ANIM_FRAME_DURATION) {
            anim_timer = timer_read32();
            animation_phase();
        }
    }

void oled_task_user(void) {
        render_anim();
        oled_set_cursor(0,6);
        oled_write_P(PSTR("Duck\nBoard\n"), false);
    oled_write_P(PSTR("-----\n"), false);
    // Host Keyboard Layer Status
    oled_write_P(PSTR("Mode:\n"), false);
    oled_write_P(PSTR("\n"), false);
    // add in oled display for our custom layers
    switch (get_highest_layer(layer_state)) {
        case 0:
            oled_write_P(PSTR("Base\n"), false);
            break;
        case 1:
            oled_write_P(PSTR("Func\n"), false);
            break;
        case 2:
            oled_write_P(PSTR("RGB\n"), false);
            break;
        case 3:
            oled_write_P(PSTR("Macro\n"), false);
            break;
        case 4:
            oled_write_P(PSTR("osu!\n"), false);
            break;
    }
}
#endif

void keyboard_post_init_user(void) {
  //Customise these values to debug
  debug_enable=true;
  debug_matrix=true;
  //debug_keyboard=true;
  //debug_mouse=true;
}
