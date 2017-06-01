// numeric.h
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#ifndef CUSTOM_COMMON_NUMERIC_H
#define CUSTOM_COMMON_NUMERIC_H

#include <stdint.h>

/// \brief Calculates the GCD of two integers
/// \param a first integer
/// \param b second integer
/// \return greatest common divisor of \p a and \p b
///
/// Implemented using the iterative version of Euclid's algorithm.
uint16_t gcd(uint16_t a, uint16_t b);

#endif // CUSTOM_COMMON_NUMERIC_H
