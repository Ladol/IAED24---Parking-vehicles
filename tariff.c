/**
 * Implementation of the functions related to tariffs.
 * 
 * Tariffs allow the storage of how much to charge for staying in a park.
 * 
 * Author: Adolfo Monteiro
*/
#include "tariff.h"


/**
 * @brief Creates a new tariff instance.
 * 
 * This function creates a new tariff instance based on the provided values.
 * 
 * @param value15 Pointer to the cost per 15 minutes within the 1st hour.
 * @param value15after1 Pointer to the cost per 15 minutes after the 1st hour.
 * @param valueMaxDaily Pointer to the maximum daily cost.
 * @return The new tariff instance.
 */
Tariff newTariff(const double* value15, const double* value15after1,
const double* valueMaxDaily){
    Tariff theTariff;

    theTariff.value15 = *value15;
    theTariff.value15after1 = *value15after1;
    theTariff.valueMaxDaily = *valueMaxDaily;

    return theTariff;
}


/**
 * @brief Checks if a tariff is valid.
 * 
 * This function checks if the given tariff is valid, i.e., if the cost
 * values satisfy the conditions for a valid tariff:
 * value15 > value15after1 > valueMaxDaily > 0
 * 
 * @param tariff Pointer to the tariff to be validated.
 * @return 1 if the tariff is valid, otherwise 0.
 */
int validTariff(const Tariff* tariff){
    return (
            tariff->value15 < tariff->value15after1 &&
            tariff->value15after1 < tariff->valueMaxDaily &&
            tariff->value15 > 0  // therefore they all are > 0
            ) ? 1 : 0;           
}


/**
 * @brief Calculates the parking cost based on the tariff and timestamps.
 * 
 * This function calculates the parking cost based on the provided tariff
 * and entry/exit timestamps. It considers the duration of the parking and
 * applies the appropriate tariff rates.
 * 
 * @param tariff Pointer to the tariff instance.
 * @param entryTimestamp Pointer to the entry timestamp.
 * @param exitTimestamp Pointer to the exit timestamp.
 * @return The calculated parking cost.
 */
double calculateParkingCost(const Tariff* tariff,
const Timestamp* entryTimestamp, const Timestamp* exitTimestamp){
    int days = 0, quarterHoursFirst = 0, quarterHoursAfterFirst = 0;

    // Find how many minutes the vehicle was in the park
    int minutesDiff = minutesDifference(entryTimestamp, exitTimestamp);

    // Find how many full days to charge
    while (minutesDiff >= (MINUTES_IN_DAY)){
        days++;
        minutesDiff -= (MINUTES_IN_DAY);
    }
    // Find how many quarter hours in the first hour to charge
    while (minutesDiff > 0 && quarterHoursFirst < MAXIMUM_FIRST_HOUR_QUARTERS){
        quarterHoursFirst++;
        minutesDiff -= QUARTER_HOUR_TO_MINUTES;
    }
    // Find how many quarter hours after the first hour to charge
    while (minutesDiff > 0) {
        quarterHoursAfterFirst++;
        minutesDiff -= QUARTER_HOUR_TO_MINUTES;
    }

    double totalQuartersPayment = tariff->value15 * quarterHoursFirst +
                                tariff->value15after1 * quarterHoursAfterFirst;
    // The sum of the value charged through quarter hours must be less than
    // the maximum daily cost
    if (totalQuartersPayment > tariff->valueMaxDaily){
        totalQuartersPayment = tariff->valueMaxDaily;
    }
    return totalQuartersPayment + tariff->valueMaxDaily * days;
}
