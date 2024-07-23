/**
 * Definition of the log struct, and of the function prototypes related to logs
 * 
 * Logs allow the storage of the entry and exit timestamps of specific
 * plate on a specific park.
 * 
 * Author: Adolfo Monteiro
*/
#ifndef LOG_H
#define LOG_H

#include "plate.h"
#include "tariff.h"
#include "timestamp.h"

typedef struct log{
    char plate[PLATE_LENGTH];
    char* parkName;
    Timestamp entryTimestamp; // Timestamp when the vehicle enters the park
    Timestamp exitTimestamp;  // Timestamp when the vehicle exits the park
    struct log* next;
} Log;

// Initialization
Log* newLog(const char plate[PLATE_LENGTH], char* parkName);
void copyLogTimestamps(Log* dest, const Log* source);

// Memory freeing
void freeLog(Log* log);

// Getters
char* getLogParkName(Log* log);
char* getLogPlate(Log* log);
Timestamp* getEntryTimestamp(Log* log);
Timestamp* getExitTimestamp(Log* log);
Log* findLastLog(Log* log);

// Display logs
void printLog(Log* log);
void printDailyBillLog(Log* log, Tariff* tariff);
void printFullBillLog(Log* log, Tariff* tariff);

// Sort a log
void mergeSort(Log** head, const char sortBy);

// Add a new log entry to a log linked list
void addLogtoLog(Log** head, Log* log);
#endif