#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <string.h>
#include <math.h>
#include "fonts.h"
#include "gpu.h"

#define  FCY    16000000UL    // Instruction cycle frequency, Hz
#include <libpic30.h>

uint16_t frames = 0;

volatile struct GFXConfig gfx;

__eds__ uint8_t GFXDisplayBuffer[GFX_MAX_BUFFER_SIZE] __attribute__((section("DISPLAY"),space(eds)));

volatile static int fb_ready = 0;
volatile static int hline = 0;
volatile static int next_fb = 0;

void __attribute__((interrupt, auto_psv))_GFX1Interrupt(void) 
{
	static int lines = 0;
	static int syncs = 0;
	static int next_fb = 1;

	if(_VMRGNIF) 
    { /* on a vertical sync, flip buffers if it's ready */
		lines = 0;
		syncs++;
		if(fb_ready == 1 && gfx.frame_buffers == DOUBLEBUFFERED) 
        {
			if(next_fb)
            {
                gpu_setfb(&GFXDisplayBuffer[0]);
            }
            else
            {
                gpu_setfb(&GFXDisplayBuffer[0]+gfx.fb_offset);
            }
			next_fb = !next_fb;
		}
		//Nop();Nop();
        hline = lines;
		fb_ready = 0;
		_VMRGNIF = 0;
	} 
    else if(_HMRGNIF) 
    { /* on each horizontal sync, ...? */
		lines++;
        hline = lines;
		_HMRGNIF = 0;
	}
	_GFX1IF = 0;
}

void gpu_init(void)
{
    _VMRGNIF = 0;
    _VMRGNIE = 1;
    _GFX1IE = 1;
}

void config_graphics(void) 
{
    _G1CLKSEL = 1;
    _GCLKDIV = gfx.clock_div;

    // Display buffer:
    G1DPADRL = (unsigned long)(&GFXDisplayBuffer[0]) & 0xFFFF;

    // Work area 1
    G1W1ADRL = (unsigned long)(&GFXDisplayBuffer[0]) & 0xFFFF;

    // Work area 2
    G1W2ADRL = (unsigned long)(&GFXDisplayBuffer[0]) & 0xFFFF;

    if(gfx.frame_buffers == DOUBLEBUFFERED)
    {
        G1DPADRH = (unsigned long)(&GFXDisplayBuffer[0]+gfx.fb_offset) >>16 & 0xFF;
        G1W1ADRH = (unsigned long)(&GFXDisplayBuffer[0]+gfx.fb_offset) >>16 & 0xFF;
        G1W2ADRH = (unsigned long)(&GFXDisplayBuffer[0]+gfx.fb_offset) >>16 & 0xFF;
    }


    G1PUW = gfx.hres;
    G1PUH = gfx.vres;

    // Using PIC24F manual section 43 page 37-38
    _DPMODE = 1;      /* TFT */
    _GDBEN = 0xFFFF;
    _DPW = _PUW = gfx.hres; // Work area and FB size so GPU works
    _DPH = _PUH = gfx.vres;
    _DPWT = gfx.hfp + gfx.hpw + gfx.hbp + gfx.hres;

    // _DPHT may need to be adjusted for vertical centering
    _DPHT = (gfx.vfp + gfx.vpw + gfx.vbp)*2 + gfx.vres;
    _DPCLKPOL = 0;
    _DPENOE = 0;
    _DPENPOL = 0;
    _DPVSOE = 1;          /* use VSYNC */
    _DPHSOE = 1;          /* use HSYNC */
    _DPVSPOL = VSPOL;     /* VSYNC negative polarity (if VSPOL = 0)*/
    _DPHSPOL = HSPOL;     /* HSYNC negative polarity (if HSPOL = 0)*/

    // _ACTLINE may need to be adjusted for vertical centering
    _ACTLINE = _VENST = gfx.vfp + gfx.vpw + gfx.vbp - VENST_FUDGE;
    _ACTPIX = _HENST = gfx.hfp + gfx.hpw + gfx.hbp - HENST_FUDGE;
    _VSST = gfx.vfp;
    _HSST = gfx.hfp;
    _VSLEN = gfx.vpw;
    _HSLEN = gfx.hpw;
    _DPPWROE = 0;
    _DPPINOE = 1;
    _DPPOWER = 1;

    int logc=0;
    while (gfx.bpp>>logc > 1) { logc++; }
    _DPBPP = _PUBPP = logc;

    _VMRGNIE = 1;
    _HMRGNIE = 1;

    _G1EN = 1;
    __delay_ms(1);
}

