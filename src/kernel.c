#include "../kernel/driver/monitor.h"

/**********************************************
 kmain()
 Punto de entrada de c�digo C.
 *************************************************/

kmain() {
	monitor_clear();
	monitor_write("Hello, world!");
}
