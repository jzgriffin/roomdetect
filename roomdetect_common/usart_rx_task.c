// usart_rx_task.c
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#include "usart_rx_task.h"

enum {
    STATE_INITIAL,
    STATE_CONFIGURE,
    STATE_READY,
    STATE_RECEIVE,
};

static bool push_usart_rx_queue(uint8_t data)
{
    if (is_usart_rx_queue_full()) {
        return false;
    }

    usart_rx_queue[usart_rx_queue_tail] = data;
    ++usart_rx_queue_size;
    ++usart_rx_queue_tail;
    if (usart_rx_queue_tail == USART_RX_QUEUE_SIZE) {
        usart_rx_queue_tail = 0;
    }
    return true;
}

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
        case STATE_RECEIVE:
            if (has_usart_rx()) {
                state = STATE_RECEIVE;
            }
            else {
                state = STATE_READY;
            }
            break;

        default:
            state = STATE_INITIAL;
            break;
    }

    // Actions
    switch (state) {
        case STATE_CONFIGURE:
            enable_usart_rx();
            break;

        case STATE_RECEIVE:
            push_usart_rx_queue(rx_usart_async());
            break;

        default:
            break;
    }

    return state;
}

bool is_usart_rx_queue_empty()
{
    return usart_rx_queue_size == 0;
}

bool is_usart_rx_queue_full()
{
    return usart_rx_queue_size == USART_RX_QUEUE_SIZE;
}

uint8_t peek_usart_rx_queue()
{
    if (is_usart_rx_queue_empty()) {
        return UINT8_MAX;
    }

    return usart_rx_queue[usart_rx_queue_head];
}

uint8_t pop_usart_rx_queue()
{
    if (is_usart_rx_queue_empty()) {
        return UINT8_MAX;
    }

    uint8_t data = usart_rx_queue[usart_rx_queue_head];
    --usart_rx_queue_size;
    ++usart_rx_queue_head;
    if (usart_rx_queue_head == USART_RX_QUEUE_SIZE) {
        usart_rx_queue_head = 0;
    }
    return data;
}

bool pop_usart_rx_queue_array(uint8_t *data, size_t size)
{
    if (usart_rx_queue_size < size) {
        return false;
    }

    for (size_t i = 0; i < size; ++i) {
        data[i] = pop_usart_rx_queue();
    }
    return true;
}

task_t usart_rx_task = {
    STATE_INITIAL, // Initial state
    tick,          // Tick function
    1,             // Millisecond period
};

uint8_t usart_rx_queue[USART_RX_QUEUE_SIZE];
size_t usart_rx_queue_size;
size_t usart_rx_queue_head;
size_t usart_rx_queue_tail;
