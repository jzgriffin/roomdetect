// main.c
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
// Assignment: Custom Project - Room Detector - Compute Application
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#include "adc.h"
#include "schedule.h"
#include "usart_tx_task.h"
#include <stddef.h>

enum {
    REPORT_STATE_INITIAL,
    REPORT_STATE_CONFIGURE,
    REPORT_STATE_START,
    REPORT_STATE_WAIT,
    REPORT_STATE_REPORT,
    REPORT_STATE_NEXT,
};

static task_state_t report_tick(task_state_t state)
{
    static adc_pin_t adc_pin;

    switch (state) {
        case REPORT_STATE_INITIAL:
            state = REPORT_STATE_CONFIGURE;
            break;

        case REPORT_STATE_CONFIGURE:
            state = REPORT_STATE_START;
            break;

        case REPORT_STATE_START:
            state = REPORT_STATE_WAIT;

        case REPORT_STATE_WAIT:
            if (is_adc_ready()) {
                state = REPORT_STATE_REPORT;
            }
            else {
                state = REPORT_STATE_WAIT;
            }
            break;

        case REPORT_STATE_REPORT:
            state = REPORT_STATE_NEXT;
            break;

        case REPORT_STATE_NEXT:
            state = REPORT_STATE_START;
            break;

        default:
            state = REPORT_STATE_INITIAL;
            break;
    }

    switch (state) {
        case REPORT_STATE_CONFIGURE:
            enable_adc();
            select_adc_pin(adc_pin = ADC_PIN_0);
            break;

        case REPORT_STATE_START:
            start_adc();
            break;

        case REPORT_STATE_REPORT: {
            uint16_t reading = read_adc();
            uint8_t data[3];
            data[0] = adc_pin;
            data[1] = reading >> 8;
            data[2] = reading >> 0;
            push_usart_tx_queue_packet(data, sizeof data / sizeof *data);
            break;
        }

        case REPORT_STATE_NEXT:
            switch (adc_pin) {
                case ADC_PIN_0: adc_pin = ADC_PIN_1; break;
                case ADC_PIN_1: adc_pin = ADC_PIN_2; break;
                case ADC_PIN_2: adc_pin = ADC_PIN_0; break;
                default: adc_pin = ADC_PIN_0; break;
            }
            select_adc_pin(adc_pin);
            break;

        default:
            break;
    }

    return state;
}

task_t report_task = {
    REPORT_STATE_INITIAL,
    report_tick,
    500,
};

int main()
{
    schedule_task(&usart_tx_task);
    schedule_task(&report_task);
    run_schedule();
}
