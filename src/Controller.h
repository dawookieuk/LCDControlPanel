#include <Arduino.h>

#ifndef _CONTROLLER_H
#define _CONTROLLER_H

#define CONTROLLER_NULL  0
#define CONTROLLER_UP    1
#define CONTROLLER_DOWN  2
#define CONTROLLER_ENTER 3
#define CONTROLLER_LEFT  4
#define CONTROLLER_RIGHT 5

// Orientation defs for the rotary contoller
#define CONTROLLER_AXIS_HOR 	0	// Clockwise = right	Anti-Clockwise = left	Analogs
#define CONTROLLER_AXIS_VER		1	// Clockwise = down		Anti-Clockwise = up		Menus & Lists
#define CONTROLLER_AXIS_VER_INV 2	// Clockwise = up		Anti-Clockwise = down	Floats & Ints

#define CONTROLLER_UPDATE_PERIOD 5	// We don't check the encoder more often than this (in milliseconds) no matter how often it's called.

class Controller
{
	protected:
		void deBounce();
		void (*deBounceLoop)();
		unsigned long controllerLastUpdate;
	
	public:
		Controller();
		virtual byte read( byte controllerOrientation );
		void attachCallBack( void (*callback)() );
		void detachCallBack();
};

class rotaryController: public Controller
{
	private:
		unsigned int nextDelay, backDelay;
		byte pinA, pinB, pinButton;
		bool encALast;
		
	public:
		rotaryController( byte pinA, byte pinB, byte pinButton, unsigned int nextDelay = 750, unsigned int backDelay = 1000 );
		byte read( byte controllerOrientation );
};

class switchController: public Controller
{
	private:
		byte upPin, downPin, rightPin, leftPin, enterPin, prevPin, repeatCount;
		bool readPin( byte thePin );
		
	public:
		switchController( byte upPin, byte downPin, byte leftPin, byte rightPin, byte enterPin );
		byte read( byte controllerOrientation );
};

#endif
