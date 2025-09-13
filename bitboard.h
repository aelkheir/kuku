#ifndef INCLUDE_KUKU_BITBOARD_H_
#define INCLUDE_KUKU_BITBOARD_H_

#include "types.h"
#include <cassert>
#include <string>

namespace Kuku {
namespace Bitboards {
std::string pretty(Bitboard b);
}

[[maybe_unused]] constexpr Bitboard FileABB { 0x0000000000108421ULL };
[[maybe_unused]] constexpr Bitboard FileBBB { FileABB << 1 };
[[maybe_unused]] constexpr Bitboard FileCBB { FileABB << 2 };
[[maybe_unused]] constexpr Bitboard FileDBB { FileABB << 3 };
[[maybe_unused]] constexpr Bitboard FileEBB { FileABB << 4 };
constexpr Bitboard OffBoard { ~(FileABB | FileBBB | FileCBB | FileDBB | FileEBB) };

[[maybe_unused]] constexpr Bitboard FileABBB { FileABB | FileBBB };
[[maybe_unused]] constexpr Bitboard FileDEBB { FileDBB | FileEBB };

constexpr Bitboard square_bb(Square s) { return (1ULL << s); }

inline Bitboard operator&(Bitboard b, Square s) { return b & square_bb(s); }
inline Bitboard operator|(Bitboard b, Square s) { return b | square_bb(s); }
inline Bitboard operator^(Bitboard b, Square s) { return b ^ square_bb(s); }
inline Bitboard& operator|=(Bitboard& b, Square s) { return b |= square_bb(s); }
inline Bitboard& operator^=(Bitboard& b, Square s) { return b ^= square_bb(s); }

template <Direction D>
constexpr Bitboard shift(Bitboard b) {
    return D == NORTH         ? b << 5 & ~OffBoard
         : D == SOUTH         ? b >> 5
         : D == NORTH + NORTH ? b << 10 & ~OffBoard
         : D == SOUTH + SOUTH ? b >> 10
         : D == EAST          ? (b & ~FileEBB) << 1
         : D == WEST          ? (b & ~FileABB) >> 1
         : D == EAST + EAST   ? (b & ~FileDEBB) << 2
         : D == WEST + WEST   ? (b & ~FileABBB) >> 2
                              : 0;
}

inline int bit_count(Bitboard bitboard) {
    int count { 0 };
    while (bitboard) {
        count++;
        bitboard &= bitboard - 1;
    }
    return count;
}

inline Square lsb(Bitboard b) {
    assert(b);

#if defined(__GNUC__) // GCC, Clang, ICX

    return Square(__builtin_ctzll(b));

#elif defined(_MSC_VER)
#ifdef _WIN64 // MSVC, WIN64

    unsigned long idx;
    _BitScanForward64(&idx, b);
    return Square(idx);

#else // MSVC, WIN32
    unsigned long idx;

    if (b & 0xffffffff) {
        _BitScanForward(&idx, int32_t(b));
        return Square(idx);
    } else {
        _BitScanForward(&idx, int32_t(b >> 32));
        return Square(idx + 32);
    }
#endif
#else // Compiler is neither GCC nor MSVC compatible
#error "Compiler not supported."
#endif
}

inline Square pop_lsb(Bitboard& b) {
    assert(b);
    const Square s = lsb(b);
    b &= b - 1;
    return s;
}

} // namespace Kuku

#endif // INCLUDE_KUKU_BITBOARD_H_
