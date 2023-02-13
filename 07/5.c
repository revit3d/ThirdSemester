#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int
main (void) {
    enum DATE_OFFSETS {
        YEAR_OFFSET   = 1900,
        MON_OFFSET    = 1,
        YDAY_OFFSET   = 1,
        HOUR_OFFSET   = 1,
        MINUTE_OFFSET = 1,
        MONDAY        = 1,
        WEEK          = 7,
        SUNDAY        = 7,
        NOT_SPECIFIED = -1,
    };
    enum TIME_CONVERSION {
        SECONDS_MINUTE = 60,
        MINUTES_HOUR   = 60,
        HOURS_DAY      = 24,
    };
    enum SYNODIC_INTERVAL {
        //29 days 12 hours 44 minutes
        DAYS_SYNODIC_INTERV    = 29,
        HOURS_SYNODIC_INTERV   = 12,
        MINUTES_SYNODIC_INTERV = 44,
    };
    enum SYNODIC_DATE {
        //2021 may 26 11:14:00 UTC
        SYNODIC_YEAR   = 2021,
        SYNODIC_MON    = 5,
        SYNODIC_MDAY   = 26,
        SYNODIC_HOUR   = 11,
        SYNODIC_MINUTE = 14,
        SYNODIC_SECOND = 0,
    };
    enum {
        DAYS_THRESHOLD = 255,
    };

    //29 days 12 hours 44 minutes
    time_t off = ((DAYS_SYNODIC_INTERV * HOURS_DAY 
        + HOURS_SYNODIC_INTERV)        * MINUTES_HOUR 
        + MINUTES_SYNODIC_INTERV)      * SECONDS_MINUTE;
    int year;
    scanf("%d", &year);
    struct tm dt = {
        .tm_year  = year - YEAR_OFFSET,
        .tm_mon   = 0,
        .tm_mday  = 1,
        .tm_hour  = 0,
        .tm_min   = 0,
        .tm_sec   = 0,
        .tm_isdst = NOT_SPECIFIED,
    };
    time_t tgt = timegm(&dt) + DAYS_THRESHOLD * HOURS_DAY * MINUTES_HOUR * SECONDS_MINUTE;
    //2021 may 26 11:14:00 UTC
    dt.tm_year  = SYNODIC_YEAR   - YEAR_OFFSET;
    dt.tm_mon   = SYNODIC_MON    - MON_OFFSET;
    dt.tm_mday  = SYNODIC_MDAY;
    dt.tm_hour  = SYNODIC_HOUR   - HOUR_OFFSET;
    dt.tm_min   = SYNODIC_MINUTE - MINUTE_OFFSET;
    dt.tm_sec   = SYNODIC_SECOND;
    dt.tm_isdst = NOT_SPECIFIED;
    time_t start = timegm(&dt);
    long long dst = ((long long)tgt - start) / off + ((long long)tgt - start > 0);
    time_t ans = start + dst * off;
    struct tm *res = gmtime(&ans);
    if (res->tm_wday == MONDAY) {
        res->tm_mday += WEEK;
    } else {
        res->tm_mday += WEEK - (res->tm_wday == 0 ? SUNDAY : res->tm_wday) + MONDAY;
    }
    //skip three weeks
    res->tm_mday += WEEK * 3;
    res->tm_isdst = NOT_SPECIFIED;
    timegm(res);
    printf("%04d-%02d-%02d\n", res->tm_year + YEAR_OFFSET, res->tm_mon + MON_OFFSET, res->tm_mday);

    return 0;
}