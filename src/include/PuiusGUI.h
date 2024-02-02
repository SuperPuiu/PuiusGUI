/*
 ________  ___  ___  ___  ___  ___  ________           ________  ___  ___  ___
|\   __  \|\  \|\  \|\  \|\  \|\  \|\   ____\         |\   ____\|\  \|\  \|\  \
\ \  \|\  \ \  \\\  \ \  \ \  \\\  \ \  \___|_        \ \  \___|\ \  \\\  \ \  \
 \ \   ____\ \  \\\  \ \  \ \  \\\  \ \_____  \        \ \  \  __\ \  \\\  \ \  \
  \ \  \___|\ \  \\\  \ \  \ \  \\\  \|____|\  \        \ \  \|\  \ \  \\\  \ \  \
   \ \__\    \ \_______\ \__\ \_______\____\_\  \        \ \_______\ \_______\ \__\
    \|__|     \|_______|\|__|\|_______|\_________\        \|_______|\|_______|\|__|
                                      \|_________|
 PuiusGUI is a small GUI library which is made around SDL2, made out of two files.
It provides one Color3 struct, and multiple GUI objects.
The project is licensed under MIT license. SDL_image.h, SDL.h and SDL_ttf.h are required to compile.
*/

#ifndef SDL_LAYER
#define SDL_LAYER

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <stdbool.h>

enum GUI_TYPE {
    TEXTLABEL,
    TEXTBOX,
    TEXTBUTTON,
    IMAGELABEL,
    IMAGEBUTTON,
};

enum TEXT_XALIGNMENT {
    X_CENTER,
    LEFT,
    RIGHT,
};

enum TEXT_YALIGNMENT {
    TOP,
    Y_CENTER,
    BOTTOM,
};

struct Color3 {
  int R;
  int G;
  int B;
  int A;
};

struct guiProperties {
  int PositionX; int PositionY;
  int SizeX; int SizeY;

  int BorderSize;
  int OutlineSize;
  bool Visible;

  struct Color3 BackgroundColor;
  struct Color3 TextColor;
  struct Color3 BorderColor;

  SDL_Texture *Image;

  enum GUI_TYPE Type;
  enum TEXT_XALIGNMENT TextXAlignment;
  enum TEXT_YALIGNMENT TextYAlignment;

  bool Pressed;
  bool Active;
  bool Hovered;
  bool TextEditable;
  bool Focused;

  bool TextFits;
  bool TextScaled;
  bool TextWrapped;
  int TextSize;

  void (*MouseDown)(int guiIndex);
  void (*MouseEnter)(int guiIndex);
  void (*MouseLeave)(int guiIndex);
  void (*FocusLost)(int guiIndex);

  TTF_Font *Font;

  SDL_Rect TextRectangle;
  SDL_Texture *TextureText;
  char *Text;
};

void DrawRectangleRec(struct guiProperties rectangle);

int initLayer(SDL_Renderer *renderer, SDL_Window *window);
int changeDefaultFont(char *fontName, int fontSize);
void uninitLayer();

void processInput();

struct Color3 initColor3(int R, int G, int B, int A);

int ConstructGUI(enum GUI_TYPE);
void renderGUI();
void updateGUI(int GUI_Index);
void updateAllGUI();

extern struct guiProperties guiArray[100];
extern int lastGUI_item;
extern int isFocused;
extern int currentGUI_Focused;
extern int inputs[258];
extern int running;

#endif
