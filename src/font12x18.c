#include <stdint.h>
#include "font12x18.h"

#define ____________ 0,0,
#define ___________O 0,0x1,
#define __________O_ 0,0x2,
#define __________OO 0,0x3,
#define _________O__ 0,0x4,
#define _________O_O 0,0x5,
#define _________OO_ 0,0x6,
#define _________OOO 0,0x7,
#define ________O___ 0,0x8,
#define ________O__O 0,0x9,
#define ________O_O_ 0,0xa,
#define ________O_OO 0,0xb,
#define ________OO__ 0,0xc,
#define ________OO_O 0,0xd,
#define ________OOO_ 0,0xe,
#define ________OOOO 0,0xf,
#define _______O____ 0,0x10,
#define _______O___O 0,0x11,
#define _______O__O_ 0,0x12,
#define _______O__OO 0,0x13,
#define _______O_O__ 0,0x14,
#define _______O_O_O 0,0x15,
#define _______O_OO_ 0,0x16,
#define _______O_OOO 0,0x17,
#define _______OO___ 0,0x18,
#define _______OO__O 0,0x19,
#define _______OO_O_ 0,0x1a,
#define _______OO_OO 0,0x1b,
#define _______OOO__ 0,0x1c,
#define _______OOO_O 0,0x1d,
#define _______OOOO_ 0,0x1e,
#define _______OOOOO 0,0x1f,
#define ______O_____ 0,0x20,
#define ______O____O 0,0x21,
#define ______O___O_ 0,0x22,
#define ______O___OO 0,0x23,
#define ______O__O__ 0,0x24,
#define ______O__O_O 0,0x25,
#define ______O__OO_ 0,0x26,
#define ______O__OOO 0,0x27,
#define ______O_O___ 0,0x28,
#define ______O_O__O 0,0x29,
#define ______O_O_O_ 0,0x2a,
#define ______O_O_OO 0,0x2b,
#define ______O_OO__ 0,0x2c,
#define ______O_OO_O 0,0x2d,
#define ______O_OOO_ 0,0x2e,
#define ______O_OOOO 0,0x2f,
#define ______OO____ 0,0x30,
#define ______OO___O 0,0x31,
#define ______OO__O_ 0,0x32,
#define ______OO__OO 0,0x33,
#define ______OO_O__ 0,0x34,
#define ______OO_O_O 0,0x35,
#define ______OO_OO_ 0,0x36,
#define ______OO_OOO 0,0x37,
#define ______OOO___ 0,0x38,
#define ______OOO__O 0,0x39,
#define ______OOO_O_ 0,0x3a,
#define ______OOO_OO 0,0x3b,
#define ______OOOO__ 0,0x3c,
#define ______OOOO_O 0,0x3d,
#define ______OOOOO_ 0,0x3e,
#define ______OOOOOO 0,0x3f,
#define _____O______ 0,0x40,
#define _____O_____O 0,0x41,
#define _____O____O_ 0,0x42,
#define _____O____OO 0,0x43,
#define _____O___O__ 0,0x44,
#define _____O___O_O 0,0x45,
#define _____O___OO_ 0,0x46,
#define _____O___OOO 0,0x47,
#define _____O__O___ 0,0x48,
#define _____O__O__O 0,0x49,
#define _____O__O_O_ 0,0x4a,
#define _____O__O_OO 0,0x4b,
#define _____O__OO__ 0,0x4c,
#define _____O__OO_O 0,0x4d,
#define _____O__OOO_ 0,0x4e,
#define _____O__OOOO 0,0x4f,
#define _____O_O____ 0,0x50,
#define _____O_O___O 0,0x51,
#define _____O_O__O_ 0,0x52,
#define _____O_O__OO 0,0x53,
#define _____O_O_O__ 0,0x54,
#define _____O_O_O_O 0,0x55,
#define _____O_O_OO_ 0,0x56,
#define _____O_O_OOO 0,0x57,
#define _____O_OO___ 0,0x58,
#define _____O_OO__O 0,0x59,
#define _____O_OO_O_ 0,0x5a,
#define _____O_OO_OO 0,0x5b,
#define _____O_OOO__ 0,0x5c,
#define _____O_OOO_O 0,0x5d,
#define _____O_OOOO_ 0,0x5e,
#define _____O_OOOOO 0,0x5f,
#define _____OO_____ 0,0x60,
#define _____OO____O 0,0x61,
#define _____OO___O_ 0,0x62,
#define _____OO___OO 0,0x63,
#define _____OO__O__ 0,0x64,
#define _____OO__O_O 0,0x65,
#define _____OO__OO_ 0,0x66,
#define _____OO__OOO 0,0x67,
#define _____OO_O___ 0,0x68,
#define _____OO_O__O 0,0x69,
#define _____OO_O_O_ 0,0x6a,
#define _____OO_O_OO 0,0x6b,
#define _____OO_OO__ 0,0x6c,
#define _____OO_OO_O 0,0x6d,
#define _____OO_OOO_ 0,0x6e,
#define _____OO_OOOO 0,0x6f,
#define _____OOO____ 0,0x70,
#define _____OOO___O 0,0x71,
#define _____OOO__O_ 0,0x72,
#define _____OOO__OO 0,0x73,
#define _____OOO_O__ 0,0x74,
#define _____OOO_O_O 0,0x75,
#define _____OOO_OO_ 0,0x76,
#define _____OOO_OOO 0,0x77,
#define _____OOOO___ 0,0x78,
#define _____OOOO__O 0,0x79,
#define _____OOOO_O_ 0,0x7a,
#define _____OOOO_OO 0,0x7b,
#define _____OOOOO__ 0,0x7c,
#define _____OOOOO_O 0,0x7d,
#define _____OOOOOO_ 0,0x7e,
#define _____OOOOOOO 0,0x7f,
#define ____O_______ 0,0x80,
#define ____O______O 0,0x81,
#define ____O_____O_ 0,0x82,
#define ____O_____OO 0,0x83,
#define ____O____O__ 0,0x84,
#define ____O____O_O 0,0x85,
#define ____O____OO_ 0,0x86,
#define ____O____OOO 0,0x87,
#define ____O___O___ 0,0x88,
#define ____O___O__O 0,0x89,
#define ____O___O_O_ 0,0x8a,
#define ____O___O_OO 0,0x8b,
#define ____O___OO__ 0,0x8c,
#define ____O___OO_O 0,0x8d,
#define ____O___OOO_ 0,0x8e,
#define ____O___OOOO 0,0x8f,
#define ____O__O____ 0,0x90,
#define ____O__O___O 0,0x91,
#define ____O__O__O_ 0,0x92,
#define ____O__O__OO 0,0x93,
#define ____O__O_O__ 0,0x94,
#define ____O__O_O_O 0,0x95,
#define ____O__O_OO_ 0,0x96,
#define ____O__O_OOO 0,0x97,
#define ____O__OO___ 0,0x98,
#define ____O__OO__O 0,0x99,
#define ____O__OO_O_ 0,0x9a,
#define ____O__OO_OO 0,0x9b,
#define ____O__OOO__ 0,0x9c,
#define ____O__OOO_O 0,0x9d,
#define ____O__OOOO_ 0,0x9e,
#define ____O__OOOOO 0,0x9f,
#define ____O_O_____ 0,0xa0,
#define ____O_O____O 0,0xa1,
#define ____O_O___O_ 0,0xa2,
#define ____O_O___OO 0,0xa3,
#define ____O_O__O__ 0,0xa4,
#define ____O_O__O_O 0,0xa5,
#define ____O_O__OO_ 0,0xa6,
#define ____O_O__OOO 0,0xa7,
#define ____O_O_O___ 0,0xa8,
#define ____O_O_O__O 0,0xa9,
#define ____O_O_O_O_ 0,0xaa,
#define ____O_O_O_OO 0,0xab,
#define ____O_O_OO__ 0,0xac,
#define ____O_O_OO_O 0,0xad,
#define ____O_O_OOO_ 0,0xae,
#define ____O_O_OOOO 0,0xaf,
#define ____O_OO____ 0,0xb0,
#define ____O_OO___O 0,0xb1,
#define ____O_OO__O_ 0,0xb2,
#define ____O_OO__OO 0,0xb3,
#define ____O_OO_O__ 0,0xb4,
#define ____O_OO_O_O 0,0xb5,
#define ____O_OO_OO_ 0,0xb6,
#define ____O_OO_OOO 0,0xb7,
#define ____O_OOO___ 0,0xb8,
#define ____O_OOO__O 0,0xb9,
#define ____O_OOO_O_ 0,0xba,
#define ____O_OOO_OO 0,0xbb,
#define ____O_OOOO__ 0,0xbc,
#define ____O_OOOO_O 0,0xbd,
#define ____O_OOOOO_ 0,0xbe,
#define ____O_OOOOOO 0,0xbf,
#define ____OO______ 0,0xc0,
#define ____OO_____O 0,0xc1,
#define ____OO____O_ 0,0xc2,
#define ____OO____OO 0,0xc3,
#define ____OO___O__ 0,0xc4,
#define ____OO___O_O 0,0xc5,
#define ____OO___OO_ 0,0xc6,
#define ____OO___OOO 0,0xc7,
#define ____OO__O___ 0,0xc8,
#define ____OO__O__O 0,0xc9,
#define ____OO__O_O_ 0,0xca,
#define ____OO__O_OO 0,0xcb,
#define ____OO__OO__ 0,0xcc,
#define ____OO__OO_O 0,0xcd,
#define ____OO__OOO_ 0,0xce,
#define ____OO__OOOO 0,0xcf,
#define ____OO_O____ 0,0xd0,
#define ____OO_O___O 0,0xd1,
#define ____OO_O__O_ 0,0xd2,
#define ____OO_O__OO 0,0xd3,
#define ____OO_O_O__ 0,0xd4,
#define ____OO_O_O_O 0,0xd5,
#define ____OO_O_OO_ 0,0xd6,
#define ____OO_O_OOO 0,0xd7,
#define ____OO_OO___ 0,0xd8,
#define ____OO_OO__O 0,0xd9,
#define ____OO_OO_O_ 0,0xda,
#define ____OO_OO_OO 0,0xdb,
#define ____OO_OOO__ 0,0xdc,
#define ____OO_OOO_O 0,0xdd,
#define ____OO_OOOO_ 0,0xde,
#define ____OO_OOOOO 0,0xdf,
#define ____OOO_____ 0,0xe0,
#define ____OOO____O 0,0xe1,
#define ____OOO___O_ 0,0xe2,
#define ____OOO___OO 0,0xe3,
#define ____OOO__O__ 0,0xe4,
#define ____OOO__O_O 0,0xe5,
#define ____OOO__OO_ 0,0xe6,
#define ____OOO__OOO 0,0xe7,
#define ____OOO_O___ 0,0xe8,
#define ____OOO_O__O 0,0xe9,
#define ____OOO_O_O_ 0,0xea,
#define ____OOO_O_OO 0,0xeb,
#define ____OOO_OO__ 0,0xec,
#define ____OOO_OO_O 0,0xed,
#define ____OOO_OOO_ 0,0xee,
#define ____OOO_OOOO 0,0xef,
#define ____OOOO____ 0,0xf0,
#define ____OOOO___O 0,0xf1,
#define ____OOOO__O_ 0,0xf2,
#define ____OOOO__OO 0,0xf3,
#define ____OOOO_O__ 0,0xf4,
#define ____OOOO_O_O 0,0xf5,
#define ____OOOO_OO_ 0,0xf6,
#define ____OOOO_OOO 0,0xf7,
#define ____OOOOO___ 0,0xf8,
#define ____OOOOO__O 0,0xf9,
#define ____OOOOO_O_ 0,0xfa,
#define ____OOOOO_OO 0,0xfb,
#define ____OOOOOO__ 0,0xfc,
#define ____OOOOOO_O 0,0xfd,
#define ____OOOOOOO_ 0,0xfe,
#define ____OOOOOOOO 0,0xff,
#define ___O________ 0x1,0,
#define ___O_______O 0x1,0x1,
#define ___O______O_ 0x1,0x2,
#define ___O______OO 0x1,0x3,
#define ___O_____O__ 0x1,0x4,
#define ___O_____O_O 0x1,0x5,
#define ___O_____OO_ 0x1,0x6,
#define ___O_____OOO 0x1,0x7,
#define ___O____O___ 0x1,0x8,
#define ___O____O__O 0x1,0x9,
#define ___O____O_O_ 0x1,0xa,
#define ___O____O_OO 0x1,0xb,
#define ___O____OO__ 0x1,0xc,
#define ___O____OO_O 0x1,0xd,
#define ___O____OOO_ 0x1,0xe,
#define ___O____OOOO 0x1,0xf,
#define ___O___O____ 0x1,0x10,
#define ___O___O___O 0x1,0x11,
#define ___O___O__O_ 0x1,0x12,
#define ___O___O__OO 0x1,0x13,
#define ___O___O_O__ 0x1,0x14,
#define ___O___O_O_O 0x1,0x15,
#define ___O___O_OO_ 0x1,0x16,
#define ___O___O_OOO 0x1,0x17,
#define ___O___OO___ 0x1,0x18,
#define ___O___OO__O 0x1,0x19,
#define ___O___OO_O_ 0x1,0x1a,
#define ___O___OO_OO 0x1,0x1b,
#define ___O___OOO__ 0x1,0x1c,
#define ___O___OOO_O 0x1,0x1d,
#define ___O___OOOO_ 0x1,0x1e,
#define ___O___OOOOO 0x1,0x1f,
#define ___O__O_____ 0x1,0x20,
#define ___O__O____O 0x1,0x21,
#define ___O__O___O_ 0x1,0x22,
#define ___O__O___OO 0x1,0x23,
#define ___O__O__O__ 0x1,0x24,
#define ___O__O__O_O 0x1,0x25,
#define ___O__O__OO_ 0x1,0x26,
#define ___O__O__OOO 0x1,0x27,
#define ___O__O_O___ 0x1,0x28,
#define ___O__O_O__O 0x1,0x29,
#define ___O__O_O_O_ 0x1,0x2a,
#define ___O__O_O_OO 0x1,0x2b,
#define ___O__O_OO__ 0x1,0x2c,
#define ___O__O_OO_O 0x1,0x2d,
#define ___O__O_OOO_ 0x1,0x2e,
#define ___O__O_OOOO 0x1,0x2f,
#define ___O__OO____ 0x1,0x30,
#define ___O__OO___O 0x1,0x31,
#define ___O__OO__O_ 0x1,0x32,
#define ___O__OO__OO 0x1,0x33,
#define ___O__OO_O__ 0x1,0x34,
#define ___O__OO_O_O 0x1,0x35,
#define ___O__OO_OO_ 0x1,0x36,
#define ___O__OO_OOO 0x1,0x37,
#define ___O__OOO___ 0x1,0x38,
#define ___O__OOO__O 0x1,0x39,
#define ___O__OOO_O_ 0x1,0x3a,
#define ___O__OOO_OO 0x1,0x3b,
#define ___O__OOOO__ 0x1,0x3c,
#define ___O__OOOO_O 0x1,0x3d,
#define ___O__OOOOO_ 0x1,0x3e,
#define ___O__OOOOOO 0x1,0x3f,
#define ___O_O______ 0x1,0x40,
#define ___O_O_____O 0x1,0x41,
#define ___O_O____O_ 0x1,0x42,
#define ___O_O____OO 0x1,0x43,
#define ___O_O___O__ 0x1,0x44,
#define ___O_O___O_O 0x1,0x45,
#define ___O_O___OO_ 0x1,0x46,
#define ___O_O___OOO 0x1,0x47,
#define ___O_O__O___ 0x1,0x48,
#define ___O_O__O__O 0x1,0x49,
#define ___O_O__O_O_ 0x1,0x4a,
#define ___O_O__O_OO 0x1,0x4b,
#define ___O_O__OO__ 0x1,0x4c,
#define ___O_O__OO_O 0x1,0x4d,
#define ___O_O__OOO_ 0x1,0x4e,
#define ___O_O__OOOO 0x1,0x4f,
#define ___O_O_O____ 0x1,0x50,
#define ___O_O_O___O 0x1,0x51,
#define ___O_O_O__O_ 0x1,0x52,
#define ___O_O_O__OO 0x1,0x53,
#define ___O_O_O_O__ 0x1,0x54,
#define ___O_O_O_O_O 0x1,0x55,
#define ___O_O_O_OO_ 0x1,0x56,
#define ___O_O_O_OOO 0x1,0x57,
#define ___O_O_OO___ 0x1,0x58,
#define ___O_O_OO__O 0x1,0x59,
#define ___O_O_OO_O_ 0x1,0x5a,
#define ___O_O_OO_OO 0x1,0x5b,
#define ___O_O_OOO__ 0x1,0x5c,
#define ___O_O_OOO_O 0x1,0x5d,
#define ___O_O_OOOO_ 0x1,0x5e,
#define ___O_O_OOOOO 0x1,0x5f,
#define ___O_OO_____ 0x1,0x60,
#define ___O_OO____O 0x1,0x61,
#define ___O_OO___O_ 0x1,0x62,
#define ___O_OO___OO 0x1,0x63,
#define ___O_OO__O__ 0x1,0x64,
#define ___O_OO__O_O 0x1,0x65,
#define ___O_OO__OO_ 0x1,0x66,
#define ___O_OO__OOO 0x1,0x67,
#define ___O_OO_O___ 0x1,0x68,
#define ___O_OO_O__O 0x1,0x69,
#define ___O_OO_O_O_ 0x1,0x6a,
#define ___O_OO_O_OO 0x1,0x6b,
#define ___O_OO_OO__ 0x1,0x6c,
#define ___O_OO_OO_O 0x1,0x6d,
#define ___O_OO_OOO_ 0x1,0x6e,
#define ___O_OO_OOOO 0x1,0x6f,
#define ___O_OOO____ 0x1,0x70,
#define ___O_OOO___O 0x1,0x71,
#define ___O_OOO__O_ 0x1,0x72,
#define ___O_OOO__OO 0x1,0x73,
#define ___O_OOO_O__ 0x1,0x74,
#define ___O_OOO_O_O 0x1,0x75,
#define ___O_OOO_OO_ 0x1,0x76,
#define ___O_OOO_OOO 0x1,0x77,
#define ___O_OOOO___ 0x1,0x78,
#define ___O_OOOO__O 0x1,0x79,
#define ___O_OOOO_O_ 0x1,0x7a,
#define ___O_OOOO_OO 0x1,0x7b,
#define ___O_OOOOO__ 0x1,0x7c,
#define ___O_OOOOO_O 0x1,0x7d,
#define ___O_OOOOOO_ 0x1,0x7e,
#define ___O_OOOOOOO 0x1,0x7f,
#define ___OO_______ 0x1,0x80,
#define ___OO______O 0x1,0x81,
#define ___OO_____O_ 0x1,0x82,
#define ___OO_____OO 0x1,0x83,
#define ___OO____O__ 0x1,0x84,
#define ___OO____O_O 0x1,0x85,
#define ___OO____OO_ 0x1,0x86,
#define ___OO____OOO 0x1,0x87,
#define ___OO___O___ 0x1,0x88,
#define ___OO___O__O 0x1,0x89,
#define ___OO___O_O_ 0x1,0x8a,
#define ___OO___O_OO 0x1,0x8b,
#define ___OO___OO__ 0x1,0x8c,
#define ___OO___OO_O 0x1,0x8d,
#define ___OO___OOO_ 0x1,0x8e,
#define ___OO___OOOO 0x1,0x8f,
#define ___OO__O____ 0x1,0x90,
#define ___OO__O___O 0x1,0x91,
#define ___OO__O__O_ 0x1,0x92,
#define ___OO__O__OO 0x1,0x93,
#define ___OO__O_O__ 0x1,0x94,
#define ___OO__O_O_O 0x1,0x95,
#define ___OO__O_OO_ 0x1,0x96,
#define ___OO__O_OOO 0x1,0x97,
#define ___OO__OO___ 0x1,0x98,
#define ___OO__OO__O 0x1,0x99,
#define ___OO__OO_O_ 0x1,0x9a,
#define ___OO__OO_OO 0x1,0x9b,
#define ___OO__OOO__ 0x1,0x9c,
#define ___OO__OOO_O 0x1,0x9d,
#define ___OO__OOOO_ 0x1,0x9e,
#define ___OO__OOOOO 0x1,0x9f,
#define ___OO_O_____ 0x1,0xa0,
#define ___OO_O____O 0x1,0xa1,
#define ___OO_O___O_ 0x1,0xa2,
#define ___OO_O___OO 0x1,0xa3,
#define ___OO_O__O__ 0x1,0xa4,
#define ___OO_O__O_O 0x1,0xa5,
#define ___OO_O__OO_ 0x1,0xa6,
#define ___OO_O__OOO 0x1,0xa7,
#define ___OO_O_O___ 0x1,0xa8,
#define ___OO_O_O__O 0x1,0xa9,
#define ___OO_O_O_O_ 0x1,0xaa,
#define ___OO_O_O_OO 0x1,0xab,
#define ___OO_O_OO__ 0x1,0xac,
#define ___OO_O_OO_O 0x1,0xad,
#define ___OO_O_OOO_ 0x1,0xae,
#define ___OO_O_OOOO 0x1,0xaf,
#define ___OO_OO____ 0x1,0xb0,
#define ___OO_OO___O 0x1,0xb1,
#define ___OO_OO__O_ 0x1,0xb2,
#define ___OO_OO__OO 0x1,0xb3,
#define ___OO_OO_O__ 0x1,0xb4,
#define ___OO_OO_O_O 0x1,0xb5,
#define ___OO_OO_OO_ 0x1,0xb6,
#define ___OO_OO_OOO 0x1,0xb7,
#define ___OO_OOO___ 0x1,0xb8,
#define ___OO_OOO__O 0x1,0xb9,
#define ___OO_OOO_O_ 0x1,0xba,
#define ___OO_OOO_OO 0x1,0xbb,
#define ___OO_OOOO__ 0x1,0xbc,
#define ___OO_OOOO_O 0x1,0xbd,
#define ___OO_OOOOO_ 0x1,0xbe,
#define ___OO_OOOOOO 0x1,0xbf,
#define ___OOO______ 0x1,0xc0,
#define ___OOO_____O 0x1,0xc1,
#define ___OOO____O_ 0x1,0xc2,
#define ___OOO____OO 0x1,0xc3,
#define ___OOO___O__ 0x1,0xc4,
#define ___OOO___O_O 0x1,0xc5,
#define ___OOO___OO_ 0x1,0xc6,
#define ___OOO___OOO 0x1,0xc7,
#define ___OOO__O___ 0x1,0xc8,
#define ___OOO__O__O 0x1,0xc9,
#define ___OOO__O_O_ 0x1,0xca,
#define ___OOO__O_OO 0x1,0xcb,
#define ___OOO__OO__ 0x1,0xcc,
#define ___OOO__OO_O 0x1,0xcd,
#define ___OOO__OOO_ 0x1,0xce,
#define ___OOO__OOOO 0x1,0xcf,
#define ___OOO_O____ 0x1,0xd0,
#define ___OOO_O___O 0x1,0xd1,
#define ___OOO_O__O_ 0x1,0xd2,
#define ___OOO_O__OO 0x1,0xd3,
#define ___OOO_O_O__ 0x1,0xd4,
#define ___OOO_O_O_O 0x1,0xd5,
#define ___OOO_O_OO_ 0x1,0xd6,
#define ___OOO_O_OOO 0x1,0xd7,
#define ___OOO_OO___ 0x1,0xd8,
#define ___OOO_OO__O 0x1,0xd9,
#define ___OOO_OO_O_ 0x1,0xda,
#define ___OOO_OO_OO 0x1,0xdb,
#define ___OOO_OOO__ 0x1,0xdc,
#define ___OOO_OOO_O 0x1,0xdd,
#define ___OOO_OOOO_ 0x1,0xde,
#define ___OOO_OOOOO 0x1,0xdf,
#define ___OOOO_____ 0x1,0xe0,
#define ___OOOO____O 0x1,0xe1,
#define ___OOOO___O_ 0x1,0xe2,
#define ___OOOO___OO 0x1,0xe3,
#define ___OOOO__O__ 0x1,0xe4,
#define ___OOOO__O_O 0x1,0xe5,
#define ___OOOO__OO_ 0x1,0xe6,
#define ___OOOO__OOO 0x1,0xe7,
#define ___OOOO_O___ 0x1,0xe8,
#define ___OOOO_O__O 0x1,0xe9,
#define ___OOOO_O_O_ 0x1,0xea,
#define ___OOOO_O_OO 0x1,0xeb,
#define ___OOOO_OO__ 0x1,0xec,
#define ___OOOO_OO_O 0x1,0xed,
#define ___OOOO_OOO_ 0x1,0xee,
#define ___OOOO_OOOO 0x1,0xef,
#define ___OOOOO____ 0x1,0xf0,
#define ___OOOOO___O 0x1,0xf1,
#define ___OOOOO__O_ 0x1,0xf2,
#define ___OOOOO__OO 0x1,0xf3,
#define ___OOOOO_O__ 0x1,0xf4,
#define ___OOOOO_O_O 0x1,0xf5,
#define ___OOOOO_OO_ 0x1,0xf6,
#define ___OOOOO_OOO 0x1,0xf7,
#define ___OOOOOO___ 0x1,0xf8,
#define ___OOOOOO__O 0x1,0xf9,
#define ___OOOOOO_O_ 0x1,0xfa,
#define ___OOOOOO_OO 0x1,0xfb,
#define ___OOOOOOO__ 0x1,0xfc,
#define ___OOOOOOO_O 0x1,0xfd,
#define ___OOOOOOOO_ 0x1,0xfe,
#define ___OOOOOOOOO 0x1,0xff,
#define __O_________ 0x2,0,
#define __O________O 0x2,0x1,
#define __O_______O_ 0x2,0x2,
#define __O_______OO 0x2,0x3,
#define __O______O__ 0x2,0x4,
#define __O______O_O 0x2,0x5,
#define __O______OO_ 0x2,0x6,
#define __O______OOO 0x2,0x7,
#define __O_____O___ 0x2,0x8,
#define __O_____O__O 0x2,0x9,
#define __O_____O_O_ 0x2,0xa,
#define __O_____O_OO 0x2,0xb,
#define __O_____OO__ 0x2,0xc,
#define __O_____OO_O 0x2,0xd,
#define __O_____OOO_ 0x2,0xe,
#define __O_____OOOO 0x2,0xf,
#define __O____O____ 0x2,0x10,
#define __O____O___O 0x2,0x11,
#define __O____O__O_ 0x2,0x12,
#define __O____O__OO 0x2,0x13,
#define __O____O_O__ 0x2,0x14,
#define __O____O_O_O 0x2,0x15,
#define __O____O_OO_ 0x2,0x16,
#define __O____O_OOO 0x2,0x17,
#define __O____OO___ 0x2,0x18,
#define __O____OO__O 0x2,0x19,
#define __O____OO_O_ 0x2,0x1a,
#define __O____OO_OO 0x2,0x1b,
#define __O____OOO__ 0x2,0x1c,
#define __O____OOO_O 0x2,0x1d,
#define __O____OOOO_ 0x2,0x1e,
#define __O____OOOOO 0x2,0x1f,
#define __O___O_____ 0x2,0x20,
#define __O___O____O 0x2,0x21,
#define __O___O___O_ 0x2,0x22,
#define __O___O___OO 0x2,0x23,
#define __O___O__O__ 0x2,0x24,
#define __O___O__O_O 0x2,0x25,
#define __O___O__OO_ 0x2,0x26,
#define __O___O__OOO 0x2,0x27,
#define __O___O_O___ 0x2,0x28,
#define __O___O_O__O 0x2,0x29,
#define __O___O_O_O_ 0x2,0x2a,
#define __O___O_O_OO 0x2,0x2b,
#define __O___O_OO__ 0x2,0x2c,
#define __O___O_OO_O 0x2,0x2d,
#define __O___O_OOO_ 0x2,0x2e,
#define __O___O_OOOO 0x2,0x2f,
#define __O___OO____ 0x2,0x30,
#define __O___OO___O 0x2,0x31,
#define __O___OO__O_ 0x2,0x32,
#define __O___OO__OO 0x2,0x33,
#define __O___OO_O__ 0x2,0x34,
#define __O___OO_O_O 0x2,0x35,
#define __O___OO_OO_ 0x2,0x36,
#define __O___OO_OOO 0x2,0x37,
#define __O___OOO___ 0x2,0x38,
#define __O___OOO__O 0x2,0x39,
#define __O___OOO_O_ 0x2,0x3a,
#define __O___OOO_OO 0x2,0x3b,
#define __O___OOOO__ 0x2,0x3c,
#define __O___OOOO_O 0x2,0x3d,
#define __O___OOOOO_ 0x2,0x3e,
#define __O___OOOOOO 0x2,0x3f,
#define __O__O______ 0x2,0x40,
#define __O__O_____O 0x2,0x41,
#define __O__O____O_ 0x2,0x42,
#define __O__O____OO 0x2,0x43,
#define __O__O___O__ 0x2,0x44,
#define __O__O___O_O 0x2,0x45,
#define __O__O___OO_ 0x2,0x46,
#define __O__O___OOO 0x2,0x47,
#define __O__O__O___ 0x2,0x48,
#define __O__O__O__O 0x2,0x49,
#define __O__O__O_O_ 0x2,0x4a,
#define __O__O__O_OO 0x2,0x4b,
#define __O__O__OO__ 0x2,0x4c,
#define __O__O__OO_O 0x2,0x4d,
#define __O__O__OOO_ 0x2,0x4e,
#define __O__O__OOOO 0x2,0x4f,
#define __O__O_O____ 0x2,0x50,
#define __O__O_O___O 0x2,0x51,
#define __O__O_O__O_ 0x2,0x52,
#define __O__O_O__OO 0x2,0x53,
#define __O__O_O_O__ 0x2,0x54,
#define __O__O_O_O_O 0x2,0x55,
#define __O__O_O_OO_ 0x2,0x56,
#define __O__O_O_OOO 0x2,0x57,
#define __O__O_OO___ 0x2,0x58,
#define __O__O_OO__O 0x2,0x59,
#define __O__O_OO_O_ 0x2,0x5a,
#define __O__O_OO_OO 0x2,0x5b,
#define __O__O_OOO__ 0x2,0x5c,
#define __O__O_OOO_O 0x2,0x5d,
#define __O__O_OOOO_ 0x2,0x5e,
#define __O__O_OOOOO 0x2,0x5f,
#define __O__OO_____ 0x2,0x60,
#define __O__OO____O 0x2,0x61,
#define __O__OO___O_ 0x2,0x62,
#define __O__OO___OO 0x2,0x63,
#define __O__OO__O__ 0x2,0x64,
#define __O__OO__O_O 0x2,0x65,
#define __O__OO__OO_ 0x2,0x66,
#define __O__OO__OOO 0x2,0x67,
#define __O__OO_O___ 0x2,0x68,
#define __O__OO_O__O 0x2,0x69,
#define __O__OO_O_O_ 0x2,0x6a,
#define __O__OO_O_OO 0x2,0x6b,
#define __O__OO_OO__ 0x2,0x6c,
#define __O__OO_OO_O 0x2,0x6d,
#define __O__OO_OOO_ 0x2,0x6e,
#define __O__OO_OOOO 0x2,0x6f,
#define __O__OOO____ 0x2,0x70,
#define __O__OOO___O 0x2,0x71,
#define __O__OOO__O_ 0x2,0x72,
#define __O__OOO__OO 0x2,0x73,
#define __O__OOO_O__ 0x2,0x74,
#define __O__OOO_O_O 0x2,0x75,
#define __O__OOO_OO_ 0x2,0x76,
#define __O__OOO_OOO 0x2,0x77,
#define __O__OOOO___ 0x2,0x78,
#define __O__OOOO__O 0x2,0x79,
#define __O__OOOO_O_ 0x2,0x7a,
#define __O__OOOO_OO 0x2,0x7b,
#define __O__OOOOO__ 0x2,0x7c,
#define __O__OOOOO_O 0x2,0x7d,
#define __O__OOOOOO_ 0x2,0x7e,
#define __O__OOOOOOO 0x2,0x7f,
#define __O_O_______ 0x2,0x80,
#define __O_O______O 0x2,0x81,
#define __O_O_____O_ 0x2,0x82,
#define __O_O_____OO 0x2,0x83,
#define __O_O____O__ 0x2,0x84,
#define __O_O____O_O 0x2,0x85,
#define __O_O____OO_ 0x2,0x86,
#define __O_O____OOO 0x2,0x87,
#define __O_O___O___ 0x2,0x88,
#define __O_O___O__O 0x2,0x89,
#define __O_O___O_O_ 0x2,0x8a,
#define __O_O___O_OO 0x2,0x8b,
#define __O_O___OO__ 0x2,0x8c,
#define __O_O___OO_O 0x2,0x8d,
#define __O_O___OOO_ 0x2,0x8e,
#define __O_O___OOOO 0x2,0x8f,
#define __O_O__O____ 0x2,0x90,
#define __O_O__O___O 0x2,0x91,
#define __O_O__O__O_ 0x2,0x92,
#define __O_O__O__OO 0x2,0x93,
#define __O_O__O_O__ 0x2,0x94,
#define __O_O__O_O_O 0x2,0x95,
#define __O_O__O_OO_ 0x2,0x96,
#define __O_O__O_OOO 0x2,0x97,
#define __O_O__OO___ 0x2,0x98,
#define __O_O__OO__O 0x2,0x99,
#define __O_O__OO_O_ 0x2,0x9a,
#define __O_O__OO_OO 0x2,0x9b,
#define __O_O__OOO__ 0x2,0x9c,
#define __O_O__OOO_O 0x2,0x9d,
#define __O_O__OOOO_ 0x2,0x9e,
#define __O_O__OOOOO 0x2,0x9f,
#define __O_O_O_____ 0x2,0xa0,
#define __O_O_O____O 0x2,0xa1,
#define __O_O_O___O_ 0x2,0xa2,
#define __O_O_O___OO 0x2,0xa3,
#define __O_O_O__O__ 0x2,0xa4,
#define __O_O_O__O_O 0x2,0xa5,
#define __O_O_O__OO_ 0x2,0xa6,
#define __O_O_O__OOO 0x2,0xa7,
#define __O_O_O_O___ 0x2,0xa8,
#define __O_O_O_O__O 0x2,0xa9,
#define __O_O_O_O_O_ 0x2,0xaa,
#define __O_O_O_O_OO 0x2,0xab,
#define __O_O_O_OO__ 0x2,0xac,
#define __O_O_O_OO_O 0x2,0xad,
#define __O_O_O_OOO_ 0x2,0xae,
#define __O_O_O_OOOO 0x2,0xaf,
#define __O_O_OO____ 0x2,0xb0,
#define __O_O_OO___O 0x2,0xb1,
#define __O_O_OO__O_ 0x2,0xb2,
#define __O_O_OO__OO 0x2,0xb3,
#define __O_O_OO_O__ 0x2,0xb4,
#define __O_O_OO_O_O 0x2,0xb5,
#define __O_O_OO_OO_ 0x2,0xb6,
#define __O_O_OO_OOO 0x2,0xb7,
#define __O_O_OOO___ 0x2,0xb8,
#define __O_O_OOO__O 0x2,0xb9,
#define __O_O_OOO_O_ 0x2,0xba,
#define __O_O_OOO_OO 0x2,0xbb,
#define __O_O_OOOO__ 0x2,0xbc,
#define __O_O_OOOO_O 0x2,0xbd,
#define __O_O_OOOOO_ 0x2,0xbe,
#define __O_O_OOOOOO 0x2,0xbf,
#define __O_OO______ 0x2,0xc0,
#define __O_OO_____O 0x2,0xc1,
#define __O_OO____O_ 0x2,0xc2,
#define __O_OO____OO 0x2,0xc3,
#define __O_OO___O__ 0x2,0xc4,
#define __O_OO___O_O 0x2,0xc5,
#define __O_OO___OO_ 0x2,0xc6,
#define __O_OO___OOO 0x2,0xc7,
#define __O_OO__O___ 0x2,0xc8,
#define __O_OO__O__O 0x2,0xc9,
#define __O_OO__O_O_ 0x2,0xca,
#define __O_OO__O_OO 0x2,0xcb,
#define __O_OO__OO__ 0x2,0xcc,
#define __O_OO__OO_O 0x2,0xcd,
#define __O_OO__OOO_ 0x2,0xce,
#define __O_OO__OOOO 0x2,0xcf,
#define __O_OO_O____ 0x2,0xd0,
#define __O_OO_O___O 0x2,0xd1,
#define __O_OO_O__O_ 0x2,0xd2,
#define __O_OO_O__OO 0x2,0xd3,
#define __O_OO_O_O__ 0x2,0xd4,
#define __O_OO_O_O_O 0x2,0xd5,
#define __O_OO_O_OO_ 0x2,0xd6,
#define __O_OO_O_OOO 0x2,0xd7,
#define __O_OO_OO___ 0x2,0xd8,
#define __O_OO_OO__O 0x2,0xd9,
#define __O_OO_OO_O_ 0x2,0xda,
#define __O_OO_OO_OO 0x2,0xdb,
#define __O_OO_OOO__ 0x2,0xdc,
#define __O_OO_OOO_O 0x2,0xdd,
#define __O_OO_OOOO_ 0x2,0xde,
#define __O_OO_OOOOO 0x2,0xdf,
#define __O_OOO_____ 0x2,0xe0,
#define __O_OOO____O 0x2,0xe1,
#define __O_OOO___O_ 0x2,0xe2,
#define __O_OOO___OO 0x2,0xe3,
#define __O_OOO__O__ 0x2,0xe4,
#define __O_OOO__O_O 0x2,0xe5,
#define __O_OOO__OO_ 0x2,0xe6,
#define __O_OOO__OOO 0x2,0xe7,
#define __O_OOO_O___ 0x2,0xe8,
#define __O_OOO_O__O 0x2,0xe9,
#define __O_OOO_O_O_ 0x2,0xea,
#define __O_OOO_O_OO 0x2,0xeb,
#define __O_OOO_OO__ 0x2,0xec,
#define __O_OOO_OO_O 0x2,0xed,
#define __O_OOO_OOO_ 0x2,0xee,
#define __O_OOO_OOOO 0x2,0xef,
#define __O_OOOO____ 0x2,0xf0,
#define __O_OOOO___O 0x2,0xf1,
#define __O_OOOO__O_ 0x2,0xf2,
#define __O_OOOO__OO 0x2,0xf3,
#define __O_OOOO_O__ 0x2,0xf4,
#define __O_OOOO_O_O 0x2,0xf5,
#define __O_OOOO_OO_ 0x2,0xf6,
#define __O_OOOO_OOO 0x2,0xf7,
#define __O_OOOOO___ 0x2,0xf8,
#define __O_OOOOO__O 0x2,0xf9,
#define __O_OOOOO_O_ 0x2,0xfa,
#define __O_OOOOO_OO 0x2,0xfb,
#define __O_OOOOOO__ 0x2,0xfc,
#define __O_OOOOOO_O 0x2,0xfd,
#define __O_OOOOOOO_ 0x2,0xfe,
#define __O_OOOOOOOO 0x2,0xff,
#define __OO________ 0x3,0,
#define __OO_______O 0x3,0x1,
#define __OO______O_ 0x3,0x2,
#define __OO______OO 0x3,0x3,
#define __OO_____O__ 0x3,0x4,
#define __OO_____O_O 0x3,0x5,
#define __OO_____OO_ 0x3,0x6,
#define __OO_____OOO 0x3,0x7,
#define __OO____O___ 0x3,0x8,
#define __OO____O__O 0x3,0x9,
#define __OO____O_O_ 0x3,0xa,
#define __OO____O_OO 0x3,0xb,
#define __OO____OO__ 0x3,0xc,
#define __OO____OO_O 0x3,0xd,
#define __OO____OOO_ 0x3,0xe,
#define __OO____OOOO 0x3,0xf,
#define __OO___O____ 0x3,0x10,
#define __OO___O___O 0x3,0x11,
#define __OO___O__O_ 0x3,0x12,
#define __OO___O__OO 0x3,0x13,
#define __OO___O_O__ 0x3,0x14,
#define __OO___O_O_O 0x3,0x15,
#define __OO___O_OO_ 0x3,0x16,
#define __OO___O_OOO 0x3,0x17,
#define __OO___OO___ 0x3,0x18,
#define __OO___OO__O 0x3,0x19,
#define __OO___OO_O_ 0x3,0x1a,
#define __OO___OO_OO 0x3,0x1b,
#define __OO___OOO__ 0x3,0x1c,
#define __OO___OOO_O 0x3,0x1d,
#define __OO___OOOO_ 0x3,0x1e,
#define __OO___OOOOO 0x3,0x1f,
#define __OO__O_____ 0x3,0x20,
#define __OO__O____O 0x3,0x21,
#define __OO__O___O_ 0x3,0x22,
#define __OO__O___OO 0x3,0x23,
#define __OO__O__O__ 0x3,0x24,
#define __OO__O__O_O 0x3,0x25,
#define __OO__O__OO_ 0x3,0x26,
#define __OO__O__OOO 0x3,0x27,
#define __OO__O_O___ 0x3,0x28,
#define __OO__O_O__O 0x3,0x29,
#define __OO__O_O_O_ 0x3,0x2a,
#define __OO__O_O_OO 0x3,0x2b,
#define __OO__O_OO__ 0x3,0x2c,
#define __OO__O_OO_O 0x3,0x2d,
#define __OO__O_OOO_ 0x3,0x2e,
#define __OO__O_OOOO 0x3,0x2f,
#define __OO__OO____ 0x3,0x30,
#define __OO__OO___O 0x3,0x31,
#define __OO__OO__O_ 0x3,0x32,
#define __OO__OO__OO 0x3,0x33,
#define __OO__OO_O__ 0x3,0x34,
#define __OO__OO_O_O 0x3,0x35,
#define __OO__OO_OO_ 0x3,0x36,
#define __OO__OO_OOO 0x3,0x37,
#define __OO__OOO___ 0x3,0x38,
#define __OO__OOO__O 0x3,0x39,
#define __OO__OOO_O_ 0x3,0x3a,
#define __OO__OOO_OO 0x3,0x3b,
#define __OO__OOOO__ 0x3,0x3c,
#define __OO__OOOO_O 0x3,0x3d,
#define __OO__OOOOO_ 0x3,0x3e,
#define __OO__OOOOOO 0x3,0x3f,
#define __OO_O______ 0x3,0x40,
#define __OO_O_____O 0x3,0x41,
#define __OO_O____O_ 0x3,0x42,
#define __OO_O____OO 0x3,0x43,
#define __OO_O___O__ 0x3,0x44,
#define __OO_O___O_O 0x3,0x45,
#define __OO_O___OO_ 0x3,0x46,
#define __OO_O___OOO 0x3,0x47,
#define __OO_O__O___ 0x3,0x48,
#define __OO_O__O__O 0x3,0x49,
#define __OO_O__O_O_ 0x3,0x4a,
#define __OO_O__O_OO 0x3,0x4b,
#define __OO_O__OO__ 0x3,0x4c,
#define __OO_O__OO_O 0x3,0x4d,
#define __OO_O__OOO_ 0x3,0x4e,
#define __OO_O__OOOO 0x3,0x4f,
#define __OO_O_O____ 0x3,0x50,
#define __OO_O_O___O 0x3,0x51,
#define __OO_O_O__O_ 0x3,0x52,
#define __OO_O_O__OO 0x3,0x53,
#define __OO_O_O_O__ 0x3,0x54,
#define __OO_O_O_O_O 0x3,0x55,
#define __OO_O_O_OO_ 0x3,0x56,
#define __OO_O_O_OOO 0x3,0x57,
#define __OO_O_OO___ 0x3,0x58,
#define __OO_O_OO__O 0x3,0x59,
#define __OO_O_OO_O_ 0x3,0x5a,
#define __OO_O_OO_OO 0x3,0x5b,
#define __OO_O_OOO__ 0x3,0x5c,
#define __OO_O_OOO_O 0x3,0x5d,
#define __OO_O_OOOO_ 0x3,0x5e,
#define __OO_O_OOOOO 0x3,0x5f,
#define __OO_OO_____ 0x3,0x60,
#define __OO_OO____O 0x3,0x61,
#define __OO_OO___O_ 0x3,0x62,
#define __OO_OO___OO 0x3,0x63,
#define __OO_OO__O__ 0x3,0x64,
#define __OO_OO__O_O 0x3,0x65,
#define __OO_OO__OO_ 0x3,0x66,
#define __OO_OO__OOO 0x3,0x67,
#define __OO_OO_O___ 0x3,0x68,
#define __OO_OO_O__O 0x3,0x69,
#define __OO_OO_O_O_ 0x3,0x6a,
#define __OO_OO_O_OO 0x3,0x6b,
#define __OO_OO_OO__ 0x3,0x6c,
#define __OO_OO_OO_O 0x3,0x6d,
#define __OO_OO_OOO_ 0x3,0x6e,
#define __OO_OO_OOOO 0x3,0x6f,
#define __OO_OOO____ 0x3,0x70,
#define __OO_OOO___O 0x3,0x71,
#define __OO_OOO__O_ 0x3,0x72,
#define __OO_OOO__OO 0x3,0x73,
#define __OO_OOO_O__ 0x3,0x74,
#define __OO_OOO_O_O 0x3,0x75,
#define __OO_OOO_OO_ 0x3,0x76,
#define __OO_OOO_OOO 0x3,0x77,
#define __OO_OOOO___ 0x3,0x78,
#define __OO_OOOO__O 0x3,0x79,
#define __OO_OOOO_O_ 0x3,0x7a,
#define __OO_OOOO_OO 0x3,0x7b,
#define __OO_OOOOO__ 0x3,0x7c,
#define __OO_OOOOO_O 0x3,0x7d,
#define __OO_OOOOOO_ 0x3,0x7e,
#define __OO_OOOOOOO 0x3,0x7f,
#define __OOO_______ 0x3,0x80,
#define __OOO______O 0x3,0x81,
#define __OOO_____O_ 0x3,0x82,
#define __OOO_____OO 0x3,0x83,
#define __OOO____O__ 0x3,0x84,
#define __OOO____O_O 0x3,0x85,
#define __OOO____OO_ 0x3,0x86,
#define __OOO____OOO 0x3,0x87,
#define __OOO___O___ 0x3,0x88,
#define __OOO___O__O 0x3,0x89,
#define __OOO___O_O_ 0x3,0x8a,
#define __OOO___O_OO 0x3,0x8b,
#define __OOO___OO__ 0x3,0x8c,
#define __OOO___OO_O 0x3,0x8d,
#define __OOO___OOO_ 0x3,0x8e,
#define __OOO___OOOO 0x3,0x8f,
#define __OOO__O____ 0x3,0x90,
#define __OOO__O___O 0x3,0x91,
#define __OOO__O__O_ 0x3,0x92,
#define __OOO__O__OO 0x3,0x93,
#define __OOO__O_O__ 0x3,0x94,
#define __OOO__O_O_O 0x3,0x95,
#define __OOO__O_OO_ 0x3,0x96,
#define __OOO__O_OOO 0x3,0x97,
#define __OOO__OO___ 0x3,0x98,
#define __OOO__OO__O 0x3,0x99,
#define __OOO__OO_O_ 0x3,0x9a,
#define __OOO__OO_OO 0x3,0x9b,
#define __OOO__OOO__ 0x3,0x9c,
#define __OOO__OOO_O 0x3,0x9d,
#define __OOO__OOOO_ 0x3,0x9e,
#define __OOO__OOOOO 0x3,0x9f,
#define __OOO_O_____ 0x3,0xa0,
#define __OOO_O____O 0x3,0xa1,
#define __OOO_O___O_ 0x3,0xa2,
#define __OOO_O___OO 0x3,0xa3,
#define __OOO_O__O__ 0x3,0xa4,
#define __OOO_O__O_O 0x3,0xa5,
#define __OOO_O__OO_ 0x3,0xa6,
#define __OOO_O__OOO 0x3,0xa7,
#define __OOO_O_O___ 0x3,0xa8,
#define __OOO_O_O__O 0x3,0xa9,
#define __OOO_O_O_O_ 0x3,0xaa,
#define __OOO_O_O_OO 0x3,0xab,
#define __OOO_O_OO__ 0x3,0xac,
#define __OOO_O_OO_O 0x3,0xad,
#define __OOO_O_OOO_ 0x3,0xae,
#define __OOO_O_OOOO 0x3,0xaf,
#define __OOO_OO____ 0x3,0xb0,
#define __OOO_OO___O 0x3,0xb1,
#define __OOO_OO__O_ 0x3,0xb2,
#define __OOO_OO__OO 0x3,0xb3,
#define __OOO_OO_O__ 0x3,0xb4,
#define __OOO_OO_O_O 0x3,0xb5,
#define __OOO_OO_OO_ 0x3,0xb6,
#define __OOO_OO_OOO 0x3,0xb7,
#define __OOO_OOO___ 0x3,0xb8,
#define __OOO_OOO__O 0x3,0xb9,
#define __OOO_OOO_O_ 0x3,0xba,
#define __OOO_OOO_OO 0x3,0xbb,
#define __OOO_OOOO__ 0x3,0xbc,
#define __OOO_OOOO_O 0x3,0xbd,
#define __OOO_OOOOO_ 0x3,0xbe,
#define __OOO_OOOOOO 0x3,0xbf,
#define __OOOO______ 0x3,0xc0,
#define __OOOO_____O 0x3,0xc1,
#define __OOOO____O_ 0x3,0xc2,
#define __OOOO____OO 0x3,0xc3,
#define __OOOO___O__ 0x3,0xc4,
#define __OOOO___O_O 0x3,0xc5,
#define __OOOO___OO_ 0x3,0xc6,
#define __OOOO___OOO 0x3,0xc7,
#define __OOOO__O___ 0x3,0xc8,
#define __OOOO__O__O 0x3,0xc9,
#define __OOOO__O_O_ 0x3,0xca,
#define __OOOO__O_OO 0x3,0xcb,
#define __OOOO__OO__ 0x3,0xcc,
#define __OOOO__OO_O 0x3,0xcd,
#define __OOOO__OOO_ 0x3,0xce,
#define __OOOO__OOOO 0x3,0xcf,
#define __OOOO_O____ 0x3,0xd0,
#define __OOOO_O___O 0x3,0xd1,
#define __OOOO_O__O_ 0x3,0xd2,
#define __OOOO_O__OO 0x3,0xd3,
#define __OOOO_O_O__ 0x3,0xd4,
#define __OOOO_O_O_O 0x3,0xd5,
#define __OOOO_O_OO_ 0x3,0xd6,
#define __OOOO_O_OOO 0x3,0xd7,
#define __OOOO_OO___ 0x3,0xd8,
#define __OOOO_OO__O 0x3,0xd9,
#define __OOOO_OO_O_ 0x3,0xda,
#define __OOOO_OO_OO 0x3,0xdb,
#define __OOOO_OOO__ 0x3,0xdc,
#define __OOOO_OOO_O 0x3,0xdd,
#define __OOOO_OOOO_ 0x3,0xde,
#define __OOOO_OOOOO 0x3,0xdf,
#define __OOOOO_____ 0x3,0xe0,
#define __OOOOO____O 0x3,0xe1,
#define __OOOOO___O_ 0x3,0xe2,
#define __OOOOO___OO 0x3,0xe3,
#define __OOOOO__O__ 0x3,0xe4,
#define __OOOOO__O_O 0x3,0xe5,
#define __OOOOO__OO_ 0x3,0xe6,
#define __OOOOO__OOO 0x3,0xe7,
#define __OOOOO_O___ 0x3,0xe8,
#define __OOOOO_O__O 0x3,0xe9,
#define __OOOOO_O_O_ 0x3,0xea,
#define __OOOOO_O_OO 0x3,0xeb,
#define __OOOOO_OO__ 0x3,0xec,
#define __OOOOO_OO_O 0x3,0xed,
#define __OOOOO_OOO_ 0x3,0xee,
#define __OOOOO_OOOO 0x3,0xef,
#define __OOOOOO____ 0x3,0xf0,
#define __OOOOOO___O 0x3,0xf1,
#define __OOOOOO__O_ 0x3,0xf2,
#define __OOOOOO__OO 0x3,0xf3,
#define __OOOOOO_O__ 0x3,0xf4,
#define __OOOOOO_O_O 0x3,0xf5,
#define __OOOOOO_OO_ 0x3,0xf6,
#define __OOOOOO_OOO 0x3,0xf7,
#define __OOOOOOO___ 0x3,0xf8,
#define __OOOOOOO__O 0x3,0xf9,
#define __OOOOOOO_O_ 0x3,0xfa,
#define __OOOOOOO_OO 0x3,0xfb,
#define __OOOOOOOO__ 0x3,0xfc,
#define __OOOOOOOO_O 0x3,0xfd,
#define __OOOOOOOOO_ 0x3,0xfe,
#define __OOOOOOOOOO 0x3,0xff,
#define _O__________ 0x4,0,
#define _O_________O 0x4,0x1,
#define _O________O_ 0x4,0x2,
#define _O________OO 0x4,0x3,
#define _O_______O__ 0x4,0x4,
#define _O_______O_O 0x4,0x5,
#define _O_______OO_ 0x4,0x6,
#define _O_______OOO 0x4,0x7,
#define _O______O___ 0x4,0x8,
#define _O______O__O 0x4,0x9,
#define _O______O_O_ 0x4,0xa,
#define _O______O_OO 0x4,0xb,
#define _O______OO__ 0x4,0xc,
#define _O______OO_O 0x4,0xd,
#define _O______OOO_ 0x4,0xe,
#define _O______OOOO 0x4,0xf,
#define _O_____O____ 0x4,0x10,
#define _O_____O___O 0x4,0x11,
#define _O_____O__O_ 0x4,0x12,
#define _O_____O__OO 0x4,0x13,
#define _O_____O_O__ 0x4,0x14,
#define _O_____O_O_O 0x4,0x15,
#define _O_____O_OO_ 0x4,0x16,
#define _O_____O_OOO 0x4,0x17,
#define _O_____OO___ 0x4,0x18,
#define _O_____OO__O 0x4,0x19,
#define _O_____OO_O_ 0x4,0x1a,
#define _O_____OO_OO 0x4,0x1b,
#define _O_____OOO__ 0x4,0x1c,
#define _O_____OOO_O 0x4,0x1d,
#define _O_____OOOO_ 0x4,0x1e,
#define _O_____OOOOO 0x4,0x1f,
#define _O____O_____ 0x4,0x20,
#define _O____O____O 0x4,0x21,
#define _O____O___O_ 0x4,0x22,
#define _O____O___OO 0x4,0x23,
#define _O____O__O__ 0x4,0x24,
#define _O____O__O_O 0x4,0x25,
#define _O____O__OO_ 0x4,0x26,
#define _O____O__OOO 0x4,0x27,
#define _O____O_O___ 0x4,0x28,
#define _O____O_O__O 0x4,0x29,
#define _O____O_O_O_ 0x4,0x2a,
#define _O____O_O_OO 0x4,0x2b,
#define _O____O_OO__ 0x4,0x2c,
#define _O____O_OO_O 0x4,0x2d,
#define _O____O_OOO_ 0x4,0x2e,
#define _O____O_OOOO 0x4,0x2f,
#define _O____OO____ 0x4,0x30,
#define _O____OO___O 0x4,0x31,
#define _O____OO__O_ 0x4,0x32,
#define _O____OO__OO 0x4,0x33,
#define _O____OO_O__ 0x4,0x34,
#define _O____OO_O_O 0x4,0x35,
#define _O____OO_OO_ 0x4,0x36,
#define _O____OO_OOO 0x4,0x37,
#define _O____OOO___ 0x4,0x38,
#define _O____OOO__O 0x4,0x39,
#define _O____OOO_O_ 0x4,0x3a,
#define _O____OOO_OO 0x4,0x3b,
#define _O____OOOO__ 0x4,0x3c,
#define _O____OOOO_O 0x4,0x3d,
#define _O____OOOOO_ 0x4,0x3e,
#define _O____OOOOOO 0x4,0x3f,
#define _O___O______ 0x4,0x40,
#define _O___O_____O 0x4,0x41,
#define _O___O____O_ 0x4,0x42,
#define _O___O____OO 0x4,0x43,
#define _O___O___O__ 0x4,0x44,
#define _O___O___O_O 0x4,0x45,
#define _O___O___OO_ 0x4,0x46,
#define _O___O___OOO 0x4,0x47,
#define _O___O__O___ 0x4,0x48,
#define _O___O__O__O 0x4,0x49,
#define _O___O__O_O_ 0x4,0x4a,
#define _O___O__O_OO 0x4,0x4b,
#define _O___O__OO__ 0x4,0x4c,
#define _O___O__OO_O 0x4,0x4d,
#define _O___O__OOO_ 0x4,0x4e,
#define _O___O__OOOO 0x4,0x4f,
#define _O___O_O____ 0x4,0x50,
#define _O___O_O___O 0x4,0x51,
#define _O___O_O__O_ 0x4,0x52,
#define _O___O_O__OO 0x4,0x53,
#define _O___O_O_O__ 0x4,0x54,
#define _O___O_O_O_O 0x4,0x55,
#define _O___O_O_OO_ 0x4,0x56,
#define _O___O_O_OOO 0x4,0x57,
#define _O___O_OO___ 0x4,0x58,
#define _O___O_OO__O 0x4,0x59,
#define _O___O_OO_O_ 0x4,0x5a,
#define _O___O_OO_OO 0x4,0x5b,
#define _O___O_OOO__ 0x4,0x5c,
#define _O___O_OOO_O 0x4,0x5d,
#define _O___O_OOOO_ 0x4,0x5e,
#define _O___O_OOOOO 0x4,0x5f,
#define _O___OO_____ 0x4,0x60,
#define _O___OO____O 0x4,0x61,
#define _O___OO___O_ 0x4,0x62,
#define _O___OO___OO 0x4,0x63,
#define _O___OO__O__ 0x4,0x64,
#define _O___OO__O_O 0x4,0x65,
#define _O___OO__OO_ 0x4,0x66,
#define _O___OO__OOO 0x4,0x67,
#define _O___OO_O___ 0x4,0x68,
#define _O___OO_O__O 0x4,0x69,
#define _O___OO_O_O_ 0x4,0x6a,
#define _O___OO_O_OO 0x4,0x6b,
#define _O___OO_OO__ 0x4,0x6c,
#define _O___OO_OO_O 0x4,0x6d,
#define _O___OO_OOO_ 0x4,0x6e,
#define _O___OO_OOOO 0x4,0x6f,
#define _O___OOO____ 0x4,0x70,
#define _O___OOO___O 0x4,0x71,
#define _O___OOO__O_ 0x4,0x72,
#define _O___OOO__OO 0x4,0x73,
#define _O___OOO_O__ 0x4,0x74,
#define _O___OOO_O_O 0x4,0x75,
#define _O___OOO_OO_ 0x4,0x76,
#define _O___OOO_OOO 0x4,0x77,
#define _O___OOOO___ 0x4,0x78,
#define _O___OOOO__O 0x4,0x79,
#define _O___OOOO_O_ 0x4,0x7a,
#define _O___OOOO_OO 0x4,0x7b,
#define _O___OOOOO__ 0x4,0x7c,
#define _O___OOOOO_O 0x4,0x7d,
#define _O___OOOOOO_ 0x4,0x7e,
#define _O___OOOOOOO 0x4,0x7f,
#define _O__O_______ 0x4,0x80,
#define _O__O______O 0x4,0x81,
#define _O__O_____O_ 0x4,0x82,
#define _O__O_____OO 0x4,0x83,
#define _O__O____O__ 0x4,0x84,
#define _O__O____O_O 0x4,0x85,
#define _O__O____OO_ 0x4,0x86,
#define _O__O____OOO 0x4,0x87,
#define _O__O___O___ 0x4,0x88,
#define _O__O___O__O 0x4,0x89,
#define _O__O___O_O_ 0x4,0x8a,
#define _O__O___O_OO 0x4,0x8b,
#define _O__O___OO__ 0x4,0x8c,
#define _O__O___OO_O 0x4,0x8d,
#define _O__O___OOO_ 0x4,0x8e,
#define _O__O___OOOO 0x4,0x8f,
#define _O__O__O____ 0x4,0x90,
#define _O__O__O___O 0x4,0x91,
#define _O__O__O__O_ 0x4,0x92,
#define _O__O__O__OO 0x4,0x93,
#define _O__O__O_O__ 0x4,0x94,
#define _O__O__O_O_O 0x4,0x95,
#define _O__O__O_OO_ 0x4,0x96,
#define _O__O__O_OOO 0x4,0x97,
#define _O__O__OO___ 0x4,0x98,
#define _O__O__OO__O 0x4,0x99,
#define _O__O__OO_O_ 0x4,0x9a,
#define _O__O__OO_OO 0x4,0x9b,
#define _O__O__OOO__ 0x4,0x9c,
#define _O__O__OOO_O 0x4,0x9d,
#define _O__O__OOOO_ 0x4,0x9e,
#define _O__O__OOOOO 0x4,0x9f,
#define _O__O_O_____ 0x4,0xa0,
#define _O__O_O____O 0x4,0xa1,
#define _O__O_O___O_ 0x4,0xa2,
#define _O__O_O___OO 0x4,0xa3,
#define _O__O_O__O__ 0x4,0xa4,
#define _O__O_O__O_O 0x4,0xa5,
#define _O__O_O__OO_ 0x4,0xa6,
#define _O__O_O__OOO 0x4,0xa7,
#define _O__O_O_O___ 0x4,0xa8,
#define _O__O_O_O__O 0x4,0xa9,
#define _O__O_O_O_O_ 0x4,0xaa,
#define _O__O_O_O_OO 0x4,0xab,
#define _O__O_O_OO__ 0x4,0xac,
#define _O__O_O_OO_O 0x4,0xad,
#define _O__O_O_OOO_ 0x4,0xae,
#define _O__O_O_OOOO 0x4,0xaf,
#define _O__O_OO____ 0x4,0xb0,
#define _O__O_OO___O 0x4,0xb1,
#define _O__O_OO__O_ 0x4,0xb2,
#define _O__O_OO__OO 0x4,0xb3,
#define _O__O_OO_O__ 0x4,0xb4,
#define _O__O_OO_O_O 0x4,0xb5,
#define _O__O_OO_OO_ 0x4,0xb6,
#define _O__O_OO_OOO 0x4,0xb7,
#define _O__O_OOO___ 0x4,0xb8,
#define _O__O_OOO__O 0x4,0xb9,
#define _O__O_OOO_O_ 0x4,0xba,
#define _O__O_OOO_OO 0x4,0xbb,
#define _O__O_OOOO__ 0x4,0xbc,
#define _O__O_OOOO_O 0x4,0xbd,
#define _O__O_OOOOO_ 0x4,0xbe,
#define _O__O_OOOOOO 0x4,0xbf,
#define _O__OO______ 0x4,0xc0,
#define _O__OO_____O 0x4,0xc1,
#define _O__OO____O_ 0x4,0xc2,
#define _O__OO____OO 0x4,0xc3,
#define _O__OO___O__ 0x4,0xc4,
#define _O__OO___O_O 0x4,0xc5,
#define _O__OO___OO_ 0x4,0xc6,
#define _O__OO___OOO 0x4,0xc7,
#define _O__OO__O___ 0x4,0xc8,
#define _O__OO__O__O 0x4,0xc9,
#define _O__OO__O_O_ 0x4,0xca,
#define _O__OO__O_OO 0x4,0xcb,
#define _O__OO__OO__ 0x4,0xcc,
#define _O__OO__OO_O 0x4,0xcd,
#define _O__OO__OOO_ 0x4,0xce,
#define _O__OO__OOOO 0x4,0xcf,
#define _O__OO_O____ 0x4,0xd0,
#define _O__OO_O___O 0x4,0xd1,
#define _O__OO_O__O_ 0x4,0xd2,
#define _O__OO_O__OO 0x4,0xd3,
#define _O__OO_O_O__ 0x4,0xd4,
#define _O__OO_O_O_O 0x4,0xd5,
#define _O__OO_O_OO_ 0x4,0xd6,
#define _O__OO_O_OOO 0x4,0xd7,
#define _O__OO_OO___ 0x4,0xd8,
#define _O__OO_OO__O 0x4,0xd9,
#define _O__OO_OO_O_ 0x4,0xda,
#define _O__OO_OO_OO 0x4,0xdb,
#define _O__OO_OOO__ 0x4,0xdc,
#define _O__OO_OOO_O 0x4,0xdd,
#define _O__OO_OOOO_ 0x4,0xde,
#define _O__OO_OOOOO 0x4,0xdf,
#define _O__OOO_____ 0x4,0xe0,
#define _O__OOO____O 0x4,0xe1,
#define _O__OOO___O_ 0x4,0xe2,
#define _O__OOO___OO 0x4,0xe3,
#define _O__OOO__O__ 0x4,0xe4,
#define _O__OOO__O_O 0x4,0xe5,
#define _O__OOO__OO_ 0x4,0xe6,
#define _O__OOO__OOO 0x4,0xe7,
#define _O__OOO_O___ 0x4,0xe8,
#define _O__OOO_O__O 0x4,0xe9,
#define _O__OOO_O_O_ 0x4,0xea,
#define _O__OOO_O_OO 0x4,0xeb,
#define _O__OOO_OO__ 0x4,0xec,
#define _O__OOO_OO_O 0x4,0xed,
#define _O__OOO_OOO_ 0x4,0xee,
#define _O__OOO_OOOO 0x4,0xef,
#define _O__OOOO____ 0x4,0xf0,
#define _O__OOOO___O 0x4,0xf1,
#define _O__OOOO__O_ 0x4,0xf2,
#define _O__OOOO__OO 0x4,0xf3,
#define _O__OOOO_O__ 0x4,0xf4,
#define _O__OOOO_O_O 0x4,0xf5,
#define _O__OOOO_OO_ 0x4,0xf6,
#define _O__OOOO_OOO 0x4,0xf7,
#define _O__OOOOO___ 0x4,0xf8,
#define _O__OOOOO__O 0x4,0xf9,
#define _O__OOOOO_O_ 0x4,0xfa,
#define _O__OOOOO_OO 0x4,0xfb,
#define _O__OOOOOO__ 0x4,0xfc,
#define _O__OOOOOO_O 0x4,0xfd,
#define _O__OOOOOOO_ 0x4,0xfe,
#define _O__OOOOOOOO 0x4,0xff,
#define _O_O________ 0x5,0,
#define _O_O_______O 0x5,0x1,
#define _O_O______O_ 0x5,0x2,
#define _O_O______OO 0x5,0x3,
#define _O_O_____O__ 0x5,0x4,
#define _O_O_____O_O 0x5,0x5,
#define _O_O_____OO_ 0x5,0x6,
#define _O_O_____OOO 0x5,0x7,
#define _O_O____O___ 0x5,0x8,
#define _O_O____O__O 0x5,0x9,
#define _O_O____O_O_ 0x5,0xa,
#define _O_O____O_OO 0x5,0xb,
#define _O_O____OO__ 0x5,0xc,
#define _O_O____OO_O 0x5,0xd,
#define _O_O____OOO_ 0x5,0xe,
#define _O_O____OOOO 0x5,0xf,
#define _O_O___O____ 0x5,0x10,
#define _O_O___O___O 0x5,0x11,
#define _O_O___O__O_ 0x5,0x12,
#define _O_O___O__OO 0x5,0x13,
#define _O_O___O_O__ 0x5,0x14,
#define _O_O___O_O_O 0x5,0x15,
#define _O_O___O_OO_ 0x5,0x16,
#define _O_O___O_OOO 0x5,0x17,
#define _O_O___OO___ 0x5,0x18,
#define _O_O___OO__O 0x5,0x19,
#define _O_O___OO_O_ 0x5,0x1a,
#define _O_O___OO_OO 0x5,0x1b,
#define _O_O___OOO__ 0x5,0x1c,
#define _O_O___OOO_O 0x5,0x1d,
#define _O_O___OOOO_ 0x5,0x1e,
#define _O_O___OOOOO 0x5,0x1f,
#define _O_O__O_____ 0x5,0x20,
#define _O_O__O____O 0x5,0x21,
#define _O_O__O___O_ 0x5,0x22,
#define _O_O__O___OO 0x5,0x23,
#define _O_O__O__O__ 0x5,0x24,
#define _O_O__O__O_O 0x5,0x25,
#define _O_O__O__OO_ 0x5,0x26,
#define _O_O__O__OOO 0x5,0x27,
#define _O_O__O_O___ 0x5,0x28,
#define _O_O__O_O__O 0x5,0x29,
#define _O_O__O_O_O_ 0x5,0x2a,
#define _O_O__O_O_OO 0x5,0x2b,
#define _O_O__O_OO__ 0x5,0x2c,
#define _O_O__O_OO_O 0x5,0x2d,
#define _O_O__O_OOO_ 0x5,0x2e,
#define _O_O__O_OOOO 0x5,0x2f,
#define _O_O__OO____ 0x5,0x30,
#define _O_O__OO___O 0x5,0x31,
#define _O_O__OO__O_ 0x5,0x32,
#define _O_O__OO__OO 0x5,0x33,
#define _O_O__OO_O__ 0x5,0x34,
#define _O_O__OO_O_O 0x5,0x35,
#define _O_O__OO_OO_ 0x5,0x36,
#define _O_O__OO_OOO 0x5,0x37,
#define _O_O__OOO___ 0x5,0x38,
#define _O_O__OOO__O 0x5,0x39,
#define _O_O__OOO_O_ 0x5,0x3a,
#define _O_O__OOO_OO 0x5,0x3b,
#define _O_O__OOOO__ 0x5,0x3c,
#define _O_O__OOOO_O 0x5,0x3d,
#define _O_O__OOOOO_ 0x5,0x3e,
#define _O_O__OOOOOO 0x5,0x3f,
#define _O_O_O______ 0x5,0x40,
#define _O_O_O_____O 0x5,0x41,
#define _O_O_O____O_ 0x5,0x42,
#define _O_O_O____OO 0x5,0x43,
#define _O_O_O___O__ 0x5,0x44,
#define _O_O_O___O_O 0x5,0x45,
#define _O_O_O___OO_ 0x5,0x46,
#define _O_O_O___OOO 0x5,0x47,
#define _O_O_O__O___ 0x5,0x48,
#define _O_O_O__O__O 0x5,0x49,
#define _O_O_O__O_O_ 0x5,0x4a,
#define _O_O_O__O_OO 0x5,0x4b,
#define _O_O_O__OO__ 0x5,0x4c,
#define _O_O_O__OO_O 0x5,0x4d,
#define _O_O_O__OOO_ 0x5,0x4e,
#define _O_O_O__OOOO 0x5,0x4f,
#define _O_O_O_O____ 0x5,0x50,
#define _O_O_O_O___O 0x5,0x51,
#define _O_O_O_O__O_ 0x5,0x52,
#define _O_O_O_O__OO 0x5,0x53,
#define _O_O_O_O_O__ 0x5,0x54,
#define _O_O_O_O_O_O 0x5,0x55,
#define _O_O_O_O_OO_ 0x5,0x56,
#define _O_O_O_O_OOO 0x5,0x57,
#define _O_O_O_OO___ 0x5,0x58,
#define _O_O_O_OO__O 0x5,0x59,
#define _O_O_O_OO_O_ 0x5,0x5a,
#define _O_O_O_OO_OO 0x5,0x5b,
#define _O_O_O_OOO__ 0x5,0x5c,
#define _O_O_O_OOO_O 0x5,0x5d,
#define _O_O_O_OOOO_ 0x5,0x5e,
#define _O_O_O_OOOOO 0x5,0x5f,
#define _O_O_OO_____ 0x5,0x60,
#define _O_O_OO____O 0x5,0x61,
#define _O_O_OO___O_ 0x5,0x62,
#define _O_O_OO___OO 0x5,0x63,
#define _O_O_OO__O__ 0x5,0x64,
#define _O_O_OO__O_O 0x5,0x65,
#define _O_O_OO__OO_ 0x5,0x66,
#define _O_O_OO__OOO 0x5,0x67,
#define _O_O_OO_O___ 0x5,0x68,
#define _O_O_OO_O__O 0x5,0x69,
#define _O_O_OO_O_O_ 0x5,0x6a,
#define _O_O_OO_O_OO 0x5,0x6b,
#define _O_O_OO_OO__ 0x5,0x6c,
#define _O_O_OO_OO_O 0x5,0x6d,
#define _O_O_OO_OOO_ 0x5,0x6e,
#define _O_O_OO_OOOO 0x5,0x6f,
#define _O_O_OOO____ 0x5,0x70,
#define _O_O_OOO___O 0x5,0x71,
#define _O_O_OOO__O_ 0x5,0x72,
#define _O_O_OOO__OO 0x5,0x73,
#define _O_O_OOO_O__ 0x5,0x74,
#define _O_O_OOO_O_O 0x5,0x75,
#define _O_O_OOO_OO_ 0x5,0x76,
#define _O_O_OOO_OOO 0x5,0x77,
#define _O_O_OOOO___ 0x5,0x78,
#define _O_O_OOOO__O 0x5,0x79,
#define _O_O_OOOO_O_ 0x5,0x7a,
#define _O_O_OOOO_OO 0x5,0x7b,
#define _O_O_OOOOO__ 0x5,0x7c,
#define _O_O_OOOOO_O 0x5,0x7d,
#define _O_O_OOOOOO_ 0x5,0x7e,
#define _O_O_OOOOOOO 0x5,0x7f,
#define _O_OO_______ 0x5,0x80,
#define _O_OO______O 0x5,0x81,
#define _O_OO_____O_ 0x5,0x82,
#define _O_OO_____OO 0x5,0x83,
#define _O_OO____O__ 0x5,0x84,
#define _O_OO____O_O 0x5,0x85,
#define _O_OO____OO_ 0x5,0x86,
#define _O_OO____OOO 0x5,0x87,
#define _O_OO___O___ 0x5,0x88,
#define _O_OO___O__O 0x5,0x89,
#define _O_OO___O_O_ 0x5,0x8a,
#define _O_OO___O_OO 0x5,0x8b,
#define _O_OO___OO__ 0x5,0x8c,
#define _O_OO___OO_O 0x5,0x8d,
#define _O_OO___OOO_ 0x5,0x8e,
#define _O_OO___OOOO 0x5,0x8f,
#define _O_OO__O____ 0x5,0x90,
#define _O_OO__O___O 0x5,0x91,
#define _O_OO__O__O_ 0x5,0x92,
#define _O_OO__O__OO 0x5,0x93,
#define _O_OO__O_O__ 0x5,0x94,
#define _O_OO__O_O_O 0x5,0x95,
#define _O_OO__O_OO_ 0x5,0x96,
#define _O_OO__O_OOO 0x5,0x97,
#define _O_OO__OO___ 0x5,0x98,
#define _O_OO__OO__O 0x5,0x99,
#define _O_OO__OO_O_ 0x5,0x9a,
#define _O_OO__OO_OO 0x5,0x9b,
#define _O_OO__OOO__ 0x5,0x9c,
#define _O_OO__OOO_O 0x5,0x9d,
#define _O_OO__OOOO_ 0x5,0x9e,
#define _O_OO__OOOOO 0x5,0x9f,
#define _O_OO_O_____ 0x5,0xa0,
#define _O_OO_O____O 0x5,0xa1,
#define _O_OO_O___O_ 0x5,0xa2,
#define _O_OO_O___OO 0x5,0xa3,
#define _O_OO_O__O__ 0x5,0xa4,
#define _O_OO_O__O_O 0x5,0xa5,
#define _O_OO_O__OO_ 0x5,0xa6,
#define _O_OO_O__OOO 0x5,0xa7,
#define _O_OO_O_O___ 0x5,0xa8,
#define _O_OO_O_O__O 0x5,0xa9,
#define _O_OO_O_O_O_ 0x5,0xaa,
#define _O_OO_O_O_OO 0x5,0xab,
#define _O_OO_O_OO__ 0x5,0xac,
#define _O_OO_O_OO_O 0x5,0xad,
#define _O_OO_O_OOO_ 0x5,0xae,
#define _O_OO_O_OOOO 0x5,0xaf,
#define _O_OO_OO____ 0x5,0xb0,
#define _O_OO_OO___O 0x5,0xb1,
#define _O_OO_OO__O_ 0x5,0xb2,
#define _O_OO_OO__OO 0x5,0xb3,
#define _O_OO_OO_O__ 0x5,0xb4,
#define _O_OO_OO_O_O 0x5,0xb5,
#define _O_OO_OO_OO_ 0x5,0xb6,
#define _O_OO_OO_OOO 0x5,0xb7,
#define _O_OO_OOO___ 0x5,0xb8,
#define _O_OO_OOO__O 0x5,0xb9,
#define _O_OO_OOO_O_ 0x5,0xba,
#define _O_OO_OOO_OO 0x5,0xbb,
#define _O_OO_OOOO__ 0x5,0xbc,
#define _O_OO_OOOO_O 0x5,0xbd,
#define _O_OO_OOOOO_ 0x5,0xbe,
#define _O_OO_OOOOOO 0x5,0xbf,
#define _O_OOO______ 0x5,0xc0,
#define _O_OOO_____O 0x5,0xc1,
#define _O_OOO____O_ 0x5,0xc2,
#define _O_OOO____OO 0x5,0xc3,
#define _O_OOO___O__ 0x5,0xc4,
#define _O_OOO___O_O 0x5,0xc5,
#define _O_OOO___OO_ 0x5,0xc6,
#define _O_OOO___OOO 0x5,0xc7,
#define _O_OOO__O___ 0x5,0xc8,
#define _O_OOO__O__O 0x5,0xc9,
#define _O_OOO__O_O_ 0x5,0xca,
#define _O_OOO__O_OO 0x5,0xcb,
#define _O_OOO__OO__ 0x5,0xcc,
#define _O_OOO__OO_O 0x5,0xcd,
#define _O_OOO__OOO_ 0x5,0xce,
#define _O_OOO__OOOO 0x5,0xcf,
#define _O_OOO_O____ 0x5,0xd0,
#define _O_OOO_O___O 0x5,0xd1,
#define _O_OOO_O__O_ 0x5,0xd2,
#define _O_OOO_O__OO 0x5,0xd3,
#define _O_OOO_O_O__ 0x5,0xd4,
#define _O_OOO_O_O_O 0x5,0xd5,
#define _O_OOO_O_OO_ 0x5,0xd6,
#define _O_OOO_O_OOO 0x5,0xd7,
#define _O_OOO_OO___ 0x5,0xd8,
#define _O_OOO_OO__O 0x5,0xd9,
#define _O_OOO_OO_O_ 0x5,0xda,
#define _O_OOO_OO_OO 0x5,0xdb,
#define _O_OOO_OOO__ 0x5,0xdc,
#define _O_OOO_OOO_O 0x5,0xdd,
#define _O_OOO_OOOO_ 0x5,0xde,
#define _O_OOO_OOOOO 0x5,0xdf,
#define _O_OOOO_____ 0x5,0xe0,
#define _O_OOOO____O 0x5,0xe1,
#define _O_OOOO___O_ 0x5,0xe2,
#define _O_OOOO___OO 0x5,0xe3,
#define _O_OOOO__O__ 0x5,0xe4,
#define _O_OOOO__O_O 0x5,0xe5,
#define _O_OOOO__OO_ 0x5,0xe6,
#define _O_OOOO__OOO 0x5,0xe7,
#define _O_OOOO_O___ 0x5,0xe8,
#define _O_OOOO_O__O 0x5,0xe9,
#define _O_OOOO_O_O_ 0x5,0xea,
#define _O_OOOO_O_OO 0x5,0xeb,
#define _O_OOOO_OO__ 0x5,0xec,
#define _O_OOOO_OO_O 0x5,0xed,
#define _O_OOOO_OOO_ 0x5,0xee,
#define _O_OOOO_OOOO 0x5,0xef,
#define _O_OOOOO____ 0x5,0xf0,
#define _O_OOOOO___O 0x5,0xf1,
#define _O_OOOOO__O_ 0x5,0xf2,
#define _O_OOOOO__OO 0x5,0xf3,
#define _O_OOOOO_O__ 0x5,0xf4,
#define _O_OOOOO_O_O 0x5,0xf5,
#define _O_OOOOO_OO_ 0x5,0xf6,
#define _O_OOOOO_OOO 0x5,0xf7,
#define _O_OOOOOO___ 0x5,0xf8,
#define _O_OOOOOO__O 0x5,0xf9,
#define _O_OOOOOO_O_ 0x5,0xfa,
#define _O_OOOOOO_OO 0x5,0xfb,
#define _O_OOOOOOO__ 0x5,0xfc,
#define _O_OOOOOOO_O 0x5,0xfd,
#define _O_OOOOOOOO_ 0x5,0xfe,
#define _O_OOOOOOOOO 0x5,0xff,
#define _OO_________ 0x6,0,
#define _OO________O 0x6,0x1,
#define _OO_______O_ 0x6,0x2,
#define _OO_______OO 0x6,0x3,
#define _OO______O__ 0x6,0x4,
#define _OO______O_O 0x6,0x5,
#define _OO______OO_ 0x6,0x6,
#define _OO______OOO 0x6,0x7,
#define _OO_____O___ 0x6,0x8,
#define _OO_____O__O 0x6,0x9,
#define _OO_____O_O_ 0x6,0xa,
#define _OO_____O_OO 0x6,0xb,
#define _OO_____OO__ 0x6,0xc,
#define _OO_____OO_O 0x6,0xd,
#define _OO_____OOO_ 0x6,0xe,
#define _OO_____OOOO 0x6,0xf,
#define _OO____O____ 0x6,0x10,
#define _OO____O___O 0x6,0x11,
#define _OO____O__O_ 0x6,0x12,
#define _OO____O__OO 0x6,0x13,
#define _OO____O_O__ 0x6,0x14,
#define _OO____O_O_O 0x6,0x15,
#define _OO____O_OO_ 0x6,0x16,
#define _OO____O_OOO 0x6,0x17,
#define _OO____OO___ 0x6,0x18,
#define _OO____OO__O 0x6,0x19,
#define _OO____OO_O_ 0x6,0x1a,
#define _OO____OO_OO 0x6,0x1b,
#define _OO____OOO__ 0x6,0x1c,
#define _OO____OOO_O 0x6,0x1d,
#define _OO____OOOO_ 0x6,0x1e,
#define _OO____OOOOO 0x6,0x1f,
#define _OO___O_____ 0x6,0x20,
#define _OO___O____O 0x6,0x21,
#define _OO___O___O_ 0x6,0x22,
#define _OO___O___OO 0x6,0x23,
#define _OO___O__O__ 0x6,0x24,
#define _OO___O__O_O 0x6,0x25,
#define _OO___O__OO_ 0x6,0x26,
#define _OO___O__OOO 0x6,0x27,
#define _OO___O_O___ 0x6,0x28,
#define _OO___O_O__O 0x6,0x29,
#define _OO___O_O_O_ 0x6,0x2a,
#define _OO___O_O_OO 0x6,0x2b,
#define _OO___O_OO__ 0x6,0x2c,
#define _OO___O_OO_O 0x6,0x2d,
#define _OO___O_OOO_ 0x6,0x2e,
#define _OO___O_OOOO 0x6,0x2f,
#define _OO___OO____ 0x6,0x30,
#define _OO___OO___O 0x6,0x31,
#define _OO___OO__O_ 0x6,0x32,
#define _OO___OO__OO 0x6,0x33,
#define _OO___OO_O__ 0x6,0x34,
#define _OO___OO_O_O 0x6,0x35,
#define _OO___OO_OO_ 0x6,0x36,
#define _OO___OO_OOO 0x6,0x37,
#define _OO___OOO___ 0x6,0x38,
#define _OO___OOO__O 0x6,0x39,
#define _OO___OOO_O_ 0x6,0x3a,
#define _OO___OOO_OO 0x6,0x3b,
#define _OO___OOOO__ 0x6,0x3c,
#define _OO___OOOO_O 0x6,0x3d,
#define _OO___OOOOO_ 0x6,0x3e,
#define _OO___OOOOOO 0x6,0x3f,
#define _OO__O______ 0x6,0x40,
#define _OO__O_____O 0x6,0x41,
#define _OO__O____O_ 0x6,0x42,
#define _OO__O____OO 0x6,0x43,
#define _OO__O___O__ 0x6,0x44,
#define _OO__O___O_O 0x6,0x45,
#define _OO__O___OO_ 0x6,0x46,
#define _OO__O___OOO 0x6,0x47,
#define _OO__O__O___ 0x6,0x48,
#define _OO__O__O__O 0x6,0x49,
#define _OO__O__O_O_ 0x6,0x4a,
#define _OO__O__O_OO 0x6,0x4b,
#define _OO__O__OO__ 0x6,0x4c,
#define _OO__O__OO_O 0x6,0x4d,
#define _OO__O__OOO_ 0x6,0x4e,
#define _OO__O__OOOO 0x6,0x4f,
#define _OO__O_O____ 0x6,0x50,
#define _OO__O_O___O 0x6,0x51,
#define _OO__O_O__O_ 0x6,0x52,
#define _OO__O_O__OO 0x6,0x53,
#define _OO__O_O_O__ 0x6,0x54,
#define _OO__O_O_O_O 0x6,0x55,
#define _OO__O_O_OO_ 0x6,0x56,
#define _OO__O_O_OOO 0x6,0x57,
#define _OO__O_OO___ 0x6,0x58,
#define _OO__O_OO__O 0x6,0x59,
#define _OO__O_OO_O_ 0x6,0x5a,
#define _OO__O_OO_OO 0x6,0x5b,
#define _OO__O_OOO__ 0x6,0x5c,
#define _OO__O_OOO_O 0x6,0x5d,
#define _OO__O_OOOO_ 0x6,0x5e,
#define _OO__O_OOOOO 0x6,0x5f,
#define _OO__OO_____ 0x6,0x60,
#define _OO__OO____O 0x6,0x61,
#define _OO__OO___O_ 0x6,0x62,
#define _OO__OO___OO 0x6,0x63,
#define _OO__OO__O__ 0x6,0x64,
#define _OO__OO__O_O 0x6,0x65,
#define _OO__OO__OO_ 0x6,0x66,
#define _OO__OO__OOO 0x6,0x67,
#define _OO__OO_O___ 0x6,0x68,
#define _OO__OO_O__O 0x6,0x69,
#define _OO__OO_O_O_ 0x6,0x6a,
#define _OO__OO_O_OO 0x6,0x6b,
#define _OO__OO_OO__ 0x6,0x6c,
#define _OO__OO_OO_O 0x6,0x6d,
#define _OO__OO_OOO_ 0x6,0x6e,
#define _OO__OO_OOOO 0x6,0x6f,
#define _OO__OOO____ 0x6,0x70,
#define _OO__OOO___O 0x6,0x71,
#define _OO__OOO__O_ 0x6,0x72,
#define _OO__OOO__OO 0x6,0x73,
#define _OO__OOO_O__ 0x6,0x74,
#define _OO__OOO_O_O 0x6,0x75,
#define _OO__OOO_OO_ 0x6,0x76,
#define _OO__OOO_OOO 0x6,0x77,
#define _OO__OOOO___ 0x6,0x78,
#define _OO__OOOO__O 0x6,0x79,
#define _OO__OOOO_O_ 0x6,0x7a,
#define _OO__OOOO_OO 0x6,0x7b,
#define _OO__OOOOO__ 0x6,0x7c,
#define _OO__OOOOO_O 0x6,0x7d,
#define _OO__OOOOOO_ 0x6,0x7e,
#define _OO__OOOOOOO 0x6,0x7f,
#define _OO_O_______ 0x6,0x80,
#define _OO_O______O 0x6,0x81,
#define _OO_O_____O_ 0x6,0x82,
#define _OO_O_____OO 0x6,0x83,
#define _OO_O____O__ 0x6,0x84,
#define _OO_O____O_O 0x6,0x85,
#define _OO_O____OO_ 0x6,0x86,
#define _OO_O____OOO 0x6,0x87,
#define _OO_O___O___ 0x6,0x88,
#define _OO_O___O__O 0x6,0x89,
#define _OO_O___O_O_ 0x6,0x8a,
#define _OO_O___O_OO 0x6,0x8b,
#define _OO_O___OO__ 0x6,0x8c,
#define _OO_O___OO_O 0x6,0x8d,
#define _OO_O___OOO_ 0x6,0x8e,
#define _OO_O___OOOO 0x6,0x8f,
#define _OO_O__O____ 0x6,0x90,
#define _OO_O__O___O 0x6,0x91,
#define _OO_O__O__O_ 0x6,0x92,
#define _OO_O__O__OO 0x6,0x93,
#define _OO_O__O_O__ 0x6,0x94,
#define _OO_O__O_O_O 0x6,0x95,
#define _OO_O__O_OO_ 0x6,0x96,
#define _OO_O__O_OOO 0x6,0x97,
#define _OO_O__OO___ 0x6,0x98,
#define _OO_O__OO__O 0x6,0x99,
#define _OO_O__OO_O_ 0x6,0x9a,
#define _OO_O__OO_OO 0x6,0x9b,
#define _OO_O__OOO__ 0x6,0x9c,
#define _OO_O__OOO_O 0x6,0x9d,
#define _OO_O__OOOO_ 0x6,0x9e,
#define _OO_O__OOOOO 0x6,0x9f,
#define _OO_O_O_____ 0x6,0xa0,
#define _OO_O_O____O 0x6,0xa1,
#define _OO_O_O___O_ 0x6,0xa2,
#define _OO_O_O___OO 0x6,0xa3,
#define _OO_O_O__O__ 0x6,0xa4,
#define _OO_O_O__O_O 0x6,0xa5,
#define _OO_O_O__OO_ 0x6,0xa6,
#define _OO_O_O__OOO 0x6,0xa7,
#define _OO_O_O_O___ 0x6,0xa8,
#define _OO_O_O_O__O 0x6,0xa9,
#define _OO_O_O_O_O_ 0x6,0xaa,
#define _OO_O_O_O_OO 0x6,0xab,
#define _OO_O_O_OO__ 0x6,0xac,
#define _OO_O_O_OO_O 0x6,0xad,
#define _OO_O_O_OOO_ 0x6,0xae,
#define _OO_O_O_OOOO 0x6,0xaf,
#define _OO_O_OO____ 0x6,0xb0,
#define _OO_O_OO___O 0x6,0xb1,
#define _OO_O_OO__O_ 0x6,0xb2,
#define _OO_O_OO__OO 0x6,0xb3,
#define _OO_O_OO_O__ 0x6,0xb4,
#define _OO_O_OO_O_O 0x6,0xb5,
#define _OO_O_OO_OO_ 0x6,0xb6,
#define _OO_O_OO_OOO 0x6,0xb7,
#define _OO_O_OOO___ 0x6,0xb8,
#define _OO_O_OOO__O 0x6,0xb9,
#define _OO_O_OOO_O_ 0x6,0xba,
#define _OO_O_OOO_OO 0x6,0xbb,
#define _OO_O_OOOO__ 0x6,0xbc,
#define _OO_O_OOOO_O 0x6,0xbd,
#define _OO_O_OOOOO_ 0x6,0xbe,
#define _OO_O_OOOOOO 0x6,0xbf,
#define _OO_OO______ 0x6,0xc0,
#define _OO_OO_____O 0x6,0xc1,
#define _OO_OO____O_ 0x6,0xc2,
#define _OO_OO____OO 0x6,0xc3,
#define _OO_OO___O__ 0x6,0xc4,
#define _OO_OO___O_O 0x6,0xc5,
#define _OO_OO___OO_ 0x6,0xc6,
#define _OO_OO___OOO 0x6,0xc7,
#define _OO_OO__O___ 0x6,0xc8,
#define _OO_OO__O__O 0x6,0xc9,
#define _OO_OO__O_O_ 0x6,0xca,
#define _OO_OO__O_OO 0x6,0xcb,
#define _OO_OO__OO__ 0x6,0xcc,
#define _OO_OO__OO_O 0x6,0xcd,
#define _OO_OO__OOO_ 0x6,0xce,
#define _OO_OO__OOOO 0x6,0xcf,
#define _OO_OO_O____ 0x6,0xd0,
#define _OO_OO_O___O 0x6,0xd1,
#define _OO_OO_O__O_ 0x6,0xd2,
#define _OO_OO_O__OO 0x6,0xd3,
#define _OO_OO_O_O__ 0x6,0xd4,
#define _OO_OO_O_O_O 0x6,0xd5,
#define _OO_OO_O_OO_ 0x6,0xd6,
#define _OO_OO_O_OOO 0x6,0xd7,
#define _OO_OO_OO___ 0x6,0xd8,
#define _OO_OO_OO__O 0x6,0xd9,
#define _OO_OO_OO_O_ 0x6,0xda,
#define _OO_OO_OO_OO 0x6,0xdb,
#define _OO_OO_OOO__ 0x6,0xdc,
#define _OO_OO_OOO_O 0x6,0xdd,
#define _OO_OO_OOOO_ 0x6,0xde,
#define _OO_OO_OOOOO 0x6,0xdf,
#define _OO_OOO_____ 0x6,0xe0,
#define _OO_OOO____O 0x6,0xe1,
#define _OO_OOO___O_ 0x6,0xe2,
#define _OO_OOO___OO 0x6,0xe3,
#define _OO_OOO__O__ 0x6,0xe4,
#define _OO_OOO__O_O 0x6,0xe5,
#define _OO_OOO__OO_ 0x6,0xe6,
#define _OO_OOO__OOO 0x6,0xe7,
#define _OO_OOO_O___ 0x6,0xe8,
#define _OO_OOO_O__O 0x6,0xe9,
#define _OO_OOO_O_O_ 0x6,0xea,
#define _OO_OOO_O_OO 0x6,0xeb,
#define _OO_OOO_OO__ 0x6,0xec,
#define _OO_OOO_OO_O 0x6,0xed,
#define _OO_OOO_OOO_ 0x6,0xee,
#define _OO_OOO_OOOO 0x6,0xef,
#define _OO_OOOO____ 0x6,0xf0,
#define _OO_OOOO___O 0x6,0xf1,
#define _OO_OOOO__O_ 0x6,0xf2,
#define _OO_OOOO__OO 0x6,0xf3,
#define _OO_OOOO_O__ 0x6,0xf4,
#define _OO_OOOO_O_O 0x6,0xf5,
#define _OO_OOOO_OO_ 0x6,0xf6,
#define _OO_OOOO_OOO 0x6,0xf7,
#define _OO_OOOOO___ 0x6,0xf8,
#define _OO_OOOOO__O 0x6,0xf9,
#define _OO_OOOOO_O_ 0x6,0xfa,
#define _OO_OOOOO_OO 0x6,0xfb,
#define _OO_OOOOOO__ 0x6,0xfc,
#define _OO_OOOOOO_O 0x6,0xfd,
#define _OO_OOOOOOO_ 0x6,0xfe,
#define _OO_OOOOOOOO 0x6,0xff,
#define _OOO________ 0x7,0,
#define _OOO_______O 0x7,0x1,
#define _OOO______O_ 0x7,0x2,
#define _OOO______OO 0x7,0x3,
#define _OOO_____O__ 0x7,0x4,
#define _OOO_____O_O 0x7,0x5,
#define _OOO_____OO_ 0x7,0x6,
#define _OOO_____OOO 0x7,0x7,
#define _OOO____O___ 0x7,0x8,
#define _OOO____O__O 0x7,0x9,
#define _OOO____O_O_ 0x7,0xa,
#define _OOO____O_OO 0x7,0xb,
#define _OOO____OO__ 0x7,0xc,
#define _OOO____OO_O 0x7,0xd,
#define _OOO____OOO_ 0x7,0xe,
#define _OOO____OOOO 0x7,0xf,
#define _OOO___O____ 0x7,0x10,
#define _OOO___O___O 0x7,0x11,
#define _OOO___O__O_ 0x7,0x12,
#define _OOO___O__OO 0x7,0x13,
#define _OOO___O_O__ 0x7,0x14,
#define _OOO___O_O_O 0x7,0x15,
#define _OOO___O_OO_ 0x7,0x16,
#define _OOO___O_OOO 0x7,0x17,
#define _OOO___OO___ 0x7,0x18,
#define _OOO___OO__O 0x7,0x19,
#define _OOO___OO_O_ 0x7,0x1a,
#define _OOO___OO_OO 0x7,0x1b,
#define _OOO___OOO__ 0x7,0x1c,
#define _OOO___OOO_O 0x7,0x1d,
#define _OOO___OOOO_ 0x7,0x1e,
#define _OOO___OOOOO 0x7,0x1f,
#define _OOO__O_____ 0x7,0x20,
#define _OOO__O____O 0x7,0x21,
#define _OOO__O___O_ 0x7,0x22,
#define _OOO__O___OO 0x7,0x23,
#define _OOO__O__O__ 0x7,0x24,
#define _OOO__O__O_O 0x7,0x25,
#define _OOO__O__OO_ 0x7,0x26,
#define _OOO__O__OOO 0x7,0x27,
#define _OOO__O_O___ 0x7,0x28,
#define _OOO__O_O__O 0x7,0x29,
#define _OOO__O_O_O_ 0x7,0x2a,
#define _OOO__O_O_OO 0x7,0x2b,
#define _OOO__O_OO__ 0x7,0x2c,
#define _OOO__O_OO_O 0x7,0x2d,
#define _OOO__O_OOO_ 0x7,0x2e,
#define _OOO__O_OOOO 0x7,0x2f,
#define _OOO__OO____ 0x7,0x30,
#define _OOO__OO___O 0x7,0x31,
#define _OOO__OO__O_ 0x7,0x32,
#define _OOO__OO__OO 0x7,0x33,
#define _OOO__OO_O__ 0x7,0x34,
#define _OOO__OO_O_O 0x7,0x35,
#define _OOO__OO_OO_ 0x7,0x36,
#define _OOO__OO_OOO 0x7,0x37,
#define _OOO__OOO___ 0x7,0x38,
#define _OOO__OOO__O 0x7,0x39,
#define _OOO__OOO_O_ 0x7,0x3a,
#define _OOO__OOO_OO 0x7,0x3b,
#define _OOO__OOOO__ 0x7,0x3c,
#define _OOO__OOOO_O 0x7,0x3d,
#define _OOO__OOOOO_ 0x7,0x3e,
#define _OOO__OOOOOO 0x7,0x3f,
#define _OOO_O______ 0x7,0x40,
#define _OOO_O_____O 0x7,0x41,
#define _OOO_O____O_ 0x7,0x42,
#define _OOO_O____OO 0x7,0x43,
#define _OOO_O___O__ 0x7,0x44,
#define _OOO_O___O_O 0x7,0x45,
#define _OOO_O___OO_ 0x7,0x46,
#define _OOO_O___OOO 0x7,0x47,
#define _OOO_O__O___ 0x7,0x48,
#define _OOO_O__O__O 0x7,0x49,
#define _OOO_O__O_O_ 0x7,0x4a,
#define _OOO_O__O_OO 0x7,0x4b,
#define _OOO_O__OO__ 0x7,0x4c,
#define _OOO_O__OO_O 0x7,0x4d,
#define _OOO_O__OOO_ 0x7,0x4e,
#define _OOO_O__OOOO 0x7,0x4f,
#define _OOO_O_O____ 0x7,0x50,
#define _OOO_O_O___O 0x7,0x51,
#define _OOO_O_O__O_ 0x7,0x52,
#define _OOO_O_O__OO 0x7,0x53,
#define _OOO_O_O_O__ 0x7,0x54,
#define _OOO_O_O_O_O 0x7,0x55,
#define _OOO_O_O_OO_ 0x7,0x56,
#define _OOO_O_O_OOO 0x7,0x57,
#define _OOO_O_OO___ 0x7,0x58,
#define _OOO_O_OO__O 0x7,0x59,
#define _OOO_O_OO_O_ 0x7,0x5a,
#define _OOO_O_OO_OO 0x7,0x5b,
#define _OOO_O_OOO__ 0x7,0x5c,
#define _OOO_O_OOO_O 0x7,0x5d,
#define _OOO_O_OOOO_ 0x7,0x5e,
#define _OOO_O_OOOOO 0x7,0x5f,
#define _OOO_OO_____ 0x7,0x60,
#define _OOO_OO____O 0x7,0x61,
#define _OOO_OO___O_ 0x7,0x62,
#define _OOO_OO___OO 0x7,0x63,
#define _OOO_OO__O__ 0x7,0x64,
#define _OOO_OO__O_O 0x7,0x65,
#define _OOO_OO__OO_ 0x7,0x66,
#define _OOO_OO__OOO 0x7,0x67,
#define _OOO_OO_O___ 0x7,0x68,
#define _OOO_OO_O__O 0x7,0x69,
#define _OOO_OO_O_O_ 0x7,0x6a,
#define _OOO_OO_O_OO 0x7,0x6b,
#define _OOO_OO_OO__ 0x7,0x6c,
#define _OOO_OO_OO_O 0x7,0x6d,
#define _OOO_OO_OOO_ 0x7,0x6e,
#define _OOO_OO_OOOO 0x7,0x6f,
#define _OOO_OOO____ 0x7,0x70,
#define _OOO_OOO___O 0x7,0x71,
#define _OOO_OOO__O_ 0x7,0x72,
#define _OOO_OOO__OO 0x7,0x73,
#define _OOO_OOO_O__ 0x7,0x74,
#define _OOO_OOO_O_O 0x7,0x75,
#define _OOO_OOO_OO_ 0x7,0x76,
#define _OOO_OOO_OOO 0x7,0x77,
#define _OOO_OOOO___ 0x7,0x78,
#define _OOO_OOOO__O 0x7,0x79,
#define _OOO_OOOO_O_ 0x7,0x7a,
#define _OOO_OOOO_OO 0x7,0x7b,
#define _OOO_OOOOO__ 0x7,0x7c,
#define _OOO_OOOOO_O 0x7,0x7d,
#define _OOO_OOOOOO_ 0x7,0x7e,
#define _OOO_OOOOOOO 0x7,0x7f,
#define _OOOO_______ 0x7,0x80,
#define _OOOO______O 0x7,0x81,
#define _OOOO_____O_ 0x7,0x82,
#define _OOOO_____OO 0x7,0x83,
#define _OOOO____O__ 0x7,0x84,
#define _OOOO____O_O 0x7,0x85,
#define _OOOO____OO_ 0x7,0x86,
#define _OOOO____OOO 0x7,0x87,
#define _OOOO___O___ 0x7,0x88,
#define _OOOO___O__O 0x7,0x89,
#define _OOOO___O_O_ 0x7,0x8a,
#define _OOOO___O_OO 0x7,0x8b,
#define _OOOO___OO__ 0x7,0x8c,
#define _OOOO___OO_O 0x7,0x8d,
#define _OOOO___OOO_ 0x7,0x8e,
#define _OOOO___OOOO 0x7,0x8f,
#define _OOOO__O____ 0x7,0x90,
#define _OOOO__O___O 0x7,0x91,
#define _OOOO__O__O_ 0x7,0x92,
#define _OOOO__O__OO 0x7,0x93,
#define _OOOO__O_O__ 0x7,0x94,
#define _OOOO__O_O_O 0x7,0x95,
#define _OOOO__O_OO_ 0x7,0x96,
#define _OOOO__O_OOO 0x7,0x97,
#define _OOOO__OO___ 0x7,0x98,
#define _OOOO__OO__O 0x7,0x99,
#define _OOOO__OO_O_ 0x7,0x9a,
#define _OOOO__OO_OO 0x7,0x9b,
#define _OOOO__OOO__ 0x7,0x9c,
#define _OOOO__OOO_O 0x7,0x9d,
#define _OOOO__OOOO_ 0x7,0x9e,
#define _OOOO__OOOOO 0x7,0x9f,
#define _OOOO_O_____ 0x7,0xa0,
#define _OOOO_O____O 0x7,0xa1,
#define _OOOO_O___O_ 0x7,0xa2,
#define _OOOO_O___OO 0x7,0xa3,
#define _OOOO_O__O__ 0x7,0xa4,
#define _OOOO_O__O_O 0x7,0xa5,
#define _OOOO_O__OO_ 0x7,0xa6,
#define _OOOO_O__OOO 0x7,0xa7,
#define _OOOO_O_O___ 0x7,0xa8,
#define _OOOO_O_O__O 0x7,0xa9,
#define _OOOO_O_O_O_ 0x7,0xaa,
#define _OOOO_O_O_OO 0x7,0xab,
#define _OOOO_O_OO__ 0x7,0xac,
#define _OOOO_O_OO_O 0x7,0xad,
#define _OOOO_O_OOO_ 0x7,0xae,
#define _OOOO_O_OOOO 0x7,0xaf,
#define _OOOO_OO____ 0x7,0xb0,
#define _OOOO_OO___O 0x7,0xb1,
#define _OOOO_OO__O_ 0x7,0xb2,
#define _OOOO_OO__OO 0x7,0xb3,
#define _OOOO_OO_O__ 0x7,0xb4,
#define _OOOO_OO_O_O 0x7,0xb5,
#define _OOOO_OO_OO_ 0x7,0xb6,
#define _OOOO_OO_OOO 0x7,0xb7,
#define _OOOO_OOO___ 0x7,0xb8,
#define _OOOO_OOO__O 0x7,0xb9,
#define _OOOO_OOO_O_ 0x7,0xba,
#define _OOOO_OOO_OO 0x7,0xbb,
#define _OOOO_OOOO__ 0x7,0xbc,
#define _OOOO_OOOO_O 0x7,0xbd,
#define _OOOO_OOOOO_ 0x7,0xbe,
#define _OOOO_OOOOOO 0x7,0xbf,
#define _OOOOO______ 0x7,0xc0,
#define _OOOOO_____O 0x7,0xc1,
#define _OOOOO____O_ 0x7,0xc2,
#define _OOOOO____OO 0x7,0xc3,
#define _OOOOO___O__ 0x7,0xc4,
#define _OOOOO___O_O 0x7,0xc5,
#define _OOOOO___OO_ 0x7,0xc6,
#define _OOOOO___OOO 0x7,0xc7,
#define _OOOOO__O___ 0x7,0xc8,
#define _OOOOO__O__O 0x7,0xc9,
#define _OOOOO__O_O_ 0x7,0xca,
#define _OOOOO__O_OO 0x7,0xcb,
#define _OOOOO__OO__ 0x7,0xcc,
#define _OOOOO__OO_O 0x7,0xcd,
#define _OOOOO__OOO_ 0x7,0xce,
#define _OOOOO__OOOO 0x7,0xcf,
#define _OOOOO_O____ 0x7,0xd0,
#define _OOOOO_O___O 0x7,0xd1,
#define _OOOOO_O__O_ 0x7,0xd2,
#define _OOOOO_O__OO 0x7,0xd3,
#define _OOOOO_O_O__ 0x7,0xd4,
#define _OOOOO_O_O_O 0x7,0xd5,
#define _OOOOO_O_OO_ 0x7,0xd6,
#define _OOOOO_O_OOO 0x7,0xd7,
#define _OOOOO_OO___ 0x7,0xd8,
#define _OOOOO_OO__O 0x7,0xd9,
#define _OOOOO_OO_O_ 0x7,0xda,
#define _OOOOO_OO_OO 0x7,0xdb,
#define _OOOOO_OOO__ 0x7,0xdc,
#define _OOOOO_OOO_O 0x7,0xdd,
#define _OOOOO_OOOO_ 0x7,0xde,
#define _OOOOO_OOOOO 0x7,0xdf,
#define _OOOOOO_____ 0x7,0xe0,
#define _OOOOOO____O 0x7,0xe1,
#define _OOOOOO___O_ 0x7,0xe2,
#define _OOOOOO___OO 0x7,0xe3,
#define _OOOOOO__O__ 0x7,0xe4,
#define _OOOOOO__O_O 0x7,0xe5,
#define _OOOOOO__OO_ 0x7,0xe6,
#define _OOOOOO__OOO 0x7,0xe7,
#define _OOOOOO_O___ 0x7,0xe8,
#define _OOOOOO_O__O 0x7,0xe9,
#define _OOOOOO_O_O_ 0x7,0xea,
#define _OOOOOO_O_OO 0x7,0xeb,
#define _OOOOOO_OO__ 0x7,0xec,
#define _OOOOOO_OO_O 0x7,0xed,
#define _OOOOOO_OOO_ 0x7,0xee,
#define _OOOOOO_OOOO 0x7,0xef,
#define _OOOOOOO____ 0x7,0xf0,
#define _OOOOOOO___O 0x7,0xf1,
#define _OOOOOOO__O_ 0x7,0xf2,
#define _OOOOOOO__OO 0x7,0xf3,
#define _OOOOOOO_O__ 0x7,0xf4,
#define _OOOOOOO_O_O 0x7,0xf5,
#define _OOOOOOO_OO_ 0x7,0xf6,
#define _OOOOOOO_OOO 0x7,0xf7,
#define _OOOOOOOO___ 0x7,0xf8,
#define _OOOOOOOO__O 0x7,0xf9,
#define _OOOOOOOO_O_ 0x7,0xfa,
#define _OOOOOOOO_OO 0x7,0xfb,
#define _OOOOOOOOO__ 0x7,0xfc,
#define _OOOOOOOOO_O 0x7,0xfd,
#define _OOOOOOOOOO_ 0x7,0xfe,
#define _OOOOOOOOOOO 0x7,0xff,
#define O___________ 0x8,0,
#define O__________O 0x8,0x1,
#define O_________O_ 0x8,0x2,
#define O_________OO 0x8,0x3,
#define O________O__ 0x8,0x4,
#define O________O_O 0x8,0x5,
#define O________OO_ 0x8,0x6,
#define O________OOO 0x8,0x7,
#define O_______O___ 0x8,0x8,
#define O_______O__O 0x8,0x9,
#define O_______O_O_ 0x8,0xa,
#define O_______O_OO 0x8,0xb,
#define O_______OO__ 0x8,0xc,
#define O_______OO_O 0x8,0xd,
#define O_______OOO_ 0x8,0xe,
#define O_______OOOO 0x8,0xf,
#define O______O____ 0x8,0x10,
#define O______O___O 0x8,0x11,
#define O______O__O_ 0x8,0x12,
#define O______O__OO 0x8,0x13,
#define O______O_O__ 0x8,0x14,
#define O______O_O_O 0x8,0x15,
#define O______O_OO_ 0x8,0x16,
#define O______O_OOO 0x8,0x17,
#define O______OO___ 0x8,0x18,
#define O______OO__O 0x8,0x19,
#define O______OO_O_ 0x8,0x1a,
#define O______OO_OO 0x8,0x1b,
#define O______OOO__ 0x8,0x1c,
#define O______OOO_O 0x8,0x1d,
#define O______OOOO_ 0x8,0x1e,
#define O______OOOOO 0x8,0x1f,
#define O_____O_____ 0x8,0x20,
#define O_____O____O 0x8,0x21,
#define O_____O___O_ 0x8,0x22,
#define O_____O___OO 0x8,0x23,
#define O_____O__O__ 0x8,0x24,
#define O_____O__O_O 0x8,0x25,
#define O_____O__OO_ 0x8,0x26,
#define O_____O__OOO 0x8,0x27,
#define O_____O_O___ 0x8,0x28,
#define O_____O_O__O 0x8,0x29,
#define O_____O_O_O_ 0x8,0x2a,
#define O_____O_O_OO 0x8,0x2b,
#define O_____O_OO__ 0x8,0x2c,
#define O_____O_OO_O 0x8,0x2d,
#define O_____O_OOO_ 0x8,0x2e,
#define O_____O_OOOO 0x8,0x2f,
#define O_____OO____ 0x8,0x30,
#define O_____OO___O 0x8,0x31,
#define O_____OO__O_ 0x8,0x32,
#define O_____OO__OO 0x8,0x33,
#define O_____OO_O__ 0x8,0x34,
#define O_____OO_O_O 0x8,0x35,
#define O_____OO_OO_ 0x8,0x36,
#define O_____OO_OOO 0x8,0x37,
#define O_____OOO___ 0x8,0x38,
#define O_____OOO__O 0x8,0x39,
#define O_____OOO_O_ 0x8,0x3a,
#define O_____OOO_OO 0x8,0x3b,
#define O_____OOOO__ 0x8,0x3c,
#define O_____OOOO_O 0x8,0x3d,
#define O_____OOOOO_ 0x8,0x3e,
#define O_____OOOOOO 0x8,0x3f,
#define O____O______ 0x8,0x40,
#define O____O_____O 0x8,0x41,
#define O____O____O_ 0x8,0x42,
#define O____O____OO 0x8,0x43,
#define O____O___O__ 0x8,0x44,
#define O____O___O_O 0x8,0x45,
#define O____O___OO_ 0x8,0x46,
#define O____O___OOO 0x8,0x47,
#define O____O__O___ 0x8,0x48,
#define O____O__O__O 0x8,0x49,
#define O____O__O_O_ 0x8,0x4a,
#define O____O__O_OO 0x8,0x4b,
#define O____O__OO__ 0x8,0x4c,
#define O____O__OO_O 0x8,0x4d,
#define O____O__OOO_ 0x8,0x4e,
#define O____O__OOOO 0x8,0x4f,
#define O____O_O____ 0x8,0x50,
#define O____O_O___O 0x8,0x51,
#define O____O_O__O_ 0x8,0x52,
#define O____O_O__OO 0x8,0x53,
#define O____O_O_O__ 0x8,0x54,
#define O____O_O_O_O 0x8,0x55,
#define O____O_O_OO_ 0x8,0x56,
#define O____O_O_OOO 0x8,0x57,
#define O____O_OO___ 0x8,0x58,
#define O____O_OO__O 0x8,0x59,
#define O____O_OO_O_ 0x8,0x5a,
#define O____O_OO_OO 0x8,0x5b,
#define O____O_OOO__ 0x8,0x5c,
#define O____O_OOO_O 0x8,0x5d,
#define O____O_OOOO_ 0x8,0x5e,
#define O____O_OOOOO 0x8,0x5f,
#define O____OO_____ 0x8,0x60,
#define O____OO____O 0x8,0x61,
#define O____OO___O_ 0x8,0x62,
#define O____OO___OO 0x8,0x63,
#define O____OO__O__ 0x8,0x64,
#define O____OO__O_O 0x8,0x65,
#define O____OO__OO_ 0x8,0x66,
#define O____OO__OOO 0x8,0x67,
#define O____OO_O___ 0x8,0x68,
#define O____OO_O__O 0x8,0x69,
#define O____OO_O_O_ 0x8,0x6a,
#define O____OO_O_OO 0x8,0x6b,
#define O____OO_OO__ 0x8,0x6c,
#define O____OO_OO_O 0x8,0x6d,
#define O____OO_OOO_ 0x8,0x6e,
#define O____OO_OOOO 0x8,0x6f,
#define O____OOO____ 0x8,0x70,
#define O____OOO___O 0x8,0x71,
#define O____OOO__O_ 0x8,0x72,
#define O____OOO__OO 0x8,0x73,
#define O____OOO_O__ 0x8,0x74,
#define O____OOO_O_O 0x8,0x75,
#define O____OOO_OO_ 0x8,0x76,
#define O____OOO_OOO 0x8,0x77,
#define O____OOOO___ 0x8,0x78,
#define O____OOOO__O 0x8,0x79,
#define O____OOOO_O_ 0x8,0x7a,
#define O____OOOO_OO 0x8,0x7b,
#define O____OOOOO__ 0x8,0x7c,
#define O____OOOOO_O 0x8,0x7d,
#define O____OOOOOO_ 0x8,0x7e,
#define O____OOOOOOO 0x8,0x7f,
#define O___O_______ 0x8,0x80,
#define O___O______O 0x8,0x81,
#define O___O_____O_ 0x8,0x82,
#define O___O_____OO 0x8,0x83,
#define O___O____O__ 0x8,0x84,
#define O___O____O_O 0x8,0x85,
#define O___O____OO_ 0x8,0x86,
#define O___O____OOO 0x8,0x87,
#define O___O___O___ 0x8,0x88,
#define O___O___O__O 0x8,0x89,
#define O___O___O_O_ 0x8,0x8a,
#define O___O___O_OO 0x8,0x8b,
#define O___O___OO__ 0x8,0x8c,
#define O___O___OO_O 0x8,0x8d,
#define O___O___OOO_ 0x8,0x8e,
#define O___O___OOOO 0x8,0x8f,
#define O___O__O____ 0x8,0x90,
#define O___O__O___O 0x8,0x91,
#define O___O__O__O_ 0x8,0x92,
#define O___O__O__OO 0x8,0x93,
#define O___O__O_O__ 0x8,0x94,
#define O___O__O_O_O 0x8,0x95,
#define O___O__O_OO_ 0x8,0x96,
#define O___O__O_OOO 0x8,0x97,
#define O___O__OO___ 0x8,0x98,
#define O___O__OO__O 0x8,0x99,
#define O___O__OO_O_ 0x8,0x9a,
#define O___O__OO_OO 0x8,0x9b,
#define O___O__OOO__ 0x8,0x9c,
#define O___O__OOO_O 0x8,0x9d,
#define O___O__OOOO_ 0x8,0x9e,
#define O___O__OOOOO 0x8,0x9f,
#define O___O_O_____ 0x8,0xa0,
#define O___O_O____O 0x8,0xa1,
#define O___O_O___O_ 0x8,0xa2,
#define O___O_O___OO 0x8,0xa3,
#define O___O_O__O__ 0x8,0xa4,
#define O___O_O__O_O 0x8,0xa5,
#define O___O_O__OO_ 0x8,0xa6,
#define O___O_O__OOO 0x8,0xa7,
#define O___O_O_O___ 0x8,0xa8,
#define O___O_O_O__O 0x8,0xa9,
#define O___O_O_O_O_ 0x8,0xaa,
#define O___O_O_O_OO 0x8,0xab,
#define O___O_O_OO__ 0x8,0xac,
#define O___O_O_OO_O 0x8,0xad,
#define O___O_O_OOO_ 0x8,0xae,
#define O___O_O_OOOO 0x8,0xaf,
#define O___O_OO____ 0x8,0xb0,
#define O___O_OO___O 0x8,0xb1,
#define O___O_OO__O_ 0x8,0xb2,
#define O___O_OO__OO 0x8,0xb3,
#define O___O_OO_O__ 0x8,0xb4,
#define O___O_OO_O_O 0x8,0xb5,
#define O___O_OO_OO_ 0x8,0xb6,
#define O___O_OO_OOO 0x8,0xb7,
#define O___O_OOO___ 0x8,0xb8,
#define O___O_OOO__O 0x8,0xb9,
#define O___O_OOO_O_ 0x8,0xba,
#define O___O_OOO_OO 0x8,0xbb,
#define O___O_OOOO__ 0x8,0xbc,
#define O___O_OOOO_O 0x8,0xbd,
#define O___O_OOOOO_ 0x8,0xbe,
#define O___O_OOOOOO 0x8,0xbf,
#define O___OO______ 0x8,0xc0,
#define O___OO_____O 0x8,0xc1,
#define O___OO____O_ 0x8,0xc2,
#define O___OO____OO 0x8,0xc3,
#define O___OO___O__ 0x8,0xc4,
#define O___OO___O_O 0x8,0xc5,
#define O___OO___OO_ 0x8,0xc6,
#define O___OO___OOO 0x8,0xc7,
#define O___OO__O___ 0x8,0xc8,
#define O___OO__O__O 0x8,0xc9,
#define O___OO__O_O_ 0x8,0xca,
#define O___OO__O_OO 0x8,0xcb,
#define O___OO__OO__ 0x8,0xcc,
#define O___OO__OO_O 0x8,0xcd,
#define O___OO__OOO_ 0x8,0xce,
#define O___OO__OOOO 0x8,0xcf,
#define O___OO_O____ 0x8,0xd0,
#define O___OO_O___O 0x8,0xd1,
#define O___OO_O__O_ 0x8,0xd2,
#define O___OO_O__OO 0x8,0xd3,
#define O___OO_O_O__ 0x8,0xd4,
#define O___OO_O_O_O 0x8,0xd5,
#define O___OO_O_OO_ 0x8,0xd6,
#define O___OO_O_OOO 0x8,0xd7,
#define O___OO_OO___ 0x8,0xd8,
#define O___OO_OO__O 0x8,0xd9,
#define O___OO_OO_O_ 0x8,0xda,
#define O___OO_OO_OO 0x8,0xdb,
#define O___OO_OOO__ 0x8,0xdc,
#define O___OO_OOO_O 0x8,0xdd,
#define O___OO_OOOO_ 0x8,0xde,
#define O___OO_OOOOO 0x8,0xdf,
#define O___OOO_____ 0x8,0xe0,
#define O___OOO____O 0x8,0xe1,
#define O___OOO___O_ 0x8,0xe2,
#define O___OOO___OO 0x8,0xe3,
#define O___OOO__O__ 0x8,0xe4,
#define O___OOO__O_O 0x8,0xe5,
#define O___OOO__OO_ 0x8,0xe6,
#define O___OOO__OOO 0x8,0xe7,
#define O___OOO_O___ 0x8,0xe8,
#define O___OOO_O__O 0x8,0xe9,
#define O___OOO_O_O_ 0x8,0xea,
#define O___OOO_O_OO 0x8,0xeb,
#define O___OOO_OO__ 0x8,0xec,
#define O___OOO_OO_O 0x8,0xed,
#define O___OOO_OOO_ 0x8,0xee,
#define O___OOO_OOOO 0x8,0xef,
#define O___OOOO____ 0x8,0xf0,
#define O___OOOO___O 0x8,0xf1,
#define O___OOOO__O_ 0x8,0xf2,
#define O___OOOO__OO 0x8,0xf3,
#define O___OOOO_O__ 0x8,0xf4,
#define O___OOOO_O_O 0x8,0xf5,
#define O___OOOO_OO_ 0x8,0xf6,
#define O___OOOO_OOO 0x8,0xf7,
#define O___OOOOO___ 0x8,0xf8,
#define O___OOOOO__O 0x8,0xf9,
#define O___OOOOO_O_ 0x8,0xfa,
#define O___OOOOO_OO 0x8,0xfb,
#define O___OOOOOO__ 0x8,0xfc,
#define O___OOOOOO_O 0x8,0xfd,
#define O___OOOOOOO_ 0x8,0xfe,
#define O___OOOOOOOO 0x8,0xff,
#define O__O________ 0x9,0,
#define O__O_______O 0x9,0x1,
#define O__O______O_ 0x9,0x2,
#define O__O______OO 0x9,0x3,
#define O__O_____O__ 0x9,0x4,
#define O__O_____O_O 0x9,0x5,
#define O__O_____OO_ 0x9,0x6,
#define O__O_____OOO 0x9,0x7,
#define O__O____O___ 0x9,0x8,
#define O__O____O__O 0x9,0x9,
#define O__O____O_O_ 0x9,0xa,
#define O__O____O_OO 0x9,0xb,
#define O__O____OO__ 0x9,0xc,
#define O__O____OO_O 0x9,0xd,
#define O__O____OOO_ 0x9,0xe,
#define O__O____OOOO 0x9,0xf,
#define O__O___O____ 0x9,0x10,
#define O__O___O___O 0x9,0x11,
#define O__O___O__O_ 0x9,0x12,
#define O__O___O__OO 0x9,0x13,
#define O__O___O_O__ 0x9,0x14,
#define O__O___O_O_O 0x9,0x15,
#define O__O___O_OO_ 0x9,0x16,
#define O__O___O_OOO 0x9,0x17,
#define O__O___OO___ 0x9,0x18,
#define O__O___OO__O 0x9,0x19,
#define O__O___OO_O_ 0x9,0x1a,
#define O__O___OO_OO 0x9,0x1b,
#define O__O___OOO__ 0x9,0x1c,
#define O__O___OOO_O 0x9,0x1d,
#define O__O___OOOO_ 0x9,0x1e,
#define O__O___OOOOO 0x9,0x1f,
#define O__O__O_____ 0x9,0x20,
#define O__O__O____O 0x9,0x21,
#define O__O__O___O_ 0x9,0x22,
#define O__O__O___OO 0x9,0x23,
#define O__O__O__O__ 0x9,0x24,
#define O__O__O__O_O 0x9,0x25,
#define O__O__O__OO_ 0x9,0x26,
#define O__O__O__OOO 0x9,0x27,
#define O__O__O_O___ 0x9,0x28,
#define O__O__O_O__O 0x9,0x29,
#define O__O__O_O_O_ 0x9,0x2a,
#define O__O__O_O_OO 0x9,0x2b,
#define O__O__O_OO__ 0x9,0x2c,
#define O__O__O_OO_O 0x9,0x2d,
#define O__O__O_OOO_ 0x9,0x2e,
#define O__O__O_OOOO 0x9,0x2f,
#define O__O__OO____ 0x9,0x30,
#define O__O__OO___O 0x9,0x31,
#define O__O__OO__O_ 0x9,0x32,
#define O__O__OO__OO 0x9,0x33,
#define O__O__OO_O__ 0x9,0x34,
#define O__O__OO_O_O 0x9,0x35,
#define O__O__OO_OO_ 0x9,0x36,
#define O__O__OO_OOO 0x9,0x37,
#define O__O__OOO___ 0x9,0x38,
#define O__O__OOO__O 0x9,0x39,
#define O__O__OOO_O_ 0x9,0x3a,
#define O__O__OOO_OO 0x9,0x3b,
#define O__O__OOOO__ 0x9,0x3c,
#define O__O__OOOO_O 0x9,0x3d,
#define O__O__OOOOO_ 0x9,0x3e,
#define O__O__OOOOOO 0x9,0x3f,
#define O__O_O______ 0x9,0x40,
#define O__O_O_____O 0x9,0x41,
#define O__O_O____O_ 0x9,0x42,
#define O__O_O____OO 0x9,0x43,
#define O__O_O___O__ 0x9,0x44,
#define O__O_O___O_O 0x9,0x45,
#define O__O_O___OO_ 0x9,0x46,
#define O__O_O___OOO 0x9,0x47,
#define O__O_O__O___ 0x9,0x48,
#define O__O_O__O__O 0x9,0x49,
#define O__O_O__O_O_ 0x9,0x4a,
#define O__O_O__O_OO 0x9,0x4b,
#define O__O_O__OO__ 0x9,0x4c,
#define O__O_O__OO_O 0x9,0x4d,
#define O__O_O__OOO_ 0x9,0x4e,
#define O__O_O__OOOO 0x9,0x4f,
#define O__O_O_O____ 0x9,0x50,
#define O__O_O_O___O 0x9,0x51,
#define O__O_O_O__O_ 0x9,0x52,
#define O__O_O_O__OO 0x9,0x53,
#define O__O_O_O_O__ 0x9,0x54,
#define O__O_O_O_O_O 0x9,0x55,
#define O__O_O_O_OO_ 0x9,0x56,
#define O__O_O_O_OOO 0x9,0x57,
#define O__O_O_OO___ 0x9,0x58,
#define O__O_O_OO__O 0x9,0x59,
#define O__O_O_OO_O_ 0x9,0x5a,
#define O__O_O_OO_OO 0x9,0x5b,
#define O__O_O_OOO__ 0x9,0x5c,
#define O__O_O_OOO_O 0x9,0x5d,
#define O__O_O_OOOO_ 0x9,0x5e,
#define O__O_O_OOOOO 0x9,0x5f,
#define O__O_OO_____ 0x9,0x60,
#define O__O_OO____O 0x9,0x61,
#define O__O_OO___O_ 0x9,0x62,
#define O__O_OO___OO 0x9,0x63,
#define O__O_OO__O__ 0x9,0x64,
#define O__O_OO__O_O 0x9,0x65,
#define O__O_OO__OO_ 0x9,0x66,
#define O__O_OO__OOO 0x9,0x67,
#define O__O_OO_O___ 0x9,0x68,
#define O__O_OO_O__O 0x9,0x69,
#define O__O_OO_O_O_ 0x9,0x6a,
#define O__O_OO_O_OO 0x9,0x6b,
#define O__O_OO_OO__ 0x9,0x6c,
#define O__O_OO_OO_O 0x9,0x6d,
#define O__O_OO_OOO_ 0x9,0x6e,
#define O__O_OO_OOOO 0x9,0x6f,
#define O__O_OOO____ 0x9,0x70,
#define O__O_OOO___O 0x9,0x71,
#define O__O_OOO__O_ 0x9,0x72,
#define O__O_OOO__OO 0x9,0x73,
#define O__O_OOO_O__ 0x9,0x74,
#define O__O_OOO_O_O 0x9,0x75,
#define O__O_OOO_OO_ 0x9,0x76,
#define O__O_OOO_OOO 0x9,0x77,
#define O__O_OOOO___ 0x9,0x78,
#define O__O_OOOO__O 0x9,0x79,
#define O__O_OOOO_O_ 0x9,0x7a,
#define O__O_OOOO_OO 0x9,0x7b,
#define O__O_OOOOO__ 0x9,0x7c,
#define O__O_OOOOO_O 0x9,0x7d,
#define O__O_OOOOOO_ 0x9,0x7e,
#define O__O_OOOOOOO 0x9,0x7f,
#define O__OO_______ 0x9,0x80,
#define O__OO______O 0x9,0x81,
#define O__OO_____O_ 0x9,0x82,
#define O__OO_____OO 0x9,0x83,
#define O__OO____O__ 0x9,0x84,
#define O__OO____O_O 0x9,0x85,
#define O__OO____OO_ 0x9,0x86,
#define O__OO____OOO 0x9,0x87,
#define O__OO___O___ 0x9,0x88,
#define O__OO___O__O 0x9,0x89,
#define O__OO___O_O_ 0x9,0x8a,
#define O__OO___O_OO 0x9,0x8b,
#define O__OO___OO__ 0x9,0x8c,
#define O__OO___OO_O 0x9,0x8d,
#define O__OO___OOO_ 0x9,0x8e,
#define O__OO___OOOO 0x9,0x8f,
#define O__OO__O____ 0x9,0x90,
#define O__OO__O___O 0x9,0x91,
#define O__OO__O__O_ 0x9,0x92,
#define O__OO__O__OO 0x9,0x93,
#define O__OO__O_O__ 0x9,0x94,
#define O__OO__O_O_O 0x9,0x95,
#define O__OO__O_OO_ 0x9,0x96,
#define O__OO__O_OOO 0x9,0x97,
#define O__OO__OO___ 0x9,0x98,
#define O__OO__OO__O 0x9,0x99,
#define O__OO__OO_O_ 0x9,0x9a,
#define O__OO__OO_OO 0x9,0x9b,
#define O__OO__OOO__ 0x9,0x9c,
#define O__OO__OOO_O 0x9,0x9d,
#define O__OO__OOOO_ 0x9,0x9e,
#define O__OO__OOOOO 0x9,0x9f,
#define O__OO_O_____ 0x9,0xa0,
#define O__OO_O____O 0x9,0xa1,
#define O__OO_O___O_ 0x9,0xa2,
#define O__OO_O___OO 0x9,0xa3,
#define O__OO_O__O__ 0x9,0xa4,
#define O__OO_O__O_O 0x9,0xa5,
#define O__OO_O__OO_ 0x9,0xa6,
#define O__OO_O__OOO 0x9,0xa7,
#define O__OO_O_O___ 0x9,0xa8,
#define O__OO_O_O__O 0x9,0xa9,
#define O__OO_O_O_O_ 0x9,0xaa,
#define O__OO_O_O_OO 0x9,0xab,
#define O__OO_O_OO__ 0x9,0xac,
#define O__OO_O_OO_O 0x9,0xad,
#define O__OO_O_OOO_ 0x9,0xae,
#define O__OO_O_OOOO 0x9,0xaf,
#define O__OO_OO____ 0x9,0xb0,
#define O__OO_OO___O 0x9,0xb1,
#define O__OO_OO__O_ 0x9,0xb2,
#define O__OO_OO__OO 0x9,0xb3,
#define O__OO_OO_O__ 0x9,0xb4,
#define O__OO_OO_O_O 0x9,0xb5,
#define O__OO_OO_OO_ 0x9,0xb6,
#define O__OO_OO_OOO 0x9,0xb7,
#define O__OO_OOO___ 0x9,0xb8,
#define O__OO_OOO__O 0x9,0xb9,
#define O__OO_OOO_O_ 0x9,0xba,
#define O__OO_OOO_OO 0x9,0xbb,
#define O__OO_OOOO__ 0x9,0xbc,
#define O__OO_OOOO_O 0x9,0xbd,
#define O__OO_OOOOO_ 0x9,0xbe,
#define O__OO_OOOOOO 0x9,0xbf,
#define O__OOO______ 0x9,0xc0,
#define O__OOO_____O 0x9,0xc1,
#define O__OOO____O_ 0x9,0xc2,
#define O__OOO____OO 0x9,0xc3,
#define O__OOO___O__ 0x9,0xc4,
#define O__OOO___O_O 0x9,0xc5,
#define O__OOO___OO_ 0x9,0xc6,
#define O__OOO___OOO 0x9,0xc7,
#define O__OOO__O___ 0x9,0xc8,
#define O__OOO__O__O 0x9,0xc9,
#define O__OOO__O_O_ 0x9,0xca,
#define O__OOO__O_OO 0x9,0xcb,
#define O__OOO__OO__ 0x9,0xcc,
#define O__OOO__OO_O 0x9,0xcd,
#define O__OOO__OOO_ 0x9,0xce,
#define O__OOO__OOOO 0x9,0xcf,
#define O__OOO_O____ 0x9,0xd0,
#define O__OOO_O___O 0x9,0xd1,
#define O__OOO_O__O_ 0x9,0xd2,
#define O__OOO_O__OO 0x9,0xd3,
#define O__OOO_O_O__ 0x9,0xd4,
#define O__OOO_O_O_O 0x9,0xd5,
#define O__OOO_O_OO_ 0x9,0xd6,
#define O__OOO_O_OOO 0x9,0xd7,
#define O__OOO_OO___ 0x9,0xd8,
#define O__OOO_OO__O 0x9,0xd9,
#define O__OOO_OO_O_ 0x9,0xda,
#define O__OOO_OO_OO 0x9,0xdb,
#define O__OOO_OOO__ 0x9,0xdc,
#define O__OOO_OOO_O 0x9,0xdd,
#define O__OOO_OOOO_ 0x9,0xde,
#define O__OOO_OOOOO 0x9,0xdf,
#define O__OOOO_____ 0x9,0xe0,
#define O__OOOO____O 0x9,0xe1,
#define O__OOOO___O_ 0x9,0xe2,
#define O__OOOO___OO 0x9,0xe3,
#define O__OOOO__O__ 0x9,0xe4,
#define O__OOOO__O_O 0x9,0xe5,
#define O__OOOO__OO_ 0x9,0xe6,
#define O__OOOO__OOO 0x9,0xe7,
#define O__OOOO_O___ 0x9,0xe8,
#define O__OOOO_O__O 0x9,0xe9,
#define O__OOOO_O_O_ 0x9,0xea,
#define O__OOOO_O_OO 0x9,0xeb,
#define O__OOOO_OO__ 0x9,0xec,
#define O__OOOO_OO_O 0x9,0xed,
#define O__OOOO_OOO_ 0x9,0xee,
#define O__OOOO_OOOO 0x9,0xef,
#define O__OOOOO____ 0x9,0xf0,
#define O__OOOOO___O 0x9,0xf1,
#define O__OOOOO__O_ 0x9,0xf2,
#define O__OOOOO__OO 0x9,0xf3,
#define O__OOOOO_O__ 0x9,0xf4,
#define O__OOOOO_O_O 0x9,0xf5,
#define O__OOOOO_OO_ 0x9,0xf6,
#define O__OOOOO_OOO 0x9,0xf7,
#define O__OOOOOO___ 0x9,0xf8,
#define O__OOOOOO__O 0x9,0xf9,
#define O__OOOOOO_O_ 0x9,0xfa,
#define O__OOOOOO_OO 0x9,0xfb,
#define O__OOOOOOO__ 0x9,0xfc,
#define O__OOOOOOO_O 0x9,0xfd,
#define O__OOOOOOOO_ 0x9,0xfe,
#define O__OOOOOOOOO 0x9,0xff,
#define O_O_________ 0xa,0,
#define O_O________O 0xa,0x1,
#define O_O_______O_ 0xa,0x2,
#define O_O_______OO 0xa,0x3,
#define O_O______O__ 0xa,0x4,
#define O_O______O_O 0xa,0x5,
#define O_O______OO_ 0xa,0x6,
#define O_O______OOO 0xa,0x7,
#define O_O_____O___ 0xa,0x8,
#define O_O_____O__O 0xa,0x9,
#define O_O_____O_O_ 0xa,0xa,
#define O_O_____O_OO 0xa,0xb,
#define O_O_____OO__ 0xa,0xc,
#define O_O_____OO_O 0xa,0xd,
#define O_O_____OOO_ 0xa,0xe,
#define O_O_____OOOO 0xa,0xf,
#define O_O____O____ 0xa,0x10,
#define O_O____O___O 0xa,0x11,
#define O_O____O__O_ 0xa,0x12,
#define O_O____O__OO 0xa,0x13,
#define O_O____O_O__ 0xa,0x14,
#define O_O____O_O_O 0xa,0x15,
#define O_O____O_OO_ 0xa,0x16,
#define O_O____O_OOO 0xa,0x17,
#define O_O____OO___ 0xa,0x18,
#define O_O____OO__O 0xa,0x19,
#define O_O____OO_O_ 0xa,0x1a,
#define O_O____OO_OO 0xa,0x1b,
#define O_O____OOO__ 0xa,0x1c,
#define O_O____OOO_O 0xa,0x1d,
#define O_O____OOOO_ 0xa,0x1e,
#define O_O____OOOOO 0xa,0x1f,
#define O_O___O_____ 0xa,0x20,
#define O_O___O____O 0xa,0x21,
#define O_O___O___O_ 0xa,0x22,
#define O_O___O___OO 0xa,0x23,
#define O_O___O__O__ 0xa,0x24,
#define O_O___O__O_O 0xa,0x25,
#define O_O___O__OO_ 0xa,0x26,
#define O_O___O__OOO 0xa,0x27,
#define O_O___O_O___ 0xa,0x28,
#define O_O___O_O__O 0xa,0x29,
#define O_O___O_O_O_ 0xa,0x2a,
#define O_O___O_O_OO 0xa,0x2b,
#define O_O___O_OO__ 0xa,0x2c,
#define O_O___O_OO_O 0xa,0x2d,
#define O_O___O_OOO_ 0xa,0x2e,
#define O_O___O_OOOO 0xa,0x2f,
#define O_O___OO____ 0xa,0x30,
#define O_O___OO___O 0xa,0x31,
#define O_O___OO__O_ 0xa,0x32,
#define O_O___OO__OO 0xa,0x33,
#define O_O___OO_O__ 0xa,0x34,
#define O_O___OO_O_O 0xa,0x35,
#define O_O___OO_OO_ 0xa,0x36,
#define O_O___OO_OOO 0xa,0x37,
#define O_O___OOO___ 0xa,0x38,
#define O_O___OOO__O 0xa,0x39,
#define O_O___OOO_O_ 0xa,0x3a,
#define O_O___OOO_OO 0xa,0x3b,
#define O_O___OOOO__ 0xa,0x3c,
#define O_O___OOOO_O 0xa,0x3d,
#define O_O___OOOOO_ 0xa,0x3e,
#define O_O___OOOOOO 0xa,0x3f,
#define O_O__O______ 0xa,0x40,
#define O_O__O_____O 0xa,0x41,
#define O_O__O____O_ 0xa,0x42,
#define O_O__O____OO 0xa,0x43,
#define O_O__O___O__ 0xa,0x44,
#define O_O__O___O_O 0xa,0x45,
#define O_O__O___OO_ 0xa,0x46,
#define O_O__O___OOO 0xa,0x47,
#define O_O__O__O___ 0xa,0x48,
#define O_O__O__O__O 0xa,0x49,
#define O_O__O__O_O_ 0xa,0x4a,
#define O_O__O__O_OO 0xa,0x4b,
#define O_O__O__OO__ 0xa,0x4c,
#define O_O__O__OO_O 0xa,0x4d,
#define O_O__O__OOO_ 0xa,0x4e,
#define O_O__O__OOOO 0xa,0x4f,
#define O_O__O_O____ 0xa,0x50,
#define O_O__O_O___O 0xa,0x51,
#define O_O__O_O__O_ 0xa,0x52,
#define O_O__O_O__OO 0xa,0x53,
#define O_O__O_O_O__ 0xa,0x54,
#define O_O__O_O_O_O 0xa,0x55,
#define O_O__O_O_OO_ 0xa,0x56,
#define O_O__O_O_OOO 0xa,0x57,
#define O_O__O_OO___ 0xa,0x58,
#define O_O__O_OO__O 0xa,0x59,
#define O_O__O_OO_O_ 0xa,0x5a,
#define O_O__O_OO_OO 0xa,0x5b,
#define O_O__O_OOO__ 0xa,0x5c,
#define O_O__O_OOO_O 0xa,0x5d,
#define O_O__O_OOOO_ 0xa,0x5e,
#define O_O__O_OOOOO 0xa,0x5f,
#define O_O__OO_____ 0xa,0x60,
#define O_O__OO____O 0xa,0x61,
#define O_O__OO___O_ 0xa,0x62,
#define O_O__OO___OO 0xa,0x63,
#define O_O__OO__O__ 0xa,0x64,
#define O_O__OO__O_O 0xa,0x65,
#define O_O__OO__OO_ 0xa,0x66,
#define O_O__OO__OOO 0xa,0x67,
#define O_O__OO_O___ 0xa,0x68,
#define O_O__OO_O__O 0xa,0x69,
#define O_O__OO_O_O_ 0xa,0x6a,
#define O_O__OO_O_OO 0xa,0x6b,
#define O_O__OO_OO__ 0xa,0x6c,
#define O_O__OO_OO_O 0xa,0x6d,
#define O_O__OO_OOO_ 0xa,0x6e,
#define O_O__OO_OOOO 0xa,0x6f,
#define O_O__OOO____ 0xa,0x70,
#define O_O__OOO___O 0xa,0x71,
#define O_O__OOO__O_ 0xa,0x72,
#define O_O__OOO__OO 0xa,0x73,
#define O_O__OOO_O__ 0xa,0x74,
#define O_O__OOO_O_O 0xa,0x75,
#define O_O__OOO_OO_ 0xa,0x76,
#define O_O__OOO_OOO 0xa,0x77,
#define O_O__OOOO___ 0xa,0x78,
#define O_O__OOOO__O 0xa,0x79,
#define O_O__OOOO_O_ 0xa,0x7a,
#define O_O__OOOO_OO 0xa,0x7b,
#define O_O__OOOOO__ 0xa,0x7c,
#define O_O__OOOOO_O 0xa,0x7d,
#define O_O__OOOOOO_ 0xa,0x7e,
#define O_O__OOOOOOO 0xa,0x7f,
#define O_O_O_______ 0xa,0x80,
#define O_O_O______O 0xa,0x81,
#define O_O_O_____O_ 0xa,0x82,
#define O_O_O_____OO 0xa,0x83,
#define O_O_O____O__ 0xa,0x84,
#define O_O_O____O_O 0xa,0x85,
#define O_O_O____OO_ 0xa,0x86,
#define O_O_O____OOO 0xa,0x87,
#define O_O_O___O___ 0xa,0x88,
#define O_O_O___O__O 0xa,0x89,
#define O_O_O___O_O_ 0xa,0x8a,
#define O_O_O___O_OO 0xa,0x8b,
#define O_O_O___OO__ 0xa,0x8c,
#define O_O_O___OO_O 0xa,0x8d,
#define O_O_O___OOO_ 0xa,0x8e,
#define O_O_O___OOOO 0xa,0x8f,
#define O_O_O__O____ 0xa,0x90,
#define O_O_O__O___O 0xa,0x91,
#define O_O_O__O__O_ 0xa,0x92,
#define O_O_O__O__OO 0xa,0x93,
#define O_O_O__O_O__ 0xa,0x94,
#define O_O_O__O_O_O 0xa,0x95,
#define O_O_O__O_OO_ 0xa,0x96,
#define O_O_O__O_OOO 0xa,0x97,
#define O_O_O__OO___ 0xa,0x98,
#define O_O_O__OO__O 0xa,0x99,
#define O_O_O__OO_O_ 0xa,0x9a,
#define O_O_O__OO_OO 0xa,0x9b,
#define O_O_O__OOO__ 0xa,0x9c,
#define O_O_O__OOO_O 0xa,0x9d,
#define O_O_O__OOOO_ 0xa,0x9e,
#define O_O_O__OOOOO 0xa,0x9f,
#define O_O_O_O_____ 0xa,0xa0,
#define O_O_O_O____O 0xa,0xa1,
#define O_O_O_O___O_ 0xa,0xa2,
#define O_O_O_O___OO 0xa,0xa3,
#define O_O_O_O__O__ 0xa,0xa4,
#define O_O_O_O__O_O 0xa,0xa5,
#define O_O_O_O__OO_ 0xa,0xa6,
#define O_O_O_O__OOO 0xa,0xa7,
#define O_O_O_O_O___ 0xa,0xa8,
#define O_O_O_O_O__O 0xa,0xa9,
#define O_O_O_O_O_O_ 0xa,0xaa,
#define O_O_O_O_O_OO 0xa,0xab,
#define O_O_O_O_OO__ 0xa,0xac,
#define O_O_O_O_OO_O 0xa,0xad,
#define O_O_O_O_OOO_ 0xa,0xae,
#define O_O_O_O_OOOO 0xa,0xaf,
#define O_O_O_OO____ 0xa,0xb0,
#define O_O_O_OO___O 0xa,0xb1,
#define O_O_O_OO__O_ 0xa,0xb2,
#define O_O_O_OO__OO 0xa,0xb3,
#define O_O_O_OO_O__ 0xa,0xb4,
#define O_O_O_OO_O_O 0xa,0xb5,
#define O_O_O_OO_OO_ 0xa,0xb6,
#define O_O_O_OO_OOO 0xa,0xb7,
#define O_O_O_OOO___ 0xa,0xb8,
#define O_O_O_OOO__O 0xa,0xb9,
#define O_O_O_OOO_O_ 0xa,0xba,
#define O_O_O_OOO_OO 0xa,0xbb,
#define O_O_O_OOOO__ 0xa,0xbc,
#define O_O_O_OOOO_O 0xa,0xbd,
#define O_O_O_OOOOO_ 0xa,0xbe,
#define O_O_O_OOOOOO 0xa,0xbf,
#define O_O_OO______ 0xa,0xc0,
#define O_O_OO_____O 0xa,0xc1,
#define O_O_OO____O_ 0xa,0xc2,
#define O_O_OO____OO 0xa,0xc3,
#define O_O_OO___O__ 0xa,0xc4,
#define O_O_OO___O_O 0xa,0xc5,
#define O_O_OO___OO_ 0xa,0xc6,
#define O_O_OO___OOO 0xa,0xc7,
#define O_O_OO__O___ 0xa,0xc8,
#define O_O_OO__O__O 0xa,0xc9,
#define O_O_OO__O_O_ 0xa,0xca,
#define O_O_OO__O_OO 0xa,0xcb,
#define O_O_OO__OO__ 0xa,0xcc,
#define O_O_OO__OO_O 0xa,0xcd,
#define O_O_OO__OOO_ 0xa,0xce,
#define O_O_OO__OOOO 0xa,0xcf,
#define O_O_OO_O____ 0xa,0xd0,
#define O_O_OO_O___O 0xa,0xd1,
#define O_O_OO_O__O_ 0xa,0xd2,
#define O_O_OO_O__OO 0xa,0xd3,
#define O_O_OO_O_O__ 0xa,0xd4,
#define O_O_OO_O_O_O 0xa,0xd5,
#define O_O_OO_O_OO_ 0xa,0xd6,
#define O_O_OO_O_OOO 0xa,0xd7,
#define O_O_OO_OO___ 0xa,0xd8,
#define O_O_OO_OO__O 0xa,0xd9,
#define O_O_OO_OO_O_ 0xa,0xda,
#define O_O_OO_OO_OO 0xa,0xdb,
#define O_O_OO_OOO__ 0xa,0xdc,
#define O_O_OO_OOO_O 0xa,0xdd,
#define O_O_OO_OOOO_ 0xa,0xde,
#define O_O_OO_OOOOO 0xa,0xdf,
#define O_O_OOO_____ 0xa,0xe0,
#define O_O_OOO____O 0xa,0xe1,
#define O_O_OOO___O_ 0xa,0xe2,
#define O_O_OOO___OO 0xa,0xe3,
#define O_O_OOO__O__ 0xa,0xe4,
#define O_O_OOO__O_O 0xa,0xe5,
#define O_O_OOO__OO_ 0xa,0xe6,
#define O_O_OOO__OOO 0xa,0xe7,
#define O_O_OOO_O___ 0xa,0xe8,
#define O_O_OOO_O__O 0xa,0xe9,
#define O_O_OOO_O_O_ 0xa,0xea,
#define O_O_OOO_O_OO 0xa,0xeb,
#define O_O_OOO_OO__ 0xa,0xec,
#define O_O_OOO_OO_O 0xa,0xed,
#define O_O_OOO_OOO_ 0xa,0xee,
#define O_O_OOO_OOOO 0xa,0xef,
#define O_O_OOOO____ 0xa,0xf0,
#define O_O_OOOO___O 0xa,0xf1,
#define O_O_OOOO__O_ 0xa,0xf2,
#define O_O_OOOO__OO 0xa,0xf3,
#define O_O_OOOO_O__ 0xa,0xf4,
#define O_O_OOOO_O_O 0xa,0xf5,
#define O_O_OOOO_OO_ 0xa,0xf6,
#define O_O_OOOO_OOO 0xa,0xf7,
#define O_O_OOOOO___ 0xa,0xf8,
#define O_O_OOOOO__O 0xa,0xf9,
#define O_O_OOOOO_O_ 0xa,0xfa,
#define O_O_OOOOO_OO 0xa,0xfb,
#define O_O_OOOOOO__ 0xa,0xfc,
#define O_O_OOOOOO_O 0xa,0xfd,
#define O_O_OOOOOOO_ 0xa,0xfe,
#define O_O_OOOOOOOO 0xa,0xff,
#define O_OO________ 0xb,0,
#define O_OO_______O 0xb,0x1,
#define O_OO______O_ 0xb,0x2,
#define O_OO______OO 0xb,0x3,
#define O_OO_____O__ 0xb,0x4,
#define O_OO_____O_O 0xb,0x5,
#define O_OO_____OO_ 0xb,0x6,
#define O_OO_____OOO 0xb,0x7,
#define O_OO____O___ 0xb,0x8,
#define O_OO____O__O 0xb,0x9,
#define O_OO____O_O_ 0xb,0xa,
#define O_OO____O_OO 0xb,0xb,
#define O_OO____OO__ 0xb,0xc,
#define O_OO____OO_O 0xb,0xd,
#define O_OO____OOO_ 0xb,0xe,
#define O_OO____OOOO 0xb,0xf,
#define O_OO___O____ 0xb,0x10,
#define O_OO___O___O 0xb,0x11,
#define O_OO___O__O_ 0xb,0x12,
#define O_OO___O__OO 0xb,0x13,
#define O_OO___O_O__ 0xb,0x14,
#define O_OO___O_O_O 0xb,0x15,
#define O_OO___O_OO_ 0xb,0x16,
#define O_OO___O_OOO 0xb,0x17,
#define O_OO___OO___ 0xb,0x18,
#define O_OO___OO__O 0xb,0x19,
#define O_OO___OO_O_ 0xb,0x1a,
#define O_OO___OO_OO 0xb,0x1b,
#define O_OO___OOO__ 0xb,0x1c,
#define O_OO___OOO_O 0xb,0x1d,
#define O_OO___OOOO_ 0xb,0x1e,
#define O_OO___OOOOO 0xb,0x1f,
#define O_OO__O_____ 0xb,0x20,
#define O_OO__O____O 0xb,0x21,
#define O_OO__O___O_ 0xb,0x22,
#define O_OO__O___OO 0xb,0x23,
#define O_OO__O__O__ 0xb,0x24,
#define O_OO__O__O_O 0xb,0x25,
#define O_OO__O__OO_ 0xb,0x26,
#define O_OO__O__OOO 0xb,0x27,
#define O_OO__O_O___ 0xb,0x28,
#define O_OO__O_O__O 0xb,0x29,
#define O_OO__O_O_O_ 0xb,0x2a,
#define O_OO__O_O_OO 0xb,0x2b,
#define O_OO__O_OO__ 0xb,0x2c,
#define O_OO__O_OO_O 0xb,0x2d,
#define O_OO__O_OOO_ 0xb,0x2e,
#define O_OO__O_OOOO 0xb,0x2f,
#define O_OO__OO____ 0xb,0x30,
#define O_OO__OO___O 0xb,0x31,
#define O_OO__OO__O_ 0xb,0x32,
#define O_OO__OO__OO 0xb,0x33,
#define O_OO__OO_O__ 0xb,0x34,
#define O_OO__OO_O_O 0xb,0x35,
#define O_OO__OO_OO_ 0xb,0x36,
#define O_OO__OO_OOO 0xb,0x37,
#define O_OO__OOO___ 0xb,0x38,
#define O_OO__OOO__O 0xb,0x39,
#define O_OO__OOO_O_ 0xb,0x3a,
#define O_OO__OOO_OO 0xb,0x3b,
#define O_OO__OOOO__ 0xb,0x3c,
#define O_OO__OOOO_O 0xb,0x3d,
#define O_OO__OOOOO_ 0xb,0x3e,
#define O_OO__OOOOOO 0xb,0x3f,
#define O_OO_O______ 0xb,0x40,
#define O_OO_O_____O 0xb,0x41,
#define O_OO_O____O_ 0xb,0x42,
#define O_OO_O____OO 0xb,0x43,
#define O_OO_O___O__ 0xb,0x44,
#define O_OO_O___O_O 0xb,0x45,
#define O_OO_O___OO_ 0xb,0x46,
#define O_OO_O___OOO 0xb,0x47,
#define O_OO_O__O___ 0xb,0x48,
#define O_OO_O__O__O 0xb,0x49,
#define O_OO_O__O_O_ 0xb,0x4a,
#define O_OO_O__O_OO 0xb,0x4b,
#define O_OO_O__OO__ 0xb,0x4c,
#define O_OO_O__OO_O 0xb,0x4d,
#define O_OO_O__OOO_ 0xb,0x4e,
#define O_OO_O__OOOO 0xb,0x4f,
#define O_OO_O_O____ 0xb,0x50,
#define O_OO_O_O___O 0xb,0x51,
#define O_OO_O_O__O_ 0xb,0x52,
#define O_OO_O_O__OO 0xb,0x53,
#define O_OO_O_O_O__ 0xb,0x54,
#define O_OO_O_O_O_O 0xb,0x55,
#define O_OO_O_O_OO_ 0xb,0x56,
#define O_OO_O_O_OOO 0xb,0x57,
#define O_OO_O_OO___ 0xb,0x58,
#define O_OO_O_OO__O 0xb,0x59,
#define O_OO_O_OO_O_ 0xb,0x5a,
#define O_OO_O_OO_OO 0xb,0x5b,
#define O_OO_O_OOO__ 0xb,0x5c,
#define O_OO_O_OOO_O 0xb,0x5d,
#define O_OO_O_OOOO_ 0xb,0x5e,
#define O_OO_O_OOOOO 0xb,0x5f,
#define O_OO_OO_____ 0xb,0x60,
#define O_OO_OO____O 0xb,0x61,
#define O_OO_OO___O_ 0xb,0x62,
#define O_OO_OO___OO 0xb,0x63,
#define O_OO_OO__O__ 0xb,0x64,
#define O_OO_OO__O_O 0xb,0x65,
#define O_OO_OO__OO_ 0xb,0x66,
#define O_OO_OO__OOO 0xb,0x67,
#define O_OO_OO_O___ 0xb,0x68,
#define O_OO_OO_O__O 0xb,0x69,
#define O_OO_OO_O_O_ 0xb,0x6a,
#define O_OO_OO_O_OO 0xb,0x6b,
#define O_OO_OO_OO__ 0xb,0x6c,
#define O_OO_OO_OO_O 0xb,0x6d,
#define O_OO_OO_OOO_ 0xb,0x6e,
#define O_OO_OO_OOOO 0xb,0x6f,
#define O_OO_OOO____ 0xb,0x70,
#define O_OO_OOO___O 0xb,0x71,
#define O_OO_OOO__O_ 0xb,0x72,
#define O_OO_OOO__OO 0xb,0x73,
#define O_OO_OOO_O__ 0xb,0x74,
#define O_OO_OOO_O_O 0xb,0x75,
#define O_OO_OOO_OO_ 0xb,0x76,
#define O_OO_OOO_OOO 0xb,0x77,
#define O_OO_OOOO___ 0xb,0x78,
#define O_OO_OOOO__O 0xb,0x79,
#define O_OO_OOOO_O_ 0xb,0x7a,
#define O_OO_OOOO_OO 0xb,0x7b,
#define O_OO_OOOOO__ 0xb,0x7c,
#define O_OO_OOOOO_O 0xb,0x7d,
#define O_OO_OOOOOO_ 0xb,0x7e,
#define O_OO_OOOOOOO 0xb,0x7f,
#define O_OOO_______ 0xb,0x80,
#define O_OOO______O 0xb,0x81,
#define O_OOO_____O_ 0xb,0x82,
#define O_OOO_____OO 0xb,0x83,
#define O_OOO____O__ 0xb,0x84,
#define O_OOO____O_O 0xb,0x85,
#define O_OOO____OO_ 0xb,0x86,
#define O_OOO____OOO 0xb,0x87,
#define O_OOO___O___ 0xb,0x88,
#define O_OOO___O__O 0xb,0x89,
#define O_OOO___O_O_ 0xb,0x8a,
#define O_OOO___O_OO 0xb,0x8b,
#define O_OOO___OO__ 0xb,0x8c,
#define O_OOO___OO_O 0xb,0x8d,
#define O_OOO___OOO_ 0xb,0x8e,
#define O_OOO___OOOO 0xb,0x8f,
#define O_OOO__O____ 0xb,0x90,
#define O_OOO__O___O 0xb,0x91,
#define O_OOO__O__O_ 0xb,0x92,
#define O_OOO__O__OO 0xb,0x93,
#define O_OOO__O_O__ 0xb,0x94,
#define O_OOO__O_O_O 0xb,0x95,
#define O_OOO__O_OO_ 0xb,0x96,
#define O_OOO__O_OOO 0xb,0x97,
#define O_OOO__OO___ 0xb,0x98,
#define O_OOO__OO__O 0xb,0x99,
#define O_OOO__OO_O_ 0xb,0x9a,
#define O_OOO__OO_OO 0xb,0x9b,
#define O_OOO__OOO__ 0xb,0x9c,
#define O_OOO__OOO_O 0xb,0x9d,
#define O_OOO__OOOO_ 0xb,0x9e,
#define O_OOO__OOOOO 0xb,0x9f,
#define O_OOO_O_____ 0xb,0xa0,
#define O_OOO_O____O 0xb,0xa1,
#define O_OOO_O___O_ 0xb,0xa2,
#define O_OOO_O___OO 0xb,0xa3,
#define O_OOO_O__O__ 0xb,0xa4,
#define O_OOO_O__O_O 0xb,0xa5,
#define O_OOO_O__OO_ 0xb,0xa6,
#define O_OOO_O__OOO 0xb,0xa7,
#define O_OOO_O_O___ 0xb,0xa8,
#define O_OOO_O_O__O 0xb,0xa9,
#define O_OOO_O_O_O_ 0xb,0xaa,
#define O_OOO_O_O_OO 0xb,0xab,
#define O_OOO_O_OO__ 0xb,0xac,
#define O_OOO_O_OO_O 0xb,0xad,
#define O_OOO_O_OOO_ 0xb,0xae,
#define O_OOO_O_OOOO 0xb,0xaf,
#define O_OOO_OO____ 0xb,0xb0,
#define O_OOO_OO___O 0xb,0xb1,
#define O_OOO_OO__O_ 0xb,0xb2,
#define O_OOO_OO__OO 0xb,0xb3,
#define O_OOO_OO_O__ 0xb,0xb4,
#define O_OOO_OO_O_O 0xb,0xb5,
#define O_OOO_OO_OO_ 0xb,0xb6,
#define O_OOO_OO_OOO 0xb,0xb7,
#define O_OOO_OOO___ 0xb,0xb8,
#define O_OOO_OOO__O 0xb,0xb9,
#define O_OOO_OOO_O_ 0xb,0xba,
#define O_OOO_OOO_OO 0xb,0xbb,
#define O_OOO_OOOO__ 0xb,0xbc,
#define O_OOO_OOOO_O 0xb,0xbd,
#define O_OOO_OOOOO_ 0xb,0xbe,
#define O_OOO_OOOOOO 0xb,0xbf,
#define O_OOOO______ 0xb,0xc0,
#define O_OOOO_____O 0xb,0xc1,
#define O_OOOO____O_ 0xb,0xc2,
#define O_OOOO____OO 0xb,0xc3,
#define O_OOOO___O__ 0xb,0xc4,
#define O_OOOO___O_O 0xb,0xc5,
#define O_OOOO___OO_ 0xb,0xc6,
#define O_OOOO___OOO 0xb,0xc7,
#define O_OOOO__O___ 0xb,0xc8,
#define O_OOOO__O__O 0xb,0xc9,
#define O_OOOO__O_O_ 0xb,0xca,
#define O_OOOO__O_OO 0xb,0xcb,
#define O_OOOO__OO__ 0xb,0xcc,
#define O_OOOO__OO_O 0xb,0xcd,
#define O_OOOO__OOO_ 0xb,0xce,
#define O_OOOO__OOOO 0xb,0xcf,
#define O_OOOO_O____ 0xb,0xd0,
#define O_OOOO_O___O 0xb,0xd1,
#define O_OOOO_O__O_ 0xb,0xd2,
#define O_OOOO_O__OO 0xb,0xd3,
#define O_OOOO_O_O__ 0xb,0xd4,
#define O_OOOO_O_O_O 0xb,0xd5,
#define O_OOOO_O_OO_ 0xb,0xd6,
#define O_OOOO_O_OOO 0xb,0xd7,
#define O_OOOO_OO___ 0xb,0xd8,
#define O_OOOO_OO__O 0xb,0xd9,
#define O_OOOO_OO_O_ 0xb,0xda,
#define O_OOOO_OO_OO 0xb,0xdb,
#define O_OOOO_OOO__ 0xb,0xdc,
#define O_OOOO_OOO_O 0xb,0xdd,
#define O_OOOO_OOOO_ 0xb,0xde,
#define O_OOOO_OOOOO 0xb,0xdf,
#define O_OOOOO_____ 0xb,0xe0,
#define O_OOOOO____O 0xb,0xe1,
#define O_OOOOO___O_ 0xb,0xe2,
#define O_OOOOO___OO 0xb,0xe3,
#define O_OOOOO__O__ 0xb,0xe4,
#define O_OOOOO__O_O 0xb,0xe5,
#define O_OOOOO__OO_ 0xb,0xe6,
#define O_OOOOO__OOO 0xb,0xe7,
#define O_OOOOO_O___ 0xb,0xe8,
#define O_OOOOO_O__O 0xb,0xe9,
#define O_OOOOO_O_O_ 0xb,0xea,
#define O_OOOOO_O_OO 0xb,0xeb,
#define O_OOOOO_OO__ 0xb,0xec,
#define O_OOOOO_OO_O 0xb,0xed,
#define O_OOOOO_OOO_ 0xb,0xee,
#define O_OOOOO_OOOO 0xb,0xef,
#define O_OOOOOO____ 0xb,0xf0,
#define O_OOOOOO___O 0xb,0xf1,
#define O_OOOOOO__O_ 0xb,0xf2,
#define O_OOOOOO__OO 0xb,0xf3,
#define O_OOOOOO_O__ 0xb,0xf4,
#define O_OOOOOO_O_O 0xb,0xf5,
#define O_OOOOOO_OO_ 0xb,0xf6,
#define O_OOOOOO_OOO 0xb,0xf7,
#define O_OOOOOOO___ 0xb,0xf8,
#define O_OOOOOOO__O 0xb,0xf9,
#define O_OOOOOOO_O_ 0xb,0xfa,
#define O_OOOOOOO_OO 0xb,0xfb,
#define O_OOOOOOOO__ 0xb,0xfc,
#define O_OOOOOOOO_O 0xb,0xfd,
#define O_OOOOOOOOO_ 0xb,0xfe,
#define O_OOOOOOOOOO 0xb,0xff,
#define OO__________ 0xc,0,
#define OO_________O 0xc,0x1,
#define OO________O_ 0xc,0x2,
#define OO________OO 0xc,0x3,
#define OO_______O__ 0xc,0x4,
#define OO_______O_O 0xc,0x5,
#define OO_______OO_ 0xc,0x6,
#define OO_______OOO 0xc,0x7,
#define OO______O___ 0xc,0x8,
#define OO______O__O 0xc,0x9,
#define OO______O_O_ 0xc,0xa,
#define OO______O_OO 0xc,0xb,
#define OO______OO__ 0xc,0xc,
#define OO______OO_O 0xc,0xd,
#define OO______OOO_ 0xc,0xe,
#define OO______OOOO 0xc,0xf,
#define OO_____O____ 0xc,0x10,
#define OO_____O___O 0xc,0x11,
#define OO_____O__O_ 0xc,0x12,
#define OO_____O__OO 0xc,0x13,
#define OO_____O_O__ 0xc,0x14,
#define OO_____O_O_O 0xc,0x15,
#define OO_____O_OO_ 0xc,0x16,
#define OO_____O_OOO 0xc,0x17,
#define OO_____OO___ 0xc,0x18,
#define OO_____OO__O 0xc,0x19,
#define OO_____OO_O_ 0xc,0x1a,
#define OO_____OO_OO 0xc,0x1b,
#define OO_____OOO__ 0xc,0x1c,
#define OO_____OOO_O 0xc,0x1d,
#define OO_____OOOO_ 0xc,0x1e,
#define OO_____OOOOO 0xc,0x1f,
#define OO____O_____ 0xc,0x20,
#define OO____O____O 0xc,0x21,
#define OO____O___O_ 0xc,0x22,
#define OO____O___OO 0xc,0x23,
#define OO____O__O__ 0xc,0x24,
#define OO____O__O_O 0xc,0x25,
#define OO____O__OO_ 0xc,0x26,
#define OO____O__OOO 0xc,0x27,
#define OO____O_O___ 0xc,0x28,
#define OO____O_O__O 0xc,0x29,
#define OO____O_O_O_ 0xc,0x2a,
#define OO____O_O_OO 0xc,0x2b,
#define OO____O_OO__ 0xc,0x2c,
#define OO____O_OO_O 0xc,0x2d,
#define OO____O_OOO_ 0xc,0x2e,
#define OO____O_OOOO 0xc,0x2f,
#define OO____OO____ 0xc,0x30,
#define OO____OO___O 0xc,0x31,
#define OO____OO__O_ 0xc,0x32,
#define OO____OO__OO 0xc,0x33,
#define OO____OO_O__ 0xc,0x34,
#define OO____OO_O_O 0xc,0x35,
#define OO____OO_OO_ 0xc,0x36,
#define OO____OO_OOO 0xc,0x37,
#define OO____OOO___ 0xc,0x38,
#define OO____OOO__O 0xc,0x39,
#define OO____OOO_O_ 0xc,0x3a,
#define OO____OOO_OO 0xc,0x3b,
#define OO____OOOO__ 0xc,0x3c,
#define OO____OOOO_O 0xc,0x3d,
#define OO____OOOOO_ 0xc,0x3e,
#define OO____OOOOOO 0xc,0x3f,
#define OO___O______ 0xc,0x40,
#define OO___O_____O 0xc,0x41,
#define OO___O____O_ 0xc,0x42,
#define OO___O____OO 0xc,0x43,
#define OO___O___O__ 0xc,0x44,
#define OO___O___O_O 0xc,0x45,
#define OO___O___OO_ 0xc,0x46,
#define OO___O___OOO 0xc,0x47,
#define OO___O__O___ 0xc,0x48,
#define OO___O__O__O 0xc,0x49,
#define OO___O__O_O_ 0xc,0x4a,
#define OO___O__O_OO 0xc,0x4b,
#define OO___O__OO__ 0xc,0x4c,
#define OO___O__OO_O 0xc,0x4d,
#define OO___O__OOO_ 0xc,0x4e,
#define OO___O__OOOO 0xc,0x4f,
#define OO___O_O____ 0xc,0x50,
#define OO___O_O___O 0xc,0x51,
#define OO___O_O__O_ 0xc,0x52,
#define OO___O_O__OO 0xc,0x53,
#define OO___O_O_O__ 0xc,0x54,
#define OO___O_O_O_O 0xc,0x55,
#define OO___O_O_OO_ 0xc,0x56,
#define OO___O_O_OOO 0xc,0x57,
#define OO___O_OO___ 0xc,0x58,
#define OO___O_OO__O 0xc,0x59,
#define OO___O_OO_O_ 0xc,0x5a,
#define OO___O_OO_OO 0xc,0x5b,
#define OO___O_OOO__ 0xc,0x5c,
#define OO___O_OOO_O 0xc,0x5d,
#define OO___O_OOOO_ 0xc,0x5e,
#define OO___O_OOOOO 0xc,0x5f,
#define OO___OO_____ 0xc,0x60,
#define OO___OO____O 0xc,0x61,
#define OO___OO___O_ 0xc,0x62,
#define OO___OO___OO 0xc,0x63,
#define OO___OO__O__ 0xc,0x64,
#define OO___OO__O_O 0xc,0x65,
#define OO___OO__OO_ 0xc,0x66,
#define OO___OO__OOO 0xc,0x67,
#define OO___OO_O___ 0xc,0x68,
#define OO___OO_O__O 0xc,0x69,
#define OO___OO_O_O_ 0xc,0x6a,
#define OO___OO_O_OO 0xc,0x6b,
#define OO___OO_OO__ 0xc,0x6c,
#define OO___OO_OO_O 0xc,0x6d,
#define OO___OO_OOO_ 0xc,0x6e,
#define OO___OO_OOOO 0xc,0x6f,
#define OO___OOO____ 0xc,0x70,
#define OO___OOO___O 0xc,0x71,
#define OO___OOO__O_ 0xc,0x72,
#define OO___OOO__OO 0xc,0x73,
#define OO___OOO_O__ 0xc,0x74,
#define OO___OOO_O_O 0xc,0x75,
#define OO___OOO_OO_ 0xc,0x76,
#define OO___OOO_OOO 0xc,0x77,
#define OO___OOOO___ 0xc,0x78,
#define OO___OOOO__O 0xc,0x79,
#define OO___OOOO_O_ 0xc,0x7a,
#define OO___OOOO_OO 0xc,0x7b,
#define OO___OOOOO__ 0xc,0x7c,
#define OO___OOOOO_O 0xc,0x7d,
#define OO___OOOOOO_ 0xc,0x7e,
#define OO___OOOOOOO 0xc,0x7f,
#define OO__O_______ 0xc,0x80,
#define OO__O______O 0xc,0x81,
#define OO__O_____O_ 0xc,0x82,
#define OO__O_____OO 0xc,0x83,
#define OO__O____O__ 0xc,0x84,
#define OO__O____O_O 0xc,0x85,
#define OO__O____OO_ 0xc,0x86,
#define OO__O____OOO 0xc,0x87,
#define OO__O___O___ 0xc,0x88,
#define OO__O___O__O 0xc,0x89,
#define OO__O___O_O_ 0xc,0x8a,
#define OO__O___O_OO 0xc,0x8b,
#define OO__O___OO__ 0xc,0x8c,
#define OO__O___OO_O 0xc,0x8d,
#define OO__O___OOO_ 0xc,0x8e,
#define OO__O___OOOO 0xc,0x8f,
#define OO__O__O____ 0xc,0x90,
#define OO__O__O___O 0xc,0x91,
#define OO__O__O__O_ 0xc,0x92,
#define OO__O__O__OO 0xc,0x93,
#define OO__O__O_O__ 0xc,0x94,
#define OO__O__O_O_O 0xc,0x95,
#define OO__O__O_OO_ 0xc,0x96,
#define OO__O__O_OOO 0xc,0x97,
#define OO__O__OO___ 0xc,0x98,
#define OO__O__OO__O 0xc,0x99,
#define OO__O__OO_O_ 0xc,0x9a,
#define OO__O__OO_OO 0xc,0x9b,
#define OO__O__OOO__ 0xc,0x9c,
#define OO__O__OOO_O 0xc,0x9d,
#define OO__O__OOOO_ 0xc,0x9e,
#define OO__O__OOOOO 0xc,0x9f,
#define OO__O_O_____ 0xc,0xa0,
#define OO__O_O____O 0xc,0xa1,
#define OO__O_O___O_ 0xc,0xa2,
#define OO__O_O___OO 0xc,0xa3,
#define OO__O_O__O__ 0xc,0xa4,
#define OO__O_O__O_O 0xc,0xa5,
#define OO__O_O__OO_ 0xc,0xa6,
#define OO__O_O__OOO 0xc,0xa7,
#define OO__O_O_O___ 0xc,0xa8,
#define OO__O_O_O__O 0xc,0xa9,
#define OO__O_O_O_O_ 0xc,0xaa,
#define OO__O_O_O_OO 0xc,0xab,
#define OO__O_O_OO__ 0xc,0xac,
#define OO__O_O_OO_O 0xc,0xad,
#define OO__O_O_OOO_ 0xc,0xae,
#define OO__O_O_OOOO 0xc,0xaf,
#define OO__O_OO____ 0xc,0xb0,
#define OO__O_OO___O 0xc,0xb1,
#define OO__O_OO__O_ 0xc,0xb2,
#define OO__O_OO__OO 0xc,0xb3,
#define OO__O_OO_O__ 0xc,0xb4,
#define OO__O_OO_O_O 0xc,0xb5,
#define OO__O_OO_OO_ 0xc,0xb6,
#define OO__O_OO_OOO 0xc,0xb7,
#define OO__O_OOO___ 0xc,0xb8,
#define OO__O_OOO__O 0xc,0xb9,
#define OO__O_OOO_O_ 0xc,0xba,
#define OO__O_OOO_OO 0xc,0xbb,
#define OO__O_OOOO__ 0xc,0xbc,
#define OO__O_OOOO_O 0xc,0xbd,
#define OO__O_OOOOO_ 0xc,0xbe,
#define OO__O_OOOOOO 0xc,0xbf,
#define OO__OO______ 0xc,0xc0,
#define OO__OO_____O 0xc,0xc1,
#define OO__OO____O_ 0xc,0xc2,
#define OO__OO____OO 0xc,0xc3,
#define OO__OO___O__ 0xc,0xc4,
#define OO__OO___O_O 0xc,0xc5,
#define OO__OO___OO_ 0xc,0xc6,
#define OO__OO___OOO 0xc,0xc7,
#define OO__OO__O___ 0xc,0xc8,
#define OO__OO__O__O 0xc,0xc9,
#define OO__OO__O_O_ 0xc,0xca,
#define OO__OO__O_OO 0xc,0xcb,
#define OO__OO__OO__ 0xc,0xcc,
#define OO__OO__OO_O 0xc,0xcd,
#define OO__OO__OOO_ 0xc,0xce,
#define OO__OO__OOOO 0xc,0xcf,
#define OO__OO_O____ 0xc,0xd0,
#define OO__OO_O___O 0xc,0xd1,
#define OO__OO_O__O_ 0xc,0xd2,
#define OO__OO_O__OO 0xc,0xd3,
#define OO__OO_O_O__ 0xc,0xd4,
#define OO__OO_O_O_O 0xc,0xd5,
#define OO__OO_O_OO_ 0xc,0xd6,
#define OO__OO_O_OOO 0xc,0xd7,
#define OO__OO_OO___ 0xc,0xd8,
#define OO__OO_OO__O 0xc,0xd9,
#define OO__OO_OO_O_ 0xc,0xda,
#define OO__OO_OO_OO 0xc,0xdb,
#define OO__OO_OOO__ 0xc,0xdc,
#define OO__OO_OOO_O 0xc,0xdd,
#define OO__OO_OOOO_ 0xc,0xde,
#define OO__OO_OOOOO 0xc,0xdf,
#define OO__OOO_____ 0xc,0xe0,
#define OO__OOO____O 0xc,0xe1,
#define OO__OOO___O_ 0xc,0xe2,
#define OO__OOO___OO 0xc,0xe3,
#define OO__OOO__O__ 0xc,0xe4,
#define OO__OOO__O_O 0xc,0xe5,
#define OO__OOO__OO_ 0xc,0xe6,
#define OO__OOO__OOO 0xc,0xe7,
#define OO__OOO_O___ 0xc,0xe8,
#define OO__OOO_O__O 0xc,0xe9,
#define OO__OOO_O_O_ 0xc,0xea,
#define OO__OOO_O_OO 0xc,0xeb,
#define OO__OOO_OO__ 0xc,0xec,
#define OO__OOO_OO_O 0xc,0xed,
#define OO__OOO_OOO_ 0xc,0xee,
#define OO__OOO_OOOO 0xc,0xef,
#define OO__OOOO____ 0xc,0xf0,
#define OO__OOOO___O 0xc,0xf1,
#define OO__OOOO__O_ 0xc,0xf2,
#define OO__OOOO__OO 0xc,0xf3,
#define OO__OOOO_O__ 0xc,0xf4,
#define OO__OOOO_O_O 0xc,0xf5,
#define OO__OOOO_OO_ 0xc,0xf6,
#define OO__OOOO_OOO 0xc,0xf7,
#define OO__OOOOO___ 0xc,0xf8,
#define OO__OOOOO__O 0xc,0xf9,
#define OO__OOOOO_O_ 0xc,0xfa,
#define OO__OOOOO_OO 0xc,0xfb,
#define OO__OOOOOO__ 0xc,0xfc,
#define OO__OOOOOO_O 0xc,0xfd,
#define OO__OOOOOOO_ 0xc,0xfe,
#define OO__OOOOOOOO 0xc,0xff,
#define OO_O________ 0xd,0,
#define OO_O_______O 0xd,0x1,
#define OO_O______O_ 0xd,0x2,
#define OO_O______OO 0xd,0x3,
#define OO_O_____O__ 0xd,0x4,
#define OO_O_____O_O 0xd,0x5,
#define OO_O_____OO_ 0xd,0x6,
#define OO_O_____OOO 0xd,0x7,
#define OO_O____O___ 0xd,0x8,
#define OO_O____O__O 0xd,0x9,
#define OO_O____O_O_ 0xd,0xa,
#define OO_O____O_OO 0xd,0xb,
#define OO_O____OO__ 0xd,0xc,
#define OO_O____OO_O 0xd,0xd,
#define OO_O____OOO_ 0xd,0xe,
#define OO_O____OOOO 0xd,0xf,
#define OO_O___O____ 0xd,0x10,
#define OO_O___O___O 0xd,0x11,
#define OO_O___O__O_ 0xd,0x12,
#define OO_O___O__OO 0xd,0x13,
#define OO_O___O_O__ 0xd,0x14,
#define OO_O___O_O_O 0xd,0x15,
#define OO_O___O_OO_ 0xd,0x16,
#define OO_O___O_OOO 0xd,0x17,
#define OO_O___OO___ 0xd,0x18,
#define OO_O___OO__O 0xd,0x19,
#define OO_O___OO_O_ 0xd,0x1a,
#define OO_O___OO_OO 0xd,0x1b,
#define OO_O___OOO__ 0xd,0x1c,
#define OO_O___OOO_O 0xd,0x1d,
#define OO_O___OOOO_ 0xd,0x1e,
#define OO_O___OOOOO 0xd,0x1f,
#define OO_O__O_____ 0xd,0x20,
#define OO_O__O____O 0xd,0x21,
#define OO_O__O___O_ 0xd,0x22,
#define OO_O__O___OO 0xd,0x23,
#define OO_O__O__O__ 0xd,0x24,
#define OO_O__O__O_O 0xd,0x25,
#define OO_O__O__OO_ 0xd,0x26,
#define OO_O__O__OOO 0xd,0x27,
#define OO_O__O_O___ 0xd,0x28,
#define OO_O__O_O__O 0xd,0x29,
#define OO_O__O_O_O_ 0xd,0x2a,
#define OO_O__O_O_OO 0xd,0x2b,
#define OO_O__O_OO__ 0xd,0x2c,
#define OO_O__O_OO_O 0xd,0x2d,
#define OO_O__O_OOO_ 0xd,0x2e,
#define OO_O__O_OOOO 0xd,0x2f,
#define OO_O__OO____ 0xd,0x30,
#define OO_O__OO___O 0xd,0x31,
#define OO_O__OO__O_ 0xd,0x32,
#define OO_O__OO__OO 0xd,0x33,
#define OO_O__OO_O__ 0xd,0x34,
#define OO_O__OO_O_O 0xd,0x35,
#define OO_O__OO_OO_ 0xd,0x36,
#define OO_O__OO_OOO 0xd,0x37,
#define OO_O__OOO___ 0xd,0x38,
#define OO_O__OOO__O 0xd,0x39,
#define OO_O__OOO_O_ 0xd,0x3a,
#define OO_O__OOO_OO 0xd,0x3b,
#define OO_O__OOOO__ 0xd,0x3c,
#define OO_O__OOOO_O 0xd,0x3d,
#define OO_O__OOOOO_ 0xd,0x3e,
#define OO_O__OOOOOO 0xd,0x3f,
#define OO_O_O______ 0xd,0x40,
#define OO_O_O_____O 0xd,0x41,
#define OO_O_O____O_ 0xd,0x42,
#define OO_O_O____OO 0xd,0x43,
#define OO_O_O___O__ 0xd,0x44,
#define OO_O_O___O_O 0xd,0x45,
#define OO_O_O___OO_ 0xd,0x46,
#define OO_O_O___OOO 0xd,0x47,
#define OO_O_O__O___ 0xd,0x48,
#define OO_O_O__O__O 0xd,0x49,
#define OO_O_O__O_O_ 0xd,0x4a,
#define OO_O_O__O_OO 0xd,0x4b,
#define OO_O_O__OO__ 0xd,0x4c,
#define OO_O_O__OO_O 0xd,0x4d,
#define OO_O_O__OOO_ 0xd,0x4e,
#define OO_O_O__OOOO 0xd,0x4f,
#define OO_O_O_O____ 0xd,0x50,
#define OO_O_O_O___O 0xd,0x51,
#define OO_O_O_O__O_ 0xd,0x52,
#define OO_O_O_O__OO 0xd,0x53,
#define OO_O_O_O_O__ 0xd,0x54,
#define OO_O_O_O_O_O 0xd,0x55,
#define OO_O_O_O_OO_ 0xd,0x56,
#define OO_O_O_O_OOO 0xd,0x57,
#define OO_O_O_OO___ 0xd,0x58,
#define OO_O_O_OO__O 0xd,0x59,
#define OO_O_O_OO_O_ 0xd,0x5a,
#define OO_O_O_OO_OO 0xd,0x5b,
#define OO_O_O_OOO__ 0xd,0x5c,
#define OO_O_O_OOO_O 0xd,0x5d,
#define OO_O_O_OOOO_ 0xd,0x5e,
#define OO_O_O_OOOOO 0xd,0x5f,
#define OO_O_OO_____ 0xd,0x60,
#define OO_O_OO____O 0xd,0x61,
#define OO_O_OO___O_ 0xd,0x62,
#define OO_O_OO___OO 0xd,0x63,
#define OO_O_OO__O__ 0xd,0x64,
#define OO_O_OO__O_O 0xd,0x65,
#define OO_O_OO__OO_ 0xd,0x66,
#define OO_O_OO__OOO 0xd,0x67,
#define OO_O_OO_O___ 0xd,0x68,
#define OO_O_OO_O__O 0xd,0x69,
#define OO_O_OO_O_O_ 0xd,0x6a,
#define OO_O_OO_O_OO 0xd,0x6b,
#define OO_O_OO_OO__ 0xd,0x6c,
#define OO_O_OO_OO_O 0xd,0x6d,
#define OO_O_OO_OOO_ 0xd,0x6e,
#define OO_O_OO_OOOO 0xd,0x6f,
#define OO_O_OOO____ 0xd,0x70,
#define OO_O_OOO___O 0xd,0x71,
#define OO_O_OOO__O_ 0xd,0x72,
#define OO_O_OOO__OO 0xd,0x73,
#define OO_O_OOO_O__ 0xd,0x74,
#define OO_O_OOO_O_O 0xd,0x75,
#define OO_O_OOO_OO_ 0xd,0x76,
#define OO_O_OOO_OOO 0xd,0x77,
#define OO_O_OOOO___ 0xd,0x78,
#define OO_O_OOOO__O 0xd,0x79,
#define OO_O_OOOO_O_ 0xd,0x7a,
#define OO_O_OOOO_OO 0xd,0x7b,
#define OO_O_OOOOO__ 0xd,0x7c,
#define OO_O_OOOOO_O 0xd,0x7d,
#define OO_O_OOOOOO_ 0xd,0x7e,
#define OO_O_OOOOOOO 0xd,0x7f,
#define OO_OO_______ 0xd,0x80,
#define OO_OO______O 0xd,0x81,
#define OO_OO_____O_ 0xd,0x82,
#define OO_OO_____OO 0xd,0x83,
#define OO_OO____O__ 0xd,0x84,
#define OO_OO____O_O 0xd,0x85,
#define OO_OO____OO_ 0xd,0x86,
#define OO_OO____OOO 0xd,0x87,
#define OO_OO___O___ 0xd,0x88,
#define OO_OO___O__O 0xd,0x89,
#define OO_OO___O_O_ 0xd,0x8a,
#define OO_OO___O_OO 0xd,0x8b,
#define OO_OO___OO__ 0xd,0x8c,
#define OO_OO___OO_O 0xd,0x8d,
#define OO_OO___OOO_ 0xd,0x8e,
#define OO_OO___OOOO 0xd,0x8f,
#define OO_OO__O____ 0xd,0x90,
#define OO_OO__O___O 0xd,0x91,
#define OO_OO__O__O_ 0xd,0x92,
#define OO_OO__O__OO 0xd,0x93,
#define OO_OO__O_O__ 0xd,0x94,
#define OO_OO__O_O_O 0xd,0x95,
#define OO_OO__O_OO_ 0xd,0x96,
#define OO_OO__O_OOO 0xd,0x97,
#define OO_OO__OO___ 0xd,0x98,
#define OO_OO__OO__O 0xd,0x99,
#define OO_OO__OO_O_ 0xd,0x9a,
#define OO_OO__OO_OO 0xd,0x9b,
#define OO_OO__OOO__ 0xd,0x9c,
#define OO_OO__OOO_O 0xd,0x9d,
#define OO_OO__OOOO_ 0xd,0x9e,
#define OO_OO__OOOOO 0xd,0x9f,
#define OO_OO_O_____ 0xd,0xa0,
#define OO_OO_O____O 0xd,0xa1,
#define OO_OO_O___O_ 0xd,0xa2,
#define OO_OO_O___OO 0xd,0xa3,
#define OO_OO_O__O__ 0xd,0xa4,
#define OO_OO_O__O_O 0xd,0xa5,
#define OO_OO_O__OO_ 0xd,0xa6,
#define OO_OO_O__OOO 0xd,0xa7,
#define OO_OO_O_O___ 0xd,0xa8,
#define OO_OO_O_O__O 0xd,0xa9,
#define OO_OO_O_O_O_ 0xd,0xaa,
#define OO_OO_O_O_OO 0xd,0xab,
#define OO_OO_O_OO__ 0xd,0xac,
#define OO_OO_O_OO_O 0xd,0xad,
#define OO_OO_O_OOO_ 0xd,0xae,
#define OO_OO_O_OOOO 0xd,0xaf,
#define OO_OO_OO____ 0xd,0xb0,
#define OO_OO_OO___O 0xd,0xb1,
#define OO_OO_OO__O_ 0xd,0xb2,
#define OO_OO_OO__OO 0xd,0xb3,
#define OO_OO_OO_O__ 0xd,0xb4,
#define OO_OO_OO_O_O 0xd,0xb5,
#define OO_OO_OO_OO_ 0xd,0xb6,
#define OO_OO_OO_OOO 0xd,0xb7,
#define OO_OO_OOO___ 0xd,0xb8,
#define OO_OO_OOO__O 0xd,0xb9,
#define OO_OO_OOO_O_ 0xd,0xba,
#define OO_OO_OOO_OO 0xd,0xbb,
#define OO_OO_OOOO__ 0xd,0xbc,
#define OO_OO_OOOO_O 0xd,0xbd,
#define OO_OO_OOOOO_ 0xd,0xbe,
#define OO_OO_OOOOOO 0xd,0xbf,
#define OO_OOO______ 0xd,0xc0,
#define OO_OOO_____O 0xd,0xc1,
#define OO_OOO____O_ 0xd,0xc2,
#define OO_OOO____OO 0xd,0xc3,
#define OO_OOO___O__ 0xd,0xc4,
#define OO_OOO___O_O 0xd,0xc5,
#define OO_OOO___OO_ 0xd,0xc6,
#define OO_OOO___OOO 0xd,0xc7,
#define OO_OOO__O___ 0xd,0xc8,
#define OO_OOO__O__O 0xd,0xc9,
#define OO_OOO__O_O_ 0xd,0xca,
#define OO_OOO__O_OO 0xd,0xcb,
#define OO_OOO__OO__ 0xd,0xcc,
#define OO_OOO__OO_O 0xd,0xcd,
#define OO_OOO__OOO_ 0xd,0xce,
#define OO_OOO__OOOO 0xd,0xcf,
#define OO_OOO_O____ 0xd,0xd0,
#define OO_OOO_O___O 0xd,0xd1,
#define OO_OOO_O__O_ 0xd,0xd2,
#define OO_OOO_O__OO 0xd,0xd3,
#define OO_OOO_O_O__ 0xd,0xd4,
#define OO_OOO_O_O_O 0xd,0xd5,
#define OO_OOO_O_OO_ 0xd,0xd6,
#define OO_OOO_O_OOO 0xd,0xd7,
#define OO_OOO_OO___ 0xd,0xd8,
#define OO_OOO_OO__O 0xd,0xd9,
#define OO_OOO_OO_O_ 0xd,0xda,
#define OO_OOO_OO_OO 0xd,0xdb,
#define OO_OOO_OOO__ 0xd,0xdc,
#define OO_OOO_OOO_O 0xd,0xdd,
#define OO_OOO_OOOO_ 0xd,0xde,
#define OO_OOO_OOOOO 0xd,0xdf,
#define OO_OOOO_____ 0xd,0xe0,
#define OO_OOOO____O 0xd,0xe1,
#define OO_OOOO___O_ 0xd,0xe2,
#define OO_OOOO___OO 0xd,0xe3,
#define OO_OOOO__O__ 0xd,0xe4,
#define OO_OOOO__O_O 0xd,0xe5,
#define OO_OOOO__OO_ 0xd,0xe6,
#define OO_OOOO__OOO 0xd,0xe7,
#define OO_OOOO_O___ 0xd,0xe8,
#define OO_OOOO_O__O 0xd,0xe9,
#define OO_OOOO_O_O_ 0xd,0xea,
#define OO_OOOO_O_OO 0xd,0xeb,
#define OO_OOOO_OO__ 0xd,0xec,
#define OO_OOOO_OO_O 0xd,0xed,
#define OO_OOOO_OOO_ 0xd,0xee,
#define OO_OOOO_OOOO 0xd,0xef,
#define OO_OOOOO____ 0xd,0xf0,
#define OO_OOOOO___O 0xd,0xf1,
#define OO_OOOOO__O_ 0xd,0xf2,
#define OO_OOOOO__OO 0xd,0xf3,
#define OO_OOOOO_O__ 0xd,0xf4,
#define OO_OOOOO_O_O 0xd,0xf5,
#define OO_OOOOO_OO_ 0xd,0xf6,
#define OO_OOOOO_OOO 0xd,0xf7,
#define OO_OOOOOO___ 0xd,0xf8,
#define OO_OOOOOO__O 0xd,0xf9,
#define OO_OOOOOO_O_ 0xd,0xfa,
#define OO_OOOOOO_OO 0xd,0xfb,
#define OO_OOOOOOO__ 0xd,0xfc,
#define OO_OOOOOOO_O 0xd,0xfd,
#define OO_OOOOOOOO_ 0xd,0xfe,
#define OO_OOOOOOOOO 0xd,0xff,
#define OOO_________ 0xe,0,
#define OOO________O 0xe,0x1,
#define OOO_______O_ 0xe,0x2,
#define OOO_______OO 0xe,0x3,
#define OOO______O__ 0xe,0x4,
#define OOO______O_O 0xe,0x5,
#define OOO______OO_ 0xe,0x6,
#define OOO______OOO 0xe,0x7,
#define OOO_____O___ 0xe,0x8,
#define OOO_____O__O 0xe,0x9,
#define OOO_____O_O_ 0xe,0xa,
#define OOO_____O_OO 0xe,0xb,
#define OOO_____OO__ 0xe,0xc,
#define OOO_____OO_O 0xe,0xd,
#define OOO_____OOO_ 0xe,0xe,
#define OOO_____OOOO 0xe,0xf,
#define OOO____O____ 0xe,0x10,
#define OOO____O___O 0xe,0x11,
#define OOO____O__O_ 0xe,0x12,
#define OOO____O__OO 0xe,0x13,
#define OOO____O_O__ 0xe,0x14,
#define OOO____O_O_O 0xe,0x15,
#define OOO____O_OO_ 0xe,0x16,
#define OOO____O_OOO 0xe,0x17,
#define OOO____OO___ 0xe,0x18,
#define OOO____OO__O 0xe,0x19,
#define OOO____OO_O_ 0xe,0x1a,
#define OOO____OO_OO 0xe,0x1b,
#define OOO____OOO__ 0xe,0x1c,
#define OOO____OOO_O 0xe,0x1d,
#define OOO____OOOO_ 0xe,0x1e,
#define OOO____OOOOO 0xe,0x1f,
#define OOO___O_____ 0xe,0x20,
#define OOO___O____O 0xe,0x21,
#define OOO___O___O_ 0xe,0x22,
#define OOO___O___OO 0xe,0x23,
#define OOO___O__O__ 0xe,0x24,
#define OOO___O__O_O 0xe,0x25,
#define OOO___O__OO_ 0xe,0x26,
#define OOO___O__OOO 0xe,0x27,
#define OOO___O_O___ 0xe,0x28,
#define OOO___O_O__O 0xe,0x29,
#define OOO___O_O_O_ 0xe,0x2a,
#define OOO___O_O_OO 0xe,0x2b,
#define OOO___O_OO__ 0xe,0x2c,
#define OOO___O_OO_O 0xe,0x2d,
#define OOO___O_OOO_ 0xe,0x2e,
#define OOO___O_OOOO 0xe,0x2f,
#define OOO___OO____ 0xe,0x30,
#define OOO___OO___O 0xe,0x31,
#define OOO___OO__O_ 0xe,0x32,
#define OOO___OO__OO 0xe,0x33,
#define OOO___OO_O__ 0xe,0x34,
#define OOO___OO_O_O 0xe,0x35,
#define OOO___OO_OO_ 0xe,0x36,
#define OOO___OO_OOO 0xe,0x37,
#define OOO___OOO___ 0xe,0x38,
#define OOO___OOO__O 0xe,0x39,
#define OOO___OOO_O_ 0xe,0x3a,
#define OOO___OOO_OO 0xe,0x3b,
#define OOO___OOOO__ 0xe,0x3c,
#define OOO___OOOO_O 0xe,0x3d,
#define OOO___OOOOO_ 0xe,0x3e,
#define OOO___OOOOOO 0xe,0x3f,
#define OOO__O______ 0xe,0x40,
#define OOO__O_____O 0xe,0x41,
#define OOO__O____O_ 0xe,0x42,
#define OOO__O____OO 0xe,0x43,
#define OOO__O___O__ 0xe,0x44,
#define OOO__O___O_O 0xe,0x45,
#define OOO__O___OO_ 0xe,0x46,
#define OOO__O___OOO 0xe,0x47,
#define OOO__O__O___ 0xe,0x48,
#define OOO__O__O__O 0xe,0x49,
#define OOO__O__O_O_ 0xe,0x4a,
#define OOO__O__O_OO 0xe,0x4b,
#define OOO__O__OO__ 0xe,0x4c,
#define OOO__O__OO_O 0xe,0x4d,
#define OOO__O__OOO_ 0xe,0x4e,
#define OOO__O__OOOO 0xe,0x4f,
#define OOO__O_O____ 0xe,0x50,
#define OOO__O_O___O 0xe,0x51,
#define OOO__O_O__O_ 0xe,0x52,
#define OOO__O_O__OO 0xe,0x53,
#define OOO__O_O_O__ 0xe,0x54,
#define OOO__O_O_O_O 0xe,0x55,
#define OOO__O_O_OO_ 0xe,0x56,
#define OOO__O_O_OOO 0xe,0x57,
#define OOO__O_OO___ 0xe,0x58,
#define OOO__O_OO__O 0xe,0x59,
#define OOO__O_OO_O_ 0xe,0x5a,
#define OOO__O_OO_OO 0xe,0x5b,
#define OOO__O_OOO__ 0xe,0x5c,
#define OOO__O_OOO_O 0xe,0x5d,
#define OOO__O_OOOO_ 0xe,0x5e,
#define OOO__O_OOOOO 0xe,0x5f,
#define OOO__OO_____ 0xe,0x60,
#define OOO__OO____O 0xe,0x61,
#define OOO__OO___O_ 0xe,0x62,
#define OOO__OO___OO 0xe,0x63,
#define OOO__OO__O__ 0xe,0x64,
#define OOO__OO__O_O 0xe,0x65,
#define OOO__OO__OO_ 0xe,0x66,
#define OOO__OO__OOO 0xe,0x67,
#define OOO__OO_O___ 0xe,0x68,
#define OOO__OO_O__O 0xe,0x69,
#define OOO__OO_O_O_ 0xe,0x6a,
#define OOO__OO_O_OO 0xe,0x6b,
#define OOO__OO_OO__ 0xe,0x6c,
#define OOO__OO_OO_O 0xe,0x6d,
#define OOO__OO_OOO_ 0xe,0x6e,
#define OOO__OO_OOOO 0xe,0x6f,
#define OOO__OOO____ 0xe,0x70,
#define OOO__OOO___O 0xe,0x71,
#define OOO__OOO__O_ 0xe,0x72,
#define OOO__OOO__OO 0xe,0x73,
#define OOO__OOO_O__ 0xe,0x74,
#define OOO__OOO_O_O 0xe,0x75,
#define OOO__OOO_OO_ 0xe,0x76,
#define OOO__OOO_OOO 0xe,0x77,
#define OOO__OOOO___ 0xe,0x78,
#define OOO__OOOO__O 0xe,0x79,
#define OOO__OOOO_O_ 0xe,0x7a,
#define OOO__OOOO_OO 0xe,0x7b,
#define OOO__OOOOO__ 0xe,0x7c,
#define OOO__OOOOO_O 0xe,0x7d,
#define OOO__OOOOOO_ 0xe,0x7e,
#define OOO__OOOOOOO 0xe,0x7f,
#define OOO_O_______ 0xe,0x80,
#define OOO_O______O 0xe,0x81,
#define OOO_O_____O_ 0xe,0x82,
#define OOO_O_____OO 0xe,0x83,
#define OOO_O____O__ 0xe,0x84,
#define OOO_O____O_O 0xe,0x85,
#define OOO_O____OO_ 0xe,0x86,
#define OOO_O____OOO 0xe,0x87,
#define OOO_O___O___ 0xe,0x88,
#define OOO_O___O__O 0xe,0x89,
#define OOO_O___O_O_ 0xe,0x8a,
#define OOO_O___O_OO 0xe,0x8b,
#define OOO_O___OO__ 0xe,0x8c,
#define OOO_O___OO_O 0xe,0x8d,
#define OOO_O___OOO_ 0xe,0x8e,
#define OOO_O___OOOO 0xe,0x8f,
#define OOO_O__O____ 0xe,0x90,
#define OOO_O__O___O 0xe,0x91,
#define OOO_O__O__O_ 0xe,0x92,
#define OOO_O__O__OO 0xe,0x93,
#define OOO_O__O_O__ 0xe,0x94,
#define OOO_O__O_O_O 0xe,0x95,
#define OOO_O__O_OO_ 0xe,0x96,
#define OOO_O__O_OOO 0xe,0x97,
#define OOO_O__OO___ 0xe,0x98,
#define OOO_O__OO__O 0xe,0x99,
#define OOO_O__OO_O_ 0xe,0x9a,
#define OOO_O__OO_OO 0xe,0x9b,
#define OOO_O__OOO__ 0xe,0x9c,
#define OOO_O__OOO_O 0xe,0x9d,
#define OOO_O__OOOO_ 0xe,0x9e,
#define OOO_O__OOOOO 0xe,0x9f,
#define OOO_O_O_____ 0xe,0xa0,
#define OOO_O_O____O 0xe,0xa1,
#define OOO_O_O___O_ 0xe,0xa2,
#define OOO_O_O___OO 0xe,0xa3,
#define OOO_O_O__O__ 0xe,0xa4,
#define OOO_O_O__O_O 0xe,0xa5,
#define OOO_O_O__OO_ 0xe,0xa6,
#define OOO_O_O__OOO 0xe,0xa7,
#define OOO_O_O_O___ 0xe,0xa8,
#define OOO_O_O_O__O 0xe,0xa9,
#define OOO_O_O_O_O_ 0xe,0xaa,
#define OOO_O_O_O_OO 0xe,0xab,
#define OOO_O_O_OO__ 0xe,0xac,
#define OOO_O_O_OO_O 0xe,0xad,
#define OOO_O_O_OOO_ 0xe,0xae,
#define OOO_O_O_OOOO 0xe,0xaf,
#define OOO_O_OO____ 0xe,0xb0,
#define OOO_O_OO___O 0xe,0xb1,
#define OOO_O_OO__O_ 0xe,0xb2,
#define OOO_O_OO__OO 0xe,0xb3,
#define OOO_O_OO_O__ 0xe,0xb4,
#define OOO_O_OO_O_O 0xe,0xb5,
#define OOO_O_OO_OO_ 0xe,0xb6,
#define OOO_O_OO_OOO 0xe,0xb7,
#define OOO_O_OOO___ 0xe,0xb8,
#define OOO_O_OOO__O 0xe,0xb9,
#define OOO_O_OOO_O_ 0xe,0xba,
#define OOO_O_OOO_OO 0xe,0xbb,
#define OOO_O_OOOO__ 0xe,0xbc,
#define OOO_O_OOOO_O 0xe,0xbd,
#define OOO_O_OOOOO_ 0xe,0xbe,
#define OOO_O_OOOOOO 0xe,0xbf,
#define OOO_OO______ 0xe,0xc0,
#define OOO_OO_____O 0xe,0xc1,
#define OOO_OO____O_ 0xe,0xc2,
#define OOO_OO____OO 0xe,0xc3,
#define OOO_OO___O__ 0xe,0xc4,
#define OOO_OO___O_O 0xe,0xc5,
#define OOO_OO___OO_ 0xe,0xc6,
#define OOO_OO___OOO 0xe,0xc7,
#define OOO_OO__O___ 0xe,0xc8,
#define OOO_OO__O__O 0xe,0xc9,
#define OOO_OO__O_O_ 0xe,0xca,
#define OOO_OO__O_OO 0xe,0xcb,
#define OOO_OO__OO__ 0xe,0xcc,
#define OOO_OO__OO_O 0xe,0xcd,
#define OOO_OO__OOO_ 0xe,0xce,
#define OOO_OO__OOOO 0xe,0xcf,
#define OOO_OO_O____ 0xe,0xd0,
#define OOO_OO_O___O 0xe,0xd1,
#define OOO_OO_O__O_ 0xe,0xd2,
#define OOO_OO_O__OO 0xe,0xd3,
#define OOO_OO_O_O__ 0xe,0xd4,
#define OOO_OO_O_O_O 0xe,0xd5,
#define OOO_OO_O_OO_ 0xe,0xd6,
#define OOO_OO_O_OOO 0xe,0xd7,
#define OOO_OO_OO___ 0xe,0xd8,
#define OOO_OO_OO__O 0xe,0xd9,
#define OOO_OO_OO_O_ 0xe,0xda,
#define OOO_OO_OO_OO 0xe,0xdb,
#define OOO_OO_OOO__ 0xe,0xdc,
#define OOO_OO_OOO_O 0xe,0xdd,
#define OOO_OO_OOOO_ 0xe,0xde,
#define OOO_OO_OOOOO 0xe,0xdf,
#define OOO_OOO_____ 0xe,0xe0,
#define OOO_OOO____O 0xe,0xe1,
#define OOO_OOO___O_ 0xe,0xe2,
#define OOO_OOO___OO 0xe,0xe3,
#define OOO_OOO__O__ 0xe,0xe4,
#define OOO_OOO__O_O 0xe,0xe5,
#define OOO_OOO__OO_ 0xe,0xe6,
#define OOO_OOO__OOO 0xe,0xe7,
#define OOO_OOO_O___ 0xe,0xe8,
#define OOO_OOO_O__O 0xe,0xe9,
#define OOO_OOO_O_O_ 0xe,0xea,
#define OOO_OOO_O_OO 0xe,0xeb,
#define OOO_OOO_OO__ 0xe,0xec,
#define OOO_OOO_OO_O 0xe,0xed,
#define OOO_OOO_OOO_ 0xe,0xee,
#define OOO_OOO_OOOO 0xe,0xef,
#define OOO_OOOO____ 0xe,0xf0,
#define OOO_OOOO___O 0xe,0xf1,
#define OOO_OOOO__O_ 0xe,0xf2,
#define OOO_OOOO__OO 0xe,0xf3,
#define OOO_OOOO_O__ 0xe,0xf4,
#define OOO_OOOO_O_O 0xe,0xf5,
#define OOO_OOOO_OO_ 0xe,0xf6,
#define OOO_OOOO_OOO 0xe,0xf7,
#define OOO_OOOOO___ 0xe,0xf8,
#define OOO_OOOOO__O 0xe,0xf9,
#define OOO_OOOOO_O_ 0xe,0xfa,
#define OOO_OOOOO_OO 0xe,0xfb,
#define OOO_OOOOOO__ 0xe,0xfc,
#define OOO_OOOOOO_O 0xe,0xfd,
#define OOO_OOOOOOO_ 0xe,0xfe,
#define OOO_OOOOOOOO 0xe,0xff,
#define OOOO________ 0xf,0,
#define OOOO_______O 0xf,0x1,
#define OOOO______O_ 0xf,0x2,
#define OOOO______OO 0xf,0x3,
#define OOOO_____O__ 0xf,0x4,
#define OOOO_____O_O 0xf,0x5,
#define OOOO_____OO_ 0xf,0x6,
#define OOOO_____OOO 0xf,0x7,
#define OOOO____O___ 0xf,0x8,
#define OOOO____O__O 0xf,0x9,
#define OOOO____O_O_ 0xf,0xa,
#define OOOO____O_OO 0xf,0xb,
#define OOOO____OO__ 0xf,0xc,
#define OOOO____OO_O 0xf,0xd,
#define OOOO____OOO_ 0xf,0xe,
#define OOOO____OOOO 0xf,0xf,
#define OOOO___O____ 0xf,0x10,
#define OOOO___O___O 0xf,0x11,
#define OOOO___O__O_ 0xf,0x12,
#define OOOO___O__OO 0xf,0x13,
#define OOOO___O_O__ 0xf,0x14,
#define OOOO___O_O_O 0xf,0x15,
#define OOOO___O_OO_ 0xf,0x16,
#define OOOO___O_OOO 0xf,0x17,
#define OOOO___OO___ 0xf,0x18,
#define OOOO___OO__O 0xf,0x19,
#define OOOO___OO_O_ 0xf,0x1a,
#define OOOO___OO_OO 0xf,0x1b,
#define OOOO___OOO__ 0xf,0x1c,
#define OOOO___OOO_O 0xf,0x1d,
#define OOOO___OOOO_ 0xf,0x1e,
#define OOOO___OOOOO 0xf,0x1f,
#define OOOO__O_____ 0xf,0x20,
#define OOOO__O____O 0xf,0x21,
#define OOOO__O___O_ 0xf,0x22,
#define OOOO__O___OO 0xf,0x23,
#define OOOO__O__O__ 0xf,0x24,
#define OOOO__O__O_O 0xf,0x25,
#define OOOO__O__OO_ 0xf,0x26,
#define OOOO__O__OOO 0xf,0x27,
#define OOOO__O_O___ 0xf,0x28,
#define OOOO__O_O__O 0xf,0x29,
#define OOOO__O_O_O_ 0xf,0x2a,
#define OOOO__O_O_OO 0xf,0x2b,
#define OOOO__O_OO__ 0xf,0x2c,
#define OOOO__O_OO_O 0xf,0x2d,
#define OOOO__O_OOO_ 0xf,0x2e,
#define OOOO__O_OOOO 0xf,0x2f,
#define OOOO__OO____ 0xf,0x30,
#define OOOO__OO___O 0xf,0x31,
#define OOOO__OO__O_ 0xf,0x32,
#define OOOO__OO__OO 0xf,0x33,
#define OOOO__OO_O__ 0xf,0x34,
#define OOOO__OO_O_O 0xf,0x35,
#define OOOO__OO_OO_ 0xf,0x36,
#define OOOO__OO_OOO 0xf,0x37,
#define OOOO__OOO___ 0xf,0x38,
#define OOOO__OOO__O 0xf,0x39,
#define OOOO__OOO_O_ 0xf,0x3a,
#define OOOO__OOO_OO 0xf,0x3b,
#define OOOO__OOOO__ 0xf,0x3c,
#define OOOO__OOOO_O 0xf,0x3d,
#define OOOO__OOOOO_ 0xf,0x3e,
#define OOOO__OOOOOO 0xf,0x3f,
#define OOOO_O______ 0xf,0x40,
#define OOOO_O_____O 0xf,0x41,
#define OOOO_O____O_ 0xf,0x42,
#define OOOO_O____OO 0xf,0x43,
#define OOOO_O___O__ 0xf,0x44,
#define OOOO_O___O_O 0xf,0x45,
#define OOOO_O___OO_ 0xf,0x46,
#define OOOO_O___OOO 0xf,0x47,
#define OOOO_O__O___ 0xf,0x48,
#define OOOO_O__O__O 0xf,0x49,
#define OOOO_O__O_O_ 0xf,0x4a,
#define OOOO_O__O_OO 0xf,0x4b,
#define OOOO_O__OO__ 0xf,0x4c,
#define OOOO_O__OO_O 0xf,0x4d,
#define OOOO_O__OOO_ 0xf,0x4e,
#define OOOO_O__OOOO 0xf,0x4f,
#define OOOO_O_O____ 0xf,0x50,
#define OOOO_O_O___O 0xf,0x51,
#define OOOO_O_O__O_ 0xf,0x52,
#define OOOO_O_O__OO 0xf,0x53,
#define OOOO_O_O_O__ 0xf,0x54,
#define OOOO_O_O_O_O 0xf,0x55,
#define OOOO_O_O_OO_ 0xf,0x56,
#define OOOO_O_O_OOO 0xf,0x57,
#define OOOO_O_OO___ 0xf,0x58,
#define OOOO_O_OO__O 0xf,0x59,
#define OOOO_O_OO_O_ 0xf,0x5a,
#define OOOO_O_OO_OO 0xf,0x5b,
#define OOOO_O_OOO__ 0xf,0x5c,
#define OOOO_O_OOO_O 0xf,0x5d,
#define OOOO_O_OOOO_ 0xf,0x5e,
#define OOOO_O_OOOOO 0xf,0x5f,
#define OOOO_OO_____ 0xf,0x60,
#define OOOO_OO____O 0xf,0x61,
#define OOOO_OO___O_ 0xf,0x62,
#define OOOO_OO___OO 0xf,0x63,
#define OOOO_OO__O__ 0xf,0x64,
#define OOOO_OO__O_O 0xf,0x65,
#define OOOO_OO__OO_ 0xf,0x66,
#define OOOO_OO__OOO 0xf,0x67,
#define OOOO_OO_O___ 0xf,0x68,
#define OOOO_OO_O__O 0xf,0x69,
#define OOOO_OO_O_O_ 0xf,0x6a,
#define OOOO_OO_O_OO 0xf,0x6b,
#define OOOO_OO_OO__ 0xf,0x6c,
#define OOOO_OO_OO_O 0xf,0x6d,
#define OOOO_OO_OOO_ 0xf,0x6e,
#define OOOO_OO_OOOO 0xf,0x6f,
#define OOOO_OOO____ 0xf,0x70,
#define OOOO_OOO___O 0xf,0x71,
#define OOOO_OOO__O_ 0xf,0x72,
#define OOOO_OOO__OO 0xf,0x73,
#define OOOO_OOO_O__ 0xf,0x74,
#define OOOO_OOO_O_O 0xf,0x75,
#define OOOO_OOO_OO_ 0xf,0x76,
#define OOOO_OOO_OOO 0xf,0x77,
#define OOOO_OOOO___ 0xf,0x78,
#define OOOO_OOOO__O 0xf,0x79,
#define OOOO_OOOO_O_ 0xf,0x7a,
#define OOOO_OOOO_OO 0xf,0x7b,
#define OOOO_OOOOO__ 0xf,0x7c,
#define OOOO_OOOOO_O 0xf,0x7d,
#define OOOO_OOOOOO_ 0xf,0x7e,
#define OOOO_OOOOOOO 0xf,0x7f,
#define OOOOO_______ 0xf,0x80,
#define OOOOO______O 0xf,0x81,
#define OOOOO_____O_ 0xf,0x82,
#define OOOOO_____OO 0xf,0x83,
#define OOOOO____O__ 0xf,0x84,
#define OOOOO____O_O 0xf,0x85,
#define OOOOO____OO_ 0xf,0x86,
#define OOOOO____OOO 0xf,0x87,
#define OOOOO___O___ 0xf,0x88,
#define OOOOO___O__O 0xf,0x89,
#define OOOOO___O_O_ 0xf,0x8a,
#define OOOOO___O_OO 0xf,0x8b,
#define OOOOO___OO__ 0xf,0x8c,
#define OOOOO___OO_O 0xf,0x8d,
#define OOOOO___OOO_ 0xf,0x8e,
#define OOOOO___OOOO 0xf,0x8f,
#define OOOOO__O____ 0xf,0x90,
#define OOOOO__O___O 0xf,0x91,
#define OOOOO__O__O_ 0xf,0x92,
#define OOOOO__O__OO 0xf,0x93,
#define OOOOO__O_O__ 0xf,0x94,
#define OOOOO__O_O_O 0xf,0x95,
#define OOOOO__O_OO_ 0xf,0x96,
#define OOOOO__O_OOO 0xf,0x97,
#define OOOOO__OO___ 0xf,0x98,
#define OOOOO__OO__O 0xf,0x99,
#define OOOOO__OO_O_ 0xf,0x9a,
#define OOOOO__OO_OO 0xf,0x9b,
#define OOOOO__OOO__ 0xf,0x9c,
#define OOOOO__OOO_O 0xf,0x9d,
#define OOOOO__OOOO_ 0xf,0x9e,
#define OOOOO__OOOOO 0xf,0x9f,
#define OOOOO_O_____ 0xf,0xa0,
#define OOOOO_O____O 0xf,0xa1,
#define OOOOO_O___O_ 0xf,0xa2,
#define OOOOO_O___OO 0xf,0xa3,
#define OOOOO_O__O__ 0xf,0xa4,
#define OOOOO_O__O_O 0xf,0xa5,
#define OOOOO_O__OO_ 0xf,0xa6,
#define OOOOO_O__OOO 0xf,0xa7,
#define OOOOO_O_O___ 0xf,0xa8,
#define OOOOO_O_O__O 0xf,0xa9,
#define OOOOO_O_O_O_ 0xf,0xaa,
#define OOOOO_O_O_OO 0xf,0xab,
#define OOOOO_O_OO__ 0xf,0xac,
#define OOOOO_O_OO_O 0xf,0xad,
#define OOOOO_O_OOO_ 0xf,0xae,
#define OOOOO_O_OOOO 0xf,0xaf,
#define OOOOO_OO____ 0xf,0xb0,
#define OOOOO_OO___O 0xf,0xb1,
#define OOOOO_OO__O_ 0xf,0xb2,
#define OOOOO_OO__OO 0xf,0xb3,
#define OOOOO_OO_O__ 0xf,0xb4,
#define OOOOO_OO_O_O 0xf,0xb5,
#define OOOOO_OO_OO_ 0xf,0xb6,
#define OOOOO_OO_OOO 0xf,0xb7,
#define OOOOO_OOO___ 0xf,0xb8,
#define OOOOO_OOO__O 0xf,0xb9,
#define OOOOO_OOO_O_ 0xf,0xba,
#define OOOOO_OOO_OO 0xf,0xbb,
#define OOOOO_OOOO__ 0xf,0xbc,
#define OOOOO_OOOO_O 0xf,0xbd,
#define OOOOO_OOOOO_ 0xf,0xbe,
#define OOOOO_OOOOOO 0xf,0xbf,
#define OOOOOO______ 0xf,0xc0,
#define OOOOOO_____O 0xf,0xc1,
#define OOOOOO____O_ 0xf,0xc2,
#define OOOOOO____OO 0xf,0xc3,
#define OOOOOO___O__ 0xf,0xc4,
#define OOOOOO___O_O 0xf,0xc5,
#define OOOOOO___OO_ 0xf,0xc6,
#define OOOOOO___OOO 0xf,0xc7,
#define OOOOOO__O___ 0xf,0xc8,
#define OOOOOO__O__O 0xf,0xc9,
#define OOOOOO__O_O_ 0xf,0xca,
#define OOOOOO__O_OO 0xf,0xcb,
#define OOOOOO__OO__ 0xf,0xcc,
#define OOOOOO__OO_O 0xf,0xcd,
#define OOOOOO__OOO_ 0xf,0xce,
#define OOOOOO__OOOO 0xf,0xcf,
#define OOOOOO_O____ 0xf,0xd0,
#define OOOOOO_O___O 0xf,0xd1,
#define OOOOOO_O__O_ 0xf,0xd2,
#define OOOOOO_O__OO 0xf,0xd3,
#define OOOOOO_O_O__ 0xf,0xd4,
#define OOOOOO_O_O_O 0xf,0xd5,
#define OOOOOO_O_OO_ 0xf,0xd6,
#define OOOOOO_O_OOO 0xf,0xd7,
#define OOOOOO_OO___ 0xf,0xd8,
#define OOOOOO_OO__O 0xf,0xd9,
#define OOOOOO_OO_O_ 0xf,0xda,
#define OOOOOO_OO_OO 0xf,0xdb,
#define OOOOOO_OOO__ 0xf,0xdc,
#define OOOOOO_OOO_O 0xf,0xdd,
#define OOOOOO_OOOO_ 0xf,0xde,
#define OOOOOO_OOOOO 0xf,0xdf,
#define OOOOOOO_____ 0xf,0xe0,
#define OOOOOOO____O 0xf,0xe1,
#define OOOOOOO___O_ 0xf,0xe2,
#define OOOOOOO___OO 0xf,0xe3,
#define OOOOOOO__O__ 0xf,0xe4,
#define OOOOOOO__O_O 0xf,0xe5,
#define OOOOOOO__OO_ 0xf,0xe6,
#define OOOOOOO__OOO 0xf,0xe7,
#define OOOOOOO_O___ 0xf,0xe8,
#define OOOOOOO_O__O 0xf,0xe9,
#define OOOOOOO_O_O_ 0xf,0xea,
#define OOOOOOO_O_OO 0xf,0xeb,
#define OOOOOOO_OO__ 0xf,0xec,
#define OOOOOOO_OO_O 0xf,0xed,
#define OOOOOOO_OOO_ 0xf,0xee,
#define OOOOOOO_OOOO 0xf,0xef,
#define OOOOOOOO____ 0xf,0xf0,
#define OOOOOOOO___O 0xf,0xf1,
#define OOOOOOOO__O_ 0xf,0xf2,
#define OOOOOOOO__OO 0xf,0xf3,
#define OOOOOOOO_O__ 0xf,0xf4,
#define OOOOOOOO_O_O 0xf,0xf5,
#define OOOOOOOO_OO_ 0xf,0xf6,
#define OOOOOOOO_OOO 0xf,0xf7,
#define OOOOOOOOO___ 0xf,0xf8,
#define OOOOOOOOO__O 0xf,0xf9,
#define OOOOOOOOO_O_ 0xf,0xfa,
#define OOOOOOOOO_OO 0xf,0xfb,
#define OOOOOOOOOO__ 0xf,0xfc,
#define OOOOOOOOOO_O 0xf,0xfd,
#define OOOOOOOOOOO_ 0xf,0xfe,
#define OOOOOOOOOOOO 0xf,0xff,

