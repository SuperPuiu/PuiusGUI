# PuiusGUI
This project aims to give an easy to use, single file GUI library for SDL2.

The header contains multiple functions that you can use, but most important is `InitLayer`, `ConstructGUI` and `UpdateAllGUI`. Additionaly, there are variables such as `IsFocused`, `LastGUI_item` and `CurrentGUI_Focused`. If you need more information, consider checking the documentation.

# Examples
The library contains one example which is used to test most of the PuiusGUI functionality. It is important to note that it can be easily built by using `make`. test.c can be found in the `src/tests`.

# Building
To build, PuiusGUI.c requires SDL.h and SDL_image.h and SDL_ttf.h.

use `make buildSO` and `make install` to build and respectively install the library.
Alternatively, use the cmake file provided.

# License
PuiusGUI.c and PuiusGUI.h are licensed under the MIT license.
