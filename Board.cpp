#include "Board.hpp"

#include <ostream>
#include <cassert>
#include <cmath>
#include <vector>
#include <iostream>
#include "MoveGenerator.hpp"

Board::Board():WHITE_PAWNS(0),WHITE_KNIGHTS(0),WHITE_BISHOPS(0),WHITE_ROOKS(0),WHITE_QUEENS(0),WHITE_KINGS(0),
               BLACK_PAWNS(0),BLACK_KNIGHTS(0),BLACK_BISHOPS(0),BLACK_ROOKS(0),BLACK_QUEENS(0),BLACK_KINGS(0),
               wKingIndex(0), bKingIndex(0), turn_(PieceColor::White), r_(CastlingRights::None)
{
}

// need to get the piece on the correct position based  on the coordinate and an integer
// by using bitwise OR on a number with the correct position 1 (multiple of 2), it can set the corrrect 1 and keep all other ones
// the index of the square + 1 is the place the 1 should go, so index(C1) = 2 -> 2**n=4 = 100 :: index(E1) = 4 -> 2**n = 16 = 10000
// in reverse its easy just take the position of the 1 -1 and thats the index

void Board::setPiece(const Square& square, const Piece::Optional& piece) {
    if(piece){
        auto p = *piece;
        uint64_t positionNumber = (uint64_t)  1<<square.index();
        // first check if the space is occupied
        if(Board::piece(square)){
            // remove old piece
            Board::removePiece(square, *Board::piece(square));
        }
        if(p.type() == PieceType::Pawn){
            if(p.color() == PieceColor::White)  WHITE_PAWNS |= positionNumber;
            else                                BLACK_PAWNS |= positionNumber;
        } else if(p.type() == PieceType::Knight){
            if(p.color() == PieceColor::White)  WHITE_KNIGHTS |= positionNumber;
            else                                BLACK_KNIGHTS |= positionNumber;
        } else if(p.type() == PieceType::Bishop){
            if(p.color() == PieceColor::White)  WHITE_BISHOPS |= positionNumber;
            else                                BLACK_BISHOPS |= positionNumber;
        } else if(p.type() == PieceType::Rook){
            if(p.color() == PieceColor::White)  WHITE_ROOKS |= positionNumber;
            else                                BLACK_ROOKS |= positionNumber;
        } else if(p.type() == PieceType::Queen){
            if(p.color() == PieceColor::White)  WHITE_QUEENS |= positionNumber;
            else                                BLACK_QUEENS |= positionNumber;
        } else if(p.type() == PieceType::King){
            if(p.color() == PieceColor::White){  
                WHITE_KINGS |= positionNumber;
                wKingIndex = square.index();
            }else{
                BLACK_KINGS |= positionNumber;
                bKingIndex = square.index();
            }                                
        } 
    }
}

void Board::removePiece(const Square& square, const Piece& piece){
    if(piece.type() == PieceType::Pawn && piece.color() == PieceColor::White)   WHITE_PAWNS^=((uint64_t)1<<square.index());
    if(piece.type() == PieceType::Rook && piece.color() == PieceColor::White)   WHITE_ROOKS^=((uint64_t)1<<square.index());
    if(piece.type() == PieceType::Bishop && piece.color() == PieceColor::White) WHITE_BISHOPS^=((uint64_t)1<<square.index());
    if(piece.type() == PieceType::Knight && piece.color() == PieceColor::White) WHITE_KNIGHTS^=((uint64_t)1<<square.index());
    if(piece.type() == PieceType::Queen && piece.color() == PieceColor::White)  WHITE_QUEENS^=((uint64_t)1<<square.index());
    if(piece.type() == PieceType::King && piece.color() == PieceColor::White)   WHITE_KINGS^=((uint64_t)1<<square.index());
    
    if(piece.type() == PieceType::Pawn && piece.color() == PieceColor::Black)   BLACK_PAWNS^=((uint64_t)1<<square.index());
    if(piece.type() == PieceType::Rook && piece.color() == PieceColor::Black)   BLACK_ROOKS^=((uint64_t)1<<square.index());
    if(piece.type() == PieceType::Bishop && piece.color() == PieceColor::Black) BLACK_BISHOPS^=((uint64_t)1<<square.index());
    if(piece.type() == PieceType::Knight && piece.color() == PieceColor::Black) BLACK_KNIGHTS^=((uint64_t)1<<square.index());
    if(piece.type() == PieceType::Queen && piece.color() == PieceColor::Black)  BLACK_QUEENS^=((uint64_t)1<<square.index());
    if(piece.type() == PieceType::King && piece.color() == PieceColor::Black)   BLACK_KINGS^=((uint64_t)1<<square.index());
}

