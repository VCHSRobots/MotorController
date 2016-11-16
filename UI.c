/*
 * UI.c
 *
 * Helper routines that uses KKLcd and the Button hardware to provide
 * a user interface.  
 *
 * The model for these routines is a display that is 128x64 pixels,
 * with 3 buttons underneath.  The buttons are numbered 2, 1, and 0,
 * from left to right.  The X direction is towards the right,
 * and the Y direction is down.  The origin is the top left.  
 * 
 * Unless otherwise indicated, all locations on the display are
 * given in pixels.
 *
 * The supported fonts are: 6x8, 8x12, and 12x16.  Only digits
 * and some grammar is supported in the 12x16 (see KKLcd.c).  
 * These are referred to as "SMALL", "MEDIUM", and "LARGE", or
 * S, M, and L. The origin for an individual character is the
 * top-left.  
 *
 * Note that the normal sequence of calls is:
 *
 *    UI_Setup();  // Done at powerup
 *    UI_Clear();   // Starts a new screen full of data
 *    UI_Str---();  // To write the various strings
 *    UI_Update();  // To cause the display to be updated to show the
 *                  // writes since the last Clear() or Update().
 *
 * That is, you must call UI_Update to make the writes to appear
 * on the screen.
 *
 * Created: 5/28/2013 3:08:53 PM
 * Author: Dal
 */

#include "MainDef.h"
#include "SysClock.h"   // Needed for Debounce
#include "KKLcd.h"      // Needed for LCD Display
#include "Utility.h"    // Needed for string conversion
#include "UI.h"
#include <string.h>

// All these must be consistent with KKLcd...
#define MAX_X 128              // Size of Lcd in X
#define MAX_Y 64               // Size of Lcd in Y
#define SMALL_X 6              // Number of pixels in X for small font
#define SMALL_Y 8              // Number of pixels in Y for small font
#define MEDIUM_X 8             // Number of pixels in X for medium font
#define MEDIUM_Y 12            // Number of pixels in Y for medium font
#define LARGE_X 12             // Number of pixels in X for large font
#define LARGE_Y 16             // Number of pixels in Y for large font
#define SMALL_MAXCHARS MAX_X/SMALL_X   // Number of characters per line for small font
#define MEDIUM_MAXCHARS MAX_X/MEDIUM_X // Number of characters per line for small font
#define LARGE_MAXCHARS MAX_X/LARGE_X   // Number of characters per line for small font

uint8_t giUiX = 0;           // Current cursor location in X
uint8_t giUiY = 0;           // Current cursor location in Y
uint8_t giUiLastFontSize = 8;  // Font size used on last output.
extern uint8_t KKLcd_X0; // KKLcd's location of X.
extern uint8_t KKLcd_Y0; // KKLcd's location of Y.
 
 // -------------------------------------------------------
 // UI_Setup()
 // Called at powerup to setup the LCD and buttons.
 void UI_Setup()
 {
    DDRC &= ~(_BV(Button_0) | _BV(Button_1) | _BV(Button_2));  // Set as input pins
    PORTC |= (_BV(Button_0) | _BV(Button_1) | _BV(Button_2));  // Turn the pullups on
    KKLcd_Setup();
 }

// -------------------------------------------------------
// UI_Shutdown()
// Shuts down the UI and configures for minimum power usage
// for the LCD.
void UI_Shutdown()
{
    //!!! Must be written!
}

// --------------------------------------------------------
// UI_SetX()
// Sets the current cursor location for X
void UI_SetX(uint8_t x)
{
    giUiX = x;
}

// --------------------------------------------------------
// UI_SetY()
// Sets the current cursor location for Y
void UI_SetY(uint8_t y)
{
    giUiY = y;
}

// --------------------------------------------------------
// UI_SetXY()
// Set the current cursor location for X and Y.
void UI_SetXY(uint8_t x, uint8_t y)
{
    giUiX = x;
    giUiY = y;
}

// --------------------------------------------------------
// UI_GetX()
// Gets the current cursor location in X.
uint8_t UI_GetX()
{
    return giUiX;
}

// --------------------------------------------------------
// UI_GetY()
// Gets the current cursor location in Y.
uint8_t UI_GetY()
{
    return giUiY;
}

// --------------------------------------------------------
// UI_CRLF()
// Causes the cursor to be move to the beginning of the
// next line.  The size of the last font is used to
// determine where the next line begins.
void UI_CrLf()
{
    giUiX = 0;
    giUiY += giUiLastFontSize;
}

// --------------------------------------------------------
// UI_StrS()
// Prints a zero-terminated string, in RAM, on the LCD
// starting at the current cursor location.  The cursor
// location is updated.  The text is wrapped if 
// necessary.  The small font is used.
void UI_StrS(const char *s)
{
    giUiLastFontSize = 8;
    while(*s != 0)
    {
        KKLcd_X0 = giUiX;
        KKLcd_Y0 = MAX_Y - giUiY - SMALL_Y - 1;
        KKLcd_Put6x8Char(*s++);
        giUiX += SMALL_X;
        if(giUiX > MAX_X - SMALL_X) 
        {
            giUiX = 0;
            giUiY += SMALL_Y;
        }
    }
}

