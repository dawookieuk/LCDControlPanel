#include "LCDControlPanel.h"

void DefaultCallBack( byte dummyValue, float anotherDummyValue )
{
}

MenuItemFloat::MenuItemFloat( const char * name, float minValue, float maxValue, byte valuePrecision, void (*callBack)( byte, float )  ): MenuBase( name ), minValue( minValue ), maxValue( maxValue ), stepValue( stepValue ), valuePrecision( valuePrecision )
{
	stepValue = 1.0;
	itemValue = minValue;
	CallBack = callBack;
}

MenuItemFloat::MenuItemFloat( const char * name, float minValue, float maxValue, float stepValue, byte valuePrecision, void (*callBack)( byte, float )  ): MenuBase( name ), minValue( minValue ), maxValue( maxValue ), stepValue( stepValue ), valuePrecision( valuePrecision )
{
	itemValue = minValue;
	CallBack = callBack;
}

MenuItemFloat::MenuItemFloat( const char * name, float minValue, float maxValue, float stepValue, float startValue, byte valuePrecision, void (*callBack)( byte, float )  ): MenuBase( name ), minValue( minValue ), maxValue( maxValue ), stepValue( stepValue ), valuePrecision( valuePrecision )
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

void MenuItemFloat::updateDisplay( LCDControlPanel *myLCDControlPanel )
{
	byte valueLength;
	char outputBuffer[ myLCDControlPanel->lcdWidth - 2 ];
	
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

	dtostrf( itemValue, 0, 1, outputBuffer );	// We use the avr dtostrf because sprintf doesn't support floats on ardiuno.
	valueLength = strlen( outputBuffer );

	myLCDControlPanel->lcdPrint( itemValue, valuePrecision, ( myLCDControlPanel->lcdWidth - valueLength - 1 ) / 2, myLCDControlPanel->lcdHeight - 1 );
}
	
void MenuItemFloat::begin( LCDControlPanel *myLCDControlPanel )
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

void MenuItemFloat::setItemValue( float newValue )
{
	if ( newValue >= minValue && newValue <= maxValue )
	{
		itemValue = newValue;
	}
}

float MenuItemFloat::getItemValue()
{
	return itemValue;
}
