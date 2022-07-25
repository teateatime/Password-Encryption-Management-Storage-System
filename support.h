#ifndef SUPPORT_H
#define SUPPORT_H

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

void clearKeyboardBuffer(void);

void CreateNewAccIDAndWriteIt(int n);

void CheckIfAccIDExist(int* num);

void CreateNewAccID(int* n);

void CheckIfAccIDIsYours(int* IDNum, int* num);

void CreateNewPassIDAndWriteIt(int* n);

void CheckIfPassIDExist(int* n);

Boolean CheckIfPassIDIsYoursBeforeUpdating(int* num, int* acc, sqlite3* db);

void EmailChecker(char* name);

Boolean LoginChecker(char* line);

void DatabaseIsNotFull(int* answer);

void DatabaseIsFull(int* answer);

void DatabaseIsEmptyForUser(int* answer, int* IDNum);

void checkIfLoginEmailExists(char* line, int* IDNum);

Boolean refWordValidCheck(char* ref, int* IDNum);

void PasswordIDChecker(int* num);

Boolean CheckIfPasswordID_IsYours(int* num, int IDNum, sqlite3* db);

Boolean CheckIfEmailExistsAlready(char* email);

char* CheckIfItsYourEmail(char* email, int IDNum);

int PrintDataInDB(void *NotUsed, int argc, char **argv, char **ColName);

int PrintUpdatedDataInDB(void *data, int argc, char **argv, char **ColName);

void Encryption(char* line);

char* Decryption(char* line);

void RetrieveData(sqlite3* db, int num);

#endif
