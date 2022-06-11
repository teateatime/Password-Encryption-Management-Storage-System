#ifndef MANAGER_H
#define MANAGER_H

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

void clearKeyboardBuffer(void);

void StorePassword(char* arr, int* IDNum, sqlite3* db);

void TestPassword(char* arr, int* IDNum, sqlite3* db);

void generatePassword(char* arr, int* IDNum, sqlite3* db);

void ScramblePassword(char* arr, int* IDNum, sqlite3* db);

void SearchPassword(char* arr, int* IDNum, sqlite3* db);

void UpdateCurrentPass(char* arr, int* IDNum, sqlite3* db);

void DeletePasswordContent(char* arr, int* IDNum, sqlite3* db);

int PrintData(void *NotUsed, int argc, char **argv, char **ColName);

int PrintData2(void *data, int argc, char **argv, char **ColName);

Boolean is_substr_char(char* s, char* str);

void IDChecker(int n);

void IDChecker2(int* num);

void IDChecker3(int* n);

void IDChecker4(int* IDNum, int* num);

void IDChecker5(int* n);

void IDChecker6(int* n);

Boolean IDChecker7(int* num, int* acc, sqlite3* db);

void NameChecker(char* name);

Boolean NameChecker2(char* line);

void answerChecker(int* answer);

void answerChecker2(int* answer);

void answerChecker3(int* answer, int* IDNum);

void emailChecker(char* line, int* IDNum);

void Encryption(char* line);

char* Decryption(char* line);

void RetrieveData(sqlite3* db, int num);

Boolean refWordValidCheck(char* ref, int* IDNum);

void PasswordIDChecker(int* num);

Boolean PasswordIDChecker2(int* num, int IDNum);

Boolean emailValid(char* email);

char* emailValid2(char* email, int IDNum);

#endif
