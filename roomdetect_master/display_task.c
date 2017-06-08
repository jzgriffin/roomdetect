// display_task.c
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
// Assignment: Custom Project - Room Detector - Master Application
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#include "display_task.h"
#include "detection_task.h"
#include "training_task.h"
#include "mode_task.h"
#include "lcd_task.h"
#include <stdio.h>
#include <string.h>

enum {
    STATE_INITIAL,
    STATE_CONFIGURE,
    STATE_CHANGE_MODE,

    // Detection mode
    STATE_DETECTION_ENTER,
    STATE_DETECTION_READY,
    STATE_DISPLAY_NO_DETECT_WAIT,
    STATE_DISPLAY_NO_DETECT,
    STATE_DISPLAY_DETECT_WAIT,
    STATE_DISPLAY_DETECT,
    STATE_DISPLAY_SAVE_WAIT,
    STATE_DISPLAY_SAVE,
    STATE_DISPLAY_RESET_WAIT,
    STATE_DISPLAY_RESET,
    STATE_DISPLAY_ERASE_WAIT,
    STATE_DISPLAY_ERASE,

    // Training mode
    STATE_TRAINING_ENTER,
    STATE_TRAINING_READY,
    STATE_SELECT,
    STATE_DISPLAY_SELECT_WAIT,
    STATE_DISPLAY_SELECT,
    STATE_TRAIN,
    STATE_DISPLAY_TRAIN_WAIT,
    STATE_DISPLAY_TRAIN,
    STATE_TRAIN_WAIT,
};

