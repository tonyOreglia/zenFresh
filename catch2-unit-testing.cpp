#define CATCH_CONFIG_MAIN  
#include "catch2.hpp"

#include "move.h"
#include "game.h"
#include "position.h"
#include "./bitboard_utilities/bitboard-utilities.h"
#include "./bitboard_utilities/generate-bitboard-lookup-tables.h"

TEST_CASE( "King is on starting square", "[starting position]" ) {
    Position position;
    REQUIRE( position.GetKingBitBoard(BLACK) == 1ULL << 4 );
}