// --------------------------------------------------------
// UI_StrM()
// Prints a zero-terminated string, in RAM, on the LCD
// starting at the current cursor location.  The cursor
// location is updated.  The text is wrapped if
// necessary.  The medium font is used.
void UI_StrM(const char *s)
{
    giUiLastFontSize = 12;
    while(*s != 0)
    {
        KKLcd_X0 = giUiX;
        KKLcd_Y0 = MAX_Y - giUiY - MEDIUM_Y - 1;
        KKLcd_Put8x12Char(*s++);
        giUiX += MEDIUM_X;
        if(giUiX > MAX_X - MEDIUM_X)
        {
            giUiX = 0;
            giUiY += MEDIUM_Y;
        }
    }
}

// --------------------------------------------------------
// UI_StrL()
// Prints a zero-terminated string, in RAM, on the LCD
// starting at the current cursor location.  The cursor
// location is updated.  The text is wrapped if
// necessary.  The large font is used.
void UI_StrL(const char *s)
{
    giUiLastFontSize = 16;
    while(*s != 0)
    {
        KKLcd_X0 = giUiX;
        KKLcd_Y0 = MAX_Y - giUiY - LARGE_Y - 1;
        KKLcd_Put12x16Char(*s++);
        giUiX += LARGE_X;
        if(giUiX > MAX_X - LARGE_X)
        {
            giUiX = 0;
            giUiY += LARGE_Y;
        }
    }
}
 
// --------------------------------------------------------
// UI_StrSP()
// Prints a zero-terminated string, in PGM, on the LCD
// starting at the current cursor location.  The cursor
// location is updated.  The text is wrapped if
// necessary.  The small font is used.
void UI_StrSP(PGM_P s)
{
    giUiLastFontSize = 8;
    while(pgm_read_byte(s) != 0)
    {
        KKLcd_X0 = giUiX;
        KKLcd_Y0 = MAX_Y - giUiY - SMALL_Y;
        char c = pgm_read_byte(s++);
        KKLcd_Put6x8Char(c);
        giUiX += SMALL_X;
        if(giUiX > MAX_X - SMALL_X)
        {
            giUiX = 0;
            giUiY += SMALL_Y;
        }
    }
}

// --------------------------------------------------------
// UI_StrMP()
// Prints a zero-terminated string, in PGM, on the LCD
// starting at the current cursor location.  The cursor
// location is updated.  The text is wrapped if
// necessary.  The medium font is used.
void UI_StrMP(PGM_P s)
{
    giUiLastFontSize = 12;
    while(pgm_read_byte(s) != 0)
    {
        KKLcd_X0 = giUiX;
        KKLcd_Y0 = MAX_Y - giUiY - MEDIUM_Y;
        char c = pgm_read_byte(s++);
        KKLcd_Put8x12Char(c);
        giUiX += MEDIUM_X;
        if(giUiX > MAX_X - MEDIUM_X)
        {
            giUiX = 0;
            giUiY += MEDIUM_Y;
        }
    }
}

// --------------------------------------------------------
// UI_StrLP()
// Prints a zero-terminated string, in PGM, on the LCD
// starting at the current cursor location.  The cursor
// location is updated.  The text is wrapped if
// necessary.  The large font is used.
void UI_StrLP(PGM_P s)
{
    giUiLastFontSize = 16;
    while(pgm_read_byte(s) != 0)
    {
        KKLcd_X0 = giUiX;
        KKLcd_Y0 = MAX_Y - giUiY - LARGE_Y;
        char c = pgm_read_byte(s++);
        KKLcd_Put12x16Char(c);
        giUiX += LARGE_X;
        if(giUiX > MAX_X - LARGE_X)
        {
            giUiX = 0;
            giUiY += LARGE_Y;
        }
    }
}

// --------------------------------------------------------
// UI_StrXYS()
// Prints a zero-terminated string, in RAM, on the LCD
// starting at the given cursor location.  The cursor
// location is updated.  The text is wrapped if
// necessary.  The small font is used.
void UI_StrXYS(uint8_t x, uint8_t y, const char *s)
{
    giUiX = x;
    giUiY = y;
    UI_StrS(s);
}

// --------------------------------------------------------
// UI_StrXYM()
// Prints a zero-terminated string, in RAM, on the LCD
// starting at the given cursor location.  The cursor
// location is updated.  The text is wrapped if
// necessary.  The medium font is used.
void UI_StrXYM(uint8_t x, uint8_t y, const char *s)
{
     giUiX = x;
     giUiY = y;
     UI_StrM(s);   
}

// --------------------------------------------------------
// UI_StrXYL()
// Prints a zero-terminated string, in RAM, on the LCD
// starting at the given cursor location.  The cursor
// location is updated.  The text is wrapped if
// necessary.  The large font is used.
void UI_StrXYL(uint8_t x, uint8_t y, const char *s)
{
     giUiX = x;
     giUiY = y;
     UI_StrL(s);
}
  

// --------------------------------------------------------
// UI_StrXYSP()
// Prints a zero-terminated string, in PGM, on the LCD
// starting at the given cursor location.  The cursor
// location is updated.  The text is wrapped if
// necessary.  The small font is used.
void UI_StrXYSP(uint8_t x, uint8_t y, PGM_P s)
{
    giUiX = x;
    giUiY = y;
    UI_StrSP(s);
}

