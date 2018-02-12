#include "game.h"

Game::Game(Position position) {
    this->position = position;
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
    GeneratePawnMoves(move_list);
}

void Game::PushSingleMoveFromValidMovesBBToMovesVector(
        short origin_square,
        uint64_t &valid_moves_bb,
        vector <Move>& move_list
    ) {
    short destination_position = lsb_scan(valid_moves_bb);
    valid_moves_bb ^= single_index_bitboard_[destination_position];
    Move move(origin_square, destination_position);
    if (
        single_index_bitboard_[destination_position] &
        position.GetOccupiedSquaresBitBoard(!position.GetSideToMove())
    ) move.SetCaptureFlag();
    move_list.push_back(move);
}

void Game::PushValidMovesBBToMovesVector(
        short origin_square,
        uint64_t &valid_moves_bb,
        vector <Move>& move_list
    ) {
        while(valid_moves_bb) {
            PushSingleMoveFromValidMovesBBToMovesVector(origin_square, valid_moves_bb, move_list);
        }
    }

void Game::GenerateBishopMoves(vector <Move>& move_list) {
    uint64_t bishop_bitboard_copy = position.GetBishopsBitBoard();
    while(bishop_bitboard_copy) {
        uint8_t bishop_position =
            EjectIndexFromBitboard(bishop_bitboard_copy, single_index_bitboard_);
        uint64_t valid_moves_bb = GenerateValidDiagonalSlidingMovesBB(bishop_position);
        valid_moves_bb &= ~position.GetActiveSidesOccupiedSquaresBB(); 
        PushValidMovesBBToMovesVector(bishop_position, valid_moves_bb, move_list);
    }
}

void Game::GenerateRookMoves(vector <Move>& move_list) {
    uint64_t rook_bitboard_copy = position.GetRooksBitBoard();
    while(rook_bitboard_copy) {
        uint8_t rook_position =
            EjectIndexFromBitboard(rook_bitboard_copy, single_index_bitboard_);
        uint64_t valid_moves_bb = GenerateValidStraightSlidingMovesBB(rook_position);
        valid_moves_bb &= ~position.GetActiveSidesOccupiedSquaresBB();
        PushValidMovesBBToMovesVector(rook_position, valid_moves_bb, move_list);
    }
}

void Game::GenerateQueenMoves(vector <Move>& move_list) {
    uint64_t queen_bitboard_copy = position.GetQueenBitBoard();
    while(queen_bitboard_copy) {
        uint8_t queen_position =
            EjectIndexFromBitboard(queen_bitboard_copy, single_index_bitboard_);
        uint64_t valid_moves_bb =
            GenerateValidStraightSlidingMovesBB(queen_position) |
            GenerateValidDiagonalSlidingMovesBB(queen_position);
        valid_moves_bb &= ~position.GetActiveSidesOccupiedSquaresBB();
        PushValidMovesBBToMovesVector(queen_position, valid_moves_bb, move_list); 
    }
}

void Game::GenerateKnightMoves(vector <Move>& move_list) {
    uint64_t knight_bb_copy = position.GetKnightsBitBoard();
    while(knight_bb_copy) {
        uint8_t knight_position =
            EjectIndexFromBitboard(knight_bb_copy, single_index_bitboard_);
        uint64_t valid_moves_bb = knight_attack_bitboard_lookup[knight_position];
        valid_moves_bb &= ~position.GetActiveSidesOccupiedSquaresBB();
        PushValidMovesBBToMovesVector(knight_position, valid_moves_bb, move_list); 
    }
}

/**
 * FUNCTION INCOMPLETE
 * Generate valid King moves including castling. Castling moves must check for:
 * - Does not move King through an attack
 * - Position castling permission flags allow the castle
 * - There are no pieces in the way of the castling move
 **/
void Game::GenerateKingMoves(vector <Move>& move_list) {
    uint64_t king_bb_copy = position.GetKingBitBoard();
    uint8_t king_position = lsb_scan(king_bb_copy);
    uint64_t valid_moves_bb =
        king_move_bitboard_lookup[position.GetSideToMove()][king_position];
    valid_moves_bb &= ~position.GetActiveSidesOccupiedSquaresBB();
    PushValidMovesBBToMovesVector(king_position, valid_moves_bb, move_list);
}