// we asume no 2 pieces can be on 1 square
// lets say a white pawn is at C1, so index(C1) = 2, WHITE_PAWNS = 0b0...00100, so WHITE_PAWNS >> index(C!) = 0b0...01, to chek if it
// at this location, a simple modulo 2 check is sufficient
Piece::Optional Board::piece(const Square& square) const {
    uint64_t index = (uint64_t) 1 << square.index();
    if(WHITE_PAWNS & index){return Piece(PieceColor::White, PieceType::Pawn);}
    if(WHITE_ROOKS & index){return Piece(PieceColor::White, PieceType::Rook);}
    if(WHITE_BISHOPS & index){return Piece(PieceColor::White, PieceType::Bishop);}
    if(WHITE_KNIGHTS & index){return Piece(PieceColor::White, PieceType::Knight);}
    if(WHITE_QUEENS & index){return Piece(PieceColor::White, PieceType::Queen);}
    if(WHITE_KINGS & index){return Piece(PieceColor::White, PieceType::King);}

    if(BLACK_PAWNS & index){return Piece(PieceColor::Black, PieceType::Pawn);}
    if(BLACK_ROOKS & index){return Piece(PieceColor::Black, PieceType::Rook);}
    if(BLACK_BISHOPS & index){return Piece(PieceColor::Black, PieceType::Bishop);}
    if(BLACK_KNIGHTS & index){return Piece(PieceColor::Black, PieceType::Knight);}
    if(BLACK_QUEENS & index){return Piece(PieceColor::Black, PieceType::Queen);}
    if(BLACK_KINGS & index){return Piece(PieceColor::Black, PieceType::King);}

    return std::nullopt;
}

PieceColor Board::getColor(const Square& square) const {
    uint64_t index = (uint64_t) 1 << square.index();
    if((WHITE_PAWNS | WHITE_BISHOPS | WHITE_KINGS | WHITE_KNIGHTS | WHITE_QUEENS | WHITE_ROOKS) & index) return PieceColor::White;
    return PieceColor::Black;
}

void Board::setTurn(PieceColor turn) {
    turn_ = turn;
}

PieceColor Board::turn() const {
    return turn_;
}

void Board::setCastlingRights(CastlingRights cr) {
    r_ = cr;
}

CastlingRights Board::castlingRights() const {
    return r_;
}

void Board::setEnPassantSquare(const Square::Optional& square) {
    e_ = square;
}

Square::Optional Board::enPassantSquare() const {
    return e_;
}

