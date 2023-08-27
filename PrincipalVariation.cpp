#include "PrincipalVariation.hpp"

#include <ostream>

PrincipalVariation::PrincipalVariation(bool isMate, int score, std::vector<Move> moves):isMate_(isMate), score_(score), moves_(moves){}

bool PrincipalVariation::isMate() const {
    return isMate_;
}

int PrincipalVariation::score() const {
    return score_;
}

std::size_t PrincipalVariation::length() const {
    return moves_.size();
}

PrincipalVariation::MoveIter PrincipalVariation::begin() const {
    return moves_.rbegin();
}

PrincipalVariation::MoveIter PrincipalVariation::end() const {
    return moves_.rend();
}

std::ostream& operator<<(std::ostream& os, const PrincipalVariation& pv) {
    os << "Moves: ";
    for(auto i = pv.begin(); i < pv.end(); i++) os << *i <<" ";
    os << "\t Score: " << pv.score() << "\t";
    os << "Len: " << pv.length() << "\t";
    os << "Is mate: " << pv.isMate() << "\n";
    return os;
}
