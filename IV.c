#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    printf("Hello World! \n");
    SDL_Surface *pImage = SDL_LoadBMP("test.bmp");

    if (!pImage)
    {
        SDL_Log("Failed to load image: %s", SDL_GetError());
        return 1;
    }
    SDL_Window *pWindow = SDL_CreateWindow("Image Viewer", pImage->w, pImage->h, 0);
    SDL_Surface *pSurface = SDL_GetWindowSurface(pWindow);

    SDL_BlitSurface(pImage, NULL, pSurface, NULL);
    SDL_UpdateWindowSurface(pWindow);
    SDL_Delay(5000);

    SDL_DestroySurface(pImage);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    return 0;
}