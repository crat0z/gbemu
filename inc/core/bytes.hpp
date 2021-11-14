#ifndef BYTES_HPP
#define BYTES_HPP

#include <cstdint>

static uint16_t swap_byte_order(uint16_t s) { return (s >> 8) | (s << 8); }

#endif