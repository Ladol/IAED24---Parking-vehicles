/**
 * Implementation of the functions related to parks, such as:
 * Creating a new park, registering vehicle entries/exits in a park,
 * removing a park from the system, displaying a park's billings,
 * checking if a plate is inside a park, among others.
 * 
 * Author: Adolfo Monteiro
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "park.h"

// Maximum number of parks in the system
#define MAX_PARKS 20


/**
 * @brief Creates a new park node.
 * 
 * This function allocates memory for a new park node and initializes
 * its attributes.
 * 
 * @param name Name of the park.
 * @param capacity Pointer to the capacity of the park.
 * @param tariff Pointer to the tariff of the park.
 * @return Pointer to the newly created park node.
 */
Park *newPark(char *name, const int* capacity, const Tariff* tariff){
    Park *newParkNode = (Park *)malloc(sizeof(Park));

    // Memory allocation for name was done previously
    newParkNode->name = name;
    newParkNode->capacity = *capacity;
    newParkNode->availableSlots = *capacity;
    newParkNode->tariff = *tariff;

    newParkNode->logTable = newHashtable();
    newParkNode->next = NULL;

    return newParkNode;
}


/**
 * @brief Frees memory allocated for a park node.
 * 
 * This function frees the memory allocated for a park node,
 * including its name and log hashtable.
 * 
 * @param park Pointer to the park node to free.
 */
void freePark(Park *park){
    free(park->name);
    freeHashtable(park->logTable);
    free(park);
}


/**
 * @brief Frees memory allocated for all park nodes in the linked list.
 * 
 * This function frees the memory allocated for all park nodes in the linked
 * list, including their names and log tables.
 * 
 * @param headPark Pointer to the head of the park linked list.
 */
void freeAllParks(Park *headPark){
    while (headPark != NULL){
        Park *nextPark = headPark->next;
        freePark(headPark);
        headPark = nextPark;
    }
}


/**
 * @brief Retrieves the name of a park.
 * 
 * @param park Pointer to the park.
 * @return Pointer to the name of the park.
 */
char* getParkName(Park* park){
    return park->name;
}


/**
 * @brief Retrieves the number of available parking spots in a park.
 * 
 * @param park Pointer to the park.
 * @return Pointer to the number of available parking spots.
 */
int* getAvailableSpots(Park *park){
    return &(park->availableSlots);
}


/**
 * @brief Retrieves the capacity of a park.
 * 
 * @param park Pointer to the park.
 * @return Pointer to the capacity of the park.
 */
int* getCapacity(Park* park){
    return &(park->capacity);
}


/**
 * @brief Retrieves the tariff of a park.
 * 
 * @param park Pointer to the park.
 * @return Pointer to the tariff of the park.
 */
Tariff* getTariff(Park* park){
    return &(park->tariff);
}


/**
 * @brief Retrieves the log hashtable of a park.
 * 
 * @param park Pointer to the park.
 * @return Pointer to the log hashtable of the park.
 */
Hashtable* getTable(const Park* park){
    return park->logTable;
}


/**
 * @brief Calculates the total number of parks in the linked list.
 * 
 * @param headPark Pointer to the head of the park linked list.
 * @return Total number of parks in the linked list.
 */
int totalParks(Park *headPark){
    int count = 0;

    while (headPark != NULL){
        count++;
        headPark = headPark->next;
    }

    return count;
}


/**
 * @brief Finds the last park in the linked list of parks.
 * 
 * @param headPark Pointer to the head of the park linked list.
 * @return Pointer to the last park in the linked list.
 */
Park *findLastPark(Park *headPark){
    for(;headPark != NULL && headPark->next != NULL; headPark = headPark->next)
        ;
    return headPark;
}


/**
 * @brief Retrieves a park with the specified name from the linked list
 * of parks.
 * 
 * @param headPark Pointer to the head of the park linked list.
 * @param parkName Name of the park to retrieve.
 * @return Pointer to the park with the specified name if found, otherwise NULL
 */
Park *getPark(Park *headPark, const char *parkName){
    // Transverse the linked list, looking for the park with name parkName
    while (headPark != NULL){
        if (strcmp(parkName, getParkName(headPark)) == 0){
            return headPark;
        }
        headPark = headPark->next;
    }

    return NULL;
}


/**
 * @brief Retrieves all logs associated with a specific plate across all parks.
 * 
 * @param headPark Pointer to the head of the park linked list.
 * @param plate The license plate to retrieve logs for.
 * @return Pointer to the linked list of logs associated with the given plate.
 *         The logs are sorted first by park name and then by entry timestamp,
 * both in ascending order.
 */
