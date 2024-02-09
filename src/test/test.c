#include "PuiusGUI.h"

struct inputStruct input;
int game_running = 1;

void buttonCallback(int guiIndex) {
    guiArray[guiIndex].Visible = 0;
}

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

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_MUL);

    int index1 = ConstructGUI(TEXTBOX);
    int index2 = ConstructGUI(TEXTLABEL);
    int index3 = ConstructGUI(TEXTBUTTON);

    guiArray[index2].PositionX = 200;
    guiArray[index1].PositionY = 100;
    guiArray[index1].PositionX = 100;
    guiArray[index2].PositionY = 200;

    guiArray[index2].TextColor = initColor3(0, 100, 255, 255);
    guiArray[index2].BackgroundColor = initColor3(0, 0, 0, 255);

    guiArray[index2].Text = "Label";
    guiArray[index3].Text = "Button";

    guiArray[index3].MouseDown = buttonCallback;
    guiArray[index1].TextXAlignment = X_CENTER;
    guiArray[index1].TextYAlignment = Y_CENTER;

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
