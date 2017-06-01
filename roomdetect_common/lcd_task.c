// lcd_task.c
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#include "lcd_task.h"
#include <stddef.h>

enum {
    STATE_INITIAL,
    STATE_CONFIGURE,
    STATE_READY,
    STATE_CLEAR,
    STATE_MOVE,
    STATE_WRITE,
    STATE_NEXT,
};

bool is_lcd_ready;
bool should_lcd_clear;
bool should_lcd_write;
char lcd_buffer[LCD_BUFFER_SIZE + 1];
uint8_t lcd_position;

static task_state_t tick(task_state_t state)
{
    static size_t index;

    // Transitions
    switch (state) {
        case STATE_INITIAL:
            state = STATE_CONFIGURE;
            break;

        case STATE_CONFIGURE:
            state = STATE_READY;
            break;

        case STATE_READY:
            if (should_lcd_clear) {
                state = STATE_CLEAR;
            }
            else if (should_lcd_write) {
                if (lcd_buffer[0] == '\0') {
                    state = STATE_READY;
                }
                else {
                    state = STATE_MOVE;
                    index = 0;
                }
            }
            else {
                state = STATE_READY;
            }
            break;

        case STATE_CLEAR:
            state = STATE_READY;
            break;

        case STATE_MOVE:
            state = STATE_WRITE;
            break;

        case STATE_WRITE:
            state = STATE_NEXT;
            break;

        case STATE_NEXT:
            if (lcd_buffer[index] == '\0' || index >= LCD_BUFFER_SIZE) {
                state = STATE_READY;
            }
            else {
                state = STATE_MOVE;
            }
            break;

        default:
            state = STATE_INITIAL;
            break;
    }

    // Actions
    switch (state) {
        case STATE_CONFIGURE:
            map_lcd_ports();
            enable_lcd();
            break;

        case STATE_READY:
            is_lcd_ready = true;
            break;

        case STATE_CLEAR:
            is_lcd_ready = false;
            should_lcd_clear = false;
            clear_lcd();
            break;

        case STATE_MOVE:
            is_lcd_ready = false;
            move_lcd_cursor(lcd_position + index);
            break;

        case STATE_WRITE:
            should_lcd_write = false;
            write_lcd_data(lcd_buffer[index]);
            break;

        case STATE_NEXT:
            ++index;
            break;

        default:
            break;
    }

    return state;
}

task_t lcd_task = {
    STATE_INITIAL, // Initial state
    tick,          // Tick function
    2,             // Millisecond period
};
