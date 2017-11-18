#include <stdint.h>
#include "font6x8.h"

#define ______ 0,
#define _____O 0x1,
#define ____O_ 0x2,
#define ____OO 0x3,
#define ___O__ 0x4,
#define ___O_O 0x5,
#define ___OO_ 0x6,
#define ___OOO 0x7,
#define __O___ 0x8,
#define __O__O 0x9,
#define __O_O_ 0xa,
#define __O_OO 0xb,
#define __OO__ 0xc,
#define __OO_O 0xd,
#define __OOO_ 0xe,
#define __OOOO 0xf,
#define _O____ 0x10,
#define _O___O 0x11,
#define _O__O_ 0x12,
#define _O__OO 0x13,
#define _O_O__ 0x14,
#define _O_O_O 0x15,
#define _O_OO_ 0x16,
#define _O_OOO 0x17,
#define _OO___ 0x18,
#define _OO__O 0x19,
#define _OO_O_ 0x1a,
#define _OO_OO 0x1b,
#define _OOO__ 0x1c,
#define _OOO_O 0x1d,
#define _OOOO_ 0x1e,
#define _OOOOO 0x1f,
#define O_____ 0x20,
#define O____O 0x21,
#define O___O_ 0x22,
#define O___OO 0x23,
#define O__O__ 0x24,
#define O__O_O 0x25,
#define O__OO_ 0x26,
#define O__OOO 0x27,
#define O_O___ 0x28,
#define O_O__O 0x29,
#define O_O_O_ 0x2a,
#define O_O_OO 0x2b,
#define O_OO__ 0x2c,
#define O_OO_O 0x2d,
#define O_OOO_ 0x2e,
#define O_OOOO 0x2f,
#define OO____ 0x30,
#define OO___O 0x31,
#define OO__O_ 0x32,
#define OO__OO 0x33,
#define OO_O__ 0x34,
#define OO_O_O 0x35,
#define OO_OO_ 0x36,
#define OO_OOO 0x37,
#define OOO___ 0x38,
#define OOO__O 0x39,
#define OOO_O_ 0x3a,
#define OOO_OO 0x3b,
#define OOOO__ 0x3c,
#define OOOO_O 0x3d,
#define OOOOO_ 0x3e,
#define OOOOOO 0x3f,

