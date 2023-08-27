#ifndef CHESS_ENGINE_PRINCIPALVARIATION_HPP
#define CHESS_ENGINE_PRINCIPALVARIATION_HPP

#include "Move.hpp"
#include "Piece.hpp"

#include <vector>

class PrincipalVariation {
public:

    using MoveIter = std::vector<Move>::const_reverse_iterator;

    PrincipalVariation(bool isMate, int score, std::vector<Move> moves);

    bool isMate() const;
    int score() const;

    std::size_t length() const;
    MoveIter begin() const;
    MoveIter end() const;
private:
    bool isMate_;
    int score_;
    std::vector<Move> moves_;
};

std::ostream& operator<<(std::ostream& os, const PrincipalVariation& pv);

#endif
