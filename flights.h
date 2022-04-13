#ifndef FLIGHTS_H
#define FLIGHTS_H

#include "models.h"

#define FLIGHT_OUTPUT_LIST_FMT "%s %s %s %02d-%02d-%04d %02d:%02d\n"
#define FLIGHT_AIRPORT_OUTPUT_LIST_FMT "%s %s %02d-%02d-%04d %02d:%02d\n"

/* ERRORS */
#define INVALID_FLIGHT_CODE "invalid flight code"
#define FLIGHT_ALREADY_EXISTS "flight already exists"
#define FLIGHT_NO_SUCH_AIRPORT_ID "%s: no such airport ID\n"
#define TOO_MANY_FLIGHTS "too many flights"
#define INVALID_DURATION "invalid duration"
#define INVALID_CAPACITY "invalid capacity"


void add_flight(
    Flight*** flights,
    int* flights_length,
    Airport* airports,
    int airports_length,
    Date* now,
    char* flight_code,
    char* airport_origin_id,
    char* airport_destination_id,
    char* departure_date,
    char* departure_time,
    char* duration,
    char* capacity
);

void list_flights(Flight** flights, const int flights_length);

void list_airport_flights(
    Flight** flights,
    const int flights_length,
    Airport* (*airport)(Flight*),
    DateTime (*flight_moment)(Flight*),
    int (*cmp_flights)(Flight*, Flight*)
);
void list_departing_flights(Flight** flights, const int flights_length);
void list_arriving_flights(Flight** flights, const int flights_length);

#endif /* FLIGHTS_H */
