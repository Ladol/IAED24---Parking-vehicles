/**
 * Definition of the park struct, and of the function prototypes related it.
 * 
 * Parks allow the management of vehicle entries and exits in & out of them.
 * 
 * Author: Adolfo Monteiro
*/
#ifndef PARK_H
#define PARK_H

#include "hashtable.h"
#include "log.h"
#include "plate.h"
#include "tariff.h"

typedef struct park {
    char* name; // name of the park
    int capacity;
    int availableSlots;
    Tariff tariff; // how much to charge for staying in the park
    Hashtable* logTable; // to store entries & exits of vehicles
    struct park* next;
} Park;


// Initializer
Park* newPark(char *name, const int* capacity, const Tariff* tariff);

// Free
void freePark(Park* park);
void freeAllParks(Park* headPark);

// Getters
char* getParkName(Park* park);
int* getAvailableSpots(Park* park);
int* getCapacity(Park* park);
Tariff* getTariff(Park* park);
Hashtable* getTable(const Park* park);
int totalParks(Park* headPark);
Park* findLastPark(Park* headPark);
Park* getPark(Park* headPark, const char* parkName);
Log* getPlateLogs(Park* headPark, const char plate[PLATE_LENGTH]);

// Removal / Insertion
int removePark(Park** headPark, const char* parkName);
int addPark(Park** headPark, Park* park);

// Print parks
void printParks(Park* headPark);
void printParksAlphabetically(Park* headPark);

// Check for plates in parks
int plateInPark(const Park* park, const char plate[PLATE_LENGTH]);
int plateInAnyPark(Park* headPark, const char plate[PLATE_LENGTH]);

void registerEntryExit(Park* park, const char plate[PLATE_LENGTH],
                        const Timestamp* timestamp);

void showParkBilling(Park* park, const Timestamp* timestamp);
#endif