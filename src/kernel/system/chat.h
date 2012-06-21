#ifndef CHAT_H
#define CHAT_H
#define CHAT_BUFFER 80

void chatInsert(char c);
void communicate();
void clearChatBuffer();
void chat_to_bash();

static u16int chatTerminal[TERMINAL_SIZE];
static char chatBuffer[CHAT_BUFFER];
u16int chatBufPointer;

#endif
