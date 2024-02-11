#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "include/PuiusGUI.h"

SDL_Renderer *globalRenderer;
SDL_Window *globalWindow;

struct GuiProperties GuiArray[100];
int Inputs[258];
int Running = true;
int LastGUI_item = -1;

TTF_Font *Font;

int MouseY, MouseX;
int LeftButtonDown = false;

// Textbox values
int IsFocused = false;
int Cursor = 0;
int CurrentGUI_Focused = -1;

void defaultCallback(int a) {} // Used for ConstructGUI()
void defaultCallbackHover(int index) { // Used for ConstructGUI()
    GuiArray[index].BackgroundColor.R += 10;
    GuiArray[index].BackgroundColor.G += 10;
    GuiArray[index].BackgroundColor.B += 10;
}

void defaultCallbackHoverLeave(int index) {
    GuiArray[index].BackgroundColor.R -= 10;
    GuiArray[index].BackgroundColor.G -= 10;
    GuiArray[index].BackgroundColor.B -= 10;
}

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

int CollosionRectPoint(struct GuiProperties rect, int pointX, int pointY) {
    if (pointX > rect.PositionX && pointX < rect.PositionX + rect.SizeX && pointY > rect.PositionY && pointY < rect.PositionY + rect.SizeY)
            return true;
        return false;
}

void DrawRectangleRec(struct GuiProperties rectangle) {
    /* Set the color to the specified color, draw the rectangle, reset the color. */
    if (globalRenderer) {
        SDL_Color oldColor = {0, 0, 0, 0};
        SDL_GetRenderDrawColor(globalRenderer, &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);

        SDL_Rect newRect = {
            rectangle.PositionX,
            rectangle.PositionY,
            rectangle.SizeX,
            rectangle.SizeY
        };

        SDL_SetRenderDrawColor(globalRenderer, rectangle.BackgroundColor.R, rectangle.BackgroundColor.G, rectangle.BackgroundColor.B, rectangle.BackgroundColor.A);
        SDL_RenderFillRect(globalRenderer, &newRect);
        SDL_SetRenderDrawColor(globalRenderer, oldColor.r, oldColor.g, oldColor.b, oldColor.a);

     } else {
         printf("[PUIUS GUI] Tried drawing a rectangle without initializing globalRenderer.\n (use initLayer(SDL_Renderer* renderer, SDL_Window *window) to initialize globalRenderer)");
    }
}

void DrawLine(int StartX, int StartY, int EndX, int EndY, struct Color3 Color) {
    /* Set the color to the specified color, draw the line, reset the color. */
    if (globalRenderer) {
        SDL_Color oldColor = {0, 0, 0, 0};
        SDL_GetRenderDrawColor(globalRenderer, &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);

        SDL_SetRenderDrawColor(globalRenderer, Color.R, Color.G, Color.B, 255);
        SDL_RenderDrawLine(globalRenderer, StartX, StartY, EndX, EndY);
        SDL_SetRenderDrawColor(globalRenderer, oldColor.r, oldColor.g, oldColor.b, oldColor.a);
    } else {
        printf("[PUIUS GUI] Tried drawing a line without initializing globalRenderer.\n (use initLayer(SDL_Renderer* renderer, SDL_Window *window) to initialize globalRenderer)");
    }
}

void DrawText(struct GuiProperties gui) {
    if (globalRenderer) {
        SDL_Color oldColor = {0, 0, 0, 0};
        SDL_GetRenderDrawColor(globalRenderer, &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);

        SDL_SetRenderDrawColor(globalRenderer, gui.TextColor.R, gui.TextColor.G, gui.TextColor.B, gui.TextColor.A);
        SDL_RenderCopy(globalRenderer, gui.TextureText, NULL, &gui.TextRectangle);
        SDL_SetRenderDrawColor(globalRenderer, oldColor.r, oldColor.g, oldColor.b, oldColor.a);
    }
}

