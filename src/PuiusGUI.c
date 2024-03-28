#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

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

SDL_Renderer *AssignedRenderer;

struct GuiProperties *GuiArray[100];
struct ListProperties *ListArray[100];
bool Inputs[258];
bool Running = true;

int LastList = -1;
int LastGUI_item = -1;
int TabSize = 4; /* How many spaces are added for a tab */

TTF_Font *Font;

int MouseY, MouseX;
bool LeftButtonDown = false;

// Textbox values
bool IsFocused = false;
size_t Cursor = 0;
int CurrentGUI_Focused = -1;

void defaultCallback() {} // Used for ConstructGUI()

struct Color3 InitColor3(int R, int G, int B, int A) {
    struct Color3 Color;

    Color.R = R;
    Color.B = B;
    Color.G = G;
    Color.A = A;

    return Color;
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
    #endif
}

void DrawRectangleRec(struct GuiProperties *GUI) {
    struct GuiProperties rectangle = *GUI;

    /* Set the color to the specified color, draw the rectangle, reset the color. */
    if (AssignedRenderer) {
        SDL_Color oldColor = {0, 0, 0, 0};
        SDL_GetRenderDrawColor(GUI->AssignedRenderer, &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);

        SDL_Rect newRect = {
            rectangle.PositionX,
            rectangle.PositionY,
            rectangle.SizeX,
            rectangle.SizeY
        };

        SDL_SetRenderDrawColor(GUI->AssignedRenderer, rectangle.BackgroundColor.R, rectangle.BackgroundColor.G, rectangle.BackgroundColor.B, rectangle.BackgroundColor.A);
        SDL_RenderFillRect(GUI->AssignedRenderer, &newRect);
        SDL_SetRenderDrawColor(GUI->AssignedRenderer, oldColor.r, oldColor.g, oldColor.b, oldColor.a);

     } else {
         GUI_INTERNAL_ERROR("[PUIUS GUI] Tried drawing a rectangle without initializing AssignedRenderer.\n (use InitGUI(SDL_Renderer *Renderer, char *FontPath, int FontSize) to initialize AssignedRenderer)\n");
    }
}

void DrawLine(int StartX, int StartY, int EndX, int EndY, struct Color3 Color, SDL_Renderer *AssignedRenderer) {
    /* Set the color to the specified color, draw the line, reset the color. */
    if (AssignedRenderer) {
        SDL_Color oldColor = {0, 0, 0, 0};
        SDL_GetRenderDrawColor(AssignedRenderer, &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);

        SDL_SetRenderDrawColor(AssignedRenderer, Color.R, Color.G, Color.B, 255);
        SDL_RenderDrawLine(AssignedRenderer, StartX, StartY, EndX, EndY);
        SDL_SetRenderDrawColor(AssignedRenderer, oldColor.r, oldColor.g, oldColor.b, oldColor.a);
    } else {
        GUI_INTERNAL_ERROR("[PUIUS GUI] Tried drawing a line without initializing AssignedRenderer.\n (use InitGUI(SDL_Renderer *Renderer, char *FontPath, int FontSize) to initialize AssignedRenderer)\n");
    }
}

void DrawText(struct GuiProperties *GUI) {
    struct GuiProperties gui = *GUI;

    if (AssignedRenderer) {
        SDL_Color oldColor = {0, 0, 0, 0};
        SDL_GetRenderDrawColor(GUI->AssignedRenderer, &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);

        SDL_SetRenderDrawColor(GUI->AssignedRenderer, gui.TextColor.R, gui.TextColor.G, gui.TextColor.B, gui.TextColor.A);
        SDL_RenderCopy(GUI->AssignedRenderer, gui.TextureText, NULL, &gui.TextRectangle);
        SDL_SetRenderDrawColor(GUI->AssignedRenderer, oldColor.r, oldColor.g, oldColor.b, oldColor.a);
    }
}

