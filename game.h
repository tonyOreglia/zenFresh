#ifndef GAME_H
#define GAME_H

#include <vector>

#include "move.h"
#include "position.h"
#include "./bitboard_utilities/bitboard-utilities.h"
#include "./bitboard_utilities/generate-bitboard-lookup-tables.h"

#define DEPTH 8
#define MAX_MOVES_FROM_SINGLE_POSITION 100

using namespace std; 

class Game {
public:
    Game(Position position);
    void GenerateMoves(vector <Move>& move_list);
    void GenerateRookMoves(vector <Move>& move_list);
    void GenerateBishopMoves(vector <Move>& move_list);
    void GenerateKnightMoves(vector <Move>& move_list);
    void GenerateKingMoves(vector <Move>& move_list);
    void GenerateQueenMoves(vector <Move>& move_list);
    void GenerateWhitePawnMoves(vector <Move>& move_list);
    void GenerateBlackPawnMoves(vector <Move>& move_list);
    void PushSingleMoveFromValidMovesBBToMovesVector(
        uint8_t origin,
        uint64_t &valid_moves_bb,
        vector <Move>& move_list);
    uint64_t GenerateValidMovesNorthBitboard(char index);
    uint64_t GenerateValidMovesEastBitboard(char index);
    uint64_t GenerateValidMovesSouthBitboard(char index);
    uint64_t GenerateValidMovesWestBitboard(char index);
    uint64_t GenerateValidMovesNorthEastBitboard(char index);
    uint64_t GenerateValidMovesNorthWestBitboard(char index);
    uint64_t GenerateValidMovesSouthEastBitboard(char index);
    uint64_t GenerateValidMovesSouthWestBitboard(char index);
    uint64_t GenerateValidDiagonalSlidingMovesBB(char index);
    uint64_t GenerateValidStraightSlidingMovesBB(char index);
private:
    Position position;
    BitBoardLookupTables& bitboard_lookup_;
    vector <Move> game_history;
    vector <Move> *potential_moves_;

};

#endif
