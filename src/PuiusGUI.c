#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "include/PuiusGUI.h"

/* Color3s */
struct Color3 WHITE;
struct Color3 BLACK;
struct Color3 RED;
struct Color3 BLUE;
struct Color3 LIME;
struct Color3 GRAY;
struct Color3 VIOLET;

struct Color3 DefaultBackgroundColor;
struct Color3 DefaultTextColor;
struct Color3 DefaultBorderColor;
TEXT_XALIGNMENT DefaultX_Alignment;
TEXT_YALIGNMENT DefaultY_Alignment;

SDL_Renderer *AssignedRenderer;

struct GuiProperties *GuiArray[100];
struct ListProperties *ListArray[100];

bool Inputs[258];
bool Running = true;
bool LeftButtonDown = false;
bool IsFocused;

int CurrentList = -1, LastElement = -1;
int LastList = -1;
int LastGUI_item = -1;
int TabSize = 4; /* How many spaces are added for a tab */
int MouseY, MouseX;
int CurrentGUI_Focused = -1;
size_t Cursor = 0;

TTF_Font *Font;

void defaultCallback() {} // Used for ConstructGUI()

struct Color3 InitColor3(int R, int G, int B, int A) {
  struct Color3 Color;

  Color.R = R;
  Color.B = B;
  Color.G = G;
  Color.A = A;

  return Color;
}

bool CollisionGUIs(struct GuiProperties *GUI_1, struct GuiProperties *GUI_2) {
  // return !(r1.x + r1.width < r2.x || r1.y + r1.height < r2.y || r1.x > r2.x + r2.width || r1.y > r2.y + r2.height);
  return !(GUI_1->PositionX + GUI_1->SizeX < GUI_2->PositionX || GUI_1->PositionY + GUI_1->SizeY < GUI_2->PositionY || GUI_1->PositionX > GUI_2->PositionX + GUI_2->SizeX || GUI_1->PositionY > GUI_2->PositionY + GUI_2->SizeY);
}

SDL_Rect CollisionArea(struct GuiProperties *GUI_1, struct GuiProperties *GUI_2) {
  int LeftX, RightX, TopY, BottomY;
  SDL_Rect NewRectangle;

  LeftX = fmax(GUI_1->PositionX, GUI_2->PositionY);
  RightX = fmin(GUI_1->PositionX + GUI_1->SizeX, GUI_2->PositionX + GUI_2->SizeX);
  TopY = fmax(GUI_1->PositionY, GUI_2->PositionY);
  BottomY = fmin(GUI_1->PositionY + GUI_1->SizeY, GUI_2->PositionY + GUI_2->SizeY);
  
  // NewRectangle = {LeftX, TopY, RightX - Left, BottomY - TopY};
  NewRectangle.x = LeftX;
  NewRectangle.y = TopY;
  NewRectangle.w = RightX - LeftX;
  NewRectangle.h = BottomY - TopY;

  return NewRectangle;
}

SDL_Point InitPoint(int x, int y) {
  SDL_Point point;
  point.x = x;
  point.y = y;

  return point;
}

int CollosionRectPoint(struct GuiProperties *GUI, int PointX, int PointY) {
  struct GuiProperties Rect = *GUI;

  if (PointX > Rect.PositionX && PointX < Rect.PositionX + Rect.SizeX && PointY > Rect.PositionY && PointY < Rect.PositionY + Rect.SizeY)
    return true;
  return false;
}

void GUI_INTERNAL_ERROR(const char *error) {
#ifndef SUPPRESS_GUI_ERRORS
  printf("\x1b[31m%s\x1b[0m", error);
  printf("%s\n", SDL_GetError());
#endif
}

void DrawRectangleRec(SDL_Rect Rectangle, struct Color3 Color, SDL_Renderer *Renderer) {
  /* Set the color to the specified color, draw the rectangle, reset the color. */
  if (Renderer) {
    SDL_Color oldColor = {0, 0, 0, 0};
    SDL_GetRenderDrawColor(Renderer, &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);

    SDL_SetRenderDrawColor(Renderer, Color.R, Color.G, Color.B, Color.A);
    SDL_RenderFillRect(Renderer, &Rectangle);
    SDL_SetRenderDrawColor(Renderer, oldColor.r, oldColor.g, oldColor.b, oldColor.a);

  } else
  GUI_INTERNAL_ERROR("[PUIUS GUI] Unable to DrawRectangleRec without GUI->AssignedRenderer. Did you initialize the library?\n");
}

