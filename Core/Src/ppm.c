/*
 * ppm.c
 *
 *  Created on: Oct 12, 2021
 *      Author: Florian
 */

#include "ppm.h"

#define PPM_TCNT TCNT1

#define NUM_CHANNELS 8
volatile unsigned int ppm[NUM_CHANNELS];

volatile int currentChannel = -1;

#define T_MIN (1*F_CPU/1000) // 1ms = 12000 Ticks @ 12Mhz = Stick min position
#define T_MAX (2*F_CPU/1000) // 2ms = 24000 Ticks @ 12Mhz = Stick max position
#define T_OUT (3*F_CPU/1000) // 3ms = 36000 Ticks @ 12Mhz = Timeout

volatile char PPM_NEW_DATA;
volatile unsigned int TIME_OLD;

void PPM_Init()
{
  PPM_NEW_DATA = 0;
  currentChannel = -1;
}

unsigned char PPM_GetChannel(int channel)
{
  return 0;
}

void PPM_TimeoutHandler()
{
  if (currentChannel != -1)
  {
    PPM_NEW_DATA = 1;
  }

  currentChannel = -1;
}

void PPM_PulseHandler()
{
//  unsigned int time = ICR1;
//  OCR1A = time + T_OUT;
//  if (time>timeOld)
//  {
//    time=time-timeOld;
//  }
//  else
//  {
//    time = time + (0xffff - timeOld) + 1;
//  }
//  timeOld = ICR1;
//  if (currentChannel >= 0 && currentChannel < NUM_CHANNELS)
//  {
//    ppm[currentChannel]=time;
//  }

  ++currentChannel;
}