Log *getPlateLogs(Park *headPark, const char plate[PLATE_LENGTH]){
    Log *plateLogs = NULL;

    // Transverse the parks
    while (headPark != NULL){
        // Get the logs stored at the same hashtable's index as plate
        Log* currentLog = getLogAtIndex(getTable(headPark),
            plateHash(plate, getSize(getTable(headPark))));

        // Transverse the logs, looking for log's with the correct plate
        while (currentLog != NULL) {
            if (strcmp(getLogPlate(currentLog), plate) == 0) {
                // If the currentLog's plate is the plate we're looking for,
                // add it to plateLogs
                Log *newLogAux = newLog(plate, getParkName(headPark));
                copyLogTimestamps(newLogAux, currentLog);
                addLogtoLog(&plateLogs, newLogAux);
            }
            currentLog = currentLog->next;
        }
        headPark = headPark->next;
    }

    mergeSort(&plateLogs, 'e');
    return plateLogs; 
}


/**
 * @brief Removes a park from the linked list of parks.
 * 
 * @param headPark Pointer to the pointer to the head of the park linked list.
 * @param parkName Name of the park to be removed.
 * @return 1 if the park is successfully removed, 0 otherwise.
 */
int removePark(Park **headPark, const char *parkName){
    // Park to remove doesn't exist
    if (getPark(*headPark, parkName) == NULL){
        printf("%s: no such parking.\n", parkName);
        return 0;
    }

    Park *nextPark;
    // Park to remove is the first park (headPark)
    if (strcmp(getParkName(*headPark), parkName) == 0){
        nextPark = (*headPark)->next;
        freePark(*headPark);
        *headPark = nextPark;
        return 1;
    }

    Park *currentPark = *headPark;
    // Park to remove is after the first park
    while (currentPark->next != NULL){
        if (strcmp(getParkName(currentPark->next), parkName) == 0){
            nextPark = currentPark->next->next;
            freePark(currentPark->next);
            currentPark->next = nextPark;
            return 1;
        }
        currentPark = currentPark->next;
    }

    return 0;
}


/**
 * @brief Adds a new park to the end of the linked list of parks.
 * 
 * Verifies if the park is valid and if the maximum amount of parks hasn't
 * been reached, and if so adds the park to the end of the linked list of parks
 * 
 * @param headPark Pointer to the pointer to the head of the park linked list.
 * @param park Pointer to the park to be added.
 * @return 1 if the park is successfully added, 0 otherwise.
 */
int addPark(Park **headPark, Park *park){
    if (getPark(*headPark, park->name) != NULL){
        printf("%s: parking already exists.\n", park->name);
        return 0;
    }
    
    int isCapacityValid = (*getCapacity(park) > 0);
    int isTariffValid = validTariff(getTariff(park));
    int isMaxParksReached = totalParks(*headPark) >= MAX_PARKS;

    // If the park is not valid or if the maximum number of parks has been
    // reached, display the appropriate error message
    if (!isCapacityValid || !isTariffValid || isMaxParksReached){
        if (!isCapacityValid){
            printf("%d: invalid capacity.\n", *getCapacity(park));
        }
        else if (!isTariffValid) printf("invalid cost.\n");
        else if (isMaxParksReached) printf("too many parks.\n");
        return 0;
    }

    // If there are no parks in the linked list add it as the head
    if (*headPark == NULL){
        *headPark = park;
    }
    else{ // Otherwise add it in the end of the list
        findLastPark(*headPark)->next = park;
    }

    return 1;
}


/**
 * @brief Prints information about all parks in the linked list of parks.
 * 
 * Prints each park's name, capacity and available spots, by order of creation
 * of the parks.
 * 
 * @param headPark Pointer to the head of the park linked list.
 */
void printParks(Park *headPark){
    while (headPark != NULL){
        printf("%s %d %d\n", getParkName(headPark),
            *getCapacity(headPark), *getAvailableSpots(headPark));
        headPark = headPark->next;
    }
}


/**
 * @brief Prints the names of all parks in the linked list of parks in
 * alphabetical order.
 * 
 * Uses bubble sort because the list is always small (20 or less parks)
 * 
 * @param headPark Pointer to the head of the park linked list.
 */
void printParksAlphabetically(Park *headPark){
    // Count the number of parks
    int count = totalParks(headPark);
    Park *aux = headPark;

    // Allocate memory for an array to store park names
    char **parkNames = (char **)malloc(count * sizeof(char *));

    // Copy park names into the array
    int i = 0;
    while (aux != NULL){
        parkNames[i] = getParkName(aux);
        i++;
        aux = aux->next;
    }

    // Sort the park names array alphabetically using bubble sort
    for (i = 0; i < count - 1; i++){
        for (int j = 0; j < count - i - 1; j++){
            if (strcmp(parkNames[j], parkNames[j + 1]) > 0) {
                // Swap park names if they are out of order
                char *temp = parkNames[j];
                parkNames[j] = parkNames[j + 1];
                parkNames[j + 1] = temp;
            }
        }
    }

    // Print the sorted park names
    for (i = 0; i < count; i++){
        printf("%s\n", parkNames[i]);
    }

    // Free the memory allocated for the park names array
    free(parkNames);
}