void DrawLine(int StartX, int StartY, int EndX, int EndY, struct Color3 Color, SDL_Renderer *AssignedRenderer) {
  /* Set the color to the specified color, draw the line, reset the color. */
  SDL_Color oldColor = {0, 0, 0, 0};
  SDL_GetRenderDrawColor(AssignedRenderer, &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);

  SDL_SetRenderDrawColor(AssignedRenderer, Color.R, Color.G, Color.B, 255);
  SDL_RenderDrawLine(AssignedRenderer, StartX, StartY, EndX, EndY);
  SDL_SetRenderDrawColor(AssignedRenderer, oldColor.r, oldColor.g, oldColor.b, oldColor.a);
}

void DrawText(struct GuiProperties *GUI) {
  struct GuiProperties gui = *GUI;

  if (GUI->AssignedRenderer) {
    SDL_Color oldColor = {0, 0, 0, 0};
    SDL_GetRenderDrawColor(GUI->AssignedRenderer, &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);

    SDL_SetRenderDrawColor(GUI->AssignedRenderer, gui.TextColor.R, gui.TextColor.G, gui.TextColor.B, gui.TextColor.A);
    SDL_RenderCopy(GUI->AssignedRenderer, gui.TextureText, NULL, &gui.TextRectangle);
    SDL_SetRenderDrawColor(GUI->AssignedRenderer, oldColor.r, oldColor.g, oldColor.b, oldColor.a);
  }
}

void DrawTextureEx(struct GuiProperties *GUI) {
  if (!GUI->Image)
    GUI_INTERNAL_ERROR("[PUIUS GUI] GUI element contains no image property! Did you forget to add the image pointer?\n");

  if (GUI->AssignedRenderer) {
    SDL_Color OldColor = {0, 0, 0, 0};
    SDL_GetRenderDrawColor(GUI->AssignedRenderer, &OldColor.r, &OldColor.g, &OldColor.b, &OldColor.a);

    int Texture_x = 0, Texture_y = 0;

    if (SDL_QueryTexture(GUI->Image, NULL, NULL, &Texture_x, &Texture_y) != 0)
      GUI_INTERNAL_ERROR("[PUIUS GUI] SDL_QueryTexture failed! SDL Error:");

    SDL_Rect NewRect = {GUI->PositionX, GUI->PositionY, GUI->SizeX, GUI->SizeY};
    SDL_Rect Part = {0, 0, Texture_x, Texture_y};

    SDL_SetRenderDrawColor(GUI->AssignedRenderer, GUI->BackgroundColor.R, GUI->BackgroundColor.G, GUI->BackgroundColor.B, GUI->BackgroundColor.A);
    SDL_RenderCopyEx(GUI->AssignedRenderer, GUI->Image, &Part, &NewRect, 0, NULL, SDL_FLIP_NONE);
    SDL_SetRenderDrawColor(GUI->AssignedRenderer, OldColor.r, OldColor.g, OldColor.b, OldColor.a);

  } else
  GUI_INTERNAL_ERROR("[PUIUS GUI] Tried drawing a texture without an AssignedRenderer. Have you initialized the library?");
}

