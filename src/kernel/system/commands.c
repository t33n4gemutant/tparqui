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
		serial_listen(1);
	} else if (current_terminal == 10) { //Chat mode
		if (!strcmp(buffer, "exit")) {
			monitor_clear();
			monitor_switch_to(1);
			monitor_put('\n');
			serial_listen(0);
		}

		int i = 0;
		while (buffer[i]) {
			port_serial_write(buffer[i++]);
		}
		//Send new line character to host
		port_serial_write('\n');
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
