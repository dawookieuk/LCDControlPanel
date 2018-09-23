#include "Controller.h"

#define DEBOUNCE_DELAY	5	// Debounce period in milliseconds.

void defaultDebounceLoop()
{
}

void Controller::deBounce()
{
	// ISR friendly way of delaying as delay() just chews CPU, screws up timings and is generally icky.
	unsigned long currentMillis, targetMillis;
	
	targetMillis = millis() + DEBOUNCE_DELAY;
	
	while( millis() < targetMillis )
	{
		deBounceLoop();
	}
}

byte Controller::read( byte controllerOrientation )
{
}

void Controller::attachCallBack( void( *myCallBack )() )
{
	deBounceLoop = myCallBack;
}

void Controller::detachCallBack()
{
	deBounceLoop = &defaultDebounceLoop;
}

Controller::Controller()
{
	deBounceLoop = &defaultDebounceLoop;
}