void WriteToTextBox(char *str) {
  if(IsFocused == false)
    return;

  if(GuiArray[CurrentGUI_Focused]->TextEditable == false)
    return;
  // printf("%i\n", Cursor);
  struct GuiProperties *GUI = GuiArray[CurrentGUI_Focused];

  int textLength = strlen(GUI->Text);
  char *alloc;

  alloc = malloc(textLength * 2);

  if (alloc == NULL) {
    GUI_INTERNAL_ERROR("[PUIUS GUI] Failed to allocate memory for new string when writing to the focused textbox.");
    UninitGUI();
    exit(1);
  }

  if (strcmp(str, "ENTER") == 0) {
    if (GUI->MultiLine == false) {
      /* If MultiLine is false, just unfocus from the box. */
      strcpy(alloc, GUI->Text);
      GuiArray[CurrentGUI_Focused]->FocusLost(CurrentGUI_Focused);
      IsFocused = false;
    } else {
      /* Else, we have to add a newline character. */
      /* To do so, we loop over the entire string and move the characters accordingly. */ 
      for (size_t i = 0; i <= strlen(alloc); i++) {
        /* If the cursor position is equal to i, then insert to the new buffer \n at cursor position and move 
         * the old character by one. */
        if (i == Cursor) {
          alloc[i] = '\n';
          alloc[i + 1] = GUI->Text[i];
          continue;
        /* If i is larger than the cursor position then move the next character by one 
         * as in the previous statement we inserted \n at cursor position and moved the rest of the string by one */
        } else if (i > Cursor) {
          alloc[i + 1] = GUI->Text[i];
        /* If it's below cursor then copy the text as it is. */
        } else {
          alloc[i] = GUI->Text[i];
        }
      };
      
      /* Move the cursor after the newline character. */
      Cursor = Cursor + 1;
    }
  }
  else if(strcmp(str, "BACKSPACE") == 0) {
    /* Copy the string and then for every character which is after Cursor - 1 replace with the next character, thus 
     * removing one character. */
    strcpy(alloc, GUI->Text);
    for (size_t i = (Cursor - 1); alloc[i] != '\0'; i++)
      alloc[i] = alloc[i + 1];
    
    /* If the cursor is above 0, move it one character behind */
    if (Cursor > 0)
      Cursor -= 1;
  }
  else {
    /* If no special character was detected, insert the character normally. 
     * Inserting a character goes like inserting a newline character.*/
    for (size_t i = 0; i <= strlen(alloc); i++) {
      if (i == Cursor) {
        alloc[i] = str[0];
        alloc[i + 1] = GUI->Text[i];
        continue;
      } else if (i > Cursor) {
        alloc[i + 1] = GUI->Text[i];
      } else {
        alloc[i] = GUI->Text[i];
      }
    };

    Cursor = Cursor + 1;
  }
  /* Free the old text and update the GUI. */
  free(GUI->Text);
  
  GUI->Text = alloc;
  UpdateGUI(CurrentGUI_Focused, true);
}

void ScrollFrame(int WheelSpeed) { 
  for (int i = 0; i <= LastGUI_item; i++) {
    bool isColliding = CollosionRectPoint(GuiArray[i], MouseX, MouseY);

    if (isColliding && GuiArray[i]->ScrollActive == true && GuiArray[i]->Visible == true) {
      printf("X: %i, Y: %i\n", GuiArray[i]->CavnasX, GuiArray[i]->CavnasY);
      if (GuiArray[i]->HorizontalScrolling == false)
        GuiArray[i]->CavnasY -= WheelSpeed;
      else
        GuiArray[i]->CavnasX += WheelSpeed;
    }

    for (int j = 0; j <= LastGUI_item; j++)
      if (GuiArray[j]->Parent == i)
        UpdateGUI(j, false);
  }
}

void ProcessInput() {
  SDL_Event event;

  while(SDL_PollEvent(&event)) {
    switch(event.type) {
      case SDL_QUIT:
        Running = false;
        break;
      case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_RETURN)
          WriteToTextBox("ENTER");
        else if (event.key.keysym.sym == SDLK_BACKSPACE)
          WriteToTextBox("BACKSPACE");
        else if (event.key.keysym.sym == SDLK_LEFT && IsFocused) {
          if (Cursor >= 1)
            Cursor--;
        }
        else if (event.key.keysym.sym == SDLK_RIGHT && IsFocused) {
          if (Cursor < strlen(GuiArray[CurrentGUI_Focused]->Text))
            Cursor++;
        }

        Inputs[event.key.keysym.scancode] = true;
        break;
      case SDL_KEYUP:
        Inputs[event.key.keysym.scancode] = false;
        break;
      case SDL_MOUSEWHEEL:
        ScrollFrame(event.wheel.y);
        break;
      case SDL_MOUSEMOTION:
        SDL_GetMouseState(&MouseX, &MouseY);
        break;
      case SDL_MOUSEBUTTONUP:
        if (event.button.button == SDL_BUTTON_LEFT) {
          Inputs[257] = false;
          LeftButtonDown = false;
        } else if (event.button.button == SDL_BUTTON_RIGHT)
          Inputs[258] = false;

        break;
      case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == SDL_BUTTON_LEFT) {
          Inputs[257] = true;

          if (IsFocused) {
            IsFocused = false;
            GuiArray[CurrentGUI_Focused]->FocusLost(CurrentGUI_Focused);
          }

          LeftButtonDown = true;
        } else if (event.button.button == SDL_BUTTON_RIGHT)
          Inputs[258] = true;

        break;
      case SDL_TEXTINPUT:
        WriteToTextBox(event.text.text);
    }
  }
}

