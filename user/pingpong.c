#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
	int fds[2];
	// create a pipe, with read fd in fds[0], 
	// and write fd in fds[1]
	pipe(fds);

	int pid = fork();
	if (pid == 0) {
		// child
		char buf[4];
		read(fds[0], buf, sizeof(buf));

		int child_pid = getpid();
		printf("%d: received %s\n", child_pid, buf);

		write(fds[1], "pong", 4);
	} else {
		// parent
		write(fds[1], "ping", 4);
		wait();

		char buf[4];
		read(fds[0], buf, sizeof(buf));

		int parent_pid = getpid();
		printf("%d: received %s\n", parent_pid,buf);
	}

	exit();
}
