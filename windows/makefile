# This windows side has alot of libraries that were built with the Silent Radiance windows build.
# So heres a layout
# Main.cpp - the Game.
#   libraylib_static.a - Raylib graphics package.
#     libglfw3.a - A binder between the real graphics system and Conceptual OpenGL world.  Builds windows, has keyboard events, etc.
# simple_curl_web_stream.o - Part of SR. Uses Curl to read the web based server.
#     libcurl.a - a library for curl - a general purpose accesser of web things and ftp things
#        libssl.a  - The secure sockets layer so libcurl can do https: secura calls, SSL is now called TLS
#           libcrypto.a - Cryptogfraphy routines that help libssl verify and decrypt the web feed.
#        libz.a - the Z compression and decompression, used to save time in sending data.
#        wsock32 - windows network socket stuff - I think its needed by libcurl
#        crypt32 - windows cryptography stufd
# ws2_32 - dont know
# mingw32 - interface between a mingw32 built program and general windows programs
# winmm - dont know
# m - math routines, like square root
# pthread - support for multiple threads.
# simple_decrypt.o - an additional encrypt/decrypt of packets which keeps anyone from decoding the stream, as per the US law.
#                   does it need to be there? yes, because it is illegal for prosecuters to decrypt any code. A little gotya if 
#                   copyright becomes an issue. 
# simple_processor.o - a simple processor of the packets that converts them into sound buffers
#    libopus.a       - the opus codec that works like mp3 but is fast for real time compression
# simple_seven_eight_decode.o - code to handle a special 7 bit version of the web stream, which can be used by browsers.
#                              You see, browsers cannot read full binary data, only the utf8 subset, so we convert the 
#                              8 bit data to a save 7 bit for this purpose.
#                              Each streeam server has a download.cgi and a download7.cgi stream for this.
# simple_sound_raylib.o - code to play the sound in raylib.  There are other sound drivers such as alsa, etc that are
#			    interchangeable.  raylib works but is less efficient than alsa. so sdl2 is available for windows as well
#     (libraylib_static.a)    - uses raylib
#       raudio.o              - sometimes added - a submodule of raylib to handle sound
# simple_packet_summary.o     - This code converts the meta data in the stream into readable information about the music
#                                beat, tempo, primary frequency, stuff like that.   This analysis is done by the server
#                                so the client doesnt have to.  If you are interested, libaubio does the work on the server side.


system: main_raylib.exe main.exe

main.exe:	main.o 
	 $(CC)  -g -O2 -Wl,-rpath -Wl,LIBDIR  -o main.exe  main.o  libglfw3.a libraylib_static.a  -L/usr/local/lib  \
	  -L.  -lpthread -lm -DPLATFORM_DESKTOP -lglfw3  -lgdi32 -lwinmm -pthread
	  
main_raylib.exe:	main_raylib.o simple_curl_web_stream.o simple_decrypt.o \
      simple_processor.o simple_seven_eight_decode.o simple_sound_raylib.o \
      simple_packet_summary.o 
	 $(CC)  -g -O2 -Wl,-rpath -Wl,LIBDIR  -o main_raylib.exe  main_raylib.o \
	  libraylib_static.a -L/usr/local/lib  simple_curl_web_stream.o simple_decrypt.o simple_processor.o \
	    simple_packet_summary.o \
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

simple_packet_summary.o: ../simple_packet_summary.c
	$(CC) -c  -g -O2 -I. -Wl,-rpath -Wl,LIBDIR  ../simple_packet_summary.c