// this implementation does not support the promoted to part
void Board::makeMove(const Move& move) {
    // remove the piece from the 'from' part
    Piece::Optional fromPieceOpt = piece(move.from());
    if(!fromPieceOpt) return;
    Piece piece = *fromPieceOpt;

    removePiece(move.from(), piece);

    // add the piece to the 'to' part if not a promotion
    if(!move.promotion())   setPiece(move.to(), piece);
    else                    setPiece(move.to(), Piece(turn(), move.promotion().value()));

    // if an en pasant was executed, the oposing pawn should be removed
    if(piece.type() == PieceType::Pawn && move.from().file() != move.to().file() && enPassantSquare()){
        // check if it was actually an en passant
        Square epsq = *enPassantSquare();
        if(epsq.file() == move.to().file() && epsq.rank() == move.to().rank()){
            // a zero should be set on the square an en pasant was executed on
            unsigned enpasfile = move.to().file();
            unsigned enpasrank = move.from().rank();
            Square::Optional sq = Square::fromCoordinates(enpasfile, enpasrank);
            if(!sq) return;
            if(piece.color() == PieceColor::White)    BLACK_PAWNS^=((uint64_t)1<<(*sq).index());
            else                                    WHITE_PAWNS^=((uint64_t)1<<(*sq).index());
        }
    }

    // en passant square set
    // move needs to be from a pawn and 2 steps, the an en pasant is possible
    if(piece.type() == PieceType::Pawn && (move.from().rank() - move.to().rank() == 2 || move.to().rank() - move.from().rank() == 2)){
        // the skipped square is the en passant square
        Square::Optional sq = Square::fromCoordinates(move.to().file(),(move.from().rank() + move.to().rank())>>1);
        if(!sq) return;
        setEnPassantSquare(*sq);
    }else{
        setEnPassantSquare(std::nullopt);
    }

    // castling (need to move the rook)
    // this is a king move with 2 places
    if(piece.type() == PieceType::King && (move.from().file()-move.to().file() == 2 || move.to().file()-move.from().file() == 2)){
        // color and direction are important
        if(piece.color() == PieceColor::Black){
            // castling on black side from file=4 to file=2 or file=6
            if(move.to().file() == 2){
                // move left rook (index 56 -> index 59)
                BLACK_ROOKS^=((uint64_t)1<<56);
                BLACK_ROOKS|=((uint64_t)1<<59);
            }else{
                // move right rook (index 63 -> index 61)
                BLACK_ROOKS^=((uint64_t)1<<63);
                BLACK_ROOKS|=((uint64_t)1<<61);
            }
        } else {
            // castling on White side from file=4 to file=2 or file=6
            if(move.to().file() == 2){
                // move left rook (index 0 -> index 3)
                WHITE_ROOKS^=((uint64_t)1);
                WHITE_ROOKS|=((uint64_t)1<<3);
            }else{
                // move right rook (index 7 -> index 5)
                WHITE_ROOKS^=((uint64_t)1<<7);
                WHITE_ROOKS|=((uint64_t)1<<5);
            }
        }
    }

    // if a king moved, the right to castle is removed
    if(piece.type() == PieceType::King){
        if(piece.color() == PieceColor::Black)  setCastlingRights(castlingRights() & ~CastlingRights::Black);
        else                                    setCastlingRights(castlingRights() & ~CastlingRights::White);
    }

    // if a rook moved, the castling rights are removed for that rook
    if(piece.type() == PieceType::Rook){
        if(piece.color() == PieceColor::Black){
            if(move.from().file() == 0)         setCastlingRights(castlingRights() & ~CastlingRights::BlackQueenside);
            else if(move.from().file() == 7)    setCastlingRights(castlingRights() & ~CastlingRights::BlackKingside);
        }else{
            if(move.from().file() == 0)         setCastlingRights(castlingRights() & ~CastlingRights::WhiteQueenside);
            else if(move.from().file() == 7)    setCastlingRights(castlingRights() & ~CastlingRights::WhiteKingside);
        }
    }

    // if a rook was captured on their starting position, the castling rights should be removed
    if(move.to().rank() == 0 && piece.color() == PieceColor::Black){
        // white side
        if(move.to().file() == 0 && (castlingRights() & CastlingRights::WhiteQueenside) != CastlingRights::None) setCastlingRights(castlingRights() & ~CastlingRights::WhiteQueenside);
        else if(move.to().file() == 7 && (castlingRights() & CastlingRights::WhiteKingside) != CastlingRights::None) setCastlingRights(castlingRights() & ~CastlingRights::WhiteKingside);
    } else if(move.to().rank() == 7 && piece.color() == PieceColor::White){
        // white side
        if(move.to().file() == 0 && (castlingRights() & CastlingRights::BlackQueenside) != CastlingRights::None) setCastlingRights(castlingRights() & ~CastlingRights::BlackQueenside);
        else if(move.to().file() == 7 && (castlingRights() & CastlingRights::BlackKingside) != CastlingRights::None) setCastlingRights(castlingRights() & ~CastlingRights::BlackKingside);
    }


    turn_=!turn_;
}

// generates all squares for a given uint64
void Board::generateSquares(squareVec& squares, uint64_t pieces) const{
    int i = 0;
    for(uint64_t p = pieces ; p > 0 ; p=p>>1){
        if(p & 1){
            auto s = Square::fromIndex(i);
            if(s){
                squares.push_back(*s);
            }
        }
        ++i;
    }
}

