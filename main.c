/* Robinson Paniagua
 * Lab# 2 Interrupts
 * May 5th 2017
 * main.c
 *         Debugger Configurations
Notes -1) Installed Updated TivaWare Library from either website or Resource Explorer
 *       2) Reminder to set Debugger to ICDI, and Device to TM4C
 *       3) Changed File Properties Configuration to include Arm Compiler > include option> File Path: TivaWare Library
 *       4) Changed File Debug Configurations Arm Linker>File Path Search>include :TivaWare Library
 *       5) Driver Library needs to be installed as project, compiled and linked before the debugger can work properly.
 */
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/pin_map.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"

void Timer0IntHandler();
int main(void) {
	uint32_t Period;
	
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN); //Initializes the board Clock
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);                                      //Enables the GPIO Clock Gating Control
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);         //Set the GPIO Pin Direction
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);                                     // Enables the Peripheral Timer
	TimerConfigure(TIMER0_BASE,TIMER_CFG_PERIODIC);                                   //COnfigures the TIMER0
	Period = (SysCtlClockGet()/10)/2;                                                 // Sets the period to 4 MHZ -> 400/10/2
	TimerLoadSet(TIMER0_BASE,TIMER_A, Period -10);                                     //Sets the timer
	IntEnable(INT_TIMER0A);
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	IntMasterEnable();
	TimerEnable(TIMER0_BASE, TIMER_A);

	while(1)
	{
		Timer0IntHandler();        //Calls on the interrupt function

	}


}

// Interrupt Function
void Timer0IntHandler(void)
{
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);         //Initializes timer

	if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2))          //If GPIO Pin 2 (Led Blue) is on then turn it off
	{
		GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0);
	}

	else                                                  //Turn On GPIO Pin 2, Green LED
	{
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 4);
	}
}
