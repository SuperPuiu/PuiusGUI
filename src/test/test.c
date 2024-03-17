/*
 * PUIUS GUI 2024
 * The example was made specifically for unix operating systems. Use the repository's makefile to build.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "../include/PuiusGUI.h"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

struct GuiProperties *Frame;

void ListFunction() {
    if (Frame->Visible == true) {
        Frame->Visible = false;
    } else {
        Frame->Visible = true;
    }

    UpdateAllGUI();
}

int main() {
    struct Color3 BackgroundColor = InitColor3(22, 22, 22, 255);

    char Path[256]; /* Current Working Directory */
    char ImagePath[256 + 26];
    char FontPath[256 + 26];

    getcwd(Path, sizeof(Path));
    getcwd(ImagePath, sizeof(Path));
    getcwd(FontPath, sizeof(Path));

    strcat(ImagePath, "/test/PixelatedPuius.png");
    strcat(FontPath, "/test/font1.ttf");

    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    SDL_Window *window = SDL_CreateWindow(
        "PuiusGUI Example",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_BORDERLESS
    );

    int CenterX = WINDOW_WIDTH / 2;
    int CenterY = WINDOW_HEIGHT / 2;

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    InitGUI(renderer, window);

    ChangeDefaultFont(FontPath, 16);

    SDL_Texture *texture = NULL;
    texture = IMG_LoadTexture(renderer, ImagePath);

    struct GuiProperties *Label1 = PSConstructGUI(TEXTLABEL, 0, 0, 200, 20); /* Could make the arguments more flexible if needed */
    struct GuiProperties *Button1 = PSConstructGUI(TEXTBUTTON, 0, 0, DEFAULT_ELEMENT_WIDTH, DEFAULT_ELEMENT_HEIGHT);
    struct GuiProperties *Box1 = PSConstructGUI(TEXTBOX, 0, 0, 0, DEFAULT_ELEMENT_HEIGHT + 25);
    struct GuiProperties *Image1 = PSConstructGUI(IMAGELABEL, 0, 0, 128, 128);
    Frame = PSConstructGUI(TEXTLABEL, 0, 0, 100, 200);

    /* Construct a list */
    ConstructList(UILIST, Frame->BodyIndex, 0, 0);

    /* *          * */
    /* * LABEL #1 * */
    /* *          * */
    /* Label coordinates */
    Label1->PositionX = CenterX - Label1->SizeX / 2;
    Label1->PositionY = CenterY - Label1->SizeY + 15;

    /* Label text-related properties */
    Label1->TextXAlignment = X_CENTER;
    Label1->TextYAlignment = Y_CENTER;
    Label1->Text = "PuiusGUI Example";

    /* Label colors */
    Label1->BackgroundColor = BackgroundColor;
    Label1->TextColor = WHITE;
    Label1->BorderColor = WHITE;

    /* Label miscellaneous */
    Label1->TextSize = 16;

    /* *        * */
    /* * BUTTON * */
    /* *        * */
    /* Button coordinates */
    Button1->PositionX = CenterX - Button1->SizeX / 2;
    Button1->PositionY = CenterY - Button1->SizeY + 50;

    /* Button text-related properties */
    Button1->Text = "LIST";
    Button1->TextXAlignment = X_CENTER;
    Button1->TextYAlignment = Y_CENTER;

    /* Button colors */
    Button1->BackgroundColor = BackgroundColor;
    Button1->TextColor = WHITE;
    Button1->BorderColor = WHITE;

    /* Button callbacks */
    Button1->MouseDown = ListFunction;

    /* *       * */
    /* * IMAGE * */
    /* *       * */
    /* Image coordinates */
    Image1->PositionX = CenterX - Image1->SizeX / 2;
    Image1->PositionY = CenterY - Image1->SizeY / 2 - 68;

    /* Image miscellaneous */
    Image1->BorderSize = 0;
    Image1->Image = texture;
    Image1->BackgroundColor = InitColor3(0, 0, 0, 0);

    /* *     * */
    /* * BOX * */
    /* *     * */
    /* Box coordinates */
    Box1->SizeX = 100;
    Box1->PositionX = Button1->PositionX + Box1->SizeX;
    Box1->PositionY = Button1->PositionY;

    /* Box colors */
    Box1->BackgroundColor = BackgroundColor;
    Box1->TextColor = WHITE;
    Box1->BorderColor = WHITE;

    /* Box text-related properties */
    // Box1->TextWrapped = true;
    Box1->TextSize = 12;

    /* *        * */
    /* *  FRAME * */
    /* *        * */
    /* Frame coordinates */
    Frame->PositionX = (Button1->PositionX + Button1->SizeX / 2) - Frame->SizeX / 2;
    Frame->PositionY = Button1->PositionY + Button1->SizeY;

    /* Frame Colors */
    Frame->BackgroundColor = BackgroundColor;

    /* Frame miscellaneous */
    Frame->Visible = false;
    Frame->Text = "";

    for (int i = 0; i < 3; i++) {
        struct GuiProperties *NewLabel = PConstructGUI(TEXTLABEL, 0, 0);

        NewLabel->SizeX = Frame->SizeX;
        NewLabel->Parent = Frame->BodyIndex;
        NewLabel->Text = "Test";
    }

    UpdateAllGUI();

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    while(Running == true) {
        ProcessInput();

        if(Inputs[41] == true)
            Running = false;

        SDL_SetRenderDrawColor(renderer, 33, 33, 33, 255);
        SDL_RenderClear(renderer);
        RenderGUI();

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    UninitGUI();
}
