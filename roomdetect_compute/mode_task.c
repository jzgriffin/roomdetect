// mode_task.c
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
// Assignment: Custom Project - Room Detector - Compute Application
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#include "mode_task.h"
#include "training_task.h"
#include "usart_packet_rx_task.h"

enum {
    STATE_INITIAL,
    STATE_CONFIGURE,
    STATE_READY,
    STATE_CHECK_PACKET,
    STATE_DISCARD_PACKET,
    STATE_RECEIVE_DETECTION,
    STATE_RECEIVE_TRAINING,
};

static task_state_t tick(task_state_t state)
{
    // Transitions
    switch (state) {
        case STATE_INITIAL:
            state = STATE_CONFIGURE;
            break;

        case STATE_CONFIGURE:
            state = STATE_READY;
            break;

        case STATE_READY:
            if (!is_usart_packet_rx_queue_empty()) {
                state = STATE_CHECK_PACKET;
            }
            else {
                state = STATE_READY;
            }
            break;

        case STATE_CHECK_PACKET: {
            const usart_packet_t *packet = peek_usart_packet_rx_queue();
            if (packet->size == 0) {
                state = STATE_DISCARD_PACKET;
            }
            else if (packet->data[0] == 0x00) {
                if (packet->size == 1) {
                    state = STATE_RECEIVE_DETECTION;
                }
                else {
                    state = STATE_DISCARD_PACKET;
                }
            }
            else if (packet->data[0] == 0x01) {
                if (packet->size == 2) {
                    state = STATE_RECEIVE_TRAINING;
                }
                else {
                    state = STATE_DISCARD_PACKET;
                }
            }
            else {
                state = STATE_READY;
            }
            break;
        }

        case STATE_DISCARD_PACKET:
        case STATE_RECEIVE_DETECTION:
        case STATE_RECEIVE_TRAINING:
            state = STATE_READY;
            break;

        default:
            state = STATE_INITIAL;
            break;
    }

    // Actions
    switch (state) {
        case STATE_CONFIGURE:
            device_mode = DEVICE_MODE_DETECTION;
            // TODO: Load model from EEPROM
            break;

        case STATE_DISCARD_PACKET:
            pop_usart_packet_rx_queue();
            break;

        case STATE_RECEIVE_DETECTION:
            pop_usart_packet_rx_queue();
            device_mode = DEVICE_MODE_DETECTION;
            break;

        case STATE_RECEIVE_TRAINING: {
            usart_packet_t packet = pop_usart_packet_rx_queue();
            training_room = packet.data[1];
            device_mode = DEVICE_MODE_TRAINING;
            break;
        }

        default:
            break;
    }

    return state;
}

task_t mode_task = {
    STATE_INITIAL, // Initial state
    tick,          // Tick function
    16,            // Millisecond period
};
device_mode_t device_mode;
model_t model;
EEMEM model_t model_eeprom;