void DrawTextureEx(struct GuiProperties rectangle) {
    if (globalRenderer) {
        SDL_Color oldColor = {0, 0, 0, 0};
        SDL_GetRenderDrawColor(globalRenderer, &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);

        SDL_Rect newRect = {
            rectangle.PositionX,
            rectangle.PositionY,
            rectangle.SizeX,
            rectangle.SizeY
        };

        SDL_SetRenderDrawColor(globalRenderer, rectangle.BackgroundColor.R, rectangle.BackgroundColor.G, rectangle.BackgroundColor.B, rectangle.BackgroundColor.A);
        SDL_RenderCopyEx(globalRenderer, rectangle.Image, NULL, &newRect, 0, NULL, SDL_FLIP_NONE);
        SDL_SetRenderDrawColor(globalRenderer, oldColor.r, oldColor.g, oldColor.b, oldColor.a);

     } else {
         printf("[PUIUS GUI] Tried drawing a texture without initializing globalRenderer.\n (use initLayer(SDL_Renderer* renderer, SDL_Window *window) to initialize globalRenderer)");
    }
}

void WriteToTextBox(char *str) {
    if(IsFocused == false) {
        return;
    }
    // printf("%i\n", Cursor);

    int textLength = strlen(GuiArray[CurrentGUI_Focused].Text);
    char *alloc;

    alloc = malloc(textLength * 2);

    if (alloc == NULL) {
        printf("[PUIUS GUI] Failed to allocate memory for new string when writing to a textbox.");
        exit(1);
    }

    if (strcmp(str, "ENTER") == 0) {
        for (int i = 0; i <= strlen(alloc); i++) {
            if (i == Cursor) {
                alloc[i] = '\n';
                alloc[i + 1] = GuiArray[CurrentGUI_Focused].Text[i];
                continue;
            } else if (i > Cursor) {
                alloc[i + 1] = GuiArray[CurrentGUI_Focused].Text[i];
            } else {
                alloc[i] = GuiArray[CurrentGUI_Focused].Text[i];
            }
        };

        Cursor = Cursor + 1;
    }
    else if(strcmp(str, "BACKSPACE") == 0) {
        strcpy(alloc, GuiArray[CurrentGUI_Focused].Text);
        for (int i = (Cursor - 1); alloc[i] != '\0'; i++) {
            alloc[i] = alloc[i + 1];
        }

        if (Cursor > 0)
            Cursor -= 1;
    }
    else {
        for (int i = 0; i <= strlen(alloc); i++) {
            if (i == Cursor) {
                alloc[i] = str[0];
                alloc[i + 1] = GuiArray[CurrentGUI_Focused].Text[i];
                continue;
            } else if (i > Cursor) {
                alloc[i + 1] = GuiArray[CurrentGUI_Focused].Text[i];
            } else {
                alloc[i] = GuiArray[CurrentGUI_Focused].Text[i];
            }
        };

        Cursor = Cursor + 1;
    }
    free(GuiArray[CurrentGUI_Focused].Text);

    GuiArray[CurrentGUI_Focused].Text = alloc;
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
                Inputs[event.key.keysym.scancode] = 1;
                break;
            case SDL_KEYUP:
                Inputs[event.key.keysym.scancode] = 0;
                break;
            case SDL_MOUSEMOTION:
                SDL_GetMouseState(&MouseX, &MouseY);
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    Inputs[257] = 0;
                    LeftButtonDown = false;
                    // input->LEFT_BUTTON = 0;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    Inputs[257] = 1;

                    if (IsFocused) {
                        IsFocused = false;
                        GuiArray[CurrentGUI_Focused].FocusLost(CurrentGUI_Focused);
                    }

                    LeftButtonDown = true;
                }
                break;
            case SDL_TEXTINPUT:
                WriteToTextBox(event.text.text);
        }
    }
}

