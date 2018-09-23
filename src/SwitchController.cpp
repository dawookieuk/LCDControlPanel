#include "Controller.h"

#define SWITCH_DELAY 200	

switchController::switchController(byte upPin, byte downPin, byte leftPin, byte rightPin, byte enterPin )
	: upPin(upPin), downPin(downPin), leftPin(leftPin), rightPin(rightPin), enterPin(enterPin)
{
	pinMode( upPin , INPUT_PULLUP );
	pinMode( downPin , INPUT_PULLUP );
	pinMode( leftPin , INPUT_PULLUP );
	pinMode( rightPin , INPUT_PULLUP );
	pinMode( enterPin , INPUT_PULLUP );
}

bool switchController::readPin( byte thePin )
{
	if ( digitalRead( thePin ) == HIGH )	// As pressing switches pulls the pin low the logic is inverted - pressed = LOW/false;
	{
		return false;
	}
	else
	{
		deBounce();	// Debounce switch.
		return true;
	}
}

byte switchController::read( byte controllerOrientation )	// For switch controller we ignore the orientation, dummy it for the compiler.
{
	byte controllerValue = CONTROLLER_NULL;
	unsigned long targetMillis = millis() + SWITCH_DELAY;
	
	// Periodically read the switches, no need to do this too often.
	if( millis() >= ( controllerLastUpdate + CONTROLLER_UPDATE_PERIOD ) )
	{
		if ( readPin( upPin ) == true )
		{
			controllerValue = CONTROLLER_UP;
		}
		else if ( readPin( downPin ) == true )
		{
			controllerValue = CONTROLLER_DOWN;
		}
		else if ( readPin( leftPin ) == true )
		{
			controllerValue = CONTROLLER_LEFT;
		}
		else if ( readPin( rightPin ) == true )
		{
			controllerValue = CONTROLLER_RIGHT;
		}
		else if ( readPin( enterPin ) == true )
		{
			controllerValue = CONTROLLER_ENTER;

			while( readPin( enterPin ) == true )	// Loop until the switch is released.
			{
				deBounce();	// Debounce the initial press, delay for the rest.
			}
		}

		if ( controllerValue != CONTROLLER_NULL )	// For any switch value other than null introduce a delay so the wetware can keep up.
		{
			while( millis() < targetMillis )
			{
				deBounce();
			};
		}
	}
		
	return controllerValue;
}
