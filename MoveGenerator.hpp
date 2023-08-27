#ifndef CHESS_ENGINE_OWNCLASS
#define CHESS_ENGINE_OWNCLASS

#include "Move.hpp"
#include "Square.hpp"
#include "Move.hpp"

class MoveGenerator {
public:
    using squareVec = std::vector<Square>;
    static void getSquares(uint64_t positions, squareVec& sqvec){
        int i = 0;
        for(uint64_t p = positions ; p > 0 ; p=p>>1){
            if(p & 1){
                auto s = Square::fromIndex(i);
                if(s){
                    sqvec.push_back(*s);
                }
            }
            ++i;
        }
    }

    using MoveVec = std::vector<Move>;
    // sliding types
    static void generateRookMoves(MoveVec& moves, uint64_t startPos, uint64_t friendlies, uint64_t enemies){
        squareVec v;
        getSquares(startPos, v);
        for(Square sq : v){
            auto index = sq.index();
            // up
            for(int i = 1 ; i <= 8 && index < 56 ; ++i){
                if(((i*8)+index) > 63) break; // not in bounds
                if(((uint64_t) 1 << (index + (i*8))) & friendlies) break; // friendly blocked path
                auto res = Square::fromIndex(index + (i*8));
                if(!res) break;
                if(((uint64_t) 1 << (index + (i*8))) & enemies){ // capture
                    moves.push_back(*(new Move(sq, *res)));
                    break;
                }
                moves.push_back(*(new Move(sq, *res))); // empty spot
            }
            // left
            for(int i = 1 ; ((index&7) > 0) && i <= 8 ; i++){
                if(((uint64_t) 1 << (index-i)) & friendlies) break; // block
                auto res = Square::fromIndex(index-i);
                if(!res) break;
                if(((uint64_t) 1 << (index-i)) & enemies){ // capture
                    moves.push_back(*(new Move(sq, *res)));
                    break;
                }
                moves.push_back(*(new Move(sq, *res)));
                if(((index-i)&7) == 0) break;
            }
            // right
            for(int i = 1 ; ((index&7) < 7) && i <= 8 ; i++){
                if(((uint64_t) 1 << (index+i)) & friendlies) break;
                auto res = Square::fromIndex(index+i);
                if(!res) break;
                if(((uint64_t) 1 << (index+i)) & enemies){
                    moves.push_back(*(new Move(sq, *res)));
                    break;
                }
                moves.push_back(*(new Move(sq, *res)));
                if(((i+index)&7) == 7) break;
            }
            // down
            for(int i = 1 ; index > 7 && i <= 8 ; i++){
                if(((uint64_t) 1 << (index-(i*8))) & friendlies) break;
                auto res = Square::fromIndex(index-(i*8));
                if(!res) break;
                if(((uint64_t) 1 << (index-(i*8))) & enemies){
                    moves.push_back(*(new Move(sq, *res)));
                    break;
                }
                moves.push_back(*(new Move(sq, *res)));
                if((index-(i*8)) < 8) break;
            }
        }
    }
    static void generateBishopMoves(MoveVec& moves, uint64_t startPos, uint64_t friendlies, uint64_t enemies){
        squareVec v;
        getSquares(startPos, v);
        for(Square sq : v){
            auto index = sq.index();
            // auto file = sq.file();
            // auto rank = sq.rank();
            for(int i = 1 ; index < 56 && i <= 8 ; i++){
                if((((i*7)+index)>>3) == 8) break;// not in bounds of board: upper bound
                if((((i*7)+index)&7) == 7) break;// not in bounds of board: left bound
                if(((uint64_t) 1 << ((i*7)+index)) & friendlies) break;
                auto res = Square::fromIndex(((i*7)+index));
                if(!res) break;
                if(((uint64_t) 1 << ((i*7)+index)) & enemies){
                    moves.push_back(*(new Move(sq, *res)));
                    break;
                }
                moves.push_back(*(new Move(sq, *res)));
            }
            for(int i = 1 ; index < 56 && i <= 8 ; i++){
                if((((i*9)+index)>>3) == 8) break;// not in bounds of board: upper bound
                if((((i*9)+index)&7) == 0) break;// not in bounds of board: right bound
                if(((uint64_t) 1 << ((i*9)+index)) & friendlies) break;
                auto res = Square::fromIndex(((i*9)+index));
                if(!res) break;
                if(((uint64_t) 1 << ((i*9)+index)) & enemies){
                    moves.push_back(*(new Move(sq, *res)));
                    break;
                }
                moves.push_back(*(new Move(sq, *res)));
            }
            for(int i = 1 ; index > 7 && i <= 8 ; i++){
                if(((index-(i*7))&7) == 0) break;// not in bounds of board: right bound
                if(((uint64_t) 1 << (index-(i*7))) & friendlies) break;
                auto res = Square::fromIndex(index-(i*7));
                if(!res) break;
                if(((uint64_t) 1 << (index-(i*7))) & enemies){
                    moves.push_back(*(new Move(sq, *res)));
                    break;
                }
                moves.push_back(*(new Move(sq, *res)));
                if(((index-(i*7))>>3) == 0) break;// not in bounds of board: bottom bound
            }
            for(int i = 1 ; index > 7 && i <= 8 ; i++){
                if((((index-(i*9)))&7) == 7) break;// not in bounds of board: left bound
                if(((uint64_t) 1 << (index-(i*9))) & friendlies) break;
                auto res = Square::fromIndex(index-(i*9));
                if(!res) break;
                if(((uint64_t) 1 << (index-(i*9))) & enemies){
                    moves.push_back(*(new Move(sq, *res)));
                    break;
                }
                moves.push_back(*(new Move(sq, *res)));
                if(((index-(i*9))>>3) == 0) break;// not in bounds of board: bottom bound
            }
        }
    }
    static void generateQueenMoves(MoveVec& moves, uint64_t startPos, uint64_t friendlies, uint64_t enemies){
        generateRookMoves(moves, startPos, friendlies, enemies);
        generateBishopMoves(moves, startPos, friendlies, enemies);
    }
    // king done in board because needs extra checks with castling
    // jumping types
    static void generateKnightMoves(MoveVec& moves, uint64_t startPos, uint64_t friendlies){
        squareVec v;
        getSquares(startPos, v);
        for(Square sq : v){
            auto rank = sq.rank();
            auto file = sq.file();
            auto res1 = Square::fromCoordinates(file + 1, rank + 2);
            if(res1) if(!(((uint64_t) 1 << (*res1).index()) & friendlies)) moves.push_back(*(new Move(sq, *res1)));
            auto res2 = Square::fromCoordinates(file + 1, rank - 2);
            if(res2) if(!(((uint64_t) 1 << (*res2).index()) & friendlies)) moves.push_back(*(new Move(sq, *res2)));
            auto res3 = Square::fromCoordinates(file + 2, rank + 1);
            if(res3) if(!(((uint64_t) 1 << (*res3).index()) & friendlies)) moves.push_back(*(new Move(sq, *res3)));
            auto res4 = Square::fromCoordinates(file + 2, rank - 1);
            if(res4) if(!(((uint64_t) 1 << (*res4).index()) & friendlies)) moves.push_back(*(new Move(sq, *res4)));
            auto res5 = Square::fromCoordinates(file - 1, rank + 2);
            if(res5) if(!(((uint64_t) 1 << (*res5).index()) & friendlies)) moves.push_back(*(new Move(sq, *res5)));
            auto res6 = Square::fromCoordinates(file - 1, rank - 2);
            if(res6) if(!(((uint64_t) 1 << (*res6).index()) & friendlies)) moves.push_back(*(new Move(sq, *res6)));
            auto res7 = Square::fromCoordinates(file - 2, rank + 1);
            if(res7) if(!(((uint64_t) 1 << (*res7).index()) & friendlies)) moves.push_back(*(new Move(sq, *res7)));
            auto res8 = Square::fromCoordinates(file - 2, rank - 1);
            if(res8) if(!(((uint64_t) 1 << (*res8).index()) & friendlies)) moves.push_back(*(new Move(sq, *res8)));
        }
    }
    // pawn done in board because needs extra checks with en passante

};



#endif