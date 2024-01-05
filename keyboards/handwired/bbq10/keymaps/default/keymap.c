// Copyright 2024 viteo (viteo@tut.by)
// SPDX-License-Identifier: GPL-2.0-or-later


#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
     * │ Q │ W │ E │ R │ T │ Y │ U │ I │ O │ P │
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │ A │ S │ D │ F │ G │ H │ J │ K │ L │BkS│
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │Alt│ Z │ X │ C │ V │ B │ N │ M │ $ │Ent│
     * └───┼───┼───┼───┴───┴───┴───┼───┼───┼───┘
     *     │LSh│LCt│  S p a c e    │Sym│RSh│
     *     └───┴───┴───────────────┴───┴───┘
     */
    [0] = LAYOUT(
            KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, 
            KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_BSPC, 
            KC_LALT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_DLR,  KC_ENT, 
                     KC_LSFT, KC_LCTL,                KC_SPC,              MO(1),   KC_RSFT
    ),
    /*
     * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
     * │ # │ 1 │ 2 │ 3 │ ( │ ) │ _ │ - │ + │ @ │
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │ * │ 4 │ 5 │ 6 │ / │ : │ ; │ ' │ " │BkS│
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │Bre│ 7 │ 8 │ 9 │ ? │ ! │ , │ . │Mut│Ent│
     * └───┼───┼───┼───┴───┴───┴───┼───┼───┼───┘
     *     │BlD│ 0 │   BlToggle    │Sym│BlU│
     *     └───┴───┴───────────────┴───┴───┘
     */
    [1] = LAYOUT(
            KC_HASH, KC_1,    KC_2,    KC_3,    KC_LPRN, KC_RPRN, KC_UNDS, KC_MINS, KC_PLUS, KC_AT, 
            KC_PAST, KC_4,    KC_5,    KC_6,    KC_SLASH,KC_COLN, KC_SCLN, KC_QUOT, KC_DQUO, KC_BSPC, 
            BL_BRTG, KC_7,    KC_8,    KC_9,    KC_QUES, KC_EXLM, KC_COMM, KC_DOT,  KC_MUTE, KC_ENT, 
                     BL_DOWN, KC_0,                   BL_TOGG,             MO(1),   BL_UP
    )
};