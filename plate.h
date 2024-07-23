/**
 * Prototypes for the functions related to licence plates
 * 
 * Author: Adolfo Monteiro
*/
#ifndef PLATE_H
#define PLATE_H

// XX-XX-XX plus 1 space for \0
#define PLATE_LENGTH 9

void printPlate(const char plate[PLATE_LENGTH]);
int validPlate(const char plate[PLATE_LENGTH]);
#endif