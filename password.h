#ifndef PASSWORD_H
#define PASSWORD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include "status.h"

typedef struct Password {
    char* data;
    int size;
    int capacity;
    int strength;
} password;

typedef void* PASS;

PASS Password_init_default(void);

void password_destroy(PASS* pItem);

PASS Password_Str(char* str);

PASS Password_Calc_Strength(password* str, char* s);

PASS Password_Calc_Strength2(password* str, char* s);

void Password_Print_Strength(char* str);

char* Password_C_Str(password* str);

PASS Update_Password(char* s);

PASS Update_Password_By_Scramble(char* s, char* p);

Boolean is_substr(char* s, password* str);

Boolean CheckPassValid(password* str);

Boolean CheckAlpha(password* str);

Boolean CheckNum(password* str);

Boolean CheckSpecialChars(password* str);

Boolean CheckSpecialChars2(char str);

Boolean CheckSpaces(password* str);

int getSize(PASS pass);

int getCapacity(PASS pass);

int getStrength(PASS pass);

char* getData(PASS pass);

#endif