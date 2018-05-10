#ifndef move__H
#define move__H

#include <cstdint>
#include <iostream>

#include "./bitboard_utilities/bitboard-utilities.h"

#define ORIGIN_MASK 0xFC00
#define DESTINATION_MASK 0x3F0
#define FLAGS_MASK 0xF
#define PROMOTION_MASK 0x8
#define CAPTURE_MASK 0x4
#define SPECIAL_1_MASK 0x2
#define SPECIAL_2_MASK 0x1
#define SPECIAL_1_2_MASK 0x3
#define EN_PASSANT_CAPTURE_MASK 0x5

using namespace std; 

class Move {
private:
    /**
    Bits 0-5: origin square (highest order bits)
    Bit 6-11: destination square
    Bit 12: Capture Flag
    Bit 13: Promotion Flag
    Bit 14-15: Special Bits covering promotion piece type, castling info,
               double pawn push, and en passant capture.
    Bits 12-15:
     * promotion | capture | special 1 | special 2
     * 0 | 0 | 0 | 0 - quiet move
     * 0 | 0 | 0 | 1 - double pawn push
     * 0 | 0 | 1 | 0 - king side castle
     * 0 | 0 | 1 | 1 - queen side castle
     * 0 | 1 | 0 | 0 - captures
     * 0 | 1 | 0 | 1 - en passant capture
     * 1 | 0 | 0 | 0 - knight promotion
     * 1 | 0 | 0 | 1 - bishop promotion
     * 1 | 0 | 1 | 0 - rook promotion
     * 1 | 0 | 1 | 1 - queen promotion
     * 1 | 1 | 0 | 0 - knight promotion capture
     * 1 | 1 | 0 | 1 - bishop promotion capture
     * 1 | 1 | 1 | 0 - rook promotion capture
     * 1 | 1 | 1 | 1 - queen promotion capture
     */
    uint16_t move_;
public:
    Move() { move_ = 0; }
    Move(uint16_t move) { move_ = move; }
    Move(short origin, short destination) { move_ = origin << 10 | destination << 4; }
    void PrintMoveInBinary() {cout << bitset<16>(move_);}
    void SetOriginSquare(uint8_t origin) { move_ |= (origin << 10); }
    void SetDestinationSquare(uint8_t destination) { move_ |= (destination << 4); }
    void SetDoublePawnPushFlag() { move_ |= SPECIAL_2_MASK; }
    void SetCaptureFlag() { move_ |= CAPTURE_MASK; }
    void SetPromotionFlag() { move_ |= PROMOTION_MASK; }
    void SetKingSideCastleFlag() { move_ |= SPECIAL_1_MASK; }
    void SetQueenSideCastleFlag() { move_ |= SPECIAL_1_2_MASK; }
    void SetEnPassantCaptureFlag() { move_ |=  EN_PASSANT_CAPTURE_MASK; }
    void SetPromotionPieceToQueen() { move_ |=  SPECIAL_1_2_MASK; }
    void SetPromotionPieceToRook() { move_ |= SPECIAL_1_MASK; }
    void SetPromotionPieceToKnight() { move_ &= ~SPECIAL_1_2_MASK; }
    void SetPromotionPieceToBishop() { move_ |= SPECIAL_2_MASK; }

    uint16_t GetOriginSquare() { return move_ >> 10; }
    uint16_t GetDestinationSquare() { return ( move_ & DESTINATION_MASK ) >> 4; }
    bool IsDoublePawnPush() { return (move_ & FLAGS_MASK) == 1; }
    bool IsCapture() { return (bool)(move_ & CAPTURE_MASK); }
    bool IsPromotion() { return  (bool)(move_ & PROMOTION_MASK); }
    bool IsKingSideCastle() { return (move_ & SPECIAL_1_2_MASK) == 2; }
    bool IsQueenSideCastle() { return (move_ & SPECIAL_1_2_MASK) == 3; }
    bool IsEnPassantCapture() { return (move_ & EN_PASSANT_CAPTURE_MASK) == 5; }
    bool PromotePawnToQueen() { return (move_ & SPECIAL_1_2_MASK) == 3; }
    bool PromotePawnToRook() { return (move_ & SPECIAL_1_2_MASK) == 2; }
    bool PromotePawnToKnight() { return !(move_ & SPECIAL_1_2_MASK); }
    bool PromotePawnToBishop() { return (move_ & SPECIAL_1_2_MASK) == 1; }

    uint16_t GetMove() { return move_; }
    void PrintMove();
};

#endif
