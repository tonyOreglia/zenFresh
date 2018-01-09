#include "bitboard-utilities.h"

/**
 * Return index of least significant bitboard bit.
 * That is starting from A8.
 */ 
unsigned lsb_scan(uint64_t bitboard){
    return __builtin_ctzll(bitboard);
}

/**
 * Return index of most significant bit.
 * Starting from H1.
 */ 
unsigned msb_scan(uint64_t bitboard){
    return 63 - __builtin_clzll(bitboard);
}

// uint8_t EjectIndexFromBitboard(uint64_t &bb) {
//     uint8_t index = lsb_scan(bb);
//     bb ^= position.bitboard_lookup.single_index_bitboard_[bishop_position];
// }