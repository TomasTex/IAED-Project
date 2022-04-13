#ifndef MODELS_H
#define MODELS_H

#define MAX_AIRPORTS 40
#define MAX_FLIGHTS 30000


#define DATE_SEPARATOR "-"
#define TIME_SEPARATOR ":"
#define DATE_TIME_SEPARATOR " "


#define AIRPORT_ID_LENGTH 4
#define AIRPORT_COUNTRY_MAX_LENGTH 31
#define AIRPORT_CITY_MAX_LENGTH 51

#define FLIGHT_MIN_CODE_LENGTH 4
#define FLIGHT_MAX_CODE_LENGTH 7

#define FLIGHT_MIN_CAPACITY 10
#define FLIGHT_MAX_CAPACITY 100

#define FLIGHT_MAX_DURATION_IN_HOURS 12

typedef struct Airport Airport;

typedef struct Flight Flight;

typedef struct {
    unsigned short day;
    unsigned short month;
    unsigned short year;
} Date;

typedef struct {
    unsigned short hour;
    unsigned short min;
} Time;

typedef struct {
    Date date;
    Time time;
} DateTime;


struct Airport {
    char id[AIRPORT_ID_LENGTH];
    char country[AIRPORT_COUNTRY_MAX_LENGTH];
    char city[AIRPORT_CITY_MAX_LENGTH];

    Flight** flights_incoming;
    int n_flights_incoming;

    Flight** flights_outgoing;
    int n_flights_outgoing;
};


struct Flight {
    char code[FLIGHT_MAX_CODE_LENGTH];

    Airport* origin;
    Airport* destination;

    DateTime departure_at;
    DateTime arrive_at;

    unsigned int capacity;
};


#endif /* MODELS_H */
