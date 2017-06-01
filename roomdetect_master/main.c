// main.c
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
// Assignment: Custom Project - Room Detector - Master Application
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#include "lcd_task.h"
#include "schedule.h"
#include "usart_packet_rx_task.h"
#include "usart_rx_task.h"
#include <stddef.h>

#define MODE_PIN    PIN0
#define SAVE_PIN    PIN1
#define RESET_PIN   PIN2
#define CONFIRM_PIN PIN3
#define INC_PIN     PIN4
#define DEC_PIN     PIN5
#define BUSY_PIN    PIN6

enum {
    ECHO_STATE_INITIAL,
    ECHO_STATE_READY,
    ECHO_STATE_ECHO,
};

static char nibble_hex(uint8_t n)
{
    return n < 0x0A ? (n + '0') : (n - 0x0A + 'A');
}

static char high_nibble_hex(uint8_t n)
{
    return nibble_hex(n >> 4);
}

static char low_nibble_hex(uint8_t n)
{
    return nibble_hex(n & 0x0F);
}

static char *write_hex(uint8_t n, char *target)
{
    *target++ = high_nibble_hex(n);
    *target++ = low_nibble_hex(n);
    *target = '\0';
    return target;
}

static task_state_t echo_tick(task_state_t state)
{
    switch (state) {
        case ECHO_STATE_INITIAL:
            state = ECHO_STATE_READY;
            break;

        case ECHO_STATE_READY:
            if (is_lcd_ready && !is_usart_packet_rx_queue_empty()) {
                state = ECHO_STATE_ECHO;
            }
            else {
                state = ECHO_STATE_READY;
            }
            break;

        case ECHO_STATE_ECHO:
            state = ECHO_STATE_READY;
            break;

        default:
            state = ECHO_STATE_INITIAL;
            break;
    }

    switch (state) {
        case ECHO_STATE_ECHO: {
            usart_packet_t packet = pop_usart_packet_rx_queue();
            char *target = write_hex(packet.size, lcd_buffer);
            for (size_t i = 0; i < packet.size && i < 30; ++i) {
                target = write_hex(packet.data[i], target);
            }
            should_lcd_clear = true;
            should_lcd_write = true;
            break;
        }

        default:
            break;
    }

    return state;
}

task_t echo_task = {
    ECHO_STATE_INITIAL,
    echo_tick,
    16,
};

int main()
{
    schedule_task(&lcd_task);
    schedule_task(&usart_rx_task);
    schedule_task(&usart_packet_rx_task);
    schedule_task(&echo_task);
    run_schedule();
}
