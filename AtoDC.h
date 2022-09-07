/* 
 * File:   AtoDC.h
 * Author: Louis Diomande
 *
 * Created on April 5, 2022, 8:36 PM
 */

#ifndef ATODC_H
#define	ATODC_H
#include "xc.h"

#ifdef	__cplusplus
extern "C" {
#endif

void adc_init();
void initBuffer();
void timer1_init();
int finger0();
int finger1();
int finger2();
int finger3();
int finger4();



#ifdef	__cplusplus
}
#endif

#endif	/* ATODC_H */

