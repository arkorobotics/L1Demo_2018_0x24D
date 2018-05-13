#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <string.h>
#include <math.h>
#include "xc.h"
#include "alu.h"
#include "gpu.h"
#include "audio.h"
#include "color.h"
#include "music.h"
#include "scene.h"
#include "sprites.h"

struct Scene scene[TOTAL_NUM_SCENES];

void (*scene_func)(void);

void scene_init(void)
{
    // Configure settings for all scenes
    scene[0].scene_id = 0;
    scene[0].start_time = 0;
    scene[0].stop_time = 100000;
    scene[0].music_track_id = 0;
    scene[0].res = RES_160x480;
    scene[0].fb_num = SINGLEBUFFERED;
    scene[0].color_depth = BPP_4;

    scene[1].scene_id = 1;
    scene[1].start_time = 10;
    scene[1].stop_time = 20;
    scene[1].music_track_id = 0;
    scene[1].res = RES_320x480;
    scene[1].fb_num = SINGLEBUFFERED;
    scene[1].color_depth = BPP_2;

    // Set the current scene to the scene #0
    scene_func = &scene_zero;
    gpu_set_res(scene[0].res, scene[0].fb_num, scene[0].color_depth);
}

void scene_render_frame(void)
{
    static uint16_t scene_index = 0;

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
                scene_func = &scene_zero;
                break;
            case 1:
                scene_func = &scene_one;
                break;
            default:
                scene_func = &scene_zero;
                break;
        }
        
        // Configure new graphics settings
        gpu_set_res(scene[scene_index].res, scene[scene_index].fb_num, scene[scene_index].color_depth);
        
        // TODO: Update music track here

    }

    scene_func();
}

void scene_zero(void)
{
    static uint16_t init = 0;
    static uint16_t scene_count = 0;
    static uint16_t drawcount = 0;

    static double h = 0.008;
    static double a = 10;
    static double b = 28;
    static double c = 2.66;

    static double x = 0;
    static double y = 10;
    static double z = 10;
    static float angle = 0;

    if(init == 0)
    {
        int i;

        for(i=1; i<16; i++)
        {
            gpu_clut_set(i, rgb_2_565((16*i), (16*i), (16*i) ));
        }

        init = 1;
    }

    alu_calc_rot_matrix(angle, 0, 0, 1);

    if(angle < 360)
    {
        angle += 10;
    }
    else
    {
        angle = 0;
    }

    while(drawcount < 1000)
    {
        // LORENZ ATTRACTOR   ¯\_(ツ)_/¯
        x+=h*a*(y-x);               
        y+=h*(x*(b-z)-y);          
        z+=h*(x*y-c*z);

        // Rotate it!
        alu_rot(x, y, z);

        rcc_color( ((uint16_t)(outputMatrix[2][0]/4)%15) );

        rcc_pixel((uint16_t)(2*(outputMatrix[0][0]+30)),(uint16_t)(3*(outputMatrix[1][0]+50)));

        //rcc_rec((uint16_t)(4*(x+20)),(uint16_t)(6*(y+40)),(uint16_t)(z/10),(uint16_t)((z/10)*gfx.hscale));

        drawcount++;
    }

    drawcount = 0;
    
    x = 0;
    y = 10;
    z = 10;

    scene_count++;
}

void scene_one(void)
{
    rcc_color(1); 
    rcc_rec(0,0,gfx.hres-1,gfx.vres-1);
}

