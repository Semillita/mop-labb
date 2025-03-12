#include "gameloop.h"
#include "ports.h"

void start_game_loop(void (*interrupt_handler)(), unsigned int period_in_us)
{
    *((void (**)(void))(0x2001C000 + 0x3C)) = interrupt_handler;

	unsigned int system_frequency = 168; // Clocks per microsecond 
	// Ladda systick med halv period (i klockor)
	*STK_LOAD = (period_in_us / 2) * system_frequency;
	// Starta systick MED INTERRUPT
	*STK_CTRL = 7; 
}

void stop_game_loop()
{
	*STK_CTRL &= ~0x1; 
}