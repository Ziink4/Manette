/*
 * ppm.h
 *
 *  Created on: Oct 12, 2021
 *      Author: Florian Brenot
 */

#ifndef INC_PPM_H_
#define INC_PPM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

extern volatile bool PPM_NEW_DATA;

/**
  * @brief Initializes the PPM data structures
  * @param  timer_freq frequency of the timer used as PPM input
  */
void PPM_Init(uint32_t timer_freq);

uint8_t PPM_GetChannel(unsigned int channel);

/// Handles PPM timer overflow interrupt
void PPM_TimeoutHandler();

/**
  * @brief Handles PPM timer capture interrupt
  * @param  pulse_width timer capture value
  * @return new timer overflow value to set
  */
uint32_t PPM_PulseHandler(uint32_t pulse_width);

#ifdef __cplusplus
}
#endif

#endif /* INC_PPM_H_ */
