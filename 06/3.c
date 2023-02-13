#include <time.h>
#include <stdio.h>
#include <errno.h>

int
main (void) {
    enum {
        START_YEAR = 1900,
        START_MON = 0,
        START_DAY = 1,
        START_HOUR = 12,
        START_MINUTES = 0,
        START_SECONDS = 0,
        SUCCESS = 0,
        DAYLIGHT_SVG_UNKNOWN = -1,
        DAYS_IN_WEEK = 7,
        THURSDAY = 4,
        MON_SHIFT = 1,
        MOD = 3,
    };
    struct tm curdate;
    int year;
    int thurs_num;
    int prev_mon;
    scanf("%d", &year);
    year -= START_YEAR;
    curdate.tm_year = year;
    curdate.tm_mon = START_MON;
    curdate.tm_mday = START_DAY - 1;
    curdate.tm_hour = START_HOUR;
    curdate.tm_min = START_MINUTES;
    curdate.tm_sec = START_SECONDS;
    errno = SUCCESS;
    do {
        curdate.tm_mday++;
        curdate.tm_isdst = DAYLIGHT_SVG_UNKNOWN;
        timegm(&curdate);
        if (errno) {
            return 1;
        }
    } while (curdate.tm_wday != THURSDAY);

    thurs_num = 1;
    while (curdate.tm_year == year) {
        //this is a constant from the problem, how do I name it?
        if (curdate.tm_mday % MOD != 0
            && (thurs_num & 1) == 0) {
            printf("%d %d\n", curdate.tm_mon + MON_SHIFT, curdate.tm_mday);
        }
        curdate.tm_mday += DAYS_IN_WEEK;
        prev_mon = curdate.tm_mon;
        curdate.tm_isdst = DAYLIGHT_SVG_UNKNOWN;
        timegm(&curdate);
        if (prev_mon == curdate.tm_mon) {
            thurs_num++;
        } else {
            thurs_num = 1;
        }
    }

    return 0;
}