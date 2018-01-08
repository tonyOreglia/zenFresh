#include "game.h"

Game::Game(Position position) {
    this->position = position;
    bitboard_lookup_ = position.bitboard_lookup;
    potential_moves_.reserve(DEPTH);
    for (char i = 0; i < DEPTH; i++) {
        potential_moves_[i].reserve(MAX_MOVES_FROM_SINGLE_POSITION);
    }
}

void Game::GenerateMoves() {
    // Generate rook moves
    char j=0;
    uint64_t rook_bitboard_copy = position.GetRooksBitBoard();


    while(rook_bitboard_copy) {
        char rook_position = lsb_scan(rook_bitboard_copy);
        rook_bitboard_copy ^= position.bitboard_lookup.single_index_bitboard_[rook_position];

        uint64_t valid_attack_bitboard = GenerateValidMovesNorthBitboard(rook_position) |
            GenerateValidMovesEastBitboard(rook_position) |
            GenerateValidMovesSouthBitboard(rook_position) |
            GenerateValidMovesWestBitboard(rook_position);
        valid_attack_bitboard &= ~position.GetActiveSidesOccupiedSquaresBB();

       
        while(valid_attack_set) {
            char destination_index = lsb_scan(valid_attack_set);
            valid_attack_set ^= position.bitboard_lookup.single_index_bitboard_[destination_index];

            
            push_single_move(sigBit, sq, position->rooks+position->side);


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

uint64_t Game::GenerateValidMovesNortEasthBitboard(char index) {
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

uint64_t Game::GenerateValidMovesNortWesthBitboard(char index) {
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
            bitboard_lookup_.south_west_array_bitboard_lookup[most_significant_bit]);
    }
    return bitboard_lookup_.south_west_array_bitboard_lookup[index];
}
