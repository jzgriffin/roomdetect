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

#define SAMPLE_SIZE 16

enum {
    STATE_INITIAL,
    STATE_CONFIGURE,
    STATE_STANDBY,
    STATE_POLL,
    STATE_START,
    STATE_WAIT,
    STATE_READ,
    STATE_NEXT,
    STATE_OUTPUT,
};

static task_state_t tick(task_state_t state)
{
    static const adc_pin_t pins[MODEL_VECTOR_SIZE] = {
        ADC_PIN_0, ADC_PIN_1, ADC_PIN_2,
    };
    static accum readings[MODEL_VECTOR_SIZE];
    static size_t index;
    static size_t samples;

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
                state = STATE_POLL;
            }
            else {
                state = STATE_STANDBY;
            }
            break;
            
        case STATE_POLL:
            state = STATE_START;
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
                state = STATE_NEXT;
            }
            else {
                state = STATE_START;
            }
            break;
            
        case STATE_NEXT:
            if (samples == SAMPLE_SIZE) {
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
            break;
            
        case STATE_POLL:
            index = 0;
            samples = 0;
            for (size_t i = 0; i < MODEL_VECTOR_SIZE; ++i) {
                readings[i] = 0K;
            }
            break;

        case STATE_START:
            select_adc_pin(pins[index]);
            start_adc();
            are_readings_ready = false;
            should_poll_readings = false;
            break;

        case STATE_READ:
            readings[index] += read_adc_voltage();
            ++index;
            break;
            
        case STATE_NEXT:
            index = 0;
            ++samples;
            break;

        case STATE_OUTPUT:
            for (size_t i = 0; i < MODEL_VECTOR_SIZE; ++i) {
                reading_vector[i] = readings[i] / (accum)SAMPLE_SIZE;
            }
            are_readings_ready = true;
            break;

        default:
            break;
    }

    return state;
}

task_t reading_task = {
    STATE_INITIAL, // Initial state
    tick,          // Tick function
    1,             // Millisecond period
};
accum reading_vector[MODEL_VECTOR_SIZE];
bool are_readings_ready;
bool should_poll_readings;
