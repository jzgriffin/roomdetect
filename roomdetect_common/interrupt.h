// interrupt.h
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#ifndef CUSTOM_COMMON_INTERRUPT_H
#define CUSTOM_COMMON_INTERRUPT_H

#include <stdint.h>

/// \brief Adds an active interrupter, enabling interrupts if necessary
/// \return number of active interrupters
uint8_t enable_interrupts();

/// \brief Removes an active interrupter, disabling interrupts if they are no
/// longer needed
/// \return number of active interrupters
uint8_t disable_interrupts();

#endif // CUSTOM_COMMON_INTERRUPT_H
