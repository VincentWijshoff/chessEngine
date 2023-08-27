#include "EngineFactory.hpp"
#include "Ownclass.cpp"

std::unique_ptr<Engine> EngineFactory::createEngine() {
    return std::make_unique<Ownclass>();
}
