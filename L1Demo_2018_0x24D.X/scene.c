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
    scene[0].scene_id = 0;
    scene[0].start_time = 0;
    scene[0].stop_time = 1000000;
    scene[0].music_track_id = 0;
    scene[0].res = RES_160x480;
    scene[0].fb_num = DOUBLEBUFFERED;
    scene[0].color_depth = BPP_4;

    scene[1].scene_id = 1;
    scene[1].start_time = 1000000;
    scene[1].stop_time = 20000000;
    scene[1].music_track_id = 0;
    scene[1].res = RES_160x480;
    scene[1].fb_num = DOUBLEBUFFERED;
    scene[1].color_depth = BPP_4;

    scene_func = &scene_zero;
}

void scene_render_frame(void)
{
    static uint16_t scene_index = 0;

    if(scene[scene_index].stop_time < time)
    {
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
        
        gpu_set_res(scene[scene_index].res, scene[scene_index].fb_num, scene[scene_index].color_depth);
        
        // TODO: Update music track here

        if(scene_index < TOTAL_NUM_SCENES)
        {
            scene_index++;
        }
    }

    scene_func();
}

void scene_zero(void)
{
    rcc_color(1);
    rcc_line(rand()%(gfx.hres-2),rand()%(gfx.vres-2),rand()%(gfx.hres-2),rand()%(gfx.vres-2),1);
}

void scene_one(void)
{
    rcc_color(1); 
    rcc_rec(0,0,gfx.hres-1,gfx.vres-1);
}