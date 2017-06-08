// detection_task.c
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
// Assignment: Custom Project - Room Detector - Master Application
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#include "detection_task.h"
#include "display_task.h"
#include "mode_task.h"
#include "bit.h"
#include "usart_packet_rx_task.h"
#include "usart_tx_task.h"
#include <avr/io.h>

#define ERASE_HOLD_TICKS (3000 / detection_task.period)

enum {
    SAVE_PIN = PIN2,
    RESET_PIN = PIN3,

    BUTTONS_MASK =
        _BV(SAVE_PIN) |
        _BV(RESET_PIN),
};

enum {
    STATE_INITIAL,
    STATE_CONFIGURE,
    STATE_DISABLED,
    STATE_STANDBY,
    STATE_ENABLED,
    STATE_READY,
    STATE_SAVE,
    STATE_SAVE_HOLD,
    STATE_RESET,
    STATE_RESET_HOLD,
    STATE_ERASE,
    STATE_ERASE_HOLD,
    STATE_CHECK_PACKET,
    STATE_DISCARD_PACKET,
    STATE_RECEIVE_DETECTION,
};

static bool send_save_packet()
{
    uint8_t data[] = {0x02};
    return push_usart_tx_queue_packet(data, sizeof data / sizeof *data);
}

static bool send_reset_packet()
{
    uint8_t data[] = {0x03};
    return push_usart_tx_queue_packet(data, sizeof data / sizeof *data);
}

static bool send_erase_packet()
{
    uint8_t data[] = {0x04};
    return push_usart_tx_queue_packet(data, sizeof data / sizeof *data);
}

static task_state_t tick(task_state_t state)
{
    static uint16_t tick_count;

    const bool save_bn = test_bit(PINA, SAVE_PIN);
    const bool reset_bn = test_bit(PINA, RESET_PIN);
    const uint8_t bn_down =
        save_bn +
        reset_bn;

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
            if (device_mode != DEVICE_MODE_DETECTION) {
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
            if (device_mode != DEVICE_MODE_DETECTION) {
                state = STATE_DISABLED;
            }
            else if (bn_down == 1) {
                if (save_bn) {
                    state = STATE_SAVE;
                }
                else if (reset_bn) {
                    state = STATE_RESET;
                }
                else {
                    state = STATE_READY;
                }
            }
            else if (!is_usart_packet_rx_queue_empty()) {
                state = STATE_CHECK_PACKET;
            }
            else {
                state = STATE_READY;
            }
            break;

        case STATE_SAVE:
        case STATE_SAVE_HOLD:
            if (device_mode != DEVICE_MODE_DETECTION) {
                state = STATE_DISABLED;
            }
            else if (save_bn) {
                state = STATE_SAVE_HOLD;
            }
            else {
                state = STATE_READY;
            }
            break;

        case STATE_RESET:
            if (device_mode != DEVICE_MODE_DETECTION) {
                state = STATE_DISABLED;
            }
            else if (reset_bn) {
                state = STATE_RESET_HOLD;
                tick_count = 0;
            }
            else {
                state = STATE_READY;
            }
            break;

        case STATE_RESET_HOLD:
            if (device_mode != DEVICE_MODE_DETECTION) {
                state = STATE_DISABLED;
            }
            else if (reset_bn) {
                if (tick_count == ERASE_HOLD_TICKS) {
                    state = STATE_ERASE;
                }
                else {
                    state = STATE_RESET_HOLD;
                }
            }
            else {
                state = STATE_READY;
            }
            break;

        case STATE_ERASE:
        case STATE_ERASE_HOLD:
            if (device_mode != DEVICE_MODE_DETECTION) {
                state = STATE_DISABLED;
            }
            else if (reset_bn) {
                state = STATE_ERASE_HOLD;
            }
            else {
                state = STATE_READY;
            }
            break;

        case STATE_CHECK_PACKET:
            if (device_mode != DEVICE_MODE_DETECTION) {
                state = STATE_DISABLED;
            }
            else {
                const usart_packet_t *packet = peek_usart_packet_rx_queue();
                if (packet->size >= 1 && packet->data[0] == 0x00) {
                    state = STATE_RECEIVE_DETECTION;
                }
                else {
                    state = STATE_DISCARD_PACKET;
                }
            }
            break;

        case STATE_DISCARD_PACKET:
            state = STATE_READY;
            break;

        case STATE_RECEIVE_DETECTION:
            state = STATE_READY;
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
            break;

        case STATE_ENABLED:
            detected_room = UINT8_MAX;
            break;

        case STATE_SAVE:
            send_save_packet();
            should_display_save = true;
            break;

        case STATE_RESET:
            send_reset_packet();
            should_display_reset = true;
            break;

        case STATE_RESET_HOLD:
            ++tick_count;
            break;

        case STATE_ERASE:
            send_erase_packet();
            should_display_erase = true;
            break;

        case STATE_DISCARD_PACKET:
            pop_usart_packet_rx_queue();
            break;

        case STATE_RECEIVE_DETECTION: {
            usart_packet_t packet = pop_usart_packet_rx_queue();
            detected_room = packet.data[1];
            break;
        }

        default:
            break;
    }

    return state;
}

task_t detection_task = {
    STATE_INITIAL, // Initial state
    tick,          // Tick function
    20,            // Millisecond period
};
uint8_t detected_room;
