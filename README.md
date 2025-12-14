# Image-viewer/Vibe-Mixer

I spent the last month doing Machine Learning. I got bored of tensors and Python, so I built this raw Image Manipulator in C and SDL3 to remember what manual memory management feels like.

**Features**
* **RGB Sliders:** Real-time pixel manipulation.
* **Custom UI:** Buttons and sliders drawn pixel-by-pixel from scratch.
* **Upload & Save:** Load BMPs dynamically and export your work.

**How it Works**
There is no AI here. It locks the window surface, iterates through the pixel array, and modifies RGB values via bitwise operations.

**Build & Run**
Just run the included `.bat` file.

<img width="1055" height="532" alt="image" src="https://github.com/user-attachments/assets/f348a8f5-76c7-4448-9df2-5229ce6d31d9" />
