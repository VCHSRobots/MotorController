/*
 * Utility.c
 *
 * Simple utilities...
 *
 * Created: 4/25/2013 8:59:11 AM
 * Author: Dal
 */ 

#include "MainDef.h"
#include "Utility.h"
#include <string.h>

// --------------------------------------------------------
// ByteToAsciiHex()
// Given a byte, returns two byte string with zero
// terminator to represent the value in hex.  Leading
// zeros are shown.
void ByteToAsciiHex(uint8_t v, char *buf)
{
	buf[0] = HexDigitToChar( ((v >> 4) & 0x0F) );
	buf[1] = HexDigitToChar( (v & 0x0F) );
	buf[2] = 0;
}

// --------------------------------------------------------
// WordToAsciiHex()
// Given a 16-bit word, fills a four char string (with
// zero terminator) to represent the value in hex.
// Leading zeros are shown.
void WordToAsciiHex(uint16_t v, char *buf)
{
	buf[0] = HexDigitToChar( (uint8_t) (((v >> 12) & 0x0F)) );
	buf[1] = HexDigitToChar( (uint8_t) (((v >>  8) & 0x0F)) );
	buf[2] = HexDigitToChar( (uint8_t) (((v >>  4) & 0x0F)) );
	buf[3] = HexDigitToChar( (uint8_t) ((v & 0x0F)) );
	buf[4] = 0;
}

// --------------------------------------------------------
// HexDigitToChar()
// Converts a value from 0-15 to a Hex Character. 
// Capitalized.
char HexDigitToChar(uint8_t v)
{
	if(v <= 9) return v + '0';
	else return v + 'A' - 10;
}

// --------------------------------------------------------
// RighttJustifyInPlace()
// Shifts the input string to the right by adding spaces
// until it is the desired field size.  If the output is
// the same size or longer than the input, nothing is changed.
void RightJustifyInPlace(char *pBuf, uint8_t nFieldSize)
{
	uint8_t nLen = strlen(pBuf);
    if(nLen >= nFieldSize) return;  // Nothing to do.
	//if(nLen > nFieldSize)
    //{
        //// Its a formatting error.  Return a field of stars.
        //for(int i = 0; i < nFieldSize; i++)
        //{
            //*pBuf++ = '*';
        //}
        //*pBuf = 0;
        //return;
    //}
    // All okay... At least one space will be inserted.
	for(int i = 0; i <= nLen; i++)
	{
		// Starting from zero allows zero terminator to be moved as well as the
		// normal characters.
		pBuf[nFieldSize - i] = pBuf[nLen - i];
	}
	uint8_t nSpaces = nFieldSize - nLen;
	memset(pBuf, ' ', nSpaces);
}	

// --------------------------------------------------------
// SignedAddWithOverflowClamp()
// Adds two signed 16-bit numbers while protecting against
// overflow and underflow.
int16_t SignedAddWithOverflowClamp(int16_t a, int16_t b)
{
	if(b > 0 && a > 0)
	{
		if(b > 32767 - a) return 32767;
	}
	if(b < 0 && a < 0)
	{
		if(b < -32768 - a) return -32768;
	}
	return a + b;
}

// --------------------------------------------------------
// SignedAddWithOverflowClamp()
// Adds one signed 16-bit and one unsigned 16-bit number
// together, while protecting against overflow and underflow.
uint16_t UnsignedAddWithOverflowClamp(uint16_t a, int16_t b)
{
	if(b > 0)
	{
		uint16_t room = 65535u - a;
		if((uint16_t) b > room) return 65535;
	}
	else
	{
		if((uint16_t)(0-b) > a) return 0;
	}
	return a + b;
}

// --------------------------------------------------------
// SecondsToHMS()
// Converts a number of seconds into hours, minutes, and
// seconds. Works for inputs up to 255 hours, which is
// a max input value of 918,000 seconds.  The output is
// an array of three bytes: Hours, Minutes, Seconds.
void SecondsToHMS(uint32_t t, uint8_t *pHMS)
{
	if(t > 918000ul) t = 918000ul;
	*pHMS = (uint8_t) (t / 3600ul);
	t -= *pHMS++ * 3600ul;
	*pHMS = (uint8_t) (t / 60ul);
	t -= *pHMS++ * 60ul;
	*pHMS = (uint8_t) t;
}

// --------------------------------------------------------
// HMSToStr()
// Converts a byte array containing Hours, Minutes, Seconds
// into a string with the format hh:mm:ss.
void HMSToStr(uint8_t *pHMS, char *pBuf)
{
	char tBuf[10];
	for(int i = 0; i < 3; i++)
	{
		ToStr(*pHMS++, tBuf, U_LeadingZeros);
		*pBuf++ = tBuf[3];
		*pBuf++ = tBuf[4];
		*pBuf++ = ':';
	}
	*(--pBuf) = 0;
}

