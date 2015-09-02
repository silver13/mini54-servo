// servo library for mini54 boards
// the library supports exactly one servo
//
//
// usage:
//
//
// #include "servo.h"
//
//
// in setup call
// servoinit();
//
// call each loop
// setservo( int value);
//
// the servo only updates every 20ms
// a 20 ms LPF might help
//
// servo values currently range from 9000 to 24000
// they correspond with 800uS - 2000uS approx ( guess)
// they are the end limits of a hk-5330 servo
// they are (probably) not in ms*10 but they are liniar
//
// needs to be called at least every 20ms
// for 50Hz update rate
// 
// if using the led pins comment them out of the flashing code or they will "flash" the servo
//
// you can miss calling the servo, nothing will happen
//
