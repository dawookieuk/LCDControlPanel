#include "LCDControlPanel.h"

MenuBase::MenuBase( const char * name): name( name )
{
	nextItem = 0;
	name = 0;
}

void MenuBase::begin( LCDControlPanel * )
{
}
