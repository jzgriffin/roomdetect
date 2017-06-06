// training_task.c
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
// Assignment: Custom Project - Room Detector - Compute Application
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#include "training_task.h"
#include "mode_task.h"
#include "reading_task.h"

enum {
    STATE_INITIAL,
    STATE_CONFIGURE,
    STATE_DISABLED,
    STATE_STANDBY,
    STATE_ENABLED,
    STATE_READY,
    STATE_TRAIN,
};

static task_state_t tick(task_state_t state)
{
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
            if (are_readings_ready) {
                state = STATE_TRAIN;
            }
            else {
                state = STATE_READY;
            }
            break;

        case STATE_TRAIN:
            state = STATE_READY;
            break;

        default:
            state = STATE_INITIAL;
            break;
    }

    // Actions
    switch (state) {
        case STATE_CONFIGURE:
            training_room = 0;
            break;

        case STATE_READY:
            should_poll_readings = true;
            break;

        case STATE_TRAIN:
            update_room(&model.rooms[training_room], reading_vector);
            break;

        default:
            break;
    }

    return state;
}

task_t training_task = {
    STATE_INITIAL, // Initial state
    tick,          // Tick function
    16,            // Millisecond period
};
uint8_t training_room;
