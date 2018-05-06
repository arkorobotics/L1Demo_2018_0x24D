#ifndef GPU_H 
#define GPU_H 

struct GFXConfig
{
   uint16_t frame_buffers;

   uint16_t clock_div;

   uint32_t hres;
   uint32_t vres;

   uint32_t bpp;

   uint32_t hfp;
   uint32_t hpw;
   uint32_t hbp;

   uint32_t vfp;
   uint32_t vpw;
   uint32_t vbp;

   uint32_t hscale;

   uint32_t buffer_size;

   uint32_t fb_offset;
};

typedef enum 
{ 
	RES_320x240,
	RES_80x480,
	RES_160x480,
	RES_320x480,
	RES_480x480,
	RES_640x480
} resolution;

typedef enum
{
	SINGLEBUFFERED = 1,
	DOUBLEBUFFERED = 2
} framebuffers;

typedef enum
{
	BPP_1 = 1,
	BPP_2 = 2,
	BPP_4 = 4,
	BPP_8 = 8,
	BPP_16 = 16
} colordepth;

extern volatile struct GFXConfig gfx;  

#define VENST_FUDGE 0
#define HENST_FUDGE 0
#define VSPOL 0              /* sync polarities */
#define HSPOL 0

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

#define GFX_MAX_BUFFER_SIZE 76800

extern uint16_t frames;

extern __eds__ uint8_t GFXDisplayBuffer[GFX_MAX_BUFFER_SIZE] __attribute__((section("DISPLAY"),space(eds)));

void __attribute__((interrupt, auto_psv))_GFX1Interrupt(void);
void gpu_init(void);
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
uint8_t gpu_set_res(resolution res, framebuffers fb, colordepth bpp);
void waitForBufferFlip(void);
uint16_t getHsync();
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
