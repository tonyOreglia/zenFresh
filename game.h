#ifndef GAME_H
#define GAME_H

#include <vector>

#include "move.h"
#include "position.h"
#include "./bitboard_utilities/bitboard-utilities.cpp"
#include "./bitboard_utilities/generate-bitboard-lookup-tables.h"

#define DEPTH 8
#define MAX_MOVES_FROM_SINGLE_POSITION 100

using namespace std; 

class Game {
public:
    Game(Position position);
    void GenerateMoves();
    uint64_t GenerateValidMovesNorthBitboard(char index);
    uint64_t GenerateValidMovesEastBitboard(char index);
    uint64_t GenerateValidMovesSouthBitboard(char index);
    uint64_t GenerateValidMovesWestBitboard(char index);
    uint64_t GenerateValidMovesNortEasthBitboard(char index);
    uint64_t GenerateValidMovesNortWesthBitboard(char index);
    uint64_t GenerateValidMovesSouthEastBitboard(char index);
    uint64_t GenerateValidMovesSouthWestBitboard(char index);
private:
    Position position;
    BitBoardLookupTables& bitboard_lookup_;
    vector <Move> game_history;
    vector <vector <Move>> potential_moves_;

};

#endif
