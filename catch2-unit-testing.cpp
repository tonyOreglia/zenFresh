#define CATCH_CONFIG_MAIN  
#include "catch2.hpp"

#include "move.h"
#include "game.h"
#include "position.h"
#include "./bitboard_utilities/bitboard-utilities.h"
#include "./bitboard_utilities/generate-bitboard-lookup-tables.h"

TEST_CASE( "Validate starting position", "[position]" ) {
    calculateAllLookupBbs();
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
    SECTION("Position class accessor functions") {

    }
}

TEST_CASE("Move class") {
    SECTION("constructors") {
        Move move;
        REQUIRE(move.GetMove() == 0);
        Move move2(0, 63);
        REQUIRE(move2.GetMove() == 1008);
        Move move3(63, 0);
        REQUIRE(move3.GetMove() == 64512);
        Move move4(62,63);
        REQUIRE(move4.GetMove() == 64496);
        Move move5(move4.GetMove());
        REQUIRE(move4.GetMove() == 64496);
    }
    SECTION("set and get origin square functions") {
        Move move;
        move.SetOriginSquare(1);
        REQUIRE(move.GetOriginSquare() == 1);
    }
    SECTION("set and get destination square functions") {
        Move move;
        move.SetDestinationSquare(63);
        REQUIRE(move.GetDestinationSquare() == 63);
    }
    SECTION("set and get double pawn push flag functions") {
        Move move;
        move.SetDoublePawnPushFlag();
        REQUIRE(!!move.IsDoublePawnPush() == true);
        Move move2;
        REQUIRE(!!move2.IsDoublePawnPush() == false);
    }
    SECTION("set and get capture flag") {
        Move move;
        move.SetCaptureFlag();
        REQUIRE(!!move.IsCapture() == true);
        Move move2;
        REQUIRE(!!move2.IsCapture() == false);
    }
    SECTION("set and get promotion flag") {
        Move move;
        move.SetPromotionFlag();
        REQUIRE(!!move.IsPromotion() == true);
        Move move2;
        REQUIRE(!!move2.IsPromotion() == false);
    }
    SECTION("castling flags") {
        Move move;
        REQUIRE(move.IsKingSideCastle() == false);
        REQUIRE(move.IsQueenSideCastle() == false);
        move.SetKingSideCastleFlag();
        REQUIRE(move.IsKingSideCastle() == true);
        REQUIRE(move.IsQueenSideCastle() == false);
        move.SetQueenSideCastleFlag();
        REQUIRE(move.IsQueenSideCastle() == true);
        
    }
    SECTION("en passant capture flag") {
        Move move;
        REQUIRE(move.IsEnPassantCapture() == false);
        move.SetEnPassantCaptureFlag();
        REQUIRE(move.IsEnPassantCapture() == true);
    }
    SECTION("promote pawn to queen") {
        Move move;
        move.SetPromotionPieceToQueen();
        REQUIRE(move.PromotePawnToQueen() == true);
        REQUIRE(move.PromotePawnToRook() == false);
        REQUIRE(move.PromotePawnToKnight() == false);
        REQUIRE(move.PromotePawnToBishop() == false);
    }
    SECTION("promote pawn to rook") {
        Move move;
        move.SetPromotionPieceToRook();
        REQUIRE(move.PromotePawnToRook() == true);
        REQUIRE(move.PromotePawnToQueen() == false);
        REQUIRE(move.PromotePawnToKnight() == false);
        REQUIRE(move.PromotePawnToBishop() == false);
    }
    SECTION("promote pawn to knight") {
        Move move;
        move.SetPromotionPieceToKnight();
        REQUIRE(move.PromotePawnToKnight() == true);
        REQUIRE(move.PromotePawnToRook() == false);
        REQUIRE(move.PromotePawnToQueen() == false);
        REQUIRE(move.PromotePawnToBishop() == false);
    }
    SECTION("promote pawn to bishop") {
        Move move;
        move.SetPromotionPieceToBishop();
        REQUIRE(move.PromotePawnToBishop() == true);
        REQUIRE(move.PromotePawnToRook() == false);
        REQUIRE(move.PromotePawnToQueen() == false);
        REQUIRE(move.PromotePawnToKnight() == false);
    }
}