/**
 * @brief Checks if a given plate is present in a specific park.
 * 
 * @param park Pointer to the park to check.
 * @param plate The license plate to search for.
 * @return 1 if the plate is currently inside the park, 0 otherwise
 */
int plateInPark(const Park *park, const char plate[PLATE_LENGTH]){
    // It's in the park if it has an entry (therefore has a log) but
    // no corresponding exit
    return getPlateLastLogWithoutExit(getTable(park), plate) != NULL;
}


/**
 * @brief Checks if a given plate is inside any park of the linked list.
 * 
 * @param headPark Pointer to the head of the park linked list.
 * @param plate The license plate to search for.
 * @return 1 if the plate is currently inside any park, 0 otherwise
 */
int plateInAnyPark(Park *headPark, const char plate[PLATE_LENGTH]){
    // It is in any park if it is in one of the parks
    while (headPark != NULL){
        if (plateInPark(headPark, plate)){
            return 1;
        }
        headPark = headPark->next;
    }
    return 0;
}


/**
 * @brief Registers the entry or exit of a vehicle with the given plate
 * in the specified park.
 * 
 * @param park Pointer to the park where the entry or exit is being registered.
 * @param plate The license plate of the vehicle.
 * @param timestamp Pointer to the timestamp of the entry or exit.
 */
void registerEntryExit(Park *park, const char plate[PLATE_LENGTH],
const Timestamp* timestamp){
    int* availableSpots = getAvailableSpots(park);

    if (plateInPark(park, plate)){
        // If the plate is already in the park, we're adding it's exit
        (*availableSpots)++;
        // Find the plate's latest entry log in the park
        Log* plateLastLog = getPlateLastLogWithoutExit(getTable(park), plate);

        // Set the plate's latest log's exit to the given timestamp
        Timestamp* exitTimestamp = getExitTimestamp(plateLastLog);
        copyTimestamp(exitTimestamp, timestamp);

        // Display the exit message
        printPlate(plate);
        printf(" ");
        printTimestamp(getEntryTimestamp(plateLastLog));
        printf(" ");
        printTimestamp(exitTimestamp);
        printf(" %.2f\n", calculateParkingCost(getTariff(park),
                            getEntryTimestamp(plateLastLog), exitTimestamp));
        return;
    }

    // We're adding an entry
    (*availableSpots)--;
    Log *newLogEntry = newLog(plate, getParkName(park));
    copyTimestamp(getEntryTimestamp(newLogEntry), timestamp);
    addLogToTable(getTable(park), newLogEntry);
    printf("%s %d\n", getParkName(park), *availableSpots);
}


/**
 * @brief Displays the billing information for the park based on the
 * given timestamp.
 * 
 * @param park Pointer to the park for which to display billing information.
 * @param timestamp Pointer to the timestamp used to filter billing information
 * 
 * If the timestamp is the INITIAL_TIMESTAMP it means that no date was
 * specified on the 'f' command and therefore displays all the daily park
 * billing since creation. Otherwise, if a timestamp was specified, displays
 * the park billing for the specified day in the timestamp (i.e. billing for
 * each exit in that day).
 */
void showParkBilling(Park *park, const Timestamp* timestamp){
    Log *currentLog = NULL;
    Log *exitLog = NULL; // head of the linked list to store exit logs
    Log *newLogEntry = NULL; // helper to build the linked list
    Timestamp* exitTimestamp;

    Hashtable* ht = getTable(park);
    unsigned int tableSize = getSize(ht);

    for (unsigned int i = 0; i < tableSize; i++){
        currentLog = getLogAtIndex(ht, i);
        // Transverse the park logs (entries & exits)
        while (currentLog != NULL) {
            exitTimestamp = getExitTimestamp(currentLog);
            // Verify if the log is of interest to be displayed
            if (!isInitialTimestamp(exitTimestamp) && // Must be an exit
                // If the input timestamp is INITIAL_TIMESTAMP we retrieve all
                // logs with an exit
                (isInitialTimestamp(timestamp) ||
                // Otherwise, retrieve the logs with the specified exit date
                    compareDate(exitTimestamp, timestamp) == 0)){
                // Build up a Log linked list with the exit logs of interest        
                newLogEntry = newLog(getLogPlate(currentLog), getParkName(park));
                copyLogTimestamps(newLogEntry, currentLog);
                addLogtoLog(&exitLog, newLogEntry);
            }
            currentLog = currentLog->next;
        }
    }

    // Sort the logs according to the exit timestamp
    mergeSort(&exitLog, 's');

    Tariff* parkTariff = getTariff(park);
    if (!isInitialTimestamp(timestamp)) {
        // Print bills for the specified day
        printDailyBillLog(exitLog, parkTariff);
    } else {
        // No timestamp was specified: print daily bills since park creation
        printFullBillLog(exitLog, parkTariff);
    }

    freeLog(exitLog);
}