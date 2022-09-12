#define _GNU_SOURCE

#include "my_string.h"
#include "password.h"
#include "manager.h"
#include "pems.h"
#include "login.h"
#include "support.h"
#include "utility.h"
#include "store.h"

int PEMS(int* IDNum) {
    FILE* fp;
    fp = fopen("Text/dictionary.txt", "r");

    if (!fp) {
        printf("Error: Failed to open file!\n");
        exit(1);
    }

    fseek(fp, 0, SEEK_END);
    int length = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    length += 1;

    char* arr = (char*)malloc(sizeof(char) * (length));

    char c; int i = 0;
    c = fgetc(fp);

    while (c != EOF) {
        arr[i++] = c;
        c = fgetc(fp);
    }

    arr[i] = '\0';

    sqlite3 *db;
    char *ErrMsg = 0;
    char *sql;
    int ret;

    ret = sqlite3_open("password.db", &db);

    if (ret) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 0;
    } else {
        fprintf(stderr, "Opened database successfully\n");
    }

    sql = "CREATE TABLE IF NOT EXISTS PASSWORD(ID INT, PASS_ID INT, Name TEXT, Email CHAR[50], Password CHAR[50]);";

    ret = sqlite3_exec(db, sql, 0, 0, &ErrMsg);

    if (ret != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", ErrMsg);
        sqlite3_free(ErrMsg);
    } else {
        fprintf(stdout, "Table created successfully\n");
    }

    int answer = 0;
    printf("Welcome to the Password Encryption Manangement System!!!\n\n");

    printf("Press (1) if you want to store your password\n");
    printf("Press (2) if you want to test your password strength\n");
    printf("Press (3) if you want us to generate a password for you and store it for safe keeping\n");
    printf("Press (4) if you want to scramble your current password to create a new pass and store that for safe keeping\n");
    printf("Press (5) if you want to search your password(s) inside the database\n");
    printf("Press (6) if you want to update your password(s) inside the database\n");
    printf("Press (7) if you want to delete your password(s) inside the database\n");
    printf("Press (8) to quit\n");
    int noc = scanf("%d", &answer);
    clearKeyboardBuffer();

    while (noc != 1) {
        printf("Not a number between 1-8! Enter again!\n");
        noc = scanf("%d", &answer);
        clearKeyboardBuffer();
    }

    while (answer < 1 || answer > 8) {
        printf("Not a number between 1-8! Enter again!\n");
        scanf("%d", &answer);
        clearKeyboardBuffer();
    }

    if (answer == 5 || answer == 6 || answer == 7) {
        FILE* fp3;
        FILE* fp4;

        fp3 = fopen("Text/Names_Storage.txt", "r");
        fp4 = fopen("Text/ID_Storage.txt", "r");

        if (!fp3 || !fp4) {
            printf("Error: Unable to open files.\n");
            exit(1);
        }

        fseek(fp3, 0, SEEK_END);
        fseek(fp4, 0, SEEK_END);
        int size = ftell(fp3);
        int size2 = ftell(fp4);

        while ((size == 0 || size2 == 0) && (answer == 5 || answer == 6 || answer == 7)) {
            printf("Database is empty. Please select a different option:\n");
            noc = scanf("%d", &answer);
            clearKeyboardBuffer();

            while (noc != 1) {
                printf("Not a number between 1-8! Enter again!\n");
                noc = scanf("%d", &answer);
                clearKeyboardBuffer();
            }

            while (answer < 1 || answer > 8) {
                printf("Not a number between 1-8! Enter again!\n");
                scanf("%d", &answer);
                clearKeyboardBuffer();
            }
        }
        fclose(fp3);
        fclose(fp4);
    }

    DatabaseIsFull(&answer);
    DatabaseIsEmptyForUser(&answer, IDNum);

    while (answer > 0 && answer < 9) {
        if (answer == 1) {
            printf("Entering StorePassword()...\n");
            StorePassword(arr, IDNum, db);

            printOptionsinMain();
            noc = scanf("%d", &answer);
            clearKeyboardBuffer();
            while (noc != 1) {
                printf("Not a number between 1-8! Enter again!\n");
                noc = scanf("%d", &answer);
                clearKeyboardBuffer();
            }

            while (answer < 1 || answer > 8) {
                printf("Not a number between 1-8! Enter again!\n");
                scanf("%d", &answer);
                clearKeyboardBuffer();
            }

            DatabaseIsNotFull(&answer);
            DatabaseIsFull(&answer);
            DatabaseIsEmptyForUser(&answer, IDNum);

        } else if (answer == 2) {
            printf("Entering TestPassword()...\n");
            TestPassword(arr, IDNum, db);

            printOptionsinMain();
            noc = scanf("%d", &answer);
            clearKeyboardBuffer();
            while (noc != 1) {
                printf("Not a number between 1-8! Enter again!\n");
                noc = scanf("%d", &answer);
                clearKeyboardBuffer();
            }

            while (answer < 1 || answer > 8) {
                printf("Not a number between 1-8! Enter again!\n");
                scanf("%d", &answer);
                clearKeyboardBuffer();
            }

            DatabaseIsNotFull(&answer);
            DatabaseIsFull(&answer);
            DatabaseIsEmptyForUser(&answer, IDNum);

        } else if (answer == 3) {
            printf("Entering generatePassword()...\n");
            generatePassword(arr, IDNum, db);

            printOptionsinMain();
            noc = scanf("%d", &answer);
            clearKeyboardBuffer();
            while (noc != 1) {
                printf("Not a number between 1-8! Enter again!\n");
                noc = scanf("%d", &answer);
                clearKeyboardBuffer();
            }

            while (answer < 1 || answer > 8) {
                printf("Not a number between 1-8! Enter again!\n");
                scanf("%d", &answer);
                clearKeyboardBuffer();
            }

            DatabaseIsNotFull(&answer);
            DatabaseIsFull(&answer);
            DatabaseIsEmptyForUser(&answer, IDNum);

        } else if (answer == 4) {
            printf("Entering ScramblePassword()...\n");
            ScramblePassword(arr, IDNum, db);

            printOptionsinMain();
            noc = scanf("%d", &answer);
            clearKeyboardBuffer();
            while (noc != 1) {
                printf("Not a number between 1-8! Enter again!\n");
                noc = scanf("%d", &answer);
                clearKeyboardBuffer();
            }

            while (answer < 1 || answer > 8) {
                printf("Not a number between 1-8! Enter again!\n");
                scanf("%d", &answer);
                clearKeyboardBuffer();
            }

            DatabaseIsNotFull(&answer);
            DatabaseIsFull(&answer);
            DatabaseIsEmptyForUser(&answer, IDNum);

        } else if (answer == 5) {
            printf("Entering SearchPassword()...\n");
            SearchPassword(arr, IDNum, db);

            printOptionsinMain();
            noc = scanf("%d", &answer);
            clearKeyboardBuffer();
            while (noc != 1) {
                printf("Not a number between 1-8! Enter again!\n");
                noc = scanf("%d", &answer);
                clearKeyboardBuffer();
            }

            while (answer < 1 || answer > 8) {
                printf("Not a number between 1-8! Enter again!\n");
                scanf("%d", &answer);
                clearKeyboardBuffer();
            }

            DatabaseIsNotFull(&answer);
            DatabaseIsFull(&answer);
            DatabaseIsEmptyForUser(&answer, IDNum);

        } else if (answer == 6) {
            printf("Entering UpdateCurrentPass()...\n");
            UpdateCurrentPass(arr, IDNum, db);

            printOptionsinMain();
            noc = scanf("%d", &answer);
            clearKeyboardBuffer();
            while (noc != 1) {
                printf("Not a number between 1-8! Enter again!\n");
                noc = scanf("%d", &answer);
                clearKeyboardBuffer();
            }

            while (answer < 1 || answer > 8) {
                printf("Not a number between 1-8! Enter again!\n");
                scanf("%d", &answer);
                clearKeyboardBuffer();
            }

            DatabaseIsNotFull(&answer);
            DatabaseIsFull(&answer);
            DatabaseIsEmptyForUser(&answer, IDNum);

        } else if (answer == 7) {
            printf("Entering DeletePasswordContent()...\n");
            DeletePasswordContent(arr, IDNum, db);

            printOptionsinMain();
            noc = scanf("%d", &answer);
            clearKeyboardBuffer();
            while (noc != 1) {
                printf("Not a number between 1-8! Enter again!\n");
                noc = scanf("%d", &answer);
                clearKeyboardBuffer();
            }

            while (answer < 1 || answer > 8) {
                printf("Not a number between 1-8! Enter again!\n");
                scanf("%d", &answer);
                clearKeyboardBuffer();
            }

            DatabaseIsNotFull(&answer);
            DatabaseIsFull(&answer);
            DatabaseIsEmptyForUser(&answer, IDNum);
        } else {
            printf("Exiting out program...\nHave a nice day.\n");
            fclose(fp);
            sqlite3_close(db);
            free(arr);
            return 0;
        }
    }

    return 0;
}

