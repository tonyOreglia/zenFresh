#define CATCH_CONFIG_MAIN  
#include "catch2.hpp"

#include "move.h"
#include "game.h"
#include "position.h"
#include "./bitboard_utilities/bitboard-utilities.h"
#include "./bitboard_utilities/generate-bitboard-lookup-tables.h"

TEST_CASE( "Validate starting position", "[position]" ) {
    Position position;
    SECTION("Blacks starting position should match expectation") {
        REQUIRE( position.GetKingBitBoard(BLACK) == 1ULL << 4 );
        REQUIRE( position.GetQueenBitBoard(BLACK) == 1ULL << 3 );
        REQUIRE( position.GetKnightsBitBoard(BLACK) == (1ULL << 1 | 1ULL << 6) );
        REQUIRE( position.GetBishopsBitBoard(BLACK) == (1ULL << 2 | 1ULL << 5) );
        REQUIRE( position.GetRooksBitBoard(BLACK) == (1ULL | 1ULL << 7) );
        REQUIRE( position.GetPawnsBitBoard(BLACK) == 0xFF00 );
    }
    
    SECTION("Whites starting position should match expectation") {
        REQUIRE( position.GetKingBitBoard(WHITE) == 1ULL << 60 );
        REQUIRE( position.GetQueenBitBoard(WHITE) == 1ULL << 59 );
        REQUIRE( position.GetKnightsBitBoard(WHITE) == (1ULL << 57 | 1ULL << 62) );
        REQUIRE( position.GetBishopsBitBoard(WHITE) == (1ULL << 58 | 1ULL << 61) );
        REQUIRE( position.GetRooksBitBoard(WHITE) == (1ULL << 56 | 1ULL << 63) );
        REQUIRE( position.GetPawnsBitBoard(WHITE) == 0xFF000000000000 );
    }
}

TEST_CASE("Basic Rook Movement", "[movement]") {
    Position position(
        (char *)"k7/8/8/3R4/8/8/8/K7",
        (char *)"w",
        (char *)"----",
        (char *)"-",
        (char *)"1",
        (char *)"1"
    );
    SECTION("Number of moves is 17") {
        Game game(position);
        vector <Move> moves_list = game.GetMovesVector(0);
        game.GenerateMoves(moves_list);
        REQUIRE( moves_list.size() == 17 );
    }
}
/**
 * Things I can test
 * 
 * Movement
 * * For each piece
 * * * rook
 * * * queen
 * * * king
 * * * bishop
 * * * knight
 * * * pawn
 * * Attacks
 * * castling
 * * pawn movement
 * * * attacking pawns
 * * * en pasant
 * * * double pawn push
 * 
 * Position
 * * Starting position (X)
 * * illegal positions
 * * castling rights
 * * move counts
 * 
 * Move Generation
 * * perft
 * 
 * 
 **/