// --------------------------------------------------------
// UI_StrXYMP()
// Prints a zero-terminated string, in PGM, on the LCD
// starting at the given cursor location.  The cursor
// location is updated.  The text is wrapped if
// necessary.  The medium font is used.
void UI_StrXYMP(uint8_t x, uint8_t y, PGM_P s)
{
    giUiX = x;
    giUiY = y;
    UI_StrMP(s);
}
// --------------------------------------------------------
// UI_StrXYLP()
// Prints a zero-terminated string, in PGM, on the LCD
// starting at the given cursor location.  The cursor
// location is updated.  The text is wrapped if
// necessary.  The large font is used.
void UI_StrXYLP(uint8_t x, uint8_t y, PGM_P s)
{
    giUiX = x;
    giUiY = y;
    UI_StrLP(s);
}

// --------------------------------------------------------
// UI_NumS()
// Prints a number with the specified format at the
// current location, with the given field width.  The
// Format is composed of the format flags found in
// utility.h.  The max allowed field width is MAXNUMCHARS,
// defined in UI.h. The small font is used.  
void UI_NumS(int16_t num, uint8_t nFieldWidth, uint8_t Format)
{
    if(nFieldWidth > MAXNUMCHARS) nFieldWidth = MAXNUMCHARS;
    char buf[MAXNUMCHARS + 2];
    ToStr(num, buf, Format);
    RightJustifyInPlace(buf, nFieldWidth);
    UI_StrS(buf);
}

// --------------------------------------------------------
// UI_NumM()
// Prints a number with the specified format at the
// current location, with the given field width.  The
// Format is composed of the format flags found in
// utility.h.  The max allowed field width is MAXNUMCHARS,
// defined in UI.h. The medium font is used.
void UI_NumM(int16_t num, uint8_t nFieldWidth, uint8_t Format)
{
    if(nFieldWidth > MAXNUMCHARS) nFieldWidth = MAXNUMCHARS;
    char buf[MAXNUMCHARS + 2];
    ToStr(num, buf, Format);
    RightJustifyInPlace(buf, nFieldWidth);
    UI_StrM(buf);
}

// --------------------------------------------------------
// UI_NumL()
// Prints a number with the specified format at the
// current location, with the given field width.  The
// Format is composed of the format flags found in
// utility.h.  The max allowed field width is MAXNUMCHARS,
// defined in UI.h. The large font is used.
void UI_NumL(int16_t num, uint8_t nFieldWidth, uint8_t Format)
{
    if(nFieldWidth > MAXNUMCHARS) nFieldWidth = MAXNUMCHARS;
    char buf[MAXNUMCHARS + 2];
    ToStr(num, buf, Format);
    RightJustifyInPlace(buf, nFieldWidth);
    UI_StrL(buf);
}

// --------------------------------------------------------
// UI_NumXYS()
// Prints a number with the specified format at the
// given location, with the given field width.  The
// Format is composed of the format flags found in
// utility.h.  The max allowed field width is MAXNUMCHARS,
// defined in UI.h. The small font is used.
void UI_NumXYS(uint8_t x, uint8_t y, int16_t num, uint8_t nFieldWidth, uint8_t Format)
{
    giUiX = x;
    giUiY = y;
    UI_NumS(num, nFieldWidth, Format);
}

// --------------------------------------------------------
// UI_NumXYM()
// Prints a number with the specified format at the
// given location, with the given field width.  The
// Format is composed of the format flags found in
// utility.h.  The max allowed field width is MAXNUMCHARS,
// defined in UI.h. The medium font is used.
void UI_NumXYM(uint8_t x, uint8_t y, int16_t num, uint8_t nFieldWidth, uint8_t Format)
{
    giUiX = x;
    giUiY = y;
    UI_NumM(num, nFieldWidth, Format);
}

// --------------------------------------------------------
// UI_NumXYL()
// Prints a number with the specified format at the
// given location, with the given field width.  The
// Format is composed of the format flags found in
// utility.h.  The max allowed field width is MAXNUMCHARS,
// defined in UI.h. The large font is used.
void UI_NumXYL(uint8_t x, uint8_t y, int16_t num, uint8_t nFieldWidth, uint8_t Format)
{
    giUiX = x;
    giUiY = y;
    UI_NumL(num, nFieldWidth, Format);
}

// --------------------------------------------------------
// UI_ParamS()
// Prints a name/value pair on the current line, and
// then advances the cursor to start at the same X
// position on the line below.  The value is given
// 9 characters on the right side of the display, which
// includes an equal sign. The Name is given the left
// over area.  Nothing is printed if the value cannot
// fit on the line.  The small font is used.
void UI_ParamS(PGM_P name, int16_t v, uint8_t Format)
{
    uint8_t nChars = (MAX_X - giUiX) / SMALL_X;
    if(nChars < 9) return;
    uint8_t iLastX = giUiX;
    uint8_t iLastY = giUiY;
    uint8_t nName = strlen(name);
    uint8_t nRoom = nChars - 9;
    if(nName > nRoom ) nName = nRoom;
    char buf[20];
    if(nName > 0)
    {
        strncpy(buf, name, nName);
        buf[nName + 1] = 0;
        UI_StrL(buf);
    }
    UI_SetX(MAX_X - 9*SMALL_X);
    UI_StrL("=");
    ToStr(v, buf, Format);
    RightJustifyInPlace(buf, 8);
    UI_StrS(buf);
    giUiX = iLastX;
    giUiY = iLastY + SMALL_Y;
}

