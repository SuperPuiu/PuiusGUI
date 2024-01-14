#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <string.h>
#include <stdlib.h>

#include "include/PuiusGUI.h"

SDL_Renderer *globalRenderer;
SDL_Window *globalWindow;

struct guiProperties guiArray[100];
int lastGUI_item = -1;

TTF_Font *Arial;

int MouseY, MouseX;
int leftButtonDown = 0;

// Textbox values
int isFocused = 0;
int cursor = 0;
int currentGUI_Focused = -1;

void defaultCallback(int a) {}

struct Color3 initColor3(int R, int G, int B) {
    struct Color3 Color;

    Color.R = R;
    Color.B = B;
    Color.G = G;

    return Color;
}

SDL_Point initPoint(int x, int y) {
    SDL_Point point;
    point.x = x;
    point.y = y;

    return point;
}

int CollosionRectPoint(struct guiProperties rect, int pointX, int pointY) {
    if (pointX > rect.PositionX && pointX < rect.PositionX + rect.SizeX && pointY > rect.PositionY && pointY < rect.PositionY + rect.SizeY)
            return 1;
        return 0;
}

void DrawRectangleRec(struct guiProperties rectangle) {
    if (globalRenderer) {
        SDL_Color oldColor = {0, 0, 0};
        SDL_GetRenderDrawColor(globalRenderer, &oldColor.r, &oldColor.g, &oldColor.b, NULL);

        SDL_Rect newRect = {
            rectangle.PositionX,
            rectangle.PositionY,
            rectangle.SizeX,
            rectangle.SizeY
        };

        SDL_SetRenderDrawColor(globalRenderer, rectangle.BackgroundColor.R, rectangle.BackgroundColor.G, rectangle.BackgroundColor.B, 255);
        SDL_RenderFillRect(globalRenderer, &newRect);
        SDL_SetRenderDrawColor(globalRenderer, oldColor.r, oldColor.g, oldColor.b, 255);

     } else {
         printf("[PUIUS GUI] Tried drawing a rectangle without initializing globalRenderer.\n (use initLayer(SDL_Renderer* renderer, SDL_Window *window) to initialize globalRenderer)");
    }
}

void DrawLine(int StartX, int StartY, int EndX, int EndY, struct Color3 Color) {
    if (globalRenderer) {
        SDL_Color oldColor = {0, 0, 0};
        SDL_GetRenderDrawColor(globalRenderer, &oldColor.r, &oldColor.g, &oldColor.b, NULL);

        SDL_SetRenderDrawColor(globalRenderer, Color.R, Color.G, Color.B, 255);
        SDL_RenderDrawLine(globalRenderer, StartX, StartY, EndX, EndY);
        SDL_SetRenderDrawColor(globalRenderer, oldColor.r, oldColor.g, oldColor.b, 255);
    } else {
        printf("[PUIUS GUI] Tried drawing a line without initializing globalRenderer.\n (use initLayer(SDL_Renderer* renderer, SDL_Window *window) to initialize globalRenderer)");
    }
}

void DrawText(struct guiProperties gui) {
    if (globalRenderer) {
        SDL_Color oldColor = {0, 0, 0};
        SDL_GetRenderDrawColor(globalRenderer, &oldColor.r, &oldColor.g, &oldColor.b, NULL);

        SDL_SetRenderDrawColor(globalRenderer, gui.TextColor.R, gui.TextColor.G, gui.TextColor.B, 255);
        SDL_RenderCopy(globalRenderer, gui.TextureText, NULL, &gui.rect);
        SDL_SetRenderDrawColor(globalRenderer, oldColor.r, oldColor.g, oldColor.b, 255);
    }
}

void DrawTextureEx(struct guiProperties rectangle) {
    if (globalRenderer) {
        SDL_Color oldColor = {0, 0, 0};
        SDL_GetRenderDrawColor(globalRenderer, &oldColor.r, &oldColor.g, &oldColor.b, NULL);

        SDL_Rect newRect = {
            rectangle.PositionX,
            rectangle.PositionY,
            rectangle.SizeX,
            rectangle.SizeY
        };

        SDL_SetRenderDrawColor(globalRenderer, rectangle.BackgroundColor.R, rectangle.BackgroundColor.G, rectangle.BackgroundColor.B, 255);
        SDL_RenderCopyEx(globalRenderer, rectangle.Image, NULL, &newRect, 0, NULL, SDL_FLIP_NONE);
        SDL_SetRenderDrawColor(globalRenderer, oldColor.r, oldColor.g, oldColor.b, 255);

     } else {
         printf("[PUIUS GUI] Tried drawing a texture without initializing globalRenderer.\n (use initLayer(SDL_Renderer* renderer, SDL_Window *window) to initialize globalRenderer)");
    }
}

