#ifndef SERIAL_H_
#define SERIAL_H_

//Initializes serial port
void init_serial();

//
int serial_received();

char read_serial();

int is_transmit_empty();

void write_serial(char c);

#endif /* SERIAL_H_ */
