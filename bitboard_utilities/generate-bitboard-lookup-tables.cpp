#include "generate-bitboard-lookup-tables.h"

uint64_t a_file = 0x101010101010101ULL;
uint64_t b_file = a_file << 1;
uint64_t c_file = a_file << 2;
uint64_t d_file = a_file << 3;
uint64_t e_file = a_file << 4;
uint64_t f_file = a_file << 5;
uint64_t g_file = a_file << 6;
uint64_t h_file = a_file << 7;

uint64_t single_index_bitboard_[64];
uint64_t en_passant_location_bb_after_double_pawn_push_bb[64];
uint64_t attacked_pawn_location_for_en_passant_capture[64];
uint64_t north_array_bitboard_lookup[64];
uint64_t south_array_bitboard_lookup[64];
uint64_t east_array_bitboard_lookup[64];
uint64_t west_array_bitboard_lookup[64];
uint64_t north_east_array_bitboard_lookup[64];
uint64_t north_west_array_bitboard_lookup[64];
uint64_t south_east_array_bitboard_lookup[64];
uint64_t south_west_array_bitboard_lookup[64];
uint64_t knight_attack_bitboard_lookup[64];
uint64_t king_move_bitboard_lookup[2][64]; // one for white one for black   
uint64_t pawn_moves_bitboard_lookup[2][64];

uint64_t fourth_rank = 0xFF00000000ULL;
uint64_t fifth_rank = 0xFF000000ULL;

void calculateAllLookupBbs() {

    for (int index = 0; index < 64; index++) {
        en_passant_location_bb_after_double_pawn_push_bb[index] = 0ULL;
        attacked_pawn_location_for_en_passant_capture[index] = 0ULL;
        north_array_bitboard_lookup[index] = 0ULL;
        south_array_bitboard_lookup[index] = 0ULL;
        east_array_bitboard_lookup[index] = 0ULL;
        west_array_bitboard_lookup[index] = 0ULL;
        north_east_array_bitboard_lookup[index] = 0ULL;
        north_west_array_bitboard_lookup[index] = 0ULL;
        south_east_array_bitboard_lookup[index] = 0ULL;
        south_west_array_bitboard_lookup[index] = 0ULL;
        knight_attack_bitboard_lookup[index] = 0ULL;
    }
    generateSingleBitLookup();
    generateArrayBitboardLookup();
    generateEnPassantBitboardLookup();
    // PrintAllBitboards();
}

void generateSingleBitLookup() {
    for (int i=0; i<64; i++) {
        single_index_bitboard_[i] = 1ULL << i;
    }
}

void generateEnPassantBitboardLookup() {
    for (char i=0; i<64; i++) {
        en_passant_location_bb_after_double_pawn_push_bb[i] = 0ULL;
        attacked_pawn_location_for_en_passant_capture[i] = 0ULL;
        if (i > 23 && i < 32) {
            en_passant_location_bb_after_double_pawn_push_bb[i] = single_index_bitboard_[i - 8];
        }
        else if (i > 31 && i < 40) {
            en_passant_location_bb_after_double_pawn_push_bb[i] = single_index_bitboard_[i + 8];
        } else if (i > 15 && i < 24) {
            attacked_pawn_location_for_en_passant_capture[i] = single_index_bitboard_[i + 8];
        } else if (i > 39 && i < 48) {
            attacked_pawn_location_for_en_passant_capture[i] = single_index_bitboard_[i - 8];
        }
    }
}

