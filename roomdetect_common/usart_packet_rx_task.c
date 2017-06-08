// usart_packet_rx_task.c
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#include "usart_packet_rx_task.h"
#include "usart_rx_task.h"

enum {
    STATE_INITIAL,
    STATE_RESET,
    STATE_READY,
    STATE_WAIT_HEADER,
    STATE_RECEIVE_HEADER,
    STATE_WAIT_SIZE,
    STATE_RECEIVE_SIZE,
    STATE_WAIT_DATA,
    STATE_RECEIVE_DATA,
    STATE_WAIT_CHECKSUM,
    STATE_RECEIVE_CHECKSUM,
    STATE_RECEIVE_PACKET,
};

static bool emplace_usart_packet_rx_queue(const uint8_t *packet, size_t size)
{
    if (is_usart_packet_rx_queue_full()) {
        return false;
    }

    usart_packet_t *tail = &usart_packet_rx_queue[usart_packet_rx_queue_tail];
    tail->size = unpack_usart_packet(packet, size, tail->data);
    if (tail->size != packet[USART_PACKET_HEADER_SIZE]) {
        return false;
    }

    ++usart_packet_rx_queue_size;
    ++usart_packet_rx_queue_tail;
    if (usart_packet_rx_queue_tail == USART_PACKET_RX_QUEUE_SIZE) {
        usart_packet_rx_queue_tail = 0;
    }
    return true;
}

static task_state_t tick(task_state_t state)
{
    static uint8_t packet[USART_PACKET_MAX_SIZE];
    static size_t size;
    static size_t target_size;
    static size_t data_size;

    // Transitions
    switch (state) {
        case STATE_INITIAL:
            state = STATE_RESET;
            break;

        case STATE_RESET:
            state = STATE_READY;
            break;

        case STATE_READY:
            if (is_usart_packet_rx_queue_full()) {
                state = STATE_READY;
            }
            else if (peek_usart_rx_queue() == usart_packet_header[0]) {
                state = STATE_RECEIVE_HEADER;
            }
            else {
                pop_usart_rx_queue();
                state = STATE_READY;
            }
            break;

        case STATE_WAIT_HEADER:
            if (size == USART_PACKET_HEADER_SIZE) {
                state = STATE_WAIT_SIZE;
            }
            else if (is_usart_rx_queue_empty()) {
                state = STATE_WAIT_HEADER;
            }
            else if (peek_usart_rx_queue() == usart_packet_header[size]) {
                state = STATE_RECEIVE_HEADER;
            }
            else {
                state = STATE_RESET;
            }
            break;

        case STATE_RECEIVE_HEADER:
            state = STATE_WAIT_HEADER;
            break;

        case STATE_WAIT_SIZE:
            if (is_usart_rx_queue_empty()) {
                state = STATE_WAIT_SIZE;
            }
            else {
                state = STATE_RECEIVE_SIZE;
            }
            break;

        case STATE_RECEIVE_SIZE:
            state = STATE_WAIT_DATA;
            break;

        case STATE_WAIT_DATA:
            if (data_size == target_size) {
                state = STATE_WAIT_CHECKSUM;
            }
            else if (is_usart_rx_queue_empty()) {
                state = STATE_WAIT_DATA;
            }
            else {
                state = STATE_RECEIVE_DATA;
            }
            break;

        case STATE_RECEIVE_DATA:
            state = STATE_WAIT_DATA;
            break;

        case STATE_WAIT_CHECKSUM:
            if (size - data_size == USART_PACKET_META_SIZE) {
                state = STATE_RECEIVE_PACKET;
            }
            else if (is_usart_rx_queue_empty()) {
                state = STATE_WAIT_CHECKSUM;
            }
            else {
                state = STATE_RECEIVE_CHECKSUM;
            }
            break;

        case STATE_RECEIVE_CHECKSUM:
            state = STATE_WAIT_CHECKSUM;
            break;

        case STATE_RECEIVE_PACKET:
            state = STATE_RESET;
            break;

        default:
            state = STATE_INITIAL;
            break;
    }

    // Actions
    switch (state) {
        case STATE_RESET:
            size = 0;
            data_size = 0;
            break;

        case STATE_RECEIVE_HEADER:
        case STATE_RECEIVE_CHECKSUM:
            packet[size++] = pop_usart_rx_queue();
            break;

        case STATE_RECEIVE_SIZE:
            target_size = packet[size++] = pop_usart_rx_queue();
            break;

        case STATE_RECEIVE_DATA:
            packet[size++] = pop_usart_rx_queue();
            ++data_size;
            break;

        case STATE_RECEIVE_PACKET:
            emplace_usart_packet_rx_queue(packet, size);
            break;

        default:
            break;
    }

    return state;
}

bool is_usart_packet_rx_queue_empty()
{
    return usart_packet_rx_queue_size == 0;
}

bool is_usart_packet_rx_queue_full()
{
    return usart_packet_rx_queue_size == USART_PACKET_RX_QUEUE_SIZE;
}

const usart_packet_t *peek_usart_packet_rx_queue()
{
    if (is_usart_packet_rx_queue_empty()) {
        return NULL;
    }

    return &usart_packet_rx_queue[usart_packet_rx_queue_head];
}

usart_packet_t pop_usart_packet_rx_queue()
{
    if (is_usart_packet_rx_queue_empty()) {
        usart_packet_t empty_packet = {0, {0x00}};
        return empty_packet;
    }

    usart_packet_t data = usart_packet_rx_queue[usart_packet_rx_queue_head];
    --usart_packet_rx_queue_size;
    ++usart_packet_rx_queue_head;
    if (usart_packet_rx_queue_head == USART_PACKET_RX_QUEUE_SIZE) {
        usart_packet_rx_queue_head = 0;
    }
    return data;
}

void clear_usart_packet_rx_queue()
{
    usart_packet_rx_queue_size = 0;
    usart_packet_rx_queue_head = 0;
    usart_packet_rx_queue_tail = 0;
}

task_t usart_packet_rx_task = {
    STATE_INITIAL, // Initial state
    tick,          // Tick function
    1,             // Millisecond period
};

usart_packet_t usart_packet_rx_queue[USART_PACKET_RX_QUEUE_SIZE];
size_t usart_packet_rx_queue_size;
size_t usart_packet_rx_queue_head;
size_t usart_packet_rx_queue_tail;
