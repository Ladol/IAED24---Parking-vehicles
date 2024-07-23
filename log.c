/**
 * Implementation of the functions related to logs.
 * 
 * Logs allow the storage of the entry and exit timestamps of specific
 * plate on a specific park.
 * 
 * Author: Adolfo Monteiro
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"


/**
 * @brief Creates a new log node with the specified plate and park name.
 * 
 * This function allocates memory for a new log node and initializes its
 * attributes.
 * The plate and park name are copied into the log node.
 * 
 * @param plate The license plate associated with the log.
 * @param parkName The name of the park associated with the log.
 * @return A pointer to the newly created log node.
 */
Log *newLog(const char plate[PLATE_LENGTH], char *parkName){
    Log *newLogNode = (Log *)malloc(sizeof(Log));

    strcpy(newLogNode->plate, plate);
    // parkName is copied from Park, no need to allocate memory twice
    newLogNode->parkName = parkName;
    newLogNode->entryTimestamp = INITIAL_TIMESTAMP;
    newLogNode->exitTimestamp = INITIAL_TIMESTAMP;
    newLogNode->next = NULL;

    return newLogNode;
}


/**
 * @brief Copies the entry and exit timestamps from the source log to the
 * destination log.
 * 
 * @param dest Pointer to the destination log where the timestamps
 * will be copied.
 * @param source Pointer to the source log from which the timestamps
 * will be copied.
 */
void copyLogTimestamps(Log* dest, const Log* source){
    copyTimestamp(&(dest->entryTimestamp), &(source->entryTimestamp));
    copyTimestamp(&(dest->exitTimestamp), &(source->exitTimestamp));
}


/**
 * @brief Frees memory allocated for a linked list of log nodes. (all of them!)
 * 
 * This function traverses the linked list starting from the head log node and
 * frees memory allocated for each log node.
 * 
 * @param log The head log node of the linked list.
 */
void freeLog(Log *log){
    Log *nextLog;
    while (log != NULL){
        nextLog = log->next;
        free(log);
        log = nextLog;
    }
}


/**
 * @brief Retrieves the name of the park associated with a log entry.
 * 
 * @param log The log entry.
 * @return The name of the park associated with the log entry.
 */
char *getLogParkName(Log *log){
    return log->parkName;
}


/**
 * @brief Retrieves the license plate associated with a log entry.
 * 
 * @param log The log entry.
 * @return The license plate associated with the log entry.
 */
char* getLogPlate(Log* log){
    return log->plate;
}


/**
 * @brief Retrieves the entry timestamp of a log entry.
 * 
 * @param log The log entry.
 * @return A pointer to the entry timestamp of the log entry.
 */
Timestamp* getEntryTimestamp(Log *log){
    return &(log->entryTimestamp);
}


/**
 * @brief Retrieves the exit timestamp of a log entry.
 * 
 * @param log The log entry.
 * @return A pointer to the exit timestamp of the log entry.
 */
Timestamp* getExitTimestamp(Log *log){
    return &(log->exitTimestamp);
}




/**
 * @brief Finds the last node in a log linked list.
 * 
 * This function traverses the log linked list to find the last node.
 * 
 * @param log The head of the log linked list.
 * @return A pointer to the last node in the log linked list,
 * or NULL if the list is empty.
 */
Log *findLastLog(Log *log){
    for(; log != NULL && log->next != NULL; log = log->next)
        ;
    return log;
}


/**
 * @brief Prints the log entries.
 * 
 * This function prints the log entries, including the park name, entry
 * timestamp and exit timestamp (if available).
 * 
 * @param log The head of the log linked list.
 */
void printLog(Log *log){
    Timestamp* exitTimestamp;
    Timestamp* entryTimestamp;

    while (log != NULL){
        exitTimestamp = getExitTimestamp(log);
        entryTimestamp = getEntryTimestamp(log);

        // Print the entry timestamp
        printf("%s ", getLogParkName(log));
        printTimestamp(entryTimestamp);

        // If an exit exists, print its timestamp
        if (!isInitialTimestamp(exitTimestamp)){
            printf(" ");
            printTimestamp(exitTimestamp);
        }
        printf("\n");
        log = log->next;
    }
}


/**
 * @brief Prints the daily bill for each entry in the log.
 * 
 * This function prints the daily bill for each entry in the log linked list.
 * It calculates the parking cost based on the given tariff and prints the
 * plate, exit time, and corresponding cost.
 * 
 * @param log The head of the log linked list.
 * @param tariff Pointer to the tariff structure containing parking rates.
 */
void printDailyBillLog(Log *log, Tariff *tariff){
    while (log != NULL){
        Timestamp* exitTimestamp = getExitTimestamp(log);

        printPlate(getLogPlate(log));
        printf(" ");
        printHourMinutes(exitTimestamp);
        printf(" ");
        printf("%.2f\n",
        calculateParkingCost(tariff, getEntryTimestamp(log), exitTimestamp));
        log = log->next;
    }
}


