#include "move.h"

void Move::PrintMove() {
    cout << "Origin Square: " << GetOriginSquare() << endl;
    cout << "Destination Square: " <<  GetDestinationSquare() << endl;
    if (IsDoublePawnPush()) cout << "This move is a double pawn push.\n";
    if (IsCapture()) cout << "This move is a capture.\n";
    if (IsEnPassantCapture()) cout << "This move is an en passant capture.";
    if (IsPromotion()) {
        cout << "This move is a promotion ";
        if (PromotePawnToQueen()) cout << " to queen.\n";
        if (PromotePawnToRook()) cout << " to rook.\n";
        if (PromotePawnToKnight()) cout << " to knight.\n";
        if (PromotePawnToBishop()) cout << " to bishop.\n";
    }
    if (RemoveKingSideCastlingRights()) cout << "This move removes king side castling rights.\n";
    if (RemoveQueenSideCastlingRights()) cout << "This move removes queen side castling rights.\n";
}