void writeToTextBox(char *str) {
    if(isFocused == 0) {
        return;
    }

    printf("%i\n", cursor);

    size_t textLength = strlen(guiArray[currentGUI_Focused].Text);
    char *alloc = malloc(textLength + 1 + 1);

    if (!alloc) {
        printf("[PUIUS GUI] Failed to allocate memory for new string when writing to a textbox.");
        exit(1);
    }
    strcpy(alloc, guiArray[currentGUI_Focused].Text);

    if (strcmp(str, "ENTER") == 0) {
        alloc[textLength] = '\n';
        alloc[textLength + (cursor)] = '\0';
    }
    else if(strcmp(str, "BACKSPACE") == 0) {
        for (int i = (cursor - 1); alloc[i] != '\0'; i++) {
            alloc[i] = alloc[i + 1];
        }
    }
    else {
        strcat(alloc, str);
    }

    cursor = strlen(alloc);

    guiArray[currentGUI_Focused].Text = alloc;
    updateGUI(currentGUI_Focused);
}

void processInput(struct inputStruct *input) {
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
                input->SDL_QUIT = 1;
                break;
            case SDL_KEYDOWN:
                // printf("%s\n", SDL_GetKeyName(event.key.keysym.sym));

                if(event.key.keysym.sym == SDLK_a)
                    input->A = 1;
                if(event.key.keysym.sym == SDLK_b)
                    input->B = 1;
                if(event.key.keysym.sym == SDLK_c)
                    input->C = 1;
                if(event.key.keysym.sym == SDLK_d)
                    input->D = 1;
                if(event.key.keysym.sym == SDLK_e)
                    input->E = 1;
                if(event.key.keysym.sym == SDLK_f)
                    input->F = 1;
                if(event.key.keysym.sym == SDLK_g)
                    input->G = 1;
                if(event.key.keysym.sym == SDLK_h)
                    input->H = 1;
                if(event.key.keysym.sym == SDLK_i)
                    input->I = 1;
                if(event.key.keysym.sym == SDLK_j)
                    input->J = 1;
                if(event.key.keysym.sym == SDLK_k)
                    input->K = 1;
                if(event.key.keysym.sym == SDLK_l)
                    input->L = 1;
                if(event.key.keysym.sym == SDLK_m)
                    input->M = 1;
                if(event.key.keysym.sym == SDLK_n)
                    input->N = 1;
                if(event.key.keysym.sym == SDLK_o)
                    input->O = 1;
                if(event.key.keysym.sym == SDLK_p)
                    input->P = 1;
                if(event.key.keysym.sym == SDLK_q)
                    input->Q = 1;
                if(event.key.keysym.sym == SDLK_r)
                    input->R = 1;
                if(event.key.keysym.sym == SDLK_s)
                    input->S = 1;
                if(event.key.keysym.sym == SDLK_t)
                    input->T = 1;
                if(event.key.keysym.sym == SDLK_u)
                    input->U = 1;
                if(event.key.keysym.sym == SDLK_v)
                    input->V = 1;
                if(event.key.keysym.sym == SDLK_w)
                    input->W = 1;
                if(event.key.keysym.sym == SDLK_w)
                    input->X = 1;
                if(event.key.keysym.sym == SDLK_x)
                    input->Y = 1;
                if(event.key.keysym.sym == SDLK_y)
                    input->Z = 1;
                if(event.key.keysym.sym == SDLK_RETURN)
                    writeToTextBox("ENTER");
                if(event.key.keysym.sym == SDLK_BACKSPACE)
                    writeToTextBox("BACKSPACE");
                if (event.key.keysym.sym == SDLK_LEFT && isFocused && cursor > 0)
                    cursor -= 1;
                if (event.key.keysym.sym == SDLK_RIGHT && isFocused && cursor < strlen(guiArray[currentGUI_Focused].Text))
                    cursor += 1;
                if(event.key.keysym.sym == SDLK_ESCAPE)
                    input->ESC = 1;
                break;
            case SDL_KEYUP:
                if(event.key.keysym.sym == SDLK_a)
                   input->A = 0;
                if(event.key.keysym.sym == SDLK_b)
                   input->B = 0;
                if(event.key.keysym.sym == SDLK_c)
                   input->C = 0;
                if(event.key.keysym.sym == SDLK_d)
                   input->D = 0;
                if(event.key.keysym.sym == SDLK_e)
                   input->E = 0;
                if(event.key.keysym.sym == SDLK_f)
                   input->F = 0;
                if(event.key.keysym.sym == SDLK_g)
                   input->G = 0;
                if(event.key.keysym.sym == SDLK_h)
                   input->H = 0;
                if(event.key.keysym.sym == SDLK_i)
                   input->I = 0;
                if(event.key.keysym.sym == SDLK_j)
                   input->J = 0;
                if(event.key.keysym.sym == SDLK_k)
                   input->K = 0;
                if(event.key.keysym.sym == SDLK_l)
                   input->L = 0;
                if(event.key.keysym.sym == SDLK_m)
                   input->M = 0;
                if(event.key.keysym.sym == SDLK_n)
                   input->N = 0;
                if(event.key.keysym.sym == SDLK_o)
                   input->O = 0;
                if(event.key.keysym.sym == SDLK_p)
                   input->P = 0;
                if(event.key.keysym.sym == SDLK_q)
                   input->Q = 0;
                if(event.key.keysym.sym == SDLK_r)
                   input->R = 0;
                if(event.key.keysym.sym == SDLK_s)
                   input->S = 0;
                if(event.key.keysym.sym == SDLK_t)
                   input->T = 0;
                if(event.key.keysym.sym == SDLK_u)
                   input->U = 0;
                if(event.key.keysym.sym == SDLK_v)
                   input->V = 0;
                if(event.key.keysym.sym == SDLK_w)
                   input->W = 0;
                if(event.key.keysym.sym == SDLK_x)
                   input->X = 0;
                if(event.key.keysym.sym == SDLK_y)
                   input->Y = 0;
                if(event.key.keysym.sym == SDLK_z)
                   input->Z = 0;
                if(event.key.keysym.sym == SDLK_ESCAPE)
                   input->ESC = 0;
                break;
            case SDL_MOUSEMOTION:
                SDL_GetMouseState(&MouseX, &MouseY);
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    leftButtonDown = 0;
                    input->LEFT_BUTTON = 0;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (isFocused)
                        isFocused = 0;

                    leftButtonDown = 1;
                    input->LEFT_BUTTON = 1;
                }
                break;
            case SDL_TEXTINPUT:
                writeToTextBox(event.text.text);
            case SDL_TEXTEDITING:

        }
    }
}

