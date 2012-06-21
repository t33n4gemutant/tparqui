#include "defs.h"
#include "stdarg.h"
#include "common.h"
#include "io.h"

static char wbuffer[80];
int bufferpointer;

char getBufferChar() {
	if (bufferpointer == 0) {
		return '\0';
	}
	char ret = wbuffer[0];
	int i = 0;
	while (i < bufferpointer) {
		wbuffer[i] = wbuffer[i + 1];
		i++;
	}
	bufferpointer--;
	return ret;
}

void sysread(int fd, char * c) {
	if (fd == STDIN) {
		*c = getBufferChar();
		return;
	}
}

void syswrite(int fd, char c) {
	if (fd == STDOUT) {
		wbuffer[bufferpointer++] = c;
	}
}

void printf(const char * format, ...) {
	int escaped = 0;
	int special = 0;
	va_list args;
	va_start(args, format);
	while (*format != '\0') {
		if (special) {
			if (*format == 'd' || *format == 'i') {
				putd(va_arg(args, int));
			}
			if (*format == 'c') {
				putc(va_arg(args, char));
			}
			if (*format == 'f') {
				putf(va_arg(args, double));
			}
			if (*format == 'u') {
				putu(va_arg(args, unsigned));
			}
		} else if (escaped) {
			if (*format == 'n') {
				putc('\n');
			} else if (*format == 't') {
				printf("    ");
			} else {
				putc(*format);
			}

		}

		if (*format == '\\') {
			if (!escaped) {
				escaped = 1;
			} else {
				escaped = 0;
			}
		} else {
			escaped = 0;
		}
		if (*format == '%') {
			special = 1;
		} else {
			special = 0;
		}
		format++;
	}
	va_end(args);
}

void putc(char c) {
	if (c == '\0') {
		return;
	}
	__write(STDOUT, c);
}

void putd(char c) {
}
void putf(char c) {
}
void putu(char c) {
}

void __write(int fileDescriptor, char c) {
	_SystemCalls(SYSTEM_WRITE, fileDescriptor, c);
}

char __read(int fd) {
	char c[1];
	_SystemCalls(SYSTEM_READ, fd, c);
	return c[0];
}

char getc() {
	return __read(STDIN);
	/*_SystemCalls(SYSTEM_READ, fileDescriptor);*/
}
