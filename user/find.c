#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

void find(int fd, char *path, char *search) {
    char buf[512], *p;

    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
        printf("find: path too long\n");
        return;
    }
    
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';

    struct dirent de;
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
            continue;
        }

        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;

        int subfd = open(buf, 0);
        struct stat st;
        fstat(subfd, &st);

        switch (st.type) {
        case T_FILE:
            close(subfd);
            if (strcmp(de.name, search) == 0) {
                // find, should return full path
                printf("%s\n", buf);
                return;
            }
            break;
        case T_DIR:
            find(subfd, buf, search);
            close(subfd);
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("arguments too few\n");
        exit();
    }
    
    char *path = argv[1];
    char *search = argv[2];

    int fd = open(path, 0);
    if (fd < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        exit();
    }

    struct stat st;
    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        exit();
    }
    
    if (st.type == T_FILE) {
        fprintf(2, "find: should provide directory as an argument\n");
        exit();
    }

    find(fd, path, search);
    close(fd);
    exit();
}
