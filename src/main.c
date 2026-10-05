#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <math.h>

#include "map.h"
#include "render.h"

#define CELL_SIZE 32

struct Player {
    float x;
    float y;
    float angle;
};

uint32_t TestTexture[8][8] =
{
    { 0xFF00FF, 0x0, 0xFF00FF, 0x0, 0xFF00FF, 0x0, 0xFF00FF, 0x0 },
    { 0x0, 0xFF00FF, 0x0, 0xFF00FF, 0x0, 0xFF00FF, 0x0, 0xFF00FF },
    { 0xFF00FF, 0x0, 0xFF00FF, 0x0, 0xFF00FF, 0x0, 0xFF00FF, 0x0 },
    { 0x0, 0xFF00FF, 0x0, 0xFF00FF, 0x0, 0xFF00FF, 0x0, 0xFF00FF },
    { 0xFF00FF, 0x0, 0xFF00FF, 0x0, 0xFF00FF, 0x0, 0xFF00FF, 0x0 },
    { 0x0, 0xFF00FF, 0x0, 0xFF00FF, 0x0, 0xFF00FF, 0x0, 0xFF00FF },
    { 0xFF00FF, 0x0, 0xFF00FF, 0x0, 0xFF00FF, 0x0, 0xFF00FF, 0x0 },
    { 0x0, 0xFF00FF, 0x0, 0xFF00FF, 0x0, 0xFF00FF, 0x0, 0xFF00FF }
};

