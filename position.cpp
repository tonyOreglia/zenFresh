#include "position.h"

Position::Position()
{
    this->SetStartingPosition();
}

/**
    Set Position class to the position at the start of a chess game.
*/
void Position::SetStartingPosition() {
    // moving single bit to the starting position of White King and
    // assigning that value to the white king bitboard.
    bitboard.piece_bitboards[WHITE].king = 1ULL << E1;
    bitboard.piece_bitboards[WHITE].queen = 1ULL << D1;
    bitboard.piece_bitboards[WHITE].rooks = (1ULL << A1) | (1ULL << H1);
    bitboard.piece_bitboards[WHITE].knights = (1ULL << B1) | (1ULL << G1);
    bitboard.piece_bitboards[WHITE].bishops = (1ULL << C1) | (1ULL << F1);
    bitboard.piece_bitboards[WHITE].pawns = 0xFF000000000000;

    bitboard.piece_bitboards[BLACK].king = 1ULL << E8;
    bitboard.piece_bitboards[BLACK].queen = 1ULL << D8;
    bitboard.piece_bitboards[BLACK].rooks = (1ULL << A8) | (1ULL << H8);
    bitboard.piece_bitboards[BLACK].knights = (1ULL << B8) | (1ULL << G8);
    bitboard.piece_bitboards[BLACK].bishops = (1ULL << C8) | (1ULL << F8);
    bitboard.piece_bitboards[BLACK].pawns = 0xFF00;

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

/**
 * Update the Position bitboards for a single move
 * 
 * @param (Move&) Reference to class representing a single chess move.
 * @param (PieceBitboards&) Reference to struct containing bitboards representing the location
 * of each piece type for the color whose turn it is.
*/
void Position::UpdatePieceBitboard(Move& move, struct PieceBitboards &moving_sides_struct_containing_piece_bitboards) {
    uint64_t origin_square_mask = 1ULL << move.GetOriginSquare();
    uint64_t destination_square_mask = 1ULL << move.GetDestinationSquare();
    uint64_t moving_sides_piece_bitboard = GetKingBitBoard(moving_sides_struct_containing_piece_bitboards);

    if (moving_sides_piece_bitboard & origin_square_mask) {
        moving_sides_piece_bitboard ^= origin_square_mask & destination_square_mask;

    }
    moving_sides_piece_bitboard = this->GetQueenBitBoard(moving_sides_struct_containing_piece_bitboards);
    if (moving_sides_piece_bitboard & origin_square_mask) {
       moving_sides_piece_bitboard ^= origin_square_mask & destination_square_mask;
    }
    moving_sides_piece_bitboard = this->GetRooksBitBoard(moving_sides_struct_containing_piece_bitboards);
    if (moving_sides_piece_bitboard & origin_square_mask) {
        moving_sides_piece_bitboard ^= origin_square_mask & destination_square_mask;
    }
    moving_sides_piece_bitboard = this->GetBishopsBitBoard(moving_sides_struct_containing_piece_bitboards);
    if (moving_sides_piece_bitboard & origin_square_mask) {
        moving_sides_piece_bitboard ^= origin_square_mask & destination_square_mask;
    }
    moving_sides_piece_bitboard = this->GetKnightsBitBoard(moving_sides_struct_containing_piece_bitboards);
    if (moving_sides_piece_bitboard & origin_square_mask) {
        moving_sides_piece_bitboard ^= origin_square_mask & destination_square_mask;
    }
    moving_sides_piece_bitboard = this->GetPawnsBitBoard(moving_sides_struct_containing_piece_bitboards);
    if (moving_sides_piece_bitboard & origin_square_mask) {
        moving_sides_piece_bitboard ^= origin_square_mask & destination_square_mask;
    }
}

/**
 * Update instance of Position class with a single move.
 * 
 * @param (Move&) Reference to class representing a single chess move.
*/
void Position::UpdatePosition(Move& move) {
    struct PieceBitboards piece_bitboards;
    if (GetSideToMove() == WHITE) {
        piece_bitboards = bitboard.white;
    } else {
        piece_bitboards = this->bitboard.black;
    }
    // update bitboards
    /// Figure out which piece is moving
    

    // update castling, full move, half move counts, side to move
}

/**
 * Update the bitboard that represents each square that has a piece of either colour.
 * This function also updates the bitboard representing each square with a white piece,
 * and the bitboard representing each piece with a black piece.
 * Update is based on the underlying bitboards representing the location of each piece.
*/
uint64_t Position::UpdateAllOccupiedSquaresBitBoard() {
    return bitboard.all_occupied_squares =
        this->UpdateAllWhiteOccupiedSquaresBitBoard() |
        this->UpdateAllBlackOccupiedSquaresBitBoard();
}

/**
 * Update the bitboard representing the location of any white piece.
 * Update is based on the underlying bitboards representing the location
 * of each white piece type.
*/
uint64_t Position::UpdateAllWhiteOccupiedSquaresBitBoard() {
    return bitboard.piece_bitboards[WHITE].occupied_squares =
        bitboard.piece_bitboards[WHITE].king |
        bitboard.piece_bitboards[WHITE].queen |
        bitboard.piece_bitboards[WHITE].rooks |
        bitboard.piece_bitboards[WHITE].knights |
        bitboard.piece_bitboards[WHITE].bishops |
        bitboard.piece_bitboards[WHITE].pawns;
}

/**
 * Update the bitboard representing the location of any black piece.
 * Update is based on the underlying bitboards representing the location
 * of each black piece type.
*/
uint64_t Position::UpdateAllBlackOccupiedSquaresBitBoard() {
    return bitboard.piece_bitboards[BLACK].occupied_squares =
        bitboard.piece_bitboards[BLACK].king |
        bitboard.piece_bitboards[BLACK].queen |
        bitboard.piece_bitboards[BLACK].rooks |
        bitboard.piece_bitboards[BLACK].knights |
        bitboard.piece_bitboards[BLACK].bishops |
        bitboard.piece_bitboards[BLACK].pawns;
}