void config_clut() 
{
    _CLUTEN = 1; 
}

void clut_set(uint8_t index, uint16_t color) 
{
    _CLUTADR = index;
    _CLUTRWEN = 1;
    G1CLUTWR = color;
    while(_CLUTBUSY) continue;
    _CLUTRWEN = 0;
}

void config_chr(void) 
{
    while(_CMDFUL) continue;
    G1CMDL = 0xFFFF;
    G1CMDH = CHR_FGCOLOR;
    Nop();
 
    while(_CMDFUL) continue;
    G1CMDL = 0;
    G1CMDH = CHR_BGCOLOR;
    Nop();
 
    while(_CMDFUL) continue;
    G1CMDL = (uint16_t)(FontStart) & 0xFFFF;
    G1CMDH = CHR_FONTBASE;
    Nop();
 
    while(_CMDFUL) continue;
    G1CMDL = 0;
    G1CMDH = CHR_TXTAREASTART;
    Nop();
 
    while(_CMDFUL) continue;
    G1CMDL = (gfx.hres & 0xF)<<12 | gfx.vres;
    G1CMDH = CHR_TXTAREAEND | (gfx.hres >>4);
    Nop();
}

void chr_print(char *c, uint16_t x, uint16_t y, uint8_t transparent) 
{
    // Thanks Jamis for this function :P

    int maxCharHeight = ((int)gfx.vres)-21;

    if (y > maxCharHeight) 
    {
        y = maxCharHeight;
    }

    while(_CMDFUL) continue;
    G1CMDL = x<<12 | y;
    G1CMDH = CHR_PRINTPOS | (x>>4); // CHR_PRINTPOS = 0x5A00
    Nop();

    while(*c != NULL) 
    {
        while(_CMDFUL) continue;
        G1CMDL = *c;
        if(transparent == 0)
        {
            G1CMDH = CHR_PRINTCHAR;
        }
        else
        {
            G1CMDH = CHR_PRINTCHARTRANS; // transparent
        }
        Nop();

        c++;
    }
}

void rcc_color(unsigned int color) 
{
	G1CMDL = color;
	G1CMDH = RCC_COLOR;
}

void chr_fg_color(unsigned int color) 
{
    G1CMDL = color;
    G1CMDH = CHR_FGCOLOR;
}
 
void chr_bg_color(unsigned int color) 
{
    G1CMDL = color;
    G1CMDH = CHR_BGCOLOR;
}

void rcc_setdest(__eds__ uint8_t *buf) 
{
	while(!_CMDMPT) continue; // Wait for GPU to finish drawing
	G1W1ADRL = (unsigned long)(buf);
	G1W1ADRH = (unsigned long)(buf);
	G1W2ADRL = (unsigned long)(buf);
	G1W2ADRH = (unsigned long)(buf);
}
 
void gpu_setfb(__eds__ uint8_t *buf) 
{
	G1DPADRL = (unsigned long)(buf);
	G1DPADRH = (unsigned long)(buf);
}

