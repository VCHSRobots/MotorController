/*
 * Utility.h
 *
 * Created: 4/25/2013 9:05:05 AM
 * Author: Dal
 */ 

#ifndef UTILITY_H_
#define UTILITY_H_

#include <stdint.h>

void ByteToAsciiHex(uint8_t v, char *buf);
void WordToAsciiHex(uint16_t v, char *buf);
char HexDigitToChar(uint8_t v);
void RightJustifyInPlace(char *pBuf, uint8_t nFieldSize);
int16_t SignedAddWithOverflowClamp(int16_t a, int16_t b);
uint16_t UnsignedAddWithOverflowClamp(uint16_t a, int16_t b);
void SecondsToHMS(uint32_t t, uint8_t *pHMS);
void HMSToStr(uint8_t *pHMS, char *pBuf);
void I32ToStr(int32_t num, uint8_t iDec, char *pBuf);

// Major format specifiers for ToStr() routine.
#define U_Decimal      0x00   // Output as decimal number
#define U_Hex          0x20   // Output as a hex number
#define U_Binary       0x40   // Output as a binary number
#define U_Boolean      0x60   // Output as a boolean value

// Supplemental format specifications for ToDecimal()
#define U_Signed       0x00   // Input is signed.
#define U_Unsigned     0x01   // Input is unsigned.
#define U_LeadingZeros 0x02   // Include leading zeros on output.
#define U_IncludeSign  0x04   // Include sign, even if positive.
#define U_x1           0x00   // No decimal place
#define U_x10          0x08   // Put decimal before last digit
#define U_x100         0x10   // Put decimal before last two digits
#define U_x1000        0x18   // Put decimal before last three digits

// Supplemental format specifications for ToHex()
#define U_Hex4         0x20   // For Four digits
#define U_Hex2         0x21   // For Two digits
#define U_Hex0x        0x22   // To add the 0x in front of the number

// Supplemental format specification for ToBinaryStr()
#define U_Binary0b     0x41   // To add 0b in front of the number (10 chars required).

// Supplemental format specifications for ToBooleanStr()
#define U_TrueFalse    0x60   // Output as "True" or "False"
#define U_YesNo        0x61   // Output as "Yes" or "No"
#define U_TF           0x62   // Output as "T" or "F"

void ToStr(int16_t num, char *pBuf, uint8_t FormatFlags);
void ToDecimalStr(int16_t num, char *pBuf, uint8_t FormatFlags);
void ToHexStr(int16_t num, char *pBuf, uint8_t FormatFlags);
void ToBinaryStr(int16_t num, char *pBuf, uint8_t FormatFlags);
void ToBooleanStr(int16_t num, char *pBuf, uint8_t FormatFlags);

uint8_t StringWrap(const char *pStr, uint8_t nWidth);

#endif /* UTILITY_H_ */