void UpdateGUI(int GUI_Index) {
    printf("ID: %i Zindex: %i\n", GUI_Index, GuiArray[GUI_Index].Zindex);

    if (GuiArray[GUI_Index].Parent > -1 && GuiArray[GUI_Index].Parent < LastGUI_item) {
        int Parent = GuiArray[GUI_Index].Parent;

        GuiArray[GUI_Index].PositionX = GuiArray[GUI_Index].PositionX + GuiArray[Parent].PositionX;
        GuiArray[GUI_Index].PositionY = GuiArray[GUI_Index].PositionY + GuiArray[Parent].PositionY;
    }

    SDL_Color Color = {GuiArray[GUI_Index].TextColor.R, GuiArray[GUI_Index].TextColor.G, GuiArray[GUI_Index].TextColor.B};

    TTF_SetFontSize(GuiArray[GUI_Index].Font, GuiArray[GUI_Index].TextSize);
    TTF_SetFontOutline(GuiArray[GUI_Index].Font, GuiArray[GUI_Index].OutlineSize);
    SDL_Surface *surfaceMessage;

    if (GuiArray[GUI_Index].TextWrapped)
        surfaceMessage = TTF_RenderText_Blended_Wrapped(GuiArray[GUI_Index].Font, GuiArray[GUI_Index].Text, Color, GuiArray[GUI_Index].SizeX);
    else
        surfaceMessage = TTF_RenderText_Blended_Wrapped(GuiArray[GUI_Index].Font, GuiArray[GUI_Index].Text, Color, 0);

    SDL_Texture *Message = SDL_CreateTextureFromSurface(globalRenderer, surfaceMessage);

    SDL_GetClipRect(surfaceMessage, &GuiArray[GUI_Index].TextRectangle);
    // GuiArray[GUI_Index].TextRectangle.x = GuiArray[GUI_Index].PositionX;
    // GuiArray[GUI_Index].TextRectangle.y = GuiArray[GUI_Index].PositionY;

    /* Calculate the position based on gui's enums */

    if (GuiArray[GUI_Index].TextXAlignment == LEFT) {
        GuiArray[GUI_Index].TextRectangle.x = GuiArray[GUI_Index].PositionX;
    } else if (GuiArray[GUI_Index].TextXAlignment == RIGHT) {
        GuiArray[GUI_Index].TextRectangle.x = (GuiArray[GUI_Index].PositionX + GuiArray[GUI_Index].SizeX) - GuiArray[GUI_Index].TextRectangle.w;
    } else if (GuiArray[GUI_Index].TextXAlignment == X_CENTER) {
        GuiArray[GUI_Index].TextRectangle.x = (GuiArray[GUI_Index].SizeX / 2 + GuiArray[GUI_Index].PositionX) - GuiArray[GUI_Index].TextRectangle.w / 2;
    }

    if (GuiArray[GUI_Index].TextYAlignment == TOP) {
        GuiArray[GUI_Index].TextRectangle.y = GuiArray[GUI_Index].PositionY;
    } else if (GuiArray[GUI_Index].TextYAlignment == Y_CENTER) {
        GuiArray[GUI_Index].TextRectangle.y = (GuiArray[GUI_Index].SizeY / 2 + GuiArray[GUI_Index].PositionY) - GuiArray[GUI_Index].TextRectangle.h / 2;
    } else if (GuiArray[GUI_Index].TextYAlignment == BOTTOM) {
        GuiArray[GUI_Index].TextRectangle.y = (GuiArray[GUI_Index].PositionY + GuiArray[GUI_Index].SizeY) - GuiArray[GUI_Index].TextRectangle.h;
    }

    if (GuiArray[GUI_Index].TextRectangle.w > GuiArray[GUI_Index].SizeX)
        GuiArray[GUI_Index].TextFits = 0;
    else
        GuiArray[GUI_Index].TextFits = 1;

    if (GuiArray[GUI_Index].TextRectangle.h > GuiArray[GUI_Index].SizeY)
        GuiArray[GUI_Index].TextFits = 0;
    else
        GuiArray[GUI_Index].TextFits = 1;

    GuiArray[GUI_Index].TextureText = Message;
    SDL_FreeSurface(surfaceMessage);
}

void UpdateAllGUI() {
    for (int i = 0; i <= LastGUI_item; i++) {
        UpdateGUI(i);
    }
}