// --------------------------------------------------------
// UI_ParamM()
// Prints a name/value pair on the current line, and
// then advances the cursor to start at the same X
// position on the line below.  The value is given
// 9 characters on the right side of the display, which
// includes an equal sign. The Name is given the left
// over area.  Nothing is printed if the value cannot
// fit on the line.  The medium font is used.
void UI_ParamM(PGM_P name, int16_t v, uint8_t Format)
{
    uint8_t nChars = (MAX_X - giUiX) / MEDIUM_X;
    if(nChars < 9) return;
    uint8_t iLastX = giUiX;
    uint8_t iLastY = giUiY;
    uint8_t nName = strlen(name);
    uint8_t nRoom = nChars - 9;
    if(nName > nRoom ) nName = nRoom;
    char buf[20];
    if(nName > 0)
    {
        strncpy_P(buf, name, nName);
        buf[nName + 1] = 0;
        UI_StrM(buf);
    }
    UI_SetX(MAX_X - 9*MEDIUM_X);
    UI_StrM("=");
    ToStr(v, buf, Format);
    RightJustifyInPlace(buf, 8);
    UI_StrM(buf);
    giUiX = iLastX;
    giUiY = iLastY + MEDIUM_Y;
}

// --------------------------------------------------------
// UI_ParamL()
// Prints a name/value pair on the current line, and
// then advances the cursor to start at the same X
// position on the line below.  The value is given
// 9 characters on the right side of the display, which
// includes an equal sign. The Name is given the left
// over area.  Nothing is printed if the value cannot
// fit on the line.  The large font is used.
void UI_ParamL(PGM_P name, int16_t v, uint8_t Format)
{
    uint8_t nChars = (MAX_X - giUiX) / LARGE_X;
    if(nChars < 9) return;
    uint8_t iLastX = giUiX;
    uint8_t iLastY = giUiY;
    uint8_t nName = strlen(name);
    uint8_t nRoom = nChars - 9;
    if(nName > nRoom ) nName = nRoom;
    char buf[20];
    if(nName > 0)
    {
        strncpy_P(buf, name, nName);
        buf[nName + 1] = 0;
        UI_StrL(buf);
    }
    UI_SetX(MAX_X - 9*LARGE_X);
    UI_StrS("=");
    ToStr(v, buf, Format);
    RightJustifyInPlace(buf, 8);
    UI_StrS(buf);
    giUiX = iLastX;
    giUiY = iLastY + LARGE_Y;
}

// --------------------------------------------------------
// UI_Line()
// Draws a line on the screen, given the endpoints.
void UI_Line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
    KKLcd_Line(x0, MAX_Y - y0 - 1, x1, MAX_Y - y1 - 1);
}

// --------------------------------------------------------
// UI_Box()
// Draws a box on the screen, given the two corners.
void UI_Box(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
    KKLcd_Box(x0, MAX_Y - y0 - 1, x1, MAX_Y - y1 - 1);
}

// --------------------------------------------------------
// UI_MsgBoxS()
// Writes a message in a box, and waits for the user
// to acknowledge it.  The string is wrapped, broken
// either at spaces or newlines.  An "OK" is displayed at
// the bottom, near button 0.  The medium font is used
// for the title, and then the small font is used for
// the message.  The title can be null, in which
// case the entire box is just the message.
void UI_MsgBoxS(PGM_P pTitle, const char *pMsg)
{
    char tBuf[SMALL_MAXCHARS + 2];

    uint8_t iY = 0;
    if(pTitle) {UI_NewScreen(pTitle); iY = MEDIUM_Y + 2; }
    else UI_Clear();

    uint8_t nLen = strlen(pMsg);
    uint8_t ip = 0;
    while(1)
    {
        if(ip >= nLen) break;
        uint8_t iBrk = StringWrap(pMsg + ip, SMALL_MAXCHARS);
        for(uint8_t j = 0; j < iBrk; j++) tBuf[j] = pMsg[ip + j];
        tBuf[iBrk] = 0;
        ip += iBrk;
        if(pMsg[ip] == ' ' || pMsg[ip] == '\n') ip++;  // Skip spaces and newlines
        UI_StrXYS(0, iY, tBuf);
        iY += SMALL_Y;
        if(iY > MAX_Y - SMALL_Y) break;
    }
    UI_SetXY(MAX_X - 2 * SMALL_X, MAX_Y - SMALL_Y);
    UI_StrSP(PSTR("OK"));
    UI_Update();
    UI_WaitButton(UI_B0);
}

