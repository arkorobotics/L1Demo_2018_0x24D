#ifndef AUDIO_H 
#define AUDIO_H 


const static unsigned short sinetable[510] = {
0x7f,0x81,0x82,0x84,0x85,0x87,0x88,0x8a,0x8b,0x8d,0x8f,0x90,0x92,0x93,0x95,0x96,0x98,0x99,0x9b,0x9c,0x9e,0x9f,0xa1,0xa3,0xa4,0xa6,0xa7,0xa8,0xaa,0xab,0xad,0xae,0xb0,0xb1,0xb3,0xb4,0xb5,0xb7,0xb8,0xba,0xbb,0xbc,0xbe,0xbf,0xc1,0xc2,0xc3,0xc4,0xc6,0xc7,0xc8,0xca,0xcb,0xcc,0xcd,0xcf,0xd0,0xd1,0xd2,0xd3,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,0xdb,0xdc,0xdd,0xde,0xdf,0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xeb,0xeb,0xec,0xed,0xee,0xef,0xef,0xf0,0xf1,0xf1,0xf2,0xf3,0xf3,0xf4,0xf5,0xf5,0xf6,0xf6,0xf7,0xf7,0xf8,0xf8,0xf9,0xf9,0xfa,0xfa,0xfa,0xfb,0xfb,0xfb,0xfc,0xfc,0xfc,0xfc,0xfd,0xfd,0xfd,0xfd,0xfd,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfd,0xfd,0xfd,0xfd,0xfd,0xfc,0xfc,0xfc,0xfc,0xfb,0xfb,0xfb,0xfa,0xfa,0xfa,0xf9,0xf9,0xf8,0xf8,0xf7,0xf7,0xf6,0xf6,0xf5,0xf5,0xf4,0xf3,0xf3,0xf2,0xf1,0xf1,0xf0,0xef,0xef,0xee,0xed,0xec,0xeb,0xeb,0xea,0xe9,0xe8,0xe7,0xe6,0xe5,0xe4,0xe3,0xe2,0xe1,0xe0,0xdf,0xde,0xdd,0xdc,0xdb,0xda,0xd9,0xd8,0xd7,0xd6,0xd5,0xd3,0xd2,0xd1,0xd0,0xcf,0xcd,0xcc,0xcb,0xca,0xc8,0xc7,0xc6,0xc4,0xc3,0xc2,0xc1,0xbf,0xbe,0xbc,0xbb,0xba,0xb8,0xb7,0xb5,0xb4,0xb3,0xb1,0xb0,0xae,0xad,0xab,0xaa,0xa8,0xa7,0xa6,0xa4,0xa3,0xa1,0x9f,0x9e,0x9c,0x9b,0x99,0x98,0x96,0x95,0x93,0x92,0x90,0x8f,0x8d,0x8b,0x8a,0x88,0x87,0x85,0x84,0x82,0x81,0x7f,0x7d,0x7c,0x7a,0x79,0x77,0x76,0x74,0x73,0x71,0x6f,0x6e,0x6c,0x6b,0x69,0x68,0x66,0x65,0x63,0x62,0x60,0x5f,0x5d,0x5b,0x5a,0x58,0x57,0x56,0x54,0x53,0x51,0x50,0x4e,0x4d,0x4b,0x4a,0x49,0x47,0x46,0x44,0x43,0x42,0x40,0x3f,0x3d,0x3c,0x3b,0x3a,0x38,0x37,0x36,0x34,0x33,0x32,0x31,0x2f,0x2e,0x2d,0x2c,0x2b,0x29,0x28,0x27,0x26,0x25,0x24,0x23,0x22,0x21,0x20,0x1f,0x1e,0x1d,0x1c,0x1b,0x1a,0x19,0x18,0x17,0x16,0x15,0x14,0x13,0x13,0x12,0x11,0x10,0xf,0xf,0xe,0xd,0xd,0xc,0xb,0xb,0xa,0x9,0x9,0x8,0x8,0x7,0x7,0x6,0x6,0x5,0x5,0x4,0x4,0x4,0x3,0x3,0x3,0x2,0x2,0x2,0x2,0x1,0x1,0x1,0x1,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1,0x1,0x1,0x1,0x1,0x2,0x2,0x2,0x2,0x3,0x3,0x3,0x4,0x4,0x4,0x5,0x5,0x6,0x6,0x7,0x7,0x8,0x8,0x9,0x9,0xa,0xb,0xb,0xc,0xd,0xd,0xe,0xf,0xf,0x10,0x11,0x12,0x13,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2b,0x2c,0x2d,0x2e,0x2f,0x31,0x32,0x33,0x34,0x36,0x37,0x38,0x3a,0x3b,0x3c,0x3d,0x3f,0x40,0x42,0x43,0x44,0x46,0x47,0x49,0x4a,0x4b,0x4d,0x4e,0x50,0x51,0x53,0x54,0x56,0x57,0x58,0x5a,0x5b,0x5d,0x5f,0x60,0x62,0x63,0x65,0x66,0x68,0x69,0x6b,0x6c,0x6e,0x6f,0x71,0x73,0x74,0x76,0x77,0x79,0x7a,0x7c,0x7d
};

