// usart.h
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#ifndef CUSTOM_COMMON_USART_H
#define CUSTOM_COMMON_USART_H

#include <stdbool.h>
#include <stdint.h>

/// \brief Enables transmission via USART port 0
void enable_usart_rx();

/// \brief Enables reception via USART port 0
void enable_usart_tx();

/// \brief Determines whether USART is ready to transmit data
/// \return \c true if USART is ready to transmit; \c false otherwise
bool can_usart_tx();

/// \brief Determines whether data has been received via USART
/// \return \c true if USART received data; \c false otherwise
bool has_usart_rx();

/// \brief Determines whether data has been transmitted via USART
/// \return \c true if USART transmitted data; \c false otherwise
bool has_usart_tx();

/// \brief Flushes the USART receiver buffer
void flush_usart();

/// \brief Receives a byte from USART synchronously
/// \return byte received
///
/// Blocks until \ref has_usart_recv is \c true.
uint8_t rx_usart_sync();

/// \brief Receives a byte from USART asynchronously
/// \return byte received
///
/// Does not verify that USART has received data.
uint8_t rx_usart_async();

/// \brief Transmits a byte over USART synchronously
/// \param data byte to transmit
///
/// Blocks until \ref is_usart_ready is \c true.
void tx_usart_sync(uint8_t data);

/// \brief Transmits a byte over USART asynchronously
/// \param data byte to transmit
///
/// Does not verify that USART is ready to transmit.
void tx_usart_async(uint8_t data);

#endif // CUSTOM_COMMON_USART_H
