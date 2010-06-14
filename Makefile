
objects = libusb.o touchatag_reader.o touchatag_tag.o touchatag_taglist.o touchatag_time.o

%.o: src/%.c
	gcc -c $< -o $@ -g `pkg-config --cflags glib-2.0` -I/usr/include -lusb -lsqlite3

compile: $(objects)
	gcc $(objects) -o main_polling_all src/touchatag_main_polling_all.c `pkg-config --cflags glib-2.0` `pkg-config --libs glib-2.0` -I/usr/include -lusb -lsqlite3
	gcc $(objects) -o main_reader_info src/touchatag_main_reader_info.c `pkg-config --cflags glib-2.0` `pkg-config --libs glib-2.0` -I/usr/include -lusb -lsqlite3
	gcc $(objects) -o main_reader_blink src/touchatag_main_reader_blink.c `pkg-config --cflags glib-2.0` `pkg-config --libs glib-2.0` -I/usr/include -lusb -lsqlite3
clean:
	rm *.o -fv
