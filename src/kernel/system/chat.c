#include "defs.h"
#include "keyboard.h"
#include "monitor.h"
#include "commands.h"
#include "chat.h"
#include "serial.h"
#include "bash.h"

static u16int chatTerminal[TERMINAL_SIZE];
static char chatBuffer[CHAT_BUFFER];
u16int chatBufPointer;

void init_chat() {
	chatBufPointer = 0;
	int j = 0;
	 while (j < TERMINAL_SIZE) {
	 chatTerminal[j] = '\0';
	 j++;
	 }
}

void chatInsert(char c) {
	if (c == '\n') {
		communicate();
		clearChatBuffer();
	} else {
		chatBuffer[chatBufPointer++] = c;
	}
}

void communicate() {
	char * pointer = chatBuffer;
	if (!strcmp(pointer, "exit")) {
		chat_to_bash();
	} else {
		int i = 0;
		while (chatBuffer[i] != '\0') {
			port_serial_write(chatBuffer[i++]);
		}
		//Send new line character to host
		port_serial_write('\n');
	}
}

void clearChatBuffer() {
	int i = 0;
	while (i < CHAT_BUFFER) {
		chatBuffer[i] = '\0';
		i++;
	}
	chatBufPointer = 0;
}

void chat_to_bash() {
	current_program = BASH;
	//Stop listening to serial port
	monitor_put('\n');
	serial_listen(0);
	monitor_switch_to(terminals[current_terminal - 1], chatTerminal);
}
