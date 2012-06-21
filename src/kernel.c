#include "kernel/system/monitor.h"
#include "kernel/system/descriptor_tables.h"
#include "kernel/system/timer.h"
#include "kernel/system/isr.h"
#include "util/logger.h"
#include "kernel/system/kasm.h"
#include "kernel/system/kernel.h"
#include "kernel/system/bash.h"

void int80h(u8int sysCallNumber, void ** args);
void stop();
void int80handler(registers_t regs);
int current_program = -1;

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

	// Initialize modules
	init_monitor();
	init_timer(50);
	init_keyboard();
	current_program = BASH;
	init_bash();
	init_chat();
	init_serial();
	register_interrupt_handler(80, &int80handler);
	run = 1;
	_Sti();
	while (run) {
		char c = getc(STDIN);
		if (c != '\0') {
			monitor_put(c);
			if (current_program == BASH) {
				bashInsert(c);
			}
			if (current_program == CHAT) {
				chatInsert(c);
			}
		}
	}
}

void int80handler(registers_t regs) {
	_int80hand(regs);
}

void int80h(u8int sysCallNumber, void ** args) {

	/*switch (sysCallNumber) {*/
	switch ((u8int) args[15]) {

	case SYSTEM_READ:
		sysread((u8int) args[12], (char *) args[1]);
		break;
	case SYSTEM_WRITE:
		syswrite((u8int) args[12], (char) args[1]);
		break;
	default:
		monitor_write(
				"Error: Se llamo a la int80 con un valor no reconocido:<");
		monitor_write_dec(sysCallNumber);
		monitor_write(">; fileDescriptor: ");
		monitor_write_dec((u8int) args[0]);
		monitor_put('\n');
		monitor_write("Otros valores");
		monitor_write("  <");
		monitor_write_dec((u8int) args[1]);
		monitor_write(">  <");
		monitor_write_dec((u8int) args[2]);
		monitor_write(">  <");
		monitor_write_dec((u8int) args[3]);
		monitor_write(">  <");
		monitor_write_dec((u8int) args[4]);
		monitor_write(">  <");
		monitor_write_dec((u8int) args[5]);
		monitor_write(">  <");
		monitor_write_dec((u8int) args[6]);
		monitor_write(">  <");
		monitor_write_dec((u8int) args[7]);
		monitor_write(">  <");
		monitor_write_dec((u8int) args[8]);
		monitor_write(">  <");
		monitor_write_dec((u8int) args[9]);
		monitor_write(">  <");
		monitor_write_dec((u8int) args[10]);
		monitor_write(">  <");
		monitor_write_dec((u8int) args[11]);
		monitor_write(">  <");
		monitor_write_dec((u8int) args[12]);
		monitor_write(">  <");
		monitor_write_dec((u8int) args[13]);
		monitor_write(">  <");
		monitor_write_dec((u8int) args[14]);
		monitor_write(">  <");
		monitor_write_dec((u8int) args[15]);
		monitor_write(">  <");
		monitor_write_dec((u8int) args[16]);
		monitor_write(">\n");
		break;
	}
	return;
}

void stop() {
	monitor_write("STOPPED");
	while (1)
		;
}