void generateArrayBitboardLookup() {
    for (int index=0; index<64; index++) {
        int north_of_index = index;
        while (north_of_index > 7) {
            north_array_bitboard_lookup[index] |= single_index_bitboard_[north_of_index - 8];
            north_of_index -= 8;
        }
        int south_of_index = index;
        while (south_of_index < 56) {
            south_array_bitboard_lookup[index] |= single_index_bitboard_[south_of_index + 8];
            south_of_index += 8;
        }
        int east_of_index = index;
        while (!((single_index_bitboard_[east_of_index]) & h_file)) {
            east_array_bitboard_lookup[index] |= single_index_bitboard_[east_of_index + 1];
            east_of_index += 1;
        }
        int west_of_index = index;
        while (!((single_index_bitboard_[west_of_index]) & a_file)) {
            west_array_bitboard_lookup[index] |= single_index_bitboard_[west_of_index - 1];
            west_of_index -= 1;
        }
        int north_east_of_index = index;
        while(!((single_index_bitboard_[north_east_of_index]) & h_file) && north_east_of_index > 7) {
            north_east_array_bitboard_lookup[index] |= single_index_bitboard_[north_east_of_index - 7];
            north_east_of_index -= 7;
        }
        int north_west_of_index = index;
        while(!((single_index_bitboard_[north_west_of_index]) & a_file) && north_west_of_index > 8) {
            north_west_array_bitboard_lookup[index] |= single_index_bitboard_[north_west_of_index - 9];
            north_west_of_index -= 9;
        }
        int south_east_of_index = index;
        while(!((single_index_bitboard_[south_east_of_index]) & h_file) && south_east_of_index < 56) {
            south_east_array_bitboard_lookup[index] |= single_index_bitboard_[south_east_of_index + 9];
            south_east_of_index += 9;
        }
        int south_west_of_index = index;
        while(!((single_index_bitboard_[south_west_of_index]) & a_file) && south_west_of_index < 56) {
            south_west_array_bitboard_lookup[index] |= single_index_bitboard_[south_west_of_index + 7];
            south_west_of_index += 7;
        }
        if((index + 10) < 64 && !(single_index_bitboard_[index] & (h_file | g_file)))
            { knight_attack_bitboard_lookup[index] |= single_index_bitboard_[index + 10]; }
        if((index + 6) < 64 && !(single_index_bitboard_[index] & (a_file | b_file)))
            { knight_attack_bitboard_lookup[index] |= single_index_bitboard_[index + 6]; }
        if((index + 17) < 64 && !(single_index_bitboard_[index] & h_file))  
            { knight_attack_bitboard_lookup[index] |= single_index_bitboard_[index + 17]; }
        if((index + 15) < 64 && !(single_index_bitboard_[index] & a_file)) 
            { knight_attack_bitboard_lookup[index] |= single_index_bitboard_[index + 15]; }
        if( (index - 10) >= 0 && !(single_index_bitboard_[index] & (a_file | b_file)))
            { knight_attack_bitboard_lookup[index] |= single_index_bitboard_[index - 10]; }
        if( (index - 6) >= 0 && !(single_index_bitboard_[index] & (h_file | g_file)))
            { knight_attack_bitboard_lookup[index] |= single_index_bitboard_[index - 6]; }
        if( (index - 17) >= 0 && !(single_index_bitboard_[index] & a_file)) 
            { knight_attack_bitboard_lookup[index] |= single_index_bitboard_[index - 17]; }
        if( (index - 15) >= 0 && !(single_index_bitboard_[index] & h_file))
            { knight_attack_bitboard_lookup[index] |= single_index_bitboard_[index - 15]; }
        
        king_move_bitboard_lookup[0][index] = 0ULL;
        if (index != 63) king_move_bitboard_lookup[0][index] |= single_index_bitboard_[index + 1];
        if (index != 0) king_move_bitboard_lookup[0][index] |= single_index_bitboard_[index - 1];
        if (index <= 55) king_move_bitboard_lookup[0][index] |= single_index_bitboard_[index + 8];
        if (index >= 8) king_move_bitboard_lookup[0][index] |= single_index_bitboard_[index - 8];
        if (index <= 54) king_move_bitboard_lookup[0][index] |= single_index_bitboard_[index + 9];
        if (index >= 9) king_move_bitboard_lookup[0][index] |= single_index_bitboard_[index - 9];
        if (index <= 56) king_move_bitboard_lookup[0][index] |= single_index_bitboard_[index + 7];
        if (index >= 7) king_move_bitboard_lookup[0][index] |= single_index_bitboard_[index - 7];
        king_move_bitboard_lookup[1][index] = king_move_bitboard_lookup[0][index];
        if (single_index_bitboard_[index] & a_file) {
            king_move_bitboard_lookup[0][index] &= ~h_file;
            king_move_bitboard_lookup[1][index] &= ~h_file;
            }
        if (single_index_bitboard_[index] & h_file) {
            king_move_bitboard_lookup[0][index] &= ~a_file; 
            king_move_bitboard_lookup[1][index] &= ~a_file; 
            }
        if (index == 4) {
            king_move_bitboard_lookup[0][4] |= single_index_bitboard_[2] |
                single_index_bitboard_[6];
            }
        if (index == 60) {
            king_move_bitboard_lookup[1][60] |= single_index_bitboard_[62] |
                single_index_bitboard_[58];
            }

        // PAWN MOVE
        pawn_moves_bitboard_lookup[0][index] = 0ULL;
        pawn_moves_bitboard_lookup[1][index] = 0ULL;

        if (index <= 56) pawn_moves_bitboard_lookup[0][index] |= single_index_bitboard_[index + 7];
        if (index <= 55) pawn_moves_bitboard_lookup[0][index] |= single_index_bitboard_[index + 8];
        if (index <= 54) pawn_moves_bitboard_lookup[0][index] |= single_index_bitboard_[index + 9];

        if (index >= 7) pawn_moves_bitboard_lookup[1][index] |= single_index_bitboard_[index - 7];
        if (index >= 8) pawn_moves_bitboard_lookup[1][index] |= single_index_bitboard_[index - 8];
        if (index >= 9) pawn_moves_bitboard_lookup[1][index] |= single_index_bitboard_[index - 9];
       
        if (index > 7 && index < 16) {
            pawn_moves_bitboard_lookup[0][index] |= single_index_bitboard_[index + 16];
        }
        if (index > 47 && index < 56) {
            pawn_moves_bitboard_lookup[1][index] |= single_index_bitboard_[index - 16];
        }
        if (single_index_bitboard_[index] & a_file) {
            pawn_moves_bitboard_lookup[0][index] &= ~h_file;
            pawn_moves_bitboard_lookup[1][index] &= ~h_file;

        }
        if (single_index_bitboard_[index] & h_file) {
            pawn_moves_bitboard_lookup[0][index] &= ~a_file; 
            pawn_moves_bitboard_lookup[1][index] &= ~a_file; 
        }
    }
}

