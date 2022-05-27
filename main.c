#define  _GNU_SOURCE

#include "my_string.h"
#include "password.h"
#include "manager.h"

int main(int argc, char* argv[]) {
    FILE* fp;
    srand(time(NULL));

    fp = fopen("dictionary.txt", "r");

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

    sql = "CREATE TABLE IF NOT EXISTS PASSWORD(ID INT NOT NULL UNIQUE, Name TEXT, Password CHAR[50]);";

    ret = sqlite3_exec(db, sql, 0, 0, &ErrMsg);

    if (ret != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", ErrMsg);
        sqlite3_free(ErrMsg);
    } else {
        fprintf(stdout, "Table created successfully\n");
    }

    int answer = 0;
    printf("Welcome to Password Encrpytion Manangement System!!!\n\n");

    printf("Press (1) if you want to store your password\n");
    printf("Press (2) if you want to test your password strength\n");
    printf("Press (3) if you want us to generate a password for you and store it for safe keeping\n");
    printf("Press (4) if you want to scramble your current password and store it for safe keeping\n");
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

        fp3 = fopen("Names_Storage.txt", "r");
        fp4 = fopen("ID_Storage.txt", "r");

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

    answerChecker2(&answer);

    while (answer > 0 && answer < 9) {
        if (answer == 1) {
            printf("Entering StorePassword()...\n\n");
            StorePassword(arr, db);
            printf("\n");
            printf("Press (1) if you want to store your password\n");
            printf("Press (2) if you want to test your password strength\n");
            printf("Press (3) if you want us to generate a password for you and store it for safe keeping\n");
            printf("Press (4) if you want to scramble your current password and store it for safe keeping\n");
            printf("Press (5) if you want to search your password(s) inside the database\n");
            printf("Press (6) if you want to update your password(s) inside the database\n");
            printf("Press (7) if you want to delete your password(s) inside the database\n");
            printf("Press (8) to quit\n");
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

            answerChecker(&answer);
            answerChecker2(&answer);
        } else if (answer == 2) {
            printf("Entering TestPassword()...\n\n");
            TestPassword(arr, db);
            printf("\n");
            printf("Back to Main\n");
            printf("Press (1) if you want to store your password\n");
            printf("Press (2) if you want to test your password strength\n");
            printf("Press (3) if you want us to generate a password for you and store it for safe keeping\n");
            printf("Press (4) if you want to scramble your current password and store it for safe keeping\n");
            printf("Press (5) if you want to search your password(s) inside the database\n");
            printf("Press (6) if you want to update your password(s) inside the database\n");
            printf("Press (7) if you want to delete your password(s) inside the database\n");
            printf("Press (8) to quit\n");
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

            answerChecker(&answer);
            answerChecker2(&answer);
        } else if (answer == 3) {
            printf("Entering generatePassword()...\n\n");
            generatePassword(arr, db);
            printf("\n");
            printf("Back to Main\n");
            printf("Press (1) if you want to store your password\n");
            printf("Press (2) if you want to test your password strength\n");
            printf("Press (3) if you want us to generate a password for you and store it for safe keeping\n");
            printf("Press (4) if you want to scramble your current password and store it for safe keeping\n");
            printf("Press (5) if you want to search your password(s) inside the database\n");
            printf("Press (6) if you want to update your password(s) inside the database\n");
            printf("Press (7) if you want to delete your password(s) inside the database\n");
            printf("Press (8) to quit\n");
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

            answerChecker(&answer);
            answerChecker2(&answer);
        } else if (answer == 4) {
            printf("Entering ScramblePassword()...\n\n");
            ScramblePassword(arr, db);
            printf("\n");
            printf("Back to Main\n");
            printf("Press (1) if you want to store your password\n");
            printf("Press (2) if you want to test your password strength\n");
            printf("Press (3) if you want us to generate a password for you and store it for safe keeping\n");
            printf("Press (4) if you want to scramble your current password and store it for safe keeping\n");
            printf("Press (5) if you want to search your password(s) inside the database\n");
            printf("Press (6) if you want to update your password(s) inside the database\n");
            printf("Press (7) if you want to delete your password(s) inside the database\n");
            printf("Press (8) to quit\n");
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

            answerChecker(&answer);
            answerChecker2(&answer);
        } else if (answer == 5) {
            printf("Entering SearchPassword()...\n\n");
            SearchPassword(arr, db);
            printf("\n");
            printf("Back to Main\n");
            printf("Press (1) if you want to store your password\n");
            printf("Press (2) if you want to test your password strength\n");
            printf("Press (3) if you want us to generate a password for you and store it for safe keeping\n");
            printf("Press (4) if you want to scramble your current password and store it for safe keeping\n");
            printf("Press (5) if you want to search your password(s) inside the database\n");
            printf("Press (6) if you want to update your password(s) inside the database\n");
            printf("Press (7) if you want to delete your password(s) inside the database\n");
            printf("Press (8) to quit\n");
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

            answerChecker(&answer);
            answerChecker2(&answer);
        } else if (answer == 6) {
            printf("Entering UpdateCurrentPass()...\n\n");
            UpdateCurrentPass(arr, db);
            printf("\n");
            printf("Back to Main\n");
            printf("Press (1) if you want to store your password\n");
            printf("Press (2) if you want to test your password strength\n");
            printf("Press (3) if you want us to generate a password for you and store it for safe keeping\n");
            printf("Press (4) if you want to scramble your current password and store it for safe keeping\n");
            printf("Press (5) if you want to search your password(s) inside the database\n");
            printf("Press (6) if you want to update your password(s) inside the database\n");
            printf("Press (7) if you want to delete your password(s) inside the database\n");
            printf("Press (8) to quit\n");
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

            answerChecker(&answer);
            answerChecker2(&answer);
        } else if (answer == 7) {
            printf("Entering DeletePasswordContent()...\n\n");
            DeletePasswordContent(arr, db);
            printf("\n");
            printf("Back to Main\n");
            printf("Press (1) if you want to store your password\n");
            printf("Press (2) if you want to test your password strength\n");
            printf("Press (3) if you want us to generate a password for you and store it for safe keeping\n");
            printf("Press (4) if you want to scramble your current password and store it for safe keeping\n");
            printf("Press (5) if you want to search your password(s) inside the database\n");
            printf("Press (6) if you want to update your password(s) inside the database\n");
            printf("Press (7) if you want to delete your password(s) inside the database\n");
            printf("Press (8) to quit\n");
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

            answerChecker(&answer);
            answerChecker2(&answer);
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
