# PuiusGUI
PuiusGUI is a small, lightweight and compact library using primarily SDL for render backend. By design, PuiusGUI provides a variety of functions and structs useful for rendering plenty of stuff, such as buttons, both image and text labels, image and text buttons and many more. Another important thing to note is the simplicity of the API, which was designed like that and improved since the beginning. The choice of using C as the programming language for this project also makes it easy to port other languages.

# Features
- Single-header library
- Simple and easy to use API
- Documentation (found in api.md)
- Extremely small codebase (~1k Lines of Code)

# Building
Building PuiusGUI.c requires SDL2_ttf, SDL2_image and SDL2 itself. To make it easier, the project also provides a cmake file and a makefile with everything required. 

PuiusGUI contains one example file which tests as many properties as possible in a maneer where it can be seen how easy it is to do it. The file is contained in `src/tests` and the compiling process can be done using both the cmake and make files provided in the repository.
# License
The project is licensed under the MIT license, which can also be found in the header file.
