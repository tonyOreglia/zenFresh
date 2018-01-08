#ifndef GENERATE_BITBOARD_LOOKUP_TABLES
#define GENERATE_BITBOARD_LOOKUP_TABLES

#include <cstdint>
#include <bitset>
#include <iostream>

using namespace std;

class BitBoardLookupTables
{
private:
    void GenerateEnPassantBitboardLookup(); 
    void GenerateArrayBitboardLookup();
    void GenerateSingleBitLookup();
    void GenerateSliderPieceBitboardLookup();
public: 
    BitBoardLookupTables();
    void PrintAllBitboards();
    void PrintBitBoard(uint64_t bitboard);
    uint64_t single_index_bitboard_[64]; // look up single bit bitboard by index
    uint64_t en_passant_bitboad_lookup_by_pawn_destination[64];
    uint64_t attacked_pawn_location_for_en_passant_capture[64];
    uint64_t north_array_bitboard_lookup[64];
    uint64_t south_array_bitboard_lookup[64];
    uint64_t east_array_bitboard_lookup[64];
    uint64_t west_array_bitboard_lookup[64];
    uint64_t north_east_array_bitboard_lookup[64];
    uint64_t north_west_array_bitboard_lookup[64];
    uint64_t south_east_array_bitboard_lookup[64];
    uint64_t south_west_array_bitboard_lookup[64];
    uint64_t knight_attack_bitboard_lookup[64];
    uint64_t rook_moves_bitboard_lookup[64];
    uint64_t bishop_moves_bitboard_lookup[64];
    uint64_t queen_moves_bitboard_lookup[64];\
    uint64_t a_file;
    uint64_t b_file;
    uint64_t c_file;
    uint64_t d_file;
    uint64_t e_file;
    uint64_t f_file;
    uint64_t g_file;
    uint64_t h_file;
};

#endif