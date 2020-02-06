#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void prime_process(int pfd) {
    int p;
    if (read(pfd, &p, sizeof(p)) <= 0) {
        return;
    }

    printf("prime %d\n", p);

    int child_fds[2];
    pipe(child_fds);

    int pid = fork();
    if (pid == 0) {
        close(child_fds[1]);
        prime_process(child_fds[0]);
        close(child_fds[0]);
        exit();
    } else {
        close(child_fds[0]);
        int v;
        while (read(pfd, &v, sizeof(v))) {
            if (v % p != 0) {
                write(child_fds[1], &v, sizeof(v));
            }
        }
        close(child_fds[1]);
        wait();
    }
}

void primes() {
    int fds[2];
    pipe(fds);

    int pid = fork();
    if (pid == 0) {
        close(fds[1]);
        prime_process(fds[0]);
        close(fds[0]);
        exit();
    } else {
        close(fds[0]);
        for (int i = 2; i <= 35; i++) {
            write(fds[1], &i, sizeof(i));
        }
        close(fds[1]);
        wait();
    }
}

int main() {
    primes();
    exit();
}
