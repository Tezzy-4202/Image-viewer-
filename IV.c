#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        return 1;
    }

    SDL_Surface *pImage = SDL_LoadBMP("test.bmp");
    if (!pImage)
    {
        return 1;
    }

    int maxWidth = 1000;
    int maxHeight = 800;
    int finalW = pImage->w;
    int finalH = pImage->h;

    if (finalW > maxWidth)
    {
        float scale = (float)maxWidth / finalW;
        finalW = maxWidth;
        finalH = (int)(finalH * scale);
    }
    if (finalH > maxHeight)
    {
        float scale = (float)maxHeight / finalH;
        finalH = maxHeight;
        finalW = (int)(finalW * scale);
    }

    SDL_Window *pWindow = SDL_CreateWindow("Vibe Manipulator", finalW, finalH, 0);
    if (!pWindow)
    {
        SDL_DestroySurface(pImage);
        return 1;
    }

    SDL_Surface *pWindowSurface = SDL_GetWindowSurface(pWindow);

    SDL_Rect destRect = {0, 0, finalW, finalH};
    SDL_BlitSurfaceScaled(pImage, NULL, pWindowSurface, &destRect, SDL_SCALEMODE_LINEAR);
    SDL_UpdateWindowSurface(pWindow);

    int running = 1;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = 0;
            }
        }
        SDL_Delay(10);
    }

    SDL_DestroySurface(pImage);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    return 0;
}