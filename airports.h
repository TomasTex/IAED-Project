#ifndef AIRPORTS_H
#define AIRPORTS_H

#include "models.h"

#define AIRPORT_ADD_OUTPUT_FMT "airport %s\n"
#define AIRPORT_LIST_OUTPUT_FMT "%s %s %s %d\n"

/* ERRORS */
#define INVALID_AIRPORT "invalid airport ID"
#define TOO_MANY_AIRPORTS "too many airports"
#define DUPLICATE_AIRPORT "duplicate airport"
#define NO_SUCH_AIRPORT_ID "%s: no such airport ID\n"


unsigned int is_valid_airport_id(char* airport_id);

void add_airport(Airport* airports, int* airports_length, char* id, char* country, char* city);

void list_all_airports_sorted(Airport* airports, int airports_length);
void list_airports(Airport* airports, int airports_length, char** airports_ids, int airports_ids_len);

int find_airport_index_by_id(char* id, Airport *airports, int airports_length);

void add_incoming_flight(Airport *airport, Flight* flight);
void add_outgoing_flight(Airport *airport, Flight* flight);

void remove_incoming_flight(Airport *airport, Flight* flight);
void remove_outgoing_flight(Airport *airport, Flight* flight);

void list_flights_of_airport(
    char* airport_id,
    Airport *airports,
    int airports_length,
    Flight** (*flights)(Airport*),
    int (*n_flights)(Airport*),
    void (*list_flights)(Flight**, const int)
);

void list_incoming_flights(char* airport_id, Airport *airports, int airports_length);
void list_outgoing_flights(char* airport_id, Airport *airports, int airports_length);

#endif /* AIRPORTS_H */
