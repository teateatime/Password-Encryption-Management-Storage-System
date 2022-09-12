#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

void clearKeyboardBuffer(void);

void printOptions(void);

void menu_selection(int answer, char* arr, int* IDNum, sqlite3* db);

int PrintDataInDB(void *NotUsed, int argc, char **argv, char **ColName);

int PrintUpdatedDataInDB(void *data, int argc, char **argv, char **ColName);

void Encryption(char* line);

char* Decryption(char* line);

void RetrieveData(sqlite3* db, int num);

void DatabaseIsNotFull(int* answer);

void DatabaseIsFull(int* answer);

void DatabaseIsEmptyForUser(int* answer, int* IDNum);

#endif