TEST_CASE("Basic Rook Movement", "[movement]") {
    calculateAllLookupBbs();
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
    SECTION("side to move is set correctly") {
        Position pos;
        REQUIRE(pos.GetSideToMove() == true);
        Move move(55, 47);
        pos.MakeMove(move);
        REQUIRE(pos.GetSideToMove() == false);
    }
}

TEST_CASE("Basic Bishop Movement", "[movement]") {
    calculateAllLookupBbs();
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
    calculateAllLookupBbs();
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
    calculateAllLookupBbs();
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
    /**
     * ****k***
     * ********
     * ********
     * ********
     * ********
     * ********
     * ********
     * ****K***
     */ 
    SECTION("Castling Moves for black") {
        Game game(position3);
        vector <Move> moves_list = game.GetMovesVector(0);
        game.GenerateMoves(moves_list);
        REQUIRE( moves_list.size() == 7 );
    }
    SECTION("White cannot castle kingside into check") {
        Position position(
            (char *)"4k3/8/8/8/8/8/6r1/4K3",
            (char *)"w",
            (char *)"KQkq",
            (char *)"-",
            (char *)"1",
            (char *)"1"
        );
      /**
       * ****k***
       * ********
       * ********
       * ********
       * ********
       * ********
       * ********
       * ****K***
       */ 
        Game game(position);
        vector <Move> moves_list = game.GetMovesVector(0);
        game.GenerateMoves(moves_list);
        REQUIRE( moves_list.size() == 6 );
    }
    SECTION("White cannot castle kingside through check") {
        Position position(
            (char *)"4k3/8/8/8/8/8/5r2/4K3",
            (char *)"w",
            (char *)"KQkq",
            (char *)"-",
            (char *)"1",
            (char *)"1"
        );
        Game game(position);
        vector <Move> moves_list = game.GetMovesVector(0);
        game.GenerateMoves(moves_list);
        REQUIRE( moves_list.size() == 6 );
    }
    SECTION("White cannot castle if flags do not allow it") {
        Position position(
            (char *)"4k3/8/8/8/8/8/5r2/4K3",
            (char *)"w",
            (char *)"----",
            (char *)"-",
            (char *)"1",
            (char *)"1"
        );
        Game game(position);
        vector <Move> moves_list = game.GetMovesVector(0);
        game.GenerateMoves(moves_list);
        REQUIRE( moves_list.size() == 5 );
    }
}

TEST_CASE("Basic Knight Movement", "[movement]") {
    calculateAllLookupBbs();
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
    calculateAllLookupBbs();
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
    SECTION("Promotion") {
        Position position(
            (char *)"8/7P/8/8/8/8/8/k6K",
            (char *)"w",
            (char *)"----",
            (char *)"-",
            (char *)"1",
            (char *)"1"
        );
        Game game(position);
        vector <Move> moves_list = game.GetMovesVector(0);
        game.GenerateMoves(moves_list);
        REQUIRE(moves_list.size() == 7);

        position = *(new Position(
            (char *)"k6K/8/8/8/8/8/7p/8",
            (char *)"b",
            (char *)"----",
            (char *)"-",
            (char *)"1",
            (char *)"1"
        ));
        game = *(new Game(position));
        moves_list = game.GetMovesVector(0);
        game.GenerateMoves(moves_list);
        REQUIRE(moves_list.size() == 7);
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
 * * * attacking pawns (X)
 * * * en pasant (X)
 * * * double pawn push (X)
 * 
 * Position
 * * Starting position (X)
 * * illegal positions 
 * * castling rights (X)
 * * move counts (X)
 * 
 * Move Generation
 * * perft
 * 
 * 
 **/