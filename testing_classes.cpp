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

int main(int argc, char *argv[]) {
    Position position;
    PrintHumanReadableBoard(position);
    Move move;
    move.SetOriginSquare(E2);
    move.SetDestinationSquare(E4);
    move.SetDoublePawnPushFlag();
    PrintMove(move);
    return 0;
}

void PrintMove(Move& move) {
    cout << "origin square: " << move.GetOriginSquare() << endl;
    cout << "destination square: " << move.GetDestinationSquare() << endl;
    if (move.GetDoublePawnPushFlag()) {
        cout << "This is a doulbe pawn push\n";
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

    storeBitboardInPiecesArray(pieces, position.GetWhitePawnsBitBoard(), 'P');
    storeBitboardInPiecesArray(pieces, position.GetWhiteRooksBitBoard(), 'R');
    storeBitboardInPiecesArray(pieces, position.GetWhiteBishopsBitBoard(), 'B');
    storeBitboardInPiecesArray(pieces, position.GetWhiteKingBitBoard(), 'K');
    storeBitboardInPiecesArray(pieces, position.GetWhiteKnightsBitBoard(), 'N');
    storeBitboardInPiecesArray(pieces, position.GetWhiteQueenBitBoard(), 'Q');

    storeBitboardInPiecesArray(pieces, position.GetBlackPawnsBitBoard(), 'p');
    storeBitboardInPiecesArray(pieces, position.GetBlackRooksBitBoard(), 'r');
    storeBitboardInPiecesArray(pieces, position.GetBlackBishopsBitBoard(), 'b');
    storeBitboardInPiecesArray(pieces, position.GetBlackKingBitBoard(), 'k');
    storeBitboardInPiecesArray(pieces, position.GetBlackKnightsBitBoard(), 'n');
    storeBitboardInPiecesArray(pieces, position.GetBlackQueenBitBoard(), 'q');

    PrintHumanReadableBoardFromPiecesArray(pieces);
}

void PrintPosition(Position& position) {
    cout << "\nAll occupied square:\n";
    PrintBitBoard(position.GetAllOccupiedSquaresBitBoard());
    cout << "\nAll white occupied square:\n";
    PrintBitBoard(position.GetWhiteOccupiedSquaresBitBoard());
    cout << "\nAll black occupied square:\n";
    PrintBitBoard(position.GetBlackOccupiedSquaresBitBoard());

    cout << "\nblack king bitboard:\n";
    PrintBitBoard(position.GetBlackKingBitBoard());
    cout << "\nblack queen bitboard:\n";
    PrintBitBoard(position.GetBlackQueenBitBoard());
    cout << "\nblack rooks bitboard:\n";
    PrintBitBoard(position.GetBlackRooksBitBoard()); 
    cout << "\nblack bishops bitboard:\n";
    PrintBitBoard(position.GetBlackBishopsBitBoard()); 
    cout << "\nblack knights bitboard:\n";
    PrintBitBoard(position.GetBlackKnightsBitBoard()); 
    cout << "\nblack pawns bitboard:\n";
    PrintBitBoard(position.GetBlackPawnsBitBoard());

    cout << "\nwhite king bitboard:\n";
    PrintBitBoard(position.GetWhiteKingBitBoard());
    cout << "\nwhite queen bitboard:\n";
    PrintBitBoard(position.GetWhiteQueenBitBoard());
    cout << "\nwhite rooks bitboard:\n";
    PrintBitBoard(position.GetWhiteRooksBitBoard()); 
    cout << "\nwhite bishops bitboard:\n";
    PrintBitBoard(position.GetWhiteBishopsBitBoard()); 
    cout << "\nwhite knights bitboard:\n";
    PrintBitBoard(position.GetWhiteKnightsBitBoard()); 
    cout << "\nwhite pawns bitboard:\n";
    PrintBitBoard(position.GetWhitePawnsBitBoard()); 

    cout << "\nEn passante bitboard:\n";
    PrintBitBoard(position.GetEnPassanteBitBoard());
    cout << "\nside to move: ";
    if (position.GetSideToMove()) 
        cout << "WHITE\n";
    else 
        cout << "BLACK\n";
    if (position.GetWhiteCanCastleKingSideFlag())
        cout << "\nWhite can castle King Side";
    if (position.GetWhiteCanCastleQueenSideFlag())
        cout << "\nWhite can castle queen side";
    if (position.GetBlackCanCastleKingSideFlag())
        cout << "\nBlack can castle king side";
    if (position.GetBlackCanCastleQueenSideFlag())
        cout << "\nblack can castle queen side";
    cout << "\nhalf move count: ";
    cout << unsigned(position.GetHalfMoveCount()) << endl;
    cout << "\nfull move count: ";
    cout << position.GetFullMoveCount() << endl;

}


unsigned lsb_scan(uint64_t bitboard){
    return __builtin_ctzll(bitboard);
}

unsigned msb_scan(uint64_t bitboard){
    return 63 - __builtin_clzll(bitboard);
}