/*
Silent Radiance wifi broadcast for digital silent disco.
Copyright (C) 2017-2019 Hibbard M. Engler

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






//
// Created by hib on 6/17/16.
//


#include "simple_processor.h"


#ifndef PACKETMAXSIZE
#define PACKETMAXSIZE 4000
#endif

#ifndef logit
#define logit(...) {fprintf(stderr,__VA_ARGS__);fprintf(stderr,"\n");}
#endif

OpusDecoder *opusdecoder=NULL;

static int frame=0;
FILE *write_file_spec= NULL;

volatile int soundringhead=0; /* start of the soundringqueue. normally incremented by sound.  But could overflow */
volatile int soundringfirst=0; /* also incrmented by processor.c - probably the first viable sound in the ring */
volatile int soundringtail=0; /* the end on the sound ring. this is incremented by processor.c whenever a packet is added */
volatile int soundringcurrent=-1; /* we are playing this RIGHT NOW: -1 if we are not playing.  This is updated by the sound subsystem */

/*
collections :  either char opus or short, depending on if the length is negative (opus) or positive (PCM). ordered by frame number. has gaps.
            collectioncommands - only filled on a few.

commands -> the commands active via frame

collectioncommands - commands that were loaded for the packet but not yet assigned to their commandring/soundring.
This is for SR01 and SR00.
When the soundring is incremented, the commandring is swapped to point to a collection command, again saving us to copy.

soundring - goes to sound.
commandring - list of latest commands - useful for SR01, SR00 not so much
commandring is congruent with soundring.   Where collectioncommands is loose.

	     Note - the buffers in collections are swapped with the buffers in soundring.   This is done as a quick way to change soundrings without having to copy memory - again.
	                  
            -> sound
*/	    


volatile short *soundring[SOUNDRING_COUNT];  /* points to soundring_area */
volatile short soundring_area[SOUNDRING_COUNT][960];




  
  
volatile struct processor_stat soundstat_area[SOUNDRING_COUNT];
volatile struct processor_stat *soundstat[SOUNDRING_COUNT];

volatile char *commandring[SOUNDRING_COUNT]; /* points to commandring_area */
volatile char commandring_area[SOUNDRING_COUNT][2000]; /* commands for the sound ring */
volatile int commandlen[SOUNDRING_COUNT];





/* This handles the multi-packet stuff */
int base_frame=-1;
int collectionhead=0;
short *collections[COLLECTION_COUNT]; /* points to colections_area */
volatile short collections_area[COLLECTION_COUNT][960];

volatile struct processor_stat collectionstat_area[COLLECTION_COUNT];
volatile struct processor_stat *collectionstat[COLLECTION_COUNT];


unsigned char *collectioncommands[COLLECTION_COUNT]; /* points to collectioncommands_area */
volatile char collectioncommands_area[COLLECTION_COUNT][2000]; /* shared with commandring_area */
int collectioncommandlen[COLLECTION_COUNT];
int collectioncommandcount=0;
int command_frame[COLLECTION_COUNT];/* -1 if unknown and need to be processed */

int collection_size;



int opus_frame=-1;
float minpcm=0.;
float maxpcm=0.;


void init_processor() {
opusdecoder = NULL;
frame = 0;

    { /* opus */
        int err;
        opusdecoder = opus_decoder_create((opus_int32)(48000),2,&err);
        if (err != OPUS_OK)
        {
            logit("OPUS decoder fail %d\n",err);
            opusdecoder = NULL; /* error - decode will just be off */
	    return;
        }

    }

/* init soundrings */
int i;
for (i=0;i<SOUNDRING_COUNT;i++) {
  soundring[i] =soundring_area[i];
  }
soundringhead=0;
soundringtail=0;
soundringcurrent=-1;
return;
}



void shutdown_processor() {
}


int process_packet_main(int flag_override,int recvStringLen,unsigned char *packetbuffer) {

int justwrite=0;
short *soundBuffer;
int soundBufferSize = 0;
//fprintf(stderr,"process_packet_main override %d sampleSize %d\n",flag_override,sampleSize);
if (!opusdecoder) return -1;
    
int sampleSize=recvStringLen;




if (sampleSize<12+2) return -1;
if (sampleSize >PACKETMAXSIZE-2) {
//  logit("sampleSize too big");
  return -1;
  }

// should handle sr00 and sr01 - just reads the one packet    



unsigned int flag=0;
{ /* compute flag */
        int l = packetbuffer[0]+packetbuffer[1]*256+PER_FRAME_OVERHEAD;
	if (l<=PER_FRAME_OVERHEAD) {
	  logit("bad collection no first frame");
	  return -2;
	  }
	if (l>=sampleSize) {
	  logit("bad collection first length too big: %d",l);
	  return -3;
	  }
}

    
    
int framestart;
int framesize;
/* figure out how many opus packets are there. */
if ((packetbuffer [0])||(packetbuffer[1]) ) {
  framestart=2;
  unsigned int pl1 = packetbuffer[0];
  unsigned int pl2 = packetbuffer[1]; 
  pl2 = pl2 <<8;

  framesize= pl1+pl2;
  }
else return (-5);

int opus_result;
float pcm[480];
soundBuffer = (short *)soundring_area[soundringtail];
soundBufferSize = 0;
opus_result = opus_decode_float(opusdecoder,((unsigned char *)(packetbuffer))+2,
                                            (opus_int32)(framesize),pcm,(int)240,(int)0);
  int i;
  if (opus_result != 240) {
    logit("opus_Result is %d not 240\n",opus_result);
    return(-6);
    }
  for (i=0;i<480;i++) {
    short s;
                    //     if (i<3)   	        logit("test %d %f,%f\n",i,pcm[i+i],pcm[i+i+1]);
    float ma;
    float m;
    ma=pcm[i];
    m=ma*32768.;
    if (m>32767.) {
      s=32767;
      }
    else if (m<-32768.) {
      s=-32768;
      }
    else {
      s=(int)m;
      }
    soundBuffer[soundBufferSize++]=s;
    } /* for each sample * 2 channels */

soundringtail= (soundringtail+1) % SOUNDRING_COUNT;
if (soundringhead==soundringtail) {
  soundringhead = (soundringhead+1) % SOUNDRING_COUNT;
  }
return 0;
} /* process_packet_main */








void process_packet_ignore_framestart(int recvStringLen, unsigned char *packetbuffer)
{
int t=process_packet_main(4,recvStringLen,packetbuffer);
if (t) fprintf(stderr,"e%d.",t);
}

void process_packet(int recvStringLen, unsigned char *packetbuffer) 
{
int t=process_packet_main(0,recvStringLen,packetbuffer);
if (t) fprintf(stderr,"e%d.",t);
}
