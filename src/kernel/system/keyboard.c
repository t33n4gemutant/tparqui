#include "keyboard.h"
#include "isr.h"
#include "monitor.h"
#include "commands.h"
#include "../../util/logger.h"

unsigned char lcase[60] = { 0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9',
		'0', '-', '=', '\b', /* Backspace */
		'\t', /* Tab */'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[',
		']', '\n', /* Enter key */
		0, /* 29 - Control */'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
		'\'', '`', 0,
		/* Left shift */'\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
		0, /* Right shift */'*', 0,
		/* Alt */' ' /* Space bar */
};

unsigned int accen[60] = { 0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9',
		'0', '-', '=', '\b', /* Backspace */
		'\t', /* Tab */'q', 'w', 130, 'r', 't', 'y', 163, 161, 162, 'p', '[',
		']', '\n', /* Enter key */
		0, /* 29 - Control */160, 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
		'\'', '`', 0,
		/* Left shift */'\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
		0, /* Right shift */'*', 0,
		/* Alt */' ' /* Space bar */
};

unsigned int spanl[60] = { 0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9',
		'0', '\'', '¿', '\b', /* Backspace */
		'\t', /* Tab */'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', ' ',
		'+', '\n', /* Enter key */
		0, /* 29 - Control */'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'ñ',
		'{', '|', 0,
		/* Left shift */'<', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-',
		0, /* Right shift */'*', 0,
		/* Alt */' ' /* Space bar */
};

unsigned int spanu[60] = { 0, 27, '!', '"', '#', '$', '%', '&', '/', '(', ')',
		'=', '?', '¡', '\b', /* Backspace */
		'\t', /* Tab */'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', ' ',
		'*', '\n', /* Enter key */
		0, /* 29 - Control */'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'Ñ',
		'[', '°', 0,
		/* Left shift */'|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ';', ':', '_',
		0, /* Right shift */'*', 0,
		/* Alt */' ' /* Space bar */
};

unsigned char ucase[60] = { 0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(',
		')', '_', '+', '\b', /* Backspace */
		'\t', /* Tab */'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{',
		'}', '\n', /* Enter key */
		0, /* 29 - Control */'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',
		'"', '~', 0,
		/* Left shift */'|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',
		0, /* Right shift */'*', 0,
		/* Alt */' ' /* Space bar */
};

/* Handles the keyboard interrupt */
void keyboard_handler(registers_t regs) {
	u8int scancode;

	/* Read from the keyboard's data buffer */
	scancode = inb(0x60);
	log(L_INFO, "scancode: %d", scancode);
	handleScanCode(scancode);
}

void handleScanCode(unsigned char scanCode) {
	if (!checkSpecialKey(scanCode)) {
		if (!IS_BREAK(scanCode)) {
			char c = translateSc(CLEAR_BREAK_BIT(scanCode));
			if (!isShortcut(c)) {
				putc(c);
			}
		}
	}
}

int checkSpecialKey(unsigned char scanCode) {
	int ret = 0, fbit;
	if (IS_ESCAPE()) {
		switch (scanCode) {
		case 0x53:
			kbFlags |= DEL;
			break;
		case 0xD3:
			kbFlags &= ~DEL;
			break;
		default:
			break;
		}
		kbFlags &= ~ESCAPE;
		return 1;
	}
	switch (scanCode) {
	/* Shifts */
	case 0x2A:
		kbFlags |= UCASE;
		break;
	case 0xAA:
		kbFlags &= ~UCASE;
		break;
	case 0x36:
		kbFlags |= UCASE;
		break;
	case 0xB6:
		kbFlags &= ~UCASE;
		break;

		/* Ctrls */
	case 0x1D:
		kbFlags |= CTRL;
		break;
	case 0x9D:
		kbFlags &= ~CTRL;
		break;

		/* Alts */
	case 0x38:
		kbFlags |= ALT;
		break;
	case 0xB8:
		kbFlags &= ~ALT;
		break;
		/* Escaped scan codes */
	case 0xE0:
		kbFlags |= ESCAPE;
		break;
		/* Accents */
	case 0x28:
		kbFlags &= ~ACCENT;
		break;
	case 0xA8:
		kbFlags |= ACCENT;
		break;
		/* F1 through F10 */
	case 0x3B:
	case 0x3C:
	case 0x3D:
	case 0x3E:
	case 0x3F:
	case 0x40:
	case 0x41:
	case 0x42:
	case 0x43:
	case 0x44:
		kbFlags |= FN;
		fbit = 1 << (scanCode - 0x3B);
		fKeys |= fbit;
		break;
	case 0xBB:
	case 0xBC:
	case 0xBD:
	case 0xBE:
	case 0xBF:
	case 0xC0:
	case 0xC1:
	case 0xC2:
	case 0xC3:
	case 0xC4:
		fbit = (1 << (scanCode - 0xBB));
		kbFlags &= ~FN;
		fbit = 0; // turns off all F key flags
		fKeys &= fbit;
		break;
	default:
		ret = 0;
		break;
	}
	return ret;
}

char translateSc(unsigned char scanCode) {
	if (SHIFT_PRESSED()) {
		if (IS_SPANISH()) {
			return spanu[scanCode];
		} else {
			return ucase[scanCode];
		}
	} else {
		if (IS_SPANISH()) {
			return spanl[scanCode];
		}
	}
	if (IS_CTRL()) {
		return accen[scanCode];
	}
	return lcase[scanCode];
}

/* Installs the keyboard handler into IRQ1 */

void init_keyboard() {
	fKeys = 0;
	register_interrupt_handler(IRQ1, &keyboard_handler);
}
