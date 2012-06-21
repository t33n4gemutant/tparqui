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

	//Enable all interrupts
	_mascaraPIC1(0x00);
	_mascaraPIC2(0x00);

	//Initialize modules
	init_monitor();
	init_timer(50);
	init_keyboard();
	init_serial();
	while (1)
		;
}
