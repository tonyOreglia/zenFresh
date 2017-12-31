#ifndef POSITION_H
#define POSITION_H

#include <cstdint>
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
        PieceBitboards white;
        PieceBitboards black;
    } bitboard;
public:
    Position();
    void SetStartingPosition();
    void UpdatePosition(Move& move);
    void UpdatePieceBitboard(Move& move, struct PieceBitboards *piece_bitboards);

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

    uint64_t GetKingBitBoard(struct PieceBitboards piece_bitboard) {return piece_bitboard.king;}
    uint64_t GetQueenBitBoard(struct PieceBitboards piece_bitboard) {return piece_bitboard.queen;}
    uint64_t GetRooksBitBoard(struct PieceBitboards piece_bitboard) {return piece_bitboard.rooks;}
    uint64_t GetKnightsBitBoard(struct PieceBitboards piece_bitboard) {return piece_bitboard.knights;}
    uint64_t GetBishopsBitBoard(struct PieceBitboards piece_bitboard) {return piece_bitboard.bishops;}
    uint64_t GetPawnsBitBoard(struct PieceBitboards piece_bitboard) {return piece_bitboard.pawns;}
    
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
