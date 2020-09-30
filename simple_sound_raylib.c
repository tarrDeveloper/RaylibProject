/*
Silent Radiance wifi broadcast for digital silent disco.
Copyright (C) 2017 Hibbard M. Engler

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License   
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/



/*

This example reads standard from input and writes
to the default PCM device for 5 seconds of data.

*/

#include <stdio.h> /* for printf() and fprintf() */
#include <stdlib.h> /* for atoi() and exit() */
#include <string.h> /* for memset() */
#include <time.h>
#include <sched.h>
#include <unistd.h> /* for close() */
#include <errno.h>

/*#define THREADING_ON 1 
^^^ for threadding example - doesnt help much just burns a cpu */

#include "simple_processor.h"
//#include "sound_generic.h"

#include "simple_sound_raylib.h"

#include "raylib.h"

static volatile int wipe_sound=0;  // quick buffer clear

short zero_buffer[480];

short * getNextQueue240() {

if (wipe_sound) { // quick to help clear when changing the channel
  wipe_sound=0;
  soundringhead = soundringtail;
  soundringcurrent = -1;
  }
  
char status;
   status='z';
      short *nextBuffer=zero_buffer;

  
    if (soundringhead == soundringtail) { /* Empty zero it! */
        status='a';
        soundringcurrent=-1;
    } else {
        if (soundringcurrent==-1) {  /* was emptied. Check to see if we have enough */
            /* Here we should wait until we got at least 4 or so */
            int queue_size;
            queue_size = soundringtail-soundringhead;
            if (queue_size<0) queue_size += SOUNDRING_COUNT;
	    
            if (queue_size>24) { /* ok, we have 24 consecutive ones. Go at it! */
                soundringfirst=soundringhead;
                soundringcurrent=soundringhead;
		status='2';
                }
	    }
        /* OK - here we may or may not be doing real sound.  so do one of the other */
        if (soundringcurrent==-1) { /* still not good enough */
            /* do nothing here - we are already set to zero_buffer */
        }
        else if (soundringcurrent==soundringhead) { /* Good enough */
            soundringcurrent = soundringhead;
            nextBuffer = (short *)soundring[soundringcurrent];
	    status=' ';
            soundringhead = (soundringhead + 1) % SOUNDRING_COUNT;
        } else { /* soundringcurrent is pointing ot the last frame. See if the frames are consecutive */
            soundringcurrent = soundringhead;
            nextBuffer = (short *)soundring[soundringcurrent];
            soundringhead = (soundringhead + 1) % SOUNDRING_COUNT;
            }
        }
//    if (nextBuffer==zero_buffer) {fprintf(stderr,"%c",status);}
    return  nextBuffer;

}


  int init_send_sound=1;
  long loops;
  int rc;
  int size;
  unsigned int val;
  int dir;
  int frames;
  volatile char *buffer;
  struct pollfd *ourpolls;
  int soundpolls_count;
  int socket_poll;
  
  int sound_state = 0;
  
  static int frame=0;
int no_sound = 0;


static AudioStream stream;




int init_receiver_sound(int block) {
  
  InitAudioDevice();              // Initialize audio device

/* block is not an option in rc */
    
  // Init raw audio stream (sample rate: 48000, sample size: 16bit-short, channels: 2-stereo)
  stream = InitAudioStream(48000, 16, 2);
      
  PlayAudioStream(stream);        // Start processing stream buffer (no data loaded currently)
     
int err;

  frames = 240;

  size=96000;
  buffer = (char *) malloc(size);
  

sound_state=0;
{
  int i;
  for (i=0;i<480;i++) {
//    zero_buffer[i]=(i*80)%32768;   /* for debugging */
    zero_buffer[i]=0;
    }
  }

return 0;
}


int finish_play_sound() {
  CloseAudioStream(stream);
  CloseAudioDevice();
  free(buffer);
  buffer=0;
return(0);
}


void DieWithError(char *errorMessage); /* External error handling function */

void DieWithError(char *errorMessage)
{
perror(errorMessage);
exit(1);
}


void finish_stream_sound() {
wipe_sound=1;
soundringhead = soundringtail-1;
if (soundringhead<0) soundringhead +=SOUNDRING_COUNT;
}






 


int playOneSoundBuffer() {
if (IsAudioStreamProcessed(stream)) {
  int i;
  int offset=0;
  for (i=0;i<16;i++) {
    char *soundBuffer;
    int soundBufferSize = 960;
    soundBuffer = (char *)getNextQueue240();
    memcpy(buffer+offset,soundBuffer,soundBufferSize);
    offset += soundBufferSize;
    }
  UpdateAudioStream(stream, buffer, offset/2);
  }
else {
            struct timespec thislong;
             thislong.tv_sec = 0;
             thislong.tv_nsec = 12000000; /* 12 milliseconds */
             nanosleep(&thislong, &thislong);

  }
}