static task_state_t tick(task_state_t state)
{
    static uint8_t last_room;

    // Transitions
    switch (state) {
        case STATE_INITIAL:
            state = STATE_CONFIGURE;
            break;

        case STATE_CONFIGURE:
        case STATE_CHANGE_MODE:
            if (device_mode == DEVICE_MODE_TRAINING) {
                state = STATE_TRAINING_ENTER;
            }
            else {
                state = STATE_DETECTION_ENTER;
            }
            break;

        case STATE_DETECTION_ENTER:
            state = STATE_DISPLAY_NO_DETECT_WAIT;
            break;

        case STATE_DETECTION_READY:
            if (device_mode != DEVICE_MODE_DETECTION) {
                state = STATE_CHANGE_MODE;
            }
            else if (last_room != detected_room) {
                if (detected_room == UINT8_MAX) {
                    state = STATE_DISPLAY_NO_DETECT_WAIT;
                }
                else {
                    state = STATE_DISPLAY_DETECT_WAIT;
                }
            }
            else if (should_display_save) {
                state = STATE_DISPLAY_SAVE_WAIT;
            }
            else if (should_display_reset) {
                state = STATE_DISPLAY_RESET_WAIT;
            }
            else if (should_display_erase) {
                state = STATE_DISPLAY_ERASE_WAIT;
            }
            else {
                state = STATE_DETECTION_READY;
            }
            break;

        case STATE_DISPLAY_NO_DETECT:
        case STATE_DISPLAY_DETECT:
        case STATE_DISPLAY_SAVE:
        case STATE_DISPLAY_RESET:
        case STATE_DISPLAY_ERASE:
            state = STATE_DETECTION_READY;
            break;

        case STATE_DISPLAY_NO_DETECT_WAIT:
            if (device_mode != DEVICE_MODE_DETECTION) {
                state = STATE_CHANGE_MODE;
            }
            else if (is_lcd_ready) {
                state = STATE_DISPLAY_NO_DETECT;
            }
            else {
                state = STATE_DISPLAY_NO_DETECT_WAIT;
            }
            break;

        case STATE_DISPLAY_DETECT_WAIT:
            if (device_mode != DEVICE_MODE_DETECTION) {
                state = STATE_CHANGE_MODE;
            }
            else if (is_lcd_ready) {
                state = STATE_DISPLAY_DETECT;
            }
            else {
                state = STATE_DISPLAY_DETECT_WAIT;
            }
            break;

        case STATE_DISPLAY_SAVE_WAIT:
            if (device_mode != DEVICE_MODE_DETECTION) {
                state = STATE_CHANGE_MODE;
            }
            else if (is_lcd_ready) {
                state = STATE_DISPLAY_SAVE;
            }
            else {
                state = STATE_DISPLAY_SAVE_WAIT;
            }
            break;

        case STATE_DISPLAY_RESET_WAIT:
            if (device_mode != DEVICE_MODE_DETECTION) {
                state = STATE_CHANGE_MODE;
            }
            else if (is_lcd_ready) {
                state = STATE_DISPLAY_RESET;
            }
            else {
                state = STATE_DISPLAY_RESET_WAIT;
            }
            break;

        case STATE_DISPLAY_ERASE_WAIT:
            if (device_mode != DEVICE_MODE_DETECTION) {
                state = STATE_CHANGE_MODE;
            }
            else if (is_lcd_ready) {
                state = STATE_DISPLAY_ERASE;
            }
            else {
                state = STATE_DISPLAY_ERASE_WAIT;
            }
            break;

        case STATE_TRAINING_ENTER:
            state = STATE_DISPLAY_SELECT_WAIT;
            break;

        case STATE_TRAINING_READY:
            if (device_mode != DEVICE_MODE_TRAINING) {
                state = STATE_CHANGE_MODE;
            }
            else if (training_room != last_room) {
                state = STATE_DISPLAY_SELECT_WAIT;
            }
            else if (is_training) {
                state = STATE_DISPLAY_TRAIN_WAIT;
            }
            else {
                state = STATE_TRAINING_READY;
            }
            break;

        case STATE_DISPLAY_SELECT:
            state = STATE_TRAINING_READY;
            break;

        case STATE_DISPLAY_SELECT_WAIT:
            if (device_mode != DEVICE_MODE_TRAINING) {
                state = STATE_CHANGE_MODE;
            }
            else if (is_lcd_ready) {
                state = STATE_DISPLAY_SELECT;
            }
            else {
                state = STATE_DISPLAY_SELECT_WAIT;
            }
            break;

        case STATE_DISPLAY_TRAIN_WAIT:
            if (device_mode != DEVICE_MODE_TRAINING) {
                state = STATE_CHANGE_MODE;
            }
            else if (is_lcd_ready) {
                state = STATE_DISPLAY_TRAIN;
            }
            else {
                state = STATE_DISPLAY_TRAIN_WAIT;
            }
            break;

        case STATE_DISPLAY_TRAIN:
        case STATE_TRAIN_WAIT:
            if (device_mode != DEVICE_MODE_TRAINING) {
                state = STATE_CHANGE_MODE;
            }
            else {
                state = STATE_TRAIN_WAIT;
            }
            break;

        default:
            state = STATE_INITIAL;
            break;
    }

    // Actions
    switch (state) {
        case STATE_DETECTION_ENTER:
        case STATE_TRAINING_ENTER:
            last_room = UINT8_MAX;
            break;

        case STATE_DISPLAY_NO_DETECT:
            last_room = UINT8_MAX;
            sprintf(lcd_buffer, "No detection");
            lcd_position = 0;
            should_lcd_clear = true;
            should_lcd_write = true;
            break;

        case STATE_DISPLAY_DETECT:
            last_room = detected_room;
            sprintf(lcd_buffer, "Detected room   %d", detected_room);
            lcd_position = 0;
            should_lcd_clear = true;
            should_lcd_write = true;
            break;

        case STATE_DISPLAY_SAVE:
            should_display_save = false;
            sprintf(lcd_buffer, "Saved model     to EEPROM");
            lcd_position = 0;
            should_lcd_clear = true;
            should_lcd_write = true;
            break;

        case STATE_DISPLAY_RESET:
            should_display_reset = false;
            sprintf(lcd_buffer, "Reset model     from EEPROM");
            lcd_position = 0;
            should_lcd_clear = true;
            should_lcd_write = true;
            break;

        case STATE_DISPLAY_ERASE:
            should_display_erase = false;
            sprintf(lcd_buffer, "Erased model    in SRAM");
            lcd_position = 0;
            should_lcd_clear = true;
            should_lcd_write = true;
            break;

        case STATE_DISPLAY_SELECT:
            last_room = training_room;
            sprintf(lcd_buffer, "Train room: %d", training_room);
            lcd_position = 0;
            should_lcd_clear = true;
            should_lcd_write = true;
            break;

        case STATE_DISPLAY_TRAIN:
            sprintf(lcd_buffer, "Training room   %d...", training_room);
            lcd_position = 0;
            should_lcd_clear = true;
            should_lcd_write = true;
            break;

        default:
            break;
    }

    return state;
}

task_t display_task = {
    STATE_INITIAL, // Initial state
    tick,          // Tick function
    20,            // Millisecond period
};
bool should_display_save;
bool should_display_reset;
bool should_display_erase;
