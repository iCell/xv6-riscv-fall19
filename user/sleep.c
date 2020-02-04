#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
	if (argc <= 1) {
		printf("should provide the ticks you want\n");
		exit();
	}

	int duration = atoi(argv[1]);
	if (sleep(duration) < 0) {
		printf("invoke kernel sleep error\n");
		exit();
	}
	exit();
}
