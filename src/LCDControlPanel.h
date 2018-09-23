#ifndef _LCD_CONTROL_H
#define _LCD_CONTROL_H

#define ELEMENTS(x)  (sizeof(x) / sizeof(x[0]))

#define LED_PIN 13

// Resolution defs for MenuItemAnalog
#define LO_RES	5
#define HI_RES	1

// Character & misc defs
#define CURSOR 126
#define ROSRUC 127

#define UP true
#define DN false

#define CH_UP_ARROW 0	// User defined character references.
#define CH_DN_ARROW 1
#define BLOCK_BASE 2	// Base of block characters for Hi Res Bargraph.

#define L_ARROW	0x7F	// Indicators in the lists.
#define R_ARROW 0x7E

#define GT_THAN	0x3E	// < and > symbols for use in int and float items.
#define LS_THAN	0x3C

#define LEFT_BRACKET	0x5B	// [ and ] symbols for use in analog items.
#define RIGHT_BRACKET	0x5D

#define ENTER_CALLBACK	0
#define EXIT_CALLBACK	1
#define MODIFY_CALLBACK	3

#include <Arduino.h>
#include <avr/pgmspace.h>
#include <LiquidCrystal_I2C.h>

#include "Controller.h"

// Forward class declarations to prevent circular references confusing the compiler.
class Menu;
class MenuItemList;
class MenuItemInteger;
class MenuItemFloat;
class MenuItemAnalog;
class LCDControlPanel;

class LCDControlPanel
{
	protected:
		void createCustomChars();

	public:
		LiquidCrystal_I2C *lcdDisplay;
		Controller *inputDevice;
		Menu *menuRoot;
		void (*lcdLoop)();
		byte lcdWidth, lcdHeight;
		
		LCDControlPanel( LiquidCrystal_I2C * lcdDisplay, Controller * inputDevice, byte lcdWidth, byte lcdHeight );
		LCDControlPanel( LiquidCrystal_I2C * lcdDisplay, Controller * inputDevice, byte lcdWidth, byte lcdHeight, void (*lcdLoop)() );
		void begin();
		void attachMenu( Menu * );
		void lcdPrintProgStr( const char *text, byte x, byte y );
		void lcdPrint( const char *text, byte x, byte y );
		void lcdPrint( int value, byte x, byte y );
		void lcdPrint( float value, byte precision, byte x, byte y );
		void lcdWrite( byte theChar , byte x, byte y );
		void lcdClear();
		byte controllerRead( byte controllerOrientation );
};

class MenuBase	// Base for derived classes, only used for inheritance.
{
	public:
		const char *name;
		MenuBase *nextItem;

		MenuBase( const char * name );
		virtual void begin( LCDControlPanel * );
};

class Menu : public MenuBase
{
	private:
		void updateDisplay( LCDControlPanel * );
		MenuBase *firstChild;
		byte itemCount, lcdOrigin, lcdCurrentLine;
	
	public:
		Menu();
		void begin( LCDControlPanel * );
		void addItemBefore( MenuBase * , MenuBase * );
		void addItemAfter(  MenuBase * , MenuBase * );
		void deleteItem( MenuBase * );
};

class MenuItemList : public MenuBase
{
	protected:
		const char * const * listItems;
		void (*CallBack)( byte callBackType, byte listIndex );
		void updateDisplay( LCDControlPanel * );
		
	public:
		byte itemCount, lcdOrigin, lcdCurrentLine;

		MenuItemList( const char * name, const char * const * ListItems , byte numListItems, void (*CallBack)( byte, byte ) );
		void begin( LCDControlPanel * );
		void setItemValue( LCDControlPanel *, byte );
		byte getItemValue();
};

class MenuItemInteger : public MenuBase
{
	protected:
		int itemValue, minValue, maxValue, stepValue;
		void (*CallBack)( byte callBackType, int menuValue );
		void updateDisplay( LCDControlPanel * );
		
	public:
		MenuItemInteger( const char * name, int minValue, int maxValue, void (*CallBack)( byte, int )  );
		MenuItemInteger( const char * name, int minValue, int maxValue, int stepValue, void (*CallBack)( byte, int )  );
		MenuItemInteger( const char * name, int minValue, int maxValue, int stepValue, int startValue, void (*CallBack)( byte, int )  );
		void begin( LCDControlPanel * );
		void setItemValue( int );
		int getItemValue();
};
	
class MenuItemFloat : public MenuBase
{
	protected:
		float itemValue, minValue, maxValue, stepValue, menuValue;
		void (*CallBack)( byte callBackType, float menuValue );
		void updateDisplay( LCDControlPanel * );
		byte valuePrecision;
		
	public:
		MenuItemFloat( const char * name, float minValue, float maxValue, byte valuePrecision, void (*CallBack)( byte, float )  );
		MenuItemFloat( const char * name, float minValue, float maxValue, float stepValue, byte valuePrecision, void (*CallBack)( byte, float )  );
		MenuItemFloat( const char * name, float minValue, float maxValue, float stepValue, float startValue, byte valuePrecision, void (*CallBack)( byte, float )  );
		void begin( LCDControlPanel * );
		void setItemValue( float );
		float getItemValue();
};

class MenuItemAnalog : public MenuBase
{
	protected:
		byte itemValue, barResolution;
		byte stripeCount;
		
		void (*CallBack)( byte callBackType, byte menuValue );
		void updateDisplay( LCDControlPanel * );
		
	public:
		MenuItemAnalog( const char * name, byte startValue, byte barResolution,  void (*CallBack)( byte, byte )  );
		void begin( LCDControlPanel * );
		void setItemValue( byte );
		byte getItemValue();
};

#endif
