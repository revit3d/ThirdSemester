#include <stdio.h>
#include <time.h>

int
main (void) {
    int year;
    int ans = 0;
    scanf("%d", &year);
    year -= 1900;
    struct tm dt = {
        .tm_year  = year,
        .tm_mon   = 0,
        .tm_mday  = 1,
        .tm_hour  = 0,
        .tm_min   = 0,
        .tm_sec   = 0,
        .tm_isdst = -1,
    };
    timegm(&dt);
    while (dt.tm_year == year) {
        if (dt.tm_yday + 1 == 1
            || dt.tm_yday + 1 == 2
            || dt.tm_yday + 1 == 4
            || dt.tm_yday + 1 == 8
            || dt.tm_yday + 1 == 16
            || dt.tm_yday + 1 == 32
            || dt.tm_yday + 1 == 64
            || dt.tm_yday + 1 == 128
            || dt.tm_yday + 1 == 256
            || dt.tm_wday == 0
            || dt.tm_wday == 6) {
            ans++;
        }
        dt.tm_isdst = -1;
        dt.tm_mday += 1;
        timegm(&dt);
    }
    printf("%d\n", ans);

    return 0;
}   