void Game::AddPawnMoveToMoveList(
    vector <Move>& move_list,
    uint64_t &valid_single_push_moves_bb,
    short originSquareDirectionAndDistanceFromDestination
    ) {
        uint8_t destination_position = lsb_scan(valid_single_push_moves_bb);
        valid_single_push_moves_bb ^= single_index_bitboard_[destination_position];
        Move move(destination_position + originSquareDirectionAndDistanceFromDestination, destination_position);
        move_list.push_back(move);
    }

/**
 * INCOMPLETE FUNCTION
 * Generate valid pawn moves. Special cases include:
 * - En passant captures [X]
 * - Promotion moves [ ]
 * This function differs from other move generation functions in that it calculates for all pawns
 * in parallel, rather than each piece serially.
 **/
void Game::GeneratePawnMoves(vector <Move>& move_list) {
    uint64_t pawn_bb_copy = position.GetPawnsBitBoard();
    uint64_t valid_single_push_moves_bb = pawn_bb_copy >> 8;
    uint64_t valid_double_push_pawn_moves_bb;
    uint64_t valid_pawn_attacks_shift_seven;
    uint64_t valid_pawn_attacks_shift_nine;
    short multiplier;
    if (position.GetSideToMove()) {
        valid_single_push_moves_bb = pawn_bb_copy >> 8;
        valid_single_push_moves_bb ^= (valid_single_push_moves_bb & position.GetAllOccupiedSquaresBitBoard());
        valid_double_push_pawn_moves_bb = (valid_single_push_moves_bb >> 8) & fourth_rank;
        valid_pawn_attacks_shift_seven = (pawn_bb_copy & ~h_file) >> 7;
        valid_pawn_attacks_shift_nine = (pawn_bb_copy & ~a_file) >> 9;
        multiplier = 1;
    } else {
        valid_single_push_moves_bb = pawn_bb_copy << 8;
        valid_single_push_moves_bb ^= (valid_single_push_moves_bb & position.GetAllOccupiedSquaresBitBoard());
        valid_double_push_pawn_moves_bb = (valid_single_push_moves_bb << 8) & fourth_rank;
        valid_pawn_attacks_shift_seven = (pawn_bb_copy & ~h_file) << 7;
        valid_pawn_attacks_shift_nine = (pawn_bb_copy & ~a_file) << 9;
        multiplier = -1;
    }

    while(valid_single_push_moves_bb) {
        // need to handle promotion logic here.
        AddPawnMoveToMoveList(move_list, valid_single_push_moves_bb, 8 * multiplier);
    }

    valid_double_push_pawn_moves_bb &= ~position.GetAllOccupiedSquaresBitBoard();
    while(valid_double_push_pawn_moves_bb) {
        AddPawnMoveToMoveList(move_list, valid_double_push_pawn_moves_bb, 16 * multiplier);
        move_list.back().SetDoublePawnPushFlag();
    }

    uint64_t squares_pawns_can_attack =
        position.GetOccupiedSquaresBitBoard(!position.GetSideToMove()) | position.GetEnPassanteBitBoard();
    valid_pawn_attacks_shift_seven &= squares_pawns_can_attack;
    while(valid_pawn_attacks_shift_seven) {
        AddPawnMoveToMoveList(move_list, valid_pawn_attacks_shift_seven, 7 * multiplier);
        move_list.back().SetCaptureFlag();
    }
    valid_pawn_attacks_shift_nine &= squares_pawns_can_attack;
    while(valid_pawn_attacks_shift_nine) {
        AddPawnMoveToMoveList(move_list, valid_pawn_attacks_shift_nine, 9 * multiplier);
        move_list.back().SetCaptureFlag();
    } 
}

uint64_t Game::GenerateValidMovesNorthBitboard(char index) {
    uint64_t bb_occupied_squares_overlap_with_north_array = 
        position.GetAllOccupiedSquaresBitBoard() &
        north_array_bitboard_lookup[index];    
    if(bb_occupied_squares_overlap_with_north_array) {
        char most_significant_bit = msb_scan(bb_occupied_squares_overlap_with_north_array);
        return (north_array_bitboard_lookup[index] ^
            north_array_bitboard_lookup[most_significant_bit]);
    }
    return north_array_bitboard_lookup[index];
}

