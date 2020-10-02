system: main.exe main_raylib.exe

main.exe:	main.o 
	 $(CC)  -g -O2 -Wl,-rpath -Wl,LIBDIR  -o main.exe  main.o  libglfw3.a libraylib_static.a  -L/usr/local/lib  \
	  -L.  -lpthread -lm -DPLATFORM_DESKTOP -lglfw3  -lgdi32 -lwinmm -pthread
main_raylib.exe:	main_raylib.o simple_curl_web_stream.o simple_decrypt.o \
      simple_processor.o simple_seven_eight_decode.o simple_sound_raylib.o 
	 $(CC)  -g -O2 -Wl,-rpath -Wl,LIBDIR  -o main_raylib.exe  main_raylib.o \
	  libraylib_static.a -L/usr/local/lib  simple_curl_web_stream.o simple_decrypt.o simple_processor.o \
	    simple_seven_eight_decode.o simple_sound_raylib.o -L. libraylib_static.a libopus.a libcurl.a libssl.a  libcrypto.a libz.a libglfw3.a \
	    -lwsock32 -lcrypt32 -lws2_32 -lmingw32    -lpthread  -lgdi32 -lwinmm -lm -DPLATFORM_DESKTOP -pthread

main_alsa.o:	../main.cpp
	$(CXX) -c -g -O2 -DPLATFORM_DESKTOP -D_DEFAULT_SOURCE  -DSILENTRADIANCE_ON -DUSING_ALSA -o main_alsa.o ../main.cpp

main.o:	../main.cpp
	$(CXX) -c -g -O2 -I. -DPLATFORM_DESKTOP -D_DEFAULT_SOURCE  -o main.o ../main.cpp

main_raylib.o:	../main.cpp
	$(CXX) -c -g -O2 -I. -DPLATFORM_DESKTOP -D_DEFAULT_SOURCE   -DSILENTRADIANCE_ON -DUSING_ALSA -o main_raylib.o ../main.cpp

simple_sound_raylib.o: ../simple_sound_raylib.c
	$(CC) -c  -g -O2 -I. -Wl,-rpath -Wl,LIBDIR -DPLATFORM_DESKTOP -D_DEFAULT_SOURCE  ../simple_sound_raylib.c
	
simple_curl_web_stream.o: ../simple_curl_web_stream.c
	$(CC) -c  -g -O2 -I. -Wl,-rpath -Wl,LIBDIR   -DCURL_STATICLIB ../simple_curl_web_stream.c

simple_decrypt.o: ../simple_decrypt.c
	$(CC) -c  -g -O2 -I . -Wl,-rpath -Wl,LIBDIR  ../simple_decrypt.c

simple_processor.o: ../simple_processor.c
	$(CC) -c  -g -O2 -I . -Wl,-rpath -Wl,LIBDIR  ../simple_processor.c

simple_seven_eight_decode.o: ../simple_seven_eight_decode.c
	$(CC) -c  -g -O2 -I. -Wl,-rpath -Wl,LIBDIR  ../simple_seven_eight_decode.c

simple_sound_alsa.o: ../simple_sound_alsa.c
	$(CC) -c  -g -O2 -I. -Wl,-rpath -Wl,LIBDIR  ../simple_sound_alsa.c
