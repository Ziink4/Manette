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

volatile int CURRENT_CHANNEL = -1;

volatile char PPM_NEW_DATA;
volatile unsigned int TIME_OLD;

unsigned int T_MIN = 0;
unsigned int T_MAX = 0;
unsigned int T_OUT = 0;

void PPM_Init(unsigned int timer_freq)
{
  PPM_NEW_DATA = 0;
  CURRENT_CHANNEL = -1;

  T_MIN = (1 * timer_freq / 1000); // 1ms = Stick min position
  T_MAX = (2 * timer_freq / 1000); // 2ms = Stick max position
  T_OUT = (3 * timer_freq / 1000); // 3ms = Timeout
}

unsigned char PPM_GetChannel(unsigned int channel)
{
  unsigned int t = ppm[channel];

  // Convert T_MIN...T_MAX -> 0...255
  if (t < T_MIN) t = T_MIN;

  t -= T_MIN;
  t /= ((T_MAX - T_MIN) / 255);

  if (t > 255) t = 255;

  return ((unsigned char)t);
}

void PPM_TimeoutHandler()
{
  if (CURRENT_CHANNEL != -1)
  {
    PPM_NEW_DATA = 1;
  }

  CURRENT_CHANNEL = -1;
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

  ++CURRENT_CHANNEL;
}
