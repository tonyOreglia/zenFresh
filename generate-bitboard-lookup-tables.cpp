#include "generate-bitboard-lookup-tables.h"


BitBoardLookupTables::BitBoardLookupTables() {
    GenerateEnPassantBitboardLookup();
}

void BitBoardLookupTables::GenerateEnPassantBitboardLookup() {
    for (char i=0; i<64; i++) {
        if (i > 23 && i < 32) {
            en_passant_bitboad_lookup_by_pawn_destination[i] = 1ULL << (i - 8);
        }
        else if (i > 31 && i < 40) {
            en_passant_bitboad_lookup_by_pawn_destination[i] = 1ULL << (i + 8);
        }
    }
}