void DrawTextureEx(struct GuiProperties *GUI) {
    struct GuiProperties Rectangle = *GUI;

    if (!Rectangle.Image)
        GUI_INTERNAL_ERROR("[PUIUS GUI] GUI element contains no image property! Did you forget to add the image pointer?\n");

    if (AssignedRenderer) {
        SDL_Color OldColor = {0, 0, 0, 0};
        SDL_GetRenderDrawColor(GUI->AssignedRenderer, &OldColor.r, &OldColor.g, &OldColor.b, &OldColor.a);

        int Texture_x = 0, Texture_y = 0;

        if (SDL_QueryTexture(Rectangle.Image, NULL, NULL, &Texture_x, &Texture_y) != 0) {
            GUI_INTERNAL_ERROR("[PUIUS GUI] SDL_QueryTexture failed! SDL Error:");
            GUI_INTERNAL_ERROR(SDL_GetError());
            GUI_INTERNAL_ERROR("\n");
        }

        SDL_Rect NewRect = {
            Rectangle.PositionX,
            Rectangle.PositionY,
            Rectangle.SizeX,
            Rectangle.SizeY
        };

        SDL_Rect Part = {
            0,
            0,
            Texture_x,
            Texture_y
        };

        SDL_SetRenderDrawColor(GUI->AssignedRenderer, Rectangle.BackgroundColor.R, Rectangle.BackgroundColor.G, Rectangle.BackgroundColor.B, Rectangle.BackgroundColor.A);
        SDL_RenderCopyEx(GUI->AssignedRenderer, Rectangle.Image, &Part, &NewRect, 0, NULL, SDL_FLIP_NONE);
        SDL_SetRenderDrawColor(GUI->AssignedRenderer, OldColor.r, OldColor.g, OldColor.b, OldColor.a);

     } else {
         GUI_INTERNAL_ERROR("[PUIUS GUI] Tried drawing a texture without initializing the library. (use InitGUI(SDL_Renderer *Renderer, char *FontPath, int FontSize) to initialize AssignedRenderer)\n");
    }
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
        GUI_INTERNAL_ERROR("[PUIUS GUI] Failed to allocate memory for new string when writing to the focused textbox.\n");
        exit(1);
    }

    if (strcmp(str, "ENTER") == 0) {
        if (GUI->MultiLine == false) {
            strcpy(alloc, GUI->Text);
            GuiArray[CurrentGUI_Focused]->FocusLost(CurrentGUI_Focused);
            IsFocused = false;
        } else {
            for (size_t i = 0; i <= strlen(alloc); i++) {
                if (i == Cursor) {
                    alloc[i] = '\n';
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
    }
    else if(strcmp(str, "BACKSPACE") == 0) {
        strcpy(alloc, GUI->Text);
        for (size_t i = (Cursor - 1); alloc[i] != '\0'; i++) {
            alloc[i] = alloc[i + 1];
        }

        if (Cursor > 0)
            Cursor -= 1;
    }
    else {
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
    free(GUI->Text);

    GUI->Text = alloc;
    UpdateGUI(CurrentGUI_Focused);
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
    struct GuiProperties GUI = *GuiArray[GUI_Index]; /* Dereference the pointer */

    for (int i = 0; i <= LastList; i++) {
        if (GUI.Parent == ListArray[i]->Parent) {
            return i;
        }
    }

    return -1;
}

void UpdateGUI(int GUI_Index) {
    /* Variables */
    static int CurrentList = -1, LastElement = -1;
    int List = ParentHasList(GUI_Index);

    int success = 1;
    char error[100] = "empty";
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
            GuiArray[GUI_Index]->PositionX = GuiArray[Parent]->PositionX;
            GuiArray[GUI_Index]->PositionY = GuiArray[Parent]->PositionY;
        } else if (ListArray[CurrentList]->Direction == DOWN) {
            GuiArray[GUI_Index]->PositionX = GuiArray[Parent]->PositionX;
            GuiArray[GUI_Index]->PositionY = GuiArray[LastElement]->PositionY + GUI.SizeY + ListArray[CurrentList]->PaddingY;
        } else if (ListArray[CurrentList]->Direction == UP) {
            GuiArray[GUI_Index]->PositionX = GuiArray[Parent]->PositionX;
            GuiArray[GUI_Index]->PositionY = GuiArray[LastElement]->PositionY - GUI.SizeY - ListArray[CurrentList]->PaddingY;
        } else if (ListArray[CurrentList]->Direction == DLEFT) {
            GuiArray[GUI_Index]->PositionX = GuiArray[LastElement]->PositionX - GUI.SizeX - ListArray[CurrentList]->PaddingX;
            GuiArray[GUI_Index]->PositionY = GuiArray[Parent]->PositionY;
        } else if (ListArray[CurrentList]->Direction == DRIGHT) {
            GuiArray[GUI_Index]->PositionX = GuiArray[LastElement]->PositionX + GUI.SizeX + ListArray[CurrentList]->PaddingX;
            GuiArray[GUI_Index]->PositionY = GuiArray[Parent]->PositionY;
        }

        LastElement = GUI_Index;
    }

    /* Font related */
    if (!GUI.Font)
        sprintf(error, "[PUIUS GUI] GuiArray[%i].Font is inexistent!\n", GUI_Index);

    success = TTF_SetFontSize(GuiArray[GUI_Index]->Font, GUI.TextSize);
    if (success == -1)
        sprintf(error, "[PUIUS GUI] TTF_SetFontSize failed for GUI element with %i as index!\n", GUI_Index);

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


    /* TextFits logic */
    if (GUI.TextRectangle.w > GUI.SizeX)
        GuiArray[GUI_Index]->TextFits = 0;
    else
        GuiArray[GUI_Index]->TextFits = 1;

    if (GUI.TextRectangle.h > GUI.SizeY)
        GuiArray[GUI_Index]->TextFits = 0;
    else
        GuiArray[GUI_Index]->TextFits = 1;

    /* Inherit parent properties */
    if (GUI.Parent > -1)
        GuiArray[GUI_Index]->Visible = GuiArray[GUI.Parent]->Visible;

    /* Ending */
    if (strcmp(error, "empty") != 0) {
        GUI_INTERNAL_ERROR(error);
        GUI_INTERNAL_ERROR(SDL_GetError());
    }

    GuiArray[GUI_Index]->TextureText = Message;
    SDL_FreeSurface(surfaceMessage);
}

void UpdateAllGUI() {
    for (int i = 0; i <= LastGUI_item; i++)
        UpdateGUI(i);
}

void HandleGUI(int CurrentGUI) {
    /* TODO: Rework the function */

    struct GuiProperties GUI = *GuiArray[CurrentGUI];
    int isColliding = CollosionRectPoint(GuiArray[CurrentGUI], MouseX, MouseY);

    if ((isColliding && GUI.Type == BUTTON) || (isColliding && GUI.Type == TEXTBOX)) {
        if (GuiArray[CurrentGUI]->Hovered == false && GuiArray[CurrentGUI]->Active == true)
            GuiArray[CurrentGUI]->MouseEnter(CurrentGUI);

        GuiArray[CurrentGUI]->Hovered = true;

        if (LeftButtonDown) {
            /* Trigger MouseDown callback */
            if (GuiArray[CurrentGUI]->Pressed == false && GuiArray[CurrentGUI]->Active == true)
                GuiArray[CurrentGUI]->MouseDown(CurrentGUI);

            /* Change Property */
            GuiArray[CurrentGUI]->Pressed = true;
            Cursor = strlen(GuiArray[CurrentGUI]->Text);

            if (IsFocused && GUI.Type == TEXTBOX) {
                CurrentGUI_Focused = CurrentGUI;
            } else if (!IsFocused) {
                SDL_StartTextInput();

                CurrentGUI_Focused = CurrentGUI;
                IsFocused = true;
                GuiArray[CurrentGUI]->Focused = true;
            }

        }
        else {
            GuiArray[CurrentGUI]->Pressed = false;
        }
    }
    else if ((!isColliding && GUI.Type == BUTTON) || (!isColliding && GUI.Type == TEXTBOX)) {
        if (GuiArray[CurrentGUI]->Hovered == true && GuiArray[CurrentGUI]->Active == true)
            GuiArray[CurrentGUI]->MouseLeave(CurrentGUI);

        GuiArray[CurrentGUI]->Hovered = false;
        GuiArray[CurrentGUI]->Pressed = false;
    }
}

struct GuiProperties* PSConstructGUI(enum GUI_TYPE GUI, int X, int Y, int Width, int Height) {
        LastGUI_item += 1;

        SDL_Color SDL_BLACK = {0, 0, 0, 255};
        struct GuiProperties *newGUI = malloc(sizeof(struct GuiProperties));

        newGUI->BodyIndex = LastGUI_item;
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
        newGUI->TextXAlignment = LEFT;
        newGUI->TextYAlignment = TOP;

        newGUI->Text = "Text";

        if (GUI == TEXTLABEL)
            newGUI->TextEditable = false;
        else if (GUI == BUTTON)
            newGUI->TextEditable = false;
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
        } else if (GUI == SCROLLFRAME)
            newGUI->TextEditable = false;

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
    struct GuiProperties GUI = *GuiArray[CurrentGUI_Focused];

    char s = ' '; /* :moyai: */

    for (size_t i = 0; i < Cursor; i++) {
        if (GUI.Text[i] == '\n') {
            alloc[0] = '\0';
            CursorY += 1;
        }

        if (GUI.Text[i] != '\n')
            strncat(alloc, &GUI.Text[i], 1);
        else
            strncat(alloc, &s, 1);
    }

    TTF_SizeText(GUI.Font, alloc, &TextWidth, NULL);
    TextHeight = TTF_FontHeight(GUI.Font);
    int CursorX = TextWidth + GUI.TextRectangle.x;

    if (CursorY > 1)
        CursorX -= 4;

    CursorY = CursorY * TextHeight + GUI.TextRectangle.y;

    DrawLine(CursorX, CursorY, CursorX, CursorY - TextHeight, InitColor3(255 - GUI.BackgroundColor.R, 255 - GUI.BackgroundColor.G, 255 - GUI.BackgroundColor.B, GUI.BackgroundColor.A), GUI.AssignedRenderer);
    free(alloc);
}

