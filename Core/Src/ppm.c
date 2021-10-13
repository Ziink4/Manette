/*
 * ppm.c
 *
 *  Created on: Oct 12, 2021
 *      Author: Florian
 */

#include "ppm.h"

#define NUM_CHANNELS 8
volatile uint32_t ppm[NUM_CHANNELS];

volatile int CURRENT_CHANNEL = -1;
volatile bool PPM_NEW_DATA;
volatile uint32_t TIME_OLD;

uint32_t MINIMAL_INTERVAL = 0;
uint32_t MAXIMAL_INTERVAL = 0;
uint32_t TIMEOUT_INTERVAL = 0;

const uint32_t TIMER_MAX = 0xFFFF;

void PPM_Init(uint32_t timer_freq)
{
  CURRENT_CHANNEL = NUM_CHANNELS;
  PPM_NEW_DATA = true;
  TIME_OLD = 0;

  MINIMAL_INTERVAL = (1 * timer_freq / 1000); // Stick min position
  MAXIMAL_INTERVAL = (2 * timer_freq / 1000); // Stick max position
  TIMEOUT_INTERVAL = (3 * timer_freq / 1000); // Timeout
}

uint8_t PPM_GetChannel(uint8_t channel)
{
  uint32_t t = ppm[channel];

  // Convert MINIMAL_INTERVAL...MAXIMAL_INTERVAL -> 0...255
  if (t < MINIMAL_INTERVAL)
  {
    t = MINIMAL_INTERVAL;
  }

  t -= MINIMAL_INTERVAL;
  t /= (MAXIMAL_INTERVAL - MINIMAL_INTERVAL) / 255;

  if (t > 255)
  {
    t = 255;
  }

  return ((uint8_t)t);
}

void PPM_TimeoutHandler()
{
  if (CURRENT_CHANNEL == NUM_CHANNELS)
  {
    return;
  }

  PPM_NEW_DATA = true;
  CURRENT_CHANNEL = NUM_CHANNELS;
}

uint32_t PPM_PulseHandler(uint32_t pulse_width)
{
  if (CURRENT_CHANNEL == NUM_CHANNELS)
  {
    // Handling first pulse
    CURRENT_CHANNEL = 0;
  }
  else
  {
    // Handling subsequent pulses
    uint32_t time = pulse_width;

    if (time > TIME_OLD)
    {
      time = time - TIME_OLD;
    }
    else
    {
      time = time + (TIMER_MAX - TIME_OLD) + 1;
    }

    ppm[CURRENT_CHANNEL] = time;

    if (CURRENT_CHANNEL < (NUM_CHANNELS - 1))
    {
      ++CURRENT_CHANNEL;
    }
  }

  TIME_OLD = pulse_width;

  return pulse_width + TIMEOUT_INTERVAL;
}
