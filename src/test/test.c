#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../include/PuiusGUI.h"

void buttonCallback(int guiIndex) {
    GuiArray[guiIndex].Visible = 0;
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
    InitLayer(renderer, window);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_MUL);

    int index4 = ConstructGUI(TEXTLABEL, 10, 10);
    int index1 = ConstructGUI(TEXTBOX, 100, 100);
    int index2 = ConstructGUI(TEXTLABEL, 200, 200);
    int index3 = ConstructGUI(TEXTBUTTON, 0, 0);

    GuiArray[index2].TextColor = InitColor3(0, 100, 255, 255);
    GuiArray[index2].BackgroundColor = InitColor3(0, 0, 0, 255);

    GuiArray[index2].Text = "Label";
    GuiArray[index3].Text = "Button";

    GuiArray[index3].MouseDown = buttonCallback;
    GuiArray[index1].TextXAlignment = X_CENTER;
    GuiArray[index1].TextYAlignment = Y_CENTER;

    GuiArray[index1].BorderSize = 5;
    GuiArray[index1].TextWrapped = true;
    GuiArray[index1].TextSize = 32;
    GuiArray[index3].OutlineSize = 1;

    GuiArray[index1].SizeY = 300;

    GuiArray[index2].Parent = index4;
    GuiArray[index2].Zindex = 3;
    GuiArray[index4].Zindex = 1;
    GuiArray[index4].BackgroundColor = InitColor3(0, 0, 0, 255);
    GuiArray[index4].TextColor = InitColor3(255, 255, 255, 255);

    UpdateAllGUI();

    while(Running == 1) {
        ProcessInput();

        if(Inputs[41] == 1)
            Running = 0;

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        RenderGUI();

        SDL_RenderPresent(renderer);
    }

    printf("%s\n", GuiArray[index1].Text);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    UninitLayer();
    return 0;
}
