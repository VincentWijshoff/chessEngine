#include "Piece.hpp"

#include <ostream>
#include <utility>

Piece::Piece(PieceColor color, PieceType type)
    :color_(color), type_(type)
{
}

Piece::Optional Piece::fromSymbol(char symbol) {
    // P (pawn), N (knight), B (bishop), R (rook), Q (queen), and K (king). Uppercase letters are used for white, lowercase for black
    char lowerSym = (char) tolower(symbol);
    if(lowerSym == "p"[0]){
        return islower(symbol) ? Piece(PieceColor::Black, PieceType::Pawn) : Piece(PieceColor::White, PieceType::Pawn);
    } else if(lowerSym == "n"[0]){
        return islower(symbol) ? Piece(PieceColor::Black, PieceType::Knight) : Piece(PieceColor::White, PieceType::Knight);
    } else if(lowerSym == "b"[0]){
        return islower(symbol) ? Piece(PieceColor::Black, PieceType::Bishop) : Piece(PieceColor::White, PieceType::Bishop);
    } else if(lowerSym == "r"[0]){
        return islower(symbol) ? Piece(PieceColor::Black, PieceType::Rook) : Piece(PieceColor::White, PieceType::Rook);
    } else if(lowerSym == "q"[0]){
        return islower(symbol) ? Piece(PieceColor::Black, PieceType::Queen) : Piece(PieceColor::White, PieceType::Queen);
    } else if(lowerSym == "k"[0]){
        return islower(symbol) ? Piece(PieceColor::Black, PieceType::King) : Piece(PieceColor::White, PieceType::King);
    }
    return std::nullopt;
}

PieceColor Piece::color() const {
    return color_;
}

PieceType Piece::type() const {
    return type_;
}

bool operator==(const Piece& lhs, const Piece& rhs) {
    return lhs.color() == rhs.color() && lhs.type() == rhs.type();
}

std::ostream& operator<<(std::ostream& os, const Piece& piece) {
    char* rslt;
    if(piece.type() == PieceType::Pawn){
        rslt = const_cast<char *>(piece.color() == PieceColor::White ? "P" : "p");
        return os << rslt;
    } else if(piece.type() == PieceType::Knight){
        rslt = const_cast<char *>(piece.color() == PieceColor::White ? "N" : "n");
        return os << rslt;
    } else if(piece.type() == PieceType::Bishop){
        rslt = const_cast<char *>(piece.color() == PieceColor::White ? "B" : "b");
        return os << rslt;
    } else if(piece.type() == PieceType::Rook){
        rslt = const_cast<char *>(piece.color() == PieceColor::White ? "R" : "r");
        return os << rslt;
    } else if(piece.type() == PieceType::Queen){
        rslt = const_cast<char *>(piece.color() == PieceColor::White ? "Q" : "q");
        return os << rslt;
    } else if(piece.type() == PieceType::King){
        rslt = const_cast<char *>(piece.color() == PieceColor::White ? "K" : "k");
        return os << rslt;
    }
    (void)piece;
    return os;
}

PieceColor operator!(PieceColor color) {
    return color == PieceColor::White ? PieceColor::Black : PieceColor::White;
}
