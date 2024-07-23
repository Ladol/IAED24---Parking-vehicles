/**
 * Main file for the park management system project.
 * 
 * This file allows the reading and processing of user input.
 * 
 * This project allows the creation and management of a parking system.
 * Vehicles may enter and exit the parks. Vehicles pay for staying in a park,
 * the billing information can be processed and displayed.
 * 
 * Author: Adolfo Monteiro
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "park.h"

void command_p(char entry_data[BUFSIZ]);

int valid_inputs_commands_e_s(const char command, const char* parkName,
    const char plate[PLATE_LENGTH], const Timestamp* timestamp);

void commands_e_s(char entry_data[BUFSIZ]);
void command_v(char entry_data[BUFSIZ]);
void command_f(char entry_data[BUFSIZ]);
void command_r(char entry_data[BUFSIZ]);

// headPark stores a pointer to the first park in a parks linked list
Park* headPark = NULL;
// Timestamp for the last entry/exit in a parking
Timestamp lastTimestamp;


/**
 * @brief Main function to process user commands.
 * 
 * Reads user input from stdin and executes corresponding commands until
 * termination command is received.
 * 
 * @return 0 on successful completion.
 */
int main(){
    char entry_data[BUFSIZ]; // Stores the user input
    lastTimestamp = INITIAL_TIMESTAMP;

    // Main loop to get a full line of input, and process it
    while (fgets(entry_data, sizeof(entry_data), stdin) != NULL){
        // First character of the input determines the command
        switch (entry_data[0]){
            case 'q': // quit
                freeAllParks(headPark);
                return 0;
            case 'p': // Show parks or create a new one
                command_p(entry_data);
                break;
            case 'e': // Vehicle entry in a park
            case 's': // Vehicle exit out of a park
                commands_e_s(entry_data);
                break;
            case 'v': // List the park usage by a certain vehicle
                command_v(entry_data);
                break;
            case 'f': // List a parks billing
                command_f(entry_data);
                break;
            case 'r': // Remove a park from the system
                command_r(entry_data);
        }
    }

    return 0;
}


/**
 * @brief Processes the command to add a new park.
 * 
 * This function parses the input command and extracts the necessary
 * information to either display the current parks or to create a new park.
 * 
 * @param entry_data The input command string, possibly containing information
 * about the new park.
 */
void command_p(char entry_data[BUFSIZ]){
    char* parkName;
    int capacity;
    double value15, value15after1, valueMaxDaily;

    // Use sscanf to parse the entry data
    // First try to match the park name if it is between quotes
    int result = sscanf(entry_data, "p \"%m[^\"]\" %d%lf%lf%lf",
        &parkName, &capacity, &value15, &value15after1, &valueMaxDaily);
    if (result != 5){
        // The park name is not between quotes
        result = sscanf(entry_data, "p%ms%d%lf%lf%lf",
            &parkName, &capacity, &value15, &value15after1, &valueMaxDaily);
    }

    // If sscanf didn't read 5 values that means we just display the parks
    if (result != 5){
        free(parkName);  // just in case
        printParks(headPark);
        return;
    }

    // The command is to create a new park
    Tariff tariff = newTariff(&value15, &value15after1, &valueMaxDaily);
    Park* park = newPark(parkName, &capacity, &tariff);

    // Add the park to the list
    if (!addPark(&headPark, park)){
        // If the adding was unsuccessful free allocated memory
        freePark(park);
    }
}


/**
 * @brief Validates the inputs for commands 'e' and 's'.
 * 
 * This function checks whether the inputs provided for the entry ('e')
 * or exit ('s') commands are valid.
 * It verifies if the specified park exists, if the parking is not full, if the
 * license plate is valid, if the vehicle is not already parked (for entry),
 * if the vehicle is parked (for exit), and if the timestamp is valid.
 * 
 * @param command The command character ('e' for entry, 's' for exit).
 * @param parkName The name of the park.
 * @param plate The license plate of the vehicle.
 * @param timestamp Pointer to the timestamp indicating the entry or exit time.
 * @return int Returns 1 if all inputs are valid, otherwise returns 0.
 */
int valid_inputs_commands_e_s(const char command, const char* parkName,
const char plate[PLATE_LENGTH], const Timestamp* timestamp){
    Park* park = getPark(headPark, parkName);
    // Verify if park exists
    if(park == NULL){
        printf("%s: no such parking.\n", parkName);
        return 0;
    }

    // In case of an entry, verifiy if the park isn't full
    if(command == 'e' && *getAvailableSpots(park) <= 0){
        printf("%s: parking is full.\n", parkName);
        return 0;
    }

    // Verify if the plate is valid
    if(!validPlate(plate)){
        printf("%s: invalid licence plate.\n", plate);
        return 0;
    }

    int plateIsInAnyPark = plateInAnyPark(headPark, plate);
    // Verify that the vehicle isn't already in a park in case of an entry
    // Or that it isn't already outside all parks in case of an exit
    if ((command == 'e' && plateIsInAnyPark) ||
    (command == 's' && !plateIsInAnyPark)){
        printf("%s: invalid vehicle %s.\n",
            plate, (command == 'e') ? "entry" : "exit");
        return 0;
    }

    // Verify that the timestamp is valid and not before lastTimestamp
    if (!validTimestamp(timestamp) ||
    compareTimestamps(&lastTimestamp, timestamp) == 1){
        printf("invalid date.\n");
        return 0;
    }

    return 1;
}