const static unsigned short sinetable_small[256] = {
0x8000,0x8327,0x864e,0x8973,0x8c98,0x8fba,
0x92da,0x95f7,0x9911,0x9c27,0x9f38,0xa244,
0xa54c,0xa84d,0xab48,0xae3c,0xb12a,0xb40f,
0xb6ed,0xb9c2,0xbc8e,0xbf50,0xc209,0xc4b7,
0xc75b,0xc9f4,0xcc81,0xcf02,0xd177,0xd3e0,
0xd63b,0xd889,0xdac9,0xdcfb,0xdf1e,0xe133,
0xe339,0xe52f,0xe716,0xe8ec,0xeab3,0xec68,
0xee0d,0xefa1,0xf123,0xf294,0xf3f3,0xf540,
0xf67a,0xf7a3,0xf8b8,0xf9bb,0xfaab,0xfb88,
0xfc52,0xfd08,0xfdab,0xfe3b,0xfeb7,0xff1f,
0xff73,0xffb4,0xffe1,0xfff9,0xfffe,0xffef,
0xffcd,0xff96,0xff4b,0xfeed,0xfe7b,0xfdf6,
0xfd5c,0xfcb0,0xfbef,0xfb1c,0xfa36,0xf93c,
0xf830,0xf711,0xf5df,0xf49b,0xf345,0xf1de,
0xf064,0xeed9,0xed3d,0xeb8f,0xe9d2,0xe803,
0xe625,0xe436,0xe238,0xe02b,0xde0f,0xdbe4,
0xd9ab,0xd763,0xd50f,0xd2ad,0xd03e,0xcdc3,
0xcb3c,0xc8a9,0xc60b,0xc361,0xc0ae,0xbdf0,
0xbb29,0xb858,0xb57f,0xb29d,0xafb4,0xacc3,
0xa9cb,0xa6cd,0xa3c9,0xa0bf,0x9db0,0x9a9c,
0x9785,0x9469,0x914b,0x8e29,0x8b06,0x87e1,
0x84ba,0x8193,0x7e6c,0x7b45,0x781e,0x74f9,
0x71d6,0x6eb4,0x6b96,0x687a,0x6563,0x624f,
0x5f40,0x5c36,0x5932,0x5634,0x533c,0x504b,
0x4d62,0x4a80,0x47a7,0x44d6,0x420f,0x3f51,
0x3c9e,0x39f4,0x3756,0x34c3,0x323c,0x2fc1,
0x2d52,0x2af0,0x289c,0x2654,0x241b,0x21f0,
0x1fd4,0x1dc7,0x1bc9,0x19da,0x17fc,0x162d,
0x1470,0x12c2,0x1126,0xf9b,0xe21,0xcba,
0xb64,0xa20,0x8ee,0x7cf,0x6c3,0x5c9,
0x4e3,0x410,0x34f,0x2a3,0x209,0x184,
0x112,0xb4,0x69,0x32,0x10,0x1,
0x6,0x1e,0x4b,0x8c,0xe0,0x148,
0x1c4,0x254,0x2f7,0x3ad,0x477,0x554,
0x644,0x747,0x85c,0x985,0xabf,0xc0c,
0xd6b,0xedc,0x105e,0x11f2,0x1397,0x154c,
0x1713,0x18e9,0x1ad0,0x1cc6,0x1ecc,0x20e1,
0x2304,0x2536,0x2776,0x29c4,0x2c1f,0x2e88,
0x30fd,0x337e,0x360b,0x38a4,0x3b48,0x3df6,
0x40af,0x4371,0x463d,0x4912,0x4bf0,0x4ed5,
0x51c3,0x54b7,0x57b2,0x5ab3,0x5dbb,0x60c7,
0x63d8,0x66ee,0x6a08,0x6d25,0x7045,0x7367,
0x768c,0x79b1,0x7cd8,0x8000
};

extern volatile uint32_t time;

void audio_init(void);
void __attribute__((__interrupt__)) _T1Interrupt(void);

#endif