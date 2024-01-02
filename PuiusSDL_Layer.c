#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "include/PuiusSDL_Layer.h"

SDL_Renderer *globalRenderer;
SDL_Window *globalWindow;

struct guiProperties guiArray[100];
int lastGUI_item = 0;

int MouseY, MouseX;
int leftButtonDown = 0;

int CollisionRect(struct rectProperties rect1, struct rectProperties rect2) {
    if (rect1.Position.X < rect2.Position.X + rect2.Size.X
        && rect1.Position.X + rect1.Size.X > rect2.Position.X
        && rect1.Position.Y < rect2.Position.Y + rect2.Size.Y
        && rect1.Position.Y + rect1.Size.Y > rect2.Position.Y)
            return 1;
        return 0;
}

int CollosionRectPoint(struct rectProperties rect, struct Vector2 point) {
    if (point.X > rect.Position.X && point.X < rect.Position.X + rect.Size.X && point.Y > rect.Position.Y && point.Y < rect.Position.Y + rect.Size.Y)
            return 1;
        return 0;
}

float approach(float flGoal, float flCurrent, float deltaTime) {
    float flDifference = flGoal - flCurrent;
    
    if (flDifference > deltaTime)
        return flCurrent + deltaTime;
    if (flDifference < deltaTime)
        return flCurrent - deltaTime;
    
    return flGoal;
}

void DrawRectangleRec(struct rectProperties rectangle) {
    if (globalRenderer) {
        SDL_Rect newRect = {
            rectangle.Position.X,
            rectangle.Position.Y,
            rectangle.Size.X,
            rectangle.Size.Y
        };

        SDL_SetRenderDrawColor(globalRenderer, rectangle.Color.R, rectangle.Color.G, rectangle.Color.B, 255);
        SDL_RenderFillRect(globalRenderer, &newRect);
     } else {
         printf("[PUIUS_LAYER] Tried drawing a rectangle without initializing globalRenderer.\n (use initLayer(SDL_Renderer* renderer) to initialize globalRenderer)");
    }
}

void processInput(struct inputStruct *input) {
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
                input->SDL_QUIT = 1;
                break;
            case SDL_KEYDOWN:
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
                    leftButtonDown = 1;
                    input->LEFT_BUTTON = 1;
                }
                break;
        }
    }

}

void initRect(struct rectProperties *rectanglePointer, struct Vector2 Position, struct Vector2 Size, struct Vector2 Velocity, struct Color3 Color, int active) {
    rectanglePointer->Position = Position;
    rectanglePointer->Size = Size;
    rectanglePointer->Velocity = Velocity;
    rectanglePointer->Color = Color;
    rectanglePointer->Active = active;
}

void initVector2(struct Vector2 *Vector, float X, float Y) {
    Vector->X = X;
    Vector->Y = Y;
}

void handleGUI(int currentGUI) {
    struct Vector2 Vector;
    struct Vector2 Velocity;
    struct rectProperties rect;

    initVector2(&Velocity, 0, 0);
    initVector2(&Vector, MouseY, MouseX);
    initRect(&rect, guiArray[currentGUI].Position, guiArray[currentGUI].Size, Velocity, guiArray[currentGUI].Color, 1);

    int isColliding = CollosionRectPoint(rect, Vector);

    if (isColliding) {
        guiArray[currentGUI].Hovered = 1;
        guiArray[currentGUI].Color.R = 0;

        if (leftButtonDown)
            guiArray[currentGUI].Pressed = 1;
        else
            guiArray[currentGUI].Pressed = 0;
    }
    else {
        guiArray[currentGUI].Hovered = 0;
        guiArray[currentGUI].Color.R = 255;

        guiArray[currentGUI].Pressed = 0;
    }

}

void createButton(struct rectProperties rectangle, char *string) {
    if (lastGUI_item < 100) {
        struct guiProperties newButton;

        newButton.Position = rectangle.Position;
        newButton.Size = rectangle.Size;
        newButton.Text = string;
        newButton.Color = rectangle.Color;

        newButton.Active = 1;
        newButton.Hovered = 0;
        newButton.Pressed = 0;

        lastGUI_item += 1;
        guiArray[lastGUI_item] = newButton;
    }
}

void renderGUI() {
    for (int i = 0; i < 100; i++) {
        if (i > lastGUI_item)
            break;
        struct rectProperties newRect;
        struct Vector2 Velocity;
        Velocity.X = 0;
        Velocity.Y = 0;

        handleGUI(i);

        initRect(&newRect, guiArray[i].Position, guiArray[i].Size, Velocity, guiArray[i].Color, 1);
        DrawRectangleRec(newRect);
    }
}

void initLayer(SDL_Renderer *renderer, SDL_Window *window) {
    globalRenderer = renderer;
    globalWindow = window;
}
