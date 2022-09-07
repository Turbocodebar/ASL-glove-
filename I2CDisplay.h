/* 
 * File:   I2CDisplay.h
 * Author: Louis Diomande
 *
 * Created on March 30, 2022, 11:57 AM
 */

#ifndef I2CDISPLAY_H
#define	I2CDISPLAY_H

#ifdef	__cplusplus
extern "C" {
#endif

void delayms(unsigned int m);
void lcd_cmd(char Package);
void lcd_init(void);
void lcd_setCursor(char x,char y);
void lcd_printChar(char myChar);
void lcd_printStr(const char s[]);


#ifdef	__cplusplus
}
#endif

#endif	/* I2CDISPLAY_H */

