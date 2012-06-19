#include "kernel/system/monitor.h"
#include "kernel/system/descriptor_tables.h"
#include "kernel/system/timer.h"
#include "kernel/system/isr.h"
#include "util/logger.h"
#include "kernel/system/kasm.h"

/**********************************************
 kmain()
 Punto de entrada de código C.
 *************************************************/

kmain() {
	// Initialize all the ISRs and segmentation
	init_descriptor_tables();

	// Initialize the monitor
	init_monitor();
	_Sti();
	init_timer(50);
	init_keyboard();
	while (1)
		;
}
