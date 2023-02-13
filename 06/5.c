#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h> 
#include <string.h>
#include <stdio.h>

enum { ERROR = -1 };

ssize_t 
get_dir_path(int fd, char *buf, int size, DIR *prev_dir) {
    enum { 
        MAX_PATH_LEN = 4 + NAME_MAX,
        PATH = 2,
    };
    struct stat chd_file_info, pr_file_info;
    struct dirent* dir_info;
    int name_len;
    int prev_fd;
    char curpath[MAX_PATH_LEN] = "./";
    if (prev_dir != NULL) {
        prev_fd = dirfd(prev_dir);
    } else {
        prev_fd = fd;
    }
    //save stat for current fd
    if (prev_fd == ERROR || fstat(prev_fd, &chd_file_info) == ERROR) {
        return ERROR;
    }
    //close previous folder
    if (prev_dir != NULL) {
        if (closedir(prev_dir) == ERROR) {
            return ERROR;
        }
    }
    //open parent directory
    DIR *dirp = opendir("..");
    //move to parent directory
    if (dirp == NULL || chdir("..") == ERROR) {
        return ERROR;
    }
    //read all files in dir until we find the file with 
    //the same inode the child directory has and not symlink
    while ((dir_info = readdir(dirp)) != NULL) {
        if (!strcmp(dir_info->d_name, ".")) {
            continue;
        }
        name_len = strlen(dir_info->d_name) + PATH;
        for (int i = PATH; i < name_len; i++) {
            curpath[i] = dir_info->d_name[i - PATH];
        }
        curpath[name_len] = '\0';
        if (lstat(curpath, &pr_file_info) == ERROR) {
            return ERROR;
        }
        if (pr_file_info.st_ino == chd_file_info.st_ino
            && pr_file_info.st_dev == chd_file_info.st_dev
            && !S_ISLNK(pr_file_info.st_mode)) {
            //found the child directory
            //check if we are in root
            if (!strcmp(dir_info->d_name, "..")) {
                if (size > 0) {
                    buf[0] = '/';
                }
                if (dirp != NULL) {
                    if (closedir(dirp) == ERROR) {
                        return ERROR;
                    }
                }
                return 1;
            }
            break;
        }
    }
    //if child is never found
    if (dir_info == NULL) {
        return ERROR;
    }
    //recursive descent to parent dir
    ssize_t path_add = get_dir_path(fd, buf, size, dirp);
    if (path_add == ERROR) {
        return ERROR;
    }
    //add current dir name to path
    name_len = strlen(curpath);
    curpath[name_len] = '/';
    for (int i = PATH; i < name_len + 1; i++) {
        if (path_add < size) {    
            buf[path_add] = curpath[i];
        }
        path_add += 1;
    }
    return path_add;
}

ssize_t 
getcwd2(int fd, char *buf, size_t size) {
    DIR *curdir = opendir(".");
    int curfd;
    int buf_len = (int)size - 1;
    if (curdir == NULL || (curfd = dirfd(curdir)) == ERROR || fchdir(fd) == ERROR) {
        return ERROR;
    }
    ssize_t dir_path_len = get_dir_path(fd, buf, buf_len, NULL);
    if (fchdir(curfd) == ERROR || closedir(curdir) == ERROR) {
        return ERROR;
    }
    if (dir_path_len > 1) {
        dir_path_len -= 1;
    }
    if (buf_len > dir_path_len) {
        buf_len = dir_path_len;
    }
    if (buf_len >= 0) {
        buf[buf_len] = '\0';
    }
    return dir_path_len;
}