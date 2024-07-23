/**
 * Implementation of the functions related to timestamps.
 * 
 * Timestamps allow the storage of a date (day, month, year) aswell as
 * hours and minutes.
 * 
 * Author: Adolfo Monteiro
*/
#include <stdio.h>
#include "timestamp.h"

// The number of days in each month of the year, february always has 28
const int DAYS_IN_MONTH[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};


/**
 * @brief Creates a new timestamp with the given date and time components.
 * 
 * This function creates a new timestamp with the specified day, month, year,
 * hour, and minute components and returns it.
 * 
 * @param d Day component of the timestamp.
 * @param m Month component of the timestamp.
 * @param y Year component of the timestamp.
 * @param h Hour component of the timestamp.
 * @param min Minute component of the timestamp.
 * @return The newly created timestamp.
 */
Timestamp newTimestamp(int d, int m, int y, int h, int min){
    Timestamp timestamp = {d, m, y, h, min};
    return timestamp;
}


/**
 * @brief Copies the values of one timestamp to another.
 * 
 * This function copies the values of the source timestamp to the destination
 * timestamp.
 * 
 * @param dest Pointer to the destination timestamp.
 * @param source Pointer to the source timestamp.
 */
void copyTimestamp(Timestamp *dest, const Timestamp *source){
    dest->day = source->day;
    dest->month = source->month;
    dest->year = source->year;
    dest->hour = source->hour;
    dest->minute = source->minute;
}


/**
 * @brief Compares two timestamps based on their dates.
 * 
 * This function compares two timestamps based on their date components
 * (year, month, and day) and returns:
 * - 1 if t1 is later than t2,
 * - 0 if t1 and t2 are equal,
 * - -1 if t1 is earlier than t2.
 * 
 * @param t1 Pointer to the first timestamp.
 * @param t2 Pointer to the second timestamp.
 * @return An integer representing the comparison result.
 */
int compareDate(const Timestamp *t1, const Timestamp *t2){
    if (t1->year != t2->year)
        return (t1->year < t2->year) ? -1 : 1;
    if (t1->month != t2->month)
        return (t1->month < t2->month) ? -1 : 1;
    if (t1->day != t2->day)
        return (t1->day < t2->day) ? -1 : 1;
    return 0;
}


/**
 * @brief Compares two timestamps.
 * 
 * This function compares two timestamps, considering both date and time
 * components, and returns:
 * - 1 if t1 is later than t2,
 * - 0 if t1 and t2 are equal,
 * - -1 if t1 is earlier than t2.
 * 
 * @param t1 Pointer to the first timestamp.
 * @param t2 Pointer to the second timestamp.
 * @return An integer representing the comparison result.
 */
int compareTimestamps(const Timestamp *t1, const Timestamp *t2){
    // First compare by date (day, month, year)
    int dateComparison = compareDate(t1, t2);
    if (dateComparison != 0)
        return dateComparison;


    // If dates are equal compare by hours and minutes
    if (t1->hour != t2->hour)
        return (t1->hour < t2->hour) ? -1 : 1;
    if (t1->minute != t2->minute)
        return (t1->minute < t2->minute) ? -1 : 1;
    return 0;
}


/**
 * @brief Checks if a timestamp is the initial (default) timestamp.
 * 
 * This function checks if a given timestamp is equal to the initial
 * timestamp constant defined as INITIAL_TIMESTAMP.
 * 
 * @param t Pointer to the timestamp to be checked.
 * @return 1 if the timestamp is the initial timestamp, otherwise 0.
 */
int isInitialTimestamp(const Timestamp *t){
    return compareTimestamps(t, &INITIAL_TIMESTAMP) == 0;
}

/**
 * @brief Validates a timestamp.
 * 
 * This function validates a timestamp by checking if its components
 * (day, month, year, hour, and minute) fall within valid ranges.
 * 
 *  ! day 29 in february is always considered invalid !
 * 
 * @param t Pointer to the timestamp to be validated.
 * @return 1 if the timestamp is valid, otherwise 0.
 */
int validTimestamp(const Timestamp* t){
    // assume valid year
    if (t->month < 1 || t->month > 12)
        return 0;

    int daysInMonth = DAYS_IN_MONTH[t->month - 1];

    // IMPORTANT: day 29 in february is always invalid!
    if (t->day < 1 || t->day > daysInMonth)
        return 0;
    if (t->hour < 0 || t->hour > 23)
        return 0;
    if (t->minute < 0 || t->minute > 59)
        return 0;

    return 1;
}


/**
 * @brief Prints the hour and minute components of a timestamp.
 * 
 * This function prints the hour and minute components of a timestamp
 * in the format "HH:MM".
 * 
 * @param t Pointer to the timestamp to be printed.
 */
void printHourMinutes(const Timestamp *t){
    printf("%02d:%02d", t->hour, t->minute);
}


/**
 * @brief Prints the date components of a timestamp.
 * 
 * This function prints the date components of a timestamp in the
 * format "DD-MM-YYYY".
 * 
 * @param t Pointer to the timestamp to be printed.
 */
void printDate(const Timestamp *t){
    printf("%02d-%02d-%d", t->day, t->month, t->year);
}


/**
 * @brief Prints the full timestamp.
 * 
 * This function prints the full timestamp, including both date and time
 * components, in the format "DD-MM-YYYY HH:MM".
 * 
 * @param t Pointer to the timestamp to be printed.
 */
void printTimestamp(const Timestamp *t){
    // no newline
    printDate(t);
    printf(" ");
    printHourMinutes(t);
}


/**
 * @brief Converts a timestamp to minutes.
 * 
 * This function calculates the total number of minutes represented
 * by the given timestamp, considering the date and time components.
 * 
 * @param t Pointer to the timestamp to be converted.
 * @return The total number of minutes represented by the timestamp.
 */
int timestampToMinutes(const Timestamp *t){
    int days = (t->year - 1) * DAYS_IN_YEAR + t->day - 1;
    for (int m = 1; m < t->month; m++){
        days += DAYS_IN_MONTH[m - 1];
    }
    return days * MINUTES_IN_DAY + t->hour * MINUTES_IN_HOUR + t->minute;
}


/**
 * @brief Calculates the difference in minutes between two timestamps.
 * 
 * This function calculates the difference in minutes between two timestamps.
 * 
 * @param t1 Pointer to the first timestamp.
 * @param t2 Pointer to the second timestamp.
 * @return The difference in minutes between t2 and t1.
 */
int minutesDifference(const Timestamp *t1, const Timestamp *t2){
    return timestampToMinutes(t2) - timestampToMinutes(t1);
}