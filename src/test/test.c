#include <stdio.h>
#include <stdlib.h>

#include "../include/PuiusGUI.h"

struct inputStruct input;
int game_running = 1;

int main() {
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    SDL_Window *window = SDL_CreateWindow(
        "Test",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        600,
        600,
        SDL_WINDOW_BORDERLESS
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    initLayer(renderer, window);

    int index1 = ConstructGUI(TEXTBOX);
    int index2 = ConstructGUI(TEXTLABEL);
    ConstructGUI(TEXTBUTTON);

    guiArray[index2].PositionX = 200;
    guiArray[index1].PositionY = 100;
    guiArray[index1].PositionX = 100;
    guiArray[index2].PositionY = 200;

    guiArray[index2].TextColor = initColor3(0, 100, 255);

    guiArray[index1].SizeY = 300;
    updateAllGUI();

    while(game_running) {
        processInput(&input);

        if (input.SDL_QUIT || input.ESC) {
            game_running = 0;
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        renderGUI();

        SDL_RenderPresent(renderer);
    }
    return 0;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    uninitLayer();
}