void DrawGUI(int i) {
    struct GuiProperties GUI = *GuiArray[i];
    struct Color3 color = InitColor3(GUI.BorderColor.R, GUI.BorderColor.G, GUI.BorderColor.B, GUI.BorderColor.A);

    // (x, y) - topleft
    // (x + sx, y) - topright
    // (x, y + sy) - bottomleft
    // (x + sx, y + sy) - bottomright

    DrawRectangleRec(&GUI);
    DrawText(&GUI);

    if (GUI.Type == IMAGELABEL)
        DrawTextureEx(&GUI);

    for (int borderNum = 0; borderNum < GUI.BorderSize; borderNum++) {
        SDL_Point topLeft = InitPoint(GUI.PositionX, GUI.PositionY);
        SDL_Point topRight = InitPoint(GUI.PositionX + GUI.SizeX, GUI.PositionY);
        SDL_Point bottomLeft = InitPoint(GUI.PositionX, GUI.PositionY + GUI.SizeY);
        SDL_Point bottomRight = InitPoint(GUI.PositionX + GUI.SizeX, GUI.PositionY + GUI.SizeY);

        DrawLine(topLeft.x, topLeft.y + borderNum, topRight.x, topRight.y + borderNum, color, GUI.AssignedRenderer); /* Top line */
        DrawLine(topLeft.x + borderNum, topLeft.y, bottomLeft.x + borderNum, bottomLeft.y, color, GUI.AssignedRenderer); /* Left line */
        DrawLine(bottomRight.x, bottomRight.y - borderNum, bottomLeft.x, bottomRight.y - borderNum, color, GUI.AssignedRenderer); /* Bottom line */
        DrawLine(bottomRight.x - borderNum, bottomRight.y, topRight.x - borderNum, topRight.y, color, GUI.AssignedRenderer); /* Right line */
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
    DefaultBackgroundColor = BLACK;

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
