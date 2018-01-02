#ifndef POSITION_H
#define POSITION_H

#include <cstdint>
#include <bitset>
#include <iostream>
#include "move.h"

#define WHITE true
#define BLACK false

enum {
        A8=0, B8, C8, D8, E8, F8, G8, H8,
        A7, B7, C7, D7, E7, F7, G7, H7,
        A6, B6, C6, D6, E6, F6, G6, H6,
        A5, B5, C5, D5, E5, F5, G5, H5,
        A4, B4, C4, D4, E4, F4, G4, H4,
        A3, B3, C3, D3, E3, F3, G3, H3,
        A2, B2, C2, D2, E2, F2, G2, H2,
        A1, B1, C1, D1, E1, F1, G1, H1
    };

/**
 * Store all information associated with a single chess position.
*/
class Position
{
private:
    uint64_t UpdateAggregateBitboardsFromPieceBitboards();
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
    struct PieceBitboards {
            uint64_t occupied_squares;
            uint64_t king;
            uint64_t queen;
            uint64_t rooks;
            uint64_t knights;
            uint64_t bishops;
            uint64_t pawns;
        };
    struct BitBoard {
        uint64_t en_passante;
        uint64_t all_occupied_squares;
        PieceBitboards piece_bitboards [2];
    } bitboard;
public:
    Position();
    void SetStartingPosition();
    void UpdatePositionWithSingleMove(Move& move);
    void UpdateAllBitboardsWithSingleMove(Move& move);

    uint64_t GetAllOccupiedSquaresBitBoard() {return bitboard.all_occupied_squares;}
    uint64_t GetEnPassanteBitBoard() {return bitboard.en_passante;}

    uint64_t GetOccupiedSquaresBitBoard(bool side_to_move) {return bitboard.piece_bitboards[side_to_move].occupied_squares;}

    uint64_t GetKingBitBoard(bool side_to_move) {return bitboard.piece_bitboards[side_to_move].king;}
    uint64_t GetQueenBitBoard(bool side_to_move) {return bitboard.piece_bitboards[side_to_move].queen;}
    uint64_t GetRooksBitBoard(bool side_to_move) {return bitboard.piece_bitboards[side_to_move].rooks;}
    uint64_t GetKnightsBitBoard(bool side_to_move) {return bitboard.piece_bitboards[side_to_move].knights;}
    uint64_t GetBishopsBitBoard(bool side_to_move) {return bitboard.piece_bitboards[side_to_move].bishops;}
    uint64_t GetPawnsBitBoard(bool side_to_move) {return bitboard.piece_bitboards[side_to_move].pawns;}

    void SetKingBitBoard(bool side_to_move, uint64_t new_bitboard) { bitboard.piece_bitboards[side_to_move].king = new_bitboard; }
    void SetQueenBitBoard(bool side_to_move, uint64_t new_bitboard) { bitboard.piece_bitboards[side_to_move].queen = new_bitboard; }
    void SetRooksBitBoard(bool side_to_move, uint64_t new_bitboard) { bitboard.piece_bitboards[side_to_move].rooks = new_bitboard; }
    void SetKnightsBitBoard(bool side_to_move, uint64_t new_bitboard) { bitboard.piece_bitboards[side_to_move].knights = new_bitboard; }
    void SetBishopsBitBoard(bool side_to_move, uint64_t new_bitboard) { bitboard.piece_bitboards[side_to_move].bishops = new_bitboard; }
    void SetPawnsBitBoard(bool side_to_move, uint64_t new_bitboard) { bitboard.piece_bitboards[side_to_move].pawns = new_bitboard; }
    
    bool GetSideToMove() {return side_to_move_;}
    bool GetWhiteCanCastleKingSideFlag() {return white_can_castle_king_side_;}
    bool GetWhiteCanCastleQueenSideFlag() {return white_can_castle_queen_side_;}
    bool GetBlackCanCastleKingSideFlag() {return black_can_castle_king_side_;}
    bool GetBlackCanCastleQueenSideFlag() {return black_can_castle_queen_side_;}
    uint8_t GetHalfMoveCount() {return half_move_count_;}
    short int GetFullMoveCount() {return full_move_count_;}

    void PrintBitBoard(uint64_t);
};

#endif
