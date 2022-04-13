#include "airports.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "sort.h"
#include "flights.h"


unsigned int is_valid_airport_id(char* airport_id) {
    int i;

    if (strlen(airport_id) != AIRPORT_ID_LENGTH - 1) {
        return 0;
    }

    for (i = 0; i < AIRPORT_ID_LENGTH - 1; ++i) {
        if (!isupper(airport_id[i])) {
            return 0;
        }
    }

    return 1;
}

void add_airport(Airport* airports, int* airports_length, char* id, char* country, char* city) {
    Airport airport;
    int i;

    /* validate airport ID */
    if (! is_valid_airport_id(id)) {
        puts(INVALID_AIRPORT);
        return;
    }

    /* validate max airports */
    if (*airports_length >= MAX_AIRPORTS) {
        puts(TOO_MANY_AIRPORTS);
        return;
    }

    /* validate duplicate airport */
    for (i = 0; i < *airports_length; ++i) {
        if (strcmp(airports[i].id, id) == 0) {
            puts(DUPLICATE_AIRPORT);
            return;
        }
    }

    strncpy(airport.id, id, AIRPORT_ID_LENGTH - 1);
    airport.id[AIRPORT_ID_LENGTH - 1] = 0;

    /* validate city is not completly empty */
    {
        int city_len = strlen(city);
        int n_spaces = 0;

        for (i = 0; i < city_len; ++i) {
            if (isspace(city[i])) {
                ++n_spaces;
            }
        }

        if (n_spaces == city_len) {
            return;
        }
    }

    strncpy(airport.country, country, AIRPORT_COUNTRY_MAX_LENGTH - 1);
    airport.country[AIRPORT_COUNTRY_MAX_LENGTH - 1] = 0;

    strncpy(airport.city, city, AIRPORT_CITY_MAX_LENGTH);
    airport.city[AIRPORT_CITY_MAX_LENGTH - 1] = 0;

    airport.flights_incoming = NULL;
    airport.n_flights_incoming = 0;

    airport.flights_outgoing = NULL;
    airport.n_flights_outgoing = 0;

    airports[(*airports_length)++] = airport;

    printf(AIRPORT_ADD_OUTPUT_FMT, airport.id);
}


int find_airport_index_by_id(char* id, Airport *airports, int airports_length) {
    int i;

    for (i = 0; i < airports_length; ++i) {
        if (strcmp(airports[i].id, id) == 0) {
            return i;
        }
    }

    return -1;
}

int cmp_airports(const Airport* airport_a, const Airport* airport_b) {
    return strcmp(airport_a->id, airport_b->id);
}

void list_all_airports_sorted(Airport *airports, int airports_length) {
    Airport* airports_cpy = calloc(airports_length, sizeof(Airport));
    memcpy(airports_cpy, airports, airports_length * sizeof(Airport));

    bsort(airports_cpy, airports_length, sizeof(Airport), (int (*) (const void*, const void*)) cmp_airports);

    {
        int i;

        for (i = 0; i < airports_length; ++i) {
            printf(AIRPORT_LIST_OUTPUT_FMT, airports_cpy[i].id, airports_cpy[i].city, airports_cpy[i].country, airports_cpy[i].n_flights_outgoing);
        }
    }

    free(airports_cpy);
    airports_cpy = NULL;
}

void list_airports(Airport* airports, int airports_length, char** airports_ids, int airports_ids_len) {
    if (airports_ids_len == 0) {
        list_all_airports_sorted(airports, airports_length);
        return;
    }

    {
        int i;

        for (i = 0; i < airports_ids_len; ++i) {
            int airport_index = find_airport_index_by_id(airports_ids[i], airports, airports_length);
            if (airport_index != -1) {
                printf(
                    AIRPORT_LIST_OUTPUT_FMT,
                    airports[airport_index].id,
                    airports[airport_index].city,
                    airports[airport_index].country,
                    airports[airport_index].n_flights_outgoing
                );
            } else {
                printf(NO_SUCH_AIRPORT_ID, airports_ids[i]);
            }
        }
    }
}

void add_incoming_flight(Airport *airport, Flight* flight) {
    size_t requested_size = (airport->n_flights_incoming + 1) * sizeof(flight);

    airport->flights_incoming = (Flight**) realloc(airport->flights_incoming, requested_size);
    airport->flights_incoming[airport->n_flights_incoming++] = flight;
}

void add_outgoing_flight(Airport *airport, Flight* flight) {
    size_t requested_size = (airport->n_flights_outgoing + 1) * sizeof(flight);

    airport->flights_outgoing = (Flight**) realloc(airport->flights_outgoing, requested_size);
    airport->flights_outgoing[airport->n_flights_outgoing++] = flight;
}


int remove_flight(Flight** flights, int flights_length, Flight* flight) {
    int i, j, k;

    for (i = 0; i < flights_length; ++i) {
        if (flights[i] == flight) {
            for (j = i, k = i + 1; k < flights_length; ++j, ++k) {
                flights[j] = flights[k];
            }

            return 1;
        }
    }

    return 0;
}

void remove_incoming_flight(Airport *airport, Flight* flight) {
    if (remove_flight(airport->flights_incoming, airport->n_flights_incoming, flight)) {
        size_t requested_size = (airport->n_flights_incoming - 1) * sizeof(flight);

        airport->flights_incoming = (Flight**) realloc(airport->flights_incoming, requested_size);
        airport->n_flights_incoming--;
    }
}

void remove_outgoing_flight(Airport *airport, Flight* flight) {
    if (! remove_flight(airport->flights_outgoing, airport->n_flights_outgoing, flight)) {
        size_t requested_size = (airport->n_flights_outgoing - 1) * sizeof(flight);

        airport->flights_outgoing = (Flight**) realloc(airport->flights_outgoing, requested_size);
        airport->n_flights_outgoing--;
    }
}

int n_incoming_flights(Airport* airport) {
    return airport->n_flights_incoming;
}

Flight** incoming_flights(Airport* airport) {
    return airport->flights_incoming;
}

int n_outgoing_flights(Airport* airport) {
    return airport->n_flights_outgoing;
}

Flight** outgoing_flights(Airport* airport) {
    return airport->flights_outgoing;
}

void list_flights_of_airport(
    char* airport_id,
    Airport *airports,
    int airports_length,
    Flight** (*flights)(Airport*),
    int (*n_flights)(Airport*),
    void (*list_flights)(Flight**, const int)
) {
    int airport_index = find_airport_index_by_id(airport_id, airports, airports_length);

    if (airport_index == -1) {
        printf(NO_SUCH_AIRPORT_ID, airport_id);
        return;
    }

    (*list_flights)((*flights)(airports + airport_index), (*n_flights)(airports + airport_index));
}

/* listar voos com chegado ao aeroporto airport_id */
void list_incoming_flights(char* airport_id, Airport* airports, int airports_length) {
    list_flights_of_airport(
        airport_id,
        airports,
        airports_length,
        incoming_flights,
        n_incoming_flights,
        list_departing_flights
    );
}

/* listar voos com partida do aeroporto airport_id */
void list_outgoing_flights(char* airport_id, Airport *airports, int airports_length) {
    list_flights_of_airport(
        airport_id,
        airports,
        airports_length,
        outgoing_flights,
        n_outgoing_flights,
        list_arriving_flights
    );
}
