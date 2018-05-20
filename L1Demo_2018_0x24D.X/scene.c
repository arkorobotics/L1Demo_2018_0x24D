#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <string.h>
#include <math.h>
#include <libpic30.h>
#include "xc.h"
#include "alu.h"
#include "gpu.h"
#include "audio.h"
#include "color.h"
#include "music.h"
#include "scene.h"
#include "sprites.h"
#include "voice.h"

struct Scene scene[TOTAL_NUM_SCENES];

void (*scene_func)(void);

void scene_init(void)
{
    // Configure settings for all scenes

    // Loading screen
    scene[0].scene_id = 0;
    scene[0].start_time = 0;
    scene[0].stop_time = 15;
    scene[0].music_track_id = 0;
    scene[0].res = RES_160x480;
    scene[0].fb_num = SINGLEBUFFERED;
    scene[0].color_depth = BPP_4;

    // Numberstation
    scene[1].scene_id = 1;
    scene[1].start_time = 15;
    scene[1].stop_time = 30;
    scene[1].music_track_id = 1;
    scene[1].res = RES_160x480;
    scene[1].fb_num = SINGLEBUFFERED;
    scene[1].color_depth = BPP_4;

    // Lorenz Attractor
    scene[2].scene_id = 2;
    scene[2].start_time = 30;
    scene[2].stop_time = 2000;
    scene[2].music_track_id = 2;
    scene[2].res = RES_160x480;
    scene[2].fb_num = SINGLEBUFFERED;
    scene[2].color_depth = BPP_4;

    // Set the current scene function
    scene_func = &scene_loadscreen;

    // Set the start time
    time_sec = scene[START_SCENE].start_time;

    // Configure the GPU for the start scene
    gpu_set_res(scene[START_SCENE].res, scene[START_SCENE].fb_num, scene[START_SCENE].color_depth);
}

void scene_render_frame(void)
{
    static uint16_t scene_index = START_SCENE;

    // Change scenes when we reach the stop time for the current scene
    if(time_sec >= scene[scene_index].stop_time)
    {
        // Increment scene index
        if(scene_index < TOTAL_NUM_SCENES)
        {
            scene_index++;
        }
        
        // Change Scenes
        switch(scene_index)
        {
            case 0:
                scene_func = &scene_loadscreen;
                break;
            case 1:
                scene_func = &scene_numberstation;
                break;
            case 2:
                scene_func = &scene_lorenz;
                break;

            default:
                scene_func = &scene_loadscreen;
                break;
        }
        
        // Configure new graphics settings
        gpu_set_res(scene[scene_index].res, scene[scene_index].fb_num, scene[scene_index].color_depth);
        
        // TODO: Update music track here

    }

    scene_func();
}

void scene_loadscreen(void)
{
    static uint8_t init = 0;
    static uint8_t ls_counter = 0;
    static uint8_t w1 = 0;
    static uint8_t w2 = 0;
    static uint8_t w3 = 0;

    if(init == 0)
    {
        init = 1;
        gpu_clear_all_fb();
        audio_init();
        gpu_clut_set(0, 0x0);
        gpu_clut_set(1, 0xffff);
        gpu_clut_set(2, 0x37e6);
        gpu_clut_set(3, 0x1542);
        gpu_chr_bg_color(0);
        gpu_chr_fg_color(3);
    }
    else
    {
        ls_counter ++;
        if (ls_counter%25 == 0)
        {
            if (w3 < 38)
            {
                w3 ++;
            }
        }
        if (ls_counter%20 == 0)
        {
            if (w2 < 38)
            {
                w2 ++;
            }
        }
        if (ls_counter%15 == 0)
        {
            if (w1 < 38)
            {
                w1 ++;
            }
        }
    }
    rcc_color(1);
    rcc_rec(5, 455, w1, 15);
    rcc_color(2);
    rcc_rec(5+w1, 455, w2, 15);
    rcc_color(3);
    rcc_rec(5+w1+w2, 455, w3, 15);
    gpu_chr_print("0x24D", 125, 460, 0);
    
    rcc_color(0);
}

void scene_numberstation(void)
{
    static uint8_t init = 0;

    if(init == 0)
    {
        init = 1;
        gpu_clear_all_fb();
        voice_init();
    }
}

void scene_lorenz(void)
{
    static uint8_t init = 0;

    // Draw a '3D' Lorenz Attractor then make it go rainbow!
    static uint16_t scene_count = 0;
    static uint16_t drawcount = 0;

    static double h = 0.008;
    static double a = 10;
    static double b = 28;
    static double c = 2.66;

    static double x = 0;
    static double y = 10;
    static double z = 10;

    static uint16_t red = 255;
    static uint16_t green = 0;
    static uint16_t blue = 0;

    if(init == 0)
    {
        init = 1;
        gpu_clear_all_fb();
        audio_init();
    }

    if(red > 0 && blue == 0)
    {
        green++;
        red--;
    }
    else if(green > 0)
    {
        green--;
        blue++;
    }
    else if(blue > 0)
    {
        blue--;
        red++;
    }


    int i;

    for(i=1; i<15; i++)
    {
        gpu_clut_set(i, rgb_2_565( (uint8_t)(16*i)*((double)red/255), (uint8_t)(16*i)*((double)green/255), (uint8_t)(16*i)*((double)blue/255) ));
    }

    while(drawcount < 10)
    {
        // LORENZ ATTRACTOR   ¯\_(ツ)_/¯
        x+=h*a*(y-x);               
        y+=h*(x*(b-z)-y);          
        z+=h*(x*y-c*z);

        rcc_color( ((uint16_t)(z/4)%15) );

        rcc_pixel((uint16_t)(3*(x+28)),(uint16_t)(5*(y+50)));

        drawcount++;
    }

    drawcount = 0;

    scene_count++;
}

void scene_two(void)
{
    gpu_clear_fb();

    // Rotate it!
    //alu_calc_rot_matrix(angle, 0, 0, 1);

    //alu_rot(x, y, z);

    rcc_color(1); 
    rcc_rec(0,0,gfx.hres-1,gfx.vres-1);
}

