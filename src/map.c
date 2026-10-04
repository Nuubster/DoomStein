#include "map.h"
#include "render.h"

#define CELL_SIZE 32

int Map[9][10] = 
{
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    { 1, 0, 1, 0, 0, 0, 1, 1, 1, 1 },
    { 1, 0, 0, 0, 1, 0, 1, 0, 0, 1 },
    { 1, 0, 0, 0, 1, 0, 1, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1, 1, 0, 1, 1, 1 },
    { 1, 0, 0, 0, 1, 0, 0, 0, 0, 1 },
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
};
// Map[y][x]

void MapSetUp()
{
    for (int mapY = 0; mapY < 10; mapY++) {
        for (int mapX = 0; mapX < 10; mapX++) {
            for (int pixelY = 1; pixelY < CELL_SIZE; pixelY++) {
                for (int pixelX = 1; pixelX < CELL_SIZE; pixelX++) 
                {
                    int screenX = mapX * CELL_SIZE + pixelX;
                    int screenY = mapY * CELL_SIZE + pixelY;

                    if (Map[mapY][mapX] == 1) { PutPixel(screenX, screenY, 0xFFFFFF); }
                    if (Map[mapY][mapX] == 0) { PutPixel(screenX, screenY, 0x2A2A2A); }  
                }
            }
        }
    }       
}




