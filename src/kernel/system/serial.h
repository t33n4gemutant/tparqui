#ifndef SERIAL_H_
#define SERIAL_H_

//Initializes serial port
void init_serial();

//
int serial_received();

char port_serial_read();

int is_transmit_empty();

void port_serial_write(char c);

#endif /* SERIAL_H_ */
