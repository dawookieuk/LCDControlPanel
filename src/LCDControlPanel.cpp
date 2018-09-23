#include "LCDControlPanel.h"

/* *******************************/
/* * LCDControlPanel class defs  */
/* *******************************/
void defaultCallBack()
{
}

LCDControlPanel::LCDControlPanel(  LiquidCrystal_I2C *lcdDisplay, Controller *inputDevice, byte lcdWidth, byte lcdHeight )
	:lcdDisplay( lcdDisplay ), inputDevice( inputDevice ), lcdWidth( lcdWidth ), lcdHeight( lcdHeight )
{
	menuRoot = 0;
	lcdLoop = &defaultCallBack;
}

LCDControlPanel::LCDControlPanel(  LiquidCrystal_I2C *lcdDisplay, Controller *inputDevice, byte lcdWidth, byte lcdHeight, void (*loop)() )
	:lcdDisplay( lcdDisplay ), inputDevice( inputDevice ), lcdWidth( lcdWidth ), lcdHeight( lcdHeight )
{
	menuRoot = 0;
	lcdLoop = loop;
}

void LCDControlPanel::begin()
{
	byte flashCount;
	
	inputDevice->attachCallBack( this->lcdLoop );	// Attach the callback to the controller device.

	lcdDisplay->begin( lcdWidth, lcdHeight );
	createCustomChars();

	lcdDisplay->clear();
	
	// Flash the display to prove it's been initialised.
	for( flashCount = 0 ; flashCount < 3 ; flashCount++ )
	{
		lcdDisplay->setBacklight( 255 );
		delay( 250 );
		lcdDisplay->setBacklight( 0 );
		delay( 250 );
	}
	lcdDisplay->setBacklight( 255 );
	
	if ( menuRoot != 0 )
	{
		menuRoot->begin( this );
	}
}

void LCDControlPanel::attachMenu( Menu *newMenuRoot )
{
	menuRoot = newMenuRoot;
}

void LCDControlPanel::lcdPrintProgStr( const char *text, byte x, byte y )
{
	lcdDisplay->setCursor( x, y );
	lcdDisplay->print( (const __FlashStringHelper *) text );
}

void LCDControlPanel::lcdPrint( int value, byte x, byte y )
{
	lcdDisplay->setCursor( x, y );
	lcdDisplay->print( value );
}

void LCDControlPanel::lcdPrint( float value, byte precision, byte x, byte y )
{
	lcdDisplay->setCursor( x, y );
	lcdDisplay->print( value, precision );
}

void LCDControlPanel::lcdWrite( byte theChar, byte x, byte y )
{
	lcdDisplay->setCursor( x, y );
	lcdDisplay->write( theChar );
}

void LCDControlPanel::lcdClear()
{
	lcdDisplay->clear();
}

void LCDControlPanel::createCustomChars()
{
	byte charCount, maxCustomChars = 6;

	// Definitions for the user defined characters for the up & down arrows and the bargraph blocks
	byte customChars[7][8] = { { B00100, B01110, B10101, B00100, B00100, B00000, B00000, B00000 },	// Up arrow
							   { B00000, B00000, B00000, B00100, B00100, B10101, B01110, B00100 },	// Dn arrow
							   { B10000, B10000, B10000, B10000, B10000, B10000, B10000, B00000 },	// Bargraph part blocks 1-5.
							   { B11000, B11000, B11000, B11000, B11000, B11000, B11000, B00000 },
							   { B11100, B11100, B11100, B11100, B11100, B11100, B11100, B00000 },
							   { B11110, B11110, B11110, B11110, B11110, B11110, B11110, B00000 },
							   { B11111, B11111, B11111, B11111, B11111, B11111, B11111, B00000 } };
	
	// Generate the custom characters from the array.
	for ( charCount = 0 ; charCount <= maxCustomChars ; charCount++)
	{
		lcdDisplay->createChar(  charCount , customChars[ charCount ] );
	}
}

byte LCDControlPanel::controllerRead( byte controllerOrientation )
{
	return inputDevice->read( controllerOrientation );
}
