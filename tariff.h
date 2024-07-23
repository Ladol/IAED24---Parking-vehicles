/**
 * Definition of the Tariff struct, and of the related function prototypes.
 * 
 * Tariffs allow the storage of how much to charge for staying in a park.
 * 
 * Author: Adolfo Monteiro
*/
#ifndef TARIFF_H
#define TARIFF_H

#include "timestamp.h"

// Maximum number of 15 minute periods to charge in the first hour
#define MAXIMUM_FIRST_HOUR_QUARTERS 4
#define QUARTER_HOUR_TO_MINUTES 15

typedef struct tariff{
    // Value per 15 minutes in the 1st hour
    double value15;
    // Value per 15 minutes after the 1st hour
    double value15after1;
    // Maximum value per day (24hours)
    double valueMaxDaily;
} Tariff;

// Initializer
Tariff newTariff(const double* value15, const double* value15after1,
const double* valueMaxDaily);

// Validation
int validTariff(const Tariff* tariff);

// Calculations
double calculateParkingCost(const Tariff* tariff,
        const Timestamp* entryTimestamp, const Timestamp* exitTimestamp);
#endif