CFLAGS=`sdl2-config --cflags`

buildTest:
	gcc -Wall -c main.c -o bin/test.o
	gcc PuiusGUI.c -Wall bin/test.o -lSDL2_image -lSDL2_ttf `sdl2-config --libs` -o bin/endProduct -lm

run:
	./bin/endProduct
buildSO:
	gcc -c -fPIC PuiusGUI.c -o bin/PuiusGUI.o
	gcc bin/PuiusGUI.o -shared -o bin/libpuiusgui.so
install:
	sudo cp bin/libpuiusgui.so /usr/local/lib/libpuiusgui.so
	sudo cp include/PuiusGUI.h /usr/local/include/puiusgui/PuiusGUI.h
