#ifndef BB_UTIL_H
#define BB_UTIL_H

#include <cstdint>

unsigned lsb_scan(uint64_t bitboard);
unsigned msb_scan(uint64_t bitboard);
short EjectIndexFromBitboard(uint64_t &bb, uint64_t *bb_lookup);

#endif