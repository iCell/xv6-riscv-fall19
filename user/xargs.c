#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"
#include "kernel/fs.h"

int main(int argc, char *argv[]) {
    int size = 0;
    char *args[MAXARG];
    for (int i = 1; i < argc; i++) {
        args[i-1] = argv[i];
        size++;
    }

    char ch, buf[512];
    while(read(0, &ch, sizeof(ch))>0) {
        if (ch == '\n') {
            break;
        }

        int len = strlen(buf);
        buf[len] = ch;
    }
    buf[strlen(buf)+1] = '\0';

    args[size] = buf;
    size++;
    
    int pid = fork();
    if (pid == 0) {
        exec(args[0], args);
        printf("exec failed\n");
    } else {
        wait();
    }

    exit();
}