uint8_t gpu_set_res(resolution res, framebuffers fb, colordepth bpp)
{
    switch(res)
    {
        case RES_320x240:
            gfx.clock_div = 58;
            gfx.hres = 320;
            gfx.vres = 240;
            gfx.hfp = 32;
            gfx.hpw = 16;
            gfx.hbp = 32;
            gfx.vfp = 5;
            gfx.vpw = 3;
            gfx.vbp = 5;
            break;
        case RES_80x480:
            gfx.clock_div = 69;
            gfx.hres = 80;
            gfx.vres = 480;
            gfx.hfp = 32;
            gfx.hpw = 16;
            gfx.hbp = 32;
            gfx.vfp = 10;
            gfx.vpw = 5;
            gfx.vbp = 10;
            break;
        case RES_160x480:
            gfx.clock_div = 47;
            gfx.hres = 160;
            gfx.vres = 480;
            gfx.hfp = 32;
            gfx.hpw = 24;
            gfx.hbp = 32;
            gfx.vfp = 10;
            gfx.vpw = 5;
            gfx.vbp = 10;
            break;
        case RES_320x480:
            gfx.clock_div = 25;
            gfx.hres = 320;
            gfx.vres = 480;
            gfx.hfp = 32;
            gfx.hpw = 48;
            gfx.hbp = 32;
            gfx.vfp = 10;
            gfx.vpw = 5;
            gfx.vbp = 10;
            break;
        case RES_480x480:
            gfx.clock_div = 17;
            gfx.hres = 480;
            gfx.vres = 480;
            gfx.hfp = 32;
            gfx.hpw = 64;
            gfx.hbp = 32;
            gfx.vfp = 10;
            gfx.vpw = 5;
            gfx.vbp = 10;
            break;
        case RES_640x480:
            gfx.clock_div = 11;
            gfx.hres = 640;
            gfx.vres = 480;
            gfx.hfp = 16;
            gfx.hpw = 96;
            gfx.hbp = 48;
            gfx.vfp = 10;
            gfx.vpw = 2;
            gfx.vbp = 33;
            break;
    }

    gfx.frame_buffers = fb;
    gfx.bpp = bpp;
    gfx.hscale = gfx.vres / gfx.hres;
    
    if(fb == DOUBLEBUFFERED)
    {
        gfx.buffer_size = 2 * ((gfx.hres * gfx.vres) / (8/gfx.bpp));
    }
    else
    {
        gfx.buffer_size = ((gfx.hres * gfx.vres) / (8/gfx.bpp));
    }
    
    gfx.fb_offset = gfx.buffer_size / 2;

    if(gfx.buffer_size <= GFX_MAX_BUFFER_SIZE)
    {
        config_graphics();
        return 1;
    }

    return 0;
}

void waitForBufferFlip() 
{
    while((!_CMDMPT) | _IPUBUSY | _RCCBUSY | _CHRBUSY) continue; // Wait for IPU, RCC, and CHR GPUs to finish drawing
    fb_ready = 1;
    while(fb_ready) continue; // wait for vsync

}

uint16_t getHsync()
{
    return hline;
}

void rcc_draw(uint16_t x, uint16_t y, uint16_t w, uint16_t h) 
{
	// destination
	while(_CMDFUL) continue;
	G1CMDL = x + (y * gfx.hres);
	G1CMDH = RCC_DESTADDR| (x + (y * (uint32_t)gfx.hres))>>16;
	Nop();
 
	// size
	while(_CMDFUL) continue;
	G1CMDL = (w<<12) | h;
	G1CMDH = RCC_RECTSIZE | (w>>4);
	Nop();
 
	// go!
	while(_CMDFUL) continue;
	G1CMDL = 0xC<<3;
	G1CMDH = RCC_STARTCOPY;
	Nop();
}

void fast_pixel(unsigned long ax, unsigned long ay) 
{
	//ax += (ay << 9) + (ay << 7);
	ax += ay * gfx.hres;
	G1CMDL = ax;
	G1CMDH = RCC_DESTADDR | ax>>16;

	G1CMDL = 0x1001; // This needs to be changed for non 80x
	G1CMDH = RCC_RECTSIZE;

	while(_CMDFUL) continue;
	G1CMDL = 0x60;
	G1CMDH = RCC_STARTCOPY;
	Nop();
}

void blank_background(void) 
{
	//rcc_color(0xff);
	//rcc_color(0x0);
    rcc_color(0);
	rcc_draw(0, 0, gfx.hres-1, gfx.vres);
}

