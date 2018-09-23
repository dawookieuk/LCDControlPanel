#include "Controller.h"

rotaryController::rotaryController( byte pinA, byte pinB, byte pinButton, unsigned int nextDelay, unsigned int backDelay )
	:pinA(pinA), pinB(pinB), pinButton(pinButton), nextDelay(nextDelay), backDelay(backDelay)
{
	// Initialise input pins
	pinMode( pinA, INPUT_PULLUP );
	pinMode( pinB, INPUT_PULLUP );
	pinMode( pinButton, INPUT_PULLUP );
	
	// Initialiase encoder A pin last state to known value.
	encALast = false;

	// Set the encoder read limiting timer to current time.
	controllerLastUpdate = millis();
}

byte rotaryController::read( byte controllerOrientation )
{
	bool encA, encB;
	byte encoderState = CONTROLLER_NULL;
	unsigned long buttonTimer;

	// Periodically read the rotary encoder.
	if( millis() >= ( controllerLastUpdate + CONTROLLER_UPDATE_PERIOD ) )
	{
	
		// Read the push button. As pressing the switch grounds the pin, LOW = pushed and HIGH = released.
		if ( digitalRead( pinButton ) == LOW )
		{
			// Button is pressed, start timer and pause for the debounce delay.
			buttonTimer = millis();
			deBounce();

			// Loop around while the button is pressed.
			while( digitalRead( pinButton ) == LOW )
			{
				deBounce();	// Give the ISRs time to do stuff, no point flying around an emtpy loop.
			}

			// Once button is released, figure out whether is was pressed (next), held (enter) or held long (back).
			if ( ( millis() - buttonTimer ) < nextDelay )
			{
				encoderState = CONTROLLER_ENTER;
			}
			else if ( ( ( millis() - buttonTimer ) > nextDelay ) && ( ( millis() - buttonTimer ) < backDelay ) )
			{
				encoderState = CONTROLLER_RIGHT;
			}
			else
			{
				encoderState = CONTROLLER_LEFT;
			}
		}

		// Read encoder pins.
		encA = digitalRead( pinA );
		encB = digitalRead( pinB );
		
		/* Rotary encoder 101. Pin A only goes low between clicks so it registers that the shaft is being turned.
		 * If A is low and the last reading of A was also low, the encoder is still being turned so we ignore it,
		 * otherwise we get lots of reads for one click of the shaft.
		 * When pin A goes low, Pin B is then read to test whether the shaft is going clockwise or anti clockwise.
		 * If A hasn't changed then the shaft hasn't been turned so nothing needs updating. */
		 
		if ( ( encA == LOW ) && ( encALast == HIGH ) )	// Has the shaft been turned?
		{
			switch( controllerOrientation )	// As we're using the rotary encoder to on both axis of the menu we need to know which is being asked for.
			{
				case CONTROLLER_AXIS_HOR:
					if( encB == LOW )
					{
						encoderState = CONTROLLER_RIGHT;	// Encoder turned clockwise.
					}   
					else
					{
						encoderState = CONTROLLER_LEFT;	// Encoder turned anti-clockwise.
					}
					break;
								
				case CONTROLLER_AXIS_VER:
					if( encB == LOW )
					{
						encoderState = CONTROLLER_DOWN;	// Encoder turned clockwise.
					}   
					else
					{
						encoderState = CONTROLLER_UP;	// Encoder turned anti-clockwise.
					}
					break;
					
				case CONTROLLER_AXIS_VER_INV:
					if( encB == LOW )
					{
						encoderState = CONTROLLER_UP;	// Encoder turned clockwise.
					}   
					else
					{
						encoderState = CONTROLLER_DOWN;	// Encoder turned anti-clockwise.
					}
					break;	
			}
		}

		encALast = encA;					// Store value of encoder A for next time, necessary to record state change.
		controllerLastUpdate = millis();	// Updates EncoderLastUpdate to prevent the encoder being read too often.
		}

	return encoderState; 
}
