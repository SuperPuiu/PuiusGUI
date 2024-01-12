/*
 ________  ___  ___  ___  ___  ___  ________           ________  ___  ___  ___     
|\   __  \|\  \|\  \|\  \|\  \|\  \|\   ____\         |\   ____\|\  \|\  \|\  \    
\ \  \|\  \ \  \\\  \ \  \ \  \\\  \ \  \___|_        \ \  \___|\ \  \\\  \ \  \   
 \ \   ____\ \  \\\  \ \  \ \  \\\  \ \_____  \        \ \  \  __\ \  \\\  \ \  \  
  \ \  \___|\ \  \\\  \ \  \ \  \\\  \|____|\  \        \ \  \|\  \ \  \\\  \ \  \ 
   \ \__\    \ \_______\ \__\ \_______\____\_\  \        \ \_______\ \_______\ \__\
    \|__|     \|_______|\|__|\|_______|\_________\        \|_______|\|_______|\|__|
                                      \|_________|                                 
                                                                                   
  Puius SDL Layer acts as a support for making games in SDL2, having very little dependencies.
It provides multiple structs and functions, such as simple collision functions and drawing functions, generally everything required for video games development.
List of provided functions:
* TextButtons, ImageButtons, TextBoxes, Frames, constructors;
* Structs such as Vector2, Color3, rectProperties, inputStruct;
* handleInput() for easy handling of input;
* DrawRectangleRec, DrawRectangleRecEx for drawing;
* CollisionRect, CollisionRectPoint for basic collision;
* Lerp for smooth value changing;

The project is licensed under MIT license. SDL_image.h, SDL.h and SDL_ttf.h are required to compile.
*/

#ifndef SDL_LAYER
#define SDL_LAYER

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

enum GUI_TYPE {
    TEXTLABEL,
    TEXTBOX,
    TEXTBUTTON,
    IMAGELABEL,
    IMAGEBUTTON,
};

enum TEXT_ALIGMENT {
    CENTERED,
    LEFT,
    RIGHT,
};

struct Color3 {
  int R;
  int G;
  int B;
};

struct guiProperties {
  int PositionX; int PositionY;
  int SizeX; int SizeY;

  struct Color3 BackgroundColor;
  struct Color3 TextColor;

  SDL_Texture *Image;

  enum GUI_TYPE Type;
  enum TEXT_ALIGMENT TextAligment;

  int Pressed;
  int Active;
  int Hovered;
  int TextEditable;
  int Focused;

  SDL_Texture *TextureText;
  char *Text;
};

struct inputStruct {
  int A; int N;
  int B; int O;
  int C; int P;
  int D; int Q;
  int E; int R;
  int F; int S;
  int G; int T;
  int H; int U;
  int I; int V;
  int J; int W;
  int K; int X;
  int L; int Y;
  int M; int Z;

  int ESC; int SDL_QUIT;
  int LEFT_BUTTON; int RIGHT_BUTTON;
};

void DrawRectangleRec(struct guiProperties rectangle);
int initLayer(SDL_Renderer *renderer, SDL_Window *window);
void uninitLayer();
void processInput(struct inputStruct *keys);

struct Color3 initColor3(int R, int G, int B);

// GUI related
int ConstructGUI(enum GUI_TYPE);
void renderGUI();
void updateGUI(int GUI_Index);
void updateAllGUI();

extern struct guiProperties guiArray[100];
extern int lastGUI_item;

#endif
