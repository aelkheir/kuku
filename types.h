#ifndef INCLUDE_KUKU_TYPES_H_
#define INCLUDE_KUKU_TYPES_H_

#include <array>
#include <cassert>
#include <cstdint>
#include <string_view>
#include <sys/types.h>

namespace Kuku {
constexpr int DOG_VALUE { 100 };
constexpr int MAX_MOVES { 256 };

constexpr int GAME_OVER { 99'999 };

constexpr auto starting_position { "bbbbb/bbbbb/ww1bb/wwwww/wwwww w" };
constexpr auto position_a { "5/3b1/1b1w1/b1b1b/wb1bw w" };
constexpr auto position_b { "1b1bw/b1bb1/1bww1/b1b1b/wb1bw w" };
constexpr auto position_c { "1b3/b1b2/1b3/b4/w4 w" };
constexpr auto position_d { "b3w/b4/5/bw3/w4 w" };

using Bitboard = uint64_t;

enum Color { WHITE, BLACK, COLOR_NB = 2 };

enum Piece {
    NO_PIECE,
    W_DOG,
    B_DOG,

    PIECE_NB = 3
};

enum Rank {
    RANK_1,
    RANK_2,
    RANK_3,
    RANK_4,
    RANK_5,
};

// clang-format off
enum File  {
    FILE_A, FILE_B, FILE_C, FILE_D, FILE_E,
};

enum Square {
    SQ_A1, SQ_B1, SQ_C1, SQ_D1, SQ_E1,
    SQ_A2, SQ_B2, SQ_C2, SQ_D2, SQ_E2,
    SQ_A3, SQ_B3, SQ_C3, SQ_D3, SQ_E3,
    SQ_A4, SQ_B4, SQ_C4, SQ_D4, SQ_E4,
    SQ_A5, SQ_B5, SQ_C5, SQ_D5, SQ_E5,
    SQ_NONE,

    SQUARE_NB = 25,
};
// clang-format on

enum Direction : int {
    NORTH = 5,
    EAST = 1,
    SOUTH = -NORTH,
    WEST = -EAST,
};

constexpr Color operator~(Color c) { return c == WHITE ? BLACK : WHITE; }

constexpr Direction operator~(Direction d) {
    return d == NORTH ? SOUTH
         : d == SOUTH ? NORTH
         : d == EAST  ? WEST
         : d == WEST  ? EAST
                      : Direction(0);
}

constexpr File& operator++(File& f) { return (f = static_cast<File>(f + 1)); }
constexpr File& operator--(File& f) { return (f = static_cast<File>(f - 1)); }
constexpr Rank& operator++(Rank& r) { return (r = static_cast<Rank>(r + 1)); }
constexpr Rank& operator--(Rank& r) { return (r = static_cast<Rank>(r - 1)); }
constexpr Square& operator++(Square& s) { return (s = static_cast<Square>(s + 1)); }
constexpr Square& operator--(Square& s) { return (s = static_cast<Square>(s - 1)); }

constexpr Direction operator+(Direction d1, Direction d2) { return Direction(int(d1) + int(d2)); }
constexpr Square operator+(Square s, Direction d) { return Square(int(s) + int(d)); }
constexpr Square operator-(Square s, Direction d) { return Square(int(s) - int(d)); }

inline Square& operator+=(Square& s, Direction d) { return s = s + d; }
inline Square& operator-=(Square& s, Direction d) { return s = s - d; }

constexpr Direction operator*(int i, Direction d) { return Direction(i * int(d)); }

constexpr Square make_square(Rank r, File f) { return Square(r * 5 + f); }

inline Color color_of(Piece pc) {
    assert(pc != NO_PIECE);
    return pc == W_DOG ? Color::WHITE : Color::BLACK;
}
// clang-format off
constexpr std::array<std::string_view, 25> SQUARE_TO_COORDINATES{
    "A1", "B1", "C1", "D1", "E1",
    "A2", "B2", "C2", "D2", "E2",
    "A3", "B3", "C3", "D3", "E3",
    "A4", "B4", "C4", "D4", "E4",
    "A5", "B5", "C5", "D5", "E5",
};

// clang-format on

constexpr std::string_view pretty_square(Square s) { return SQUARE_TO_COORDINATES[s]; }

class Move {
public:
    Move() = default;
    explicit constexpr Move(uint64_t d) :
        data { d } {}
    constexpr Move(Square from, Square to, Piece piece, Bitboard capture = 0) {
        data = (capture << 12) + (uint(piece) << 10) + (uint64_t(to) << 5) + uint64_t(from);
    }

    constexpr Square from_sq() const { return Square(data & 0x1F); }
    constexpr Square to_sq() const { return Square((data >> 5) & 0x1F); }
    constexpr Piece piece() const { return Piece((data >> 10) & 3); }
    constexpr Bitboard capture() const { return data >> 12; }

    constexpr uint64_t raw() const { return data; }

private:
    uint64_t data {};
};

} // namespace Kuku

#endif // INCLUDE_KUKU_TYPES_H_