int ParentHasList(int GUI_Index) {
  for (int i = 0; i <= LastList; i++)
    if (GuiArray[GUI_Index]->Parent == ListArray[i]->Parent)
      return i;

  return -1;
}

void UpdateGUI(int GUI_Index, bool ClearIndexes) {
  /* Updating the GUI consists of multiple steps:
   * 1. Positioning, where we account for the Parent's position and respect the UIList contained within the element;
   * 2. Update the font and its positions. Also check if the text still fits within the GUI's borders;
   * 3. Report any error which happened while updating the UI.
   * */
  if (ClearIndexes == true) {
    LastElement = -1;
    CurrentList = -1;
  }

  int List = ParentHasList(GUI_Index);
  int success = 1;

  char *error = "empty";
  struct GuiProperties GUI = *GuiArray[GUI_Index];

  SDL_Color Color = {GUI.TextColor.R, GUI.TextColor.G, GUI.TextColor.B, GUI.TextColor.A};
  SDL_Surface *surfaceMessage;

  if (List != CurrentList) {
    LastElement = GUI_Index;
    CurrentList = List;
  }

  /* Positioning */

  if (CurrentList == -1) {
    if (GUI.Parent > -1 && GUI.Parent < LastGUI_item) {
      int Parent = GUI.Parent;

      GuiArray[GUI_Index]->PositionX = GUI.PositionX + GuiArray[Parent]->PositionX;
      GuiArray[GUI_Index]->PositionY = GUI.PositionY + GuiArray[Parent]->PositionY;
    }
  } else {
    int Parent = GUI.Parent;

    if (LastElement == GUI_Index) {
      GuiArray[GUI_Index]->PositionX = GuiArray[Parent]->PositionX + GuiArray[Parent]->CavnasX;
      GuiArray[GUI_Index]->PositionY = GuiArray[Parent]->PositionY + GuiArray[Parent]->CavnasY;
    } else if (ListArray[CurrentList]->Direction == DOWN) {
      GuiArray[GUI_Index]->PositionX = GuiArray[Parent]->PositionX;
      GuiArray[GUI_Index]->PositionY = GuiArray[LastElement]->PositionY + GUI.SizeY + ListArray[CurrentList]->PaddingY;
    } else if (ListArray[CurrentList]->Direction == UP) {
      GuiArray[GUI_Index]->PositionX = GuiArray[Parent]->PositionX + GuiArray[Parent]->CavnasX;
      GuiArray[GUI_Index]->PositionY = GuiArray[LastElement]->PositionY - GUI.SizeY - ListArray[CurrentList]->PaddingY;
    } else if (ListArray[CurrentList]->Direction == DLEFT) {
      GuiArray[GUI_Index]->PositionX = GuiArray[LastElement]->PositionX - GUI.SizeX - ListArray[CurrentList]->PaddingX;
      GuiArray[GUI_Index]->PositionY = GuiArray[Parent]->PositionY + GuiArray[Parent]->CavnasY;
    } else if (ListArray[CurrentList]->Direction == DRIGHT) {
      GuiArray[GUI_Index]->PositionX = GuiArray[LastElement]->PositionX + GUI.SizeX + ListArray[CurrentList]->PaddingX;
      GuiArray[GUI_Index]->PositionY = GuiArray[Parent]->PositionY + GuiArray[Parent]->CavnasY;
    }

    // printf("New X: %i, new Y: %i\n", GuiArray[GUI_Index]->PositionX, GuiArray[GUI_Index]->PositionY);
    LastElement = GUI_Index;
  }

  /*if (GUI.Parent > -1) {
    SDL_Rect Area = CollisionArea(&GUI, GuiArray[GUI.Parent]);
    if (CollisionGUIs(&GUI, GuiArray[GUI.Parent]) == true)
      if (Area.x >= Area.w || Area.y >= Area.h)
      printf("Area X: %i, Area Y: %i, Area width: %i, Area height: %i!\n", Area.x, Area.y, Area.w, Area.h); 
  }*/

  if (!GUI.Font) {
    size_t Bytes = snprintf(NULL, 0, "[PUIUS GUI] GuiArray[%i].Font is inexistent!\n", GUI_Index) + 1;
    error = malloc(Bytes);
    snprintf(error, Bytes, "[PUIUS GUI] GuiArray[%i].Font is inexistent!\n", GUI_Index);
  }

  success = TTF_SetFontSize(GuiArray[GUI_Index]->Font, GUI.TextSize);
  if (success == -1) {
    size_t Bytes = snprintf(NULL, 0, "[PUIUS GUI] TTF_SetFontSize failed for GUI element with %i as index!\n", GUI_Index) + 1;
    error = malloc(Bytes);
    snprintf(error, Bytes, "[PUIUS GUI] TTF_SetFontSize failed for GUI element with %i as index!\n", GUI_Index);
  }

  TTF_SetFontOutline(GuiArray[GUI_Index]->Font, GuiArray[GUI_Index]->OutlineSize);

  if (GUI.TextWrapped)
    surfaceMessage = TTF_RenderText_Blended_Wrapped(GUI.Font, GUI.Text, Color, GUI.SizeX);
  else
    surfaceMessage = TTF_RenderText_Blended_Wrapped(GUI.Font, GUI.Text, Color, 0);

  SDL_Texture *Message = SDL_CreateTextureFromSurface(GuiArray[GUI_Index]->AssignedRenderer, surfaceMessage);
  SDL_GetClipRect(surfaceMessage, &GuiArray[GUI_Index]->TextRectangle);

  /* Calculate the position based on gui's enums */
  if (GUI.TextXAlignment == LEFT)
    GuiArray[GUI_Index]->TextRectangle.x = GuiArray[GUI_Index]->PositionX;
  else if (GUI.TextXAlignment == RIGHT)
    GuiArray[GUI_Index]->TextRectangle.x = (GuiArray[GUI_Index]->PositionX + GuiArray[GUI_Index]->SizeX) - GuiArray[GUI_Index]->TextRectangle.w;
  else if (GUI.TextXAlignment == X_CENTER)
    GuiArray[GUI_Index]->TextRectangle.x = (GuiArray[GUI_Index]->SizeX / 2 + GuiArray[GUI_Index]->PositionX) - GuiArray[GUI_Index]->TextRectangle.w / 2;

  if (GUI.TextYAlignment == TOP)
    GuiArray[GUI_Index]->TextRectangle.y = GuiArray[GUI_Index]->PositionY;
  else if (GUI.TextYAlignment == Y_CENTER)
    GuiArray[GUI_Index]->TextRectangle.y = (GuiArray[GUI_Index]->SizeY / 2 + GuiArray[GUI_Index]->PositionY) - GuiArray[GUI_Index]->TextRectangle.h / 2;
  else if (GUI.TextYAlignment == BOTTOM)
    GuiArray[GUI_Index]->TextRectangle.y = (GuiArray[GUI_Index]->PositionY + GuiArray[GUI_Index]->SizeY) - GuiArray[GUI_Index]->TextRectangle.h; 
  
  GuiArray[GUI_Index]->TextFits = !(GUI.TextRectangle.w >= GUI.SizeX || GUI.TextRectangle.h >= GUI.SizeY); 
  
  if (GuiArray[GUI_Index]->TextFits == false)
    GuiArray[GUI_Index]->TextColor = RED;
  else
    GuiArray[GUI_Index]->TextColor = WHITE;

  /* Inherit parent properties */
  if (GUI.Parent > -1)
    GuiArray[GUI_Index]->Visible = GuiArray[GUI.Parent]->Visible;

  /* Ending */
  if (strcmp(error, "empty") != 0) {
    GUI_INTERNAL_ERROR(error);
    free(error);
  } 

  GuiArray[GUI_Index]->TextureText = Message; 
  SDL_FreeSurface(surfaceMessage);
}

