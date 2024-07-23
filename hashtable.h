/**
 * Definition of the hashtable struct, and of the function prototypes
 * related to hashtables.
 * 
 * Hashtables allow a smart storage of linked lists of logs.
 * 
 * Author: Adolfo Monteiro
*/
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "log.h"

// Initial size of the hashtable
#define INITIAL_SIZE 53
// Load factor at which to resize the hashtable
#define LOAD_FACTOR_THRESHOLD 0.75


typedef struct Hashtable {
    Log** logs;
    unsigned int size;
    unsigned int numElements;
} Hashtable;


// Hashing function
unsigned int plateHash(const char* plate, unsigned int size);

// Initializer
Hashtable* newHashtable();

// Getters
int getSize(const Hashtable* ht);
Log* getLogAtIndex(Hashtable* ht, unsigned int index);
Log* getPlateLastLogWithoutExit(Hashtable* ht, const char plate[PLATE_LENGTH]);

// Resizing
void resizeHashtable(Hashtable** ht);

// Freeing
void freeHashtable(Hashtable* Hashtable);

void addLogToTable(Hashtable* Hashtable, Log* log);
#endif