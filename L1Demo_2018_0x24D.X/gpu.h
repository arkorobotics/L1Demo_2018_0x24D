#ifndef GPU_H 
#define GPU_H 

#define DOUBLE_BUFFERED 1

#define CLOCKDIV 25
#define HOR_RES 320UL
#define VER_RES 480UL
#define HOR_FRONT_PORCH 32
#define HOR_PULSE_WIDTH 48
#define HOR_BACK_PORCH  32
#define VER_FRONT_PORCH 10
#define VER_PULSE_WIDTH 5
#define VER_BACK_PORCH  10

//#ifdef DOUBLE_BUFFERED
////80x480@60: 8bpp
//#define CLOCKDIV 69
//#define HOR_RES 80UL
//#define VER_RES 480UL
//#define HOR_FRONT_PORCH 32
//#define HOR_PULSE_WIDTH 16
//#define HOR_BACK_PORCH  32
//#define VER_FRONT_PORCH 10
//#define VER_PULSE_WIDTH 5
//#define VER_BACK_PORCH  10
//#define BPP 8
//
//#else
////80x480@60: 16bpp
//#define CLOCKDIV 69
//#define HOR_RES 80UL
//#define VER_RES 480UL
//#define HOR_FRONT_PORCH 32
//#define HOR_PULSE_WIDTH 16
//#define HOR_BACK_PORCH  32
//#define VER_FRONT_PORCH 10
//#define VER_PULSE_WIDTH 5
//#define VER_BACK_PORCH  10
//#define BPP 16
//#define GFX_BUFFER_SIZE 76800 // This is only for BPP = 16 @480*80
//#endif

// 160x480@4bpp
//#define CLOCKDIV 47
//#define HOR_RES 160UL
//#define VER_RES 480UL
//#define HOR_FRONT_PORCH 32
//#define HOR_PULSE_WIDTH 24
//#define HOR_BACK_PORCH  32
//#define VER_FRONT_PORCH 10
//#define VER_PULSE_WIDTH 5
//#define VER_BACK_PORCH  10
//#define BPP 8

// 320x480@4bpp
//#define CLOCKDIV 25
//#define HOR_RES 320UL
//#define VER_RES 480UL
//#define HOR_FRONT_PORCH 32
//#define HOR_PULSE_WIDTH 48
//#define HOR_BACK_PORCH  32
//#define VER_FRONT_PORCH 10
//#define VER_PULSE_WIDTH 5
//#define VER_BACK_PORCH  10
//#define BPP 4

// 480x480@2bpp
//#define CLOCKDIV 17
//#define HOR_RES 480UL
//#define VER_RES 480UL
//#define HOR_FRONT_PORCH 32
//#define HOR_PULSE_WIDTH 64
//#define HOR_BACK_PORCH  32
//#define VER_FRONT_PORCH 10
//#define VER_PULSE_WIDTH 5
//#define VER_BACK_PORCH  10
//#define BPP 2

//640x480@60: 1bpp
//#define CLOCKDIV 11
//#define HOR_RES 640UL
//#define VER_RES 480UL
//#define HOR_FRONT_PORCH 16
//#define HOR_PULSE_WIDTH 96
//#define HOR_BACK_PORCH  48
//#define VER_FRONT_PORCH 10
//#define VER_PULSE_WIDTH 2
//#define VER_BACK_PORCH  33
//#define BPP 1


// QVGA:
//#define CLOCKDIV 58
//#define HOR_RES 320UL
//#define VER_RES 240UL
//#define HOR_FRONT_PORCH 32
//#define HOR_PULSE_WIDTH 16
//#define HOR_BACK_PORCH  32
//#define VER_FRONT_PORCH 5
//#define VER_PULSE_WIDTH 3
//#define VER_BACK_PORCH  5
//#define BPP 8

#ifdef DOUBLE_BUFFERED
#define BPP 2
#else
#define BPP 4
#endif

#define VENST_FUDGE 0
#define HENST_FUDGE 0
#define VSPOL 0              /* sync polarities */
#define HSPOL 0

#define PIX_W 1
#define PIX_H (VER_RES/HOR_RES)
 
#define CHR_FGCOLOR	     	 0x5000
#define CHR_BGCOLOR	     	 0x5100
#define CHR_FONTBASE         0x5200
#define CHR_PRINTCHAR        0x5300
#define CHR_PRINTCHARTRANS	 0x5380
#define CHR_TXTAREASTART     0x5800
#define CHR_TXTAREAEND       0x5900
#define CHR_PRINTPOS	     0x5A00
#define RCC_SRCADDR	     	 0x6200
#define RCC_DESTADDR	     0x6300
#define RCC_RECTSIZE	     0x6400
#define RCC_COLOR	     	 0x6600
#define RCC_STARTCOPY	     0x6700
#define IPU_SRCADDR	     	 0x7100
#define IPU_DESTADDR         0x7200
#define IPU_DECOMPRESS       0x7400

#define GFX_BUFFER_SIZE (HOR_RES * VER_RES / (8/BPP))

extern uint16_t frames;

#ifdef DOUBLE_BUFFERED
extern __eds__ uint8_t GFXDisplayBuffer[2][GFX_BUFFER_SIZE] __attribute__((section("DISPLAY"),space(eds)));
#else
extern __eds__ uint8_t GFXDisplayBuffer[GFX_BUFFER_SIZE] __attribute__((section("DISPLAY"),space(eds)));
#endif

void __attribute__((interrupt, auto_psv))_GFX1Interrupt(void);
void config_graphics(void);
void config_clut();
void clut_set(uint8_t index, uint16_t color);
void config_chr(void);
void chr_print(char *c, uint16_t x, uint16_t y, uint8_t transparent);
void rcc_color(unsigned int color);
void chr_fg_color(unsigned int color);
void chr_bg_color(unsigned int color);
void rcc_setdest(__eds__ uint8_t *buf);
void gpu_setfb(__eds__ uint8_t *buf);
void waitForBufferFlip(void);
int nonBlockingVsyncWait(void);
uint16_t getHsync();
void swapBuffers(void);
void rcc_draw(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void fast_pixel(unsigned long ax, unsigned long ay);
void blank_background(void);
void drawBorder(uint16_t c);
void cleanup(void);
void clearbuffers(void);
float radians(uint16_t angle);
int realtoint(float oldval, float oldmin, float oldmax, float newmin, float newmax);
void drawLineS(float x1, float y1, float x2, float y2);
void line(float x1, float y1, float x2, float y2, uint8_t color);
void render(float xa, float ya, float za);

#endif