void UpdateAllGUI() {
  for (int i = 0; i <= LastGUI_item; i++)
    UpdateGUI(i, false);

  CurrentList = -1;
  LastElement = -1;
}

void HandleGUI(int CurrentGUI) {
  if (GuiArray[CurrentGUI]->Visible == false)
    return;

  bool isColliding = CollosionRectPoint(GuiArray[CurrentGUI], MouseX, MouseY);

  if (isColliding == true) {
    if (GuiArray[CurrentGUI]->Type == BUTTON || GuiArray[CurrentGUI]->Type == TEXTBOX) {
      if (GuiArray[CurrentGUI]->Hovered == false && GuiArray[CurrentGUI]->Active == true) {
        GuiArray[CurrentGUI]->Hovered = true;
        GuiArray[CurrentGUI]->MouseEnter(CurrentGUI);
      }

      if (LeftButtonDown) {
        /* Trigger MouseDown callback */
        if (GuiArray[CurrentGUI]->Pressed == false && GuiArray[CurrentGUI]->Active == true)
          GuiArray[CurrentGUI]->MouseDown(CurrentGUI);

        /* Change Property */
        GuiArray[CurrentGUI]->Pressed = true;
        Cursor = strlen(GuiArray[CurrentGUI]->Text);

        if (IsFocused && GuiArray[CurrentGUI]->Type == TEXTBOX)
          CurrentGUI_Focused = CurrentGUI;
        else if (!IsFocused) {
          SDL_StartTextInput();

          CurrentGUI_Focused = CurrentGUI;
          IsFocused = true;
          GuiArray[CurrentGUI]->Focused = true;
        }

      } else {
        if (GuiArray[CurrentGUI]->Pressed == true) {
          GuiArray[CurrentGUI]->Pressed = false;
          GuiArray[CurrentGUI]->MouseUp(CurrentGUI);
        }
      }
    }
  } else {
    if (GuiArray[CurrentGUI]->Type == BUTTON || GuiArray[CurrentGUI]->Type == TEXTBOX) {
      if (GuiArray[CurrentGUI]->Hovered == true && GuiArray[CurrentGUI]->Active == true) {
        GuiArray[CurrentGUI]->MouseLeave(CurrentGUI);
        GuiArray[CurrentGUI]->Hovered = false;
        GuiArray[CurrentGUI]->Pressed = false;
      }
    }
  }
}

