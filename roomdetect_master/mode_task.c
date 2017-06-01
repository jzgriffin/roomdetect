// mode_task.c
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
// Assignment: Custom Project - Room Detector - Master Application
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#include "mode_task.h"
#include "bit.h"
#include <avr/io.h>

enum {
    MODE_PIN = PIN0,
};

enum {
    STATE_INITIAL,
    STATE_CONFIGURE,
    STATE_DETECTION,
    STATE_TRAINING,
};

static task_state_t tick(task_state_t state)
{
    const bool mode_sw = test_bit(PINA, MODE_PIN);

    // Transitions
    switch (state) {
        case STATE_INITIAL:
            state = STATE_CONFIGURE;
            break;

        case STATE_CONFIGURE:
        case STATE_DETECTION:
        case STATE_TRAINING:
            switch (mode_sw) {
                case 0: state = STATE_DETECTION; break;
                case 1: state = STATE_TRAINING; break;
                default: state = STATE_INITIAL;
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
            CLEAR_BIT(DDRA, MODE_PIN);
            SET_BIT(PORTA, MODE_PIN);
            break;

        case STATE_DETECTION:
            device_mode = DEVICE_MODE_DETECTION;
            break;

        case STATE_TRAINING:
            device_mode = DEVICE_MODE_TRAINING;
            break;

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
