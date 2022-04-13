#include "datetime.h"

#include <stdio.h>


Date parse_date(char *fmt_date) {
    Date date;

    sscanf(fmt_date, "%hu-%hu-%hu", &date.day, &date.month, &date.year);

    return date;
}

Time parse_time(char *fmt_time) {
    Time time;

    sscanf(fmt_time, "%hu:%hu", &time.hour, &time.min);

    return time;
}

DateTime parse_datetime(char* fmt_date, char* fmt_time) {
    DateTime datetime;

    datetime.date = parse_date(fmt_date);
    datetime.time = parse_time(fmt_time);

    return datetime;
}

void show_date(Date* date) {
    printf(DATE_OUTPUT_FMT, date->day, date->month, date->year);
}

DateTime datetime_increment(const DateTime* src, const Time* time) {
    DateTime dst;

    dst.date.day = src->date.day;
    dst.date.month = src->date.month;
    dst.date.year = src->date.year;

    dst.time.hour = src->time.hour + time->hour;
    dst.time.min = src->time.min + time->min;

    if (dst.time.min >= 60) {
        dst.time.min -= 60;
        dst.time.hour++;
    }

    if (dst.time.hour >= 24) {
        dst.time.hour -= 24;
        dst.date.day++;
    }

    if (dst.date.month == 2 && dst.date.day > 28) { /* leap year is out of the scope in this project */
        dst.date.month++;
        dst.date.day = 1;
    }

    if (dst.date.day > 30) {
        if (dst.date.month == 4 || dst.date.month == 6 || dst.date.month == 9 || dst.date.month == 11) {
            dst.date.month++;
            dst.date.day = 1;
        }

        if (dst.date.day > 31) {
            dst.date.month++;
            dst.date.day = 1;
        }
    }

    if (dst.date.month > 12) {
        dst.date.year++;
        dst.date.month = 1;
    }

    return dst;
}


int cmp_dates(Date* date_a, Date* date_b) {
    if (date_a->year > date_b->year) {
        return 1;
    }

    if (date_a->year < date_b->year) {
        return -1;
    }

    if (date_a->month > date_b->month) {
        return 1;
    }

    if (date_a->month < date_b->month) {
        return -1;
    }

    if (date_a->day > date_b->day) {
        return 1;
    }

    if (date_a->day < date_b->day) {
        return -1;
    }

    return 0;
}

int cmp_times(Time* time_a, Time* time_b) {
    if (time_a->hour > time_b->hour) {
        return 1;
    }

    if (time_a->hour < time_b->hour) {
        return -1;
    }

    if (time_a->min > time_b->min) {
        return 1;
    }

    if (time_a->min < time_b->min) {
        return -1;
    }

    return 0;
}

int cmp_datetimes(DateTime* datetime_a, DateTime* datetime_b) {
    {
        int date_diff = cmp_dates(&datetime_a->date, &datetime_b->date);
        if (date_diff != 0) {
            return date_diff;
        }
    }
    {
        int time_diff = cmp_times(&datetime_a->time, &datetime_b->time);
        if (time_diff != 0) {
            return time_diff;
        }
    }

    return 0;
}

/* Adapted from:
   - https://overiq.com/c-examples/c-program-to-calculate-the-difference-of-two-dates-in-years-months-and-days/
*/
Date date_diff(Date *date_a, Date *date_b) {
    Date diff = *date_a;

    if(diff.day < date_b->day) {
        if (diff.month == 2) {
            /* leap year */
            if ((diff.year % 4 == 0 && diff.year % 100 != 0) || (diff.year % 400 == 0)) {
                diff.day += 29;
            } else {
                diff.day += 28;
            }
        } else if (diff.month == 4 || diff.month == 6 || diff.month == 9 || diff.month == 11) {
           diff.day += 30;
        } else{
           diff.day += 31;
        }

        diff.month = diff.month - 1;
    }

    if (diff.month < date_b->month) {
        diff.month += 12;
        diff.year -= 1;
    }

    diff.day = diff.day - date_b->day;
    diff.month = diff.month - date_b->month;
    diff.year = diff.year - date_b->year;

    return diff;
}

int is_valid_date(Date *future, Date* now) {
    int cmp = cmp_dates(future, now);

    if (cmp == 0) {
        return 1;
    }

    if (cmp < 0) {
        return 0;
    }

    {
        Date diff = date_diff(future, now);

        if (diff.year > 1) {
            return 0;
        }

        if (diff.year == 0) {
            return 1;
        }

        return diff.year == 1 && diff.month == 0 && diff.day == 0;
    }
}