struct GuiProperties* PSConstructGUI(enum GUI_TYPE GUI, int X, int Y, int Width, int Height) {
  LastGUI_item += 1;

  SDL_Color SDL_BLACK = {0, 0, 0, 255};
  struct GuiProperties *newGUI = malloc(sizeof(struct GuiProperties));

  newGUI->ElementIndex = LastGUI_item;
  newGUI->Parent = -1;
  newGUI->Zindex = 1;

  newGUI->Type = GUI;
  newGUI->Hovered = false;
  newGUI->Pressed = false;
  newGUI->Active = true;

  newGUI->PositionX = X;
  newGUI->PositionY = Y;
  newGUI->SizeX = Width;
  newGUI->SizeY = Height;

  newGUI->Visible = true;
  newGUI->BorderSize = 1;
  newGUI->OutlineSize = 0;

  newGUI->TextSize = 16;
  newGUI->TextWrapped = false;
  newGUI->TextFits = true;
  newGUI->TextScaled = false;
  newGUI->MultiLine = false;

  newGUI->ScrollActive = false;

  newGUI->TextColor = DefaultTextColor;
  newGUI->BackgroundColor = DefaultBackgroundColor;
  newGUI->BorderColor = DefaultBorderColor;

  /* Ensure that no pointer is left which could trigger an error */
  newGUI->MouseEnter = defaultCallback;
  newGUI->MouseLeave = defaultCallback;
  newGUI->MouseUp = defaultCallback;
  newGUI->MouseDown = defaultCallback;
  newGUI->FocusLost = defaultCallback;

  newGUI->TextRectangle.x = 0;
  newGUI->TextRectangle.y = 0;

  /* Place some default enums for TextXAlignment and TextYAlignment */
  newGUI->TextXAlignment = DefaultX_Alignment;
  newGUI->TextYAlignment = DefaultY_Alignment; 

  if (GUI == TEXTLABEL) {
    newGUI->Text = "Text";
    newGUI->TextEditable = false;
  }
  else if (GUI == BUTTON) {
    newGUI->Text = "TextButton";
    newGUI->TextEditable = false;
  } 
  else if (GUI == TEXTBOX) {
    newGUI->TextEditable = true;
    char *alloc = (char*)malloc(5 * sizeof(char));

    if (!alloc) {
      GUI_INTERNAL_ERROR("[PUIUS GUI] Failed to allocate space for string.\n");
    } else {
      newGUI->Text = alloc;
      strcpy(newGUI->Text, "Text");
    }
  } else if (GUI == IMAGELABEL) {
    newGUI->TextEditable = false;
    newGUI->Text = "";
  } else if (GUI == SCROLLFRAME) {
    newGUI->TextEditable = false;
    newGUI->CavnasY = 0;
    newGUI->CavnasX = 0;
    newGUI->ScrollActive = true;
    newGUI->HorizontalScrolling = false;
    newGUI->CavnasSpeed = 1;
  }

