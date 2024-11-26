#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

int coin_flip() {
	return rand() % 2 == 0 ? true : false;
}