/**
 * @brief Processes the entry ('e') and exit ('s') commands.
 * 
 * This function parses the input command to process vehicle entries and exits.
 * It checks the validity of the inputs and and if they're valid it registers
 * the entry or exit of a vehicle in a park and updating the last timestamp for
 * these events (entry/exit).
 * 
 * @param entry_data The input command string containing information about
 * the entry or exit event.
 */
void commands_e_s(char entry_data[BUFSIZ]){
    char command;
    char* parkName;
    char plate[PLATE_LENGTH];
    int day, month, year, hour, minute;

    // Use sscanf to parse the entry data
    // First try to match the park name if it is between quotes
    int result = sscanf(entry_data, "%c \"%m[^\"]\" %8s %d-%d-%d %d:%d",
        &command, &parkName, plate, &day, &month, &year, &hour, &minute);

    if (result != 8){ // The park name is not between quotes
        result = sscanf(entry_data, "%c %ms %8s %d-%d-%d %d:%d",
            &command, &parkName, plate, &day, &month, &year, &hour, &minute);
    }


    Timestamp timestamp = newTimestamp(day, month, year, hour, minute);

    // Validate inputs
    if(!valid_inputs_commands_e_s(command, parkName, plate, &timestamp)){
        free(parkName);
        return;
    }

    Park* park = getPark(headPark, parkName);
    registerEntryExit(park, plate, &timestamp);

    // Update last entry/exit timestamp to match the parsed timestamp
    copyTimestamp(&lastTimestamp, &timestamp);

    free(parkName);
}


/**
 * @brief Processes the command to view vehicle logs.
 * 
 * This function parses the input command to view logs associated with a plate.
 * It extracts the plate from the input, validates it, retrieves the logs for
 * the specified plate from all parks, and prints the logs if available.
 * 
 * @param entry_data The input command string containing the plate.
 */
void command_v(char entry_data[BUFSIZ]){
    char plate[PLATE_LENGTH];

    // Use sscanf to parse the entry data
    sscanf(entry_data, "v %8s", plate);

    // Validate plate
    if (!validPlate(plate)){
        printf("%s: invalid licence plate.\n", plate);
        return;
    }

    // Retrieve the plate entry/exit logs and display them, if they exist
    Log* plateLogs = getPlateLogs(headPark, plate);
    if (plateLogs == NULL){
        printf("%s: no entries found in any parking.\n", plate);
    }
    else{
        printLog(plateLogs);
    }
    freeLog(plateLogs);
}


/**
 * @brief Processes the command to display the billing information for a park.
 * 
 * It extracts the park name and optional date from the input, validates
 * the inputs, retrieves the billing information for the specified park,
 * and displays it.
 * 
 * @param entry_data The input command string containing the park name for
 * which to display the billing information, and optionally a billing timestamp
 */
void command_f(char entry_data[BUFSIZ]){
    char* parkName;
    int day = 0, month = 0, year = 0;
    
    // Use sscanf to parse the entry data
    // First try to match the park name if it is between quotes
    int result = sscanf(entry_data, "f \"%m[^\"]\" %d-%d-%d",
                        &parkName, &day, &month, &year);
    if (result < 1){ // The park name is not between quotes
        result = sscanf(entry_data, "f%ms%d-%d-%d",
                        &parkName, &day, &month, &year);
    }

    // Verify if the park exists
    Park* park = getPark(headPark, parkName);
    if (park == NULL){
        printf("%s: no such parking.\n", parkName);
        free(parkName);
        return;
    }

    Timestamp t = newTimestamp(day, month, year, 0, 0);
    // Check if a date was passed in the command and if it was, validate it
    if (!isInitialTimestamp(&t)){
        if (!validTimestamp(&t) || compareTimestamps(&t, &lastTimestamp) == 1){
            printf("invalid date.\n");
            free(parkName);
            return;
        }
    }

    showParkBilling(park, &t);
    free(parkName);
}


/**
 * @brief Processes the command to remove a park from the system.
 * 
 * It removes the specified park from the system if it exists and prints the
 * names of the remaining parks alphabetically.
 * 
 * @param entry_data The input command string containing the name of the park.
 */
void command_r(char entry_data[BUFSIZ]){
    char* parkName;

    // Use sscanf to parse the entry data
    // First try to match the park name if it is between quotes
    int result = sscanf(entry_data, "r \"%m[^\"]\"", &parkName);
    if (result != 1){
        // The park name is not between quotes
        sscanf(entry_data, "r %ms", &parkName);
    }

    // Verify if park can successfuly be removed, and if so remove it
    if (removePark(&headPark, parkName)){
        printParksAlphabetically(headPark);
    }

    free(parkName);
}