uint64_t Game::GenerateValidMovesEastBitboard(char index) {
    uint64_t bb_occupied_squares_overlap_with_east_array = 
        position.GetAllOccupiedSquaresBitBoard() &
        east_array_bitboard_lookup[index];

    if(bb_occupied_squares_overlap_with_east_array) {
        char least_significant_bit = lsb_scan(bb_occupied_squares_overlap_with_east_array);
        return (east_array_bitboard_lookup[index] ^
            east_array_bitboard_lookup[least_significant_bit]);
    }
    return east_array_bitboard_lookup[index];
}

uint64_t Game::GenerateValidMovesSouthBitboard(char index) {
    uint64_t bb_occupied_squares_overlap_with_south_array = 
        position.GetAllOccupiedSquaresBitBoard() &
        south_array_bitboard_lookup[index];
    
    if(bb_occupied_squares_overlap_with_south_array) {
        char least_significant_bit = lsb_scan(bb_occupied_squares_overlap_with_south_array);
        return (south_array_bitboard_lookup[index] ^
            south_array_bitboard_lookup[least_significant_bit]);
    }
    return south_array_bitboard_lookup[index];
}

uint64_t Game::GenerateValidMovesWestBitboard(char index) {
    uint64_t bb_occupied_squares_overlap_with_west_array = 
        position.GetAllOccupiedSquaresBitBoard() &
        west_array_bitboard_lookup[index];
    
    if(bb_occupied_squares_overlap_with_west_array) {
        char most_significant_bit = msb_scan(bb_occupied_squares_overlap_with_west_array);
        return (west_array_bitboard_lookup[index] ^
            west_array_bitboard_lookup[most_significant_bit]);
    }
    return west_array_bitboard_lookup[index];
}

uint64_t Game::GenerateValidMovesNorthEastBitboard(char index) {
    uint64_t bb_occupied_squares_overlap_with_northeast_array = 
        position.GetAllOccupiedSquaresBitBoard() &
        north_east_array_bitboard_lookup[index];

    if(bb_occupied_squares_overlap_with_northeast_array) {
        char most_significant_bit = msb_scan(bb_occupied_squares_overlap_with_northeast_array);
        return (north_east_array_bitboard_lookup[index] ^
            north_east_array_bitboard_lookup[most_significant_bit]);
    }
    return north_east_array_bitboard_lookup[index];
}

uint64_t Game::GenerateValidMovesNorthWestBitboard(char index) {
    uint64_t bb_occupied_squares_overlap_with_northwest_array = 
        position.GetAllOccupiedSquaresBitBoard() &
        north_west_array_bitboard_lookup[index];
    
    if(bb_occupied_squares_overlap_with_northwest_array) {
        char most_significant_bit = msb_scan(bb_occupied_squares_overlap_with_northwest_array);
        return (north_west_array_bitboard_lookup[index] ^
            north_west_array_bitboard_lookup[most_significant_bit]);
    }
    return north_west_array_bitboard_lookup[index];
}

uint64_t Game::GenerateValidMovesSouthEastBitboard(char index) {
    uint64_t bb_occupied_squares_overlap_with_southeast_array = 
        position.GetAllOccupiedSquaresBitBoard() &
        south_east_array_bitboard_lookup[index];
    
    if(bb_occupied_squares_overlap_with_southeast_array) {
        char least_significant_bit = lsb_scan(bb_occupied_squares_overlap_with_southeast_array);
        return (south_east_array_bitboard_lookup[index] ^
            south_east_array_bitboard_lookup[least_significant_bit]);
    }
    return south_east_array_bitboard_lookup[index];
}

uint64_t Game::GenerateValidMovesSouthWestBitboard(char index) {
    uint64_t bb_occupied_squares_overlap_with_southwest_array = 
        position.GetAllOccupiedSquaresBitBoard() &
        south_west_array_bitboard_lookup[index];
    
    if(bb_occupied_squares_overlap_with_southwest_array) {
        char least_significant_bit = lsb_scan(bb_occupied_squares_overlap_with_southwest_array);
        return (south_west_array_bitboard_lookup[index] ^
            south_west_array_bitboard_lookup[least_significant_bit]);
    }
    return south_west_array_bitboard_lookup[index];
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

// uint64_t Game::PerformanceTest(short depth) {
//     short current_depth = 0;
//     uint64_t total_number_of_potential_moves = 0ULL;
//     while (current_depth < depth) {
//         GenerateMoves(potential_moves_[current_depth]);
//         total_number_of_potential_moves += potential_moves_[current_depth].size();
//         current_depth++;
//     }
// }