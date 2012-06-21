#include "keyboard.h"
#include "commands.h"
#include "monitor.h"
#include "common.h"

void getCommand(char * buffer) {
	if (!strcmp(buffer, "clear")) {
		monitor_clear();
	} else if (!strcmp(buffer, "about")) {
		monitor_write("ArquiOS v0.1 by Elli Federico and Pierri Alan");
	} else if (!strcmp(buffer, "chat")) {
		monitor_switch_to(10);
		init_serial();
	} else if (current_terminal == 10) { //Chat mode
		if (!strcmp(buffer, "exit")) {
			monitor_clear();
			monitor_switch_to(1);
			monitor_put('\n');
		}
		int i = 0;
		while (buffer[i]) {
			port_serial_write(buffer[i++]);
		}
		char c;
		do {
			c = port_serial_read();
			monitor_put(c);
		} while(c != '\n');
	}
}

int isShortcut(char c) {
	if (IS_ALT()) {
		if (c >= '1' && c <= '9') {
			monitor_switch_to((int) (c - '0'));
			return 1;
		}
	}
	return 0;
}
