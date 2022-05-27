#ifndef MANAGER_H
#define MANAGER_H

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

void clearKeyboardBuffer(void);
void StorePassword(char* arr, sqlite3* db);
void TestPassword(char* arr, sqlite3* db);
void generatePassword(char* arr, sqlite3* db);
void ScramblePassword(char* arr, sqlite3* db);
void SearchPassword(char* arr, sqlite3* db);
void UpdateCurrentPass(char* arr, sqlite3* db);
void DeletePasswordContent(char* arr, sqlite3* db);
int PrintData(void *NotUsed, int argc, char **argv, char **ColName);
int PrintData2(void *data, int argc, char **argv, char **ColName);
void IDChecker(int n);
void IDChecker2(int* num);
void NameChecker(char* name);
void answerChecker(int* answer);
void answerChecker2(int* answer);

#endif
