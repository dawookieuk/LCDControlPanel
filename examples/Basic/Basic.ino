#include <LCDControlPanel.h>
#include "SetupMenu.h"

#define LCD_WIDTH 16
#define LCD_HEIGHT 2

// The LCDControlPanel library is dependent on the NewLiquidCrystal library being installed 
// which you can get from https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads/

void lcdLoop();

//rotaryController myController( 14, 15, 16 );
switchController myController	( 8, 9, 10, 11, 12 );
LiquidCrystal_I2C myLCD( 0x27 , 2 , 1 , 0 , 4 , 5 , 6 , 7 , 3 , POSITIVE );
LCDControlPanel myControlPanel( &myLCD, &myController, LCD_WIDTH, LCD_HEIGHT, &lcdLoop );

void setup()
{
	Serial.begin( 115200 );
	Serial.println( " Begin:");

	pinMode( LED_PIN, OUTPUT);
	digitalWrite( LED_PIN, LOW );

	SetupMenu();
	
	// Attach the menu from SetupMenu() to the LCDControlPanel object	
	myControlPanel.attachMenu( &myMenu );
}

void loop()
{
  myControlPanel.begin();
}

void lcdLoop()
{
	static unsigned long myMillis = 0;
	
	if ( millis() > ( myMillis + 500 ) )
	{
		digitalWrite( LED_PIN, !digitalRead( LED_PIN ) );
		myMillis = millis();
	}
}
