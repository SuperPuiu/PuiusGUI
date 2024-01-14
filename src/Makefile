CFLAGS=`sdl2-config --cflags`

ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif

buildTest:
	gcc -Wall -c main.c -o bin/test.o
	gcc PuiusGUI.c -Wall bin/test.o -lSDL2_image -lSDL2_ttf `sdl2-config --libs` -o bin/endProduct -lm

run:
	./bin/endProduct
buildSO:
	gcc -c -fPIC PuiusGUI.c -o bin/PuiusGUI.o
	gcc bin/PuiusGUI.o -shared -o bin/libpuiusgui.so
install: bin/libpuiusgui.so
	install -d $(DESTDIR)$(PREFIX)/lib/
	install -m 644 bin/libpuiusgui.so $(DESTDIR)$(PREFIX)/lib/
	install -d $(DESTDIR)$(PREFIX)/include/
	install -m 644 include/PuiusGUI.h $(DESTDIR)$(PREFIX)/include/