// --------------------------------------------------------
// UI_MsgBoxM()
// Writes a message in a box, and waits for the user
// to acknowledge it.  The string is wrapped, broken
// either at spaces or newlines.  An "OK" is displayed at
// the bottom, near button 0.  The medium font is used
// for the title and also for the message.  The title
// can be null, in which case the entire box is just
// the message.
void UI_MsgBoxM(PGM_P pTitle, const char *pMsg)
{
    char tBuf[MEDIUM_MAXCHARS + 2];

    uint8_t iY = 0;
    if(pTitle) {UI_NewScreen(pTitle); iY = MEDIUM_Y + 2; }
    else UI_Clear();

    uint8_t nLen = strlen(pMsg);
    uint8_t ip = 0;
    while(1)
    {
        if(ip >= nLen) break;
        uint8_t iBrk = StringWrap(pMsg + ip, MEDIUM_MAXCHARS);
        for(uint8_t j = 0; j < iBrk; j++) tBuf[j] = pMsg[ip + j];
        tBuf[iBrk] = 0;
        ip += iBrk;
        if(pMsg[ip] == ' ' || pMsg[ip] == '\n') ip++;  // Skip spaces and newlines
        UI_StrXYM(0, iY, tBuf);
        iY += MEDIUM_Y;
        if(iY > MAX_Y - MEDIUM_Y) break;
    }
    UI_SetXY(MAX_X - 2 * SMALL_X, MAX_Y - SMALL_Y);
    UI_StrSP(PSTR("OK"));
    UI_Update();
    UI_WaitButton(UI_B0);
}

// --------------------------------------------------------
// UI_Options()
// Writes out menu options, in the small font, above
// each button under the display.  Each option can be
// NULL, in which case the corresponding button remains
// unlabeled.
void UI_Options(PGM_P pOption2, PGM_P pOption1, PGM_P pOption0)
{

    if(pOption0)
    {
        uint8_t n = strlen_P(pOption0);
        UI_StrXYSP(MAX_X - n * SMALL_X, MAX_Y - SMALL_Y, pOption0);
    }
    if(pOption1)
    {
        uint8_t n = strlen_P(pOption1);
        uint8_t n2 = n / 2;
        uint8_t ix = 64;  //Center of Screen
        ix -= n2 * SMALL_X;
        if(n2 * 2 != n) ix -= SMALL_X / 2;
        UI_StrXYSP(ix, MAX_Y - SMALL_Y, pOption1);
    }
    if(pOption2)
    {
        UI_SetXY(0, MAX_Y - SMALL_Y);
        UI_StrSP(pOption2);
    }
}

// --------------------------------------------------------
// UI_WaitOptions()
// Writes options (labels) next to each button, and then waits
// for one of the labeled buttons to be pressed.  Returns the
// button that  was pressed.  If a option is NULL, then the
// corresponding button goes unused, and any presses on it
// are ignored.
uint8_t UI_WaitOptions(PGM_P pOption2, PGM_P pOption1, PGM_P pOption0)
{
    UI_Options(pOption2, pOption1, pOption0);
    UI_Update();
    uint8_t Mask = 0;
    if(pOption2) Mask |= UI_B2;
    if(pOption1) Mask |= UI_B1;
    if(pOption0) Mask |= UI_B0;
    if(Mask == 0) return Mask;  // Don't wait if there are no buttons defined.
    return UI_WaitButtonByMask(Mask);
}

// --------------------------------------------------------
// UI_NewScreen()
// Starts a new screen by first clearing the display, then
// writing a title in medium font at the top, and finally
// setting the cursor to the line under the title, plus two
// rows of pixels.
void UI_NewScreen(PGM_P pTitle)
{
    UI_Clear();
    if(pTitle)
    {
        UI_StrMP(pTitle);
        UI_CrLf();
        giUiY += 2;
    }
}

// --------------------------------------------------------
// UI_Clear()
// Clears the screen and resets the cursor to the top
// left edge.
void UI_Clear()
{
    KKLcd_Clear();
    giUiX = 0;
    giUiY = 0;
}

// --------------------------------------------------------
// UI_Update()
// Causes the internal memory buffer to actually be
// displayed on the screen.  Until this call is made
// none of the other calls actually cause a change to 
// what is being shown on the screen.
void UI_Update()
{
    KKLcd_Update();
    giUiX = 0;
    giUiY = 0;
}

// --------------------------------------------------------
// UI_WaitButton()
// Waits on one button and debounces click before returning.
// Button values are defined in UI.h: UI_B0, UI_B1, or UI_B2.
void UI_WaitButton(uint8_t Button)
{
    while(!UI_TestButton(Button));
    UI_DeBounce(Button);
}

// --------------------------------------------------------
// UI_WaitAnyButton()
// Waits for any button to be pressed.  Once a button
// is pressed, it is debounced before returning.
uint8_t UI_WaitAnyButton()
{
    return UI_WaitButtonByMask(UI_B0 | UI_B1 | UI_B2);
}

// --------------------------------------------------------
// UI_WaitButtonByMask()
// Uses a mask to decide which buttons to wait for.  Once
// a button is pressed, it is debounced before returning.
// Button values are defined in UI.h: UI_B0, UI_B1, or UI_B2.
// These can be ORed together to build the mask.
uint8_t UI_WaitButtonByMask(uint8_t Mask)
{
    while(1)
    {
        uint8_t b = UI_GetButtons();
        if((b & Mask) == 0) continue;
        if(b & UI_B0) {UI_DeBounce(UI_B0); return UI_B0; }
        if(b & UI_B1) {UI_DeBounce(UI_B1); return UI_B1; }
        if(b & UI_B2) {UI_DeBounce(UI_B2); return UI_B2; }
        //if(b & UI_B3) {UI_DeBounce(UI_B3); return UI_B3; }
    }
}

