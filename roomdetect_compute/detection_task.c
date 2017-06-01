// detection_task.c
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
// Assignment: Custom Project - Room Detector - Compute Application
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#include "detection_task.h"
#include "mode_task.h"
#include "usart_packet_rx_task.h"
#include "usart_tx_task.h"

enum {
    STATE_INITIAL,
    STATE_CONFIGURE,
    STATE_DISABLED,
    STATE_STANDBY,
    STATE_ENABLED,
    STATE_READY,
    STATE_CHECK_PACKET,
    STATE_DISCARD_PACKET,
    STATE_RECEIVE_SAVE,
    STATE_RECEIVE_RESET,
    STATE_RECEIVE_ERASE,
};

static bool send_detection_packet(uint8_t room)
{
    uint8_t data[] = {0x00, room};
    return push_usart_tx_queue_packet(data, sizeof data / sizeof *data);
}

static task_state_t tick(task_state_t state)
{
    static uint16_t tick_count;

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
            else if (!is_usart_packet_rx_queue_empty()) {
                state = STATE_CHECK_PACKET;
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
                if (packet->size == 0) {
                    state = STATE_DISCARD_PACKET;
                }
                else if (packet->data[0] == 0x02) {
                    if (packet->size == 1) {
                        state = STATE_RECEIVE_SAVE;
                    }
                    else {
                        state = STATE_DISCARD_PACKET;
                    }
                }
                else if (packet->data[0] == 0x03) {
                    if (packet->size == 1) {
                        state = STATE_RECEIVE_RESET;
                    }
                    else {
                        state = STATE_DISCARD_PACKET;
                    }
                }
                else if (packet->data[0] == 0x04) {
                    if (packet->size == 1) {
                        state = STATE_RECEIVE_ERASE;
                    }
                    else {
                        state = STATE_DISCARD_PACKET;
                    }
                }
                else {
                    state = STATE_READY;
                }
            }
            break;

        case STATE_DISCARD_PACKET:
        case STATE_RECEIVE_SAVE:
        case STATE_RECEIVE_RESET:
        case STATE_RECEIVE_ERASE:
            state = STATE_READY;
            break;

        default:
            state = STATE_INITIAL;
            break;
    }

    // Actions
    switch (state) {
        case STATE_CONFIGURE:
            detected_room = 0;
            break;

        case STATE_DISABLED:
            // TODO: Stop detection
            break;

        case STATE_ENABLED:
            // TODO: Begin detection
            break;

        case STATE_READY:
            ++tick_count;
            if (tick_count == 100) {
                send_detection_packet(detected_room++);
                tick_count = 0;
            }
            break;

        case STATE_DISCARD_PACKET:
            pop_usart_packet_rx_queue();
            break;

        case STATE_RECEIVE_SAVE:
            pop_usart_packet_rx_queue();
            // TODO: Save to EEPROM
            break;

        case STATE_RECEIVE_RESET:
            pop_usart_packet_rx_queue();
            // TODO: Reset from EEPROM
            break;

        case STATE_RECEIVE_ERASE:
            pop_usart_packet_rx_queue();
            // TODO: Erase in EEPROM
            break;

        default:
            break;
    }

    return state;
}

task_t detection_task = {
    STATE_INITIAL, // Initial state
    tick,          // Tick function
    16,            // Millisecond period
};
uint8_t detected_room;
