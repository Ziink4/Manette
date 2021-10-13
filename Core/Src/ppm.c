/*
 * ppm.c
 *
 *  Created on: Oct 12, 2021
 *      Author: Florian
 */

#include "ppm.h"

#define NUM_CHANNELS 8
volatile unsigned int ppm[NUM_CHANNELS];

volatile int CURRENT_CHANNEL = -1;

volatile bool PPM_NEW_DATA;
volatile unsigned int TIME_OLD;

uint32_t MINIMAL_INTERVAL = 0;
uint32_t MAXIMAL_INTERVAL = 0;
uint32_t TIMEOUT_INTERVAL = 0;

const uint32_t TIMER_MAX = 0xFFFF;

void PPM_Init(uint32_t timer_freq)
{
  PPM_NEW_DATA = 0;
  CURRENT_CHANNEL = -1;

  MINIMAL_INTERVAL = (1 * timer_freq / 1000); // 1ms = Stick min position
  MAXIMAL_INTERVAL = (2 * timer_freq / 1000); // 2ms = Stick max position
  TIMEOUT_INTERVAL = (3 * timer_freq / 1000); // 3ms = Timeout
}

unsigned char PPM_GetChannel(unsigned int channel)
{
  unsigned int t = ppm[channel];

  // Convert MINIMAL_INTERVAL...MAXIMAL_INTERVAL -> 0...255
  if (t < MINIMAL_INTERVAL) t = MINIMAL_INTERVAL;

  t -= MINIMAL_INTERVAL;
  t /= ((MAXIMAL_INTERVAL - MINIMAL_INTERVAL) / 255);

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

uint32_t PPM_PulseHandler(uint32_t pulse_width)
{
  uint32_t time = pulse_width;
  if (time > TIME_OLD)
  {
    time = time - TIME_OLD;
  }
  else
  {
    time = time + (TIMER_MAX - TIME_OLD) + 1;
  }

  TIME_OLD = pulse_width;

  if (CURRENT_CHANNEL >= 0 && CURRENT_CHANNEL < NUM_CHANNELS)
  {
    ppm[CURRENT_CHANNEL] = time;
  }

  ++CURRENT_CHANNEL;

  return pulse_width + TIMEOUT_INTERVAL;
}