void HandleGUI(int currentGUI) {
    int isColliding = CollosionRectPoint(GuiArray[currentGUI], MouseX, MouseY);

    if ((isColliding && GuiArray[currentGUI].Type == TEXTBUTTON) || (isColliding && GuiArray[currentGUI].Type == IMAGEBUTTON) || (isColliding && GuiArray[currentGUI].Type == TEXTBOX)) {
        if (GuiArray[currentGUI].Hovered == false)
            GuiArray[currentGUI].MouseLeave(currentGUI);

        GuiArray[currentGUI].Hovered = true;

        if (LeftButtonDown) {
            /* Trigger MouseDown callback */
            if (GuiArray[currentGUI].Pressed == false)
                GuiArray[currentGUI].MouseDown(currentGUI);

            /* Change Property */
            GuiArray[currentGUI].Pressed = true;
            Cursor = strlen(GuiArray[currentGUI].Text);

            if (IsFocused && GuiArray[currentGUI].Type == TEXTBOX) {
                CurrentGUI_Focused = currentGUI;
            } else if (!IsFocused) {
                SDL_StartTextInput();

                CurrentGUI_Focused = currentGUI;
                IsFocused = true;
                GuiArray[currentGUI].Focused = true;
            }

        }
        else {
            GuiArray[currentGUI].Pressed = false;
        }
    }
    else if ((!isColliding && GuiArray[currentGUI].Type == TEXTBUTTON) || (!isColliding && GuiArray[currentGUI].Type == IMAGEBUTTON) || (!isColliding && GuiArray[currentGUI].Type == TEXTBOX)) {
        if (GuiArray[currentGUI].Hovered == true)
            GuiArray[currentGUI].MouseEnter(currentGUI);

        GuiArray[currentGUI].Hovered = false;
        GuiArray[currentGUI].Pressed = false;
    }
}

int ConstructGUI(enum GUI_TYPE GUI, int X, int Y) {
    if (LastGUI_item < 100) {
        SDL_Color BLACK = {0, 0, 0};

        struct Color3 TextColor = InitColor3(0, 0, 0, 255);
        struct Color3 BackgroundColor = InitColor3(255, 255, 255, 255);
        struct Color3 BorderColor = InitColor3(0, 0, 0, 255);

        struct GuiProperties newGUI;
        LastGUI_item += 1;

        newGUI.Parent = -1;
        newGUI.Zindex = 1;

        newGUI.Type = GUI;
        newGUI.Hovered = false;
        newGUI.Pressed = false;

        newGUI.PositionX = X;
        newGUI.PositionY = Y;
        newGUI.SizeX = 70;
        newGUI.SizeY = 25;

        newGUI.Visible = true;
        newGUI.BorderSize = 1;
        newGUI.OutlineSize = 0;

        newGUI.TextSize = 16;
        newGUI.TextWrapped = false;
        newGUI.TextFits = true;
        newGUI.TextScaled = false;

        newGUI.TextColor = TextColor;
        newGUI.BackgroundColor = BackgroundColor;
        newGUI.BorderColor = BorderColor;

        /* Ensure that no pointer is left which could trigger an error */
        newGUI.MouseEnter = defaultCallbackHover;
        newGUI.MouseDown = defaultCallback;
        newGUI.FocusLost = defaultCallback;
        newGUI.MouseLeave = defaultCallbackHoverLeave;

        SDL_Surface *surfaceMessage = TTF_RenderText_Blended(Font, "Text", BLACK);
        SDL_Texture *Message = SDL_CreateTextureFromSurface(globalRenderer, surfaceMessage);

        newGUI.TextRectangle.x = 0;
        newGUI.TextRectangle.y = 0;

        /* Place some default enums for TextXAlignment and TextYAlignment */
        newGUI.TextXAlignment = LEFT;
        newGUI.TextYAlignment = TOP;

        char *alloc = (char*)malloc(5 * sizeof(char));

        if (!alloc) {
            printf("[PUIUS GUI] Failed to allocate space for string.");
            return -1;
        }

        newGUI.Text = alloc;

        strcpy(newGUI.Text, "Text");
        newGUI.TextureText = Message;
        newGUI.Font = Font;

        if (GUI == TEXTLABEL) {
            newGUI.TextEditable = false;
        } else if (GUI == TEXTBUTTON) {
            newGUI.TextEditable = false;
        } else if (GUI == TEXTBOX) {
            newGUI.TextEditable = true;
        } else if (GUI == IMAGEBUTTON) {
            newGUI.TextEditable = false;
        } else if (GUI == IMAGELABEL) {
            newGUI.TextEditable = false;
        }

        GuiArray[LastGUI_item] = newGUI;
        SDL_FreeSurface(surfaceMessage);
        return LastGUI_item;
    } else {
        return -1;
    }
}

