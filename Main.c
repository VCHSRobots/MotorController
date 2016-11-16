/*
 * Main.c
 *
 * Program for ATmega324P to run a motor Controller.
 * 
 * Epic Robots, VCHS -- Test Equipment 
 *
 * Created: 12/12/2015 DLB
 */

// Changes made to Original Schematic:
//
// 1. Rerouted LCDSCL from PD4 (pin 18) to PD0 (pin 14)
// 2. Rerouted PWM from PC7 (pin 29) to PD4 (pin 18)
// 3. Added 1K pull down resistors to pins 2 and 7 of SN754410.

// Version 1.1

#define MainFile
#include "MainDef.h"
#include "SysClock.h"
#include "UI.h"
#include "ADC.h"
#include "PWM.h"
#include <string.h>
#include <stdio.h>

static void DoUI();
static void ShowTitle();
static void UpdateParams();
static void ControlPWM(int16_t d, bool8 forward);
static void RunMode();
static void ShowMode();
static void refreshTimer();
static int16_t GetPotMC();
//uint32_t g_maxlooptime;
static bool8 s_bForward = True;
static int newTimerMax;
static int ogCap = 300;	// This is the maximum amount of time the program can run without being refreshed.
static int newTimerMax = 300;
// static int timetolive = 300000;  // dal's way

//todo: system time

int main(void)
{
    _delay_ms(100);
    BitOn(DDRB, LedPin);
	BitOn(DDRC, PwrOnRlyPin);
	BitOn(DDRC, MotorRlyPin);
	PwrRelayOn();
	MotorRelayOff();

	BitOff(DDRC, Sw_MOnOffPin);
	BitOff(DDRC, Sw_PwrOnOffPin);
    BitOff(DDRC, Button_0);
    BitOff(DDRC, Button_1);
    BitOff(DDRC, Button_2);
	
    BitOn(PORTC, Button_0);
    BitOn(PORTC, Button_1);
    BitOn(PORTC, Button_2);

    SetupSystemTimeCounter();
    SetSlowInterrupt(GetSystemTime() + 100);
    sei();

    UI_Setup();
	ADC_Enable();
	PWM_Init();

    LedRedOn();
	
	PWM_On(50, 1500);

    ShowTitle();
	//g_maxlooptime = 0;
    while(1)
    {
		//uint32_t t0 = GetSystemTime();
		DoUI();
		//uint32_t t_dly = GetSystemTime() - t0;
		//if(t_dly > g_maxlooptime) g_maxlooptime = t_dly;
	}
}

static void DoUI()
{
	UpdateParams();
			
	uint8_t b = UI_GetButtons();
	
	int16_t d = GetPotMC();
	
	UI_Update();
	
	if (d == 0) {
		if(b & UI_B0)
		{
			UI_DeBounce(UI_B0);
			refreshTimer();
			RunMode();
			ShowTitle();
		}
		UI_Options(PSTR("Off"), PSTR("Show"), PSTR("Run"));
	}
	
	 else {
		 UI_Options(PSTR("Off"), PSTR("Show"), PSTR("N/A"));
	 }

	if(b & UI_B1)  
	{
		UI_DeBounce(UI_B1);
		refreshTimer();
		ShowMode();
		ShowTitle();
		refreshTimer();
	}
	if(b & UI_B2)
	{
		UI_DeBounce(UI_B2);
		refreshTimer();
		PwrRelayOff();
		ShowTitle();
	}
}

// Writes the pot position, current and voltage to screen.
static void UpdateParams()
{
	int16_t d = GetPotMC();
	UI_NumXYS(26, 17, d, 4, U_Decimal | U_Signed);
	int16_t v = ADC_GetBatteryVoltage();
	UI_NumXYS(26, 26, v, 5, U_Decimal | U_x1000);
	int16_t c = ADC_GetCurrent();
	UI_NumXYS(26, 35, c, 5, U_Decimal | U_x100);
	if (s_bForward) {
		UI_StrXYSP(26, 44, PSTR("Forward"));
	} else {
		UI_StrXYSP(26, 44, PSTR("Reverse"));	
	}
	
	//int16_t tt = (int16_t) g_maxlooptime;
	//UI_NumXYS(0, 46, tt, 6, U_Decimal);
	uint16_t tme = (uint16_t) (GetSystemTime() / 1000);
	int resetTime = GetSystemTime()/1000;
	UI_NumXYS(100, 20, tme, 3, U_Decimal);	
	int16_t rtme = newTimerMax;							//Original 65,46, field size 7
	UI_NumXYS(100, 40, rtme, 3, U_Decimal);				
	
	
	if (resetTime == newTimerMax) {					//auto shutdown function
		PwrRelayOff();
	}
	UI_Update();
}