/**
 * @brief Prints the full bill for each day in the log.
 * 
 * This function prints the full bill for each day in the log linked list.
 * It calculates the total parking cost for each day based on the given tariff
 * and prints the date and corresponding total cost.
 * 
 * @param log The head of the log linked list.
 * @param tariff Pointer to the tariff structure containing parking rates.
 */
void printFullBillLog(Log *log, Tariff *tariff){
    double bill = 0;
    Timestamp* currentTimestamp;
    Timestamp* exitTimestamp;
    int printLastBill = 0; // Used to know if there were any bills

    if (log != NULL){
        printLastBill = 1;
        exitTimestamp = getExitTimestamp(log);
        currentTimestamp = getExitTimestamp(log);
    }

    while (log != NULL){
        exitTimestamp = getExitTimestamp(log);

        // Check if the exit date has changed
        if (compareDate(currentTimestamp, exitTimestamp) != 0){
            // If it did, print the bill for the previous day
            printDate(currentTimestamp);
            printf(" %.2f\n", bill);
            copyTimestamp(currentTimestamp, exitTimestamp);
            bill = 0;
        }

        // Build up the bill for the current exit date
        bill += calculateParkingCost(tariff, getEntryTimestamp(log),
                                        exitTimestamp);

        log = log->next;
    }

    // Print last bill, which can't be printed inside while loop
    if (printLastBill){
        printDate(currentTimestamp);
        printf(" %.2f\n", bill);
    }
}


/**
 * @brief Merges two sorted linked lists of logs based on a specified sorting
 * criteria.
 * 
 * This function merges two sorted linked lists of logs into a single sorted
 * linked list.
 * The sorting criteria can be either by entry timestamp('e') or exit timestamp
 * 
 * @param list1 The head of the first sorted linked list.
 * @param list2 The head of the second sorted linked list.
 * @param sortBy Sorting criteria: 'e' sorts first by the log's park name and 
 * then by the log's entry timestamp, any other char sorts first by the log's
 * exit timestamp and then by the log's park name.
 * @return Log* The head of the merged sorted linked list.
 */
Log *merge(Log *list1, Log *list2, const char sortBy){
    if (list1 == NULL)
        return list2;
    if (list2 == NULL)
        return list1;

    int parkNameCmp = strcmp(getLogParkName(list1), getLogParkName(list2));

    int timestampComparison;
    if (sortBy == 'e'){
        // Sort by entry timestamp
        timestampComparison = (compareTimestamps(getEntryTimestamp(list1),
                                getEntryTimestamp(list2)) == -1);
    }
    else{
        // Sort by exit timestamp
        timestampComparison = (compareTimestamps(getExitTimestamp(list1),
                                getExitTimestamp(list2)) == -1);
    }

    // Sort by park name first and then entry timestamp for sortBy == 'e' or
    // Sort by exit timestamp and then by park name for any other sortBy
    if (parkNameCmp < 0 || (parkNameCmp == 0 && timestampComparison)){
        list1->next = merge(list1->next, list2, sortBy);
        return list1;
    }
    else{
        list2->next = merge(list1, list2->next, sortBy);
        return list2;
    }
}


/**
 * @brief Splits a linked list into two halves.
 * 
 * This function splits a given linked list into two halves using the slow and
 * fast pointer technique.
 * 
 * @param head The head of the linked list to be split.
 * @param firstHalf Pointer to store the head of the 1st half after the split.
 * @param secondHalf Pointer to store the head of the 2nd half after the split.
 */
void split(Log *head, Log **firstHalf, Log **secondHalf){
    Log *fast = head->next;
    Log *slow = head;

    while (fast != NULL){
        fast = fast->next;
        if (fast != NULL){
            slow = slow->next;
            fast = fast->next;
        }
    }

    *firstHalf = head;
    *secondHalf = slow->next;
    slow->next = NULL;
}


/**
 * @brief Sorts a linked list of logs using merge sort algorithm.
 * 
 * This function sorts a linked list of logs based on a specified sorting
 * criteria.
 * The sorting criteria can be either by entry timestamp ('e') or exit
 * timestamp (any other char).
 * 
 * @param head Pointer to the head of the linked list to be sorted.
 * @param sortBy Sorting criteria: 'e' for entry timestamp, any other char
 * for exit timestamp.
 */
void mergeSort(Log **head, const char sortBy){
    Log *tempHead = *head;
    Log *firstHalf;
    Log *secondHalf;

    if (tempHead == NULL || tempHead->next == NULL){
        return;
    }

    split(tempHead, &firstHalf, &secondHalf);

    mergeSort(&firstHalf, sortBy);
    mergeSort(&secondHalf, sortBy);

    *head = merge(firstHalf, secondHalf, sortBy);
}


/**
 * @brief Adds a new log to the end of a linked list of logs.
 * 
 * This function adds a new log node to the end of a linked list of logs.
 * If the linked list is empty, the new log becomes the head of the list.
 * !! Assumes log was initialized with newlog() !!
 * 
 * @param head Pointer to the pointer to the head of the linked list.
 * @param log Pointer to the log to be added.
 */
void addLogtoLog(Log **head, Log *log) {
    if (*head == NULL) {
        *head = log;
        return;
    }
    findLastLog(*head)->next = log;
}