void updateGUI(int GUI_Index) {
    SDL_Color Color = {guiArray[GUI_Index].TextColor.R, guiArray[GUI_Index].TextColor.G, guiArray[GUI_Index].TextColor.B};

    SDL_Surface *surfaceMessage = TTF_RenderText_Blended_Wrapped(Arial, guiArray[GUI_Index].Text, Color, 0);
    SDL_Texture *Message = SDL_CreateTextureFromSurface(globalRenderer, surfaceMessage);

    SDL_GetClipRect(surfaceMessage, &guiArray[GUI_Index].rect);
    guiArray[GUI_Index].rect.x = guiArray[GUI_Index].PositionX;
    guiArray[GUI_Index].rect.y = guiArray[GUI_Index].PositionY;

    guiArray[GUI_Index].TextureText = Message;
    SDL_FreeSurface(surfaceMessage);
}

void updateAllGUI() {
    for (int i = 0; i <= lastGUI_item; i++) {
        updateGUI(i);
    }
}

void handleGUI(int currentGUI) {
    int isColliding = CollosionRectPoint(guiArray[currentGUI], MouseX, MouseY);

    if ((isColliding && guiArray[currentGUI].Type == TEXTBUTTON) || (isColliding && guiArray[currentGUI].Type == IMAGEBUTTON) || (isColliding && guiArray[currentGUI].Type == TEXTBOX)) {
        if (guiArray[currentGUI].Hovered == 0) {
            guiArray[currentGUI].BackgroundColor.R -= 50;
            guiArray[currentGUI].BackgroundColor.G -= 50;
            guiArray[currentGUI].BackgroundColor.B -= 50;
        }

        guiArray[currentGUI].Hovered = 1;

        if (leftButtonDown) {
            // callback
            if (guiArray[currentGUI].Pressed == 0)
                guiArray[currentGUI].MouseDown(currentGUI);

            // property change
            guiArray[currentGUI].Pressed = 1;
            cursor = strlen(guiArray[currentGUI].Text);

            // other
            if (isFocused && guiArray[currentGUI].Type == TEXTBOX) {
                currentGUI_Focused = currentGUI;
            } else if (!isFocused) {
                SDL_StartTextInput();

                currentGUI_Focused = currentGUI;
                isFocused = 1;
                guiArray[currentGUI].Focused = 1;
            }

        }
        else {
            guiArray[currentGUI].Pressed = 0;
        }
    }
    else if ((!isColliding && guiArray[currentGUI].Type == TEXTBUTTON) || (!isColliding && guiArray[currentGUI].Type == IMAGEBUTTON) || (!isColliding && guiArray[currentGUI].Type == TEXTBOX)) {
        if (guiArray[currentGUI].Hovered == 1) {
            guiArray[currentGUI].BackgroundColor.R += 50;
            guiArray[currentGUI].BackgroundColor.G += 50;
            guiArray[currentGUI].BackgroundColor.B += 50;
        }

        guiArray[currentGUI].Hovered = 0;
        guiArray[currentGUI].Pressed = 0;
    }
}

