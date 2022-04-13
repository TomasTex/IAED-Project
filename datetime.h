#ifndef DATETIME_H
#define DATETIME_H

#include "models.h"

#define DATE_OUTPUT_FMT "%02hu-%02hu-%04hu\n"
#define INVALID_DATE "invalid date"

Date parse_date(char* fmt_date);
Time parse_time(char* fmt_time);

DateTime parse_datetime(char* fmt_date, char* fmt_time);

void show_date(Date* date);

DateTime datetime_increment(const DateTime* datetime, const Time* time);

int cmp_dates(Date* date_a, Date* date_b);
int cmp_times(Time* time_a, Time* time_b);
int cmp_datetimes(DateTime* datetime_a, DateTime* datetime_b);

int is_valid_date(Date *future, Date* now);

#endif /* DATETIME_H */