void drawBorder(uint16_t c) 
{
	// screen border
	rcc_color(c);
	rcc_draw(0,0, 1, gfx.vres); // left
	rcc_draw(gfx.hres-2,0,1,gfx.vres); // right
	rcc_draw(0,0, gfx.hres-2, gfx.hscale); // top
	rcc_draw(0,gfx.vres-gfx.hscale,gfx.hres-2,gfx.hscale); // bottom
}

void cleanup(void) 
{ // TODO: custom colors
	// Clean right column
	//rcc_color(0xe0); // neat effect
	//rcc_color(0xff); // white
	//rcc_color(0x00);
	rcc_color(0);
    rcc_draw(gfx.hres-1,0, 1,gfx.vres);
}

void clearbuffers(void)
{
    // Clear frame buffers

    // If double buffered, clear the second frame first
    if(gfx.frame_buffers == DOUBLEBUFFERED)
    {
        rcc_setdest(&GFXDisplayBuffer[0]+gfx.fb_offset);
        blank_background();
    }

    // Clear the first frame
    rcc_setdest(&GFXDisplayBuffer[0]);
    blank_background();
}


float radians(uint16_t angle) 
{
	return ((angle*3.14159)/180.0);
}

int realtoint(float oldval, float oldmin, float oldmax, float newmin, float newmax) 
{
	return (int)((((oldval - oldmin) * (newmax - newmin)) / (oldmax - oldmin)) + newmin);
}

void drawLineS(float x1, float y1, float x2, float y2) 
{
	return;
}

void line (float x1, float y1, float x2, float y2, uint8_t color) 
{
        unsigned int i;
        double hl=fabs(x2-x1), vl=fabs(y2-y1), length=(hl>vl)?hl:vl;
        float deltax=(x2-x1)/(float)length, deltay=(y2-y1)/(float)length;
        for (i=0; i<(int)length; i++) {
                unsigned long x=(int)(x1+=deltax), y=(int)(y1+=deltay);
                if ((x<gfx.hres)&&(y<gfx.vres)) {
                        rcc_color(color);
                        //rcc_color(0x3);
                        //rcc_draw(x,y, PIX_W,PIX_H);
                        // TODO: fix y displacement
			fast_pixel(x,y+6);// + i*PIX_H);
                        }
                }
}

void render (float xa, float ya, float za) 
{
        int i;
        float mat[4][4];            // Determine rotation matrix
        float xdeg=xa*3.1416f/180, ydeg=ya*3.1416f/180, zdeg=za*3.1416f/180;
        float sx=(float)sin(xdeg), sy=(float)sin(ydeg), sz=(float)sin(zdeg);
        float cx=(float)cos(xdeg), cy=(float)cos(ydeg), cz=(float)cos(zdeg);
        float x[8], y[8], z[8], rx[8], ry[8], rz[8], scrx[8], scry[8];

        mat[0][0]=cx*cz+sx*sy*sz, mat[1][0]=-cx*sz+cz*sx*sy, mat[2][0]=cy*sx;
        mat[0][1]=cy*sz, mat[1][1]=cy*cz, mat[2][1]=-sy;
        mat[0][2]=-cz*sx+cx*sy*sz, mat[1][2]=sx*sz+cx*cz*sy, mat[2][2]=cx*cy;
        for (i=0; i<8; i++) {
                rx[i]=x[i]*mat[0][0]+y[i]*mat[1][0]+z[i]*mat[2][0];
                ry[i]=x[i]*mat[0][1]+y[i]*mat[1][1]+z[i]*mat[2][1];
                rz[i]=x[i]*mat[0][2]+y[i]*mat[1][2]+z[i]*mat[2][2]+100;
                scrx[i]=(rx[i]*10)/rz[i]+(gfx.hres/2), scry[i]=(ry[i]*60)/rz[i]+(gfx.vres/2);
        }
        for (i=0; i<4; i++) {
                line (scrx[i], scry[i], scrx[i+4], scry[i+4],rand()%16);
                line (scrx[i], scry[i], scrx[(i+1)%4], scry[(i+1)%4],rand()%16);
                line (scrx[i+4], scry[i+4], scrx[((i+1)%4)+4], scry[((i+1)%4)+4],rand()%16);
        }
}