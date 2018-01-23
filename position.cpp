#include "position.h"

Position::Position(char *fen, char *side_to_move, char *castling_rights, char *en_passant, char *half_move_ct, char *full_move_ct) {
   
    ClearPosition();
    char board_location = 0;

    for(short fen_index=0; fen_index<strlen(fen); fen_index++) {
        char letter = fen[fen_index];
        switch (letter)
        {
            case 'p' : AddPieceToPawnsBitBoard(BLACK, bitboard_lookup.single_index_bitboard_[board_location]); break;
            case 'r' : AddPieceToRooksBitBoard(BLACK, bitboard_lookup.single_index_bitboard_[board_location]); break;
            case 'n' : AddPieceToKnightsBitBoard(BLACK, bitboard_lookup.single_index_bitboard_[board_location]); break;
            case 'b' : AddPieceToBishopsBitBoard(BLACK, bitboard_lookup.single_index_bitboard_[board_location]); break;
            case 'q' : AddPieceToQueenBitBoard(BLACK, bitboard_lookup.single_index_bitboard_[board_location]); break;
            case 'k' : AddPieceToKingBitBoard(BLACK, bitboard_lookup.single_index_bitboard_[board_location]); break;
            case 'P' : AddPieceToPawnsBitBoard(WHITE, bitboard_lookup.single_index_bitboard_[board_location]); break;
            case 'R' : AddPieceToRooksBitBoard(WHITE, bitboard_lookup.single_index_bitboard_[board_location]); break;
            case 'N' : AddPieceToKnightsBitBoard(WHITE, bitboard_lookup.single_index_bitboard_[board_location]); break;
            case 'B' : AddPieceToBishopsBitBoard(WHITE, bitboard_lookup.single_index_bitboard_[board_location]); break;
            case 'Q' : AddPieceToQueenBitBoard(WHITE, bitboard_lookup.single_index_bitboard_[board_location]); break;
            case 'K' : AddPieceToKingBitBoard(WHITE, bitboard_lookup.single_index_bitboard_[board_location]); break;
            case '/' : board_location--; break;
            case '1' : break;
            case '2' : board_location += 1; break;
            case '3' : board_location += 2; break;
            case '4' : board_location += 3; break;
            case '5' : board_location += 4; break;
            case '6' : board_location += 5; break;
            case '7' : board_location += 6; break;
            case '8' : board_location += 7; break;
            default: break;
        }
        board_location++;
    }


    UpdateAggregateBitboardsFromPieceBitboards();

    if(side_to_move[0] == 'w') SetSideToMove(WHITE);
    else if(side_to_move[0] == 'b') SetSideToMove(BLACK);

    for(char i=0; i<strlen(castling_rights); i++) {
        char letter = castling_rights[i];
        switch(letter) {
            case 'K' : SetWhiteCanCastleKingSide(true); break;
            case 'Q' : SetWhiteCanCastleQueenSide(true); break;
            case 'k' : SetBlackCanCastleKingSide(true); break;
            case 'q' : SetBlackCanCastleQueenSide(true); break;
            case '-' : break;
        }
    }

    if(en_passant[0] != '-')  {
        int file = en_passant[0] - 97;    // 'a' = 0
        int rank = en_passant[1]; // must be 3 or 6
        int index_of_en_passant = (8 - rank) * 8 + file;
        SetEnPassantBitBoard(bitboard_lookup.single_index_bitboard_[index_of_en_passant]);
    }

    SetHalfMoveCount(atoi(half_move_ct));
    SetFullMoveCount(atoi(full_move_ct));
}

void Position::ClearPosition() {
    SetKingBitBoard(BLACK, 0ULL);
    SetQueenBitBoard(BLACK, 0ULL);
    SetBishopsBitBoard(BLACK, 0ULL);
    SetKnightsBitBoard(BLACK, 0ULL);
    SetRooksBitBoard(BLACK, 0ULL);
    SetPawnsBitBoard(BLACK, 0ULL);
    SetKingBitBoard(WHITE, 0ULL);
    SetQueenBitBoard(WHITE, 0ULL);
    SetBishopsBitBoard(WHITE, 0ULL);
    SetKnightsBitBoard(WHITE, 0ULL);
    SetRooksBitBoard(WHITE, 0ULL);
    SetPawnsBitBoard(WHITE, 0ULL);
    SetEnPassantBitBoard(0ULL);

    SetWhiteCanCastleKingSide(false);
    SetWhiteCanCastleQueenSide(false);
    SetBlackCanCastleKingSide(false);
    SetBlackCanCastleQueenSide(false);

    SetHalfMoveCount(0);
    SetFullMoveCount(0);
}


