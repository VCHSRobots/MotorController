/*
 * UI.h
 *
 * Created: 5/28/2013 3:23:54 PM
 *  Author: Dal
 */ 

#ifndef UI_H_
#define UI_H_

#include "MainDef.h"  // Needed to define buttons
#include "Utility.h"  // Needed for the format flags

#define UI_B0 _BV(Button_0)
#define UI_B1 _BV(Button_1)
#define UI_B2 _BV(Button_2)

#define UI_TestButton(b) (GetButtons() & b)
#define UI_GetButtons()  GetButtons()

#define MAXNUMCHARS 20       // Maximum number of characters for numeric output.

void UI_Setup();
void UI_Shutdown();

void UI_SetX(uint8_t x);
void UI_SetY(uint8_t y);
void UI_SetXY(uint8_t x, uint8_t y);
uint8_t UI_GetX();
uint8_t UI_GetY();

void UI_CrLf();

void UI_StrS(const char *s);
void UI_StrM(const char *s);
void UI_StrL(const char *s);

void UI_StrSP(PGM_P s);
void UI_StrMP(PGM_P s);
void UI_StrLP(PGM_P s);

void UI_StrXYS(uint8_t x, uint8_t y, const char *s);
void UI_StrXYM(uint8_t x, uint8_t y, const char *s);
void UI_StrXYL(uint8_t x, uint8_t y, const char *s);

void UI_StrXYSP(uint8_t x, uint8_t y, PGM_P s);
void UI_StrXYMP(uint8_t x, uint8_t y, PGM_P s);
void UI_StrXYLP(uint8_t x, uint8_t y, PGM_P s);

void UI_NumS(int16_t num, uint8_t nFieldWidth, uint8_t Format);
void UI_NumM(int16_t num, uint8_t nFieldWidth, uint8_t Format);
void UI_NumL(int16_t num, uint8_t nFieldWidth, uint8_t Format);

void UI_NumXYS(uint8_t x, uint8_t y, int16_t num, uint8_t nFieldWidth, uint8_t Format);
void UI_NumXYM(uint8_t x, uint8_t y, int16_t num, uint8_t nFieldWidth, uint8_t Format);
void UI_NumXYL(uint8_t x, uint8_t y, int16_t num, uint8_t nFieldWidth, uint8_t Format);

void UI_ParamS(PGM_P name, int16_t v, uint8_t Format);
void UI_ParamM(PGM_P name, int16_t v, uint8_t Format);
void UI_ParamL(PGM_P name, int16_t v, uint8_t Format);

void UI_Line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void UI_Box(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);

void UI_MsgBoxS(PGM_P pTitle, const char *pMsg);
void UI_MsgBoxM(PGM_P pTitle, const char *pMsg);

void UI_Options(PGM_P pOption2, PGM_P pOption1, PGM_P pOption0);
uint8_t UI_WaitOptions(PGM_P pOption2, PGM_P pOption1, PGM_P pOption0);

void UI_NewScreen(PGM_P pTitle);

void UI_Clear();
void UI_Update();

void UI_WaitButton(uint8_t Button);
uint8_t UI_WaitAnyButton();
uint8_t UI_WaitButtonByMask(uint8_t Mask);
void UI_DeBounce(uint8_t Button);

// Access specifiers for the AccessFlag in the MenuItem structure.
#define U_RAM  0x00   // Means the value is found in RAM (default)
#define U_ROM  0x01   // Means the value is found in EEPROM
#define U_16b  0x00   // Means that the value is a word (default)
#define U_08b  0x02   // Means that the value is a byte

typedef struct _menuitem
{
    PGM_P pName;                 // The name of the menu item.
    void *pValue;                // The value of the item, to show.  (Can be null)
    int16_t Lim0;                // Lower limit for the value.
    int16_t Lim1;                // Upper limit for the value.
    uint8_t AccessFlag;          // How the data is accessed (U_ROM, U_RAM, U_08b, U_16b)
    uint8_t Format;              // The format to show it as.
    void (*DisplayRoutine)(struct _menuitem *pItem, int16_t num, char *outbuf); // The routine to call for formatting.
    void (*ServiceRoutine)(struct _menuitem *pItem);               // The routine to call if selected. 
} MenuItem;



void UI_Menu(PGM_P pTitle, MenuItem *pItems, uint8_t nItems);

uint8_t UI_ParamEdit(MenuItem *pItem);
int16_t UI_ParamGet(const MenuItem *pItem);
void UI_ParamPut(const MenuItem *pItem, int16_t v);
void UI_ParamCount(MenuItem *pItem, int16_t *pValue, int8_t dir, uint8_t Button);
void UI_ParamDisplay(MenuItem *pItem, int16_t v);
void UI_ParamUpdate(int16_t *pValue, int16_t vAdd, int16_t Lim0, int16_t Lim1, uint8_t bSigned);

typedef struct _uilistitem
{
    char *pName;
    uint16_t *pValue;
    uint8_t Format;
} UIListItem;

void UI_ScreenList(PGM_P pTitle, UIListItem *pItems, uint8_t nItems, void *token, bool8 (*UpdateCallback)(void *));
void UI_ByteDump(PGM_P pTitle, void *pBuf, uint8_t nSize);
void UI_WordDump(PGM_P pTitle, uint16_t *pBuf, uint8_t nSize);
void UI_StackDump();

#endif /* UI_H_ */