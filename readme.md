
# LCDControlPanel

On an Arduino this enables the creation of menus for the Hitachi HD44780U based LCD displays and a rotary encoder or directional switch.

The LCDControlPanel is designed to be fairly simple to implement, have a high level of flexibility in use and consume the minimum amount of resources but is it not recommended for Arduinos with the ATMega 168 microcontroller due to the limited amount of memory on those devices. The menu item types available are text lists, lo-res analogs, hi-res analogs, decimal (i.e. floating point) numbers and integer numbers. It depends upon the NewLiquidCrystal library which is available from https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads/