// --------------------------------------------------------
// I32ToStr()
// Converts a fixed point number, represented by a 
// 32bit integer, into a string.  The output will be
// 12 chars or less.  The location of the decimal
// is indicated by iDec. iDec=0 means that the decimal
// is on the far right.  A leading zero is provided
// before the decimal if needed.
void I32ToStr(int32_t num, uint8_t iDec, char *pBuf)
{
    if(num < 0) {num = -num; *pBuf++ = '-';}
    uint8_t tBuf[12];
    for(int i = 0; i < 12; i++) tBuf[i] = 0;
    uint8_t nDig = 0;
    for(int i = 0; i < 12 && num != 0 ; i++)
    {
        int32_t NextNum = num / 10;
        tBuf[nDig++] = (uint8_t) (num - (NextNum * 10));
        num = NextNum;
    }
    if(nDig <= iDec) nDig = iDec + 1;
  
    for(int i = nDig - 1; i >= 0; i--)
    {
        *pBuf++ = (char) (tBuf[i] + '0');
        if(i == iDec && i != 0) *pBuf++ = '.';
    }
    *pBuf = 0;
}

// --------------------------------------------------------
// ToStr()
// Top level routine to convert numbers into strings.  
// Uses format flags found in utility.h to format the
// number into a Decimal value, a Hex value, a binary
// value or a boolean value.  Each of these major formats
// have supplemental formats, such as leading zeros, 
// forced sign values, and placement of decimal points.
// 
// The format flags can be ORed together to specify
// a complete format.  A zero input for format flags will
// result in the default output, which is a signed
// decimal value with no leading zeros, and no decimal
// point, and no forced sign character.
//
// The number of output characters will never exceed 10.
// Therefore, the provided buffer should be 12 bytes or
// more for safety. 
//
// For more information, see the comments in the lower level
// routines for each of the major formats.


void ToStr(int16_t num, char *pBuf, uint8_t FormatFlags)
{
    uint8_t MajorFormat = FormatFlags & (U_Decimal | U_Hex | U_Binary | U_Boolean);
    if(MajorFormat == U_Decimal)  {ToDecimalStr(num, pBuf, FormatFlags); return; }
    if(MajorFormat == U_Hex)      {ToHexStr(num, pBuf, FormatFlags); return; }
    if(MajorFormat == U_Binary)   {ToBinaryStr(num, pBuf, FormatFlags); return; }
    if(MajorFormat == U_Boolean)  {ToBooleanStr(num, pBuf, FormatFlags); return; }
}

// --------------------------------------------------------
// ToDecimalStr()
// Converts a fixed point integer value to a string in radix
// 10.  Uses the U_xxx defines for the format flag as follows:
//
// U_Signed       -- Input value is signed.  (Default)
// U_UnSigned     -- Input value is unsigned.
// U_LeadingZeros -- Print leading zeros.
// U_IncludeSign  -- Include the sign, even if positive
// U_x1           -- Input is integer -- do not print decimal (Default)
// U_x10          -- Input is 10ths -- one decimal place
// U_x100         -- Input is 100ths -- two decimal places
// U_x1000        -- Input is 1000ths -- three decimal places
//
// Simply OR the appropriate defines together to build
// the flags value.
//
// At least one digit will be output before the decimal
// point, which may be a leading zero.  All leading
// zeros can be printed if the IntStr_LeadingZeros bit is set.
//
// The provided buffer must be at least 8 bytes in length
// to account for all possible outputs, including the
// zero terminator.

const uint16_t TenTable[] = {1, 10, 100, 1000, 10000};

void ToDecimalStr(int16_t num, char *pBuf, uint8_t FormatFlags)
{
    uint16_t v = (uint16_t) num;
   
    if(!(FormatFlags & U_Unsigned))
    {
        if(num < 0)
        {
            *pBuf++ = '-';
            v = (uint16_t) (0 - num);
        }
        else
        {
            if(FormatFlags & U_IncludeSign) *pBuf++ = '+';
        }
    }
    else
    {
        if(FormatFlags & U_IncludeSign)
        {
            *pBuf++ = '+';
        }
    }

    uint8_t bStart = 0;
    uint8_t nFix = (FormatFlags >> 3) & 0x03;
    if(FormatFlags & U_LeadingZeros) bStart = 1;
    for(int i = 4; i >= 0; i--)
    {
        if(nFix == i || i == 0) bStart = 1;
        //uint16_t t = pgm_read_word(&(TenTable[i]));
        uint16_t t = TenTable[i];
        uint8_t d = v / t;
        v = v - (t * d);
        if(d != 0) bStart = 1;
        if(bStart) *pBuf++ = d + '0';
        if(nFix == i && i != 0) *pBuf++ = '.';
    }
    *pBuf = 0;
}

