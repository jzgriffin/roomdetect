// reading_task.c
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
// Assignment: Custom Project - Room Detector - Compute Application
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#include "reading_task.h"
#include "adc.h"
#include <string.h>

enum {
    STATE_INITIAL,
    STATE_CONFIGURE,
    STATE_STANDBY,
    STATE_START,
    STATE_WAIT,
    STATE_READ,
    STATE_OUTPUT,
};

static task_state_t tick(task_state_t state)
{
    static const adc_pin_t pins[MODEL_VECTOR_SIZE] = {
        ADC_PIN_0, ADC_PIN_1, ADC_PIN_2,
    };
    static accum readings[MODEL_VECTOR_SIZE];
    static size_t index;

    // Transitions
    switch (state) {
        case STATE_INITIAL:
            state = STATE_CONFIGURE;
            break;

        case STATE_CONFIGURE:
            state = STATE_STANDBY;
            break;

        case STATE_STANDBY:
            if (should_poll_readings) {
                state = STATE_START;
            }
            else {
                state = STATE_STANDBY;
            }
            break;

        case STATE_START:
            state = STATE_WAIT;
            break;

        case STATE_WAIT:
            if (is_adc_ready()) {
                state = STATE_READ;
            }
            else {
                state = STATE_WAIT;
            }
            break;

        case STATE_READ:
            if (index == MODEL_VECTOR_SIZE) {
                state = STATE_OUTPUT;
            }
            else {
                state = STATE_START;
            }
            break;

        case STATE_OUTPUT:
            state = STATE_STANDBY;
            break;

        default:
            state = STATE_INITIAL;
            break;
    }

    // Actions
    switch (state) {
        case STATE_CONFIGURE:
            enable_adc();
            index = 0;
            break;

        case STATE_START:
            select_adc_pin(pins[index]);
            start_adc();
            are_readings_ready = false;
            should_poll_readings = false;
            break;

        case STATE_READ:
            readings[index] = read_adc_voltage();
            ++index;
            break;

        case STATE_OUTPUT:
            memcpy(reading_vector, readings, sizeof reading_vector);
            are_readings_ready = true;
            index = 0;
            break;

        default:
            break;
    }

    return state;
}

task_t reading_task = {
    STATE_INITIAL, // Initial state
    tick,          // Tick function
    8,             // Millisecond period
};
accum reading_vector[MODEL_VECTOR_SIZE];
bool are_readings_ready;
bool should_poll_readings;
