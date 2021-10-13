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

extern volatile char PPM_NEW_DATA;

void PPM_Init(unsigned int timer_freq);

unsigned char PPM_GetChannel(unsigned int channel);

void PPM_TimeoutHandler();

void PPM_PulseHandler();

#ifdef __cplusplus
}
#endif

#endif /* INC_PPM_H_ */
