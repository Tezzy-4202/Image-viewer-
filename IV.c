#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static char pendingFile[1024] = {0};

typedef struct
{
    SDL_Rect rect;
    float value;
    Uint32 color;
} Slider;

typedef struct
{
    SDL_Rect rect;
    Uint32 color;
} Button;

Uint8 clamp(int value)
{
    if (value < 0)
        return 0;
    if (value > 255)
        return 255;
    return (Uint8)value;
}

void DrawSlider(SDL_Surface *dst, Slider *s)
{
    SDL_FillSurfaceRect(dst, &s->rect, 0xFF333333);

    SDL_Rect fillRect = s->rect;
    fillRect.w = (int)(s->rect.w * s->value);
    SDL_FillSurfaceRect(dst, &fillRect, s->color);

    SDL_Rect knob = {fillRect.x + fillRect.w - 5, fillRect.y - 2, 10, fillRect.h + 4};
    SDL_FillSurfaceRect(dst, &knob, 0xFFFFFFFF);
}

void DrawSaveButton(SDL_Surface *dst, Button *b)
{
    SDL_FillSurfaceRect(dst, &b->rect, 0xFF555555);
    SDL_Rect inner = {b->rect.x + 5, b->rect.y + 5, b->rect.w - 10, b->rect.h - 10};
    SDL_FillSurfaceRect(dst, &inner, b->color);

    int startX = b->rect.x + (b->rect.w - 65) / 2;
    int startY = b->rect.y + (b->rect.h - 15) / 2;
    Uint32 txtColor = 0xFF000000;

    SDL_Rect s1 = {startX, startY, 15, 3};
    SDL_Rect s2 = {startX, startY, 3, 8};
    SDL_Rect s3 = {startX, startY + 7, 15, 3};
    SDL_Rect s4 = {startX + 12, startY + 7, 3, 8};
    SDL_Rect s5 = {startX, startY + 14, 15, 3};
    SDL_FillSurfaceRect(dst, &s1, txtColor);
    SDL_FillSurfaceRect(dst, &s2, txtColor);
    SDL_FillSurfaceRect(dst, &s3, txtColor);
    SDL_FillSurfaceRect(dst, &s4, txtColor);
    SDL_FillSurfaceRect(dst, &s5, txtColor);

    startX += 20;

    SDL_Rect a1 = {startX, startY, 3, 17};
    SDL_Rect a2 = {startX, startY, 15, 3};
    SDL_Rect a3 = {startX + 12, startY, 3, 17};
    SDL_Rect a4 = {startX, startY + 7, 15, 3};
    SDL_FillSurfaceRect(dst, &a1, txtColor);
    SDL_FillSurfaceRect(dst, &a2, txtColor);
    SDL_FillSurfaceRect(dst, &a3, txtColor);
    SDL_FillSurfaceRect(dst, &a4, txtColor);

    startX += 20;

    SDL_Rect v1 = {startX, startY, 3, 14};
    SDL_Rect v2 = {startX + 12, startY, 3, 14};
    SDL_Rect v3 = {startX, startY + 14, 15, 3};
    SDL_FillSurfaceRect(dst, &v1, txtColor);
    SDL_FillSurfaceRect(dst, &v2, txtColor);
    SDL_FillSurfaceRect(dst, &v3, txtColor);

    startX += 20;

    SDL_Rect e1 = {startX, startY, 3, 17};
    SDL_Rect e2 = {startX, startY, 15, 3};
    SDL_Rect e3 = {startX, startY + 7, 12, 3};
    SDL_Rect e4 = {startX, startY + 14, 15, 3};
    SDL_FillSurfaceRect(dst, &e1, txtColor);
    SDL_FillSurfaceRect(dst, &e2, txtColor);
    SDL_FillSurfaceRect(dst, &e3, txtColor);
    SDL_FillSurfaceRect(dst, &e4, txtColor);
}

