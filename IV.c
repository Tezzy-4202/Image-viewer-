#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

typedef struct {
    SDL_Rect rect;
    float value;
    Uint32 color;
} Slider;

Uint8 clamp(int value) {
    if (value < 0) return 0;
    if (value > 255) return 255;
    return (Uint8)value;
}

void DrawSlider(SDL_Surface *dst, Slider *s) {
    SDL_FillSurfaceRect(dst, &s->rect, 0xFF333333);

    SDL_Rect fillRect = s->rect;
    fillRect.w = (int)(s->rect.w * s->value);
    SDL_FillSurfaceRect(dst, &fillRect, s->color);

    SDL_Rect knob = {fillRect.x + fillRect.w - 5, fillRect.y - 2, 10, fillRect.h + 4};
    SDL_FillSurfaceRect(dst, &knob, 0xFFFFFFFF);
}

int UpdateSlider(Slider *s, float mouseX, float mouseY, int isMouseDown) {
    if (isMouseDown && 
        mouseX >= s->rect.x && mouseX <= (s->rect.x + s->rect.w) &&
        mouseY >= s->rect.y && mouseY <= (s->rect.y + s->rect.h)) {
        
        s->value = (mouseX - s->rect.x) / (float)s->rect.w;
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return 1;

    SDL_Surface *pImage = SDL_LoadBMP("test.bmp");
    if (!pImage) return 1;

    int sidebarWidth = 250;
    int maxImageW = 800;
    int maxImageH = 600;

    int imgW = pImage->w;
    int imgH = pImage->h;
    if (imgW > maxImageW) {
        float scale = (float)maxImageW / imgW;
        imgW = maxImageW;
        imgH = (int)(imgH * scale);
    }
    if (imgH > maxImageH) {
        float scale = (float)maxImageH / imgH;
        imgH = maxImageH;
        imgW = (int)(imgW * scale);
    }

    int windowW = imgW + sidebarWidth;
    int windowH = (imgH > 400) ? imgH : 400;

    SDL_Window *pWindow = SDL_CreateWindow("Vibe Mixer", windowW, windowH, 0);
    if (!pWindow) return 1;
    SDL_Surface *pWindowSurface = SDL_GetWindowSurface(pWindow);
    const SDL_PixelFormatDetails *format = SDL_GetPixelFormatDetails(pWindowSurface->format);

    Slider rSlider = {{imgW + 25, 50, 200, 30}, 1.0f, SDL_MapSurfaceRGB(pWindowSurface, 255, 0, 0)};
    Slider gSlider = {{imgW + 25, 100, 200, 30}, 1.0f, SDL_MapSurfaceRGB(pWindowSurface, 0, 255, 0)};
    Slider bSlider = {{imgW + 25, 150, 200, 30}, 1.0f, SDL_MapSurfaceRGB(pWindowSurface, 0, 0, 255)};

    int running = 1;
    int needsUpdate = 1;
    int mouseBtnDown = 0;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) running = 0;
            else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) mouseBtnDown = 1;
            else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) mouseBtnDown = 0;
        }

        float mx, my;
        SDL_GetMouseState(&mx, &my);

        if (mouseBtnDown) {
            if (UpdateSlider(&rSlider, mx, my, 1)) needsUpdate = 1;
            if (UpdateSlider(&gSlider, mx, my, 1)) needsUpdate = 1;
            if (UpdateSlider(&bSlider, mx, my, 1)) needsUpdate = 1;
        }

        if (needsUpdate) {
            SDL_FillSurfaceRect(pWindowSurface, NULL, 0xFF000000);

            SDL_Rect destRect = {0, 0, imgW, imgH};
            SDL_BlitSurfaceScaled(pImage, NULL, pWindowSurface, &destRect, SDL_SCALEMODE_LINEAR);

            SDL_LockSurface(pWindowSurface);
            Uint32 *pixels = (Uint32 *)pWindowSurface->pixels;
            int pitch = pWindowSurface->pitch / 4;

            for (int y = 0; y < imgH; y++) {
                for (int x = 0; x < imgW; x++) {
                    int i = y * pitch + x;
                    
                    Uint8 r, g, b;
                    SDL_GetRGB(pixels[i], format, NULL, &r, &g, &b);

                    r = clamp((int)(r * rSlider.value));
                    g = clamp((int)(g * gSlider.value));
                    b = clamp((int)(b * bSlider.value));

                    pixels[i] = SDL_MapRGB(format, NULL, r, g, b);
                }
            }
            SDL_UnlockSurface(pWindowSurface);

            DrawSlider(pWindowSurface, &rSlider);
            DrawSlider(pWindowSurface, &gSlider);
            DrawSlider(pWindowSurface, &bSlider);

            SDL_UpdateWindowSurface(pWindow);
            needsUpdate = 0;
        }
        SDL_Delay(10);
    }

    SDL_DestroySurface(pImage);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    return 0;
}