int ConstructGUI(enum GUI_TYPE GUI) {
    if (lastGUI_item < 100) {
        SDL_Color BLACK = {0, 0, 0};

        struct Color3 TextColor = initColor3(0, 0, 0);
        struct Color3 BackgroundColor = initColor3(255, 255, 255);

        struct guiProperties newGUI;
        lastGUI_item += 1;

        newGUI.Type = GUI;
        newGUI.Hovered = 0;
        newGUI.Pressed = 0;

        newGUI.PositionX = 0;
        newGUI.PositionY = 0;
        newGUI.SizeX = 70;
        newGUI.SizeY = 25;

        newGUI.BorderSize = 1;

        newGUI.TextColor = TextColor;
        newGUI.BackgroundColor = BackgroundColor;

        // Ensure that no pointer is left which could trigger an error
        newGUI.MouseEnter = defaultCallback;
        newGUI.MouseDown = defaultCallback;
        newGUI.FocusLost = defaultCallback;
        newGUI.MouseLeave = defaultCallback;

        SDL_Surface *surfaceMessage = TTF_RenderText_Blended(Arial, "Text", BLACK);
        SDL_Texture *Message = SDL_CreateTextureFromSurface(globalRenderer, surfaceMessage);

        char *alloc = (char*)malloc(5 * sizeof(char));

        if (!alloc) {
            printf("[PUIUS GUI] Failed to allocate space for string.");
            return -1;
        }

        newGUI.Text = alloc;

        strcpy(newGUI.Text, "Text");
        newGUI.TextureText = Message;

        if (GUI == TEXTLABEL) {
            newGUI.TextEditable = 0;
        } else if (GUI == TEXTBUTTON) {
            newGUI.TextEditable = 0;
        } else if (GUI == TEXTBOX) {
            newGUI.TextEditable = 1;
        } else if (GUI == IMAGEBUTTON) {
            newGUI.TextEditable = 0;
        } else if (GUI == IMAGELABEL) {
            newGUI.TextEditable = 0;
        }

        guiArray[lastGUI_item] = newGUI;
        SDL_FreeSurface(surfaceMessage);
        return lastGUI_item;
    } else {
        return -1;
    }
}

void renderGUI() {
    for (int i = 0; i < 100; i++) {
        if (i > lastGUI_item)
            break;

        handleGUI(i);
        struct Color3 color = initColor3(0, 0, 0);

        // SDL_RenderDrawLine(globalRenderer, guiArray[i].PositionX, guiArray[i].PositionY, guiArray[i].SizeX, guiArray[i].PositionY);
        // (x, y) - topleft
        // (x + sx, y) - topright
        // (x, y + sy) - bottomleft
        // (x + sx, y + sy) - bottomright

        SDL_Point topLeft = initPoint(guiArray[i].PositionX, guiArray[i].PositionY);
        SDL_Point topRight = initPoint(guiArray[i].PositionX + guiArray[i].SizeX, guiArray[i].PositionY);
        SDL_Point bottomLeft = initPoint(guiArray[i].PositionX, guiArray[i].PositionY + guiArray[i].SizeY);
        SDL_Point bottomRight = initPoint(guiArray[i].PositionX + guiArray[i].SizeX, guiArray[i].PositionY + guiArray[i].SizeY);

        if(guiArray[i].Type != IMAGELABEL && guiArray[i].Type != IMAGEBUTTON) {
            DrawRectangleRec(guiArray[i]);
            DrawText(guiArray[i]);
        }
        else
            DrawTextureEx(guiArray[i]);

        DrawLine(topLeft.x, topLeft.y, topRight.x, topRight.y, color);
        DrawLine(topLeft.x, topLeft.y, bottomLeft.x, bottomLeft.y, color);
        DrawLine(bottomRight.x, bottomRight.y, bottomLeft.x, bottomRight.y, color);
        DrawLine(bottomRight.x, bottomRight.y, topRight.x, topRight.y,color);
    }
}

int initLayer(SDL_Renderer *renderer, SDL_Window *window) {
    globalRenderer = renderer;
    globalWindow = window;

    Arial = TTF_OpenFont("arial.ttf", 16);

    return 1;
}

void uninitLayer() {
    for (int i = 0; i < lastGUI_item; i++) {
        SDL_DestroyTexture(guiArray[i].TextureText);
        free(guiArray[i].Text);
    }
}
