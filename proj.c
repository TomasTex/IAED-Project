#include "proj.h"

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "models.h"
#include "datetime.h"
#include "airports.h"
#include "flights.h"


void parse_args(char* buffer, Input* in) {
    char* word = NULL;
    char sep[] = " \t\n";
    size_t n_args_size = 128 * sizeof(char*);

    word = strtok(buffer, sep);
    if (!word || strlen(word) != 1) {
        return;
    }

    in->cmd = word[0];

    in->args = realloc(in->args, n_args_size);

    while((word = strtok(NULL, sep))) {
        size_t request_count = strlen(word) + 1;

        if ((in->n_args * sizeof(char*)) > n_args_size) {
            n_args_size += 128 * sizeof(char*);
            in->args = realloc(in->args, n_args_size);
        }

        in->args[in->n_args] = (char*) calloc(request_count, sizeof(char));

        strncpy(in->args[in->n_args], word, request_count);

        in->n_args++;

        if (in->cmd == 'a' && in->n_args == 2) {
            strncpy(sep, "\n", strlen(sep) + 1);
        }
    }
}



Input* parse_input() {
    Input* input = NULL;

    char *line = NULL;
    size_t line_len = 0;

    if ((getline(&line, &line_len, stdin)) != -1) {
        input = (Input *) calloc(1, sizeof(Input));

        parse_args(line, input);

        free(line);
        line = NULL;
        line_len = 0;
    }

    return input;
}

void free_input(Input* input) {
    int i;

    if (! input) {
        return;
    }

    for (i = 0; i < input->n_args; ++i) {
        free(input->args[i]);
        input->args[i] = NULL;
    }

    if (input->args) {
        free(input->args);
        input->args = NULL;
    }

    free(input);
    input = NULL;
}


void set_now(Date* now, char* date_fmt) {
    Date future = parse_date(date_fmt);

    if (!is_valid_date(&future, now)) {
        puts(INVALID_DATE);
    } else {
        *now = future;
        show_date(now);
    }
}

void free_flights(Flight*** flights, int* flights_length) {
    int i;

    for (i = 0; i < *flights_length; ++i) {
        Flight* flight = (*flights)[i];

        remove_incoming_flight(flight->destination, flight);
        remove_outgoing_flight(flight->origin, flight);

        free(flight);
        (*flights)[i] = NULL;
    }

    if (*flights) {
        free(*flights);
        *flights = NULL;
    }

    *flights_length = 0;
}

void free_airports(Airport* airports, int* airports_length) {
    int i;

    for (i = 0; i < *airports_length; ++i) {
        if (airports[i].flights_incoming) {
            free(airports[i].flights_incoming);
        }

        if (airports[i].flights_outgoing) {
            free(airports[i].flights_outgoing);
        }

        memset(airports + i, 0, sizeof(Airport));
    }

    *airports_length = 0;
}

int main() {
    Airport airports[MAX_AIRPORTS] = { 0 };
    int airports_length = 0;

    Flight** flights = NULL;
    int flights_length = 0;

    Date now = parse_date(INITIAL_DATE);

    int running = 1;
    while (running) {
        Input* input = parse_input();

        if (! input) {
            continue;
        }

        switch (input->cmd) {
            case 'a':
                if (input->n_args >= 3) {
                    add_airport(airports, &airports_length, input->args[0], input->args[1], input->args[2]);
                }
                break;

            case 'l':
                list_airports(airports, airports_length, input->args, input->n_args);
                break;

            case 'v':
                if (input->n_args >= 7) {
                    add_flight(
                        &flights,
                        &flights_length,
                        airports,
                        airports_length,
                        &now,
                        input->args[0],
                        input->args[1],
                        input->args[2],
                        input->args[3],
                        input->args[4],
                        input->args[5],
                        input->args[6]
                    );
                } else {
                    list_flights(flights, flights_length);
                }
                break;

            case 'p':
                if (input->n_args == 1) {
                    list_outgoing_flights(input->args[0], airports, airports_length);
                }
                break;

            case 'c':
                if (input->n_args == 1) {
                    list_incoming_flights(input->args[0], airports, airports_length);
                }
                break;

            case 't':
                set_now(&now, *input->args);
                break;

            case 'q':
                running = 0;
                break;
        }

        free_input(input);
    }

    free_flights(&flights, &flights_length);
    free_airports(airports, &airports_length);

    return 0;
}
