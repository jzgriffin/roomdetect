// lcd_task.h
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#ifndef CUSTOM_COMMON_LCD_TASK_H
#define CUSTOM_COMMON_LCD_TASK_H

#include "lcd.h"
#include "task.h"
#include <stdbool.h>
#include <stdint.h>

/// \brief Size of the LCD write buffer excluding the terminator
#define LCD_BUFFER_SIZE (16 * 2)

/// \brief Task for the LCD driver
extern task_t lcd_task;

/// \brief Whether or not the LCD is ready for another operation
///
/// This variable is an output from the LCD task.
///
/// If this flag is set, the LCD is polling for an operation on its inputs.
extern bool is_lcd_ready;

/// \brief Whether or not the LCD should be cleared
///
/// This variable is an input to the LCD task.
///
/// If this flag is set, the LCD will be erased.  This flag takes priority
/// over other LCD operations.  After the operation is complete, this flag
/// is cleared.
extern bool should_lcd_clear;

/// \brief Whether or not the LCD should write from the buffer
///
/// This variable is an input to the LCD task.
///
/// If this flag is set, the LCD will write from the beginning of the buffer
/// to its terminator, starting on the display at the position in
/// \ref lcd_position.  After the operation is complete, this flag is cleared.
extern bool should_lcd_write;

/// \brief The buffer from which writes to the LCD are obtained
///
/// This variable is an input to the LCD task.
extern char lcd_buffer[LCD_BUFFER_SIZE + 1];

/// \brief The position on the LCD to write at from the buffer.
///
/// This variable is an input to the LCD task.
///
/// Position 0 is the origin at the top-left.  The position increases by 1 for
/// every character toward the right, up to 16.  At 16, the position is at the
/// left end of the second row, increasing toward the right.
extern uint8_t lcd_position;

#endif // CUSTOM_COMMON_LCD_TASK_H
