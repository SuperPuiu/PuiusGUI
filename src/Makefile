CFLAGS=`sdl2-config --cflags`

ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif

buildTest:
	mkdir -p ../build
	gcc -Wall -c -Iinclude test/test.c -o ../build/test.o
	gcc -Iinclude PuiusGUI.c -Wall ../build/test.o -lSDL2_image -lSDL2_ttf `sdl2-config --libs` -o ../build/endProduct -lm

run:
	../build/endProduct
buildSO:
	mkdir -p ../build
	gcc -c -Iinclude/ -fPIC PuiusGUI.c -o ../build/PuiusGUI.o
	gcc -Iinclude ../build/PuiusGUI.o -shared -o ../build/libpuiusgui.so
install: ../build/libpuiusgui.so
	install -d $(DESTDIR)$(PREFIX)/lib/
	install -m 644 ../build/libpuiusgui.so $(DESTDIR)$(PREFIX)/lib/
	install -d $(DESTDIR)$(PREFIX)/include/
	install -m 644 include/PuiusGUI.h $(DESTDIR)$(PREFIX)/include/
