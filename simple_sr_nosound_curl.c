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
#include "simple_curl_web_stream.h"
#include "simple_processor.h"
#include <math.h>

#define PACKETMAXSIZE 4000






int main(int argc,char *argv[]) {
  
init_processor();

init_web_stream("https://pt.silentradiance.com/download.cgi");
  
#define PACKETMAXSIZE 4000
unsigned char packetbuffer[PACKETMAXSIZE];
int recvStringLen;

while (1) {
     if(we_are_streaming_web==-1) {break;}
     if (get_packet_from_web_stream(&recvStringLen,packetbuffer)) {
        process_packet(recvStringLen,packetbuffer);
	fprintf(stderr,".");
	soundringhead = soundringtail;
        }
  }
}
