#include "game.h"

Game::Game(Position position) : bitboard_lookup_(position.bitboard_lookup) {
    this->position = position;
    bitboard_lookup_ = position.bitboard_lookup;
    potential_moves_ = new vector<Move>[DEPTH];
    for (char i = 0; i < DEPTH; i++) {
        potential_moves_[i].reserve(MAX_MOVES_FROM_SINGLE_POSITION);
    }
}

void Game::GenerateMoves(vector <Move>& move_list) {
    GenerateKingMoves(move_list);
    GenerateRookMoves(move_list);
    GenerateBishopMoves(move_list);
    GenerateQueenMoves(move_list);
    GenerateKnightMoves(move_list);
    if (position.GetSideToMove() == WHITE) {
        GenerateWhitePawnMoves(move_list);
    } else {
        GenerateBlackPawnMoves(move_list);
    }
    
}

void Game::PushSingleMoveFromValidMovesBBToMovesVector(
        short origin_square,
        uint64_t &valid_moves_bb,
        vector <Move>& move_list
    ) {
    short destination_position = lsb_scan(valid_moves_bb);
    valid_moves_bb ^= bitboard_lookup_.single_index_bitboard_[destination_position];
    Move move(origin_square, destination_position);
    if (
        bitboard_lookup_.single_index_bitboard_[destination_position] &
        position.GetOccupiedSquaresBitBoard(!position.GetSideToMove())
    ) move.SetCaptureFlag();
    move_list.push_back(move);
}


void Game::GenerateBishopMoves(vector <Move>& move_list) {
    uint64_t bishop_bitboard_copy = position.GetBishopsBitBoard();

    while(bishop_bitboard_copy) {
        short bishop_position =
            EjectIndexFromBitboard(bishop_bitboard_copy, position.bitboard_lookup.single_index_bitboard_);
        uint64_t valid_moves_bb = GenerateValidDiagonalSlidingMovesBB(bishop_position);
        valid_moves_bb &= ~position.GetActiveSidesOccupiedSquaresBB();

        while(valid_moves_bb) {
            PushSingleMoveFromValidMovesBBToMovesVector(bishop_position, valid_moves_bb, move_list);
        }
    }
}

void Game::GenerateRookMoves(vector <Move>& move_list) {
    uint64_t rook_bitboard_copy = position.GetRooksBitBoard();

    while(rook_bitboard_copy) {
        uint8_t rook_position =
            EjectIndexFromBitboard(rook_bitboard_copy, position.bitboard_lookup.single_index_bitboard_);
        uint64_t valid_moves_bb = GenerateValidStraightSlidingMovesBB(rook_position);
        valid_moves_bb &= ~position.GetActiveSidesOccupiedSquaresBB();

        while(valid_moves_bb) {
            PushSingleMoveFromValidMovesBBToMovesVector(rook_position, valid_moves_bb, move_list);
            if (rook_position == 7 || rook_position == 63) move_list.back().SetRemoveKingSideCastleRightsFlag();
            if (rook_position == 0 || rook_position == 56) move_list.back().SetRemoveQueenSideCastleRightsFlag();   
        }
    }
}

void Game::GenerateQueenMoves(vector <Move>& move_list) {
    uint64_t queen_bitboard_copy = position.GetQueenBitBoard();
    while(queen_bitboard_copy) {
        uint8_t queen_position =
            EjectIndexFromBitboard(queen_bitboard_copy, position.bitboard_lookup.single_index_bitboard_);
        uint64_t valid_moves_bb =
            GenerateValidStraightSlidingMovesBB(queen_position) |
            GenerateValidDiagonalSlidingMovesBB(queen_position);
        valid_moves_bb &= ~position.GetActiveSidesOccupiedSquaresBB();
        while(valid_moves_bb) {
            PushSingleMoveFromValidMovesBBToMovesVector(queen_position, valid_moves_bb, move_list); 
        }
    }
}

void Game::GenerateKnightMoves(vector <Move>& move_list) {
    uint64_t knight_bb_copy = position.GetKnightsBitBoard();
    while(knight_bb_copy) {
        uint8_t knight_position =
            EjectIndexFromBitboard(knight_bb_copy, position.bitboard_lookup.single_index_bitboard_);
        uint64_t valid_moves_bb = position.bitboard_lookup.knight_attack_bitboard_lookup[knight_position];
        valid_moves_bb &= ~position.GetActiveSidesOccupiedSquaresBB();
        while(valid_moves_bb) {
            PushSingleMoveFromValidMovesBBToMovesVector(knight_position, valid_moves_bb, move_list); 
        }
    }
}

