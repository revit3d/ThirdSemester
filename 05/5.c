#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <libgen.h>
#include <fcntl.h>
#include <stdio.h>

enum { 
    ERROR = -1,
    SUCCESS = 0,
    NULL_TERM = 1,
    READN = 4 * 1024,
    NO_CALLOC = 100000,
};

void
get_file_path (char *srcpath, char *dstpath, char* file_path) {
    enum { SLASH = 1 };
    char *src_name = basename(srcpath);
    int srclen = strlen(src_name);
    int dstlen = strlen(dstpath);
    int pos;
    for (pos = 0; pos < dstlen; pos++) {
        file_path[pos] = dstpath[pos];
    }
    if (pos - 1 >= 0 && dstpath[pos - 1] == '/' && src_name[0] == '/') {
        pos++;
    } else if (pos - 1 >= 0 && dstpath[pos - 1] != '/' && src_name[0] != '/') {
        file_path[pos] = '/';
        pos++;
    }
    for (int i = 0; i <= srclen; i++) {
        file_path[pos + i] = src_name[i];
    }
}

void
strcp (const char *str, char* cp) {
    int strn = strlen(str);
    for (int i = 0; i <= strn; i++) {
        cp[i] = str[i];
    }
}

int 
flsh_buf(int dst, char *buf, int size) {
    int wrcnt;
    int rdcnt = size;
    while (size) {
        wrcnt = write(dst, buf + (rdcnt - size), size);
        if (wrcnt == -1) {
            return ERROR;
        }
        size -= wrcnt;
    }
    return SUCCESS;
}

int
copy_file_contents (const char* srcpath, const char* dstpath, int mode) {
    enum { READ_END = 0 };
    int src, dst;
    if ((src = open(srcpath, O_RDONLY)) == ERROR
        || (dst = open(dstpath, O_WRONLY | O_TRUNC | O_CREAT, mode)) == ERROR) {
        return ERROR;
    }
    char buf[READN];
    int rdcnt;
    int rd_step;
    while (1) {
        rdcnt = 0;
        while (rdcnt < READN) {
            rd_step = read(src, buf + rdcnt, READN - rdcnt);
            if (rd_step == ERROR) {
                return ERROR;
            }
            rdcnt += rd_step;
            if (rd_step == READ_END) {
                int res = flsh_buf(dst, buf, rdcnt);
                close(src);
                close(dst);
                return res;
            }
        }
        if (flsh_buf(dst, buf, rdcnt) == ERROR) {
            return ERROR;
        }
    }
    close(src);
    close(dst);
    return SUCCESS;
}

int
copy_file (const char *srcpath, const char *dstpath) {
    if (srcpath == NULL || dstpath == NULL) {
        return ERROR;
    }
    char srcpath_cp[NO_CALLOC];
    char dstpath_cp[NO_CALLOC];
    char new_srcpath[NO_CALLOC];
    enum { MASK = 0x1ff };
    int mode_mask = MASK;
    int src_inode;
    struct stat file_info;
    if (stat(srcpath, &file_info) == ERROR
        || S_ISDIR(file_info.st_mode) 
        || access(srcpath, R_OK) == ERROR) {
        return ERROR;
    }
    src_inode = file_info.st_ino;
    mode_mask &= file_info.st_mode;
    if (stat(dstpath, &file_info) == ERROR
        || !S_ISDIR(file_info.st_mode)) {
        if (stat(dstpath, &file_info) != ERROR
            && src_inode == file_info.st_ino) {
            return SUCCESS;
        }
        if (copy_file_contents(srcpath, dstpath, mode_mask) == ERROR) {
            return ERROR;
        }
    } else if (S_ISDIR(file_info.st_mode)) {
        strcp(srcpath, srcpath_cp);
        strcp(dstpath, dstpath_cp);
        get_file_path(srcpath_cp, dstpath_cp, new_srcpath);
        if (stat(new_srcpath, &file_info) != ERROR
            && src_inode == file_info.st_ino) {
            return SUCCESS;
        }
        if (copy_file_contents(srcpath, new_srcpath, mode_mask) == ERROR) {
            return ERROR;
        }
    }

    return SUCCESS;
}