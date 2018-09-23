// Step1: Define the menu item titles, this is the text that will appear on the LCD display.
const char Menu1_1[] PROGMEM = {"List"};
const char Menu1_2[] PROGMEM = {"Lo Res Analog"};
const char Menu1_3[] PROGMEM = {"Hi Res Analog"};
const char Menu1_4[] PROGMEM = {"Float"};
const char Menu1_5[] PROGMEM = {"Integer"};

// Step 1.5 [Optional]: Define any lists for list menus. If you are not using any list menus you can skip this step.
const char item1List1[] PROGMEM = {"On"};
const char item1List2[] PROGMEM = {"Off"};
const char item1List3[] PROGMEM = {"Wibble"};
const char item1List4[] PROGMEM = {"High"};
const char item1List5[] PROGMEM = {"Medium"};
const char item1List6[] PROGMEM = {"Low"};
const char * const item1List[] PROGMEM = { item1List1, item1List2, item1List3, item1List4, item1List5, item1List6 };

// Step 2: Define a callback functions for each item.
void listCallBack( byte callBackType, byte value )
{
	// Handle each callback separately.
	
	// The value for a list CallBack is the index of the list array.
	switch( callBackType )
	{
		case MODIFY_CALLBACK:
			Serial.print(" Modify Callback (list): ");
			Serial.println( value );
			break;
			
		case ENTER_CALLBACK:
			Serial.print(" Enter Callback (list): ");
			Serial.println( value );
			break;
			
		case EXIT_CALLBACK:
			Serial.print(" Exit Callback (list): ");
			Serial.println( value );
			break;
	}
}

void analogCallBack( byte callBackType, int value )
{
	// Handle all callbacks the same.
	Serial.print(" Callback (analog): ");
	Serial.println( value );
}

void floatCallBack( byte callBackType, float value )
{
	// Handle only one callback.
	if ( callBackType == MODIFY_CALLBACK )
	{
		Serial.print(" Modify Callback (float): ");
		Serial.println( value );
	}
}

void intCallBack( byte callBackType, int value )
{
	// Don't handle any callbacks at all.
}


// Step 3: Initialise the menu item objects.
MenuItemList	menuItem1( Menu1_1, item1List, ELEMENTS(item1List), &listCallBack );
MenuItemAnalog	menuItem2( Menu1_2, 135, LO_RES, &analogCallBack );
MenuItemAnalog	menuItem3( Menu1_3, 135, HI_RES, &analogCallBack );
MenuItemFloat	menuItem4( Menu1_4, -3.0, 10.0, 0.5, 1, &floatCallBack );
MenuItemInteger	menuItem5( Menu1_5, -2, 10, 1, &intCallBack );


// Step 4: initialise the menu object.
Menu myMenu;

SetupMenu()
{
// Step 5: build the menu structure, note use of & to denote object addresses.
	
	myMenu.addItemAfter( 0, &menuItem1 );
	myMenu.addItemAfter( 0, &menuItem2 );
	myMenu.addItemAfter( 0, &menuItem3 );
	myMenu.addItemAfter( 0, &menuItem4 );
	myMenu.addItemAfter( 0, &menuItem5 );
}
