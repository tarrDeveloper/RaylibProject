system: main_raylib main main_alsa 

# fo debug:
#CFLAGS = -g
CFLAGS = -O3

main:	main.o 
	 gcc  $(CFLAGS) -Wl,-rpath -Wl,LIBDIR  -o main  main.o -L/usr/local/lib  \
	    /usr/lib/x86_64-linux-gnu/libcurl.so  -lraylib -lGL -lpthread -ldl -lrt -lX11 -lm -DPLATFORM_DESKTOP -pthread

main_alsa:	main_alsa.o simple_curl_web_stream.o simple_decrypt.o simple_processor.o simple_seven_eight_decode.o simple_sound_alsa.o simple_packet_summary.o
	 gcc  $(CFLAGS) -Wl,-rpath -Wl,LIBDIR  -o main_alsa  main_alsa.o -L/usr/local/lib  simple_curl_web_stream.o simple_decrypt.o simple_processor.o \
	    simple_seven_eight_decode.o simple_sound_alsa.o simple_packet_summary.o \
	    /usr/lib/x86_64-linux-gnu/libcurl.so -lasound -lopus -lraylib -lGL -lpthread -ldl -lrt -lX11 -lm -DPLATFORM_DESKTOP -pthread

main_raylib:	main_raylib.o simple_curl_web_stream.o simple_decrypt.o simple_processor.o simple_packet_summary.o simple_seven_eight_decode.o simple_sound_raylib.o
	 gcc  $(CFLAGS) -Wl,-rpath -Wl,LIBDIR  -o main_raylib  main_raylib.o -L/usr/local/lib  simple_curl_web_stream.o simple_decrypt.o simple_processor.o \
	    simple_seven_eight_decode.o simple_sound_raylib.o simple_packet_summary.o \
	    /usr/lib/x86_64-linux-gnu/libcurl.so -lopus -lraylib -lGL -lpthread -ldl -lrt -lX11 -lm -DPLATFORM_DESKTOP -pthread

main_alsa.o:	../main.cpp
	g++ -c $(CFLAGS) -DPLATFORM_DESKTOP -D_DEFAULT_SOURCE    -DSILENTRADIANCE_ON -DUSING_ALSA -o main_alsa.o ../main.cpp

main_raylib.o:	../main.cpp
	g++ -c $(CFLAGS) -DPLATFORM_DESKTOP -D_DEFAULT_SOURCE    -DSILENTRADIANCE_ON -o main_raylib.o ../main.cpp

main.o:	../main.cpp
	g++ -c $(CFLAGS) -DPLATFORM_DESKTOP -D_DEFAULT_SOURCE  -o main.o ../main.cpp

simple_sound_raylib.o: ../simple_sound_raylib.c
	gcc -c  $(CFLAGS) -Wl,-rpath -Wl,LIBDIR -DPLATFORM_DESKTOP -D_DEFAULT_SOURCE  ../simple_sound_raylib.c
	
simple_curl_web_stream.o: ../simple_curl_web_stream.c
	gcc -c  $(CFLAGS) -Wl,-rpath -Wl,LIBDIR  ../simple_curl_web_stream.c

simple_decrypt.o: ../simple_decrypt.c
	gcc -c  $(CFLAGS) -Wl,-rpath -Wl,LIBDIR  ../simple_decrypt.c

simple_processor.o: ../simple_processor.c
	gcc -c  $(CFLAGS) -Wl,-rpath -Wl,LIBDIR  ../simple_processor.c

simple_seven_eight_decode.o: ../simple_seven_eight_decode.c
	gcc -c  $(CFLAGS) -Wl,-rpath -Wl,LIBDIR  ../simple_seven_eight_decode.c

simple_sound_alsa.o: ../simple_sound_alsa.c
	gcc -c  $(CFLAGS) -Wl,-rpath -Wl,LIBDIR  ../simple_sound_alsa.c


simple_packet_summary.o: ../simple_packet_summary.c
	gcc -c  $(CFLAGS)  -Wl,-rpath -Wl,LIBDIR  ../simple_packet_summary.c
