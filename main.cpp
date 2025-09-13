#include "bitboard.h"
#include "types.h"
#include <iostream>

int main() {
    std::cout << Kuku::Bitboards::pretty(Kuku::square_bb(Kuku::SQ_A1));
    return 0;
}