const uint8_t font12x18_data[95][36] = {
    // ASCII ' ' (0x20)
    [0] = {
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
    },
    // ASCII '!' (0x21)
    [1] = {
        ____________
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        ____________
        ____________
        _____OOO____
        _____OOO____
        _____OOO____
        ____________
        ____________
    },
    // ASCII '"' (0x22)
    [2] = {
        ____________
        __OOO___OOO_
        __OOO___OOO_
        __OOO___OOO_
        __OOO___OOO_
        __OOO___OOO_
        __OOO___OOO_
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
    },
    // ASCII '#' (0x23)
    [3] = {
        ____________
        _____OO__OO_
        ____OOO__OO_
        ____OO__OO__
        ____OO__OO__
        _OOOOOOOOOOO
        _OOOOOOOOOOO
        ___OO__OO___
        ___OO__OO___
        ___OO__OO___
        OOOOOOOOOOO_
        OOOOOOOOOOO_
        __OO__OO____
        __OO__OO____
        _OOO__OO____
        _OO__OOO____
        ____________
        ____________
    },
    // ASCII '$' (0x24)
    [4] = {
        ____________
        _____O______
        ___OOOOO____
        __OOOOOOO___
        _OOO_O_OO___
        _OOO_O__O___
        _OOO_O______
        _OOOOO______
        __OOOOOO____
        ___OOOOOO___
        _____OOOOO__
        _____O_OOO__
        _O___O_OOO__
        _OO__O_OOO__
        _OOOOOOOO___
        __OOOOOO____
        _____O______
        ____________
    },
    // ASCII '%' (0x25)
    [5] = {
        ____________
        _OOOO_______
        _OOOO_______
        OO__OO______
        OO__OO______
        OO__OO______
        _OOOO_____O_
        _OOOO__OOO__
        ____OOO_____
        __OOO__OOOO_
        _O_____OOOO_
        ______OO__OO
        ______OO__OO
        ______OO__OO
        _______OOOO_
        _______OOOO_
        ____________
        ____________
    },
    // ASCII '&' (0x26)
    [6] = {
        ____________
        _____OOOO___
        ___OOOOOOO__
        ___OOO___O__
        ___OOO______
        ___OOOO_____
        ____OOO_____
        ___OOOOO____
        __OOOOOO___O
        _OOO__OOO__O
        _OOO___OOO_O
        _OOO___OOOOO
        _OOO____OOOO
        __OOO___OOOO
        __OOOOOOOOOO
        ____OOOOO_OO
        ____________
        ____________
    },
    // ASCII ''' (0x27)
    [7] = {
        ____________
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
    },
    // ASCII '(' (0x28)
    [8] = {
        ____________
        _______OO___
        ______OO____
        ______OO____
        _____OOO____
        _____OOO____
        _____OO_____
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____OOO_____
        _____OO_____
        _____OOO____
        _____OOO____
        ______OO____
        ______OO____
        _______OO___
    },
    // ASCII ')' (0x29)
    [9] = {
        ____________
        ____OO______
        _____OO_____
        _____OO_____
        _____OOO____
        _____OOO____
        ______OO____
        ______OOO___
        ______OOO___
        ______OOO___
        ______OOO___
        ______OOO___
        ______OO____
        _____OOO____
        _____OOO____
        _____OO_____
        _____OO_____
        ____OO______
    },
    // ASCII '*' (0x2a)
    [10] = {
        ____________
        _____OO_____
        _____OO_____
        _OO__OO__OO_
        _OOOOOOOOOO_
        ___OOOOOO___
        ___OOOOOO___
        _OOOOOOOOOO_
        _OO__OO__OO_
        _____OO_____
        _____OO_____
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
    },
    // ASCII '+' (0x2b)
    [11] = {
        ____________
        ____________
        ____________
        ____________
        ____________
        _____OO_____
        _____OO_____
        _____OO_____
        _____OO_____
        _OOOOOOOOOO_
        _OOOOOOOOOO_
        _____OO_____
        _____OO_____
        _____OO_____
        _____OO_____
        ____________
        ____________
        ____________
    },
    // ASCII ',' (0x2c)
    [12] = {
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OO_____
        ____OOO_____
        ____OO______
    },
    // ASCII '-' (0x2d)
    [13] = {
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ___OOOOOO___
        ___OOOOOO___
        ___OOOOOO___
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
    },
    // ASCII '.' (0x2e)
    [14] = {
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____________
        ____________
    },
    // ASCII '/' (0x2f)
    [15] = {
        ____________
        _________OO_
        ________OO__
        ________OO__
        _______OO___
        _______OO___
        ______OO____
        ______OO____
        _____OO_____
        _____OO_____
        _____OO_____
        ____OO______
        ____OO______
        ___OO_______
        ___OO_______
        __OO________
        __OO________
        _OO_________
    },
    // ASCII '0' (0x30)
    [16] = {
        ____________
        ____OOOO____
        ___OOOOOO___
        __OOO__OOO__
        __OO____OO__
        _OOO____OOO_
        _OOO____OOO_
        _OOO_OO_OOO_
        _OOO_OO_OOO_
        _OOO_OO_OOO_
        _OOO____OOO_
        _OOO____OOO_
        __OO____OO__
        __OOO__OOO__
        ___OOOOOO___
        ____OOOO____
        ____________
        ____________
    },
    // ASCII '1' (0x31)
    [17] = {
        ____________
        ____OOOO____
        __OOOOOO____
        __OO_OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        __OOOOOOOOO_
        __OOOOOOOOO_
        ____________
        ____________
    },
    // ASCII '2' (0x32)
    [18] = {
        ____________
        __OOOOOO____
        _OOOOOOOOO__
        _OO____OOOO_
        _O______OOO_
        ________OOO_
        ________OOO_
        _______OOO__
        ______OOO___
        _____OOOO___
        ____OOOO____
        ___OOOO_____
        __OOOO______
        __OOO_______
        _OOOOOOOOOO_
        _OOOOOOOOOO_
        ____________
        ____________
    },
    // ASCII '3' (0x33)
    [19] = {
        ____________
        __OOOOOOO___
        _OOOOOOOOO__
        _OO_____OOO_
        _O______OOO_
        ________OOO_
        ____OOOOO___
        ____OOOOO___
        _______OOO__
        ________OOO_
        ________OOO_
        ________OOO_
        _O______OOO_
        _OO____OOOO_
        _OOOOOOOOO__
        __OOOOOO____
        ____________
        ____________
    },
    // ASCII '4' (0x34)
    [20] = {
        ____________
        ______OOO___
        _____OOOO___
        _____OOOO___
        ____OOOOO___
        ___OOOOOO___
        ___OO_OOO___
        __OO__OOO___
        __OO__OOO___
        _OO___OOO___
        _OOOOOOOOOO_
        _OOOOOOOOOO_
        ______OOO___
        ______OOO___
        ______OOO___
        ______OOO___
        ____________
        ____________
    },
    // ASCII '5' (0x35)
    [21] = {
        ____________
        _OOOOOOOOO__
        _OOOOOOOOO__
        _OOO________
        _OOO________
        _OOO________
        _OOOOOOO____
        _OOOOOOOOO__
        _O_____OOO__
        ________OOO_
        ________OOO_
        ________OOO_
        ________OOO_
        _OO____OOO__
        _OOOOOOOOO__
        __OOOOOO____
        ____________
        ____________
    },
    // ASCII '6' (0x36)
    [22] = {
        ____________
        ____OOOOO___
        ___OOOOOOO__
        __OOO____O__
        __OOO_______
        _OOO________
        _OOO_OOOO___
        _OOOOOOOOO__
        _OOOO__OOOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        __OOO__OOO__
        __OOOOOOOO__
        ____OOOOO___
        ____________
        ____________
    },
    // ASCII '7' (0x37)
    [23] = {
        ____________
        _OOOOOOOOOO_
        _OOOOOOOOOO_
        ________OOO_
        _______OOO__
        _______OOO__
        ______OOO___
        ______OOO___
        _____OOOO___
        _____OOO____
        ____OOOO____
        ____OOO_____
        ____OOO_____
        ___OOO______
        ___OOO______
        __OOO_______
        ____________
        ____________
    },
    // ASCII '8' (0x38)
    [24] = {
        ____________
        ___OOOOOO___
        __OOOOOOOO__
        _OOOO__OOOO_
        _OOO____OOO_
        _OOO____OOO_
        __OOO__OOO__
        ___OOOOOO___
        ___OOOOOO___
        __OOO__OOO__
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOOO__OOOO_
        __OOOOOOOO__
        ___OOOOOO___
        ____________
        ____________
    },
    // ASCII '9' (0x39)
    [25] = {
        ____________
        ___OOOOO____
        __OOOOOOOO__
        _OOOO__OOO__
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOOO__OOOO_
        __OOOOOOOOO_
        ___OOOO_OOO_
        ________OOO_
        _______OOO__
        __O____OOO__
        __OOOOOOO___
        ___OOOOO____
        ____________
        ____________
    },
    // ASCII ':' (0x3a)
    [26] = {
        ____________
        ____________
        ____________
        ____________
        ____________
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____________
        ____________
        ____________
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____________
        ____________
    },
    // ASCII ';' (0x3b)
    [27] = {
        ____________
        ____________
        ____________
        ____________
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____________
        ____________
        ____________
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____OO______
        ___OOO______
        ___OO_______
    },
    // ASCII '<' (0x3c)
    [28] = {
        ____________
        ____________
        ____________
        ____________
        __________O_
        _______OOOO_
        _____OOOOOO_
        ___OOOOOO___
        _OOOOO______
        _OOO________
        _OOOOO______
        ___OOOOOO___
        _____OOOOOO_
        _______OOOO_
        __________O_
        ____________
        ____________
        ____________
    },
    // ASCII '=' (0x3d)
    [29] = {
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        _OOOOOOOOOO_
        _OOOOOOOOOO_
        ____________
        ____________
        _OOOOOOOOOO_
        _OOOOOOOOOO_
        ____________
        ____________
        ____________
        ____________
        ____________
    },
    // ASCII '>' (0x3e)
    [30] = {
        ____________
        ____________
        ____________
        ____________
        _O__________
        _OOOO_______
        _OOOOOO_____
        ___OOOOOO___
        ______OOOOO_
        ________OOO_
        ______OOOOO_
        ___OOOOOO___
        _OOOOOO_____
        _OOOO_______
        _O__________
        ____________
        ____________
        ____________
    },
    // ASCII '?' (0x3f)
    [31] = {
        ____________
        ___OOOOO____
        __OOOOOOOO__
        __O____OOO__
        _______OOO__
        _______OOO__
        ______OOO___
        _____OOO____
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____________
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____________
        ____________
    },
    // ASCII '@' (0x40)
    [32] = {
        ____________
        ____OOOOO___
        ___OOOOOOO__
        __OOO___OOO_
        _OOO_____OO_
        _OO___OOOOO_
        OO___OOOOOO_
        OO__OOO_OOO_
        OO__OO___OO_
        OO__OO___OO_
        OO__OO___OO_
        OO__OOO_OOO_
        OOO__OOOOOO_
        _OO___OOOOO_
        _OOO________
        __OOO____O__
        ___OOOOOOOO_
        _____OOOOO__
    },
    // ASCII 'A' (0x41)
    [33] = {
        ____________
        ____OOOO____
        ____OOOO____
        ____OOOO____
        ____OOOO____
        ___OOOOOO___
        ___OOOOOO___
        ___OO__OO___
        ___OO__OO___
        __OOO__OOO__
        __OOOOOOOO__
        __OOOOOOOO__
        __OOO__OOO__
        __OO____OO__
        _OOO____OOO_
        _OOO____OOO_
        ____________
        ____________
    },
    // ASCII 'B' (0x42)
    [34] = {
        ____________
        _OOOOOOO____
        _OOOOOOOO___
        _OOO___OOO__
        _OOO___OOO__
        _OOO___OOO__
        _OOO___OOO__
        _OOOOOOOO___
        _OOOOOOOO___
        _OOO___OOO__
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO___OOOO_
        _OOOOOOOOO__
        _OOOOOOOO___
        ____________
        ____________
    },
    // ASCII 'C' (0x43)
    [35] = {
        ____________
        _____OOOOO__
        ___OOOOOOOO_
        __OOOO___OO_
        __OOO_______
        _OOO________
        _OOO________
        _OOO________
        _OOO________
        _OOO________
        _OOO________
        _OOO________
        __OOO_______
        __OOOO___OO_
        ___OOOOOOOO_
        _____OOOOO__
        ____________
        ____________
    },
    // ASCII 'D' (0x44)
    [36] = {
        ____________
        _OOOOOO_____
        _OOOOOOOO___
        _OOO___OOO__
        _OOO___OOO__
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO___OOO__
        _OOO___OOO__
        _OOOOOOOO___
        _OOOOOO_____
        ____________
        ____________
    },
    // ASCII 'E' (0x45)
    [37] = {
        ____________
        _OOOOOOOOOO_
        _OOOOOOOOOO_
        _OOO________
        _OOO________
        _OOO________
        _OOO________
        _OOOOOOOOO__
        _OOOOOOOOO__
        _OOO________
        _OOO________
        _OOO________
        _OOO________
        _OOO________
        _OOOOOOOOOO_
        _OOOOOOOOOO_
        ____________
        ____________
    },
    // ASCII 'F' (0x46)
    [38] = {
        ____________
        _OOOOOOOOOO_
        _OOOOOOOOOO_
        _OOO________
        _OOO________
        _OOO________
        _OOO________
        _OOOOOOOOO__
        _OOOOOOOOO__
        _OOO________
        _OOO________
        _OOO________
        _OOO________
        _OOO________
        _OOO________
        _OOO________
        ____________
        ____________
    },
    // ASCII 'G' (0x47)
    [39] = {
        ____________
        ____OOOOO___
        ___OOOOOOO__
        __OOO____O__
        __OOO_______
        _OOO________
        _OOO________
        _OOO________
        _OOO__OOOOO_
        _OOO__OOOOO_
        _OOO____OOO_
        _OOO____OOO_
        __OOO___OOO_
        __OOO___OOO_
        ___OOOOOOOO_
        ____OOOOOO__
        ____________
        ____________
    },
    // ASCII 'H' (0x48)
    [40] = {
        ____________
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOOOOOOOOO_
        _OOOOOOOOOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        ____________
        ____________
    },
    // ASCII 'I' (0x49)
    [41] = {
        ____________
        __OOOOOOOOO_
        __OOOOOOOOO_
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        __OOOOOOOOO_
        __OOOOOOOOO_
        ____________
        ____________
    },
    // ASCII 'J' (0x4a)
    [42] = {
        ____________
        ____OOOOOOO_
        ____OOOOOOO_
        ________OOO_
        ________OOO_
        ________OOO_
        ________OOO_
        ________OOO_
        ________OOO_
        ________OOO_
        ________OOO_
        ________OOO_
        _O______OOO_
        _OO____OOOO_
        _OOOOOOOOO__
        __OOOOOOO___
        ____________
        ____________
    },
    // ASCII 'K' (0x4b)
    [43] = {
        ____________
        _OOO____OOO_
        _OOO___OOOO_
        _OOO___OOO__
        _OOO__OOO___
        _OOO_OOO____
        _OOOOOO_____
        _OOOOOO_____
        _OOOOOOO____
        _OOOOOOOO___
        _OOO__OOO___
        _OOO__OOOO__
        _OOO___OOO__
        _OOO___OOOO_
        _OOO____OOO_
        _OOO____OOOO
        ____________
        ____________
    },
    // ASCII 'L' (0x4c)
    [44] = {
        ____________
        _OOO________
        _OOO________
        _OOO________
        _OOO________
        _OOO________
        _OOO________
        _OOO________
        _OOO________
        _OOO________
        _OOO________
        _OOO________
        _OOO________
        _OOO________
        _OOOOOOOOOO_
        _OOOOOOOOOO_
        ____________
        ____________
    },
    // ASCII 'M' (0x4d)
    [45] = {
        ____________
        _OOO____OOO_
        _OOO____OOO_
        _OOOO__OOOO_
        _OOOO__OOOO_
        _OOOO__OOOO_
        _OOOOOOOOOO_
        _OOO_OOOOOO_
        _OOO_OO_OOO_
        _OOO_OO_OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        ____________
        ____________
    },
    // ASCII 'N' (0x4e)
    [46] = {
        ____________
        _OOOO___OOO_
        _OOOO___OOO_
        _OOOO___OOO_
        _OOOOO__OOO_
        _OOOOO__OOO_
        _OOOOO__OOO_
        _OOO_OO_OOO_
        _OOO_OO_OOO_
        _OOO_OO_OOO_
        _OOO__OOOOO_
        _OOO__OOOOO_
        _OOO__OOOOO_
        _OOO___OOOO_
        _OOO___OOOO_
        _OOO___OOOO_
        ____________
        ____________
    },
    // ASCII 'O' (0x4f)
    [47] = {
        ____________
        ____OOOO____
        ___OOOOOO___
        __OOO__OOO__
        __OO____OO__
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        __OO____OO__
        __OOO__OOO__
        ___OOOOOO___
        ____OOOO____
        ____________
        ____________
    },
    // ASCII 'P' (0x50)
    [48] = {
        ____________
        _OOOOOOO____
        _OOOOOOOOO__
        _OOO___OOOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO___OOOO_
        _OOOOOOOOO__
        _OOOOOOO____
        _OOO________
        _OOO________
        _OOO________
        _OOO________
        _OOO________
        ____________
        ____________
    },
    // ASCII 'Q' (0x51)
    [49] = {
        ____________
        ____OOOO____
        ___OOOOOO___
        __OOO__OOO__
        __OO____OO__
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        __OO____OOO_
        __OOO__OOO__
        ___OOOOOOO__
        ____OOOOO___
        _______OOO__
        _______OOO__
    },
    // ASCII 'R' (0x52)
    [50] = {
        ____________
        _OOOOOOOO___
        _OOOOOOOOO__
        _OOO___OOOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO___OOOO_
        _OOOOOOOOO__
        _OOOOOOOO___
        _OOO__OOOO__
        _OOO___OOO__
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO_____OOO
        ____________
        ____________
    },
    // ASCII 'S' (0x53)
    [51] = {
        ____________
        ___OOOOOO___
        __OOOOOOOO__
        _OOOO___OO__
        _OOO_____O__
        _OOO________
        _OOOOO______
        __OOOOOO____
        ___OOOOOOO__
        _____OOOOOO_
        _______OOOO_
        ________OOO_
        _O______OOO_
        _OO____OOOO_
        _OOOOOOOOO__
        __OOOOOOO___
        ____________
        ____________
    },
    // ASCII 'T' (0x54)
    [52] = {
        ____________
        _OOOOOOOOOOO
        _OOOOOOOOOOO
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        ____________
        ____________
    },
    // ASCII 'U' (0x55)
    [53] = {
        ____________
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOOO__OOOO_
        __OOOOOOOO__
        ___OOOOOO___
        ____________
        ____________
    },
    // ASCII 'V' (0x56)
    [54] = {
        ____________
        _OOO____OOO_
        _OOO____OOO_
        __OO____OO__
        __OOO__OOO__
        __OOO__OOO__
        __OOO__OOO__
        __OOO__OOO__
        ___OO__OO___
        ___OO__OO___
        ___OOOOOO___
        ___OOOOOO___
        ___OOOOOO___
        ____OOOO____
        ____OOOO____
        ____OOOO____
        ____________
        ____________
    },
    // ASCII 'W' (0x57)
    [55] = {
        ____________
        OOO______OOO
        OOO______OOO
        OOO______OOO
        OOO______OOO
        _OO__OO__OO_
        _OO__OO__OO_
        _OO_OOOO_OO_
        _OO_OOOO_OO_
        _OO_OOOO_OO_
        _OO_OOOO_OO_
        _OOOO__OOOO_
        _OOOO__OOOO_
        _OOOO__OOOO_
        __OOO__OOO__
        __OOO__OOO__
        ____________
        ____________
    },
    // ASCII 'X' (0x58)
    [56] = {
        ____________
        _OOO____OOO_
        __OOO__OOO__
        __OOO__OOO__
        ___OOOOOO___
        ___OOOOOO___
        ____OOOO____
        ____OOOO____
        _____OO_____
        ____OOOO____
        ____OOOO____
        ___OOOOOO___
        ___OOOOOO___
        __OOO__OOO__
        __OOO__OOO__
        _OOO____OOO_
        ____________
        ____________
    },
    // ASCII 'Y' (0x59)
    [57] = {
        ____________
        _OO_______OO
        _OOO_____OOO
        _OOO_____OOO
        __OOO___OOO_
        __OOOO_OOOO_
        ___OOO_OOO__
        ____OOOOO___
        ____OOOOO___
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        ____________
        ____________
    },
    // ASCII 'Z' (0x5a)
    [58] = {
        ____________
        _OOOOOOOOOO_
        _OOOOOOOOOO_
        ________OOO_
        _______OOO__
        ______OOOO__
        ______OOO___
        _____OOO____
        ____OOOO____
        ____OOO_____
        ___OOO______
        __OOOO______
        __OOO_______
        _OOO________
        _OOOOOOOOOO_
        _OOOOOOOOOO_
        ____________
        ____________
    },
    // ASCII '[' (0x5b)
    [59] = {
        ____________
        ____OOOOO___
        ____OOOOO___
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____OOOOO___
        ____OOOOO___
    },
    // ASCII '\' (0x5c)
    [60] = {
        ____________
        _OO_________
        __OO________
        __OO________
        ___OO_______
        ___OO_______
        ____OO______
        ____OO______
        ____OOO_____
        _____OO_____
        _____OOO____
        ______OO____
        ______OO____
        _______OO___
        _______OO___
        ________OO__
        ________OO__
        _________OO_
    },
    // ASCII ']' (0x5d)
    [61] = {
        ____________
        ___OOOOO____
        ___OOOOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        ___OOOOO____
        ___OOOOO____
    },
    // ASCII '^' (0x5e)
    [62] = {
        ____________
        _____OOO____
        ____OOOOO___
        ____OOOOO___
        ___OOO_OOO__
        __OOO___OOO_
        _OOO_____OOO
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
    },
    // ASCII '_' (0x5f)
    [63] = {
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        OOOOOOOOOOOO
        OOOOOOOOOOOO
    },
    // ASCII '`' (0x60)
    [64] = {
        ____________
        __OOO_______
        ___OOO______
        ____OOO_____
        _____OOO____
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
    },
    // ASCII 'a' (0x61)
    [65] = {
        ____________
        ____________
        ____________
        ____________
        ____________
        ___OOOOOO___
        __OOOOOOOO__
        __O_____OOO_
        ________OOO_
        ___OOOOOOOO_
        __OOOOOOOOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO___OOOO_
        __OOOOOOOOO_
        ___OOOO_OOO_
        ____________
        ____________
    },
    // ASCII 'b' (0x62)
    [66] = {
        ____________
        _OOO________
        _OOO________
        _OOO________
        _OOO________
        _OOO________
        _OOO_OOOO___
        _OOOOOOOOO__
        _OOOO__OOOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOOO__OOOO_
        _OOOOOOOOO__
        _OOO_OOOO___
        ____________
        ____________
    },
    // ASCII 'c' (0x63)
    [67] = {
        ____________
        ____________
        ____________
        ____________
        ____________
        ____OOOOOO__
        ___OOOOOOOO_
        __OOO_____O_
        _OOO________
        _OOO________
        _OOO________
        _OOO________
        _OOO________
        __OOO_____O_
        ___OOOOOOOO_
        ____OOOOOO__
        ____________
        ____________
    },
    // ASCII 'd' (0x64)
    [68] = {
        ____________
        ________OOO_
        ________OOO_
        ________OOO_
        ________OOO_
        ________OOO_
        ___OOOO_OOO_
        __OOOOOOOOO_
        _OOOO__OOOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOOO__OOOO_
        __OOOOOOOOO_
        ___OOOO_OOO_
        ____________
        ____________
    },
    // ASCII 'e' (0x65)
    [69] = {
        ____________
        ____________
        ____________
        ____________
        ____________
        ____OOOO____
        __OOOOOOOO__
        __OOO__OOO__
        _OOO____OOO_
        _OOOOOOOOOO_
        _OOOOOOOOOO_
        _OOO________
        _OOO________
        __OOO_____O_
        __OOOOOOOOO_
        ____OOOOOO__
        ____________
        ____________
    },
    // ASCII 'f' (0x66)
    [70] = {
        ____________
        _____OOOOO__
        ____OOOOOO__
        ____OOO_____
        ____OOO_____
        ____OOO_____
        _OOOOOOOOO__
        _OOOOOOOOO__
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____________
        ____________
    },
    // ASCII 'g' (0x67)
    [71] = {
        ____________
        ____________
        ____________
        ___OOOO_OOO_
        __OOOOOOOOO_
        __OOO__OOOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        __OOO__OOOO_
        __OOOOOOOOO_
        ___OOOO_OOO_
        ________OOO_
        __O____OOOO_
        __OOOOOOOO__
        ___OOOOOO___
    },
    // ASCII 'h' (0x68)
    [72] = {
        ____________
        _OOO________
        _OOO________
        _OOO________
        _OOO________
        _OOO________
        _OOO_OOOO___
        _OOOOOOOOO__
        _OOOO___OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        ____________
        ____________
    },
    // ASCII 'i' (0x69)
    [73] = {
        ____________
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        ____________
        __OOOOOO____
        __OOOOOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _OOOOOOOOOOO
        _OOOOOOOOOOO
        ____________
        ____________
    },
    // ASCII 'j' (0x6a)
    [74] = {
        ____________
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        ____________
        __OOOOOO____
        __OOOOOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _OOOOOO_____
        _OOOOO______
    },
    // ASCII 'k' (0x6b)
    [75] = {
        ____________
        _OOO________
        _OOO________
        _OOO________
        _OOO________
        _OOO________
        _OOO___OOO__
        _OOO__OOO___
        _OOO_OOO____
        _OOOOOO_____
        _OOOOOO_____
        _OOOOOOO____
        _OOO_OOO____
        _OOO__OOO___
        _OOO___OOO__
        _OOO____OOO_
        ____________
        ____________
    },
    // ASCII 'l' (0x6c)
    [76] = {
        ____________
        _OOOOOO_____
        _OOOOOO_____
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____OOO_____
        _____OOOOOO_
        ______OOOOO_
        ____________
        ____________
    },
    // ASCII 'm' (0x6d)
    [77] = {
        ____________
        ____________
        ____________
        ____________
        ____________
        _OOOOO_OOO__
        _OOOOOOOOOO_
        _OO__OO__OO_
        _OO__OO__OO_
        _OO__OO__OO_
        _OO__OO__OO_
        _OO__OO__OO_
        _OO__OO__OO_
        _OO__OO__OO_
        _OO__OO__OO_
        _OO__OO__OO_
        ____________
        ____________
    },
    // ASCII 'n' (0x6e)
    [78] = {
        ____________
        ____________
        ____________
        ____________
        ____________
        _OOO_OOOO___
        _OOOOOOOOO__
        _OOOO___OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        ____________
        ____________
    },
    // ASCII 'o' (0x6f)
    [79] = {
        ____________
        ____________
        ____________
        ____________
        ____________
        ____OOOO____
        __OOOOOOOO__
        __OOO__OOO__
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        __OOO__OOO__
        __OOOOOOOO__
        ____OOOO____
        ____________
        ____________
    },
    // ASCII 'p' (0x70)
    [80] = {
        ____________
        ____________
        ____________
        _OOO_OOOO___
        _OOOOOOOOO__
        _OOOO__OOOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOOO__OOOO_
        _OOOOOOOOO__
        _OOO_OOOO___
        _OOO________
        _OOO________
        _OOO________
        _OOO________
    },
    // ASCII 'q' (0x71)
    [81] = {
        ____________
        ____________
        ____________
        ___OOOO_OOO_
        __OOOOOOOOO_
        _OOOO__OOOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOOO__OOOO_
        __OOOOOOOOO_
        ___OOOO_OOO_
        ________OOO_
        ________OOO_
        ________OOO_
        ________OOO_
    },
    // ASCII 'r' (0x72)
    [82] = {
        ____________
        ____________
        ____________
        ____________
        ____________
        ___OOO_OOO__
        ___OOOOOOOO_
        ___OOOO___O_
        ___OOO______
        ___OOO______
        ___OOO______
        ___OOO______
        ___OOO______
        ___OOO______
        ___OOO______
        ___OOO______
        ____________
        ____________
    },
    // ASCII 's' (0x73)
    [83] = {
        ____________
        ____________
        ____________
        ____________
        ____________
        ___OOOOOO___
        __OOOOOOOO__
        _OOO_____O__
        _OOO________
        _OOOOOOO____
        __OOOOOOOO__
        _____OOOOOO_
        ________OOO_
        _O______OOO_
        _OOOOOOOOO__
        __OOOOOOO___
        ____________
        ____________
    },
    // ASCII 't' (0x74)
    [84] = {
        ____________
        ____________
        ____OOO_____
        ____OOO_____
        ____OOO_____
        _OOOOOOOOOO_
        _OOOOOOOOOO_
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____OOO_____
        ____OOOOOOO_
        _____OOOOOO_
        ____________
        ____________
    },
    // ASCII 'u' (0x75)
    [85] = {
        ____________
        ____________
        ____________
        ____________
        ____________
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO____OOO_
        _OOO___OOOO_
        __OOOOOOOOO_
        ___OOOO_OOO_
        ____________
        ____________
    },
    // ASCII 'v' (0x76)
    [86] = {
        ____________
        ____________
        ____________
        ____________
        ____________
        _OOO____OOO_
        _OOO____OOO_
        __OOO__OOO__
        __OOO__OOO__
        __OOO__OOO__
        ___OO__OO___
        ___OOOOOO___
        ___OOOOOO___
        ____OOOO____
        ____OOOO____
        ____OOOO____
        ____________
        ____________
    },
    // ASCII 'w' (0x77)
    [87] = {
        ____________
        ____________
        ____________
        ____________
        ____________
        OO________OO
        OOO______OOO
        OOO______OOO
        _OO__OO__OO_
        _OO_OOOO_OO_
        _OO_OOOO_OO_
        _OO_OOOO_OO_
        _OOOOOOOOOO_
        __OOO__OOO__
        __OOO__OOO__
        __OOO__OOO__
        ____________
        ____________
    },
    // ASCII 'x' (0x78)
    [88] = {
        ____________
        ____________
        ____________
        ____________
        ____________
        _OOOO__OOOO_
        __OOO__OOO__
        ___OOOOOO___
        ___OOOOOO___
        ____OOOO____
        ____OOOO____
        ____OOOO____
        ___OOOOOO___
        ___OOOOOO___
        __OOO__OOO__
        _OOOO__OOOO_
        ____________
        ____________
    },
    // ASCII 'y' (0x79)
    [89] = {
        ____________
        ____________
        ____________
        _OOO____OOO_
        __OOO___OOO_
        __OOO__OOO__
        __OOO__OOO__
        ___OOO_OOO__
        ___OOO_OO___
        ___OOOOOO___
        ____OOOOO___
        ____OOOO____
        _____OOO____
        _____OOO____
        _____OO_____
        ____OOO_____
        __OOOO______
        __OOOO______
    },
    // ASCII 'z' (0x7a)
    [90] = {
        ____________
        ____________
        ____________
        ____________
        ____________
        _OOOOOOOOOO_
        _OOOOOOOOOO_
        _______OOOO_
        ______OOOO__
        _____OOOO___
        ____OOOO____
        ___OOOO_____
        __OOOO______
        _OOOO_______
        _OOOOOOOOOO_
        _OOOOOOOOOO_
        ____________
        ____________
    },
    // ASCII '{' (0x7b)
    [91] = {
        ____________
        ______OOOOO_
        _____OOOOOO_
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        ____OOOO____
        __OOOOO_____
        ____OOOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOOOOO_
        ______OOOOO_
    },
    // ASCII '|' (0x7c)
    [92] = {
        ____________
        _____OO_____
        _____OO_____
        _____OO_____
        _____OO_____
        _____OO_____
        _____OO_____
        _____OO_____
        _____OO_____
        _____OO_____
        _____OO_____
        _____OO_____
        _____OO_____
        _____OO_____
        _____OO_____
        _____OO_____
        _____OO_____
        _____OO_____
    },
    // ASCII '}' (0x7d)
    [93] = {
        ____________
        __OOOOO_____
        __OOOOOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOOO___
        ______OOOOO_
        _____OOOO___
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        _____OOO____
        __OOOOOO____
        __OOOOO_____
    },
    // ASCII '~' (0x7e)
    [94] = {
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        __OOOO____O_
        _OOOOOOOOOO_
        _O____OOOO__
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
        ____________
    },
};

const font_t font12x18 = {
    .data   = (const uint8_t *) font12x18_data,
    .width  = 12,
    .height = 18,
    .bytes  = 2,
    .first  = 32,
    .last   = 126,
};
