#ifndef POSITION_H
#define POSITION_H

#include <cstdint>
#include "move.h"

#define WHITE true
#define BLACK false

/**
 * Store all information associated with a single chess position.
*/
class Position
{
private:
    uint64_t UpdateAllOccupiedSquaresBitBoard();
    uint64_t UpdateAllWhiteOccupiedSquaresBitBoard();
    uint64_t UpdateAllBlackOccupiedSquaresBitBoard();
    bool white_can_castle_king_side_;
    bool white_can_castle_queen_side_;
    bool black_can_castle_king_side_;
    bool black_can_castle_queen_side_;
    //number of half moves since the last capture or pawn advance.
    // This is monitored to honour the fifty move rule.
    uint8_t half_move_count_;
    short int full_move_count_;
    bool side_to_move_; // using defined constants WHITE or BLACK
    // 64 bit integers store a single piece of informatio for each square
    // on the board. By combining bitboards you can store the full position.
    struct BitBoard {
        uint64_t en_passante;
        uint64_t all_occupied_squares;
        struct White {
            uint64_t occupied_squares;
            uint64_t king;
            uint64_t queen;
            uint64_t rooks;
            uint64_t knights;
            uint64_t bishops;
            uint64_t pawns;
        } white;
        struct Black {
            uint64_t occupied_squares;
            uint64_t king;
            uint64_t queen;
            uint64_t rooks;
            uint64_t knights;
            uint64_t bishops;
            uint64_t pawns;
        } black;
    } bitboard;
public:
    Position();
    void SetStartingPosition();
    void UpdatePosition(Move& move);
    uint64_t GetAllOccupiedSquaresBitBoard() {return bitboard.all_occupied_squares;}

    uint64_t GetWhiteOccupiedSquaresBitBoard() {return bitboard.white.occupied_squares;}
    uint64_t GetBlackOccupiedSquaresBitBoard() {return bitboard.black.occupied_squares;}

    uint64_t GetWhiteKingBitBoard() {return bitboard.white.king;}
    uint64_t GetWhiteQueenBitBoard() {return bitboard.white.queen;}
    uint64_t GetWhiteRooksBitBoard() {return bitboard.white.rooks;}
    uint64_t GetWhiteKnightsBitBoard() {return bitboard.white.knights;}
    uint64_t GetWhiteBishopsBitBoard() {return bitboard.white.bishops;}
    uint64_t GetWhitePawnsBitBoard() {return bitboard.white.pawns;}

    uint64_t GetBlackKingBitBoard() {return bitboard.black.king;}
    uint64_t GetBlackQueenBitBoard() {return bitboard.black.queen;}
    uint64_t GetBlackRooksBitBoard() {return bitboard.black.rooks;}
    uint64_t GetBlackKnightsBitBoard() {return bitboard.black.knights;}
    uint64_t GetBlackBishopsBitBoard() {return bitboard.black.bishops;}
    uint64_t GetBlackPawnsBitBoard() {return bitboard.black.pawns;}
    
    uint64_t GetEnPassanteBitBoard() {return bitboard.en_passante;}
    bool GetSideToMove() {return side_to_move_;}
    bool GetWhiteCanCastleKingSideFlag() {return white_can_castle_king_side_;}
    bool GetWhiteCanCastleQueenSideFlag() {return white_can_castle_queen_side_;}
    bool GetBlackCanCastleKingSideFlag() {return black_can_castle_king_side_;}
    bool GetBlackCanCastleQueenSideFlag() {return black_can_castle_queen_side_;}
    uint8_t GetHalfMoveCount() {return half_move_count_;}
    short int GetFullMoveCount() {return full_move_count_;}
};

#endif