// isPawn;  0 == not a pawn or a pawn that can capture, 
//          1 == is a pawn and not up for promotion, 
//          2 == is a pawn and up for promotion, 
//          3 == is a pawn and up for promotion, but not if a piece in the way
int Board::generateMove(Square& startSquare, unsigned endFile, unsigned endRank, MoveVec& moves, PieceColor thisColor, int isPawn) const {
    // get a starting square, create ending square and put into moves
    // return True if it should not check further in that direction
    std::optional<Square> endSquare = Square::fromCoordinates(endFile, endRank);
    if(!endSquare) return 1; // not a valid square, stop checking further
    Piece::Optional pc =  piece(*endSquare); // check if there is a piece on the new spot
    if(!pc){
        // if isPawn == 2 -> promote to queen
        Move* move;
        if(isPawn == 2 || isPawn == 3) move = new Move(startSquare, *endSquare, PieceType::Queen);
        else move = new Move(startSquare, *endSquare);
        moves.push_back(*move);
        return 0;
    }
    // a piece in that spot, definetely end, but check if can take or not
    if((*pc).color() != thisColor && isPawn != 1 && isPawn != 3){ // if different colors and not a pawn(going forward), take the piece
        // if isPawn == 2, then promotion to queen
        Move* move;
        if(isPawn == 2) move = new Move(startSquare, *endSquare, PieceType::Queen);
        else move = new Move(startSquare, *endSquare);
        moves.push_back(*move);
    }
    return 1;
}