const uint8_t font6x8_data[98][8] = {
    // ASCII ' ' (0x20)
    [0] = {
        ______
        ______
        ______
        ______
        ______
        ______
        ______
        ______
    },
    // ASCII '!' (0x21)
    [1] = {
        ___O__
        ___O__
        ___O__
        ___O__
        ______
        ______
        ___O__
        ______
    },
    // ASCII '"' (0x22)
    [2] = {
        __O_O_
        __O_O_
        __O_O_
        ______
        ______
        ______
        ______
        ______
    },
    // ASCII '#' (0x23)
    [3] = {
        __O_O_
        __O_O_
        _OOOOO
        __O_O_
        _OOOOO
        __O_O_
        __O_O_
        ______
    },
    // ASCII '$' (0x24)
    [4] = {
        ___O__
        __OOOO
        _O_O__
        __OOO_
        ___O_O
        _OOOO_
        ___O__
        ______
    },
    // ASCII '%' (0x25)
    [5] = {
        _OO___
        _OO__O
        ____O_
        ___O__
        __O___
        _O__OO
        ____OO
        ______
    },
    // ASCII '&' (0x26)
    [6] = {
        __OO__
        _O__O_
        _O_O__
        __O___
        _O_O_O
        _O__O_
        __OO_O
        ______
    },
    // ASCII ''' (0x27)
    [7] = {
        __OO__
        ___O__
        __O___
        ______
        ______
        ______
        ______
        ______
    },
    // ASCII '(' (0x28)
    [8] = {
        ____O_
        ___O__
        __O___
        __O___
        __O___
        ___O__
        ____O_
        ______
    },
    // ASCII ')' (0x29)
    [9] = {
        __O___
        ___O__
        ____O_
        ____O_
        ____O_
        ___O__
        __O___
        ______
    },
    // ASCII '*' (0x2a)
    [10] = {
        ______
        ___O__
        _O_O_O
        __OOO_
        _O_O_O
        ___O__
        ______
        ______
    },
    // ASCII '+' (0x2b)
    [11] = {
        ______
        ___O__
        ___O__
        _OOOOO
        ___O__
        ___O__
        ______
        ______
    },
    // ASCII ',' (0x2c)
    [12] = {
        ______
        ______
        ______
        ______
        __OO__
        ___O__
        __O___
        ______
    },
    // ASCII '-' (0x2d)
    [13] = {
        ______
        ______
        ______
        _OOOOO
        ______
        ______
        ______
        ______
    },
    // ASCII '.' (0x2e)
    [14] = {
        ______
        ______
        ______
        ______
        ______
        __OO__
        __OO__
        ______
    },
    // ASCII '/' (0x2f)
    [15] = {
        ______
        _____O
        ____O_
        ___O__
        __O___
        _O____
        ______
        ______
    },
    // ASCII '0' (0x30)
    [16] = {
        __OOO_
        _O___O
        _O__OO
        _O_O_O
        _OO__O
        _O___O
        __OOO_
        ______
    },
    // ASCII '1' (0x31)
    [17] = {
        ___O__
        __OO__
        ___O__
        ___O__
        ___O__
        ___O__
        __OOO_
        ______
    },
    // ASCII '2' (0x32)
    [18] = {
        __OOO_
        _O___O
        _____O
        ____O_
        ___O__
        __O___
        _OOOOO
        ______
    },
    // ASCII '3' (0x33)
    [19] = {
        _OOOOO
        ____O_
        ___O__
        ____O_
        _____O
        _O___O
        __OOO_
        ______
    },
    // ASCII '4' (0x34)
    [20] = {
        ____O_
        ___OO_
        __O_O_
        _O__O_
        _OOOOO
        ____O_
        ____O_
        ______
    },
    // ASCII '5' (0x35)
    [21] = {
        _OOOOO
        _O____
        _O____
        _OOOO_
        _____O
        _O___O
        __OOO_
        ______
    },
    // ASCII '6' (0x36)
    [22] = {
        ___OO_
        __O___
        _O____
        _OOOO_
        _O___O
        _O___O
        __OOO_
        ______
    },
    // ASCII '7' (0x37)
    [23] = {
        _OOOOO
        _____O
        ____O_
        ___O__
        __O___
        __O___
        __O___
        ______
    },
    // ASCII '8' (0x38)
    [24] = {
        __OOO_
        _O___O
        _O___O
        __OOO_
        _O___O
        _O___O
        __OOO_
        ______
    },
    // ASCII '9' (0x39)
    [25] = {
        __OOO_
        _O___O
        _O___O
        __OOOO
        _____O
        ____O_
        __OO__
        ______
    },
    // ASCII ':' (0x3a)
    [26] = {
        ______
        __OO__
        __OO__
        ______
        __OO__
        __OO__
        ______
        ______
    },
    // ASCII ';' (0x3b)
    [27] = {
        ______
        __OO__
        __OO__
        ______
        __OO__
        ___O__
        __O___
        ______
    },
    // ASCII '<' (0x3c)
    [28] = {
        ____O_
        ___O__
        __O___
        _O____
        __O___
        ___O__
        ____O_
        ______
    },
    // ASCII '=' (0x3d)
    [29] = {
        ______
        ______
        _OOOOO
        ______
        _OOOOO
        ______
        ______
        ______
    },
    // ASCII '>' (0x3e)
    [30] = {
        _O____
        __O___
        ___O__
        ____O_
        ___O__
        __O___
        _O____
        ______
    },
    // ASCII '?' (0x3f)
    [31] = {
        __OOO_
        _O___O
        _____O
        ____O_
        ___O__
        ______
        ___O__
        ______
    },
    // ASCII '@' (0x40)
    [32] = {
        __OOO_
        _O___O
        _____O
        __OO_O
        _O_O_O
        _O_O_O
        __OOO_
        ______
    },
    // ASCII 'A' (0x41)
    [33] = {
        __OOO_
        _O___O
        _O___O
        _O___O
        _OOOOO
        _O___O
        _O___O
        ______
    },
    // ASCII 'B' (0x42)
    [34] = {
        _OOOO_
        _O___O
        _O___O
        _OOOO_
        _O___O
        _O___O
        _OOOO_
        ______
    },
    // ASCII 'C' (0x43)
    [35] = {
        __OOO_
        _O___O
        _O____
        _O____
        _O____
        _O___O
        __OOO_
        ______
    },
    // ASCII 'D' (0x44)
    [36] = {
        _OOO__
        _O__O_
        _O___O
        _O___O
        _O___O
        _O__O_
        _OOO__
        ______
    },
    // ASCII 'E' (0x45)
    [37] = {
        _OOOOO
        _O____
        _O____
        _OOOO_
        _O____
        _O____
        _OOOOO
        ______
    },
    // ASCII 'F' (0x46)
    [38] = {
        _OOOOO
        _O____
        _O____
        _OOOO_
        _O____
        _O____
        _O____
        ______
    },
    // ASCII 'G' (0x47)
    [39] = {
        __OOO_
        _O___O
        _O____
        _O_OOO
        _O___O
        _O___O
        __OOOO
        ______
    },
    // ASCII 'H' (0x48)
    [40] = {
        _O___O
        _O___O
        _O___O
        _OOOOO
        _O___O
        _O___O
        _O___O
        ______
    },
    // ASCII 'I' (0x49)
    [41] = {
        __OOO_
        ___O__
        ___O__
        ___O__
        ___O__
        ___O__
        __OOO_
        ______
    },
    // ASCII 'J' (0x4a)
    [42] = {
        ___OOO
        ____O_
        ____O_
        ____O_
        ____O_
        _O__O_
        __OO__
        ______
    },
    // ASCII 'K' (0x4b)
    [43] = {
        _O___O
        _O__O_
        _O_O__
        _OO___
        _O_O__
        _O__O_
        _O___O
        ______
    },
    // ASCII 'L' (0x4c)
    [44] = {
        _O____
        _O____
        _O____
        _O____
        _O____
        _O____
        _OOOOO
        ______
    },
    // ASCII 'M' (0x4d)
    [45] = {
        _O___O
        _OO_OO
        _O_O_O
        _O_O_O
        _O___O
        _O___O
        _O___O
        ______
    },
    // ASCII 'N' (0x4e)
    [46] = {
        _O___O
        _O___O
        _OO__O
        _O_O_O
        _O__OO
        _O___O
        _O___O
        ______
    },
    // ASCII 'O' (0x4f)
    [47] = {
        __OOO_
        _O___O
        _O___O
        _O___O
        _O___O
        _O___O
        __OOO_
        ______
    },
    // ASCII 'P' (0x50)
    [48] = {
        _OOOO_
        _O___O
        _O___O
        _OOOO_
        _O____
        _O____
        _O____
        ______
    },
    // ASCII 'Q' (0x51)
    [49] = {
        __OOO_
        _O___O
        _O___O
        _O___O
        _O_O_O
        _O__O_
        __OO_O
        ______
    },
    // ASCII 'R' (0x52)
    [50] = {
        _OOOO_
        _O___O
        _O___O
        _OOOO_
        _O_O__
        _O__O_
        _O___O
        ______
    },
    // ASCII 'S' (0x53)
    [51] = {
        __OOOO
        _O____
        _O____
        __OOO_
        _____O
        _____O
        _OOOO_
        ______
    },
    // ASCII 'T' (0x54)
    [52] = {
        _OOOOO
        ___O__
        ___O__
        ___O__
        ___O__
        ___O__
        ___O__
        ______
    },
    // ASCII 'U' (0x55)
    [53] = {
        _O___O
        _O___O
        _O___O
        _O___O
        _O___O
        _O___O
        __OOO_
        ______
    },
    // ASCII 'V' (0x56)
    [54] = {
        _O___O
        _O___O
        _O___O
        _O___O
        _O___O
        __O_O_
        ___O__
        ______
    },
    // ASCII 'W' (0x57)
    [55] = {
        _O___O
        _O___O
        _O___O
        _O_O_O
        _O_O_O
        _O_O_O
        __O_O_
        ______
    },
    // ASCII 'X' (0x58)
    [56] = {
        _O___O
        _O___O
        __O_O_
        ___O__
        __O_O_
        _O___O
        _O___O
        ______
    },
    // ASCII 'Y' (0x59)
    [57] = {
        _O___O
        _O___O
        _O___O
        __O_O_
        ___O__
        ___O__
        ___O__
        ______
    },
    // ASCII 'Z' (0x5a)
    [58] = {
        _OOOOO
        _____O
        ____O_
        ___O__
        __O___
        _O____
        _OOOOO
        ______
    },
    // ASCII '[' (0x5b)
    [59] = {
        __OOO_
        __O___
        __O___
        __O___
        __O___
        __O___
        __OOO_
        ______
    },
    // ASCII '\' (0x5c)
    [60] = {
        _O___O
        __O_O_
        _OOOOO
        ___O__
        _OOOOO
        ___O__
        ___O__
        ______
    },
    // ASCII ']' (0x5d)
    [61] = {
        __OOO_
        ____O_
        ____O_
        ____O_
        ____O_
        ____O_
        __OOO_
        ______
    },
    // ASCII '^' (0x5e)
    [62] = {
        ___O__
        __O_O_
        _O___O
        ______
        ______
        ______
        ______
        ______
    },
    // ASCII '_' (0x5f)
    [63] = {
        ______
        ______
        ______
        ______
        ______
        ______
        _OOOOO
        ______
    },
    // ASCII '`' (0x60)
    [64] = {
        __O___
        ___O__
        ____O_
        ______
        ______
        ______
        ______
        ______
    },
    // ASCII 'a' (0x61)
    [65] = {
        ______
        ______
        __OOO_
        _____O
        __OOOO
        _O___O
        __OOOO
        ______
    },
    // ASCII 'b' (0x62)
    [66] = {
        _O____
        _O____
        _O_OO_
        _OO__O
        _O___O
        _O___O
        _OOOO_
        ______
    },
    // ASCII 'c' (0x63)
    [67] = {
        ______
        ______
        __OOO_
        _O____
        _O____
        _O___O
        __OOO_
        ______
    },
    // ASCII 'd' (0x64)
    [68] = {
        _____O
        _____O
        __OO_O
        _O__OO
        _O___O
        _O___O
        __OOOO
        ______
    },
    // ASCII 'e' (0x65)
    [69] = {
        ______
        ______
        __OOO_
        _O___O
        _OOOOO
        _O____
        __OOO_
        ______
    },
    // ASCII 'f' (0x66)
    [70] = {
        ___OO_
        __O__O
        __O___
        _OOO__
        __O___
        __O___
        __O___
        ______
    },
    // ASCII 'g' (0x67)
    [71] = {
        ______
        ______
        __OOOO
        _O___O
        _O___O
        __OOOO
        _____O
        __OOO_
    },
    // ASCII 'h' (0x68)
    [72] = {
        _O____
        _O____
        _O_OO_
        _OO__O
        _O___O
        _O___O
        _O___O
        ______
    },
    // ASCII 'i' (0x69)
    [73] = {
        ___O__
        ______
        __OO__
        ___O__
        ___O__
        ___O__
        __OOO_
        ______
    },
    // ASCII 'j' (0x6a)
    [74] = {
        ____O_
        ______
        ___OO_
        ____O_
        ____O_
        _O__O_
        __OO__
        ______
    },
    // ASCII 'k' (0x6b)
    [75] = {
        __O___
        __O___
        __O__O
        __O_O_
        __OO__
        __O_O_
        __O__O
        ______
    },
    // ASCII 'l' (0x6c)
    [76] = {
        __OO__
        ___O__
        ___O__
        ___O__
        ___O__
        ___O__
        __OOO_
        ______
    },
    // ASCII 'm' (0x6d)
    [77] = {
        ______
        ______
        _OO_O_
        _O_O_O
        _O_O_O
        _O___O
        _O___O
        ______
    },
    // ASCII 'n' (0x6e)
    [78] = {
        ______
        ______
        _OOOO_
        _O___O
        _O___O
        _O___O
        _O___O
        ______
    },
    // ASCII 'o' (0x6f)
    [79] = {
        ______
        ______
        __OOO_
        _O___O
        _O___O
        _O___O
        __OOO_
        ______
    },
    // ASCII 'p' (0x70)
    [80] = {
        ______
        ______
        _OOOO_
        _O___O
        _O___O
        _OOOO_
        _O____
        _O____
    },
    // ASCII 'q' (0x71)
    [81] = {
        ______
        ______
        __OO_O
        _O__OO
        _O___O
        __OOOO
        _____O
        _____O
    },
    // ASCII 'r' (0x72)
    [82] = {
        ______
        ______
        _O_OO_
        _OO__O
        _O____
        _O____
        _O____
        ______
    },
    // ASCII 's' (0x73)
    [83] = {
        ______
        ______
        __OOO_
        _O____
        __OOO_
        _____O
        _OOOO_
        ______
    },
    // ASCII 't' (0x74)
    [84] = {
        __O___
        __O___
        _OOO__
        __O___
        __O___
        __O__O
        ___OO_
        ______
    },
    // ASCII 'u' (0x75)
    [85] = {
        ______
        ______
        _O___O
        _O___O
        _O___O
        _O__OO
        __OO_O
        ______
    },
    // ASCII 'v' (0x76)
    [86] = {
        ______
        ______
        _O___O
        _O___O
        _O___O
        __O_O_
        ___O__
        ______
    },
    // ASCII 'w' (0x77)
    [87] = {
        ______
        ______
        _O___O
        _O___O
        _O___O
        _O_O_O
        __O_O_
        ______
    },
    // ASCII 'x' (0x78)
    [88] = {
        ______
        ______
        _O___O
        __O_O_
        ___O__
        __O_O_
        _O___O
        ______
    },
    // ASCII 'y' (0x79)
    [89] = {
        ______
        ______
        _O___O
        _O___O
        __OOOO
        _____O
        __OOO_
        ______
    },
    // ASCII 'z' (0x7a)
    [90] = {
        ______
        ______
        _OOOOO
        ____O_
        ___O__
        __O___
        _OOOOO
        ______
    },
    // ASCII '{' (0x7b)
    [91] = {
        ____O_
        ___O__
        ___O__
        __O___
        ___O__
        ___O__
        ____O_
        ______
    },
    // ASCII '|' (0x7c)
    [92] = {
        ___O__
        ___O__
        ___O__
        ___O__
        ___O__
        ___O__
        ___O__
        ______
    },
    // ASCII '}' (0x7d)
    [93] = {
        __O___
        ___O__
        ___O__
        ____O_
        ___O__
        ___O__
        __O___
        ______
    },
    // ASCII '~' (0x7e)
    [94] = {
        ______
        __O__O
        _O_O_O
        _O__O_
        ______
        ______
        ______
        ______
    },
    // RIGHT ARROW (0x80)
    [96] = {
        ______
        ___O__
        ____O_
        _OOOOO
        ____O_
        ___O__
        ______
        ______
    },
    // LEFT ARROW (0x81)
    [97] = {
        ______
        ___O__
        __O___
        _OOOOO
        __O___
        ___O__
        ______
        ______
    },
};

const font_t font6x8 = {
    .data   = (const uint8_t *) font6x8_data,
    .width  = 6,
    .height = 8,
    .bytes  = 1,
    .first  = 32,
    .last   = 129,
};
