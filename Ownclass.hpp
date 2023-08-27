#ifndef CHESS_ENGINE_OWNCLASS
#define CHESS_ENGINE_OWNCLASS

#include "Engine.hpp"
#include <optional>
#include <vector>
#include <tuple>

class Ownclass : public Engine {
public:

    Ownclass();

    ~Ownclass();

    std::string name() const;
    std::string version() const;
    std::string author() const;

    void newGame();
    PrincipalVariation pv(
        const Board& board,
        const TimeInfo::Optional& timeInfo = std::nullopt
    );

    std::optional<HashInfo> hashInfo() const;
    void setHashSize(std::size_t size);

private:
    int depth_ = 5;

    std::tuple<double, std::vector<Move>, int> negaMax(const Board& b, unsigned depth, double alpha, double beta);
};

#endif