// --------------------------------------------------------
// UI_DeBounce()
// De-bounce the given button.  Waits until button settles
// into the inactive state for at least 5ms.  Uses SysClock.c.
// Button values are defined in UI.h: UI_B0, UI_B1, or UI_B2.
void UI_DeBounce(uint8_t Button)
{
    while(1)
    {
        // Wait for the first release...
        while(UI_TestButton(Button));
        // Now wait 5ms.  If any activity during the 5ms, start over.

        uint32_t t1 = GetSystemTime() + 5;
        while(1)
        {
            if(UI_TestButton(Button))
            {
                t1 = GetSystemTime() + 5;
            }
            if(GetSystemTime() > t1) return;
        }
    }
}

// --------------------------------------------------------
// UI_Menu()
// Shows a menu given by an array of Menu Items.  The
// processing for each item in the menu is controlled by
// the contents of the corresponding MenuItem structure.
// If a title is provided, it is printed in the medium font
// at the top of the screen.  The items themselves are shown
// in the small font.
void UI_Menu(PGM_P pTitle, MenuItem *pItems, uint8_t nItems)
{
    uint8_t iTopItem = 0;		  // Menu item at top of current list. Range: 0 to nItems-#Lines.
    uint8_t iMenuPosition = 0;    // Actual menu item being pointed to. Range: 0 to nItems-1.
    int8_t iP = 0;                // Location of Highlighted item on output list. Range: 0 to #Lines - 1
    while(1)
    {
        UI_NewScreen(pTitle);
        uint8_t iY = UI_GetY();
        uint8_t nLines = (MAX_Y - SMALL_Y - iY) / SMALL_Y;
        iTopItem = iMenuPosition - iP;

        for(int iLine = 0; iLine < nLines; iLine++)
        {
            uint8_t iItemPtr = iTopItem + iLine;
            if(iItemPtr >= nItems) break;
            MenuItem *pI = pItems + iItemPtr;
            UI_SetXY(SMALL_Y * 2, iY);
            UI_StrSP(pI->pName);
            if(pI->DisplayRoutine || pI->pValue || pI->AccessFlag & U_ROM)
            {
                char buf[12];
                buf[0] = 0;
                int16_t v = UI_ParamGet(pI);
                if(pI->DisplayRoutine) pI->DisplayRoutine(pI, v, buf);
                else if(pI->pValue || (pI->AccessFlag & U_ROM))
                {
                    ToStr(v, buf, pI->Format);
                }
                RightJustifyInPlace(buf, 9);
                buf[0] = '=';
                UI_StrXYS(MAX_X - 9*SMALL_X, iY, buf);
            }
            if(iP == iLine)
            {
                UI_SetXY(0, iY);
                UI_StrSP(PSTR("->"));
            }
            iY += SMALL_Y;
        }

        uint8_t b = UI_WaitOptions(PSTR("BACK"), PSTR("DOWN"), PSTR("SEL"));
        if(b == UI_B2) return;
        if(b == UI_B1)
        {
            if(iMenuPosition >= nItems - 1)
            {
                // Reset to top.
                iTopItem = 0;
                iMenuPosition = 0;
                iP = 0;
            }
            else
            {
                iMenuPosition++;
                if(iP < nItems - 1) iP++;
                if(iP == nLines - 1)
                {
                    if(iMenuPosition < nItems - 1) iP = nLines - 2;
                }
            }
        }
        if(b == UI_B0)
        {
            if(pItems[iMenuPosition].ServiceRoutine)
            {
                pItems[iMenuPosition].ServiceRoutine(pItems + iMenuPosition);
            }
            else if(pItems->pValue)
            {
                UI_ParamEdit(pItems + iMenuPosition);
            }
        }
    }
}

// --------------------------------------------------------
// UI_ParamEdit()
// Parameter Edit.  Presents a value to the user, according
// to the formatting flags, and allow the user to change it
// while staying within limits.  The updated parameter is
// stored in either RAM or ROM according to the access
// flags.  Returns a status flag: 0=No Change, 1=Changed.
// Entire Screen is used.  
//
uint8_t UI_ParamEdit(MenuItem *pItem)
{
    if(!pItem->pValue && !(pItem->AccessFlag & U_ROM)) return 0;  // No value to edit.

    int16_t wOriginal = UI_ParamGet(pItem);
    int16_t w = wOriginal;

    if(pItem->pName) UI_NewScreen(pItem->pName);
    else UI_NewScreen(PSTR(""));

    UI_ParamDisplay(pItem, w);
    UI_Options(PSTR("DOWN"), PSTR("UP"), PSTR("BACK"));
    UI_Update();

    while(1)
    {
        uint8_t b = UI_GetButtons();
        if(b & UI_B0) {UI_DeBounce(UI_B0); break;}
        if(b & UI_B1) {UI_ParamCount(pItem, &w,  1, UI_B1); }
        if(b & UI_B2) {UI_ParamCount(pItem, &w, -1, UI_B2); }
    }
    uint8_t status = 0;
    if(w != wOriginal) 
    {
        status = 1;
        UI_ParamPut(pItem, w);
    }
    return status;
}

