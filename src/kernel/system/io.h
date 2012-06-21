#ifndef IO_H
#define IO_H

void __write(int fd, char c);
char __read(int fd);
char getBufferChar();
void sysread(int fd, char * c);
void syswrite(int fd, char c);
void printf(const char * format, ...);
void putc(char c);
void putd(char c);
void putu(char c);
void putf(char c);
char getc();

#endif
