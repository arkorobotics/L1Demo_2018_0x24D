/*
 Event>>  			Layerone Demo Party 2018
 Title>>  			NO TITLE YET
 Group>>			0x24D
 By>>     			Arko & Davo
 Base Libraries>>	Datagram & Hotdogs & Jamis
 ---------------------------------------------------------
 Warning>>			Don't take any of this code seriously
 Danger>>			We're super cereal
 ---------------------------------------------------------
 [ INSERT ASCII ART HERE ]
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <string.h>
#include <math.h>
#include "gpu.h"
#include "audio.h"
#include "color.h"
#include "music.h"
#include "sprites.h"
 
#define  FCY    16000000UL                                                      // Instruction cycle frequency, Hz
#include <libpic30.h>
 
#pragma config FWDTEN = OFF , GWRP = OFF , GCP = OFF , JTAGEN = OFF
#pragma config POSCMOD = HS , FCKSM = CSDCMD , FNOSC = PRIPLL , PLL96MHZ = ON , PLLDIV = DIV2
#pragma config ALTPMP = ALTPMPEN , SOSCSEL = EC

int main(void) 
{
	ANSB = 0x0000;                                                              // Configuration
	ANSC = 0x0000;
	ANSD = 0x0000;
	ANSF = 0x0000;
	ANSG = 0x0000;
	TRISB = 0x0000;

    gpu_init();
    gpu_set_res(RES_80x480, DOUBLEBUFFERED, BPP_1);

	config_clut();                                                              // Configure Color LookUp Table (CLUT)
	config_chr();                                                               // Configure Font Table
	config_audio();                                                             // Configure Audio ISR
	clearbuffers();                                                             // Clear the screen
 	
 	uint16_t c0 = 0;                                                            // Black
	uint16_t c1 = rgb_2_565(180, 180, 180);                                     // Light Grey
	uint16_t c2 = rgb_2_565(180, 180, 16);                                      // Yellow
	uint16_t c3 = rgb_2_565(16, 180, 180);                                      // Cyan

 	clut_set(0, c0);                                                            // Load CLUT
 	clut_set(1, c1);
 	clut_set(2, c2);
    clut_set(3, c3);

    // Draw
	while (1) 
	{
        blank_background();

        static uint16_t x = 0;
        static uint16_t y = 0;

        rcc_color(1);


            fast_pixel((sinetable[(2*x)%509]+10)%(gfx.hres-2),(1*sinetable[(2*(y+x))%509]+50)%(gfx.vres-2));
            //fast_pixel(x,y);
            if(x > (gfx.hres-2))
            {
                if(y > (gfx.vres-2))
                {
                    x=0;
                    y=0;
                }
                else
                {
                    y++;
                    
                    x=0;                
                }
            }
            x++;
        

        drawBorder(0);
        cleanup();                                                          // Housekeeping for VGA signaling
        waitForBufferFlip();                                                // For next Vsync
		frames++;                                                           // Increment frame count
	}

	return 0;
}