// --------------------------------------------------------
// UI_ParamGet()
// Helper routine for UI_ParamEdit().  Using the information
// in the MenuItem structure, returns the actual data as a int16_t.
int16_t UI_ParamGet(const MenuItem *pItem)
{
	uint16_t v = 0;
	if(pItem->AccessFlag & U_ROM)
    {
        if(pItem->AccessFlag & U_08b) v = (int16_t) eeprom_read_byte((uint8_t *) pItem->pValue);
        else                          v = (int16_t) eeprom_read_word((uint16_t *) pItem->pValue);
    }
    else
	{
    	if(pItem->AccessFlag & U_08b) v = (int16_t) (* ((int8_t *) (pItem->pValue)));
    	else                          v = (int16_t) (* ((int16_t *) (pItem->pValue)));
	}
	return v;
}

// --------------------------------------------------------
// UI_ParamPut()
// Helper routine for UI_ParamEdit().  Using the information
// in the MenuItem structure, stores the actual data to RAM or ROM.
void UI_ParamPut(const MenuItem *pItem, int16_t v)
{
    if(pItem->AccessFlag & U_ROM)
    {
        if(pItem->AccessFlag & U_08b) eeprom_write_byte((uint8_t *)  pItem->pValue, (uint8_t) (v & 0xFF));
        else                          eeprom_write_word((uint16_t *) pItem->pValue, (uint16_t) v);
    }
    else
    {
        uint8_t *pD = (uint8_t *) pItem->pValue;
        uint16_t w = (uint16_t) pD;
        
        if(pItem->AccessFlag & U_08b) *((uint8_t *) w) = (uint8_t) (v & 0xFF);
        else                          *((int16_t *) w) = v;
    }
}

// --------------------------------------------------------
// UI_ParamCount()
// Helper routine for UI_ParamEdit().  Counts the parameter
// while a button is held down.
void UI_ParamCount(MenuItem *pItem, int16_t *pValue, int8_t dir, uint8_t Button)
{
    uint32_t st = GetSystemTime() + 350;
    uint8_t FormatFlags = pItem->Format;
    uint8_t nTen = 0;
    int16_t nAdd = dir;
    uint8_t ModeCount = 0;
    uint8_t bSigned = False;
    uint8_t MajorFormat = FormatFlags & (U_Decimal | U_Hex | U_Binary | U_Boolean);
    if(MajorFormat == U_Decimal && !(FormatFlags & U_Unsigned)) bSigned = True;
    UI_ParamUpdate(pValue, nAdd, pItem->Lim0, pItem->Lim1, bSigned);
    UI_ParamDisplay(pItem, *pValue);
    
    while(1)
    {
        if(!(UI_GetButtons() & Button))
        {
            // The button was released.  No more counting.
            UI_DeBounce(Button);
            return;
        }
        if(GetSystemTime() > st)
        {
            UI_ParamUpdate(pValue, nAdd, pItem->Lim0, pItem->Lim1, bSigned);
            UI_ParamDisplay(pItem, *pValue);
            ModeCount++;
            if(ModeCount > 20)
            {
                ModeCount = 0;
                if(nTen < 3)
                {
                    nTen++;
                    nAdd *= 10;
                }
            }
            st = GetSystemTime() + 200;
        }
    }
}

// --------------------------------------------------------
// UI_ParamDisplay()
// Helper routine for UI_ParamEdit().  Displays a value inside
// a box, in the large font, according to the format flags.  Uses
// the center of the screen -- from raster 22 to 42.
void UI_ParamDisplay(MenuItem *pItem, int16_t v)
{
    if(pItem->DisplayRoutine)
    {
        char buf[12];
        pItem->DisplayRoutine(pItem, v, buf);
        RightJustifyInPlace(buf, 8);
        UI_StrXYL(16, 24, buf);
    }
    else UI_NumXYL(16, 24, v, 8, pItem->Format);
    UI_Box(14, 22, 114, 42);
    UI_Update();
}

// --------------------------------------------------------
// UI_ParamUpdate()
// Helper routine for UI_ParamEdit().  Adds a signed value to
// the parameter and makes sure it stays in the given bounds.
void UI_ParamUpdate(int16_t *pValue, int16_t vAdd, int16_t Lim0, int16_t Lim1, uint8_t bSigned)
{
    if(bSigned)
    {
        int16_t v = SignedAddWithOverflowClamp(*pValue, vAdd);
        if(v > Lim1) v = Lim1;
        if(v < Lim0) v = Lim0;
        *pValue = v;
    }
    else
    {
        uint16_t v = (uint16_t) (*pValue);
        v = UnsignedAddWithOverflowClamp(v, vAdd);
        if(v > (uint16_t) Lim1) v = (uint16_t) Lim1;
        if(v < (uint16_t) Lim0) v = (uint16_t) Lim0;
        *pValue = (int16_t) v;
    }
}

// --------------------------------------------------------
// UI_ScreenList()
// Displays a list of dynamic variables.  Allows the user
// to scroll through them.  If provided, the callback
// is used to update the list.  The callback returns True
// if a value in the list was changed, False otherwise.
// The callback is called with the given token.

