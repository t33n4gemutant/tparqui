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
	} else if (!strcmp(buffer, "exit") && current_terminal == 10) {
		monitor_clear();
		monitor_switch_to(1);
		monitor_put('\n');
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
