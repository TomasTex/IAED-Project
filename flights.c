#include "flights.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "datetime.h"
#include "airports.h"
#include "sort.h"

unsigned int is_valid_flight_code(char* flight_code) {
    int flight_code_len = strlen(flight_code);

    if (flight_code_len < FLIGHT_MIN_CODE_LENGTH - 1) {
        return 0;
    }

    if (flight_code_len > FLIGHT_MAX_CODE_LENGTH - 1) {
        return 0;
    }

    if (!isupper(flight_code[0]) || !isupper(flight_code[1])) {
        return 0;
    }

    {
        int i;
        for (i = 2; i < flight_code_len; ++i) {
            if (!isdigit(flight_code[i])) {
                return 0;
            }
        }
    }

    if(flight_code[2] == '0') {
        return 0;
    }

    return 1;
}


int flight_exists(
    Flight** flights,
    int flights_length,
    char* flight_code,
    Date* flight_departure_date
) {
    int i;

    for (i = 0; i < flights_length; ++i) {
        Flight* flight = flights[i];

        if (cmp_dates(&flight->departure_at.date, flight_departure_date) == 0) {
            if (strcmp(flight->code, flight_code) == 0) {
                return 1;
            }
        }
    }

    return 0;
}


int is_valid_capacity(char* capacity) {
    int i;
    int capacity_len = strlen(capacity);

    for (i = 0; i < capacity_len; ++i) {
        if (!isdigit(capacity[i])) {
            return 0;
        }
    }

    return 1;
}

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
) {
    int airport_origin_index = -1;
    int airport_destination_index = -1;
    DateTime flight_departure;
    Time flight_duration;
    int flight_capacity;

    /* validate flight code */
    if (!is_valid_flight_code(flight_code)) {
        puts(INVALID_FLIGHT_CODE);
        return;
    }

    flight_departure = parse_datetime(departure_date, departure_time);

    /* validate flight already exists*/
    if (flight_exists(*flights, *flights_length, flight_code, &flight_departure.date)) {
        puts(FLIGHT_ALREADY_EXISTS);
        return;
    }

    /* validate origin airport flight */
    airport_origin_index = find_airport_index_by_id(airport_origin_id, airports, airports_length);
    if (airport_origin_index == -1) {
        printf(FLIGHT_NO_SUCH_AIRPORT_ID, airport_origin_id);
        return;
    }

    /* validate destination airport flight */
    airport_destination_index = find_airport_index_by_id(airport_destination_id, airports, airports_length);
    if (airport_destination_index == -1) {
        printf(FLIGHT_NO_SUCH_AIRPORT_ID, airport_destination_id);
        return;
    }

    /* validate max flights allowed */
    if (*flights_length >= MAX_FLIGHTS) {
        puts(TOO_MANY_FLIGHTS);
        return;
    }

    /* validate departure date is in the future and with max future of 1 year */
    if (! is_valid_date(&flight_departure.date, now)) {
        puts(INVALID_DATE);
        return;
    }

    /* validates max duration in hours of a flight */
    flight_duration = parse_time(duration);
    if (flight_duration.hour > FLIGHT_MAX_DURATION_IN_HOURS) {
        puts(INVALID_DURATION);
        return;
    }

    if (flight_duration.hour == FLIGHT_MAX_DURATION_IN_HOURS && flight_duration.min != 0) {
        puts(INVALID_DURATION);
        return;
    }

    /* validates capacity range of a flight */
    if (!is_valid_capacity(capacity)) {
        puts(INVALID_CAPACITY);
        return;
    }

    flight_capacity = atoi(capacity);

    if (flight_capacity < FLIGHT_MIN_CAPACITY || flight_capacity > FLIGHT_MAX_CAPACITY) {
        puts(INVALID_CAPACITY);
        return;
    }

    {
        Flight* flight = (Flight*) calloc(1, sizeof(Flight));

        strncpy(flight->code, flight_code, FLIGHT_MAX_CODE_LENGTH - 1);
        flight->code[FLIGHT_MAX_CODE_LENGTH - 1] = 0;

        flight->origin = airports + airport_origin_index;
        flight->destination = airports + airport_destination_index;
        flight->departure_at = flight_departure;
        flight->arrive_at = datetime_increment(&flight_departure, &flight_duration);
        flight->capacity = flight_capacity;

        add_outgoing_flight(flight->origin, flight);
        add_incoming_flight(flight->destination, flight);

        *flights = (Flight**) realloc(*flights, (*flights_length + 1) * sizeof(Flight*));
        (*flights)[*flights_length] = flight;

        (*flights_length)++;
    }
}


Airport* flight_departure_airport(Flight* flight) {
    return flight->origin;
}

DateTime flight_departure_moment(Flight* flight) {
    return flight->departure_at;
}

Airport* flight_arrival_airport(Flight* flight) {
    return flight->destination;
}

DateTime flight_arrival_moment(Flight* flight) {
    return flight->arrive_at;
}


void list_flights(Flight** flights, const int flights_length) {
    int i;

    for (i = 0; i < flights_length; ++i) {
        printf(
            FLIGHT_OUTPUT_LIST_FMT,
            flights[i]->code,
            flights[i]->origin->id,
            flights[i]->destination->id,
            flights[i]->departure_at.date.day,
            flights[i]->departure_at.date.month,
            flights[i]->departure_at.date.year,
            flights[i]->departure_at.time.hour,
            flights[i]->departure_at.time.min
        );
    }
}

int cmp_departing_flights(Flight* flight_a, Flight* flight_b) {
    return cmp_datetimes(&flight_a->departure_at, &flight_b->departure_at);
}

int cmp_arriving_flights(Flight* flight_a, Flight* flight_b) {
    return cmp_datetimes(&flight_a->arrive_at, &flight_b->arrive_at);
}


void list_airport_flights(
    Flight** flights,
    const int flights_length,
    Airport* (*airport)(Flight*),
    DateTime (*flight_moment)(Flight*),
    int (*cmp_flights)(Flight*, Flight*)
) {
    int i;

    Flight* flights_cpy = (Flight*) malloc(flights_length * sizeof(Flight));

    for (i = 0; i < flights_length; ++i) {
        strcpy(flights_cpy[i].code, flights[i]->code);
        flights_cpy[i].origin = flights[i]->origin;
        flights_cpy[i].destination = flights[i]->destination;
        flights_cpy[i].departure_at = flights[i]->departure_at;
        flights_cpy[i].arrive_at = flights[i]->arrive_at;
        flights_cpy[i].capacity = flights[i]->capacity;
    }

    bsort(flights_cpy, flights_length, sizeof(Flight), (int (*) (const void*, const void*)) cmp_flights);

    for (i = 0; i < flights_length; ++i) {
        DateTime datetime = (*flight_moment)(flights_cpy + i);

        printf(
            FLIGHT_AIRPORT_OUTPUT_LIST_FMT,
            flights_cpy[i].code,
            (*airport)(flights_cpy + i)->id,
            datetime.date.day,
            datetime.date.month,
            datetime.date.year,
            datetime.time.hour,
            datetime.time.min
        );
    }

    free(flights_cpy);
    flights_cpy = NULL;
}

void list_departing_flights(Flight** flights, const int flights_length) {
    list_airport_flights(
        flights,
        flights_length,
        flight_departure_airport,
        flight_arrival_moment,
        cmp_arriving_flights
    );
}

void list_arriving_flights(Flight** flights, const int flights_length) {
    list_airport_flights(
        flights,
        flights_length,
        flight_arrival_airport,
        flight_departure_moment,
        cmp_departing_flights
    );
}
