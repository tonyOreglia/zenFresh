#ifndef BB_UTIL_H
#define BB_UTIL_H

#include <cstdint>

unsigned lsb_scan(uint64_t bitboard);
unsigned msb_scan(uint64_t bitboard);
// uint8_t EjectIndexFromBitboard(uint64_t &bb);

#endif