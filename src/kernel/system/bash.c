#include "defs.h"
#include "keyboard.h"
#include "monitor.h"
#include "commands.h"
#include "bash.h"
#include "serial.h"
#include "chat.h"
#include "kernel.h"

static u16int terminals[TERMINAL_QTY][TERMINAL_SIZE];
static char buffers[TERMINAL_QTY][BASH_BUFFER];
static u16int bufPointer[TERMINAL_QTY];
int current_terminal;

void init_bash() {
	current_terminal = 1;
	monitor_switch_to(terminals[current_terminal], terminals[current_terminal]);
	switch_to(2);
	switch_to(1);

	int i = 0;
	while (i < TERMINAL_QTY) {
		bufPointer[i] = 0;
		i++;
	}
	i = 1;
	int j = 0;
	while (i < TERMINAL_QTY) {
		while (j < TERMINAL_SIZE) {
			terminals[i][j] = terminals[current_terminal - 1][j];
			j++;
		}
		i++;
		j = 0;
	}
}

void bashInsert(char c) {
	if (c == '\n') {
		interpret();
		clearBashBuffer();
	} else {
		if (c == 0x08) {
			buffers[current_terminal][bufPointer[current_terminal]--] = '\0';
		} else {
			buffers[current_terminal][bufPointer[current_terminal]++] = c;
		}
	}
}

void interpret() {
	char * pointer = buffers[current_terminal];
	if (!strcmp(pointer, "clear")) {
		monitor_clear();
		monitor_put('>');
	} else if (!strcmp(pointer, "about")) {
		monitor_write("ArquiOS v0.1 by Elli Federico and Pierri Alan\n>");
	} else if (!strcmp(pointer, "help")) {
		monitor_write("about 		- Display info about this OS\n");
		monitor_write("help 		- Display this help menu\n");
		monitor_write("kblayout 	- Display current keyboard layout\n");
		monitor_write("kben 		- Switch keyboard layout to English\n");
		monitor_write("kbsp 		- Switch keyboard layout to Spanish\n");
		monitor_write(
				"chat 		- Switch to chat mode - Type exit to exit chatmode\n");
		monitor_write("exit 		- Exit OS\n");
		monitor_put('>');

	} else if (!strcmp(pointer, "kblayout")) {
		if (kbLayout) {
			monitor_write("Keyboard currently in Spanish\n>");
		} else {
			monitor_write("Keyboard currently in English\n>");
		}
	} else if (!strcmp(pointer, "kben")) {
		kbLayout = EN;
		monitor_write("Keyboard switched to English\n>");
	} else if (!strcmp(pointer, "kbsp")) {
		kbLayout = SP;
		monitor_write("Keyboard switched to Spanish\n>");
	} else if (!strcmp(pointer, "exit")) {
		run = 0;
	} else if (!strcmp(pointer, "chat")) {
		current_program = CHAT;
		bash_to_chat();
	} else {
		monitor_write("Invalid command - type 'help' for a command list\n>");
	}
}

void switch_to(int next_terminal) {
	if (current_terminal == next_terminal) {
		return;
	}
	monitor_switch_to(terminals[next_terminal - 1],
			terminals[current_terminal - 1]);
	current_terminal = next_terminal;
}

void clearBashBuffer() {
	int i = 0;
	while (i < BASH_BUFFER) {
		buffers[current_terminal][i] = '\0';
		i++;
	}
	bufPointer[current_terminal] = 0;
}

void bash_to_chat() {
	monitor_switch_to(chatTerminal, terminals[current_terminal - 1]);
	monitor_put('\n');
	//Listen to serial port
	serial_listen(1);
}
