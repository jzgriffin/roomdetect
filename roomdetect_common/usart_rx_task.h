// usart_rx_task.h
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#ifndef CUSTOM_COMMON_USART_RX_TASK_H
#define CUSTOM_COMMON_USART_RX_TASK_H

#include "task.h"
#include "usart.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/// \brief Maximum number of bytes in the RX queue
#define USART_RX_QUEUE_SIZE 1024

/// \brief Task for the USART receiver driver
extern task_t usart_rx_task;

/// \brief Queue of bytes received from USART
extern uint8_t usart_rx_queue[USART_RX_QUEUE_SIZE];

/// \brief Number of bytes in the RX queue
extern size_t usart_rx_queue_size;

/// \brief Index of the head byte in the RX queue
extern size_t usart_rx_queue_head;

/// \brief Index of the tail byte in the RX queue
extern size_t usart_rx_queue_tail;

/// \brief Determines whether the RX queue is empty
/// \return \c true if the RX queue is empty; \c false otherwise
bool is_usart_rx_queue_empty();

/// \brief Determines whether the RX queue is full
/// \return \c true if the RX queue is full; \c false otherwise
bool is_usart_rx_queue_full();

/// \brief Peeks the first byte in the USART receiver queue
/// \return first byte in the USART receiver queue
///
/// Peeking does not remove the byte from the queue.  If the queue is empty,
/// \p UINT8_MAX is returned.
uint8_t peek_usart_rx_queue();

/// \brief Pops the first byte in the USART receiver queue
/// \return first byte in the USART receiver queue
///
/// If the queue is empty, \p UINT8_MAX is returned.
uint8_t pop_usart_rx_queue();

/// \brief Pops the first bytes in the USART receiver queue into an array
/// \param data array of bytes to pop into
/// \param size number of bytes to pop
/// \return \c true if there were a sufficient number of bytes in the queue
/// and the pop succeeded; \c false otherwise
bool pop_usart_rx_queue_array(uint8_t *data, size_t size);

/// \brief Clears all packets from the USART receiver queue
void clear_usart_rx_queue();

#endif // CUSTOM_COMMON_USART_RX_TASK_H
