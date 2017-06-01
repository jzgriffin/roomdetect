// lcd.c
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#include "lcd.h"
#include "bit.h"
#include "config.h"
#include <avr/cpufunc.h>
#include <avr/io.h>
#include <util/delay.h>

#define DATA_DDR     DDRC
#define BUS_DDR      (\
        (1 << DDRC7) | \
        (1 << DDRC6) | \
        (1 << DDRC5) | \
        (1 << DDRC4) | \
        (1 << DDRC3) | \
        (1 << DDRC2) | \
        (1 << DDRC1) | \
        (1 << DDRC0))
#define DATA_PIN     PINC
#define DATA_PORT    PORTC
#define CONTROL_DDR  DDRD
#define CONTROL_PORT PORTD
#define E_DDR        DDRD7
#define E_BIT        PORTD7
#define RW_DDR       DDRD6
#define RW_BIT       PORTD6
#define RS_DDR       DDRD5
#define RS_BIT       PORTD5

#define BF_BIT 7

static void enter_write_mode()
{
    CLEAR_BIT(CONTROL_PORT, RW_BIT);
    DATA_DDR |= BUS_DDR;
}

static void enter_read_mode()
{
    SET_BIT(CONTROL_PORT, RW_BIT);
    DATA_DDR &= ~BUS_DDR;
}

void map_lcd_ports()
{
    CONTROL_DDR |=
        (1 << E_DDR) |
        (1 << RW_DDR) |
        (1 << RS_DDR);
    CLEAR_BIT(CONTROL_PORT, E_BIT);
    CLEAR_BIT(CONTROL_PORT, RW_BIT);
    CLEAR_BIT(CONTROL_PORT, RS_BIT);
}

void enable_lcd()
{
    // The HD44780U is equipped with an internal reset circuit which performs
    // its own initialization sequence.  However, this sequence leaves the
    // display disabled and in 1-line mode, but the LCM-S01602DTR/M is
    // capable of 2-line mode.  Additionally, the reset is not triggered when
    // the ATMega is programmed with a new ROM.  By replicating the steps of
    // the HD44780U manually, with the requisite modifications, the reset
    // will occur every time the LCD is enabled in code.  The instructions to
    // initialize are as follows:
    //   1. Function set:
    //     DL = 1; 8-bit interface data
    //     N = 0; 1-line display
    //     F = 0; 5x8 dot character font
    //   2. Display on/off control:
    //     D = 0; Display off
    //     C = 0; Cursor off
    //     B = 0; Blinking off
    //   3. Entry mode set:
    //     I/D = 1; Increment by 1
    //     S = 0; No shift
    //   4. Display clear

    // Wait for the hardware to become available
    while (is_lcd_busy());

    // Step 1
    write_lcd_instruction(0x38); // Function set (00111000)
    _delay_us(37);

    // Step 2
    set_lcd_display_mode(LCD_DISPLAY_ENABLED, LCD_CURSOR_DISABLED,
            LCD_BLINK_DISABLED);

    // Step 3
    set_lcd_entry_mode(LCD_ENTRY_CURSOR_LEFT);

    // Step 4
    clear_lcd();
}

void disable_lcd()
{
    set_lcd_display_mode(LCD_DISPLAY_DISABLED, LCD_CURSOR_DISABLED,
            LCD_BLINK_DISABLED);
}

void write_lcd_instruction(uint8_t instruction)
{
    CLEAR_BIT(CONTROL_PORT, RS_BIT);
    enter_write_mode();
    DATA_PORT = instruction;

    SET_BIT(CONTROL_PORT, E_BIT);
    _NOP();
    CLEAR_BIT(CONTROL_PORT, E_BIT);
}

uint8_t read_lcd_instruction()
{
    CLEAR_BIT(CONTROL_PORT, RS_BIT);
    enter_read_mode();

    SET_BIT(CONTROL_PORT, E_BIT);
    _NOP();
    uint8_t instruction = DATA_PIN;
    CLEAR_BIT(CONTROL_PORT, E_BIT);

    return instruction;
}

void write_lcd_data(uint8_t data)
{
    SET_BIT(CONTROL_PORT, RS_BIT);
    enter_write_mode();
    DATA_PORT = data;

    SET_BIT(CONTROL_PORT, E_BIT);
    _NOP();
    CLEAR_BIT(CONTROL_PORT, E_BIT);
    _delay_us(37);
}

void clear_lcd()
{
    write_lcd_instruction(_BV(0));
    _delay_ms(2);
}

void return_lcd()
{
    write_lcd_instruction(_BV(1));
    _delay_ms(2);
}

void shift_lcd(lcd_shift_mode_t shift_mode)
{
    write_lcd_instruction(_BV(4) |
            ((shift_mode & 0x02) << 2));
    _delay_us(37);
}

void set_lcd_entry_mode(lcd_entry_mode_t entry_mode)
{
    write_lcd_instruction(_BV(2) |
            ((entry_mode & 0x03) << 0));
    _delay_us(37);
}

void set_lcd_display_mode(lcd_display_mode_t display_mode,
        lcd_cursor_mode_t cursor_mode, lcd_blink_mode_t blink_mode)
{
    write_lcd_instruction(_BV(3) |
            ((display_mode & 0x01) << 2) |
            ((cursor_mode & 0x01) << 1) |
            ((blink_mode & 0x01) << 0));
    _delay_us(37);
}

void set_lcd_cgram_addr(uint8_t addr)
{
    write_lcd_instruction(_BV(6) |
            ((addr & 0x3F) << 0));
    _delay_us(37);
}

void set_lcd_ddram_addr(uint8_t addr)
{
    write_lcd_instruction(_BV(7) |
            ((addr & 0x7F) << 0));
    _delay_us(37);
}

bool is_lcd_busy()
{
    return test_bit(read_lcd_instruction(), BF_BIT);
}

lcd_status_t read_lcd_status()
{
    uint8_t instruction = read_lcd_instruction();
    lcd_status_t status;
    status.is_busy = test_bit(instruction, BF_BIT);
    status.addr = instruction & 0x7F;
    return status;
}

void move_lcd_cursor(uint8_t pos) {
    if (pos < 16) {
        set_lcd_ddram_addr(pos);
    }
    else {
        set_lcd_ddram_addr(pos + 0x40 - 16);
    }
}

void write_lcd_str(uint8_t pos, const char *str)
{
    for (uint8_t p = pos; *str; ++p, ++str) {
        move_lcd_cursor(p);
        write_lcd_data(*str);
    }
}

void write_lcd_uint(uint8_t pos, uint32_t n)
{
    // Convert the integer to a string in reverse order
    char str[12] = {'\0'};
    uint8_t len = 0;
    do {
        str[len] = (n % 10) + '0';
        n /= 10;
        len++;
    } while (n != 0);

    // Reverse the string
    for (uint8_t i = 0; i < len / 2; ++i) {
        char t = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = t;
    }

    write_lcd_str(pos, str);
}

void write_lcd_int(uint8_t pos, int32_t n)
{
    if (n < 0) {
        write_lcd_str(pos, "-");
        write_lcd_uint(pos + 1, -n);
    }
    else {
        write_lcd_uint(pos, n);
    }
}
