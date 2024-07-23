/**
 * Implementation of the functions related to licence plates.
 * 
 * Author: Adolfo Monteiro
*/
#include <stdio.h>
#include <string.h>
#include "plate.h"


/**
 * @brief Prints a licence plate.
 * 
 * This function prints the given licence plate to the standard output.x    
 * 
 * @param plate The licence plate to be printed.
 */
void printPlate(const char plate[PLATE_LENGTH]){
    printf("%s", plate);
}


/**
 * @brief Checks if a character is an uppercase letter.
 * 
 * This function checks if the given character is an uppercase letter.
 * 
 * @param c Pointer to the character to be checked.
 * @return 1 if the character is an uppercase letter, otherwise 0.
 */
int isUpperLetter(const char* c){
    return *c >= 'A' && *c <= 'Z';
}


/**
 * @brief Checks if a character is a digit.
 * 
 * This function checks if the given character is a digit.
 * 
 * @param c Pointer to the character to be checked.
 * @return 1 if the character is a digit, otherwise 0.
 */
int isDigit(const char* c){
    return *c >= '0' && *c <= '9';
}


/**
 * @brief Checks if a licence plate is valid.
 * 
 * This function checks if the given licence plate is valid.
 * 
 * A plate is valid if it is in the format XX-XX-XX where:
 * - X may only be an uppercase letter or a digit;
 * - each pair XX must be only letters or only digits;
 * - there must be atleast 1 pair of letters and 1 pair of digits.
 * 
 * @param plate The licence plate to be checked.
 * @return 1 if the licence plate is valid, otherwise 0.
 */
int validPlate(const char plate[PLATE_LENGTH]){
    if (plate[2] != '-' || plate[5] != '-')
        return 0;

    int numberPairs = 0, letterPairs = 0;

    for (int i = 0; i < PLATE_LENGTH - 1; i += 3) {
        // Each pair must be only letters or only numbers
        if (isDigit(&plate[i]) && isDigit(&plate[i + 1]))
            numberPairs++;
        else if (isUpperLetter(&plate[i]) && isUpperLetter(&plate[i + 1]))
            letterPairs++;
        else
            return 0;  // Invalid pair
    }

    // Must have atleast 1 pair of numbers and letters
    return (numberPairs > 0 && letterPairs > 0) ? 1 : 0;
}