void Game::GenerateKingMoves(vector <Move>& move_list) {
    // must check if Rook is on proper square for particular castling move.
    // this is better than checking every move if it captures a rook.
    // Rook moves will kill castling ability, but captures of rook will not.

    // this fxnality assumes there is king on the board and will cause errors if not.
    uint64_t king_bb_copy = position.GetKingBitBoard();
    uint8_t king_position = lsb_scan(king_bb_copy);
    uint64_t valid_moves_bb =
        position.bitboard_lookup.king_move_bitboard_lookup[position.GetSideToMove()][king_position];
    valid_moves_bb &= ~position.GetActiveSidesOccupiedSquaresBB();
    while(valid_moves_bb) {
        PushSingleMoveFromValidMovesBBToMovesVector(king_position, valid_moves_bb, move_list); 
        move_list.back().PrintMove();
        move_list.back().SetRemoveKingSideCastleRightsFlag();
        move_list.back().SetRemoveQueenSideCastleRightsFlag();
    }
    cout << "move cnt: " << move_list.size() << endl;
}

void Game::GenerateWhitePawnMoves(vector <Move>& move_list) {
    uint64_t pawn_bb_copy = position.GetPawnsBitBoard();
    cout << "pawn bb: \n";
    position.PrintBitBoard(pawn_bb_copy);
    uint64_t valid_single_push_moves_bb = pawn_bb_copy >> 8;
    valid_single_push_moves_bb ^= (valid_single_push_moves_bb & position.GetAllOccupiedSquaresBitBoard());
    uint64_t valid_double_push_pawn_moves_bb = (valid_single_push_moves_bb >> 8) & position.bitboard_lookup.fourth_rank;
    valid_double_push_pawn_moves_bb ^= (valid_double_push_pawn_moves_bb & position.GetAllOccupiedSquaresBitBoard());

    // position.PrintBitBoard(valid_single_push_moves_bb);
    while(valid_single_push_moves_bb) {
        uint8_t destination_position = lsb_scan(valid_single_push_moves_bb);
        valid_single_push_moves_bb ^= bitboard_lookup_.single_index_bitboard_[destination_position];
        Move move(destination_position + 8, destination_position);
        move_list.push_back(move);
    }

    while(valid_double_push_pawn_moves_bb) {
        uint8_t destination_position = lsb_scan(valid_double_push_pawn_moves_bb);
        valid_double_push_pawn_moves_bb ^= bitboard_lookup_.single_index_bitboard_[destination_position];
        Move move(destination_position + 16, destination_position);
        move.SetDoublePawnPushFlag();
        move_list.push_back(move);
    }

    uint64_t valid_pawn_attacks_right =
        (pawn_bb_copy & ~bitboard_lookup_.a_file)  >> 7;
    valid_pawn_attacks_right &=
        position.GetOccupiedSquaresBitBoard(!position.GetSideToMove()) | position.GetEnPassanteBitBoard();

    while(valid_pawn_attacks_right) {
        uint8_t destination_position = lsb_scan(valid_pawn_attacks_right);
        valid_pawn_attacks_right ^= bitboard_lookup_.single_index_bitboard_[destination_position];
        Move move(destination_position + 7, destination_position);
        move.SetCaptureFlag();
        move_list.push_back(move);
    }

    uint64_t valid_pawn_attacks_left =
        (pawn_bb_copy & ~bitboard_lookup_.a_file)  >> 9;
    valid_pawn_attacks_left &=
        position.GetOccupiedSquaresBitBoard(!position.GetSideToMove()) | position.GetEnPassanteBitBoard();

    while(valid_pawn_attacks_left) {
        uint8_t destination_position = lsb_scan(valid_pawn_attacks_left);
        valid_pawn_attacks_left ^= bitboard_lookup_.single_index_bitboard_[destination_position];
        Move move(destination_position + 9, destination_position);
        move.SetCaptureFlag();
        move_list.push_back(move);
    } 
}

