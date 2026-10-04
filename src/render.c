#include "render.h"

uint32_t framebuffer[RENDER_WIDTH * RENDER_HEIGHT];

void PutPixel(uint32_t x, uint32_t y, uint32_t color)
{
    if (x < 0 || x >= RENDER_WIDTH || y < 0 || y >= RENDER_HEIGHT) { return; }
    framebuffer[y * RENDER_WIDTH + x] = color;
}

void Clear() 
{
    for (int X = 0; X < RENDER_WIDTH; X++) {
        for (int Y = 0; Y < RENDER_HEIGHT; Y++) {
            framebuffer[Y * RENDER_WIDTH + X] = 0x2A2A2A;
        }
    }
}
