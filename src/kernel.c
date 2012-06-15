#include "kernel/system/monitor.h"
#include "kernel/system/descriptor_tables.h"
#include "kernel/system/timer.h"
#include "util/logger.h"
/**********************************************
 kmain()
 Punto de entrada de código C.
 *************************************************/

func(registers_t reg) {
	monitor_write('key pressed');
}

kmain() {
	// Initialize all the ISRs and segmentation
	init_descriptor_tables();
	register_interrupt_handler(32, &func);
	// Initialize the screen (by clearing it)
	monitor_clear();
	// Write out a sample string
	monitor_write("Hello, world!\n");
	log("chupala");
	asm volatile("int $0x3");
	asm volatile("int $0x4");

	asm volatile("sti");
	init_timer(50000);
	//asm volatile("cli");
}
