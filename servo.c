// SERVO library
// for bradwii , mini54 port
// uses timer0
// by silverx 2015
// GPL licence due bradwii's licence, for which this file is intended

#include "Mini51Series.h"
#include "lib_timers.h"

#include "servo.h"


// using pin P3.0 so it's port 3 , pin 0
// this is the forward-left led on hubsan boards (107C)
// the minus (-) output is used , the (+) is connected to 3.0V
// the led is left connected ( onboard one only )

#define _servo_port 3
#define _servo_pin 0

// if the output needs inverting
#define _servo_high 1

// command to turn servo pin on
#define SERVOON GPIO_PIN_ADDR(_servo_port, _servo_pin) = _servo_high
// command to turn servo pin off
// this is used inside the interrupt
#define SERVOOFF GPIO_PIN_ADDR(_servo_port, _servo_pin) = !_servo_high

// line to set pin to output, used during init
#define SERVOINIT GPIO_SetMode(((GPIO_T *) (P0_BASE + 0x40 * (_servo_port) )), (1 <<  (_servo_pin) ), GPIO_PMD_OUTPUT)

// minimum servo value
#define SERVOLIMMIN 9000
// max servo value
#define SERVOLIMMAX 24000

volatile uint32_t servopos = 16500;

/**
 * Timer0 IRQ
 * The Timer0 default IRQ, declared in startup_M051Series.s.
 */
void TMR0_IRQHandler(void)
{
    if(TIMER_GetIntFlag(TIMER0) == 1)
    {
    /* Clear Timer0 time-out interrupt flag */
    TIMER_ClearIntFlag(TIMER0);
		SERVOOFF;
    }
}


void servoinit()
{
  // enable clock to timers 0 and 1
  //CLK->APBCLK = CLK->APBCLK|12UL; 
  CLK_EnableModuleClock(TMR0_MODULE);

// set clock source and divider
  CLK_SetModuleClock(TMR0_MODULE , CLK_CLKSEL1_TMR0_S_IRC22M , 1);

	// pulse timer setup ( timer0)
	TIMER_Open(TIMER0, TIMER_ONESHOT_MODE, 1);
	TIMER_EnableInt(TIMER0);	
	NVIC_EnableIRQ(TMR0_IRQn);
	// TIMER_Start(TIMER0);
	SERVOINIT;
	SERVOOFF;
}

unsigned long  servotime;


void setservo( int val)
{
if ( val < SERVOLIMMIN) val = SERVOLIMMIN;
if ( val > SERVOLIMMAX) val = SERVOLIMMAX;
	servopos = val;
	// 50 Hz servo timer
	// perhaps some servos can work faster
	if (lib_timers_gettimermicroseconds(servotime) > 20000 )
	{
		// start pulse and set timer to turn pulse off
		TIMER_SET_CMP_VALUE(TIMER0, servopos);
		SERVOON;
		TIMER_Start(TIMER0);
		servotime = lib_timers_starttimer();
	}
}



