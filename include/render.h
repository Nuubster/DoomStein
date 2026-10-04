#ifndef RENDER_H
#define MAP_H

#include <stdint.h>

#define RENDER_WIDTH 450
#define RENDER_HEIGHT 350

extern uint32_t framebuffer[RENDER_WIDTH * RENDER_HEIGHT];

void PutPixel(uint32_t x, uint32_t y, uint32_t color);
void Clear(void);

#endif
