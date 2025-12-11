#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    printf("Hello World! \n");
    SDL_Window *pwindow = SDL_CreateWindow("Image Viewer", 900, 600, 0);
    SDL_Delay(3000);

    SDL_DestroyWindow(pwindow);
    SDL_Quit();
    return 0;
}