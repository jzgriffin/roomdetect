// config.h
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#ifndef CUSTOM_COMMON_CONFIG_H
#define CUSTOM_COMMON_CONFIG_H

// Clock speed of the CPU in Hertz
// Used in clock calculations and in the AVR delay functions
#ifndef F_CPU
#define F_CPU 8000000UL // By default, the ATMega 1284 runs at 8 MHz
#endif // F_CPU

// Baud rate for USART
#ifndef BAUD
#define BAUD 4800UL
#endif // BAUD

#endif // CUSTOM_COMMON_CONFIG_H