void Game::GenerateBlackPawnMoves(vector <Move>& move_list) {
    uint64_t pawn_bb_copy = position.GetPawnsBitBoard();
    uint64_t valid_single_push_moves_bb = pawn_bb_copy << 8;
    valid_single_push_moves_bb ^= (valid_single_push_moves_bb & position.GetAllOccupiedSquaresBitBoard());
    uint64_t valid_double_push_pawn_moves_bb = (valid_single_push_moves_bb << 8) & position.bitboard_lookup.fourth_rank;
    valid_double_push_pawn_moves_bb ^= (valid_double_push_pawn_moves_bb & position.GetAllOccupiedSquaresBitBoard());

    // position.PrintBitBoard(valid_single_push_moves_bb);
    while(valid_single_push_moves_bb) {
        uint8_t destination_position = lsb_scan(valid_single_push_moves_bb);
        valid_single_push_moves_bb ^= bitboard_lookup_.single_index_bitboard_[destination_position];
        Move move(destination_position - 8, destination_position);
        move_list.push_back(move);
    }

    while(valid_double_push_pawn_moves_bb) {
        uint8_t destination_position = lsb_scan(valid_double_push_pawn_moves_bb);
        valid_double_push_pawn_moves_bb ^= bitboard_lookup_.single_index_bitboard_[destination_position];
        Move move(destination_position - 16, destination_position);
        move.SetDoublePawnPushFlag();
        move_list.push_back(move);
    }

    uint64_t valid_pawn_attacks_right =
        (pawn_bb_copy & ~bitboard_lookup_.a_file)  << 7;
    valid_pawn_attacks_right &=
        position.GetOccupiedSquaresBitBoard(!position.GetSideToMove()) | position.GetEnPassanteBitBoard();

    while(valid_pawn_attacks_right) {
        uint8_t destination_position = lsb_scan(valid_pawn_attacks_right);
        valid_pawn_attacks_right ^= bitboard_lookup_.single_index_bitboard_[destination_position];
        Move move(destination_position - 7, destination_position);
        move.SetCaptureFlag();
        move_list.push_back(move);
    }

    uint64_t valid_pawn_attacks_left =
        (pawn_bb_copy & ~bitboard_lookup_.a_file)  << 9;
    valid_pawn_attacks_left &=
        position.GetOccupiedSquaresBitBoard(!position.GetSideToMove()) | position.GetEnPassanteBitBoard();

    while(valid_pawn_attacks_left) {
        uint8_t destination_position = lsb_scan(valid_pawn_attacks_left);
        valid_pawn_attacks_left ^= bitboard_lookup_.single_index_bitboard_[destination_position];
        Move move(destination_position - 9, destination_position);
        move.SetCaptureFlag();
        move_list.push_back(move);
    } 
}


uint64_t Game::GenerateValidMovesNorthBitboard(char index) {
    uint64_t bb_occupied_squares_overlap_with_north_array = 
        position.GetAllOccupiedSquaresBitBoard() &
        bitboard_lookup_.north_array_bitboard_lookup[index];    
    if(bb_occupied_squares_overlap_with_north_array) {
        char most_significant_bit = msb_scan(bb_occupied_squares_overlap_with_north_array);
        return (bitboard_lookup_.north_array_bitboard_lookup[index] ^
            bitboard_lookup_.north_array_bitboard_lookup[most_significant_bit]);
    }
    return bitboard_lookup_.north_array_bitboard_lookup[index];
}

uint64_t Game::GenerateValidMovesEastBitboard(char index) {
    uint64_t bb_occupied_squares_overlap_with_east_array = 
        position.GetAllOccupiedSquaresBitBoard() &
        bitboard_lookup_.east_array_bitboard_lookup[index];

    if(bb_occupied_squares_overlap_with_east_array) {
        char least_significant_bit = lsb_scan(bb_occupied_squares_overlap_with_east_array);
        return (bitboard_lookup_.east_array_bitboard_lookup[index] ^
            bitboard_lookup_.east_array_bitboard_lookup[least_significant_bit]);
    }
    return bitboard_lookup_.east_array_bitboard_lookup[index];
}

uint64_t Game::GenerateValidMovesSouthBitboard(char index) {
    uint64_t bb_occupied_squares_overlap_with_south_array = 
        position.GetAllOccupiedSquaresBitBoard() &
        bitboard_lookup_.south_array_bitboard_lookup[index];
    
    if(bb_occupied_squares_overlap_with_south_array) {
        char least_significant_bit = lsb_scan(bb_occupied_squares_overlap_with_south_array);
        return (bitboard_lookup_.south_array_bitboard_lookup[index] ^
            bitboard_lookup_.south_array_bitboard_lookup[least_significant_bit]);
    }
    return bitboard_lookup_.south_array_bitboard_lookup[index];
}

