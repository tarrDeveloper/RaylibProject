system: main.exe

main_alsa:	main_alsa.o simple_curl_web_stream.o simple_decrypt.o simple_processor.o simple_seven_eight_decode.o simple_sound_alsa.o
	 $(CC)  -g -O2 -Wl,-rpath -Wl,LIBDIR  -o main_alsa  main_alsa.o -L/usr/local/lib  simple_curl_web_stream.o simple_decrypt.o simple_processor.o \
	    simple_seven_eight_decode.o simple_sound_alsa.o  \
	    /usr/lib/x86_64-linux-gnu/libcurl.so -lasound -lopus -lraylib -lGL -lpthread -ldl -lrt -lX11 -lm -DPLATFORM_DESKTOP -pthread
main.exe:	main.o 
	 $(CC)  -g -O2 -Wl,-rpath -Wl,LIBDIR  -o main.exe  main.o  libglfw3.a libraylib_static.a  -L/usr/local/lib  \
	  -L.  -lpthread -lm -DPLATFORM_DESKTOP -lglfw3  -lgdi32 -lwinmm -pthread
main_raylib.exe:	main_raylib.o simple_curl_web_stream.o simple_decrypt.o simple_processor.o simple_seven_eight_decode.o simple_sound_raylib.o
	 $(CC)  -g -O2 -Wl,-rpath -Wl,LIBDIR  -o main_raylib.exe  main_raylib.o -L/usr/local/lib  simple_curl_web_stream.o simple_decrypt.o simple_processor.o \
	    simple_seven_eight_decode.o simple_sound_raylib.o  \
	    /usr/lib/x86_64-linux-gnu/libcurl.so -L. -lopus -lraylib -lGL -lpthread -ldl -lrt -lm -DPLATFORM_DESKTOP -pthread

main_alsa.o:	../main.cpp
	$(CXX) -c -g -O2 -DPLATFORM_DESKTOP -D_DEFAULT_SOURCE  -DUSING_ALSA -o main_alsa.o ../main.cpp

main.o:	../main.cpp
	$(CXX) -c -g -O2 -I. -DPLATFORM_DESKTOP -D_DEFAULT_SOURCE  -DUSING_ALSA -o main.o ../main.cpp

main_raylib.o:	../main.cpp
	$(CXX) -c -g -O2 -DPLATFORM_DESKTOP -D_DEFAULT_SOURCE  -DUSING_ALSA -o main_raylib.o ../main.cpp

simple_sound_raylib.o: ../simple_sound_raylib.c
	$(CC) -c  -g -O2 -Wl,-rpath -Wl,LIBDIR -DPLATFORM_DESKTOP -D_DEFAULT_SOURCE  ../simple_sound_raylib.c
	
simple_curl_web_stream.o: ../simple_curl_web_stream.c
	$(CC) -c  -g -O2 -Wl,-rpath -Wl,LIBDIR  ../simple_curl_web_stream.c

simple_decrypt.o: ../simple_decrypt.c
	$(CC) -c  -g -O2 -Wl,-rpath -Wl,LIBDIR  ../simple_decrypt.c

simple_processor.o: ../simple_processor.c
	$(CC) -c  -g -O2 -Wl,-rpath -Wl,LIBDIR  ../simple_processor.c

simple_seven_eight_decode.o: ../simple_seven_eight_decode.c
	$(CC) -c  -g -O2 -Wl,-rpath -Wl,LIBDIR  ../simple_seven_eight_decode.c

simple_sound_alsa.o: ../simple_sound_alsa.c
	$(CC) -c  -g -O2 -Wl,-rpath -Wl,LIBDIR  ../simple_sound_alsa.c
