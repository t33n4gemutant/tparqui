#include "defs.h"
#include "keyboard.h"
#include "commands.h"
#include "monitor.h"
#include "common.h"
#include "bash.h"

int isShortcut(char c){
	if( IS_ALT() ){
		if( c >= '1' && c <= '9'){
			switch_to((int)(c - '0'));
			return 1;
		}
	}
	return 0;
}