void printAllBitboards() {
    cout << "\tA FILE\n\n";
    printBitBoard(a_file);
    cout << "\tB FILE\n\n";
    printBitBoard(b_file);
    cout << "\tC FILE\n\n";
    printBitBoard(c_file);
    cout << "\tD FILE\n\n";
    printBitBoard(d_file);
    cout << "\tE FILE\n\n";
    printBitBoard(e_file);
    cout << "\tF FILE\n\n";
    printBitBoard(f_file);
    cout << "\tG FILE\n\n";
    printBitBoard(g_file);
    cout << "\tH FILE\n\n";
    printBitBoard(h_file);
    for (char i=0; i<64; i++) {
        cout << "\tBITBOARD LOOKUP\n\n";
        printBitBoard(single_index_bitboard_[i]);
        cout << "\tNORTH\n\n";
        printBitBoard(north_array_bitboard_lookup[i]);
        cout << "\tSOUTH\n\n";
        printBitBoard(south_array_bitboard_lookup[i]);
        cout << "\tEAST\n\n";
        printBitBoard(east_array_bitboard_lookup[i]);
        cout << "\tWEST\n\n";
        printBitBoard(west_array_bitboard_lookup[i]);
        cout << "\tNORTH EAST\n\n";
        printBitBoard(north_east_array_bitboard_lookup[i]);
        cout << "\tNORTH WEST\n\n";
        printBitBoard(north_west_array_bitboard_lookup[i]);
        cout << "\tSOUTH EAST\n\n";
        printBitBoard(south_east_array_bitboard_lookup[i]);
        cout << "\tSOUTH WEST\n\n";
        printBitBoard(south_west_array_bitboard_lookup[i]);
        cout << "\tKNIGHT ATTACK\n\n";
        printBitBoard(knight_attack_bitboard_lookup[i]);
        cout << "\tKING MOVES\n\n";
        printBitBoard(king_move_bitboard_lookup[0][i]);
        printBitBoard(king_move_bitboard_lookup[1][i]);
        cout << "\tPAWN MOVES\n\n";
        printBitBoard(pawn_moves_bitboard_lookup[0][i]);
        printBitBoard(pawn_moves_bitboard_lookup[1][i]);
        cout << "\tEN PASSANT BB LOOKUP BY PAWN DESTINATION: " << (int)i << endl;
        printBitBoard(en_passant_location_bb_after_double_pawn_push_bb[i]);
        cout << "\tATTACKED PAWN LOCATION BY EN PASSANT CAPTURE: " << (int)i << endl;
        printBitBoard(attacked_pawn_location_for_en_passant_capture[i]);
    }
}

void printBitBoard(uint64_t bitboard) {
    bitset<64> bb (bitboard);
    for(int i=0;i<64;i++) {
        cout << bb[i];
        if(((i+1) % 8) == 0) 
            cout << endl;
    }
    cout << endl;
    cout << endl;
}