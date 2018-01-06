#ifndef GENERATE_BITBOARD_LOOKUP_TABLES
#define GENERATE_BITBOARD_LOOKUP_TABLES

#include <cstdint>

class BitBoardLookupTables
{
private:
    void GenerateEnPassantBitboardLookup(); 
public: 
    BitBoardLookupTables();
    uint64_t en_passant_bitboad_lookup_by_pawn_destination[64];
};

#endif