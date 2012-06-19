// monitor.h -- Defines the interface for monitor.h
//              From JamesM's kernel development tutorials.

#ifndef MONITOR_H
#define MONITOR_H

#include "common.h"

int current_terminal;

// Write a single character out to the screen.
void monitor_put(char c);

// Clear the screen to all black.
void monitor_clear();

// Initialize the screen
void init_monitor();

// Output a null-terminated ASCII string to the monitor.
void monitor_write(char *c);

void monitor_switch_to(int terminal);

#endif // MONITOR_H