/**
    Set Position class to the position at the start of a chess game.
*/
void Position::SetStartingPosition() {
    // moving single bit to the starting position of White King and
    // assigning that value to the white king bitboard_.
    bitboard_.piece_bitboards[WHITE].king = 1ULL << E1;
    bitboard_.piece_bitboards[WHITE].queen = 1ULL << D1;
    bitboard_.piece_bitboards[WHITE].rooks = (1ULL << A1) | (1ULL << H1);
    bitboard_.piece_bitboards[WHITE].knights = (1ULL << B1) | (1ULL << G1);
    bitboard_.piece_bitboards[WHITE].bishops = (1ULL << C1) | (1ULL << F1);
    bitboard_.piece_bitboards[WHITE].pawns = 0xFF000000000000;

    bitboard_.piece_bitboards[BLACK].king = 1ULL << E8;
    bitboard_.piece_bitboards[BLACK].queen = 1ULL << D8;
    bitboard_.piece_bitboards[BLACK].rooks = (1ULL << A8) | (1ULL << H8);
    bitboard_.piece_bitboards[BLACK].knights = (1ULL << B8) | (1ULL << G8);
    bitboard_.piece_bitboards[BLACK].bishops = (1ULL << C8) | (1ULL << F8);
    bitboard_.piece_bitboards[BLACK].pawns = 0xFF00;

    bitboard_.en_passante = 0ULL;
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
        return &bitboard_.piece_bitboards[side_to_check].king;
    } else if (GetQueenBitBoard(side_to_check) & board_location_bitboard) {
        return &bitboard_.piece_bitboards[side_to_check].queen;
    } else if (GetRooksBitBoard(side_to_check) & board_location_bitboard) {
        return &bitboard_.piece_bitboards[side_to_check].rooks;
    } else if (GetBishopsBitBoard(side_to_check) & board_location_bitboard) {
       return &bitboard_.piece_bitboards[side_to_check].bishops;
    } else if (GetKnightsBitBoard(side_to_check) & board_location_bitboard) {
        return &bitboard_.piece_bitboards[side_to_check].knights;
    } else if (GetPawnsBitBoard(side_to_check) & board_location_bitboard) {
        return &bitboard_.piece_bitboards[side_to_check].pawns;
    } else {
        cout << "returning null pointer from GetPieceBitboardBasedOnBoardLocation\n";
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
 * Toggle the bits of a bitboard_.
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
        bitboard_.en_passante = 
            bitboard_lookup.en_passant_bitboad_lookup_by_pawn_destination[destination_sq];
    }
    if (move.IsEnPassantCapture()) {
        ToggleBitboardBits(bitboard_.piece_bitboards[!side_to_move_].pawns,
            bitboard_lookup.attacked_pawn_location_for_en_passant_capture[destination_sq]);
    } else if (move.IsCapture()) {
        uint64_t* captured_piece_bitboard =
            GetPieceBitboardBasedOnBoardLocation(destination_bitboard, !side_to_move_);
        ToggleBitboardBits(*captured_piece_bitboard, destination_bitboard);
        // this is used when reversing move to add the captured piece back.
        captured_pieces.push(captured_piece_bitboard);
    }
    
    // only update aggregate bitboards after updating moving piece bitboards, and
    // the attacked piece bitboard if there is one.
    UpdateAggregateBitboardsFromPieceBitboards(); 

    if (move.IsPromotion()) {
        ToggleBitboardBits(bitboard_.piece_bitboards[side_to_move_].pawns, destination_bitboard);
        if(move.PromotePawnToQueen()){
            ToggleBitboardBits(bitboard_.piece_bitboards[side_to_move_].queen, destination_bitboard);
        }
        else if(move.PromotePawnToRook()) {
            ToggleBitboardBits(bitboard_.piece_bitboards[side_to_move_].rooks, destination_bitboard);
        }
        else if(move.PromotePawnToKnight()) {
            ToggleBitboardBits(bitboard_.piece_bitboards[side_to_move_].knights, destination_bitboard);
        }
        else if(move.PromotePawnToBishop()) {
            ToggleBitboardBits(bitboard_.piece_bitboards[side_to_move_].bishops, destination_bitboard);
        }
    }

    if (move.RemoveKingSideCastlingRights()) {
        castling_rights_[side_to_move_][KING_SIDE_CASTLE] = false;
    } else if (move.RemoveQueenSideCastlingRights()) {
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
    return bitboard_.all_occupied_squares =
        this->UpdateAllWhiteOccupiedSquaresBitBoard() |
        this->UpdateAllBlackOccupiedSquaresBitBoard();
}

/**
 * Update the bitboard representing the location of any white piece.
 * Update is based on the underlying bitboards representing the location
 * of each white piece type.
*/
uint64_t Position::UpdateAllWhiteOccupiedSquaresBitBoard() {
    return bitboard_.piece_bitboards[WHITE].occupied_squares =
        bitboard_.piece_bitboards[WHITE].king |
        bitboard_.piece_bitboards[WHITE].queen |
        bitboard_.piece_bitboards[WHITE].rooks |
        bitboard_.piece_bitboards[WHITE].knights |
        bitboard_.piece_bitboards[WHITE].bishops |
        bitboard_.piece_bitboards[WHITE].pawns;
}

/**
 * Update the bitboard representing the location of any black piece.
 * Update is based on the underlying bitboards representing the location
 * of each black piece type.
*/
uint64_t Position::UpdateAllBlackOccupiedSquaresBitBoard() {
    return bitboard_.piece_bitboards[BLACK].occupied_squares =
        bitboard_.piece_bitboards[BLACK].king |
        bitboard_.piece_bitboards[BLACK].queen |
        bitboard_.piece_bitboards[BLACK].rooks |
        bitboard_.piece_bitboards[BLACK].knights |
        bitboard_.piece_bitboards[BLACK].bishops |
        bitboard_.piece_bitboards[BLACK].pawns;
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