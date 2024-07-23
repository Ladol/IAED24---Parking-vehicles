/**
 * Definition of the timestamp struct, and of the related function prototypes.
 * 
 * Timestamps allow the storage of a day, month, year, hours and minutes.
 * 
 * Author: Adolfo Monteiro
*/
#ifndef TIMESTAMP_H
#define TIMESTAMP_H

// Default value for a timestamp when unsure of what value to specify
#define INITIAL_TIMESTAMP ((Timestamp){0, 0, 0, 0, 0})
#define DAYS_IN_YEAR 365
#define MINUTES_IN_DAY 24*60
#define MINUTES_IN_HOUR 60


typedef struct timestamp{
    int day, month, year, hour, minute;
} Timestamp;


// Initializers
Timestamp newTimestamp(int d, int m, int y, int hh, int mm);
void copyTimestamp(Timestamp* dest, const Timestamp* source);

// Compare functions
int compareDate(const Timestamp* t1, const Timestamp* t2);
int compareTimestamps(const Timestamp* t1, const Timestamp* t2);
int isInitialTimestamp(const Timestamp* t);

// Validation
int validTimestamp(const Timestamp* t);
void printHourMinutes(const Timestamp* t);

// Prints
void printDate(const Timestamp* t);
void printTimestamp(const Timestamp* t);

// Convertions to minutes
int timestampToMinutes(const Timestamp* t);
int minutesDifference(const Timestamp* t1, const Timestamp* t2);
#endif