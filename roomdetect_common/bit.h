// bit.h
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#ifndef CUSTOM_COMMON_BIT_H
#define CUSTOM_COMMON_BIT_H

#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>

/// \brief Sets bit \p i of \p n
/// \param n name of the value to modify
/// \param i index of the bit to set
#define SET_BIT(n, i) n |= _BV(i)

/// \brief Clears bit \p i of \p n
/// \param n name of the value to modify
/// \param i index of the bit to clear
#define CLEAR_BIT(n, i) n &= ~_BV(i)

/// \brief Toggles bit \p i of \p n
/// \param n name of the value to modify
/// \param i index of the bit to toggle
#define TOGGLE_BIT(n, i) n ^= _BV(i)

/// \brief Changes bit \p i of \p n according to \p b
/// \param n name of the value to modify
/// \param i index of the bit to change
#define CHANGE_BIT(n, i, b) \
    if (b) { SET_BIT(n, i); } \
    else { CLEAR_BIT(n, i); }; \
    while (0)

/// \brief Tests bit \p i of \p n
/// \param n value to test
/// \param i index of the bit to test
/// \return \c true if the bit is set; \c false otherwise
inline __attribute__((always_inline))
bool test_bit(uint8_t n, uint8_t i)
{
    return (n & _BV(i)) ? true : false;
}

/// \brief Sets bit \p i of \p n
/// \param n value to modify
/// \param i index of the bit to set
/// \return \p n with bit \p i set to \c 1
inline __attribute__((always_inline))
uint8_t set_bit(uint8_t n, uint8_t i)
{
    return n | _BV(i);
}

/// \brief Clears bit \p i of \p n
/// \param n value to modify
/// \param i index of the bit to clear
/// \return \p n with bit \p i set to \c 0
inline __attribute__((always_inline))
uint8_t clear_bit(uint8_t n, uint8_t i)
{
    return n & ~_BV(i);
}

/// \brief Toggles bit \p i of \p n
/// \param n value to modify
/// \param i index of the bit to toggle
/// \return \p n with bit \p i set to \c 0 if it was initially \c 1 or \c 1 if
/// if it was initially \c 0
inline __attribute__((always_inline))
uint8_t toggle_bit(uint8_t n, uint8_t i)
{
    return n ^ _BV(i);
}

/// \brief Changes bit \p i of \p n according to \p b
/// \param n value to modify
/// \param i index of the bit to change
/// \return \p n with bit \p i set to \c 1 if \p b is \c true or \c 0 if \p b
/// is \c false
inline __attribute__((always_inline))
uint8_t change_bit(uint8_t n, uint8_t i, bool b)
{
    return b ? set_bit(n, i) : clear_bit(n, i);
}

/// \}

#endif // CUSTOM_COMMON_BIT_H
