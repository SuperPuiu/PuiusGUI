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
    UILIST, /* Used for automatic placement of GUI elements in a parent. */
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

/* Pre-made Color3 structures */

extern struct Color3 WHITE;
extern struct Color3 BLACK;
extern struct Color3 RED;
extern struct Color3 BLUE;
extern struct Color3 LIME;
extern struct Color3 GRAY;
extern struct Color3 VIOLET;

/* Pre-made Color3 structures */
struct GuiProperties {
  int PositionX; int PositionY;
  int SizeX; int SizeY;

  int Parent;
  int Zindex;
  int BodyIndex;

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

  void (*MouseDown)(int GuiIndex);
  void (*MouseEnter)(int GuiIndex);
  void (*MouseLeave)(int GuiIndex);
  void (*FocusLost)(int GuiIndex);

  TTF_Font *Font;

  SDL_Rect TextRectangle;
  SDL_Texture *TextureText;
  char *Text;
};

void DrawRectangleRec(struct GuiProperties *GUI);

int InitLayer(SDL_Renderer *renderer, SDL_Window *window);
int ChangeDefaultFont(char *FontName, int FontSize);
void UninitLayer();

void ProcessInput();

struct Color3 InitColor3(int R, int G, int B, int A);

struct GuiProperties* ConstructGUI(enum GUI_TYPE GUI, int X, int Y);
void RenderGUI();
void UpdateGUI(int GUI_Index);
void UpdateAllGUI();

extern struct GuiProperties *GuiArray[100];
extern int LastGUI_item;
extern int IsFocused;
extern int CurrentGUI_Focused;
extern bool Inputs[258];
extern int Running;

#endif
