#include "serial.h"
#include "isr.h"
#include "../../util/logger.h"

#define COM1 0x3f8

/* Handles the serial port interrupt */
void serial_handler(registers_t regs) {
	log(L_INFO, "handler");
	char c = port_serial_read();
	monitor_put(c);
}

void init_serial() {
	register_interrupt_handler(IRQ4, &serial_handler);

	outb(COM1 + 1, 0x00); // Disable all interrupts
	outb(COM1 + 3, 0x80); // Enable DLAB (set baud rate divisor)
	outb(COM1 + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
	outb(COM1 + 1, 0x00); //                  (hi byte)
	outb(COM1 + 3, 0x03); // 8 bits, no parity, one stop bit
	outb(COM1 + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
	outb(COM1 + 4, 0x0B); // IRQs enabled, RTS/DSR set
}

int serial_received() {
	return inb(COM1 + 5) & 1;
}

char port_serial_read() {
	while (serial_received() == 0)
		;
	return inb(COM1);
}

int is_transmit_empty() {
	return inb(COM1 + 5) & 0x20;
}

void port_serial_write(char a) {
	while (is_transmit_empty() == 0)
		;

	outb(COM1, a);
}
