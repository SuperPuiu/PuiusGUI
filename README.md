# PuiusGUI
This project aims to give an easy to use, single file GUI library for SDL2.

The header contains multiple functions and variables which are useful in creating GUI for your game or application. Everything is documented within docs/api.md and within the header file. The library also outputs errors on its own, unless `GUI_SUPPRESS_ERRORS` is defined.

# Examples
The library contains one example which is used to test most of the PuiusGUI functionality. It is important to note that it can be easily built by using `make`. test.c can be found in the `src/tests`.

# Building
To build, PuiusGUI.c requires SDL.h and SDL_image.h and SDL_ttf.h. It also uses stdlib.h, stdio.h and string.h but they should be already installed with the compiler.

use `make buildSO` and `make install` to build and respectively install the library.
Alternatively, use the cmake file provided.

# License
PuiusGUI.c and PuiusGUI.h are licensed under the MIT license.
