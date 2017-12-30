#ifndef MOVE_H
#define MOVE_H

#include <cstdint>

#define ORIGIN_MASK 0xFC00
#define DESTINATION_MASK 0x3F0
#define FLAGS_MASK 0xF
#define PROMOTION_MASK 0x8
#define CAPTURE_MASK 0x4
#define SPECIAL_1_MASK 0x2
#define SPECIAL_2_MASK 0x1
#define SPECIAL_1_2_MASK 0x3
#define EN_PASSANT_CAPTURE_MASK 0x5

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
     * 0 | 0 | 0 | 0 - quiet moves
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
    uint16_t move;
public:
    Move() { move = 0; }
    void SetOriginSquare(uint8_t origin) { move |= (origin << 10); }
    void SetDestinationSquare(uint8_t destination) { move |= (destination << 4); }
    void SetDoublePawnPushFlag() { move |= SPECIAL_2_MASK; }
    void SetCaptureFlag() { move |= CAPTURE_MASK; }
    void SetPromotionFlag() { move |= PROMOTION_MASK; }
    void SetKingSideCastleFlag() { move |= SPECIAL_1_MASK; }
    void SetQueenSideCastleFlag() { move |= SPECIAL_1_2_MASK; }
    void SetEnPassantCaptureFlag() { move |=  EN_PASSANT_CAPTURE_MASK; }
    void SetPromotionPieceToQueen() { move |=  SPECIAL_1_2_MASK; }
    void SetPromotionPieceToRook() { move |= SPECIAL_1_MASK; }
    void SetPromotionPieceToKnight() {;} // nothing to be done
    void SetPromotionPieceToBishop() { move |= SPECIAL_2_MASK; }

    uint16_t GetOriginSquare() { return move >> 10; }
    uint16_t GetDestinationSquare() { return ( move & DESTINATION_MASK ) >> 4; }
    bool GetDoublePawnPushFlag() { return (move & FLAGS_MASK) == 1; }
    bool GetCaptureFlag() { return (bool)(move & CAPTURE_MASK); }
    bool GetPromotionFlag() { return  (bool)(move & PROMOTION_MASK); }
    bool GetKingSideCastleFlag() { return (move & FLAGS_MASK) == 2; }
    bool GetQueenSideCastleFlag() { return (move & FLAGS_MASK) == 3; }
    bool GetEnPassantCaptureFlag() { return (move & FLAGS_MASK) == 5; }
    bool PromotePawnToQueen() { return (move & SPECIAL_1_2_MASK) == 3; }
    bool PromotePawnToRook() { return (move & SPECIAL_1_2_MASK) == 2; }
    bool PromotePawnToKnight() { return !(move & SPECIAL_1_2_MASK); }
    bool PromotePawnToBishop() { return (move & SPECIAL_1_2_MASK) == 1; }
};

#endif
