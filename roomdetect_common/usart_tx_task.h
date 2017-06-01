// usart_tx_task.h
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#ifndef CUSTOM_COMMON_USART_TX_TASK_H
#define CUSTOM_COMMON_USART_TX_TASK_H

#include "task.h"
#include "usart.h"
#include <stddef.h>
#include <stdint.h>

/// \brief Maximum number of bytes in the TX queue
#define USART_TX_QUEUE_SIZE 1024

/// \brief Task for the USART transmitter driver
extern task_t usart_tx_task;

/// \brief Queue of bytes to transmit via USART
extern uint8_t usart_tx_queue[USART_TX_QUEUE_SIZE];

/// \brief Number of bytes in the TX queue
extern size_t usart_tx_queue_size;

/// \brief Index of the head byte in the TX queue
extern size_t usart_tx_queue_head;

/// \brief Index of the tail byte in the TX queue
extern size_t usart_tx_queue_tail;

/// \brief Determines whether the TX queue is empty
/// \return \c true if the TX queue is empty; \c false otherwise
bool is_usart_tx_queue_empty();

/// \brief Determines whether the TX queue is full
/// \return \c true if the TX queue is full; \c false otherwise
bool is_usart_tx_queue_full();

/// \brief Pushes a byte into the USART transmitter queue
/// \param data byte to transmit last
/// \return \c true if the queue was not full and the push succeeded; \c false
/// otherwise
bool push_usart_tx_queue(uint8_t data);

/// \brief Pushes an array of bytes into the USART transmitter queue
/// \param data array of bytes to transmit last
/// \param size number of bytes to push
/// \return \c true if the queue was not full and the push succeeded; \c false
/// otherwise
///
/// If the number of bytes exceeds the available space in the queue, the push
/// is not attempted.
bool push_usart_tx_queue_array(const uint8_t *data, size_t size);

/// \brief Pushes an array of bytes into the USART transmitter queue as a
/// packet
/// \param data array of bytes to transmit last as a packet
/// \param size number of bytes in the packet data
/// \return \c true if the queue was not full and the push succeeded; \c false
/// otherwise
/// \see pack_usart_packet
///
/// If the number of bytes in the packet exceeds the available space in the
/// queue, the push is not attempted.
bool push_usart_tx_queue_packet(const uint8_t *data, size_t size);

#endif // CUSTOM_COMMON_USART_TX_TASK_H
