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

 
//define SR_NOSOUND
#include <stdio.h>
#include "silentradiancesimple.h"

#include <math.h>
#include <pthread.h>



#ifndef logit
#define logit(...) {fprintf(stderr,__VA_ARGS__);fprintf(stderr,"\n");}
#endif

 



void *SoundPlay(void *arg)
{
init_receiver_sound(1);

while (1) {
  playOneSoundBuffer();
  }
return NULL;
}





int main(int argc,char *argv[]) {
#define PACKETMAXSIZE 4000
unsigned char packetbuffer[PACKETMAXSIZE];
int recvStringLen;


char *url;
if (argc>=2) {
  url=argv[1];
  }
else {
//  url = "https://sr000.silentradiance.com/download.cgi";
//  url = "https://ds.silentradiance.com/download.cgi";
  url = "https://pt.silentradiance.com/download.cgi";
  }
init_web_stream(url);
  
init_processor();

    {
      pthread_t tid[1];
      int err;
      err = pthread_create(tid,NULL,&SoundPlay,NULL);
      if (err != 0) 
         logit("\ncan't create thread :[%s]", strerror(err));
      }


while (1) {
  if(we_are_streaming_web==-1) {break;}
  
  if (get_packet_from_web_stream(&recvStringLen,packetbuffer)) {
    process_packet(recvStringLen,packetbuffer);
    }
  }
}



/* end of file */
