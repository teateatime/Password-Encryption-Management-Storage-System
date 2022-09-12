#ifndef SUPPORT_H
#define SUPPORT_H

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

void CreateNewAccIDAndWriteIt(int n);

void CheckIfAccIDExist(int* num);

void CreateNewAccID(int* n);

void CheckIfAccIDIsYours(int* IDNum, int* num);

void CreateNewPassIDAndWriteIt(int* n);

void CheckIfPassIDExist(int* n);

Boolean CheckIfPassIDIsYoursBeforeUpdating(int* num, int* acc, sqlite3* db);

void EmailChecker(char* name);

Boolean LoginChecker(char* line);

void checkIfLoginEmailExists(char* line, int* IDNum);

Boolean refWordValidCheck(char* ref, int* IDNum);

void PasswordIDChecker(int* num);

Boolean CheckIfPasswordID_IsYours(int* num, int IDNum, sqlite3* db);

Boolean CheckIfEmailExistsAlready(char* email);

char* CheckIfItsYourEmail(char* email, int IDNum);

#endif
