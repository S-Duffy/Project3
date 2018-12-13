/*
 * adc.c
 *
 *  Created on: Dec 1, 2018
 *      Author: Sean
 */

#include "stdint.h"
#include "C:\Freescale\KSDK_1.3.0\examples\frdmkl25z\demo_apps\hello_world\kds\inc\uart.h"
#include "C:\Freescale\KSDK_1.3.0\examples\frdmkl25z\demo_apps\hello_world\kds\inc\fifo.h"
#include "board.h"

void ADC_Init(void)
{
  SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;

	ADC0_CFG1 |= (ADC_CFG1_MODE(1) |
			      ADC_CFG1_ADIV(2));

	ADC0_SC1A  = ADC_SC1_ADCH(31);

//  ADC0_SC1A = ADC_SC1_DIFF(1); // 1 =differential mode on
//  ADC0_SC1A = ADC_SC1_ADCH(0); // 0000 = DAD0 input
//  // ADC_CFG1_ADIV(0) = ; // TODO: Determine clock speed
//  ADC0_SC1A = ADC_CFG1_MODE(0x03); // 11 -> 16-bit conversion with DIFF =1
}

uint16_t ADC_Read(void)
{
  ADC0_SC1A = /*channelNumber*/0 & ADC_SC1_ADCH_MASK;
  while(ADC0_SC2 & ADC_SC2_ADACT_MASK);
  while(!(ADC0_SC1A & ADC_SC1_COCO_MASK));
  return ((uint16_t) ADC0_RA);
}
