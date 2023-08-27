#ifndef CHESS_ENGINE_BOARD_HPP
#define CHESS_ENGINE_BOARD_HPP

#include "Piece.hpp"
#include "Square.hpp"
#include "Move.hpp"
#include "CastlingRights.hpp"

#include <optional>
#include <iosfwd>
#include <vector>

class Board {
public:

    using Optional = std::optional<Board>;
    using MoveVec = std::vector<Move>;

    Board();

    void setPiece(const Square& square, const Piece::Optional& piece);
    Piece::Optional piece(const Square& square) const;
    void setTurn(PieceColor turn);
    PieceColor turn() const;
    void setCastlingRights(CastlingRights cr);
    CastlingRights castlingRights() const;
    void setEnPassantSquare(const Square::Optional& square);
    Square::Optional enPassantSquare() const;

    void makeMove(const Move& move);

    void pseudoLegalMoves(MoveVec& moves) const;
    void pseudoLegalMovesFrom(const Square& from, MoveVec& moves) const;

    int legalMoves(MoveVec& moves) const;

    double computeValue() const;
    bool isCheck(PieceColor color) const;
private:
    // a board is 64 bits, so for every color and type of piece we keep a UINT64 (future)
    uint64_t WHITE_PAWNS;
    uint64_t WHITE_KNIGHTS;
    uint64_t WHITE_BISHOPS;
    uint64_t WHITE_ROOKS;
    uint64_t WHITE_QUEENS;
    uint64_t WHITE_KINGS;

    uint64_t BLACK_PAWNS;
    uint64_t BLACK_KNIGHTS;
    uint64_t BLACK_BISHOPS;
    uint64_t BLACK_ROOKS;
    uint64_t BLACK_QUEENS;
    uint64_t BLACK_KINGS;

    int wKingIndex;
    int bKingIndex;

    void removePiece(const Square& square, const Piece& piece);

    using squareVec = std::vector<Square>;
    void generateSquares(squareVec& squares, uint64_t pieces) const;

    void generateKingMoves(Square& startSquare, MoveVec& moves) const;
    void generatePawnMoves(Square& startSquare, MoveVec& moves, uint64_t whites, uint64_t blacks) const;
    int generateMove(Square& startSquare, unsigned endFile, unsigned endRank, MoveVec& moves, PieceColor color, int isPawn=0) const;

    bool isInCheck(Square& sq, MoveVec& moves) const;

    int countPieces(uint64_t pieces) const;

    void evadeCheck(MoveVec& moves) const;
    PieceColor getColor(const Square& square) const;

    PieceColor turn_;
    CastlingRights r_;
    Square::Optional e_;
};

std::ostream& operator<<(std::ostream& os, const Board& board);

#endif
