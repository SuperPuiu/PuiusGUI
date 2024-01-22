# PuiusGUI
This project aims to give an easy to use, single file GUI library to use along with SDL2.

Contains:
- GUI Callbacks support;
- GUI Text Alignment;
- GUI Adjustable text properties;
- Important GUI functions.

The header contains multiple functions that you can use, but most important is `initLayer`, `ConstructGUI` and `updateAllGUI`. Additionaly, there are variables such as `isFocused`, `lastGUI_item` and `currentGUI_Focused`.

# Building
To build, PuiusGUI.c requires SDL.h and SDL_image.h and SDL_ttf.h.

If you have the required libraries installed, use `make buildTest` or simply `make` to build test.c

use `make buildSO` and `make install` to build and respectively install the library.
Alternatively, use the cmake file provided.

# License
PuiusGUI.c and PuiusGUI.h are licensed under the MIT license.
