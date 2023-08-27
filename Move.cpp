#include "Move.hpp"
#include <string.h>
#include "Square.hpp"
#include "Piece.hpp"
#include <ostream>

Move::Move(const Square& from, const Square& to,
           const std::optional<PieceType>& promotion):
           from_(from), to_(to), promotion_(promotion)
{
}

Move::Optional Move::fromUci(const std::string& uci) {
    if(uci.length() < 4){return std::nullopt;}

    std::optional<Square> from = Square::fromName(uci.substr(0, 2));
    std::optional<Square> to = Square::fromName(uci.substr(2, 2));
    if(!from || !to) return std::nullopt;

    if(uci.length() > 4){
        if(uci.length() > 5) return std::nullopt;
        auto pieceSymbol = uci.substr(4,1);
        std::optional<Piece> promotionPiece = Piece::fromSymbol(pieceSymbol[0]);
        if(promotionPiece){
            Piece piece = *promotionPiece;
            return Move(*from, *to, piece.type());
        }
        return std::nullopt;
    }
    return Move(*from, *to);
}

Square Move::from() const {
    return from_;
}

Square Move::to() const {
    return to_;
}

std::optional<PieceType> Move::promotion() const {
    return promotion_;
}

std::ostream& operator<<(std::ostream& os, const Move& move) {
    os << move.from() << move.to();
    if(move.promotion()){
        PieceType prom = *move.promotion();
        if(prom == PieceType::Pawn){os << 'p';}
        else if(prom == PieceType::Rook){os << 'r';}
        else if(prom == PieceType::Knight){os << 'n';}
        else if(prom == PieceType::Bishop){os << 'b';}      
        else if(prom == PieceType::Queen){os << 'q';}   
        else if(prom == PieceType::King){os << 'k';}
    }
    return os;
}


bool operator<(const Move& lhs, const Move& rhs) {
    return lhs.to() < rhs.to();
}

bool operator==(const Move& lhs, const Move& rhs) {
    if(lhs.promotion() && rhs.promotion()){
        return lhs.to() == rhs.to() && lhs.from() == rhs.from() && *lhs.promotion() == *rhs.promotion();
    }
    if(lhs.promotion() || rhs.promotion()){
        return false;
    }
    return lhs.to() == rhs.to() && lhs.from() == rhs.from();
}
