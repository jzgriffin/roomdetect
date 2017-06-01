// usart.c
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#include "usart.h"
#include "bit.h"
#include "config.h"
#include <avr/io.h>

static void configure()
{
    if (test_bit(UCSR0B, RXEN0) || test_bit(UCSR0B, TXEN0)) {
        return;
    }

    #include <util/setbaud.h>
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
    CHANGE_BIT(UCSR0A, U2X0, USE_2X);
    UCSR0B &= _BV(RXEN0) | _BV(TXEN0);
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
}

void enable_usart_rx()
{
    configure();
    flush_usart();
    SET_BIT(UCSR0B, RXEN0);
}

void enable_usart_tx()
{
    configure();
    tx_usart_async(0);
    SET_BIT(UCSR0B, TXEN0);
}

bool can_usart_tx()
{
    return test_bit(UCSR0A, UDRE0);
}

bool has_usart_rx()
{
    return test_bit(UCSR0A, RXC0);
}

bool has_usart_tx()
{
    return test_bit(UCSR0A, TXC0);
}

void flush_usart()
{
    __attribute__((unused)) static uint8_t dummy;
    while (has_usart_rx()) {
        dummy = UDR0;
    }
}

uint8_t rx_usart_sync()
{
    while (!has_usart_rx());
    return rx_usart_async();
}

uint8_t rx_usart_async()
{
    return UDR0;
}

void tx_usart_sync(uint8_t data)
{
    while (!can_usart_tx());
    tx_usart_async(data);
}

void tx_usart_async(uint8_t data)
{
    UDR0 = data;
}
