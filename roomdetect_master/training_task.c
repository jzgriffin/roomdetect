// training_task.c
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
// Assignment: Custom Project - Room Detector - Master Application
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#include "training_task.h"
#include "mode_task.h"
#include "bit.h"
#include "lcd_task.h"
#include "usart_tx_task.h"
#include <avr/io.h>
#include <stdio.h>
#include <string.h>

#define BUTTON_REPEAT_TICKS (500 / training_task.period)
#define MAX_TRAINING_ROOM 0x0F

enum {
    CONFIRM_PIN = PIN3,
    INC_PIN = PIN4,
    DEC_PIN = PIN5,

    BUTTONS_MASK =
        _BV(CONFIRM_PIN) |
        _BV(INC_PIN) |
        _BV(DEC_PIN),
};

enum {
    STATE_INITIAL,
    STATE_CONFIGURE,
    STATE_DISABLED,
    STATE_STANDBY,
    STATE_ENABLED,
    STATE_READY,
    STATE_INC,
    STATE_INC_HOLD,
    STATE_DEC,
    STATE_DEC_HOLD,
    STATE_CONFIRM_WAIT,
    STATE_CONFIRM,
    STATE_TRAINING,
};

static bool send_start_training_packet(uint8_t room)
{
    uint8_t data[] = {0x01, room};
    return push_usart_tx_queue_packet(data, sizeof data / sizeof *data);
}

static bool send_stop_training_packet()
{
    uint8_t data[] = {0x00};
    return push_usart_tx_queue_packet(data, sizeof data / sizeof *data);
}

static task_state_t tick(task_state_t state)
{
    static uint16_t tick_count;

    const bool confirm_bn = test_bit(PINA, CONFIRM_PIN);
    const bool inc_bn = test_bit(PINA, INC_PIN);
    const bool dec_bn = test_bit(PINA, DEC_PIN);
    const uint8_t bn_down =
        confirm_bn +
        inc_bn +
        dec_bn;

    // Transitions
    switch (state) {
        case STATE_INITIAL:
            state = STATE_CONFIGURE;
            break;

        case STATE_CONFIGURE:
            state = STATE_STANDBY;
            break;

        case STATE_DISABLED:
            state = STATE_STANDBY;
            break;

        case STATE_STANDBY:
            if (device_mode != DEVICE_MODE_TRAINING) {
                state = STATE_STANDBY;
            }
            else {
                state = STATE_ENABLED;
            }
            break;

        case STATE_ENABLED:
            state = STATE_READY;
            break;

        case STATE_READY:
            if (device_mode != DEVICE_MODE_TRAINING) {
                state = STATE_DISABLED;
            }
            else if (bn_down == 1) {
                if (confirm_bn) {
                    state = STATE_CONFIRM;
                }
                else if (inc_bn) {
                    state = STATE_INC;
                }
                else if (dec_bn) {
                    state = STATE_DEC;
                }
                else {
                    state = STATE_READY;
                }
            }
            else {
                state = STATE_READY;
            }
            break;

        case STATE_INC:
            if (device_mode != DEVICE_MODE_TRAINING) {
                state = STATE_DISABLED;
            }
            else if (inc_bn) {
                state = STATE_INC_HOLD;
                tick_count = 0;
            }
            else {
                state = STATE_READY;
            }
            break;

        case STATE_INC_HOLD:
            if (device_mode != DEVICE_MODE_TRAINING) {
                state = STATE_DISABLED;
            }
            else if (inc_bn) {
                if (tick_count == BUTTON_REPEAT_TICKS) {
                    state = STATE_INC;
                }
                else {
                    state = STATE_INC_HOLD;
                }
            }
            else {
                state = STATE_READY;
            }
            break;

        case STATE_DEC:
            if (device_mode != DEVICE_MODE_TRAINING) {
                state = STATE_DISABLED;
            }
            else if (dec_bn) {
                state = STATE_DEC_HOLD;
                tick_count = 0;
            }
            else {
                state = STATE_READY;
            }
            break;

        case STATE_DEC_HOLD:
            if (device_mode != DEVICE_MODE_TRAINING) {
                state = STATE_DISABLED;
            }
            else if (dec_bn) {
                if (tick_count == BUTTON_REPEAT_TICKS) {
                    state = STATE_DEC;
                }
                else {
                    state = STATE_DEC_HOLD;
                }
            }
            else {
                state = STATE_READY;
            }
            break;

        case STATE_CONFIRM_WAIT:
            if (is_usart_tx_queue_full()) {
                state = STATE_CONFIRM_WAIT;
            }
            else {
                state = STATE_CONFIRM;
            }
            break;

        case STATE_CONFIRM:
            state = STATE_TRAINING;
            break;

        case STATE_TRAINING:
            if (device_mode != DEVICE_MODE_TRAINING) {
                state = STATE_DISABLED;
            }
            else {
                state = STATE_TRAINING;
            }
            break;

        default:
            state = STATE_INITIAL;
            break;
    }

    // Actions
    switch (state) {
        case STATE_CONFIGURE:
            // Map the ports
            DDRA &= ~BUTTONS_MASK;
            PORTA |= BUTTONS_MASK;

            training_room = 0;
            is_training = false;
            break;

        case STATE_DISABLED:
            if (is_training) {
                send_stop_training_packet();
            }
            is_training = false;
            break;

        case STATE_ENABLED:
            training_room = 0;
            is_training = false;

            sprintf(lcd_buffer, "Train room: %d", training_room);
            lcd_position = 0;
            should_lcd_clear = true;
            should_lcd_write = true;
            break;

        case STATE_INC:
            if (training_room < MAX_TRAINING_ROOM) {
                ++training_room;
                sprintf(lcd_buffer, "Train room: %d", training_room);
                lcd_position = 0;
                should_lcd_clear = true;
                should_lcd_write = true;
            }
            break;

        case STATE_DEC:
            if (training_room > 0) {
                --training_room;
                sprintf(lcd_buffer, "Train room: %d", training_room);
                lcd_position = 0;
                should_lcd_clear = true;
                should_lcd_write = true;
            }
            break;

        case STATE_INC_HOLD:
        case STATE_DEC_HOLD:
            ++tick_count;
            break;

        case STATE_CONFIRM:
            send_start_training_packet(training_room);
            is_training = true;
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

task_t training_task = {
    STATE_INITIAL, // Initial state
    tick,          // Tick function
    50,            // Millisecond period
};
uint8_t training_room;
bool is_training;
