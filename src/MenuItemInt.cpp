#include "LCDControlPanel.h"

void DefaultCallBack( int dummyValue )
{
}

MenuItemInteger::MenuItemInteger( const char * name, int minValue, int maxValue, void ( callBack )( byte, int )  ): MenuBase( name ), minValue( minValue ), maxValue( maxValue ), stepValue( stepValue )
{
	stepValue = 1;
	itemValue = minValue;
	CallBack = callBack;
}

MenuItemInteger::MenuItemInteger( const char * name, int minValue, int maxValue, int stepValue, void ( callBack )( byte, int )  ): MenuBase( name ), minValue( minValue ), maxValue( maxValue ), stepValue( stepValue )
{
	itemValue = minValue;
	CallBack = callBack;
}

MenuItemInteger::MenuItemInteger( const char * name, int minValue, int maxValue, int stepValue, int startValue, void ( callBack )( byte, int )  ): MenuBase( name ), minValue( minValue ), maxValue( maxValue ), stepValue( stepValue )
{
	if ( startValue >= minValue && startValue <= maxValue )	// Check startValue is sane.
	{
		itemValue = startValue;
	}
	else
	{
		itemValue = minValue;
	}
	
	CallBack = callBack;
}

void MenuItemInteger::updateDisplay( LCDControlPanel *myLCDControlPanel )
{
	byte valueLength;
	
	myLCDControlPanel->lcdClear();
	
	// Print item name centred on the display.
	myLCDControlPanel->lcdPrintProgStr( name, ( myLCDControlPanel->lcdWidth - strlen_P( name ) ) / 2, 0 );

	if ( itemValue > minValue )	// less than if required
	{
		myLCDControlPanel->lcdWrite( LS_THAN, 0, myLCDControlPanel->lcdHeight - 1 );
	}
	
	if ( itemValue < maxValue ) // greater than if required
	{
		myLCDControlPanel->lcdWrite( GT_THAN, myLCDControlPanel->lcdWidth - 1, myLCDControlPanel->lcdHeight - 1 );
	}
	
	// Calculate the width of the value on the display, this way lacks elegance but is simple and computationally light.
	
	if ( itemValue <= -10000 )
	{
		valueLength = 6;
	}
	else if ( itemValue <= -1000 )
	{
		valueLength = 5;
	}
	else if ( itemValue <= -100 )
	{
		valueLength = 4;
	}
	else if ( itemValue <= -10 )
	{
		valueLength = 3;
	}
	else if ( itemValue < 0 )
	{
		valueLength = 2;
	}
	else if ( itemValue < 10 )
	{
		valueLength = 1;
	}
	else if ( itemValue < 100 )
	{
		valueLength = 2;
	}
	else if ( itemValue < 1000 )
	{
		valueLength = 3;
	}
	else if (itemValue < 10000 )
	{
		valueLength = 4;
	}
	else
	{
		valueLength = 5;
	}
	
	myLCDControlPanel->lcdPrint( itemValue, ( myLCDControlPanel->lcdWidth - valueLength ) / 2, myLCDControlPanel->lcdHeight - 1 );
}
	
void MenuItemInteger::begin( LCDControlPanel *myLCDControlPanel )
{
	bool breakOut = false;
	
	updateDisplay( myLCDControlPanel );	// Initial population of the display
	
	CallBack( ENTER_CALLBACK, itemValue );
	
	while ( breakOut == false )
	{
		switch( myLCDControlPanel->controllerRead( CONTROLLER_AXIS_VER_INV ) )
		{
			case CONTROLLER_DOWN:
				if ( itemValue > ( minValue + stepValue ) )
				{
					itemValue -= stepValue;
					updateDisplay( myLCDControlPanel );
					CallBack( MODIFY_CALLBACK, itemValue );
				}
				else if ( itemValue > minValue )
				{
					itemValue = minValue;
					updateDisplay( myLCDControlPanel );
					CallBack( MODIFY_CALLBACK, itemValue );
				}
				break;
				
			case CONTROLLER_UP:
				if ( itemValue < ( maxValue - stepValue ) )
				{
					itemValue += stepValue;
					updateDisplay( myLCDControlPanel );
					CallBack( MODIFY_CALLBACK, itemValue );
				}
				else if ( itemValue < maxValue )
				{
					itemValue = maxValue;
					updateDisplay( myLCDControlPanel );
					CallBack( MODIFY_CALLBACK, itemValue );
				}
				break;
			
			case CONTROLLER_ENTER:
				breakOut = true;
				break;

		}
		myLCDControlPanel->lcdLoop();	// Execute the callback for the parent LCDDisplay object.
	}
	CallBack( EXIT_CALLBACK, itemValue );
}

void MenuItemInteger::setItemValue( int newValue )
{
	if ( newValue >= minValue && newValue <= maxValue )
	{
		itemValue = newValue;
	}
}

int MenuItemInteger::getItemValue()
{
	return itemValue;
}
