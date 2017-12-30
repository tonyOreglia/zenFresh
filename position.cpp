#include "position.h"

Position::Position()
{
    this->SetStartingPosition();
}

void Position::SetStartingPosition() {
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

    // moving single bit to the starting position of White King and
    // assigning that value to the white king bitboard.
    bitboard.white.king = 1ULL << E1;
    bitboard.white.queen = 1ULL << D1;
    bitboard.white.rooks = (1ULL << A1) | (1ULL << H1);
    bitboard.white.knights = (1ULL << B1) | (1ULL << G1);
    bitboard.white.bishops = (1ULL << C1) | (1ULL << F1);
    bitboard.white.pawns = 0xFF000000000000;

    bitboard.black.king = 1ULL << E8;
    bitboard.black.queen = 1ULL << D8;
    bitboard.black.rooks = (1ULL << A8) | (1ULL << H8);
    bitboard.black.knights = (1ULL << B8) | (1ULL << G8);
    bitboard.black.bishops = (1ULL << C8) | (1ULL << F8);
    bitboard.black.pawns = 0xFF00;

    bitboard.en_passante = 0ULL;
    white_can_castle_king_side_ = true;
    white_can_castle_queen_side_ = true;
    black_can_castle_king_side_ = true;
    black_can_castle_queen_side_ = true;
    half_move_count_ = 0;
    full_move_count_ = 0;
    side_to_move_ = WHITE;

    this->UpdateAllOccupiedSquaresBitBoard();
}

void UpdatePosition(Move& move) {
    
}

uint64_t Position::UpdateAllOccupiedSquaresBitBoard() {
    return bitboard.all_occupied_squares =
        this->UpdateAllWhiteOccupiedSquaresBitBoard() |
        this->UpdateAllBlackOccupiedSquaresBitBoard();
}

uint64_t Position::UpdateAllWhiteOccupiedSquaresBitBoard() {
    return bitboard.white.occupied_squares =
        bitboard.white.king |
        bitboard.white.queen |
        bitboard.white.rooks |
        bitboard.white.knights |
        bitboard.white.bishops |
        bitboard.white.pawns;
}

uint64_t Position::UpdateAllBlackOccupiedSquaresBitBoard() {
    return bitboard.black.occupied_squares =
        bitboard.black.king |
        bitboard.black.queen |
        bitboard.black.rooks |
        bitboard.black.knights |
        bitboard.black.bishops |
        bitboard.black.pawns;
}