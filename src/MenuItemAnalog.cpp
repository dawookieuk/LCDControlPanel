#include "LCDControlPanel.h"

extern void DefaultCallBack( byte, byte );

MenuItemAnalog::MenuItemAnalog( const char * name, byte startValue, byte barResolution,  void (*callBack)( byte, byte )  ): MenuBase( name ), itemValue( startValue ),barResolution( barResolution )
{
	CallBack = callBack;
}

void MenuItemAnalog::updateDisplay( LCDControlPanel *myLCDControlPanel )
{
	byte lcdLoop = 0;
	
	myLCDControlPanel->lcdClear();
	
	// Print item name centred on the display.
	myLCDControlPanel->lcdPrintProgStr( name, ( myLCDControlPanel->lcdWidth - strlen_P( name ) ) / 2, 0 );
	
	// Place begin & end square brackets
	myLCDControlPanel->lcdWrite( LEFT_BRACKET, 0, myLCDControlPanel->lcdHeight - 1 );
	myLCDControlPanel->lcdWrite( RIGHT_BRACKET, myLCDControlPanel->lcdWidth - 1, myLCDControlPanel->lcdHeight - 1 );		

	// Print block characters first.
	if ( stripeCount >= 5 )	// Skip for the 0-4 range.
	{
		for ( lcdLoop = 0 ; lcdLoop < ( stripeCount / 5 ) ; lcdLoop++ )
		{
			myLCDControlPanel->lcdWrite( char( BLOCK_BASE + 4 ) , lcdLoop + 1, myLCDControlPanel->lcdHeight - 1 );	// Solid block custom character.
		}
	}
	
	// Then part block if required.
	if ( ( stripeCount % 5 ) > 0 )
	{
		myLCDControlPanel->lcdWrite( char( BLOCK_BASE + ( stripeCount % 5 ) - 1 ) , lcdLoop + 1, myLCDControlPanel->lcdHeight - 1 );
	}
}

void MenuItemAnalog::begin( LCDControlPanel *myLCDControlPanel )
{
	const byte maxStripes = ( myLCDControlPanel->lcdWidth - 2 ) * 5;		// Number of availible vertical stripes on the display.
	const float clickValue = 255.0 / maxStripes;							// What one stripe is worth, AKA click boundary.

	bool breakOut = false;
	
	stripeCount = byte ( round( ( itemValue * maxStripes ) / 255.0 ) );		// Initialise stripeCount from itemValue;
	
	if ( barResolution == LO_RES )	// If in LO_RES mode round to the nearest 5.
	{
		stripeCount = stripeCount - ( stripeCount % 5 );
	}

	updateDisplay( myLCDControlPanel );	// Initial population of the display
	
	CallBack( ENTER_CALLBACK, round( clickValue * stripeCount ) );
	
	while ( breakOut == false )
	{
		switch( myLCDControlPanel->controllerRead( CONTROLLER_AXIS_HOR ) )
		{
			case CONTROLLER_LEFT:
				if ( ( barResolution == HI_RES && stripeCount > 0 ) || ( barResolution == LO_RES && stripeCount >= 5 ) )
				{
					stripeCount -= barResolution;
					updateDisplay( myLCDControlPanel );
					CallBack( MODIFY_CALLBACK, round( clickValue * stripeCount ) );
				}
				break;
				
			case CONTROLLER_RIGHT:
				if ( ( barResolution == HI_RES && stripeCount < maxStripes ) || ( barResolution == LO_RES && stripeCount <= maxStripes - 5 ) )
				{
					stripeCount += barResolution;
					updateDisplay( myLCDControlPanel );
					CallBack( MODIFY_CALLBACK, round( clickValue * stripeCount ) );
				}
				break;
			
			case CONTROLLER_ENTER:
				breakOut = true;
				break;

		}
		myLCDControlPanel->lcdLoop();	// Execute the callback for the parent LCDDisplay object.
	}
	
	// Save itemValue for next time
	itemValue = round( clickValue * stripeCount );
	
	CallBack( EXIT_CALLBACK, round( clickValue * stripeCount ) );
}

void MenuItemAnalog::setItemValue( byte newValue )
{
	itemValue = newValue;
}

byte MenuItemAnalog::getItemValue()
{
	return itemValue;
}
