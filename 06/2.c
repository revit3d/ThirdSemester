#include <stdlib.h>
#include <limits.h>

int 
strln(char *buf) {
    int len;
    for (len = 0; len < INT_MAX; len++) {
        if (buf[len] == '\0') {
            break;
        }
    }
    return len;
}

void 
normalize_path(char *buf) {
    enum {
        PREV_DIRECTORY = 2, //len of .. (prev dir alias)
        CUR_DIRECTORY = 1   //len of . (cur dir alias)
    };
    if (buf == NULL) {
        return;
    }
    int len = strln(buf);
    if (len > 1 && buf[len - 1] == '/') {
        buf[len - 1] = '\0';
        len--;
    }
    int cnt;
    for (int i = 0; i < len; i++) {
        if (buf[i] == '/' && i + PREV_DIRECTORY < len
            && buf[i + 1] == '.' && buf[i + PREV_DIRECTORY] == '.') {
            cnt = PREV_DIRECTORY;
            i += PREV_DIRECTORY;
            if (i + 1 < len && buf[i + 1] != '/') {
                continue;
            }
            while (cnt) {
                if (i == 0) {
                    break;
                } else if (buf[i] == '/') {
                    cnt--;
                }
                buf[i] = '\0';
                i--;
            }
        } else if (buf[i] == '/' && i + CUR_DIRECTORY < len
            && buf[i + 1] == '.') {
            cnt = CUR_DIRECTORY;
            i += CUR_DIRECTORY;
            if (i + 1 < len && buf[i + 1] != '/') {
                continue;
            }
            while (cnt) {
                if (i == 0) {
                    break;
                }
                if (buf[i] == '/') {
                    cnt--;
                }
                buf[i] = '\0';
                i--;
            }
        }
    }
    int pos = 0;
    int next = 0;
    while (next < len) {
        while (buf[next] == '\0') {
            if (next == len) {
                break;
            }
            next++;
        }
        if (pos > 0 && buf[pos - 1] == '/' && buf[next] == '/') {
            pos--;
        }
        while (buf[next] != '\0') {
            buf[pos] = buf[next];
            next++;
            pos++;
        }
    }
    buf[pos] = '\0';
}