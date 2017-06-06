// adc.c
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#include "adc.h"
#include "bit.h"
#include <avr/io.h>

#define PIN_MASK 0x07

void enable_adc()
{
    ADCSRA =
        (1 << ADEN) |
        (0 << ADSC) |
        (0 << ADATE) |
        (0 << ADIF) |
        (0 << ADIE) |
        (0 << ADPS2) | (0 << ADPS1) | (0 << ADPS0);
}

void select_adc_pin(adc_pin_t pin)
{
    ADMUX &= ~PIN_MASK;
    ADMUX |= pin & PIN_MASK;
}

void start_adc()
{
    SET_BIT(ADCSRA, ADSC);
}

bool is_adc_ready()
{
    return !test_bit(ADCSRA, ADSC);
}

uint16_t read_adc()
{
    return ADC;
}

accum read_adc_voltage()
{
    return adc_to_volts(read_adc());
}

accum adc_to_volts(uint16_t r)
{
    return (accum)(r * ADC_VOLTAGE) / ADC_DIVISOR;
}
