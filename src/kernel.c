#include "kernel/system/monitor.h"
#include "kernel/system/descriptor_tables.h"
#include "kernel/system/timer.h"
#include "kernel/system/isr.h"
#include "util/logger.h"

/**********************************************
 kmain()
 Punto de entrada de código C.
 *************************************************/

kmain() {
	// Initialize all the ISRs and segmentation
	init_descriptor_tables();

	// Initialize the screen (by clearing it)
	monitor_clear();
	// Write out a sample string
	monitor_write("Hello, world!\n");
	asm volatile("int $0x3");
	asm volatile("int $0x4");

	asm volatile("sti");
	init_timer(50);
	init_keyboard();
	while(1);
	//asm volatile("cli");
}
