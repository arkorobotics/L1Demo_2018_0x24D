#ifndef SCENE_H 
#define SCENE_H 

#include "gpu.h"

#define START_SCENE 3
#define TOTAL_NUM_SCENES 4

struct Scene
{
    uint16_t scene_id;

    uint32_t start_time;
    uint32_t stop_time;

    uint8_t music_track_id;

    resolution  res;
    framebuffers fb_num;
    colordepth color_depth;
};

void scene_init(void);
void scene_render_frame(void);

void scene_loadscreen(void);
void scene_numberstation(void);
void scene_lorenz(void);
void scene_plasma(void);

#endif