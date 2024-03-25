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

MIT License

Copyright (c) 2024 SuperPuiu

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

VERSION 0.0.6:
* Fixed MultiLine bug;
* Fixed MouseEnter and MouseLeave callbacks being inverted;
* Fixed Active property of GuiProperties;
* Improved the test.c file;
* Renamed some GUI_TYPE elements;
* Removed ImageButton;
* Removed default hover callbacks;
*/

#ifndef PUIUS_GUI
#define PUIUS_GUI

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <stdbool.h>

/* Definitions */
#define DEFAULT_ELEMENT_WIDTH 70
#define DEFAULT_ELEMENT_HEIGHT 25

typedef enum GUI_TYPE {
    TEXTLABEL,
    TEXTBOX,
    BUTTON,
    IMAGELABEL,
    SCROLLFRAME,
} GUI_TYPE;

typedef enum LIST_TYPE {
    UILIST
} LIST_TYPE;

typedef enum TEXT_XALIGNMENT {
    LEFT,
    X_CENTER,
    RIGHT,
} TEXT_XALIGNMENT;

typedef enum TEXT_YALIGNMENT {
    TOP,
    Y_CENTER,
    BOTTOM,
} TEXT_YALIGNMENT;

typedef enum FILL_DIRECTION {
    UP,
    DOWN,
    DLEFT, /* DirectionLeft */
    DRIGHT /* DirectionRight */
} FILL_DIRECTION;

typedef enum SORT_ORDER {
    LAYOUTORDER,
    INDEX
} SORT_ORDER;

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
  SDL_Renderer *AssignedRenderer;

  GUI_TYPE Type;
  TEXT_XALIGNMENT TextXAlignment;
  TEXT_YALIGNMENT TextYAlignment;

  bool Pressed;
  bool Active;
  bool Hovered;
  bool TextEditable;
  bool Focused;

  bool MultiLine;
  bool TextFits;
  bool TextScaled;
  bool TextWrapped;
  int TextSize;

  void (*MouseDown)(int GuiIndex);
  void (*MouseUp)(int GuiIndex);
  void (*MouseEnter)(int GuiIndex);
  void (*MouseLeave)(int GuiIndex);
  void (*FocusLost)(int GuiIndex);

  TTF_Font *Font;

  SDL_Rect TextRectangle;
  SDL_Texture *TextureText;
  char *Text;
};

struct ListProperties {
    LIST_TYPE Type;

    int Parent;

    int PaddingX;
    int PaddingY;

    bool Wraps;

    FILL_DIRECTION Direction;
    SORT_ORDER SortOrder;
};

void DrawRectangleRec(struct GuiProperties *GUI);

int InitGUI(SDL_Renderer *Renderer, char *FontPath, int FontSize);
int ChangeDefaultFont(char *FontName, int FontSize);
void UninitGUI();

void ProcessInput();

struct Color3 InitColor3(int R, int G, int B, int A);

struct GuiProperties* PSConstructGUI(GUI_TYPE GUI, int X, int Y, int Width, int Height);
struct GuiProperties* PConstructGUI(GUI_TYPE GUI, int X, int Y);
struct ListProperties* ConstructList(LIST_TYPE Type, int Parent, int PaddingX, int PaddingY);

void RenderGUI();
void UpdateGUI(int GUI_Index);
void UpdateAllGUI();

extern struct GuiProperties *GuiArray[100];
extern struct ListProperties *ListArray[100];
extern int LastGUI_item;
extern int LastList;
extern int CurrentGUI_Focused;
extern bool Inputs[258];
extern bool Running;
extern bool IsFocused;

#endif
