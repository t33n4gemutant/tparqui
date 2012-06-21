// monitor.c -- Defines functions for writing to the monitor.
//             heavily based on Bran's kernel development tutorials,
//             but rewritten for JamesM's kernel tutorials.

#include "monitor.h"
#include "commands.h"

// The VGA framebuffer starts at 0xB8000.
u16int *video_memory = (u16int *) 0xB8000;
// Stores the cursor position.
u8int cursor_x = 0;
u8int cursor_y = 0;
char buffer[80];

int TERMINAL_SIZE = 25 * 80;
static u16int terminals[4][2000];


// Updates the hardware cursor.
static void move_cursor() {
	// The screen is 80 characters wide...
	u16int cursorLocation = cursor_y * 80 + cursor_x;
	outb(0x3D4, 14); // Tell the VGA board we are setting the high cursor byte.
	outb(0x3D5, cursorLocation >> 8); // Send the high cursor byte.
	outb(0x3D4, 15); // Tell the VGA board we are setting the low cursor byte.
	outb(0x3D5, cursorLocation); // Send the low cursor byte.
}

// Scrolls the text on the screen up by one line.
static void scroll() {

	// Get a space character with the default colour attributes.
	u8int attributeByte = (0 /*black*/<< 4) | (15 /*white*/& 0x0F);
	u16int blank = 0x20 /* space */| (attributeByte << 8);

	// Row 25 is the end, this means we need to scroll up
	if (cursor_y >= 25) {
		// Move the current text chunk that makes up the screen
		// back in the buffer by a line
		int i;
		for (i = 0 * 80; i < 24 * 80; i++) {
			video_memory[i] = video_memory[i + 80];
		}

		// The last line should now be blank. Do this by writing
		// 80 spaces to it.
		for (i = 24 * 80; i < 25 * 80; i++) {
			video_memory[i] = blank;
		}
		// The cursor should now be on the last line.
		cursor_y = 24;
	}
}

// Writes a single character out to the screen.
void monitor_put(char c) {
	// The background color is black (0), the foreground is white (15).
	u8int backColour = 0;
	u8int foreColour = 15;

	// The attribute byte is made up of two nibbles - the lower being the
	// foreground color, and the upper the background colour.
	u8int attributeByte = (backColour << 4) | (foreColour & 0x0F);
	// The attribute byte is the top 8 bits of the word we have to send to the
	// VGA board.
	u16int attribute = attributeByte << 8;
	u16int *location;

	// Handle a backspace, by moving the cursor back one space and deleting last character
	if (c == 0x08 && cursor_x) {
		buffer[cursor_x] = '\0';
		cursor_x--;
		location = video_memory + (cursor_y * 80 + cursor_x);
		if (cursor_x == 0) {
			cursor_x++;
		} else {
			*location = ' ' | attribute;
		}
	}

	// Handle a tab by increasing the cursor's X, but only to a point
	// where it is divisible by 8.
	else if (c == 0x09) {
		buffer[cursor_x - 1] = c;
		cursor_x = (cursor_x + 8) & ~(8 - 1);
	}

	// Handle carriage return
	else if (c == '\r') {
		buffer[cursor_x - 1] = c;
		cursor_x = 0;
	}

	// Handle newline by moving cursor back to left and increasing the row
	else if (c == '\n') {
		cursor_x = 0;
		cursor_y++;
		getCommand(buffer);
		int i = 0;
		while (i < 80) {
			buffer[i] = '\0';
			i++;
		}
		location = video_memory + (cursor_y * 80 + cursor_x);
		*location = '>' | attribute;
		cursor_x = 1;
	}
	// Handle any other printable character.
	else if (c >= ' ') {
		buffer[cursor_x - 1] = c;
		location = video_memory + (cursor_y * 80 + cursor_x);
		*location = c | attribute;
		cursor_x++;
	}

	// Check if we need to insert a new line because we have reached the end
	// of the screen.
	if (cursor_x >= 80) {
		cursor_x = 0;
		cursor_y++;
		int i = 0;
		while (i < 80) {
			buffer[i] = '\0';
			i++;
		}
	}

	// Scroll the screen if needed.
	scroll();
	// Move the hardware cursor.
	move_cursor();

}

