/*
 ________  ___  ___  ___  ___  ___  ________           ________  ________  ___               ___       ________      ___    ___ _______   ________
|\   __  \|\  \|\  \|\  \|\  \|\  \|\   ____\         |\   ____\|\   ___ \|\  \             |\  \     |\   __  \    |\  \  /  /|\  ___ \ |\   __  \
\ \  \|\  \ \  \\\  \ \  \ \  \\\  \ \  \___|_        \ \  \___|\ \  \_|\ \ \  \            \ \  \    \ \  \|\  \   \ \  \/  / | \   __/|\ \  \|\  \
 \ \   ____\ \  \\\  \ \  \ \  \\\  \ \_____  \        \ \_____  \ \  \ \\ \ \  \            \ \  \    \ \   __  \   \ \    / / \ \  \_|/_\ \   _  _\
  \ \  \___|\ \  \\\  \ \  \ \  \\\  \|____|\  \        \|____|\  \ \  \_\\ \ \  \____        \ \  \____\ \  \ \  \   \/  /  /   \ \  \_|\ \ \  \\  \|
   \ \__\    \ \_______\ \__\ \_______\____\_\  \         ____\_\  \ \_______\ \_______\       \ \_______\ \__\ \__\__/  / /      \ \_______\ \__\\ _\
    \|__|     \|_______|\|__|\|_______|\_________\       |\_________\|_______|\|_______|        \|_______|\|__|\|__|\___/ /        \|_______|\|__|\|__|
                                      \|_________|       \|_________|                                              \|___|/
                                                                        Puius SDL Layer
  Puius SDL Layer acts as a support for making games in SDL2, using as little libraries as possible.
  It provides multiple structs and functions, such as simple collision functions and drawing functions.
  Some of the functions it contains:
   * Simplistic GUI support;
   * Simple collision functions;
   * Different structs (Vectors, rectProperties etc.);
   * Drawing functions

   The project is licensed under MIT license.
*/

#ifndef SDL_LAYER
#define SDL_LAYER

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

struct Vector2 {
  float X;
  float Y;
};

struct Color3 {
  int R;
  int G;
  int B;
};

struct rectProperties {
  struct Vector2 Position;
  struct Vector2 Size;
  struct Vector2 Velocity;
  struct Color3 Color;

  int Active;
};

struct guiProperties {
  struct Vector2 Position;
  struct Vector2 Size;
  struct Color3 Color;
  char *Text;

  int Pressed;
  int Active;
  int Hovered;
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

void DrawRectangleRec(struct rectProperties rectangle);
void initLayer(SDL_Renderer *renderer, SDL_Window *window);
void initVector2(struct Vector2 *Vector, float X, float Y);
void initRect(struct rectProperties *rectanglePointer, struct Vector2 Position, struct Vector2 Size, struct Vector2 Velocity, struct Color3 Color, int active);
void processInput(struct inputStruct *keys);

int CollisionRect(struct rectProperties body1, struct rectProperties body2);

float lerp(float flGoal, float flCurrent, float deltaTime);

// GUI related
void createButton(struct rectProperties rectangle, char *string);
void renderGUI();

extern struct guiProperties guiArray[100];
extern int lastGUI_item;

#endif
