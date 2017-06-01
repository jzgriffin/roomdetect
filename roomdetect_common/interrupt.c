// interrupt.c
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#include "interrupt.h"
#include <avr/interrupt.h>

static uint8_t interrupters;

uint8_t enable_interrupts()
{
    ++interrupters;
    if (interrupters == 1) {
        sei();
    }

    return interrupters;
}

uint8_t disable_interrupts()
{
    if (interrupters == 0) {
        return interrupters;
    }

    --interrupters;
    if (interrupters == 0) {
        cli();
    }

    return interrupters;
}
