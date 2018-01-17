/*
    Tony's Trash Cheese Bot
*/

#ifndef POSITION_H
#define POSITION_H

#include <cstdint>
#include <bitset>
#include <iostream>
#include <stack>

#include "move.h"
#include "./bitboard_utilities/generate-bitboard-lookup-tables.h"

#define WHITE 1
#define BLACK 0
#define KING_SIDE_CASTLE 0
#define QUEEN_SIDE_CASTLE 1

using namespace std;

enum {
        A8=0, B8, C8, D8, E8, F8, G8, H8,
        A7, B7, C7, D7, E7, F7, G7, H7,
        A6, B6, C6, D6, E6, F6, G6, H6,
        A5, B5, C5, D5, E5, F5, G5, H5,
        A4, B4, C4, D4, E4, F4, G4, H4,
        A3, B3, C3, D3, E3, F3, G3, H3,
        A2, B2, C2, D2, E2, F2, G2, H2,
        A1, B1, C1, D1, E1, F1, G1, H1
    };

/**
 * Store all information associated with a single chess position.
*/
class Position
{
private:
    uint64_t UpdateAggregateBitboardsFromPieceBitboards();
    uint64_t UpdateAllWhiteOccupiedSquaresBitBoard();
    uint64_t UpdateAllBlackOccupiedSquaresBitBoard();
    bool castling_rights_[2][2];
    uint8_t half_move_count_;
    short int full_move_count_;
    bool side_to_move_; // using defined constants WHITE (true) or BLACK (false)
    struct PieceBitboards {
            uint64_t occupied_squares;
            uint64_t king;
            uint64_t queen;
            uint64_t rooks;
            uint64_t knights;
            uint64_t bishops;
            uint64_t pawns;
        };
    struct BitBoard {
        uint64_t en_passante;
        uint64_t all_occupied_squares;
        PieceBitboards piece_bitboards [2];
    } bitboard_;
    std::stack<uint64_t*> captured_pieces;
public:
    Position() : Position(
        (char *)"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", (char *)"w", (char *)"KQkq", (char *)"-", (char *)"0", (char *)"1" 
    ) {}
    Position(char *fen, char *side_to_move, char *castling_rights, char *en_passant, char *fifty_move, char *full_moves);
    void SetStartingPosition();
    void ClearPosition();
    void UpdatePositionWithSingleMove(Move& move);
    void UpdateMovingPieceBitboardWithSingleMove(uint64_t origin_bitboard, uint64_t destination_bitboard);
    uint64_t* GetPieceBitboardBasedOnBoardLocation(uint64_t board_location_bitboard, bool side_to_check);
    void ToggleBitboardBits(uint64_t& piece_bitboard, uint64_t toggle_positions);

    uint64_t GetAllOccupiedSquaresBitBoard() {return bitboard_.all_occupied_squares;}
    uint64_t GetEnPassanteBitBoard() {return bitboard_.en_passante;}
    uint64_t GetOccupiedSquaresBitBoard(bool side_to_move) {return bitboard_.piece_bitboards[side_to_move].occupied_squares;}
    uint64_t GetKingBitBoard(bool side_to_move) {return bitboard_.piece_bitboards[side_to_move].king;}
    uint64_t GetQueenBitBoard(bool side_to_move) {return bitboard_.piece_bitboards[side_to_move].queen;}
    uint64_t GetRooksBitBoard(bool side_to_move) {return bitboard_.piece_bitboards[side_to_move].rooks;}
    uint64_t GetKnightsBitBoard(bool side_to_move) {return bitboard_.piece_bitboards[side_to_move].knights;}
    uint64_t GetBishopsBitBoard(bool side_to_move) {return bitboard_.piece_bitboards[side_to_move].bishops;}
    uint64_t GetPawnsBitBoard(bool side_to_move) {return bitboard_.piece_bitboards[side_to_move].pawns;}

    uint64_t GetActiveSidesOccupiedSquaresBB() {return bitboard_.piece_bitboards[side_to_move_].occupied_squares;}
    uint64_t GetKingBitBoard() {return bitboard_.piece_bitboards[side_to_move_].king;}
    uint64_t GetQueenBitBoard() {return bitboard_.piece_bitboards[side_to_move_].queen;}
    uint64_t GetRooksBitBoard() {return bitboard_.piece_bitboards[side_to_move_].rooks;}
    uint64_t GetKnightsBitBoard() {return bitboard_.piece_bitboards[side_to_move_].knights;}
    uint64_t GetBishopsBitBoard() {return bitboard_.piece_bitboards[side_to_move_].bishops;}
    uint64_t GetPawnsBitBoard() {return bitboard_.piece_bitboards[side_to_move_].pawns;}

