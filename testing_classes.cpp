#include <cstdint>
#include <bitset>
#include <iostream>
#include "position.h"
#include "move.h"

using namespace std;

void PrintBitBoard(uint64_t bitboard);
void PrintPosition(Position& position);
void PrintMove(Move& move);
void PrintHumanReadableBoard(Position& position);
unsigned lsb_scan(uint64_t bitboard);
unsigned msb_scan(uint64_t bitboard);
void storeBitboardInPiecesArray(char *pieces, uint64_t bitboard, char pieceCharacter);
void PrintHumanReadableBoardFromPiecesArray(char *pieces);

int main(int argc, char *argv[]) {
    Position position;
    PrintHumanReadableBoard(position);
    Move move;
    move.SetOriginSquare(E2);
    move.SetDestinationSquare(E4);
    move.SetDoublePawnPushFlag();
    position.UpdatePositionWithSingleMove(move);
    PrintHumanReadableBoard(position);
    return 0;
}

void PrintMove(Move& move) {
    cout << "origin square: " << move.GetOriginSquare() << endl;
    cout << "destination square: " << move.GetDestinationSquare() << endl;
    if (move.IsDoublePawnPush()) {
        cout << "This is a double pawn push\n";
    }
}



void PrintBitBoard(uint64_t bitboard) {
    bitset<64> bb (bitboard);
    for(int i=0;i<64;i++) {
        cout << bb[i];
        if(((i+1) % 8) == 0) 
            cout << endl;
    }
    cout << endl;
    cout << endl;
}

void storeBitboardInPiecesArray(char *pieces, uint64_t bitboard, char pieceCharacter) {
    while (bitboard) {
        short bit_position = lsb_scan(bitboard);
        pieces[bit_position] = pieceCharacter;
        bitboard ^= 1ULL << bit_position;
    }
}

void PrintHumanReadableBoardFromPiecesArray(char *pieces) {
    cout << "\t\t";
    for (int i=0; i<64; i++) {
        cout << pieces[i] << "  ";
        if (!((i+1) % 8)) {
            cout << "\n\t\t";
        }
    }
    cout << "________________________";
    cout << "\n\t\ta  b  c  d  e  f  g  h\n\n";
}

void PrintHumanReadableBoard(Position& position) {
    char pieces[64] = { [0 ... 63] = '-' };

    storeBitboardInPiecesArray(pieces, position.GetPawnsBitBoard(WHITE), 'P');
    storeBitboardInPiecesArray(pieces, position.GetRooksBitBoard(WHITE), 'R');
    storeBitboardInPiecesArray(pieces, position.GetBishopsBitBoard(WHITE), 'B');
    storeBitboardInPiecesArray(pieces, position.GetKingBitBoard(WHITE), 'K');
    storeBitboardInPiecesArray(pieces, position.GetKnightsBitBoard(WHITE), 'N');
    storeBitboardInPiecesArray(pieces, position.GetQueenBitBoard(WHITE), 'Q');

    storeBitboardInPiecesArray(pieces, position.GetPawnsBitBoard(BLACK), 'p');
    storeBitboardInPiecesArray(pieces, position.GetRooksBitBoard(BLACK), 'r');
    storeBitboardInPiecesArray(pieces, position.GetBishopsBitBoard(BLACK), 'b');
    storeBitboardInPiecesArray(pieces, position.GetKingBitBoard(BLACK), 'k');
    storeBitboardInPiecesArray(pieces, position.GetKnightsBitBoard(BLACK), 'n');
    storeBitboardInPiecesArray(pieces, position.GetQueenBitBoard(BLACK), 'q');

    PrintHumanReadableBoardFromPiecesArray(pieces);
}


unsigned lsb_scan(uint64_t bitboard){
    return __builtin_ctzll(bitboard);
}

unsigned msb_scan(uint64_t bitboard){
    return 63 - __builtin_clzll(bitboard);
}