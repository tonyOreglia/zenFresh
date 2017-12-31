#include "position.h"

Position::Position()
{
    this->SetStartingPosition();
}

void Position::SetStartingPosition() {
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

void Position::UpdatePieceBitboard(Move& move, struct PieceBitboards *piece_bitboards) {
    uint64_t origin_square_mask = 1ULL << move.GetOriginSquare();
    uint64_t destination_square_mask = 1ULL << move.GetDestinationSquare();
    uint64_t piece_bitboard = this->GetKingBitBoard(*piece_bitboards);
    if (piece_bitboard & origin_square_mask) {
        piece_bitboard ^= origin_square_mask & destination_square_mask;
    }
    piece_bitboard = this->GetQueenBitBoard(*piece_bitboards);
    if (piece_bitboard & origin_square_mask) {
       piece_bitboard ^= origin_square_mask & destination_square_mask;
    }
    piece_bitboard = this->GetRooksBitBoard(*piece_bitboards);
    if (piece_bitboard & origin_square_mask) {
        piece_bitboard ^= origin_square_mask & destination_square_mask;
    }
    piece_bitboard = this->GetBishopsBitBoard(*piece_bitboards);
    if (piece_bitboard & origin_square_mask) {
        piece_bitboard ^= origin_square_mask & destination_square_mask;
    }
    piece_bitboard = this->GetKnightsBitBoard(*piece_bitboards);
    if (piece_bitboard & origin_square_mask) {
        piece_bitboard ^= origin_square_mask & destination_square_mask;
    }
    piece_bitboard = this->GetPawnsBitBoard(*piece_bitboards);
    if (piece_bitboard & origin_square_mask) {
        piece_bitboard ^= origin_square_mask & destination_square_mask;
    }
}

void Position::UpdatePosition(Move& move) {
    struct PieceBitboards *piece_bitboards;
    if (this->GetSideToMove() == WHITE) {
        piece_bitboards = &this->bitboard.white;
    } else {
        piece_bitboards = &this->bitboard.black;
    }
    // update bitboards
    /// Figure out which piece is moving
    

    // update castling, full move, half move counts, side to move
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