    void SetKingBitBoard(bool side_to_move, uint64_t new_bitboard) { bitboard_.piece_bitboards[side_to_move].king = new_bitboard; }
    void SetQueenBitBoard(bool side_to_move, uint64_t new_bitboard) { bitboard_.piece_bitboards[side_to_move].queen = new_bitboard; }
    void SetRooksBitBoard(bool side_to_move, uint64_t new_bitboard) { bitboard_.piece_bitboards[side_to_move].rooks = new_bitboard; }
    void SetKnightsBitBoard(bool side_to_move, uint64_t new_bitboard) { bitboard_.piece_bitboards[side_to_move].knights = new_bitboard; }
    void SetBishopsBitBoard(bool side_to_move, uint64_t new_bitboard) { bitboard_.piece_bitboards[side_to_move].bishops = new_bitboard; }
    void SetPawnsBitBoard(bool side_to_move, uint64_t new_bitboard) { bitboard_.piece_bitboards[side_to_move].pawns = new_bitboard; }
    void SetEnPassantBitBoard(uint64_t en_passant_bb) { bitboard_.en_passante = en_passant_bb; }

    void AddPieceToKingBitBoard(bool side_to_move, uint64_t new_bitboard) { bitboard_.piece_bitboards[side_to_move].king |= new_bitboard; }
    void AddPieceToQueenBitBoard(bool side_to_move, uint64_t new_bitboard) { bitboard_.piece_bitboards[side_to_move].queen |= new_bitboard; }
    void AddPieceToRooksBitBoard(bool side_to_move, uint64_t new_bitboard) { bitboard_.piece_bitboards[side_to_move].rooks |= new_bitboard; }
    void AddPieceToKnightsBitBoard(bool side_to_move, uint64_t new_bitboard) { bitboard_.piece_bitboards[side_to_move].knights |= new_bitboard; }
    void AddPieceToBishopsBitBoard(bool side_to_move, uint64_t new_bitboard) { bitboard_.piece_bitboards[side_to_move].bishops |= new_bitboard; }
    void AddPieceToPawnsBitBoard(bool side_to_move, uint64_t new_bitboard) { bitboard_.piece_bitboards[side_to_move].pawns |= new_bitboard; }
    
    bool GetSideToMove() {return side_to_move_;}
    void SetSideToMove(bool side_to_move) { side_to_move_ = side_to_move; }
    uint8_t GetHalfMoveCount() {return half_move_count_;}
    short int GetFullMoveCount() {return full_move_count_;}

    bool WhiteCanCastleKingSide() { return castling_rights_[WHITE][KING_SIDE_CASTLE]; }
    bool WhiteCanCastleQueenSide() { return castling_rights_[WHITE][QUEEN_SIDE_CASTLE]; }
    bool BlackCanCastleKingSide() { return castling_rights_[BLACK][KING_SIDE_CASTLE]; }
    bool BlackCanCastleQueenSide() { return castling_rights_[BLACK][QUEEN_SIDE_CASTLE]; }

    void SetWhiteCanCastleKingSide(bool castle_flag) { castling_rights_[WHITE][KING_SIDE_CASTLE] = castle_flag; }
    void SetWhiteCanCastleQueenSide(bool castle_flag) { castling_rights_[WHITE][QUEEN_SIDE_CASTLE] = castle_flag; }
    void SetBlackCanCastleKingSide(bool castle_flag) { castling_rights_[BLACK][KING_SIDE_CASTLE] = castle_flag; }
    void SetBlackCanCastleQueenSide(bool castle_flag) { castling_rights_[BLACK][QUEEN_SIDE_CASTLE] = castle_flag; }

    void SetHalfMoveCount(short half_move_count) { half_move_count_ = half_move_count; }
    void SetFullMoveCount(short full_move_count) { full_move_count_ = full_move_count; }

    void PrintBitBoard(uint64_t);
    BitBoardLookupTables bitboard_lookup;
};

#endif