  SDL_Surface *surfaceMessage = TTF_RenderText_Blended(Font, newGUI->Text, SDL_BLACK);
  SDL_Texture *Message = SDL_CreateTextureFromSurface(AssignedRenderer, surfaceMessage);

  newGUI->AssignedRenderer = AssignedRenderer;
  newGUI->TextureText = Message;
  newGUI->Font = Font;

  if (LastGUI_item > 100)
    GUI_INTERNAL_ERROR("[PUIUS GUI] WARNING! LastGUI_item reached maximum capacity (100). The GUI element won't be added to GuiArray.\n");
  else
    GuiArray[LastGUI_item] = newGUI;

  SDL_FreeSurface(surfaceMessage);
  return newGUI;
}

struct GuiProperties *PConstructGUI(GUI_TYPE GUI, int X, int Y) {
  /* A function which simply calls the internal constructor, which is exposed by the header file. */
  return PSConstructGUI(GUI, X, Y, DEFAULT_ELEMENT_WIDTH, DEFAULT_ELEMENT_HEIGHT);
}

struct ListProperties *ConstructList(LIST_TYPE Type, int Parent, int PaddingX, int PaddingY) {
  LastList++;
  struct ListProperties *newGUI = malloc(sizeof(struct ListProperties));

  newGUI->Parent = Parent;
  newGUI->PaddingX = PaddingX;
  newGUI->PaddingY = PaddingY;

  newGUI->Type = Type;
  newGUI->SortOrder = LAYOUTORDER;
  newGUI->Direction = DOWN;

  newGUI->Wraps = false;
  ListArray[LastList] = newGUI;
  return newGUI;
}

void DrawCursor() {
  // TODO: Ensure that DrawCursor works with TextWrapped enabled
  if (GuiArray[CurrentGUI_Focused]->TextEditable == false)
    return;

  int CursorY = 1;
  int TextWidth, TextHeight;

  char *alloc = (char*)malloc(Cursor * sizeof(char) + 2);
  alloc[0] = '\0';

  char s = ' '; /* :moyai: */

  for (size_t i = 0; i < Cursor; i++) {
    if (GuiArray[CurrentGUI_Focused]->Text[i] == '\n') {
      alloc[0] = '\0';
      CursorY += 1;
    }

    if (GuiArray[CurrentGUI_Focused]->Text[i] != '\n')
      strncat(alloc, &GuiArray[CurrentGUI_Focused]->Text[i], 1);
    else
      strncat(alloc, &s, 1);
  }

  TTF_SizeText(GuiArray[CurrentGUI_Focused]->Font, alloc, &TextWidth, NULL);
  TextHeight = TTF_FontHeight(GuiArray[CurrentGUI_Focused]->Font);
  int CursorX = TextWidth + GuiArray[CurrentGUI_Focused]->TextRectangle.x;

  if (CursorY > 1)
    CursorX -= 4;

  CursorY = CursorY * TextHeight + GuiArray[CurrentGUI_Focused]->TextRectangle.y;

  DrawLine(CursorX, CursorY, CursorX, CursorY - TextHeight, 
           InitColor3(255 - GuiArray[CurrentGUI_Focused]->BackgroundColor.R, 
                      255 - GuiArray[CurrentGUI_Focused]->BackgroundColor.G, 
                      255 - GuiArray[CurrentGUI_Focused]->BackgroundColor.B, 
                      GuiArray[CurrentGUI_Focused]->BackgroundColor.A), 
           GuiArray[CurrentGUI_Focused]->AssignedRenderer);
  free(alloc);
}