// Clears the screen, by copying lots of spaces to the framebuffer.
void monitor_clear() {
	// Make an attribute byte for the default colors
	u8int attributeByte = (0 /*black*/<< 4) | (15 /*white*/& 0x0F);
	u16int blank = 0x20 /* space */| (attributeByte << 8);

	int i;
	for (i = 0; i < 80 * 25; i++) {
		video_memory[i] = blank;
	}

	// Move the hardware cursor back to the start.
	cursor_x = 0;
	cursor_y = 0;
	move_cursor();
}

// Outputs a null-terminated ASCII string to the monitor.
void monitor_write(char *c) {
	int i = 0;
	while (c[i]) {
		monitor_put(c[i++]);
	}
}

void monitor_write_hex(u32int n) {
	s32int tmp;

	monitor_write("0x");

	char noZeroes = 1;

	int i;
	for (i = 28; i > 0; i -= 4) {
		tmp = (n >> i) & 0xF;
		if (tmp == 0 && noZeroes != 0) {
			continue;
		}

		if (tmp >= 0xA) {
			noZeroes = 0;
			monitor_put(tmp - 0xA + 'a');
		} else {
			noZeroes = 0;
			monitor_put(tmp + '0');
		}
	}

	tmp = n & 0xF;
	if (tmp >= 0xA) {
		monitor_put(tmp - 0xA + 'a');
	} else {
		monitor_put(tmp + '0');
	}

}

void monitor_write_dec(u32int n) {

	if (n == 0) {
		monitor_put('0');
		return;
	}

	s32int acc = n;
	char c[32];
	int i = 0;
	while (acc > 0) {
		c[i] = '0' + acc % 10;
		acc /= 10;
		i++;
	}
	c[i] = 0;

	char c2[32];
	c2[i--] = 0;
	int j = 0;
	while (i >= 0) {
		c2[i--] = c[j++];
	}
	monitor_write(c2);

}

void init_monitor() {

	// Initialize the screen (by clearing it)
	monitor_clear();
	// Write out a sample string
	monitor_write(
			"--------------------------------------------------------------------------------");
	monitor_write(
			"                               ArquiOS v0.1                                     ");
	monitor_write(
			"--------------------------------------------------------------------------------");
	monitor_write(">");

	current_terminal = 1;
	monitor_switch_to(2);
	monitor_switch_to(1);

	int i = 1;
	int j = 0;
	while (i < 10) {
		while (j < TERMINAL_SIZE) {/*
		 terminals[i][j] = (' ' | attribute);*/
			terminals[i][j] = terminals[current_terminal][j];
			j++;
		}
		i++;
		j = 0;
	}
}

void monitor_switch_to(int next_terminal) {
	if (current_terminal == next_terminal) {
		return;
	}
	u8int backColour = 0;
	u8int foreColour = 15;
	u8int attributeByte = (backColour << 4) | (foreColour & 0x0F);
	u16int attribute = attributeByte << 8;
	u16int *location;
	int i = 0;
	while (i < TERMINAL_SIZE) {
		location = video_memory + i;
		terminals[current_terminal][i] = *location;
		i++;
	}
	monitor_clear();
	u8int aux_x = 0;
	u8int aux_y = 0;
	while ((cursor_x + (cursor_y) * 80) < (TERMINAL_SIZE - 1)) {
		location = video_memory + (cursor_y * 80 + cursor_x);
		*location = terminals[next_terminal][cursor_x + cursor_y * 80];
		cursor_x++;
		if (cursor_x == 80) {
			cursor_x = 0;
			cursor_y++;
		}
		if (terminals[next_terminal][cursor_x + cursor_y * 80]
				!= (' ' | attribute)) {
			aux_x = cursor_x;
			aux_y = cursor_y;
		}
		scroll();
		//move_cursor();
	}
	cursor_x = aux_x;
	cursor_y = aux_y;
	cursor_x++;
	scroll();
	move_cursor();
	current_terminal = next_terminal;
}
