#ifndef GAME_H
#define GAME_H

#include <vector>

#include "move.h"
#include "position.h"
#include "./bitboard_utilities/bitboard-utilities.h"
#include "./bitboard_utilities/generate-bitboard-lookup-tables.h"

#define DEPTH 8
#define MAX_MOVES_FROM_SINGLE_POSITION 100
#define WHITE_KING_SIDE_CASTLE_DESTINATION 62
#define WHITE_KING_SIDE_CASTLE_SLIDE_ACROSS_SQUARE 61
#define WHITE_QUEEN_SIDE_CASTLE_DESTINATION 58
#define WHITE_QUEEN_SIDE_CASTLE_SLIDE_ACROSS_SQUARE 59
#define BLACK_KING_SIDE_CASTLE_DESTINATION 6
#define BLACK_KING_SIDE_CASTLE_SLIDE_ACROSS_SQUARE 5
#define BLACK_QUEEN_SIDE_CASTLE_DESTINATION 2
#define BLACK_QUEEN_SIDE_CASTLE_SLIDE_ACROSS_SQUARE 3

using namespace std; 

class Game {
public:
    Game(Position position);
    void GenerateMoves(vector <Move>& move_list);
    void GenerateRookMoves(vector <Move>& move_list);
    void GenerateBishopMoves(vector <Move>& move_list);
    void GenerateKnightMoves(vector <Move>& move_list);
    void GenerateKingMoves(vector <Move>& move_list);
    bool CheckIfCastlingMoveIsValid(Move move);
    bool CheckIfAnyMoveAttacksSpecificLocation(short square_to_check);
    void GenerateQueenMoves(vector <Move>& move_list);
    void GeneratePawnMoves(vector <Move>& move_list);
    void AddPawnMoveToMoveList(vector <Move>& move_list, uint64_t &valid_moves_bb, short originSquareDirectionAndDistanceFromDestination);
    void PushSingleMoveFromValidMovesBBToMovesVector(short origin, uint64_t &valid_moves_bb, vector <Move>& move_list);
    void AddPawnPromotionMovesToMoveList(vector <Move>& move_list);
    void PushValidMovesBBToMovesVector(short origin, uint64_t &valid_moves_bb, vector <Move>& move_list);
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
    uint64_t PerformanceTest(short depth);
    vector <Move> &GetMovesVector(short depth) { return potential_moves_[depth]; }
private:
    Position position;
    vector <Move> game_history;
    vector <Move> *potential_moves_;
};
#endif