uint64_t Game::GenerateValidMovesWestBitboard(char index) {
    uint64_t bb_occupied_squares_overlap_with_west_array = 
        position.GetAllOccupiedSquaresBitBoard() &
        bitboard_lookup_.west_array_bitboard_lookup[index];
    
    if(bb_occupied_squares_overlap_with_west_array) {
        char most_significant_bit = msb_scan(bb_occupied_squares_overlap_with_west_array);
        return (bitboard_lookup_.west_array_bitboard_lookup[index] ^
            bitboard_lookup_.west_array_bitboard_lookup[most_significant_bit]);
    }
    return bitboard_lookup_.west_array_bitboard_lookup[index];
}

uint64_t Game::GenerateValidMovesNorthEastBitboard(char index) {
    uint64_t bb_occupied_squares_overlap_with_northeast_array = 
        position.GetAllOccupiedSquaresBitBoard() &
        bitboard_lookup_.north_east_array_bitboard_lookup[index];

    if(bb_occupied_squares_overlap_with_northeast_array) {
        char most_significant_bit = msb_scan(bb_occupied_squares_overlap_with_northeast_array);
        return (bitboard_lookup_.north_east_array_bitboard_lookup[index] ^
            bitboard_lookup_.north_east_array_bitboard_lookup[most_significant_bit]);
    }
    return bitboard_lookup_.north_east_array_bitboard_lookup[index];
}

uint64_t Game::GenerateValidMovesNorthWestBitboard(char index) {
    uint64_t bb_occupied_squares_overlap_with_northwest_array = 
        position.GetAllOccupiedSquaresBitBoard() &
        bitboard_lookup_.north_west_array_bitboard_lookup[index];
    
    if(bb_occupied_squares_overlap_with_northwest_array) {
        char most_significant_bit = msb_scan(bb_occupied_squares_overlap_with_northwest_array);
        return (bitboard_lookup_.north_west_array_bitboard_lookup[index] ^
            bitboard_lookup_.north_west_array_bitboard_lookup[most_significant_bit]);
    }
    return bitboard_lookup_.north_west_array_bitboard_lookup[index];
}

uint64_t Game::GenerateValidMovesSouthEastBitboard(char index) {
    uint64_t bb_occupied_squares_overlap_with_southeast_array = 
        position.GetAllOccupiedSquaresBitBoard() &
        bitboard_lookup_.south_east_array_bitboard_lookup[index];
    
    if(bb_occupied_squares_overlap_with_southeast_array) {
        char least_significant_bit = lsb_scan(bb_occupied_squares_overlap_with_southeast_array);
        return (bitboard_lookup_.south_east_array_bitboard_lookup[index] ^
            bitboard_lookup_.south_east_array_bitboard_lookup[least_significant_bit]);
    }
    return bitboard_lookup_.south_east_array_bitboard_lookup[index];
}

uint64_t Game::GenerateValidMovesSouthWestBitboard(char index) {
    uint64_t bb_occupied_squares_overlap_with_southwest_array = 
        position.GetAllOccupiedSquaresBitBoard() &
        bitboard_lookup_.south_west_array_bitboard_lookup[index];
    
    if(bb_occupied_squares_overlap_with_southwest_array) {
        char least_significant_bit = lsb_scan(bb_occupied_squares_overlap_with_southwest_array);
        return (bitboard_lookup_.south_west_array_bitboard_lookup[index] ^
            bitboard_lookup_.south_west_array_bitboard_lookup[least_significant_bit]);
    }
    return bitboard_lookup_.south_west_array_bitboard_lookup[index];
}

uint64_t Game::GenerateValidDiagonalSlidingMovesBB(char index) {
    return (GenerateValidMovesNorthEastBitboard(index) |
            GenerateValidMovesSouthEastBitboard(index) |
            GenerateValidMovesSouthWestBitboard(index) |
            GenerateValidMovesNorthWestBitboard(index));
}
uint64_t Game::GenerateValidStraightSlidingMovesBB(char index) {
    return (GenerateValidMovesNorthBitboard(index) |
            GenerateValidMovesEastBitboard(index) |
            GenerateValidMovesSouthBitboard(index) |
            GenerateValidMovesWestBitboard(index));
}

uint64_t Game::PerformanceTest(short depth) {
    short current_depth = 0;
    uint64_t total_number_of_potential_moves = 0ULL;
    while (current_depth < depth) {
        GenerateMoves(potential_moves_[current_depth]);
        total_number_of_potential_moves += potential_moves_[current_depth].size();
        current_depth++;
    }
}






