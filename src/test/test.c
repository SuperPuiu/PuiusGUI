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

void CloseGame(int index) {
    Running = 0;
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

    printf("%s\n", FontPath);

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
    InitLayer(renderer, window);

    ChangeDefaultFont(FontPath, 16);

    SDL_Texture *texture = NULL;
    texture = IMG_LoadTexture(renderer, ImagePath);

    int Label1 = ConstructGUI(TEXTLABEL, 0, 0);
    int Button1 = ConstructGUI(TEXTBUTTON, 0, 0);
    int Image1 = ConstructGUI(IMAGELABEL, 0, 0);
    int Box1 = ConstructGUI(TEXTBOX, 0, 0);

    /* *          * */
    /* * LABEL #1 * */
    /* *          * */
    /* Label coordinates */
    GuiArray[Label1].SizeX = 200;
    GuiArray[Label1].SizeY = 20;
    GuiArray[Label1].PositionX = CenterX - GuiArray[Label1].SizeX / 2;
    GuiArray[Label1].PositionY = CenterY - GuiArray[Label1].SizeY + 15;

    /* Label text-related properties */
    GuiArray[Label1].TextXAlignment = X_CENTER;
    GuiArray[Label1].TextYAlignment = Y_CENTER;
    GuiArray[Label1].Text = "PuiusGUI Example";

    /* Label colors */
    GuiArray[Label1].BackgroundColor = BackgroundColor;
    GuiArray[Label1].TextColor = WHITE;
    GuiArray[Label1].BorderColor = WHITE;

    /* Label miscellaneous */
    GuiArray[Label1].TextSize = 16;

    /* *        * */
    /* * BUTTON * */
    /* *        * */
    /* Button coordinates */
    GuiArray[Button1].SizeY = 20;
    GuiArray[Button1].PositionX = CenterX - GuiArray[Button1].SizeX / 2;
    GuiArray[Button1].PositionY = CenterY - GuiArray[Button1].SizeY + 50;

    /* Button text-related properties */
    GuiArray[Button1].Text = "CLOSE";
    GuiArray[Button1].TextXAlignment = X_CENTER;
    GuiArray[Button1].TextYAlignment = Y_CENTER;

    /* Button colors */
    GuiArray[Button1].BackgroundColor = BackgroundColor;
    GuiArray[Button1].TextColor = WHITE;
    GuiArray[Button1].BorderColor = WHITE;

    /* Button callbacks */
    GuiArray[Button1].MouseDown = CloseGame;

    /* *       * */
    /* * IMAGE * */
    /* *       * */
    /* Image coordinates */
    GuiArray[Image1].SizeX = 128;
    GuiArray[Image1].SizeY = 128;
    GuiArray[Image1].PositionX = CenterX - GuiArray[Image1].SizeX / 2;
    GuiArray[Image1].PositionY = CenterY - GuiArray[Image1].SizeY / 2 - 68;

    /* Image miscellaneous */
    GuiArray[Image1].BorderSize = 0;
    GuiArray[Image1].Image = texture;

    /* *     * */
    /* * BOX * */
    /* *     * */
    /* Box coordinates */
    GuiArray[Box1].SizeX = 100;
    GuiArray[Box1].SizeY = 35;
    GuiArray[Box1].PositionX = GuiArray[Button1].PositionX + GuiArray[Box1].SizeX;
    GuiArray[Box1].PositionY = GuiArray[Button1].PositionY - 10;

    /* Box colors */
    GuiArray[Box1].BackgroundColor = BackgroundColor;
    GuiArray[Box1].TextColor = WHITE;
    GuiArray[Box1].BorderColor = WHITE;

    /* Box text-related properties */
    GuiArray[Box1].TextWrapped = true;
    GuiArray[Box1].TextSize = 12;

    UpdateAllGUI();
    while(Running == 1) {
        ProcessInput();

        if(Inputs[41] == 1)
            Running = 0;

        SDL_SetRenderDrawColor(renderer, 33, 33, 33, 255);
        SDL_RenderClear(renderer);
        RenderGUI();

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    UninitLayer();
}