void DrawUploadButton(SDL_Surface *dst, Button *b)
{
    SDL_FillSurfaceRect(dst, &b->rect, 0xFF555555);
    SDL_Rect inner = {b->rect.x + 5, b->rect.y + 5, b->rect.w - 10, b->rect.h - 10};
    SDL_FillSurfaceRect(dst, &inner, b->color);

    int startX = b->rect.x + (b->rect.w - 35) / 2;
    int startY = b->rect.y + (b->rect.h - 15) / 2;
    Uint32 txtColor = 0xFF000000;

    SDL_Rect u1 = {startX, startY, 3, 14};
    SDL_Rect u2 = {startX + 12, startY, 3, 14};
    SDL_Rect u3 = {startX, startY + 14, 15, 3};
    SDL_FillSurfaceRect(dst, &u1, txtColor);
    SDL_FillSurfaceRect(dst, &u2, txtColor);
    SDL_FillSurfaceRect(dst, &u3, txtColor);

    startX += 20;

    SDL_Rect p1 = {startX, startY, 3, 17};
    SDL_Rect p2 = {startX, startY, 15, 3};
    SDL_Rect p3 = {startX + 12, startY, 3, 8};
    SDL_Rect p4 = {startX, startY + 7, 15, 3};
    SDL_FillSurfaceRect(dst, &p1, txtColor);
    SDL_FillSurfaceRect(dst, &p2, txtColor);
    SDL_FillSurfaceRect(dst, &p3, txtColor);
    SDL_FillSurfaceRect(dst, &p4, txtColor);
}

int UpdateSlider(Slider *s, float mouseX, float mouseY, int isMouseDown)
{
    if (isMouseDown &&
        mouseX >= s->rect.x && mouseX <= (s->rect.x + s->rect.w) &&
        mouseY >= s->rect.y && mouseY <= (s->rect.y + s->rect.h))
    {

        s->value = (mouseX - s->rect.x) / (float)s->rect.w;
        return 1;
    }
    return 0;
}

int IsButtonClick(Button *b, float mouseX, float mouseY)
{
    if (mouseX >= b->rect.x && mouseX <= (b->rect.x + b->rect.w) &&
        mouseY >= b->rect.y && mouseY <= (b->rect.y + b->rect.h))
    {
        return 1;
    }
    return 0;
}