void DrawGUI(int i) {
  struct Color3 color = InitColor3(GuiArray[i]->BorderColor.R, GuiArray[i]->BorderColor.G, GuiArray[i]->BorderColor.B, GuiArray[i]->BorderColor.A);

  // (x, y) - topleft
  // (x + sx, y) - topright
  // (x, y + sy) - bottomleft
  // (x + sx, y + sy) - bottomright
  
  SDL_Rect Rect = {GuiArray[i]->PositionX, GuiArray[i]->PositionY, GuiArray[i]->SizeX, GuiArray[i]->SizeY};
  DrawRectangleRec(Rect, GuiArray[i]->BackgroundColor, GuiArray[i]->AssignedRenderer); 
  if (GuiArray[i]->Type == IMAGELABEL)
    DrawTextureEx(GuiArray[i]);

  DrawText(GuiArray[i]);

  for (int borderNum = 0; borderNum < GuiArray[i]->BorderSize; borderNum++) {
    SDL_Point topLeft = InitPoint(GuiArray[i]->PositionX, GuiArray[i]->PositionY);
    SDL_Point topRight = InitPoint(GuiArray[i]->PositionX + GuiArray[i]->SizeX, GuiArray[i]->PositionY);
    SDL_Point bottomLeft = InitPoint(GuiArray[i]->PositionX, GuiArray[i]->PositionY + GuiArray[i]->SizeY);
    SDL_Point bottomRight = InitPoint(GuiArray[i]->PositionX + GuiArray[i]->SizeX, GuiArray[i]->PositionY + GuiArray[i]->SizeY);

    DrawLine(topLeft.x, topLeft.y + borderNum, topRight.x, topRight.y + borderNum, color, GuiArray[i]->AssignedRenderer); /* Top line */
    DrawLine(topLeft.x + borderNum, topLeft.y, bottomLeft.x + borderNum, bottomLeft.y, color, GuiArray[i]->AssignedRenderer); /* Left line */
    DrawLine(bottomRight.x, bottomRight.y - borderNum, bottomLeft.x, bottomRight.y - borderNum, color, GuiArray[i]->AssignedRenderer); /* Bottom line */
    DrawLine(bottomRight.x - borderNum, bottomRight.y, topRight.x - borderNum, topRight.y, color, GuiArray[i]->AssignedRenderer); /* Right line */
  }

  if (IsFocused == true && CurrentGUI_Focused > -1)
    DrawCursor();
}

void RenderGUI() {
  int LargestZindex = 2;

  for (int x = 0; x < 40; x++) {
    if (LargestZindex + 1 == x)
      break;

    for (int i = 0; i <= LastGUI_item; i++) {
      if (GuiArray[i]->Zindex == 0)
        break;

      if (GuiArray[i]->Zindex != x || GuiArray[i]->Visible == false)
        continue;

      if (GuiArray[i]->Zindex > LargestZindex)
        LargestZindex = GuiArray[i]->Zindex;

      HandleGUI(i);
      DrawGUI(i);
    }
  }
}

int InitGUI(SDL_Renderer *Renderer, char *FontPath, int FontSize) {
  WHITE = InitColor3(255, 255, 255, 255);
  BLACK = InitColor3(0, 0, 0, 255);
  RED = InitColor3(230, 41, 55, 255);
  BLUE = InitColor3(0, 121, 241, 255);
  LIME = InitColor3(0, 158, 47, 255);
  GRAY = InitColor3(130, 130, 130, 255);
  VIOLET = InitColor3(135, 60, 190, 255);

  DefaultTextColor = BLACK;
  DefaultBorderColor = BLACK;
  DefaultBackgroundColor = WHITE;
  DefaultX_Alignment = LEFT;
  DefaultY_Alignment = TOP;

  if(!Renderer) {
    GUI_INTERNAL_ERROR("[PUIUS GUI] No render given. Did you call the function correctly?\n");
    return 1;
  }

  AssignedRenderer = Renderer;

  Font = TTF_OpenFont(FontPath, FontSize);

  if (!Font) {
    GUI_INTERNAL_ERROR("[PUIUS GUI] Failed to load default font. SDL error:");
    GUI_INTERNAL_ERROR(SDL_GetError());
    GUI_INTERNAL_ERROR("\n");

    exit(1);
  }

  return true;
}

void SetDefaultTextAlignment(TEXT_XALIGNMENT X, TEXT_YALIGNMENT Y) {
  DefaultX_Alignment = X;
  DefaultY_Alignment = Y;
}

int ChangeDefaultFont(char *FontName, int FontSize) {
  Font = TTF_OpenFont(FontName, FontSize);

  if (!Font) {
    GUI_INTERNAL_ERROR("[PUIUS GUI] Failed to load the new font. SDL error:");
    GUI_INTERNAL_ERROR(SDL_GetError());
    GUI_INTERNAL_ERROR("\n");

    return false;
  }
  return true;
}

void UninitGUI() {
  for (int i = 0; i < LastGUI_item; i++) {
    SDL_DestroyTexture(GuiArray[i]->TextureText);

    if (GuiArray[i]->Type == TEXTBOX)
      free(GuiArray[i]->Text);

    free(GuiArray[i]);
  }
}
