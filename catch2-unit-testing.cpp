#define CATCH_CONFIG_MAIN  
#include "catch2.hpp"

#include "move.h"
#include "game.h"
#include "position.h"
#include "./bitboard_utilities/bitboard-utilities.h"
#include "./bitboard_utilities/generate-bitboard-lookup-tables.h"

TEST_CASE( "Starting position", "[position]" ) {
    Position position;
    REQUIRE( position.GetKingBitBoard(BLACK) == 1ULL << 4 );
    REQUIRE( position.GetQueenBitBoard(BLACK) == 1ULL << 3 );
    REQUIRE( position.GetKnightsBitBoard(BLACK) == (1ULL << 1 | 1ULL << 6) );
    REQUIRE( position.GetBishopsBitBoard(BLACK) == (1ULL << 2 | 1ULL << 5) );
    REQUIRE( position.GetRooksBitBoard(BLACK) == (1ULL | 1ULL << 7) );
    REQUIRE( position.GetPawnsBitBoard(BLACK) == 0xFF00 );

}