void SDLCALL FileOpenCallback(void *userdata, const char *const *filelist, int filter)
{
    if (filelist && filelist[0])
    {
        snprintf(pendingFile, sizeof(pendingFile), "%s", filelist[0]);
    }
}

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        return 1;

    SDL_Surface *pImage = SDL_LoadBMP("test.bmp");
    if (!pImage)
        return 1;

    int sidebarWidth = 250;
    int maxImageW = 800;
    int maxImageH = 600;

    int imgW = pImage->w;
    int imgH = pImage->h;
    if (imgW > maxImageW)
    {
        float scale = (float)maxImageW / imgW;
        imgW = maxImageW;
        imgH = (int)(imgH * scale);
    }
    if (imgH > maxImageH)
    {
        float scale = (float)maxImageH / imgH;
        imgH = maxImageH;
        imgW = (int)(imgW * scale);
    }

    int windowW = imgW + sidebarWidth;
    int windowH = (imgH > 400) ? imgH : 400;

    SDL_Window *pWindow = SDL_CreateWindow("Vibe Mixer", windowW, windowH, 0);
    if (!pWindow)
        return 1;
    SDL_Surface *pWindowSurface = SDL_GetWindowSurface(pWindow);
    const SDL_PixelFormatDetails *format = SDL_GetPixelFormatDetails(pWindowSurface->format);

    Slider rSlider = {{imgW + 25, 50, 200, 30}, 1.0f, SDL_MapSurfaceRGB(pWindowSurface, 255, 0, 0)};
    Slider gSlider = {{imgW + 25, 100, 200, 30}, 1.0f, SDL_MapSurfaceRGB(pWindowSurface, 0, 255, 0)};
    Slider bSlider = {{imgW + 25, 150, 200, 30}, 1.0f, SDL_MapSurfaceRGB(pWindowSurface, 0, 0, 255)};

    Button saveBtn = {{imgW + 25, 250, 200, 50}, SDL_MapSurfaceRGB(pWindowSurface, 0, 255, 0)};
    Button upBtn = {{imgW + 25, 320, 200, 50}, SDL_MapSurfaceRGB(pWindowSurface, 255, 255, 0)};

    int running = 1;
    int needsUpdate = 1;
    int mouseBtnDown = 0;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = 0;
            }
            else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
            {
                mouseBtnDown = 1;
                float mx, my;
                SDL_GetMouseState(&mx, &my);

                if (IsButtonClick(&saveBtn, mx, my))
                {
                    SDL_Surface *saveSurface = SDL_CreateSurface(imgW, imgH, pWindowSurface->format);
                    SDL_Rect srcRect = {0, 0, imgW, imgH};
                    SDL_BlitSurface(pWindowSurface, &srcRect, saveSurface, NULL);
                    if (SDL_SaveBMP(saveSurface, "saved_vibe.bmp"))
                    {
                        SDL_Log("Saved!");
                    }
                    SDL_DestroySurface(saveSurface);
                }
                else if (IsButtonClick(&upBtn, mx, my))
                {
                    SDL_ShowOpenFileDialog(FileOpenCallback, NULL, pWindow, NULL, 0, NULL, 0);
                }
            }
            else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP)
            {
                mouseBtnDown = 0;
            }
        }

        if (pendingFile[0] != '\0')
        {
            SDL_Surface *newImg = SDL_LoadBMP(pendingFile);
            if (newImg)
            {
                SDL_DestroySurface(pImage);
                pImage = newImg;

                imgW = pImage->w;
                imgH = pImage->h;
                if (imgW > maxImageW)
                {
                    float scale = (float)maxImageW / imgW;
                    imgW = maxImageW;
                    imgH = (int)(imgH * scale);
                }
                if (imgH > maxImageH)
                {
                    float scale = (float)maxImageH / imgH;
                    imgH = maxImageH;
                    imgW = (int)(imgW * scale);
                }

                windowW = imgW + sidebarWidth;
                windowH = (imgH > 400) ? imgH : 400;
                SDL_SetWindowSize(pWindow, windowW, windowH);
                pWindowSurface = SDL_GetWindowSurface(pWindow);

                rSlider.rect.x = imgW + 25;
                gSlider.rect.x = imgW + 25;
                bSlider.rect.x = imgW + 25;
                saveBtn.rect.x = imgW + 25;
                upBtn.rect.x = imgW + 25;

                needsUpdate = 1;
            }
            pendingFile[0] = '\0';
        }

        float mx, my;
        SDL_GetMouseState(&mx, &my);

        if (mouseBtnDown)
        {
            if (UpdateSlider(&rSlider, mx, my, 1))
                needsUpdate = 1;
            if (UpdateSlider(&gSlider, mx, my, 1))
                needsUpdate = 1;
            if (UpdateSlider(&bSlider, mx, my, 1))
                needsUpdate = 1;
        }

        if (needsUpdate)
        {
            SDL_FillSurfaceRect(pWindowSurface, NULL, 0xFF000000);

            SDL_Rect destRect = {0, 0, imgW, imgH};
            SDL_BlitSurfaceScaled(pImage, NULL, pWindowSurface, &destRect, SDL_SCALEMODE_LINEAR);

            SDL_LockSurface(pWindowSurface);
            Uint32 *pixels = (Uint32 *)pWindowSurface->pixels;
            int pitch = pWindowSurface->pitch / 4;

            for (int y = 0; y < imgH; y++)
            {
                for (int x = 0; x < imgW; x++)
                {
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
            DrawSaveButton(pWindowSurface, &saveBtn);
            DrawUploadButton(pWindowSurface, &upBtn);

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