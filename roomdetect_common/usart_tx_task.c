// usart_tx_task.c
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#include "usart_tx_task.h"
#include "usart_packet.h"

enum {
    STATE_INITIAL,
    STATE_CONFIGURE,
    STATE_READY,
    STATE_TRANSMIT,
};

static uint8_t pop_usart_tx_queue()
{
    if (is_usart_tx_queue_empty()) {
        return UINT8_MAX;
    }

    uint8_t data = usart_tx_queue[usart_tx_queue_head];
    --usart_tx_queue_size;
    ++usart_tx_queue_head;
    if (usart_tx_queue_head == USART_TX_QUEUE_SIZE) {
        usart_tx_queue_head = 0;
    }
    return data;
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
        case STATE_TRANSMIT:
            if (can_usart_tx() && !is_usart_tx_queue_empty()) {
                state = STATE_TRANSMIT;
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
            enable_usart_tx();
            break;

        case STATE_TRANSMIT:
            tx_usart_async(pop_usart_tx_queue());
            break;

        default:
            break;
    }

    return state;
}

bool is_usart_tx_queue_empty()
{
    return usart_tx_queue_size == 0;
}

bool is_usart_tx_queue_full()
{
    return usart_tx_queue_size == USART_TX_QUEUE_SIZE;
}

bool push_usart_tx_queue(uint8_t data)
{
    if (is_usart_tx_queue_full()) {
        return false;
    }

    usart_tx_queue[usart_tx_queue_tail] = data;
    ++usart_tx_queue_size;
    ++usart_tx_queue_tail;
    if (usart_tx_queue_tail == USART_TX_QUEUE_SIZE) {
        usart_tx_queue_tail = 0;
    }
    return true;
}

bool push_usart_tx_queue_array(const uint8_t *data, size_t size)
{
    if (USART_TX_QUEUE_SIZE - usart_tx_queue_size < size) {
        return false;
    }

    for (size_t i = 0; i < size; ++i) {
        push_usart_tx_queue(data[i]);
    }
    return true;
}

bool push_usart_tx_queue_packet(const uint8_t *data, size_t size)
{
    if (USART_TX_QUEUE_SIZE - usart_tx_queue_size <
            USART_PACKET_PACKED_SIZE(size)) {
        return false;
    }

    uint8_t packet[USART_PACKET_MAX_SIZE];
    size_t packet_size = pack_usart_packet(data, size, packet);
    return push_usart_tx_queue_array(packet, packet_size);
}

task_t usart_tx_task = {
    STATE_INITIAL, // Initial state
    tick,          // Tick function
    1,             // Millisecond period
};

uint8_t usart_tx_queue[USART_TX_QUEUE_SIZE];
size_t usart_tx_queue_size;
size_t usart_tx_queue_head;
size_t usart_tx_queue_tail;
