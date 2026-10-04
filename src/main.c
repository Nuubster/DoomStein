#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <math.h>

#define WIDTH 450
// #define step 2.0f/(float)WIDTH 
// steo instead of i += 0.005f
#define HEIGHT 350
#define CELL_SIZE 32
#define PI 3.141592653589793 // Hehe nasa refference

static uint32_t framebuffer[WIDTH * HEIGHT];

static int Map[9][10] = 
{
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    { 1, 0, 1, 0, 0, 0, 1, 0, 0, 1 },
    { 1, 0, 1, 1, 1, 0, 1, 0, 0, 1 },
    { 1, 0, 1, 0, 0, 0, 1, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 1, 1, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1, 1, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
};
// Map[y][x]

void PutPixel(uint32_t x, uint32_t y, uint32_t color);
struct Player {
    float x;
    float y;
    float angle;
};

float RayCalc(float angle, struct Player Slayer) 
{
    float distance = 0.00f;

    // Collision checks for Rays
    while (distance < 300.0f) 
    {
        float rayX = Slayer.x + cosf(angle) * distance;
        float rayY = Slayer.y + sinf(angle) * distance;

        int mapX = (int)(rayX / CELL_SIZE);
        int mapY = (int)(rayY / CELL_SIZE);

         if (Map[mapY][mapX] == 1) {
             // Here we hit a wall... render?
             return distance;
         }
         // else { PutPixel(rayX, rayY, 0x33FF33); }

         distance += 1.00f;
    }
  return 300.0f;
}

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

void PutPixel(uint32_t x, uint32_t y, uint32_t color) 
{
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) { return; }
    framebuffer[y * WIDTH + x] = color;
}

void Clear() {
    for (int X = 0; X < WIDTH; X++){
        for (int Y = 0; Y < HEIGHT; Y++) {
                framebuffer[Y * WIDTH + X] = 0x2A2A2A;
        }
    }
}

int main() {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Event event;

    const double TargetFrame = 1.0 / 60.0;
    // Init
    window = SDL_CreateWindow("Wolfenstein 1D", WIDTH * 2, HEIGHT * 2, 0);
    renderer = SDL_CreateRenderer(window, NULL);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_XRGB8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);

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
        // printf("angle = %f, cos = %f, sin = %f\n", Slayer.angle, cosf(Slayer.angle), sinf(Slayer.angle));  
        // printf("X movement: %d | Y movement: %d\n", Slayer.x, Slayer.y);

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

         if (Map[mapY][mapX] == 1) {
         }
         else { PutPixel(rayX, rayY, 0x33FF33); }

         distance += 1.00f;
         }

        } 
        else 
        {
        Clear();
        float distance;

        for (int x = 0; x < WIDTH; x++) 
        { 
            float i = ((float)x / WIDTH) * 2.0f - 1.0f;

            distance = RayCalc(Slayer.angle + i, Slayer);
            float CorrectedDistance = distance * cosf((Slayer.angle + i) - Slayer.angle);

            int WallSize = 9000 / CorrectedDistance;
            int WallTop =  (HEIGHT - WallSize) / 2;
            int WallBottom = WallTop + WallSize;
  
            for (int y = 0; y < HEIGHT; y++)
            {
                if (y <= WallTop || y >= WallBottom) { PutPixel(x, y, 0x2A2A2A); }
                else { if (distance <= 50) { PutPixel(x, y, 0xFFCD00); } else { PutPixel(x, y, 0xFF671F); } } 
            }
        }
        }
                       
        

        SDL_UpdateTexture(texture, NULL, framebuffer, WIDTH * sizeof(uint32_t));

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
        //printf("After Delay: %.6f FPS: %.2f\n", elapsed ,FPS);
        //printf("FPS: %.2f\n", FPS);
    }
    return 0;
}