static void RunMode()
{
    UI_NewScreen(PSTR("RUN MODE!"));
    UI_StrXYSP(0, 17, PSTR("Pot="));  UI_StrXYSP(65, 20, PSTR("uSec"));
    UI_StrXYSP(0, 26, PSTR("Bat="));  UI_StrXYSP(65, 29, PSTR("Volts"));
    UI_StrXYSP(0, 35, PSTR("Cur="));  UI_StrXYSP(65, 38, PSTR("Amps"));
	UI_StrXYSP(0, 44, PSTR("Dir="));		
    UI_Options(PSTR("Off"), PSTR("Dir"), PSTR("Back"));
	UpdateParams();
    UI_Update();
	MotorRelayOn();
	while(1)
	{

		int16_t d = GetPotMC();
		
		ControlPWM(d, s_bForward);
		UpdateParams();			
		uint8_t b = UI_GetButtons();
		
		if (d == 0) {
		
		UI_Options(PSTR("Off"), PSTR("Dir"), PSTR("Back"));
		
		if(b & UI_B1)
		{
				UI_DeBounce(UI_B1);
				refreshTimer();
				if (s_bForward) s_bForward = False;
				else s_bForward = True;
		}
		
		}
			
		
		else
		{
			UI_Options(PSTR("Off"), PSTR("N/A"), PSTR("Back"));
		}
		
		if(b & UI_B0)
		{
			UI_DeBounce(UI_B0);
			refreshTimer();
			MotorRelayOff();
			return;
		}
		if(b & UI_B2)
		{
			UI_DeBounce(UI_B2);
			refreshTimer();
			PwrRelayOff();
			return;
		}
	}
	
}
	


static void ShowMode()
{
	UI_NewScreen(PSTR("Not READY..."));
	UI_Options(NULL, PSTR("Cont"), NULL);
	UI_Update();
	while(1)
	{
		uint8_t b = UI_GetButtons();
		if(b & UI_B1)
		{
			UI_DeBounce(UI_B1);
			refreshTimer();
			return;
		}
	}	
}

static void ShowTitle()
{
    UI_NewScreen(PSTR("VCHS Robots 2017"));
	UI_StrXYSP(0, 17, PSTR("Pot="));  UI_StrXYSP(65, 20, PSTR("uSec"));
	UI_StrXYSP(0, 26, PSTR("Bat="));  UI_StrXYSP(65, 29, PSTR("Volts"));
	UI_StrXYSP(0, 35, PSTR("Cur="));  UI_StrXYSP(65, 38, PSTR("Amps"));
	UI_StrXYSP(0, 44, PSTR("Dir="));
    UI_Options(PSTR("Off"), PSTR("Show"), PSTR("Run"));
    UI_Update();
}

static void refreshTimer()
{
		newTimerMax = ogCap + GetSystemTime()/1000;		
}

void SlowInterrupt()
{
    static bool8 bLed = False;
    if(bLed) {bLed = False; LedRedOff(); }
    else     {bLed = True; LedRedOn(); }
    SetSlowInterrupt(GetSystemTime() + 100);
	
	// Dal's way
	//if (timetolive <= 0 ) {
		////dei here
	//}
	//timetolive--
	
}

static void ControlPWM(int16_t d, bool8 forward)
{
	uint16_t width = 1500;
	if (forward) {
		width += d/2;
	} else {
		width -= d/2;
	}
	PWM_SetWidth(width);
}

static int16_t lastPotRead = 0; 

int16_t GetPotMC() {
	int16_t d;
	d = ADC_GetPot();
	
	if (d != lastPotRead) {
		lastPotRead = d;
		refreshTimer();
	}
	return d;
}