void Board::generateKingMoves(Square& startSquare, MoveVec& moves) const{
    // can only move 1 spot in each direction
    auto file = startSquare.file();
    auto rank = startSquare.rank();
    PieceColor color = getColor(startSquare);
    generateMove(startSquare, file, rank+1, moves, color);
    generateMove(startSquare, file+1, rank+1, moves, color);
    generateMove(startSquare, file-1, rank+1, moves, color);
    generateMove(startSquare, file, rank-1, moves, color);
    generateMove(startSquare, file+1, rank-1, moves, color);
    generateMove(startSquare, file-1, rank-1, moves, color);
    generateMove(startSquare, file+1, rank, moves, color);
    generateMove(startSquare, file-1, rank, moves, color);
    
    // castling
    CastlingRights rights = castlingRights();
    bool leftCheck = color == PieceColor::White ? 
                (rights & CastlingRights::WhiteQueenside) != CastlingRights::None : 
                (rights & CastlingRights::BlackQueenside) != CastlingRights::None;
    bool rightCheck = color == PieceColor::White ?
                (rights & CastlingRights::WhiteKingside) != CastlingRights::None :
                (rights & CastlingRights::BlackKingside) != CastlingRights::None;
    unsigned rk = color == PieceColor::White ? 0 : 7;
    MoveVec otherMoves; // need to know if used squares are in check, so check other colors except for king
    uint64_t whites = WHITE_PAWNS | WHITE_ROOKS | WHITE_KNIGHTS | WHITE_BISHOPS | WHITE_QUEENS | WHITE_KINGS;
    uint64_t blacks = BLACK_PAWNS | BLACK_ROOKS | BLACK_KNIGHTS | BLACK_BISHOPS | BLACK_QUEENS | BLACK_KINGS;
    if(color == PieceColor::White){
        MoveGenerator::generateQueenMoves(otherMoves, BLACK_QUEENS, blacks, whites);
        MoveGenerator::generateBishopMoves(otherMoves, BLACK_BISHOPS, blacks, whites);
        MoveGenerator::generateKnightMoves(otherMoves, BLACK_KNIGHTS, blacks);
        MoveGenerator::generateRookMoves(otherMoves, BLACK_ROOKS, blacks, whites);  
        squareVec v;
        generateSquares(v, BLACK_PAWNS);
        for(Square s : v){
            generatePawnMoves(s, otherMoves, whites, blacks);
        }  
    } else{
        MoveGenerator::generateQueenMoves(otherMoves, WHITE_QUEENS, whites, blacks);  
        MoveGenerator::generateBishopMoves(otherMoves, WHITE_BISHOPS, whites, blacks);
        MoveGenerator::generateKnightMoves(otherMoves, WHITE_KNIGHTS, whites);    
        MoveGenerator::generateRookMoves(otherMoves, WHITE_ROOKS, whites, blacks);
        squareVec v;
        generateSquares(v, WHITE_PAWNS);
        for(Square s : v){
            generatePawnMoves(s, otherMoves, whites, blacks);
        }
    }
    if(leftCheck){
        // now check if all spaces between king (4, 0) and rook (0, 0) are clear(f,r)
        Square::Optional sq1 = Square::fromCoordinates(1, rk);
        Square::Optional sq2 = Square::fromCoordinates(2, rk);
        Square::Optional sq3 = Square::fromCoordinates(3, rk);
        Square::Optional sq4 = Square::fromCoordinates(4, rk);
        if(!sq1 || !sq2 || !sq3 || !sq4) return;
        Piece::Optional pc1 = piece(*sq1);
        Piece::Optional pc2 = piece(*sq2);
        Piece::Optional pc3 = piece(*sq3);
        if(!pc1 && !pc2 && !pc3 &&
            !isInCheck(*sq2, otherMoves) && 
            !isInCheck(*sq3, otherMoves) &&
            !isInCheck(*sq4, otherMoves))    generateMove(startSquare, file-2, rank, moves, color);
    } 
    if(rightCheck){
        Square::Optional sq0 = Square::fromCoordinates(4, rk);
        Square::Optional sq1 = Square::fromCoordinates(5, rk);
        Square::Optional sq2 = Square::fromCoordinates(6, rk);
        if(!sq0 || !sq1 || !sq2) return;
        Piece::Optional pc1 = piece(*sq1);
        Piece::Optional pc2 = piece(*sq2);
        if(!pc1 && !pc2 &&
            !isInCheck(*sq0, otherMoves) && 
            !isInCheck(*sq1, otherMoves) && 
            !isInCheck(*sq2, otherMoves))    generateMove(startSquare, file+2, rank, moves, color);
    }
}
void Board::generatePawnMoves(Square& startSquare, MoveVec& moves, uint64_t whites, uint64_t blacks) const{
    auto rank = startSquare.rank();
    auto file = startSquare.file();
    PieceColor color = getColor(startSquare);
    // pawns can move 1 place forward, and be promoted if on final step
    if(color == PieceColor::White){
        if(rank != 6) generateMove(startSquare, file, rank+1, moves, color, 1); // is pawn, so no capture
        else generateMove(startSquare, file, rank+1, moves, color, 3); // promote if nothing in the way
    }else{
        if(rank != 1) generateMove(startSquare, file, rank-1, moves, color, 1); // is pawn so no capture
        else generateMove(startSquare, file, rank-1, moves, color, 3); // promote if nothing in the way
    }

    // they can move 2 places if in starting position and no piece in between
    uint64_t all = blacks | whites;
    if(color == PieceColor::White && rank == 1 && !((((uint64_t)1<<(startSquare.index()+8)) | ((uint64_t)1<<(startSquare.index()+16))) & all)){
        generateMove(startSquare, file, rank+2, moves, color, 1); // a pawn cannot capture forwards
    }else if(color == PieceColor::Black && rank == 6 && !((((uint64_t)1<<(startSquare.index()-8)) | ((uint64_t)1<<(startSquare.index()-16))) & all)){
        generateMove(startSquare, file, rank-2, moves, color, 1); // a pawn cannot capture forwards
    }

    // they can attack to the side and up on both sides, and possibly be promoted
    if(color == PieceColor::White && ((uint64_t) 1<<(startSquare.index()+7)) & blacks){ // top left
        if(rank != 6) generateMove(startSquare, file-1, rank+1, moves, color);
        else generateMove(startSquare, file-1, rank+1, moves, color, 2);
    }
    if(color == PieceColor::White && ((uint64_t) 1<<(startSquare.index()+9)) & blacks){ // top right
        if(rank != 6) generateMove(startSquare, file+1, rank+1, moves, color);
        else generateMove(startSquare, file+1, rank+1, moves, color, 2);
    }
    if(color == PieceColor::Black && ((uint64_t) 1<<(startSquare.index()-7)) & whites){ // bottom right
        if(rank != 1) generateMove(startSquare, file+1, rank-1, moves, color);
        else generateMove(startSquare, file+1, rank-1, moves, color, 2);
    }
    if(color == PieceColor::Black && ((uint64_t) 1<<(startSquare.index()-9)) & whites){ // bottom left
        if(rank != 1) generateMove(startSquare, file-1, rank-1, moves, color);
        else generateMove(startSquare, file-1, rank-1, moves, color, 2);
    }
    
    // en passant
    // the en passant square should be set
    Square::Optional enpas = enPassantSquare();
    if(!enpas) return;
    // if the pawn is next to an en passant square, an en pasant can be done
    if((file - (*enpas).file() == 1 || (*enpas).file() - file == 1) && ((*enpas).rank() - rank == 1 || rank - (*enpas).rank() == 1)){
        Square::Optional endSquare;
        turn() == PieceColor::White ? endSquare = Square::fromCoordinates((*enpas).file(), rank+1) : endSquare = Square::fromCoordinates((*enpas).file(), rank-1);
        if(!endSquare) return; // should not fire
        Move* move = new Move(startSquare, *endSquare);
        moves.push_back(*move);
    }
}

