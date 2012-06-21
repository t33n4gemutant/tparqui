#ifndef BASH_H
#define BASH_H
#define BASH_BUFFER 80

void bashInsert(char c);
void switch_to(int next_terminal);
void interpret();
void clearBashBuffer();
void bash_to_chat();
static u16int terminals[TERMINAL_QTY][TERMINAL_SIZE];
static char buffers[TERMINAL_QTY][BASH_BUFFER];
static u16int bufPointer[TERMINAL_QTY];
int current_terminal;


#endif
