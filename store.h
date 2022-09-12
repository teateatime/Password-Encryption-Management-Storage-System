#ifndef STORE_H
#define STORE_H

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

void StorePassword(char* arr, int* IDNum, sqlite3* db);

void TestPassword(char* arr, int* IDNum, sqlite3* db);

void generatePassword(char* arr, int* IDNum, sqlite3* db);

void ScramblePassword(char* arr, int* IDNum, sqlite3* db);

#endif