// --------------------------------------------------------
// ToHexStr()
// Converts a number to a string in radix 16.
//
// Uses the U_xxx defines for the format flag as follows:
//
// U_Hex4     -- Input is treated as a 16-bit word, and 4 hex digits are output.
// U_Hex2     -- Input is treated as a byte, and 2 hex digits are output.
// U_Hex0x    -- "0x" is prepended to the output.
//
// Simply OR the appropriate defines together to build
// the flags value.
//
// The provided buffer must be at least 7 bytes in length
// to account for all possible outputs, including the
// zero terminator.

void ToHexStr(int16_t num, char *pBuf, uint8_t FormatFlags)
{
    FormatFlags &= ~U_Hex;
    if(FormatFlags & U_Hex0x) {*pBuf++ = '0'; *pBuf++ = 'x'; }
    if(FormatFlags & U_Hex2)
    {
        ByteToAsciiHex((uint8_t) num, pBuf);
        pBuf += 2;
    }
    else
    {
        WordToAsciiHex(num, pBuf);
        pBuf += 4;
    }
    *pBuf = 0;
}

// --------------------------------------------------------
// ToBinaryStr()
// Converts a number to a string in radix 2.  Only the lower
// byte of the input is converted.
//
// Uses the U_xxx defines for the format flag as follows:
//
// U_Binary0b  -- "0b" is prepended to the output.
//
// Simply OR the appropriate defines together to build
// the flags value.
//
// The provided buffer must be at least 11 bytes in length
// to account for all possible outputs, including the
// zero terminator.

void ToBinaryStr(int16_t num, char *pBuf, uint8_t FormatFlags)
{
    FormatFlags &= ~U_Binary;
    if(FormatFlags & U_Binary0b) {*pBuf++ = '0'; *pBuf++ = 'b'; }
    for(int i = 0; i < 8; i++)
    {
        if(num & 0x80) *pBuf++ = '1';
        else *pBuf++ = '0';
        num = num << 1;
    }
    *pBuf = 0;
}

// --------------------------------------------------------
// ToBooleanStr()
// Converts a boolean number to a string containing a
// boolean value.  Uses the U_xxx defines for the format flag
// as follows:
//
// U_TrueFalse    -- Output is either "True" or "False". (Default)
// U_YesNo        -- Output is either "Yes" or "No".
// U_TF           -- Output is either "T" or "F".
//
// Simply OR the appropriate defines together to build
// the flags value.
//
// The provided buffer must be at least 6 bytes in length
// to account for all possible outputs, including the
// zero terminator.
//
// NOTE: even though a word is provided for the input,
// only the first byte is used to determine the boolean
// value!

void ToBooleanStr(int16_t num, char *pBuf, uint8_t FormatFlags)
{
    FormatFlags &= ~U_Boolean;
    if(FormatFlags & U_YesNo)
    {
        if(num && 0x00FF) strcpy(pBuf, "Yes");
        else strcpy(pBuf, "No");
    }
    else if(FormatFlags & U_TF)
    {
        if(num && 0x00FF) strcpy(pBuf, "T");
        else strcpy(pBuf, "F");
    }
    else
    {
        if(num && 0x00FF) strcpy(pBuf, "True");
        else strcpy(pBuf, "False");
    }
}

// --------------------------------------------------------
// StringWrap()
// Given a string, and a field width, returns an index at
// a good wrapping (breaking) point, that is less than equal to 
// the field width.  All the characters up to the returned
// index should be printed, but not the character at the index
// itself.  

uint8_t StringWrap(const char *pStr, uint8_t nWidth)
{
    if(nWidth == 0) return 0;
    uint8_t nLen = strlen(pStr);
    // First look for a newline between the start of 
    // the string and the width.
    for(uint8_t i = 0; i < nLen && i < nWidth; i++)
    {
        if(pStr[i] == '\n') return i;
    }

    // If the string length is less than the width,
    // then the decision is easy.
    if(nLen <= nWidth) return nLen;

    // Okay. Start at one past the end, and work
    // backwards until a space is found.

    for(uint8_t i = nWidth; i > 0; i--)
    {
        if(pStr[i] == ' ') return i;
    }

    // Whoops. There is no good place to break.
    // Just break at the end.
    return nWidth;
}


