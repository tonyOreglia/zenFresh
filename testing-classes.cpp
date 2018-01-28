#include <cstdint>
#include <bitset>
#include <iostream>
#include "position.h"
#include "game.h"
#include "move.h"
#include "./bitboard_utilities/bitboard-utilities.h"


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
    // Position position(
    //     (char *)"rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R",
    //     (char *)"b",
    //     (char *)"KQkq",
    //     (char *)"-",
    //     (char *)"1",
    //     (char *)"2"
    // );
    // Position position(
    //     (char *)"4k3/8/8/8/8/8/4P3/4K3",
    //     (char *)"w",
    //     (char *)"-",
    //     (char *)"-",
    //     (char *)"5",
    //     (char *)"39"
    // );

    // Position position(
    //     (char *)"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R",
    //     (char *)"w",
    //     (char *)"KQkq",
    //     (char *)"-",
    //     (char *)"1",
    //     (char *)"1"
    // );
    // Position starting = position;

    // Position position(
    //     (char *)"k7/8/8/3B4/8/8/8/K7",
    //     (char *)"w",
    //     (char *)"KQkq",
    //     (char *)"-",
    //     (char *)"1",
    //     (char *)"1"
    // );

    Position position(
        (char *)"4k3/8/8/8/8/8/8/4K3",
        (char *)"b",
        (char *)"KQkq",
        (char *)"-",
        (char *)"1",
        (char *)"1"
    );

    // Position position(
    //     (char *)"8/8/8/8/8/2r2R2/PPPPPPPP/8",
    //     (char *)"w",
    //     (char *)"KQkq",
    //     (char *)"-",
    //     (char *)"1",
    //     (char *)"1"
    // );

    Position starting = position;
    // Position position;
    Game game(position);
    vector <Move> movesList = game.GetMovesVector(0);
    game.GenerateMoves(movesList);
    cout << "move cnt: " << movesList.size() << endl;
    for(vector<Move>::iterator it = movesList.begin(); it != movesList.end(); ++it) {
        position.UpdatePositionWithSingleMove(*it);
        PrintHumanReadableBoard(position);
        position = starting;
    }
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
