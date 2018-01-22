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

short EjectIndexFromBitboard(uint64_t &bb, uint64_t* bb_lookup) {
    short index = lsb_scan(bb);
    bb ^= bb_lookup[index];
    return index;
}