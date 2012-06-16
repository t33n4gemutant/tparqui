#include "logger.h"

static void prints(char * string);
static char * numberBaseNtoString(unsigned int number, int base, char * out);
static void printLevel(enum LogLevel level);

void port_parallel_write(char c) {
    outb(0x37a, 0x04|0x08);
    outb(0x378, (unsigned char)c);
    outb(0x37a, 0x01);
}

static void logc(char c) {
	//port_serial_write(c);
	port_parallel_write(c);
}

static void printLevel(enum LogLevel level) {
    char *levelNames[] = {"FATAL", "ERROR", "INFO", "DEBUG", "TRACE"};
    logc('[');
    prints(levelNames[level]);
    prints("]\t");
}

int _log(char* file, int line, enum LogLevel level, char *formatString, ...) {
    if (level > LOG_LEVEL) {
        return -1;
    }
    va_list args;
    va_start(args, formatString);
    int integer;
    unsigned int unsigenedInteger;
    char * string;
    char out[40];
    char c;
    char end[100];
        prints(file);
        logc(':');
        prints(numberBaseNtoString(line, 10, end));
        logc(' ');
    printLevel(level);
    while (*formatString != '\0') {
        if (*formatString == '%') {

            formatString++;

            switch (*formatString) {
                case 'c':
                    c = va_arg(args, int);
                    logc(c);
                    break;
                case 's':
                    string = va_arg(args, char *);
                    prints(string);
                    break;
                case 'd':
                    integer = va_arg(args, int);
                    if (integer < 0) {
                        integer = -integer;
                        logc('-');
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
                    logc('%');
                    break;
            }
        } else {
            logc(*formatString);
        }
        formatString++;
    }
    va_end(args);
    logc('\n');
    return 0;
}

static void prints(char * string) {
    while (*string != '\0') {
        logc(*string);
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
