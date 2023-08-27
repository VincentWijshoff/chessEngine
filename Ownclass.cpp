#include "Ownclass.hpp"
#include "Board.hpp"
#include <iostream>

Ownclass::Ownclass(){}

Ownclass::~Ownclass(){}

std::string Ownclass::name() const{return "VW_Chenginge";} // VW_Chengine
std::string Ownclass::version() const{return "1.0.0";}
std::string Ownclass::author() const{return "Vincent Wijshoff";}

// called when new game starts (not guaranteed to be called for the first game played by an Engine instance):
void Ownclass::newGame(){}

// calculate and return the PV starting from the position represented by board
PrincipalVariation Ownclass::pv(
    const Board& board,
    const TimeInfo::Optional& timeInfo // completely optional, so will be ignored for now
){
    (void) timeInfo;
    // we want to use the negamax algorithm to get the sequention of moves that will yield the highest score
    std::tuple<double, std::vector<Move>, int> mv = negaMax(board, depth_,-9999999999999 ,9999999999999);
    return PrincipalVariation(std::get<2>(mv) == 1 ? true : false, std::get<2>(mv) == 0 ? (int) std::get<0>(mv) : 0, std::get<1>(mv)); 
}

std::tuple<double, std::vector<Move>, int> Ownclass::negaMax(const Board& b, unsigned depth, double alpha, double beta){
    if(depth == 0) return std::make_tuple(b.computeValue(), std::vector<Move>{}, 0);
    // get all possible moves
    Board::MoveVec possibleMoves;
    int result = b.legalMoves(possibleMoves);
    if(result == 1)         return std::make_tuple((double) -99999999, std::vector<Move>{}, 1); // checkmate
    else if(result == 2)    return std::make_tuple(b.computeValue(), std::vector<Move>{}, 2); // stalemate
    // alpha beta pruning
    std::tuple<double, std::vector<Move>, int> best((double) -99999999, std::vector<Move>{}, 0);
    for(Move m : possibleMoves){
        Board b2 = b;
        b2.makeMove(m);
        std::tuple<double, std::vector<Move>, int> tmp = negaMax(b2, depth-1, -beta, -alpha);
        (std::get<1>(tmp)).push_back(m);

        if(-(std::get<0>(tmp)) > std::get<0>(best)){
            best = tmp;
            std::get<0>(best) = -std::get<0>(tmp);
        }
        if(std::get<0>(best) > alpha) alpha = std::get<0>(best);
        if(alpha >= beta) break;
    }
    return std::make_tuple(std::get<0>(best), std::get<1>(best), std::get<2>(best));;
}

// need to overwrite if transposition tables are used
std::optional<HashInfo> Ownclass::hashInfo() const{
    return std::optional<HashInfo>();
}
void Ownclass::setHashSize(std::size_t size){
    (void) size;
}