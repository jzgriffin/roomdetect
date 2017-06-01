// usart_packet_rx_task.h
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#ifndef CUSTOM_COMMON_USART_PACKET_RX_TASK_H
#define CUSTOM_COMMON_USART_PACKET_RX_TASK_H

#include "task.h"
#include "usart_packet.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/// \brief Maximum number of packets in the RX queue
#define USART_PACKET_RX_QUEUE_SIZE 4

/// \brief Task for the USART receiver driver
extern task_t usart_packet_rx_task;

/// \brief Queue of packets received from USART
extern usart_packet_t
    usart_packet_rx_queue[USART_PACKET_RX_QUEUE_SIZE];

/// \brief Number of packets in the RX queue
extern size_t usart_packet_rx_queue_size;

/// \brief Index of the head packet in the RX queue
extern size_t usart_packet_rx_queue_head;

/// \brief Index of the tail packet in the RX queue
extern size_t usart_packet_rx_queue_tail;

/// \brief Determines whether the RX queue is empty
/// \return \c true if the RX queue is empty; \c false otherwise
bool is_usart_packet_rx_queue_empty();

/// \brief Determines whether the RX queue is full
/// \return \c true if the RX queue is full; \c false otherwise
bool is_usart_packet_rx_queue_full();

/// \brief Peeks the first packet in the USART receiver queue
/// \return first packet in the USART receiver queue
///
/// Peeking does not remove the packet from the queue.  If the queue is empty,
/// NULL is returned.
const usart_packet_t *peek_usart_packet_rx_queue();

/// \brief Pops the first packet in the USART receiver queue
/// \return first packet in the USART receiver queue
///
/// If the queue is empty, an empty packet is returned.
usart_packet_t pop_usart_packet_rx_queue();

#endif // CUSTOM_COMMON_USART_PACKET_RX_TASK_H
