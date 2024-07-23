/**
 * Implementation of the functions related to hashtables.
 * 
 * Hashtables allow a smart storage of linked lists of logs.
 * 
 * Author: Adolfo Monteiro
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

// Initial value for the hash in the djb2 algorithm
#define DJB2_CONSTANT 5381


/**
 * @brief Calculates the hash value for a license plate.
 * 
 * This function calculates the hash value for the given license plate using
 * the djb2 algorithm.
 * 
 * @param plate Pointer to the license plate string.
 * @param size Size of the hashtable.
 * @return The hash value of the license plate.
 */
unsigned int plateHash(const char* plate, unsigned int size){
    unsigned int hash = DJB2_CONSTANT;
    const char* ptr = plate;

    // djb2 algorithm
    while (*ptr){
        if (*ptr != '-') {
            hash = ((hash << 5) + hash) + *ptr;
        }
        ptr++;
    }

    return hash % size;
}


/**
 * @brief Creates a new hashtable.
 * 
 * This function allocates memory for a new hashtable, initializes its fields,
 * and returns a pointer to it.
 * 
 * @return Pointer to the newly created hashtable.
 */
Hashtable* newHashtable(){
    Hashtable* ht = (Hashtable*)malloc(sizeof(Hashtable));

    // Initialize the hashtable's logs to NULL (0)
    ht->logs = (Log**)malloc(INITIAL_SIZE * sizeof(Log*));
    memset(ht->logs, 0, INITIAL_SIZE * sizeof(Log*));

    ht->size = INITIAL_SIZE;
    ht->numElements = 0;

    return ht;
}


/**
 * @brief Gets the size of the hashtable.
 * 
 * This function returns the size of the given hashtable.
 * 
 * @param ht Pointer to the hashtable.
 * @return The size of the hashtable.
 */
int getSize(const Hashtable* ht){
    return ht->size;
}


/**
 * @brief Gets the log at the specified index in the hashtable.
 * 
 * This function retrieves the head log of the log linked list at the specified
 * index in the hashtable.
 * 
 * @param ht Pointer to the hashtable.
 * @param index The index of the log to retrieve.
 * @return Pointer to the log at the specified index, or NULL if index is
 * out of bounds or ht is NULL.
 */
Log* getLogAtIndex(Hashtable* ht, unsigned int index){
    if (ht == NULL || index >= ht->size){
        return NULL;
    }
    return ht->logs[index];
}


/**
 * @brief Gets the last log associated with the specified plate in the
 * hashtable without an exit.
 * 
 * @param ht Pointer to the hashtable.
 * @param plate The license plate to search for.
 * @return Pointer to the last log associated with the specified license plate
 * without an exit or NULL if none is found.
 */
Log* getPlateLastLogWithoutExit(Hashtable* ht, const char plate[PLATE_LENGTH]){

    // Find the head of the log linked list where the plate's log could be
    Log* currentLog = getLogAtIndex(ht, plateHash(plate, getSize(ht)));

    // Transverse the log linked list
    while (currentLog != NULL){
        // No exit means the exit timestamp is the INITIAL_TIMESTAMP
        if (strcmp(getLogPlate(currentLog), plate) == 0 &&
            isInitialTimestamp(getExitTimestamp(currentLog))){
            return currentLog;
        }
        currentLog = currentLog->next;
    }
    
    return NULL;
}


/**
 * @brief Frees memory allocated for logs in the hashtable.
 * 
 * This function iterates through all logs in the hashtable and frees
 * the memory allocated for them.
 * 
 * @param ht Pointer to the hashtable.
 */
void freeHashtableLogs(Hashtable* ht){
    for (unsigned int i = 0; i < ht->size; i++){
        freeLog(getLogAtIndex(ht, i));
    }
    free(ht->logs);
}


/**
 * @brief Checks if a given number is prime.
 * 
 * This function determines whether the given number is a prime number.
 * 
 * @param num The number to check for primality.
 * @return 1 if the number is prime, 0 otherwise.
 */
int isPrime(unsigned int num){
    // 0 and 1 are not prime
    if (num <= 1) return 0;                  
    // 2 and 3 are prime
    if (num <= 3) return 1;
    // multiples of 2 and 3 are not prime
    if (num % 2 == 0 || num % 3 == 0) return 0;

    // Check divisors of the form 6k ± 1 up to sqrt(n)
    for (unsigned int i = 5; i * i <= num; i += 6){
        if (num % i == 0 || num % (i + 2) == 0) return 0;
    }
    return 1;
}

/**
 * @brief Finds the nearest prime greater or equal than the given number.
 * 
 * 
 * @param n The number for which to find the nearest prime.
 * @return The nearest prime number greater or equal than the given number.
 */
unsigned int nearestPrime(unsigned int n){
    if (n <= 1) return 2;  // Return 2 if n is 0 or 1

    // Make sure n is odd
    if (n % 2 == 0){
        n++;
    }

    while (1){
        if (isPrime(n)) return n;
        n += 2;
    }
}


/**
 * @brief Resizes the hashtable.
 * 
 * This function resizes the given hashtable by doubling its size and
 * reallocating memory for logs, placing them in the new indexes.
 * 
 * @param ht Pointer to the pointer to the hashtable.
 */
void resizeHashtable(Hashtable** ht){
    // Set new size to a prime number close to double the current size
    // A prime number size helps reduce colisions
    unsigned int newSize = nearestPrime((*ht)->size * 2 + 1);

    // Allocate memory for logs, and set them to NULL initially
    Log** newLogs = (Log**)malloc(newSize * sizeof(Log*));
    memset(newLogs, 0, newSize * sizeof(Log*));

    // Place the old logs in the proper new indexes
    for (unsigned int i = 0; i < (*ht)->size; i++){
        Log* log = getLogAtIndex(*ht, i);
        while (log != NULL) {
            unsigned int newIndex = plateHash(getLogPlate(log), newSize);
            Log* nextLog = log->next;
            log->next = newLogs[newIndex];
            newLogs[newIndex] = log;
            log = nextLog;
        }
    }

    free((*ht)->logs); // Free old logs
    (*ht)->logs = newLogs;
    (*ht)->size = newSize;
}


/**
 * @brief Frees memory allocated for the hashtable.
 * 
 * This function frees the memory allocated for the hashtable and all its logs.
 * 
 * @param ht Pointer to the hashtable.
 */
void freeHashtable(Hashtable* ht){
    freeHashtableLogs(ht);
    free(ht);
}


/**
 * @brief Adds a log to the hashtable.
 * 
 * This function adds a log to the hashtable. It calculates the index using
 * the hash function based on the plate, and then appends the log to the end
 * of the linked list at that index.
 * 
 * @param ht Pointer to the hashtable.
 * @param log Pointer to the log to add.
 */
void addLogToTable(Hashtable* ht, Log* log){
    (ht->numElements)++;

    unsigned int index = plateHash(getLogPlate(log), getSize(ht));
    Log* currentLog = getLogAtIndex(ht, index);

    // If there are no logs at the index, make log the head of the linked list
    if (currentLog == NULL){
        ht->logs[index] = log;
        // Resizing is not needed if log was inserted as head of a linked list,
        // as this does not affect search time.
        return;
    }
    
    // Search for the end of the linked list
    while (currentLog->next != NULL){
        currentLog = currentLog->next;
    }
    currentLog->next = log; // Add it to the end

    // Check if the hashtable needs resizing, and if so do it.
    if ((double)ht->numElements / getSize(ht) > LOAD_FACTOR_THRESHOLD){
        resizeHashtable(&ht);
    }
}