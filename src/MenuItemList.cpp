#include "LCDControlPanel.h"

void DefaultCallBack( byte dummyValue, byte otherDummyValue )
{
}

MenuItemList::MenuItemList( const char * name, const char * const * listItems, byte itemCount, void (*callBack)( byte, byte )  ): MenuBase( name ), listItems( listItems ), itemCount( itemCount )
{
	itemCount = 0;
	lcdOrigin = 0;
	lcdCurrentLine = 0;
	CallBack = callBack;
}

void MenuItemList::begin( LCDControlPanel *myLCDControlPanel )
{
	byte itemCounter;
	bool breakOut = false;
	
	updateDisplay( myLCDControlPanel );	// Initial population of the display
	
	CallBack( ENTER_CALLBACK, lcdOrigin + lcdCurrentLine );
	
	while ( breakOut == false )
	{
		switch( myLCDControlPanel->controllerRead( CONTROLLER_AXIS_VER ) )
		{
			case CONTROLLER_UP:
				if ( lcdCurrentLine > 0 )	// If cursor is not on the top line
				{
					lcdCurrentLine--;
					updateDisplay( myLCDControlPanel );
					CallBack( MODIFY_CALLBACK, lcdOrigin + lcdCurrentLine );	// Value has changed so call the modify callback for this object.
				}
				else if ( lcdOrigin > 0 ) // If cursor is on the bottom line but the list isn't at the bottom of the display
				{
					lcdOrigin--;
					updateDisplay( myLCDControlPanel );
					CallBack( MODIFY_CALLBACK, lcdOrigin + lcdCurrentLine );
				}
				break;
				
			case CONTROLLER_DOWN:
				if ( lcdCurrentLine < (  myLCDControlPanel->lcdHeight -1 ) )	// If cursor is not on the bottom line
				{
					lcdCurrentLine++;
					updateDisplay( myLCDControlPanel );
					CallBack( MODIFY_CALLBACK, lcdOrigin + lcdCurrentLine );
				}
				else if ( lcdOrigin < ( itemCount - myLCDControlPanel->lcdHeight ) ) // If cursor is on the bottom line but the list isn't at the bottom of the display
				{
					lcdOrigin++;
					updateDisplay( myLCDControlPanel );
					CallBack( MODIFY_CALLBACK, lcdOrigin + lcdCurrentLine );
				}
				break;
				
			case CONTROLLER_ENTER:
				breakOut = true;
				break;

		}
		myLCDControlPanel->lcdLoop();	// Execute the callback for the parent LCDDisplay object.
	}
	
	CallBack( EXIT_CALLBACK, lcdOrigin + lcdCurrentLine );
}

void MenuItemList::updateDisplay( LCDControlPanel *myLCDControlPanel )
{
	byte displayLine;
		
	myLCDControlPanel->lcdClear();

	
	if ( lcdOrigin > 0 )	// Up arrow if required
	{
		myLCDControlPanel->lcdWrite( CH_UP_ARROW, 0, 0 );
	}
	
	if ( lcdOrigin + ( myLCDControlPanel->lcdHeight ) < itemCount ) // Down arrow if required
	{
		 myLCDControlPanel->lcdWrite( CH_DN_ARROW, 0, myLCDControlPanel->lcdHeight - 1 );
	}
	
	for ( displayLine = 0 ; displayLine <  myLCDControlPanel->lcdHeight ; displayLine++ )	// Item names
	{
		myLCDControlPanel->lcdPrintProgStr( pgm_read_word_near( &listItems[ displayLine + lcdOrigin ] ),2 , displayLine );
	}
	
	myLCDControlPanel->lcdWrite( R_ARROW, 1, lcdCurrentLine );
}

void MenuItemList::setItemValue( LCDControlPanel *myLCDControlPanel, byte newValue)
{
	if( newValue > itemCount )
	{
		return;	// Insane value, just bail.
	}
	else if( newValue < ( itemCount - myLCDControlPanel->lcdHeight ) )
	{
		lcdOrigin = newValue;
		lcdCurrentLine = 0;
	}
	else
	{
		lcdOrigin = itemCount - myLCDControlPanel->lcdHeight;
		lcdCurrentLine = newValue - lcdOrigin;
	}
}

byte MenuItemList::getItemValue()
{
	return lcdOrigin + lcdCurrentLine;
}
