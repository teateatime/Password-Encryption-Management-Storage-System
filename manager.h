#ifndef MANAGER_H
#define MANAGER_H

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

void SearchPassword(char* arr, int* IDNum, sqlite3* db);

void UpdateCurrentPass(char* arr, int* IDNum, sqlite3* db);

void DeletePasswordContent(char* arr, int* IDNum, sqlite3* db);

#endif
