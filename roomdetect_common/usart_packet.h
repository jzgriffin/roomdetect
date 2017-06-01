// usart_packet.h
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#ifndef CUSTOM_COMMON_USART_PACKET_H
#define CUSTOM_COMMON_USART_PACKET_H

// This interface defines the structure of packets used in communicating
// between devices through USART as well as functions for packing and
// unpacking data into and from packets.
//
// Packets take the following form:
//   header size data checksum
// where header is a constant number used in searching for packet beginnings,
// size is the size of the data field, data is the packed data, and checksum
// is the CRC-CCITT checksum of the data.

#include <stdint.h>

/// \brief Size of the header field for USART packets in bytes
#define USART_PACKET_HEADER_SIZE 2

/// \brief Size of the size field for USART packets in bytes
#define USART_PACKET_SIZE_SIZE 1 // Stored in uint8_t

/// \brief Maximum size of the data field for USART packets in bytes
#define USART_PACKET_DATA_MAX_SIZE UINT8_MAX

/// \brief Size of the checksum field for USART packets in bytes
#define USART_PACKET_CHECKSUM_SIZE 2

/// \brief Size of the metadata within USART packets in bytes
#define USART_PACKET_META_SIZE \
    (USART_PACKET_HEADER_SIZE + USART_PACKET_SIZE_SIZE + \
     USART_PACKET_CHECKSUM_SIZE)

/// \brief Calculates the size of a USART packet in bytes
/// \param n size of the data field
/// \return total size of the packet including metadata
#define USART_PACKET_PACKED_SIZE(n) (n + USART_PACKET_META_SIZE)

/// \brief Calculates the size of the data within a USART packet in bytes
/// \param n size of the packet
/// \return expected size of the data field
#define USART_PACKET_UNPACKED_SIZE(n) (n - USART_PACKET_META_SIZE)

/// \brief Size of the smallest possible USART packet in bytes
#define USART_PACKET_MIN_SIZE USART_PACKET_PACKED_SIZE(0)

/// \brief Size of the largest possible USART packet in bytes
#define USART_PACKET_MAX_SIZE \
    USART_PACKET_PACKED_SIZE(USART_PACKET_DATA_MAX_SIZE)

/// \brief Represents the data within a packet
///
/// This structure does not include the surrounding metadata aside from the
/// size of the data.
typedef struct {
    uint8_t size; //!< Size of the data in bytes
    uint8_t data[USART_PACKET_DATA_MAX_SIZE]; //!< Packed data
} usart_packet_t;

/// \brief Signature bytes attached at the head of packets
extern const uint8_t usart_packet_header[USART_PACKET_HEADER_SIZE];

/// \brief Packs data into a packet structure
/// \param data data to pack
/// \param size size of \p data in bytes
/// \param packet buffer to contain the packet bytes
/// \return size of the packet in bytes
///
/// \p packet must be at least \c USART_PACKET_SIZE(size) bytes in size to
/// prevent buffer overflow.
uint8_t pack_usart_packet(const uint8_t *data, uint8_t size,
        uint8_t *packet);

/// \brief Unpacks data from a packet structure
/// \param packet packet to unpack
/// \param size size of \p packet in bytes
/// \param data buffer to contain the the data bytes
/// \return size of the data in bytes
///
/// \p data must be at least \c USART_PACKET_UNPACKED_SIZE(size) bytes in size
/// to prevent buffer overflow.  If the packet is not well-formed (does not
/// contain the appropriate header, size does not match, or checksum fails),
/// \c 0 is returned.  The contents of \p data are not guaranteed after such a
/// failure.
uint8_t unpack_usart_packet(const uint8_t *packet, uint8_t size,
        uint8_t *data);

#endif // CUSTOM_COMMON_USART_PACKET_H
