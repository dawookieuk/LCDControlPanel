#include "LCDControlPanel.h"

const char rootMenuName PROGMEM = {""};
Menu::Menu(): MenuBase( rootMenuName )
{
	itemCount = 0;
	lcdOrigin = 0;
	lcdCurrentLine = 0;
}

void Menu::updateDisplay( LCDControlPanel *myLCDControlPanel )
{
	byte displayLine;		// Throw away value for use in loops, used multiple times.
	MenuBase * displayItem;	// Ditto.
	
	myLCDControlPanel->lcdClear();
	
	if ( lcdOrigin > 0 )	// Up arrow if required
	{
		myLCDControlPanel->lcdWrite( CH_UP_ARROW, 0, 0 );
	}
	
	if ( lcdOrigin + ( myLCDControlPanel->lcdHeight ) < itemCount ) // Down arrow if required
	{
		 myLCDControlPanel->lcdWrite( CH_DN_ARROW, 0, myLCDControlPanel->lcdHeight - 1 );
	}
	
	// Move displayItem to correct place in the list.
	displayItem = firstChild;
	for ( displayLine = 0 ; displayLine < lcdOrigin ; displayLine++ )
	{
		displayItem = displayItem->nextItem;
	}
	
	for ( displayLine = 0 ; displayLine <  myLCDControlPanel->lcdHeight ; displayLine++ )	// Put item names on the display.
	{
		myLCDControlPanel->lcdPrintProgStr( displayItem->name ,2 , displayLine );
		displayItem = displayItem->nextItem;
	}
	
	myLCDControlPanel->lcdWrite( R_ARROW, 1, lcdCurrentLine );	// Add right arrow as cursor for currently selected item.
}

void Menu::begin( LCDControlPanel *myLCDControlPanel )
{
	byte itemCounter;
	MenuBase * currentItem;
	
	updateDisplay( myLCDControlPanel );	// Initial population of the display
	
	while ( true )
	{
		switch( myLCDControlPanel->controllerRead( CONTROLLER_AXIS_VER ) )
		{
			case CONTROLLER_UP:
				if ( lcdCurrentLine > 0 )	// If cursor is not on the top line
				{
					lcdCurrentLine--;
					updateDisplay( myLCDControlPanel );
				}
				else if ( lcdOrigin > 0 ) // If cursor is on the top line but the list isn't at the top of the display
				{
					lcdOrigin--;
					updateDisplay( myLCDControlPanel );
				}
				break;
				
			case CONTROLLER_DOWN:
				if ( lcdCurrentLine < (  myLCDControlPanel->lcdHeight -1 ) )	// If cursor is not on the bottom line
				{
					lcdCurrentLine++;
					updateDisplay( myLCDControlPanel );
				}
				else if ( lcdOrigin < ( itemCount - myLCDControlPanel->lcdHeight ) ) // If cursor is on the bottom line but the list isn't at the bottom of the display
				{
					lcdOrigin++;
					updateDisplay( myLCDControlPanel );
				}
				break;
			
			case CONTROLLER_ENTER:
				// Move currentItem to correct place in the list.
				currentItem = firstChild;
				for ( itemCounter = 0 ; itemCounter < ( lcdOrigin + lcdCurrentLine ) ; itemCounter++ )
				{
					currentItem = currentItem->nextItem;
				}
				currentItem->begin( myLCDControlPanel );	// Pass control to selected object
				updateDisplay( myLCDControlPanel );			// and restore the display upon return from same.
				break;

		}
		
		myLCDControlPanel->lcdLoop();	// Execute the callback.
	}
}

void Menu::addItemBefore( MenuBase *targetItem , MenuBase *newItem )
{
	MenuBase *currentItem;
	
	if( ( targetItem == 0 ) || ( firstChild == targetItem ) )	// If this is the head of the list just add the item at the top.
	{
		newItem->nextItem = firstChild;
		firstChild = newItem;
	}
	else	// otherwise walk the list until targetItem is next and insert before it.
	{		
		currentItem = firstChild;
		
		while( currentItem->nextItem != targetItem )
		{
			currentItem = currentItem->nextItem;
		}
		newItem->nextItem = currentItem->nextItem;
		currentItem->nextItem = newItem;
	}
	itemCount++;
}

void Menu::addItemAfter( MenuBase *targetItem , MenuBase *newItem )
{
	MenuBase *currentItem;
	
	if ( targetItem == 0 )	// Add to tail of list.
	{
		if ( firstChild == 0 )	// Is list empty?
		{
			firstChild = newItem;
		}
		else	// List has members so insert at the tail.
		{				
			currentItem = firstChild;
			while( currentItem->nextItem != targetItem )	// Walk the list until the end.
			{
				currentItem = currentItem->nextItem;
			}
			newItem->nextItem = targetItem;
			currentItem->nextItem = newItem;
		}
	}
	else	// Insert into list
	{
		currentItem = firstChild;
		while( currentItem != targetItem )	// Walk the list until targetItem.
		{
			currentItem = currentItem->nextItem;
		}
		newItem->nextItem = currentItem->nextItem;
		currentItem->nextItem = newItem;
	}
	itemCount++;
}

void Menu::deleteItem( MenuBase *targetItem )
{
	MenuBase *tempItem, *currentItem;
	
	if ( firstChild == 0 )	// Is list empty?
	{
		return;
	}
	else if ( firstChild == targetItem ) // Or is the first Item to be deleted?
	{
		tempItem = firstChild;
		firstChild = targetItem->nextItem;
		tempItem->nextItem = 0;
	}
	else
	{
		currentItem = firstChild;
		while( currentItem->nextItem != targetItem )	// Walk the list until the target.
		{
			currentItem = currentItem->nextItem;
		}
		tempItem = currentItem->nextItem;
		currentItem->nextItem = tempItem->nextItem;
		tempItem->nextItem = 0;
	}
	itemCount--;
}