bool Board::isInCheck(Square& sq, MoveVec& moves) const{
    // need to check if the sqsuare is targeted by one of the moves
    for(Move m : moves){
        if(m.to() == sq){
            return true;
        }
    }
    return false;
}

// add all pseudo legal moves into the moves vector
void Board::pseudoLegalMoves(MoveVec& moves) const {
    
    uint64_t whites = WHITE_PAWNS | WHITE_ROOKS | WHITE_KNIGHTS | WHITE_BISHOPS | WHITE_QUEENS | WHITE_KINGS;
    uint64_t blacks = BLACK_PAWNS | BLACK_ROOKS | BLACK_KNIGHTS | BLACK_BISHOPS | BLACK_QUEENS | BLACK_KINGS;
    
    // need only the moves from the current collor
    if(turn() == PieceColor::White){ 
        MoveGenerator::generateQueenMoves(moves, WHITE_QUEENS, whites, blacks);  
        MoveGenerator::generateBishopMoves(moves, WHITE_BISHOPS, whites, blacks);
        MoveGenerator::generateKnightMoves(moves, WHITE_KNIGHTS, whites);    
        MoveGenerator::generateRookMoves(moves, WHITE_ROOKS, whites, blacks);
        squareVec v;
        generateSquares(v, WHITE_PAWNS);
        for(Square s : v){
            generatePawnMoves(s, moves, whites, blacks);
        }
        squareVec v2;
        generateSquares(v2, WHITE_KINGS);
        for(Square s2 : v2){
            generateKingMoves(s2, moves);
        }
    }else{
        MoveGenerator::generateQueenMoves(moves, BLACK_QUEENS, blacks, whites);
        MoveGenerator::generateBishopMoves(moves, BLACK_BISHOPS, blacks, whites);
        MoveGenerator::generateKnightMoves(moves, BLACK_KNIGHTS, blacks);
        MoveGenerator::generateRookMoves(moves, BLACK_ROOKS, blacks, whites);  
        squareVec v;
        generateSquares(v, BLACK_PAWNS);
        for(Square s : v){
            generatePawnMoves(s, moves, whites, blacks);
        }
        squareVec v2;
        generateSquares(v2, BLACK_KINGS);
        for(Square s2 : v2){
            generateKingMoves(s2, moves);
        }
    }
}

// adds all legal moves from the given square into the moves vector
void Board::pseudoLegalMovesFrom(const Square& from,
                                 Board::MoveVec& moves) const {
    Board::MoveVec pseudoMoves;
    pseudoLegalMoves(pseudoMoves);
    for(Move m : pseudoMoves){
        if(m.from() == from){
            moves.push_back(m);
        }
    }
}

int Board::legalMoves(MoveVec& moves) const {
    MoveVec v;
    pseudoLegalMoves(v);
    PieceColor c = turn();
    for(Move mv : v){
        // check if the move put us in check
        Board b = *this;
        b.makeMove(mv);
        if(!b.isCheck(c)) moves.push_back(mv);
    }
    if(moves.size() == 0){ // if there are no moves, and the king is in check a chekmate occured
        if(isCheck(turn())){
            return 1; // check
        }else{ // if there are no moves and the king is not check a stalemate occured
            return 2; // stalemate
        }
    }
    return 0; // nothing
}

void Board::evadeCheck(MoveVec& moves) const {
    MoveVec v;
    pseudoLegalMoves(v);
    PieceColor c = turn();
    for(Move mv : v){
        // check if the move put the king out of check
        Board b = *this;
        b.makeMove(mv);
        if(!b.isCheck(c)) moves.push_back(mv);
    }
}

std::ostream& operator<<(std::ostream& os, const Board& board) {
    for(int i = 7 ; i >= 0 ; i--){
        for(int j = 0 ; j < 17 ; j++) os << "-";
        os << std::endl;
        for(int j = 7 ; j >= 0 ; j--){
            os << "|";
            Square::Optional s = Square::fromCoordinates(j, i);
            if(!s) continue;
            Piece::Optional p = board.piece(*s);
            if(!p){
                os << " ";
            }else{
                os << *p;
            }
        }
        os << "|" << std::endl;
    }
    for(int j = 0 ; j < 17 ; j++) os << "-";
    return os << std::endl << std::endl;
}

