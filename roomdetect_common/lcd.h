// lcd.h
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#ifndef CUSTOM_COMMON_LCD_H
#define CUSTOM_COMMON_LCD_H

// The LCD module provides an interface for the LCM-S01602DTR/M 16x2
// character LCD component, which uses an HD44780U controller.  It assumes
// the following port map:
//
// Port      Direction  Purpose
// PC[7..0]  Varying    LCD data bus
// PD[7]     Output     LCD enable pin
// PD[6]     Output     LCD read/write pin
// PD[5]     Output     LCD register select pin
//
// This map may be configured using the map_lcd_ports routine.

#include <stdbool.h>
#include <stdint.h>

/// \brief Modes for the LCD shift instruction
typedef enum {
    /// \brief Shift the cursor position to the left
    LCD_SHIFT_CURSOR_LEFT   = (0 << 1) | (0 << 0),

    /// \brief Shift the cursor position to the right
    LCD_SHIFT_CURSOR_RIGHT  = (0 << 1) | (1 << 0),

    /// \brief Shift the entire display to the left
    ///
    /// The cursor follows the display shift.
    LCD_SHIFT_DISPLAY_LEFT  = (1 << 1) | (0 << 0),

    /// \brief Shift the entire display to the right
    ///
    /// The cursor follows the display shift.
    LCD_SHIFT_DISPLAY_RIGHT = (1 << 1) | (1 << 0),
} lcd_shift_mode_t;

/// \brief Modes for the LCD entry instructions
typedef enum {
    /// \brief Shift the cursor position to the left on entry
    LCD_ENTRY_CURSOR_LEFT   = (0 << 1) | (0 << 0),

    /// \brief Shift the cursor position to the right on entry
    LCD_ENTRY_CURSOR_RIGHT  = (0 << 1) | (1 << 0),

    /// \brief Shift the cursor position and entire display to the left on
    /// entry
    LCD_ENTRY_DISPLAY_LEFT  = (1 << 1) | (1 << 0),

    /// \brief Shift the cursor position and entire display to the right on
    /// entry
    LCD_ENTRY_DISPLAY_RIGHT = (1 << 1) | (0 << 0),
} lcd_entry_mode_t;

/// \brief Modes for the LCD unit
typedef enum {
    LCD_DISPLAY_DISABLED = 0, //!< Disable the display
    LCD_DISPLAY_ENABLED  = 1, //!< Enable the display
} lcd_display_mode_t;

/// \brief Modes for the LCD cursor
typedef enum {
    LCD_CURSOR_DISABLED = 0, //!< Disable the cursor
    LCD_CURSOR_ENABLED  = 1, //!< Enable the cursor
} lcd_cursor_mode_t;

/// \brief Modes for the LCD blinker
typedef enum {
    LCD_BLINK_DISABLED = 0, //!< Disable the blinker
    LCD_BLINK_ENABLED  = 1, //!< Enable the blinker
} lcd_blink_mode_t;

/// \brief Self-reported status from the LCD
typedef struct {
    bool is_busy; //!< Whether or not the LCD is busy
    uint8_t addr; //!< Current DDRAM/CGRAM address
} lcd_status_t;

/// \brief Configures the requisite port mappings for the LCD module
void map_lcd_ports();

/// \brief Enables the LCD and initializes it to an empty default state
void enable_lcd();

/// \brief Disables the LCD
void disable_lcd();

/// \brief Writes an instruction byte to the LCD
/// \param instruction byte to write
void write_lcd_instruction(uint8_t instruction);

/// \brief Reads an instruction byte from the LCD
/// \return byte read
uint8_t read_lcd_instruction();

/// \brief Writes a data byte to the LCD
/// \param data byte to write
void write_lcd_data(uint8_t data);

/// \brief Clears the LCD and resets the cursor to the origin
void clear_lcd();

/// \brief Resets the LCD cursor to the origin
void return_lcd();

/// \brief Performs a shift operation on the LCD
/// \param shift_mode mode of the shift
void shift_lcd(lcd_shift_mode_t shift_mode);

/// \brief Sets the LCD entry mode
/// \param entry_mode mode for entry
void set_lcd_entry_mode(lcd_entry_mode_t entry_mode);

/// \brief Sets the LCD display mode
/// \param display_mode mode for the display
/// \param cursor_mode mode for the cursor
/// \param blink_mode mode for the blinker
void set_lcd_display_mode(lcd_display_mode_t display_mode,
        lcd_cursor_mode_t cursor_mode, lcd_blink_mode_t blink_mode);

/// \brief Sets the LCD CGRAM address and enables reads/writes on CGRAM
/// \param addr address to seek to
void set_lcd_cgram_addr(uint8_t addr);

/// \brief Sets the LCD DDRAM address and enables reads/writes on DDRAM
/// \param addr address to seek to
void set_lcd_ddram_addr(uint8_t addr);

/// \brief Determines whether or not the LCD is busy
/// \return \c true if the LCD is busy with an operation; otherwise \c false
bool is_lcd_busy();

/// \brief Gets the self-reported status from the LCD
/// \return status from the LCD
lcd_status_t read_lcd_status();

/// \brief Moves the LCD cursor to the given character position, performing
/// line wrapping
/// \param pos absolute character position to move to
void move_lcd_cursor(uint8_t pos);

/// \brief Writes a string to the LCD at the given position
/// \param pos absolute character position to begin writing at
/// \param str string to write
void write_lcd_str(uint8_t pos, const char *str);

/// \brief Writes an unsigned integer to the LCD at the given position
/// \param pos absolute character position to begin writing at
/// \param n integer to write
void write_lcd_uint(uint8_t pos, uint32_t n);

/// \brief Writes a signed integer to the LCD at the given position
/// \param pos absolute character position to begin writing at
/// \param n integer to write
void write_lcd_int(uint8_t pos, int32_t n);

#endif // CUSTOM_COMMON_LCD_H