void DrawCursor() {

}

void DrawGUI(int i) {
    struct Color3 color = InitColor3(GuiArray[i].BorderColor.R, GuiArray[i].BorderColor.G, GuiArray[i].BorderColor.B, GuiArray[i].BorderColor.A);

    // (x, y) - topleft
    // (x + sx, y) - topright
    // (x, y + sy) - bottomleft
    // (x + sx, y + sy) - bottomright

    if(GuiArray[i].Type != IMAGELABEL && GuiArray[i].Type != IMAGEBUTTON) {
        DrawRectangleRec(GuiArray[i]);
        DrawText(GuiArray[i]);
    }
    else
        DrawTextureEx(GuiArray[i]);

    for (int borderNum = 0; borderNum < GuiArray[i].BorderSize; borderNum++) {
        SDL_Point topLeft = InitPoint(GuiArray[i].PositionX, GuiArray[i].PositionY);
        SDL_Point topRight = InitPoint(GuiArray[i].PositionX + GuiArray[i].SizeX, GuiArray[i].PositionY);
        SDL_Point bottomLeft = InitPoint(GuiArray[i].PositionX, GuiArray[i].PositionY + GuiArray[i].SizeY);
        SDL_Point bottomRight = InitPoint(GuiArray[i].PositionX + GuiArray[i].SizeX, GuiArray[i].PositionY + GuiArray[i].SizeY);

        DrawLine(topLeft.x, topLeft.y + borderNum, topRight.x, topRight.y + borderNum, color); /* Top line */
        DrawLine(topLeft.x + borderNum, topLeft.y, bottomLeft.x + borderNum, bottomLeft.y, color); /* Left line */
        DrawLine(bottomRight.x, bottomRight.y - borderNum, bottomLeft.x, bottomRight.y - borderNum, color); /* Bottom line */
        DrawLine(bottomRight.x - borderNum, bottomRight.y, topRight.x - borderNum, topRight.y,color); /* Right line */
    }
}

void RenderGUI() {
    int LargestZindex = 2;

    for (int x = 1; x < 40; x++) {
        if (LargestZindex + 1 == x)
            break;

        for (int i = 0; i < 100; i++) {
            if (i > LastGUI_item || GuiArray[i].Zindex == 0)
                break;

            if (GuiArray[i].Zindex != x || GuiArray[i].Visible == false)
                continue;

            if (GuiArray[i].Zindex > LargestZindex)
                LargestZindex = GuiArray[i].Zindex;

            HandleGUI(i);
            DrawGUI(i);
        }
    }
}

int InitLayer(SDL_Renderer *renderer, SDL_Window *window) {
    globalRenderer = renderer;
    globalWindow = window;

    Font = TTF_OpenFont("arial.ttf", 16);

    if (!Font) {
        printf("[PUIUS GUI] Failed to load default font. Does arial.ttf exist in the running directory?");

        exit(1);
    }

    return true;
}

int ChangeDefaultFont(char *fontName, int fontSize) {
    Font = TTF_OpenFont(fontName, fontSize);

    if (!Font) {
        printf("[PUIUS GUI] Failed to change font. Did you specify the right directory?");

        return false;
    }
    return true;
}

void UninitLayer() {
    for (int i = 0; i < LastGUI_item; i++) {
        SDL_DestroyTexture(GuiArray[i].TextureText);

        if (GuiArray[i].Type == TEXTBOX)
            free(GuiArray[i].Text);
    }
}
