// monitor.h -- Defines the interface for monitor.h
//              From JamesM's kernel development tutorials.

#ifndef MONITOR_H
#define MONITOR_H

#include "common.h"

// Write a single character out to the screen.
void monitor_put(char c);

// Clear the screen to all black.
void monitor_clear();

// Initialize the screen
void init_monitor();

// Output a null-terminated ASCII string to the monitor.
void monitor_write(char *c);

void monitor_switch_to(u16int * terminal, u16int * currentTerminal);

#define BASH 100
#define CHAT 101

#define TERMINAL_SIZE 2000
int current_program;

#endif // MONITOR_H