double Board::computeValue() const{
    double materialScore =   
            120 * (countPieces(WHITE_KINGS)-countPieces(BLACK_KINGS))
            + 9 * (countPieces(WHITE_QUEENS)-countPieces(BLACK_QUEENS))
            + 5 * (countPieces(WHITE_ROOKS)-countPieces(BLACK_ROOKS))
            + 3 * (countPieces(WHITE_KNIGHTS)-countPieces(BLACK_KNIGHTS))
            + 3 * (countPieces(WHITE_BISHOPS)-countPieces(BLACK_BISHOPS))
            + 1 * (countPieces(WHITE_PAWNS)-countPieces(BLACK_PAWNS));
    return materialScore * (turn() == PieceColor::White ? 1 : -1);
}

// checks if the player is in check
bool Board::isCheck(PieceColor color) const{
    // we do a reverse check on the king for all possible pieces that could check the king
    uint64_t knights, erooks, ebishops, friendly, bblocks, rblocks, eking;
    int index = 0;
    if(color == PieceColor::White){
        eking = BLACK_KINGS;
        knights = BLACK_KNIGHTS;
        erooks = BLACK_ROOKS | BLACK_QUEENS;
        ebishops = BLACK_BISHOPS | BLACK_QUEENS;
        bblocks = BLACK_KINGS | BLACK_KNIGHTS | BLACK_PAWNS | BLACK_ROOKS;
        rblocks = BLACK_KINGS | BLACK_KNIGHTS | BLACK_PAWNS | BLACK_BISHOPS;
        friendly = WHITE_ROOKS | WHITE_QUEENS | WHITE_BISHOPS | WHITE_PAWNS | WHITE_KNIGHTS;
        index = wKingIndex;
    }else{
        eking = WHITE_KINGS;
        knights = WHITE_KNIGHTS;
        erooks = WHITE_ROOKS | WHITE_QUEENS;
        ebishops = WHITE_BISHOPS | WHITE_QUEENS;
        bblocks = WHITE_KINGS | WHITE_KNIGHTS | WHITE_PAWNS | WHITE_ROOKS;
        rblocks = WHITE_KINGS | WHITE_KNIGHTS | WHITE_PAWNS | WHITE_BISHOPS;
        friendly = BLACK_ROOKS | BLACK_QUEENS | BLACK_BISHOPS | BLACK_PAWNS | BLACK_KNIGHTS;
        index = bKingIndex;
    }
    // for each piece type check if it holds the king in check
    // pawns can only hod the king in check if they are directly beside it
    if(color == PieceColor::Black){
        if((index>>3) > 0 && (
            ((index&7) < 7&&(((uint64_t) 1<< (index - 7)) & WHITE_PAWNS))
        ||  ((index&7) > 0&&(((uint64_t) 1<< (index - 9)) & WHITE_PAWNS))
            )) return true;
    }else{
        if((index>>3) < 7 && (
            ((index&7)>0&&(((uint64_t) 1<< (index + 7)) & BLACK_PAWNS))
        ||  ((index&7) < 7&&(((uint64_t) 1<< (index + 9)) & BLACK_PAWNS))
            )) return true;
    }
    // knights also have fixed spots for attack
    if(((index&7) != 7 && (index>>3) <= 5 && ((uint64_t) 1<< (index + 17)) & knights)
    || ((index&7) != 0 && (index>>3) <= 5 && ((uint64_t) 1<< (index + 15)) & knights)
    || ((index&7) <= 5 && (index>>3) != 7 && ((uint64_t) 1<< (index + 10)) & knights)
    || ((index&7) >= 2 && (index>>3) != 7 && ((uint64_t) 1<< (index + 6)) & knights)
    || ((index&7) <= 5 && (index>>3) != 0 && ((uint64_t) 1<< (index - 6)) & knights) 
    || ((index&7) >= 2 && (index>>3) != 0 && ((uint64_t) 1<< (index - 10)) & knights)
    || ((index&7) != 7 && (index>>3) >= 2 && ((uint64_t) 1<< (index - 15)) & knights) 
    || ((index&7) != 0 && (index>>3) >= 2 && ((uint64_t) 1<< (index - 17)) & knights)) return true;
    // for rooks all directions need to be checked untill a friendly or enemy rook or queen is found or the edge of the board 
    // check up
    for(int i = 1 ; index < 56 && i <= 8 ; i++){
        if(((i*8)+index) > 63) break;// not in bounds of board
        uint64_t cv = (uint64_t) 1<<((i*8)+index); // put 1 above index
        if(cv&erooks) return true; // enemy in sight
        if(cv&(friendly|rblocks)) break; // friendly blokked
    }
    // check down
    for(int i = 1 ; index > 7 && i <= 8 ; i++){
        uint64_t cv = (uint64_t) 1<<(index-(i*8)); // put 1 below index
        if(cv&erooks) return true; // enemy in sight
        if(cv&(friendly|rblocks)) break; // friendly blokked
        if((index-(i*8)) < 8) break;// not in bounds of board
    }
    // std::cout << "rooks survived " << index << "\n" << erooks << "\n";
    // check right
    for(int i = 1 ; ((index&7) < 7) && i <= 8 ; i++){
        uint64_t cv = (uint64_t) 1<<(i+index); // put 1 right of index
        if(cv&erooks) return true; // enemy in sight
        if(cv&(friendly|rblocks)) break; // friendly blokked
        if(((i+index)&7) == 7) break;// not in bounds of board
    }
    // check left
    for(int i = 1 ; ((index&7) > 0) && i <= 8 ; i++){
        uint64_t cv = (uint64_t) 1<<(index-i);
        if(cv&erooks) return true; // enemy in sight
        if(cv&(friendly|rblocks)) break; // friendly blokked
        if(((index-i)&7) == 0) break;// not in bounds of board
    }
    // for bishops all directions need to be checked until a friendly or enemy bishop or queen is found or the edge of the board
    // check top left
    for(int i = 1 ; index < 56 && i <= 8 ; i++){
        if((((i*7)+index)>>3) == 8) break;// not in bounds of board: upper bound
        if((((i*7)+index)&7) == 7) break;// not in bounds of board: left bound
        uint64_t cv = (uint64_t) 1<<((i*7)+index);
        if(cv&ebishops) return true; // enemy in sight
        if(cv&(friendly|bblocks)) break; // friendly blokked
    }
    // check top right
    for(int i = 1 ; index < 56 && i <= 8 ; i++){
        if((((i*9)+index)>>3) == 8) break;// not in bounds of board: upper bound
        if((((i*9)+index)&7) == 0) break;// not in bounds of board: right bound
        uint64_t cv = (uint64_t) 1<<((i*9)+index);
        if(cv&ebishops) return true; // enemy in sight
        if(cv&(friendly|bblocks)) break; // friendly blokked
    }
    // check bottom right
    for(int i = 1 ; index > 7 && i <= 8 ; i++){
        if(((index-(i*7))&7) == 0) break;// not in bounds of board: right bound
        uint64_t cv = (uint64_t) 1<<(index-(i*7));
        if(cv&ebishops) return true; // enemy in sight
        if(cv&(friendly|bblocks)) break; // friendly blokked
        if(((index-(i*7))>>3) == 0) break;// not in bounds of board: bottom bound
    }
    // check bottom left
    for(int i = 1 ; index > 7 && i <= 8 ; i++){
        if((((index-(i*9)))&7) == 7) break;// not in bounds of board: left bound
        uint64_t cv = (uint64_t) 1<<(index-(i*9));
        if(cv&ebishops) return true; // enemy in sight
        if(cv&(friendly|bblocks)) break; // friendly blokked
        if(((index-(i*9))>>3) == 0) break;// not in bounds of board: bottom bound
    }
    // king should be checked to remove illegal moves to be made
    // check all spots next to this king for the other king
    if(((index&7) < 7 && ((uint64_t) 1<<(index+1))&eking)
    || ((index&7) > 0 && ((uint64_t) 1<<(index-1))&eking)
    || ((index>>3) < 7 && ((uint64_t) 1<<(index+8))&eking)
    || ((index>>3) > 0 && ((uint64_t) 1<<(index-8))&eking)
    || ((index&7) < 7 && (index>>3) < 7 && ((uint64_t) 1<<(index+9))&eking)
    || ((index&7) < 7 && (index>>3) > 0 && ((uint64_t) 1<<(index-7))&eking)
    || ((index&7) > 0 && (index>>3) < 7 && ((uint64_t) 1<<(index+7))&eking)
    || ((index&7) > 0 && (index>>3) > 0 && ((uint64_t) 1<<(index-9))&eking)) return true;
    return false; // no check
}

int Board::countPieces(uint64_t pieces) const{
    int bits = 0;
    for (; pieces; ++bits) pieces &= pieces-1; // rightmost set bit removal
    return bits;
}