bin_PROGRAMS = abcmusiex abcmusiex-gui
abcmusiex_SOURCES = abcmusiex.c abcmusiex.h definitions.h functions.c functions.h abcmusiex-gui.c
man1_MANS = abcmusiex.1

all: abcmusiex abcmusiex-gui abcmusiex.1 abc2midi abc2abc yaps

.PHONY: clean

abcmusiex.1: abcmusiex
	help2man ./abcmusiex > abcmusiex.1

abcmusiex: abcmusiex.c abcmusiex.h definitions.h functions.c functions.h
	gcc -ggdb -o abcmusiex abcmusiex.c abcmusiex.h definitions.h functions.c functions.h

abcmusiex-gui: abcmusiex-gui.c
	gtk-builder-convert abcmusiex-gui.glade abcmusiex-gui.gtk
	gcc -ggdb -o abcmusiex-gui -lconfig -pthread -I/usr/include/gtk-2.0 -I/usr/lib/x86_64-linux-gnu/gtk-2.0/include -I/usr/include/gio-unix-2.0/ -I/usr/include/cairo -I/usr/include/pango-1.0 -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/pixman-1 -I/usr/include/libpng12 -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/libpng12 -I/usr/include/pango-1.0 -I/usr/include/harfbuzz -I/usr/include/pango-1.0 -I/usr/include/freetype2 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -lgtk-x11-2.0 -lgdk-x11-2.0 -lpangocairo-1.0 -latk-1.0 -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lpangoft2-1.0 -lpango-1.0 -lgobject-2.0 -lfontconfig -lfreetype -Wl,--export-dynamic -lgmodule-2.0 -pthread -lglib-2.0  abcmusiex-gui.c

abc2midi: 
	gcc -DANSILIBS -g    -c -o parseabc.o parseabc.c
	gcc -DANSILIBS -g    -c -o store.o store.c
	gcc -DANSILIBS -g    -c -o genmidi.o genmidi.c
	gcc -DANSILIBS -g    -c -o midifile.o midifile.c
	gcc -DANSILIBS -g    -c -o queues.o queues.c
	gcc -DANSILIBS -g    -c -o parser2.o parser2.c
	gcc -DANSILIBS -g    -c -o stresspat.o stresspat.c
	gcc -o abc2midi parseabc.o store.o genmidi.o queues.o parser2.o midifile.o stresspat.o -lm

abc2abc: 
	gcc -DANSILIBS -g    -c -o toabc.o toabc.c
	gcc -o abc2abc parseabc.o toabc.o

yaps: 
	gcc -DANSILIBS -g    -c -o yapstree.o yapstree.c
	gcc -DANSILIBS -g    -c -o drawtune.o drawtune.c
	gcc -DANSILIBS -g    -c -o debug.o debug.c
	gcc -DANSILIBS -g    -c -o pslib.o pslib.c
	gcc -DANSILIBS -g    -c -o position.o position.c
	gcc -o yaps parseabc.o yapstree.o drawtune.o debug.o position.o pslib.o parser2.o -o yaps

install:
	mkdir -p $(DESTDIR)/usr/bin/
	mkdir -p $(DESTDIR)/usr/share/man/man1/
	mkdir -p $(DESTDIR)/usr/share/abcmusiex/
	mkdir -p $(DESTDIR)/usr/share/applications/
	install -m 0755 abcmusiex $(DESTDIR)/usr/bin/
	install -m 0755 abcmusiex-gui $(DESTDIR)/usr/bin/
	install -m 0755 abc2midi $(DESTDIR)/usr/bin/
	install -m 0755 abc2abc $(DESTDIR)/usr/bin/
	install -m 0755 yaps $(DESTDIR)/usr/bin/
	install -m 0755 abcmusiex.1 $(DESTDIR)/usr/share/man/man1/
	install -m 0755 abcmusiex.png $(DESTDIR)/usr/share/abcmusiex/
	install -m 0755 abcmusiex-gui.gtk $(DESTDIR)/usr/share/abcmusiex/
	install -m 0755 abcmusiex-gui.glade $(DESTDIR)/usr/share/abcmusiex/
	install -m 0755 abcmusiex.desktop $(DESTDIR)/usr/share/applications/

uninstall:
	rm -rf /usr/bin/abcmusiex
	rm -rf /usr/bin/abcmusiex-gui
	rm -rf /usr/share/man/man1/abcmusiex.1
	rm -rf /usr/share/abcmusiex/
	rm -rf /usr/share/applications/abcmusiex.desktop

clean:
	rm -f abcmusiex abcmusiex-gui abcmusiex.1.gz abcmusiex-gu.gtk abc2midi abc2abc yaps *.o

