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

void printf(char * formatString, ...) {
	int integer;
	unsigned int unsigenedInteger;
	char * string;
	char out[40];
	char c;

	va_list args;

	va_start(args, formatString);

	while (*formatString != '\0') {
		if (*formatString == '%') {

			formatString++;

			switch (*formatString) {
			case 'c':
				c = va_arg(args, int);
				putc(c);
				break;
			case 's':
				string = va_arg(args, char *);
				prints(string);
				break;
			case 'd':
				integer = va_arg(args, int);
				if (integer < 0) {
					integer = -integer;
					putc('-');
				}
				prints(numberBaseNtoString(integer, 10, out));
				break;
			case 'u':
				unsigenedInteger = va_arg(args, unsigned int);
				prints(numberBaseNtoString(unsigenedInteger, 10, out));
				break;
			case 'o':
				integer = va_arg(args, unsigned int);
				prints(numberBaseNtoString(integer, 8, out));
				break;
			case 'x':
				unsigenedInteger = va_arg(args, unsigned int);
				prints(numberBaseNtoString(unsigenedInteger, 16, out));
				break;
			case '%':
				putc('%');
				break;
			}
		} else {
			putc(*formatString);
		}
		formatString++;
	}
	va_end(args);
}

int sscanf(char *stream, char *format, ...) {
	va_list ap;
	va_start(ap, format);
	int i = 0;
	int j = 0;
	int converted;

	int *integer, iTmp, iTmp2;
	char* string;
	char *chr;
	unsigned int *uinteger;

	while (format[i]) {
		if (format[i] == '%') {
			i++;
			switch (format[i++]) {
			case 'c':
				chr = va_arg(ap, char*);
				*chr = stream[j++];
				break;
			case 'd':
				integer = va_arg(ap, int *);
				iTmp = 0;
				iTmp2 = 1;
				if (stream[j] == '-') {
					iTmp2 = -1;
					j++;
				}
				while (isNumber(stream[j])) {
					iTmp = iTmp * 10 + (stream[j] - '0');
					j++;
				}
				*integer = iTmp * iTmp2;
			case 'u':
				uinteger = va_arg(ap, unsigned int *);
				iTmp = 0;
				while (isNumber(stream[j])) {
					iTmp = iTmp * 10 + (stream[j] - '0');
					j++;
				}
				*uinteger = iTmp;
				break;
			case 's':
				string = va_arg(ap, char *);
				iTmp = 0;
				while (stream[j] != ' ') {
					string[iTmp++] = stream[j++];
				}
				string[iTmp] = '\0';
				break;
			default:
				// WRONG %X
				return converted;
			}
		} else {
			if (format[i] == stream[j]) {
				i++;
				j++;
			} else {
				//WRONG FORMAT STRING
				return converted;
			}
		}
	}
}

static int isNumber(char c) {
	return (c >= '0' && c <= '9');
}

void putc(char c) {
	if (c == '\0') {
		return;
	}
	__write(STDOUT, c);
}

static void prints(char * string) {
	while (*string != '\0') {
		putc(*string);
		string++;
	}
}

static char * numberBaseNtoString(unsigned int number, int base, char * out) {

	int digits[40];
	int position = 0;
	char * numbers = "0123456789ABCDEF";
	int index = 0;

	if (number != 0) {
		while (number > 0) {
			if (number < base) {
				digits[position] = number;
				number = 0;
			} else {
				digits[position] = number % base;
				number /= base;
			}
			position++;
		}

		for (index = 0; position > 0; position--, index++) {
			out[index] = numbers[digits[position - 1] % base];
		}
		out[index] = '\0';
	} else {
		out[0] = '0';
		out[1] = '\0';
	}

	return out;
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