void printOptionsinMain(void) {
    printf("\n");
    printf("Back to Main\n");
    printf("Press (1) if you want to store your password\n");
    printf("Press (2) if you want to test your password strength\n");
    printf("Press (3) if you want us to generate a password for you and store it for safe keeping\n");
    printf("Press (4) if you want to scramble your current password to create a new pass and store that for safe keeping\n");
    printf("Press (5) if you want to search your password(s) inside the database\n");
    printf("Press (6) if you want to update your password(s) inside the database\n");
    printf("Press (7) if you want to delete your password(s) inside the database\n");
    printf("Press (8) to quit\n");
}

void CheckIfTwoUserInputValid(int r, int* userInput) {
    while (r != 1) {
        printf("Not a number between 1-2! Enter again!\n");
        r = scanf("%d", userInput);
        clearKeyboardBuffer();
    }

    while (*userInput < 1 || *userInput > 2) {
        printf("Not a number between 1-2! Enter again!\n");
        scanf("%d", userInput);
        clearKeyboardBuffer();
    }
}

bool isChar(char c) {
    return ((c >= 'a' && c <= 'z')
    || (c >= 'A' && c <= 'Z'));
}

bool isDigit(const char c) {
    return (c >= '0' && c <= '9');
}

bool is_valid(char* email) {
    if (!isChar(email[0])) {
        return FALSE;
    }

    int At = -1, Dot = -1;

    for (int i = 0; i < strlen(email); i++) {
        if (email[i] == '@') {
            At = i;
        } else if (email[i] == '.') {
            Dot = i;
        }
    }

    if (At == -1 || Dot == -1)
        return 0;
    if (At > Dot)
        return 0;

    return !(Dot >= (strlen(email) - 1));
}