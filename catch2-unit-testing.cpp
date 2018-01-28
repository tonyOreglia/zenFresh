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

    SECTION("20 valid moves from starting position") {
        Game game(position);
        vector <Move> moves_list = game.GetMovesVector(0);
        game.GenerateMoves(moves_list);
        REQUIRE( moves_list.size() == 20 );
    }
}

TEST_CASE("Basic Rook Movement", "[movement]") {
    Position position(
        (char *)"k7/8/3r4/3R4/3P4/8/8/K7",
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
        REQUIRE( moves_list.size() == 11 );
    }
}

TEST_CASE("Basic Bishop Movement", "[movement]") {
    Position position(
        (char *)"k7/8/8/3B4/4P3/8/8/K7",
        (char *)"w",
        (char *)"----",
        (char *)"-",
        (char *)"1",
        (char *)"1"
    );
    SECTION("Number of moves is 16") {
        Game game(position);
        vector <Move> moves_list = game.GetMovesVector(0);
        game.GenerateMoves(moves_list);
        REQUIRE( moves_list.size() == 13 );
    }
}

TEST_CASE("Basic Queen Movement", "[movement]") {
    Position position(
        (char *)"k7/8/3r4/3Q4/3P4/8/8/K7",
        (char *)"w",
        (char *)"----",
        (char *)"-",
        (char *)"1",
        (char *)"1"
    );
    SECTION("Number of moves is 27") {
        Game game(position);
        vector <Move> moves_list = game.GetMovesVector(0);
        game.GenerateMoves(moves_list);
        REQUIRE( moves_list.size() == 24 );
    }
}

TEST_CASE("Basic King Movement", "[movement]") {
    Position position(
        (char *)"8/8/3k4/8/3r4/3K4/3P4/8",
        (char *)"w",
        (char *)"----",
        (char *)"-",
        (char *)"1",
        (char *)"1"
    );
    SECTION("Number of moves is 8") {
        Game game(position);
        vector <Move> moves_list = game.GetMovesVector(0);
        game.GenerateMoves(moves_list);
        REQUIRE( moves_list.size() == 7 );
    }

    Position position2(
        (char *)"4k3/8/8/8/8/8/8/4K3",
        (char *)"w",
        (char *)"KQkq",
        (char *)"-",
        (char *)"1",
        (char *)"1"
    );
    SECTION("Castling Moves for white") {
        Game game(position2);
        vector <Move> moves_list = game.GetMovesVector(0);
        game.GenerateMoves(moves_list);
        REQUIRE( moves_list.size() == 7 );
    }
    Position position3(
        (char *)"4k3/8/8/8/8/8/8/4K3",
        (char *)"b",
        (char *)"KQkq",
        (char *)"-",
        (char *)"1",
        (char *)"1"
    );
    SECTION("Castling Moves for black") {
        Game game(position3);
        vector <Move> moves_list = game.GetMovesVector(0);
        game.GenerateMoves(moves_list);
        REQUIRE( moves_list.size() == 7 );
    }
}

TEST_CASE("Basic Knight Movement", "[movement]") {
    Position position(
        (char *)"k7/8/1r6/3N4/1P6/8/8/K7",
        (char *)"w",
        (char *)"----",
        (char *)"-",
        (char *)"1",
        (char *)"1"
    );
    SECTION("Number of moves is 11") {
        Game game(position);
        vector <Move> moves_list = game.GetMovesVector(0);
        game.GenerateMoves(moves_list);
        REQUIRE( moves_list.size() == 11 );
    }
}

TEST_CASE("Basic Pawn Movement", "[movement]") {
    Position position(
        (char *)"k7/8/8/1P6/8/3r1R2/P1PPPPPP/7K",
        (char *)"w",
        (char *)"----",
        (char *)"a6",
        (char *)"1",
        (char *)"1"
    );
    SECTION("Number of moves is 23") {
        Game game(position);
        vector <Move> moves_list = game.GetMovesVector(0);
        game.GenerateMoves(moves_list);
        REQUIRE( moves_list.size() == 24 );
    }
    SECTION("En passant square is 16") {
        REQUIRE(lsb_scan(position.GetEnPassanteBitBoard()) == 16);
    }
}
/**
 * Things I can test
 * 
 * Movement
 * * For each piece
 * * * rook (X)
 * * * queen (X)
 * * * king (X)
 * * * bishop (X)
 * * * knight (X)
 * * * pawn (X)
 * * Attacks (X)
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