void UI_ScreenList(PGM_P pTitle, UIListItem *pItems, uint8_t nItems, void *token, bool8 (*UpdateCallback)(void *))
{
    uint8_t iMenuPosition = 0;  // Item at top of window. Range: 0 to nItems - nLines - 1.
    while(1)
    {
        UI_NewScreen(pTitle);
        uint8_t iY = UI_GetY();
        int8_t nLines = (MAX_Y - SMALL_Y - iY) / SMALL_Y;

        for(int iLine = 0; iLine < nLines; iLine++)
        {
            uint8_t iItemPtr = iMenuPosition + iLine;
            if(iItemPtr >= nItems) break;
            UIListItem *pI = pItems + iItemPtr;
            UI_StrXYS(0, iY, pI->pName);
            UI_NumXYS(MAX_X - 8*SMALL_X, iY, *pI->pValue, 8, pI->Format);
            iY += SMALL_Y;
        }
        UI_Options(PSTR("UP"), PSTR("DOWN"), PSTR("BACK"));
        UI_Update();
        while(1)
        {
            uint8_t b = UI_GetButtons();
            if(b & UI_B0) {UI_DeBounce(UI_B0); return; }
            if(b & UI_B1)
            {
                if(iMenuPosition < (int8_t) nItems - nLines) iMenuPosition++;
            }
            if(b & UI_B2)
            {
                if(iMenuPosition > 0) iMenuPosition--;
            }
            if(UpdateCallback)
            {
                if(UpdateCallback(token)) break;
            }
        }
    }
}

// --------------------------------------------------------
// UI_ByteDump()
// Prints a buffer of bytes on the screen in Hex.  
void UI_ByteDump(PGM_P pTitle, void *pBuf, uint8_t nSize)
{
    uint8_t nItems = nSize / 4; 
    if(nItems * 4 < nSize) nItems++;
    uint8_t iMenuPosition = 0;  // Item at top of window. Range: 0 to nItems - nLines - 1.

    while(1)
    {
        UI_NewScreen(pTitle);
        uint8_t iY = UI_GetY();
        int8_t nLines = (MAX_Y - SMALL_Y - iY) / SMALL_Y;

        for(int iLine = 0; iLine < nLines; iLine++)
        {
            uint8_t iAdr0 = (iMenuPosition + iLine) * 4;
            if(iAdr0 >= nSize) break;
            UI_NumXYS(0, iY, iAdr0, 2, U_Hex2);
            UI_StrSP(PSTR(": "));
            for(int j = 0; j < 4; j++)
            {
                if(iAdr0 + j >= nSize) break;
                uint8_t bb = ((uint8_t *) pBuf)[iAdr0 + j];
                UI_NumS(bb, 3, U_Hex2);
            }
            iY += SMALL_Y;
        }
        uint8_t b = UI_WaitOptions(PSTR("UP"), PSTR("DOWN"), PSTR("BACK"));
        if(b & UI_B0) return;
        if(b & UI_B1)
        {
            if(iMenuPosition < (int8_t) nItems - nLines) iMenuPosition++;
        }
        if(b & UI_B2)
        {
            if(iMenuPosition > 0) iMenuPosition--;
        }
    }
}

// --------------------------------------------------------
// UI_WordDump()
// Prints a buffer of words on the screen in Hex.
void UI_WordDump(PGM_P pTitle, uint16_t *pBuf, uint8_t nSize)
{
    uint8_t nItems = nSize / 2;
    if(nItems * 2 < nSize) nItems++;
    uint8_t iMenuPosition = 0;  // Item at top of window. Range: 0 to nItems - nLines - 1.

    while(1)
    {
        UI_NewScreen(pTitle);
        uint8_t iY = UI_GetY();
        int8_t nLines = (MAX_Y - SMALL_Y - iY) / SMALL_Y;

        for(int iLine = 0; iLine < nLines; iLine++)
        {
            uint8_t iAdr0 = (iMenuPosition + iLine) * 2;
            if(iAdr0 >= nSize) break;
            UI_NumXYS(0, iY, iAdr0, 2, U_Hex2);
            UI_StrSP(PSTR(": "));
            for(int j = 0; j < 2; j++)
            {
                if(iAdr0 + j >= nSize) break;
                uint16_t bb = pBuf[iAdr0 + j];
                UI_NumS(bb, 5, U_Hex4);
            }
            iY += SMALL_Y;
        }
        uint8_t b = UI_WaitOptions(PSTR("UP"), PSTR("DOWN"), PSTR("BACK"));
        if(b & UI_B0) return;
        if(b & UI_B1)
        {
            if(iMenuPosition < (int8_t) nItems - nLines) iMenuPosition++;
        }
        if(b & UI_B2)
        {
            if(iMenuPosition > 0) iMenuPosition--;
        }
    }
}

// --------------------------------------------------------
// UI_StackDump()
// Dumps the top 64 bytes of the stack to the screen, or less
// if the stack isn't that high.
//static uint8_t StackBuf[64];  // To find this in the asm...
void UI_StackDump()
{
    uint16_t nn = 0x40FF - SP;  // This is for ATmega1284 only.
    uint8_t n = (uint8_t) (nn & 0xFF);
    UI_ByteDump(PSTR("Stack"), (uint8_t *) SP, n);
    //// Use static so that these variables are not placed on
    //// the very stack that we want to see.
    //uint8_t n;       
    //uint8_t i;        
    //uint8_t *p;       
//
    //n = (uint8_t) (0x40FF - SP);  // This is for ATmega1284 only!!!!
    //if(n > 64) n = 64;
    //p = (uint8_t *) SP;
    //p++;   // Because we think that the SP points to the next empty spot.
    //for(i = 0; i < n; i++) StackBuf[i] = *p++;
    //UI_ByteDump(PSTR("Stack"), StackBuf, n);
}