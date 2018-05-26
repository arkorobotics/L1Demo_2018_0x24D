#ifndef AUDIO_H 
#define AUDIO_H 

#define AUDIO_MODE 0
#define VOICE_MODE 1

const static unsigned short sinetable[512] = {
0x7f,0x81,0x82,0x84,0x85,0x87,0x88,0x8a,0x8b,0x8d,0x8f,0x90,0x92,0x93,0x95,0x96,0x98,0x99,0x9b,0x9c,0x9e,0x9f,0xa1,0xa2,0xa4,0xa5,0xa7,0xa8,0xaa,0xab,0xad,0xae,0xb0,0xb1,0xb2,0xb4,0xb5,0xb7,0xb8,0xb9,0xbb,0xbc,0xbe,0xbf,0xc0,0xc2,0xc3,0xc4,0xc6,0xc7,0xc8,0xc9,0xcb,0xcc,0xcd,0xce,0xd0,0xd1,0xd2,0xd3,0xd4,0xd5,0xd7,0xd8,0xd9,0xda,0xdb,0xdc,0xdd,0xde,0xdf,0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xe9,0xea,0xeb,0xec,0xed,0xee,0xee,0xef,0xf0,0xf0,0xf1,0xf2,0xf2,0xf3,0xf4,0xf4,0xf5,0xf5,0xf6,0xf7,0xf7,0xf8,0xf8,0xf9,0xf9,0xf9,0xfa,0xfa,0xfb,0xfb,0xfb,0xfc,0xfc,0xfc,0xfc,0xfd,0xfd,0xfd,0xfd,0xfd,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfd,0xfd,0xfd,0xfd,0xfd,0xfc,0xfc,0xfc,0xfc,0xfb,0xfb,0xfb,0xfa,0xfa,0xf9,0xf9,0xf9,0xf8,0xf8,0xf7,0xf7,0xf6,0xf5,0xf5,0xf4,0xf4,0xf3,0xf2,0xf2,0xf1,0xf0,0xf0,0xef,0xee,0xee,0xed,0xec,0xeb,0xea,0xe9,0xe9,0xe8,0xe7,0xe6,0xe5,0xe4,0xe3,0xe2,0xe1,0xe0,0xdf,0xde,0xdd,0xdc,0xdb,0xda,0xd9,0xd8,0xd7,0xd5,0xd4,0xd3,0xd2,0xd1,0xd0,0xce,0xcd,0xcc,0xcb,0xc9,0xc8,0xc7,0xc6,0xc4,0xc3,0xc2,0xc0,0xbf,0xbe,0xbc,0xbb,0xb9,0xb8,0xb7,0xb5,0xb4,0xb2,0xb1,0xb0,0xae,0xad,0xab,0xaa,0xa8,0xa7,0xa5,0xa4,0xa2,0xa1,0x9f,0x9e,0x9c,0x9b,0x99,0x98,0x96,0x95,0x93,0x92,0x90,0x8f,0x8d,0x8b,0x8a,0x88,0x87,0x85,0x84,0x82,0x81,0x7f,0x7f,0x7d,0x7c,0x7a,0x79,0x77,0x76,0x74,0x73,0x71,0x6f,0x6e,0x6c,0x6b,0x69,0x68,0x66,0x65,0x63,0x62,0x60,0x5f,0x5d,0x5c,0x5a,0x59,0x57,0x56,0x54,0x53,0x51,0x50,0x4e,0x4d,0x4c,0x4a,0x49,0x47,0x46,0x45,0x43,0x42,0x40,0x3f,0x3e,0x3c,0x3b,0x3a,0x38,0x37,0x36,0x35,0x33,0x32,0x31,0x30,0x2e,0x2d,0x2c,0x2b,0x2a,0x29,0x27,0x26,0x25,0x24,0x23,0x22,0x21,0x20,0x1f,0x1e,0x1d,0x1c,0x1b,0x1a,0x19,0x18,0x17,0x16,0x15,0x15,0x14,0x13,0x12,0x11,0x10,0x10,0xf,0xe,0xe,0xd,0xc,0xc,0xb,0xa,0xa,0x9,0x9,0x8,0x7,0x7,0x6,0x6,0x5,0x5,0x5,0x4,0x4,0x3,0x3,0x3,0x2,0x2,0x2,0x2,0x1,0x1,0x1,0x1,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1,0x1,0x1,0x1,0x1,0x2,0x2,0x2,0x2,0x3,0x3,0x3,0x4,0x4,0x5,0x5,0x5,0x6,0x6,0x7,0x7,0x8,0x9,0x9,0xa,0xa,0xb,0xc,0xc,0xd,0xe,0xe,0xf,0x10,0x10,0x11,0x12,0x13,0x14,0x15,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x30,0x31,0x32,0x33,0x35,0x36,0x37,0x38,0x3a,0x3b,0x3c,0x3e,0x3f,0x40,0x42,0x43,0x45,0x46,0x47,0x49,0x4a,0x4c,0x4d,0x4e,0x50,0x51,0x53,0x54,0x56,0x57,0x59,0x5a,0x5c,0x5d,0x5f,0x60,0x62,0x63,0x65,0x66,0x68,0x69,0x6b,0x6c,0x6e,0x6f,0x71,0x73,0x74,0x76,0x77,0x79,0x7a,0x7c,0x7d,0x7f
};

extern volatile uint8_t audio_mode;
extern volatile uint8_t audio_track;

extern volatile uint32_t time_sec;
extern volatile uint32_t time_subsec;

extern volatile unsigned short ch1_val;
extern volatile unsigned short ch2_val;
extern volatile unsigned short ch3_val;
extern volatile unsigned short ch4_val;
extern volatile unsigned short ch5_val;

void audio_init(void);
void audio_isr(void);

void __attribute__((__interrupt__)) _T1Interrupt(void);

#endif