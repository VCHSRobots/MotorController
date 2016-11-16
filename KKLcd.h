/*
 * KKLcd.h
 *
 * Created: 1/24/2013 8:00:42 AM
 *  Author: Dal
 */ 


#ifndef KKLCD_H_
#define KKLCD_H_

// Color values for Set Pixel
#define BLACK 1
#define WHITE 0

#define LCDWIDTH 128
#define LCDHEIGHT 64

#define LCD_FONT_SMALL 0   // Use this for the 6x8 Font.
#define LCD_FONT_NORMAL 1  // Use this for the 8x12 Font.
#define LCD_FONT_LARGE 2   // Use this for the 12x16 Font.

void KKLcd_Clear(void);
void KKLcd_SetPixel(uint8_t x, uint8_t y, uint8_t color);
uint8_t KKLcd_Getpixel(uint8_t x, uint8_t y);
void KKLcd_Line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void KKLcd_Box(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);

void KKLcd_SetCursor(uint8_t x0, uint8_t y0);

void KKLcd_PrintString6x8(const char *s);
void KKLcd_PrintString8x12(const char *s);
void KKLcd_PrintString12x16(const char *s);
void KKLcd_PrintString6x8_P(const char *s);
void KKLcd_PrintString8x12_P(const char *s);
void KKLcd_PrintString12x16_P(const char *s);

void KKLcd_SetNumParams(uint8_t nDigits, uint8_t radix, uint8_t FontFlag);
void KKLcd_PrintNum(int16_t num);
void KKLcd_PrintDigit(uint8_t iIndex, uint8_t FontFlag);

void KKLcd_Put6x8Char(uint8_t c);
void KKLcd_Put8x12Char(uint8_t c);
void KKLcd_Put12x16Char(uint8_t c);
void KKLcd_Put12x16(uint8_t iIndex);

void KKLcd_Setup(void);
void KKLcd_Update(void);
void KKLcd_SetBrightness(uint8_t val);

void KKLcd_PowerDown();
uint8_t KKLcd_IsPowered();

#endif /* KKLCD_H_ */