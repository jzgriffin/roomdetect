// usart_packet.c
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#include "usart_packet.h"
#include <util/crc16.h>
#include <stddef.h>

// Initial value for CRC-CCITT
#define CRC_CCITT_INITIAL 0xFFFF

const uint8_t usart_packet_header[USART_PACKET_HEADER_SIZE] = {0xCA, 0xFE};

uint8_t pack_usart_packet(const uint8_t *data, uint8_t size,
        uint8_t *packet)
{
    uint8_t *target = packet;

    // Insert header
    for (size_t i = 0; i < USART_PACKET_HEADER_SIZE; ++i) {
        *target++ = usart_packet_header[i];
    }

    // Insert size
    *target++ = size;

    // Insert data
    uint16_t checksum = CRC_CCITT_INITIAL;
    for (size_t i = 0; i < size; ++i) {
        *target++ = data[i];
        checksum = _crc_ccitt_update(checksum, data[i]);
    }

    // Insert checksum in big endian format
    *target++ = (checksum & 0xFF00) >> 8;
    *target++ = (checksum & 0x00FF) >> 0;

    return target - packet;
}

uint8_t unpack_usart_packet(const uint8_t *packet, uint8_t size,
        uint8_t *data)
{
    if (size < USART_PACKET_MIN_SIZE) {
        return 0;
    }

    // Verify header
    const uint8_t *source = packet;
    for (size_t i = 0; i < USART_PACKET_HEADER_SIZE; ++i) {
        if (*source++ != usart_packet_header[i]) {
            return 0;
        }
    }

    // Extract size
    uint8_t source_size = *source++;
    if (USART_PACKET_UNPACKED_SIZE(size) < source_size) {
        return 0;
    }

    // Extract data
    uint16_t checksum = CRC_CCITT_INITIAL;
    for (size_t i = 0; i < source_size; ++i) {
        data[i] = *source++;
        checksum = _crc_ccitt_update(checksum, data[i]);
    }

    // Verify checksum
    uint16_t source_checksum = 0;
    source_checksum |= (*source++) << 8;
    source_checksum |= (*source++) << 0;
    if (source_checksum != checksum) {
        return 0;
    }

    return source_size;
}
