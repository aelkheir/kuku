#include "bitboard.h"
#include "types.h"
#include <string>

namespace Kuku {
std::string Bitboards::pretty(Bitboard b) {
    std::string s {};
    s += "\n +---+---+---+---+---+\n";
    for (Rank r { RANK_5 }; r >= RANK_1; --r) {
        for (File f { FILE_A }; f <= FILE_E; ++f)
            s += " | " + std::string((b & make_square(r, f) ? "X" : " "));
        s += " | " + std::to_string(1 + r) + "\n +---+---+---+---+---+\n";
    }
    s += "   a   b   c   d   e  \n";
    return s;
}
} // namespace Kuku