int main() {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Event event;

    SDL_Surface *image;

    const double TargetFrame = 1.0 / 60.0;
    
    // Init
    window = SDL_CreateWindow("Wolfenstein 1D", RENDER_WIDTH * 2, RENDER_HEIGHT * 2, 0);
    renderer = SDL_CreateRenderer(window, NULL);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_XRGB8888, SDL_TEXTUREACCESS_STREAMING, RENDER_WIDTH, RENDER_HEIGHT);

    uint8_t IsRunning = 1;
    uint32_t frame = 0;
    uint8_t MapView = 1;

    // Init of Player
    struct Player Slayer;
    Slayer.x = 50;
    Slayer.y = 60;
    Slayer.angle = 4.71f;

    // Main Loop
    while (IsRunning == 1) 
    {
        uint64_t start = SDL_GetPerformanceCounter();

        const bool *keys = SDL_GetKeyboardState(NULL);
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) { IsRunning = false; }
            if (event.type == SDL_EVENT_KEY_DOWN) { 
                if (event.key.key == SDLK_TAB) { MapView = (MapView == 1) ? 0 : 1; }
            }
        }

       if (MapView == 1) {
            if (keys[SDL_SCANCODE_W]) { Slayer.y -= 1; }
            if (keys[SDL_SCANCODE_S]) { Slayer.y += 1; }
            if (keys[SDL_SCANCODE_D]) { Slayer.x += 1; }
            if (keys[SDL_SCANCODE_A]) { Slayer.x -= 1; }
       }
            // Front = (cos angle, sin angle)
       else {
            if (keys[SDL_SCANCODE_W]) { Slayer.x += cosf(Slayer.angle); Slayer.y += sinf(Slayer.angle); }
            if (keys[SDL_SCANCODE_S]) { Slayer.x -= cosf(Slayer.angle); Slayer.y -= sinf(Slayer.angle); }
            if (keys[SDL_SCANCODE_D]) { Slayer.x -= sinf(Slayer.angle); Slayer.y += cosf(Slayer.angle); }
            if (keys[SDL_SCANCODE_A]) { Slayer.x += sinf(Slayer.angle); Slayer.y -= cosf(Slayer.angle); }
       }
    
            if (keys[SDL_SCANCODE_LEFT]) { Slayer.angle -= 0.05f; }
            if (keys[SDL_SCANCODE_RIGHT]) { Slayer.angle += 0.05f; }
        
        Clear();
             
        if (MapView == 1) 
        {
        MapSetUp();

        // Rendering Player
        for (int i = -1; i < 1; i++) {
            for (int j = -1; j < 1; j++) { PutPixel(Slayer.x + i, Slayer.y + j, 0x005EB8); } 
        }
        float distance = 0;

        // Angle of POV rendered
        while (distance < 300.0f) 
        {
            float rayX = Slayer.x + cosf(Slayer.angle) * distance;
            float rayY = Slayer.y + sinf(Slayer.angle) * distance;

            int mapX = (int)(rayX / CELL_SIZE);
            int mapY = (int)(rayY / CELL_SIZE);

            if (Map[mapY][mapX] == 1) {}
            else { PutPixel(rayX, rayY, 0x33FF33); }

            distance += 1.00f;
        }
        } 
        else 
        {
        Clear();
        float distance;

        // the DDA black box
        for (int x = 0; x < RENDER_WIDTH; x++) 
        {
            // this is the FOV btw
            float i = ((float)x / RENDER_WIDTH) * 1.0f - 0.5f;

            // The direction the ray is facing
            float RayDirX = cosf(Slayer.angle + i);
            float RayDirY = sinf(Slayer.angle + i);

            // Player's position on the MAP
            float SlayerMapX = Slayer.x / CELL_SIZE;
            float SlayerMapY = Slayer.y / CELL_SIZE;

            // Player's position on the MAP(array)
            int MapX = (int)SlayerMapX;
            int MapY = (int)SlayerMapY;

            // Which Cell to move next
            int StepX = RayDirX > 0 ? 1 : -1;
            int StepY = RayDirY > 0 ? 1 : -1;

            // Distance (along the ray) between grid boundaries 
            float DeltaDistX = fabsf(1 / RayDirX);
            float DeltaDistY = fabsf(1 / RayDirY);

            // Distance (along the ray) to the next grid boundary 
            float SideDistX;
            float SideDistY;

            if (RayDirX > 0) { SideDistX = (MapX + 1 - SlayerMapX) * DeltaDistX; }
                else { SideDistX = (SlayerMapX - MapX) * DeltaDistX; }
            if (RayDirY > 0) { SideDistY = (MapY + 1 - SlayerMapY) * DeltaDistY; }
                else { SideDistY = (SlayerMapY - MapY) * DeltaDistY; }

            int HitStatus = Map[MapY][MapX] == 1 ? 1 : 0;

            // 0 Last grid boundary was Vertical
            // 1 Last grid boundary was Horizontal
            int SideStatus;
            float PerpWallDist;

            while (HitStatus == 0) 
            {
                if (SideDistX < SideDistY) { SideDistX += DeltaDistX; MapX += StepX; SideStatus = 0; } 
                else { SideDistY += DeltaDistY; MapY += StepY; SideStatus = 1; }

                HitStatus = Map[MapY][MapX] == 1 ? 1 : 0;
            }

            PerpWallDist = SideStatus == 0 ? SideDistX - DeltaDistX : SideDistY - DeltaDistY;

            // The values from DDA being used
            int WallHeight = 180 / PerpWallDist;
            int WallTop = (RENDER_HEIGHT - WallHeight) / 2;
            int WallBottom = (RENDER_HEIGHT + WallHeight) / 2;

            float HitOnWallX = SlayerMapX + (RayDirX * PerpWallDist);
            float HitOnWallY = SlayerMapY + (RayDirY * PerpWallDist);

            float TextureDivideY = 8.0f / RENDER_HEIGHT;
            float TextureDivideX = 8.0f / RENDER_WIDTH;
            int TextureStartX = SideStatus == 1 ? (int)(TextureDivideX * x) : (int)(TextureDivideX * HitOnWallX) ;

            // THE RENDERING!! ~ with the basic distance buffer
            for (int y = WallTop; y < WallBottom; y++) 
            {
                int TextureStartY = SideStatus == 0 ? (int)(TextureDivideY * y) : (int)(TextureDivideY * HitOnWallY);
                PutPixel(x, y, TestTexture[TextureStartY][TextureStartX]);

                /*
                if (PerpWallDist <= 2) {
                    PutPixel(x, y, 0xFF0000); } 
                else {
                    PutPixel(x, y, 0xC8102E); } 
                */
            }
        }
        }

        SDL_UpdateTexture(texture, NULL, framebuffer, RENDER_WIDTH * sizeof(uint32_t));

        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        uint64_t end = SDL_GetPerformanceCounter();
        double elapsed = (double)(end - start) / (double)SDL_GetPerformanceFrequency();

        // printf("Before delay: %.6f\n", elapsed);

        if (elapsed < TargetFrame) { SDL_DelayPrecise((uint64_t)((TargetFrame - elapsed) * 1000000000.0)); }
        frame++;
        
        uint64_t final = SDL_GetPerformanceCounter();

        double FrameTime = (double)(final - start) / (double)SDL_GetPerformanceFrequency();
        double FPS = 1.0 / FrameTime;
        
        if (frame % 60 == 0) { printf("FPS: %.2f\n", FPS); }

        //printf("After Delay: %.6f FPS: %.2f\n", elapsed ,FPS);    
    }
    return 0;
}

