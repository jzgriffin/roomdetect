// adc.h
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#ifndef CUSTOM_COMMON_ADC_H
#define CUSTOM_COMMON_ADC_H

#include <stdbool.h>
#include <stdfix.h>
#include <stdint.h>

/// \brief Divisor for the maximum reading produced by the ADC
#define ADC_DIVISOR 0x03FF

/// \brief Reference voltage used in the ADC circuit
#define ADC_VOLTAGE 5

/// \brief Pins that can be used with ADC
typedef enum {
    ADC_PIN_0, //!< ADC on PA0
    ADC_PIN_1, //!< ADC on PA1
    ADC_PIN_2, //!< ADC on PA2
    ADC_PIN_3, //!< ADC on PA3
    ADC_PIN_4, //!< ADC on PA4
    ADC_PIN_5, //!< ADC on PA5
    ADC_PIN_6, //!< ADC on PA6
    ADC_PIN_7, //!< ADC on PA7
} adc_pin_t;

/// \brief Enables auto-triggering analog-digital conversion on ADC0
void enable_adc();

/// \brief Selects the pin to perform ADC on
/// \param pin pin to perform ADC on
void select_adc_pin(adc_pin_t pin);

/// \brief Starts an ADC conversion
void start_adc();

/// \brief Determines whether the ADC conversion is complete
/// \return \c true if the conversion is complete and data is available
bool is_adc_ready();

/// \brief Gets the last ADC conversion reading
/// \return 10-bit ADC reading
uint16_t read_adc();

/// \brief Gets the last ADC conversion reading in volts
/// \return voltage from ADC reading
/// \see adc_to_volts
accum read_adc_voltage();

/// \brief Converts the given ADC conversion reading to volts
/// \param r 10-bit ADC reading
/// \return \p r in volts
/// \see ADC_BITS
/// \see ADC_VOLTAGE
accum adc_to_volts(uint16_t r);

#endif // CUSTOM_COMMON_ADC_H
