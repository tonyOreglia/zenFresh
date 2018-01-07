#include "position.h"

Position::Position()
{
    SetStartingPosition();
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
    castling_rights_[WHITE][QUEEN_SIDE_CASTLE] = true;
    castling_rights_[WHITE][KING_SIDE_CASTLE] = true;
    castling_rights_[BLACK][KING_SIDE_CASTLE] = true;
    castling_rights_[BLACK][QUEEN_SIDE_CASTLE] = true;
    half_move_count_ = 0;
    full_move_count_ = 0;
    side_to_move_ = WHITE;

    UpdateAggregateBitboardsFromPieceBitboards();
}

/**
 * Given a bitboard representing a single location on the board and the side to move,
 * return the piece bitboard for the piece type and color that resides there.
 * 
 * @param (uint64_t) board_location_bitboard - bitboard indicating the location to
 * check each piece bitboard for a pice.
 * @param (bool) side_to_check - Indicates weather to check white or black piece bitboards.
 */
uint64_t* Position::GetPieceBitboardBasedOnBoardLocation(uint64_t board_location_bitboard, bool side_to_check) {
    if (GetKingBitBoard(side_to_check) & board_location_bitboard) {
        return &bitboard.piece_bitboards[side_to_check].king;
    } else if (GetQueenBitBoard(side_to_check) & board_location_bitboard) {
        return &bitboard.piece_bitboards[side_to_check].queen;
    } else if (GetRooksBitBoard(side_to_check) & board_location_bitboard) {
        return &bitboard.piece_bitboards[side_to_check].rooks;
    } else if (GetBishopsBitBoard(side_to_check) & board_location_bitboard) {
       return &bitboard.piece_bitboards[side_to_check].bishops;
    } else if (GetKnightsBitBoard(side_to_check) & board_location_bitboard) {
        return &bitboard.piece_bitboards[side_to_check].knights;
    } else if (GetPawnsBitBoard(side_to_check) & board_location_bitboard) {
        return &bitboard.piece_bitboards[side_to_check].pawns;
    } else {
        return nullptr;
    }
}


/**
 * Update the Position bitboards for a single move
 * 
 * @param (uint64_t) origin - bitboard representing a single location. Origin location of a single chess move.
 * @param (uint64_t) destination - bitboard representing a single location.
 * Destination location of a single chess move.
*/
void Position::UpdateMovingPieceBitboardWithSingleMove(uint64_t origin_bitboard, uint64_t destination_bitboard) {
    uint64_t* moving_piece_bitboard = GetPieceBitboardBasedOnBoardLocation(origin_bitboard, side_to_move_);
    ToggleBitboardBits(*moving_piece_bitboard, origin_bitboard | destination_bitboard);
}

/**
 * Toggle the bits of a bitboard.
 * @param (uint64_t&) piece_bitboard - Bitboard representing the position(s) of a piece
 * type of a particular color e.g. white bishops.
 * @param (uint64_t) toggle_positions - Bitboard indicating which bit positions to be toggled.
 */ 
void Position::ToggleBitboardBits(uint64_t& piece_bitboard, uint64_t toggle_positions) {
    piece_bitboard ^= toggle_positions; 
}   

/**
 * Update instance of Position class with a single move.
 * @param (Move&) Reference to class representing a single chess move.
*/
void Position::UpdatePositionWithSingleMove(Move& move) {
    uint16_t origin_sq = move.GetOriginSquare();
    uint16_t destination_sq = move.GetDestinationSquare();
    uint64_t origin_bitboard = 1ULL << origin_sq;
    uint64_t destination_bitboard = 1ULL << destination_sq;

    UpdateMovingPieceBitboardWithSingleMove(origin_bitboard, destination_bitboard);

    if (move.IsDoublePawnPush()) {
        bitboard.en_passante = 
            bitboard_lookup.en_passant_bitboad_lookup_by_pawn_destination[destination_sq];
    }
    if (move.IsEnPassantCapture()) {
        ToggleBitboardBits(bitboard.piece_bitboards[!side_to_move_].pawns,
            bitboard_lookup.attacked_pawn_location_for_en_passant_capture[destination_sq]);
    } else if (move.IsCapture()) {
        uint64_t* captured_piece_bitboard =
            GetPieceBitboardBasedOnBoardLocation(destination_sq, !side_to_move_);
        ToggleBitboardBits(*captured_piece_bitboard, destination_bitboard);
        // this is used when reversing move to add the captured piece back.
        captured_pieces.push(captured_piece_bitboard);
    }
    

    // only update aggregate bitboards after updating moving piece bitboards, and
    // the attacked piece bitboard if there is one.
    UpdateAggregateBitboardsFromPieceBitboards(); 

    if (move.IsPromotion()) {
        ToggleBitboardBits(bitboard.piece_bitboards[side_to_move_].pawns, destination_bitboard);
        if(move.PromotePawnToQueen()){
            ToggleBitboardBits(bitboard.piece_bitboards[side_to_move_].queen, destination_bitboard);
        }
        else if(move.PromotePawnToRook()) {
            ToggleBitboardBits(bitboard.piece_bitboards[side_to_move_].rooks, destination_bitboard);
        }
        else if(move.PromotePawnToKnight()) {
            ToggleBitboardBits(bitboard.piece_bitboards[side_to_move_].knights, destination_bitboard);
        }
        else if(move.PromotePawnToBishop()) {
            ToggleBitboardBits(bitboard.piece_bitboards[side_to_move_].bishops, destination_bitboard);
        }
    }

    if (move.IsKingSideCastle()) {
        castling_rights_[side_to_move_][KING_SIDE_CASTLE] = false;
    } else if (move.IsQueenSideCastle()) {
        castling_rights_[side_to_move_][QUEEN_SIDE_CASTLE] = false;
    }
}

/**
 * Update the bitboard that represents each square that has a piece of either colour.
 * This function also updates the bitboard representing each square with a white piece,
 * and the bitboard representing each piece with a black piece.
 * Update is based on the underlying bitboards representing the location of each piece.
*/
uint64_t Position::UpdateAggregateBitboardsFromPieceBitboards() {
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

/**
 * Print a bitboard in an 8x8 grid, representing a chess board layout.
 * @param (uint64_t) bitboard - Bitboard to print.
 */
void Position::PrintBitBoard(uint64_t bitboard) {
    std::bitset<64> bb (bitboard);
    for(int i=0;i<64;i++) {
        std::cout << bb[i];
        if(((i+1) % 8) == 0) 
            std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << std::endl;
}