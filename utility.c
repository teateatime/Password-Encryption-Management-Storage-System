#define  _GNU_SOURCE

#include "utility.h"
#include "my_string.h"
#include "manager.h"
#include "store.h"

void printOptions(void) {
    printf("\n");
    printf("Press (1) if you want to store the password you want\n");
    printf("Press (2) if you want to test your password strength\n");
    printf("Press (3) if you want us to generate a password for you and store it for safe keeping\n");
    printf("Press (4) if you want to scramble your current password to create a new pass and store that for safe keeping\n");
    printf("Press (5) if you want to search your password(s) inside the database\n");
    printf("Press (6) if you want to update your password(s) inside the database\n");
    printf("Press (7) if you want to delete your password(s) inside the database\n");
    printf("Press (8) to return to main\n");
}

int PrintDataInDB(void *data, int argc, char **argv, char **ColName) {
    int i = 0;
    data = 0;
    printf("Printing...\n");
    while (i < argc) {
        printf("%s = %s\n", ColName[i], argv[i] ? argv[i] : "NULL");
        i++;
    }
    printf("\n");

    return 0;
}

int PrintUpdatedDataInDB(void *data, int argc, char **argv, char **ColName) {
    int i = 0;
    printf("Updating...\n");
    fprintf(stderr, "%s: ", (const char*)data);
    while (i < argc) {
        printf("%s = %s\n", ColName[i], argv[i] ? argv[i] : "NULL");
        i++;
    }
    printf("\n");
    return 0;
}

void Encryption(char* line) {
    for (int i = 0; (i < 100 && line[i] != '\0'); i++) {
        if (line[i] == '$') {
            line[i] += 1;
        }
        line[i] = line[i] + 3;
    }
}

char* Decryption(char* line) {
    for (int i = 0; (i < 100 && line[i] != '\0'); i++) {
        line[i] = line[i] - 3;
        if (line[i] == '%') {
            line[i] -= 1;
        }
    }
    return line;
}

void RetrieveData(sqlite3* db, int num) {
    sqlite3_stmt* st;
    char* squote = "'";
    char sql[100] = "select * from PASSWORD where PASS_ID  = '";
    char tmp[100];
    sprintf(tmp, "%d", num);
    strcat(sql, tmp);
    strcat(sql, squote);
    int rc = sqlite3_prepare_v2(db, sql, -1, &st, 0);

    char str[100] = "";
    strcat(str, squote);
    strcat(sql, tmp);
    strcat(sql, squote);

    if (rc == SQLITE_OK) {
        int idx = sqlite3_bind_parameter_index(st, str);
        int value = 4;
        sqlite3_bind_int(st, idx, value);
    } else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }

    int step = sqlite3_step(st);

    if (step == SQLITE_ROW) {
        char* s2 = (char*)sqlite3_column_text(st, 4);
        printf("Decrypted Password: %s\n", Decryption(s2));
    }

    sqlite3_finalize(st);

    return;
}

void clearKeyboardBuffer(void){
   char ch;
   scanf("%c", &ch);
   while (ch != '\n' && ch != '\0') {
      scanf("%c", &ch);
   }
}

/* Checks if database is empty or not, so we prevent users
from searching/deleting.
*/
void DatabaseIsNotFull(int* answer) {
    int noc;
    if (*answer == 5 || *answer == 6 || *answer == 7) {
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

        while ((size == 0 || size2 == 0) && (*answer == 5 || *answer == 6 || *answer == 7)) {
            printf("Database is empty. Please select a different option:\n");
            noc = scanf("%d", answer);
            clearKeyboardBuffer();

            while (noc != 1) {
                printf("Not a number between 1-8! Enter again!\n");
                noc = scanf("%d", answer);
                clearKeyboardBuffer();
            }

            while (*answer < 1 || *answer > 8) {
                printf("Not a number between 1-8! Enter again!\n");
                scanf("%d", answer);
                clearKeyboardBuffer();
            }
        }
        fclose(fp3);
        fclose(fp4);
    }
}

/* Checks if database is full or not, so we prevent users
from adding more data.
*/
void DatabaseIsFull(int* answer) {
    int noc;
    if (*answer == 1 || *answer == 3 || *answer == 4) {
        FILE* fp3;
        FILE* fp4;

        fp3 = fopen("Text/Names_Storage.txt", "r");
        fp4 = fopen("Text/Password_ID.txt", "r");

        if (!fp3 || !fp4) {
            printf("Error: Unable to open files.\n");
            exit(1);
        }

        fseek(fp3, 0, SEEK_END);
        fseek(fp4, 0, SEEK_END);
        int size = ftell(fp3);
        int size2 = ftell(fp4);

        while ((size >= 4999999 || size2 >= 4999999) && (*answer == 1 || *answer == 3 || *answer == 4)) {
            printf("Database is full. Please select a different option:\n");
            noc = scanf("%d", answer);
            clearKeyboardBuffer();

            while (noc != 1) {
                printf("Not a number between 1-8! Enter again!\n");
                noc = scanf("%d", answer);
                clearKeyboardBuffer();
            }

            while (*answer < 1 || *answer > 8) {
                printf("Not a number between 1-8! Enter again!\n");
                scanf("%d", answer);
                clearKeyboardBuffer();
            }
        }
        fclose(fp3);
        fclose(fp4);
    }
}

/* Checks if database is empty or not for that particular user, so we prevent him/her
from searching/deleting.
*/
void DatabaseIsEmptyForUser(int* answer, int* IDNum) {
    int noc;
    if (*answer == 5 || *answer == 6 || *answer == 7) {
        FILE* fp3; FILE* fplog; FILE* fpID;
        int numVal = 0;
        int lineNum = 1;

        fp3 = fopen("Text/Email_Storage.txt", "r");
        fplog = fopen("Text/Mail_Storage.txt", "r");
        fpID = fopen("Text/ID_Storage.txt", "r"); 

        if (!fp3 || !fplog || !fpID) {
            printf("Error: Unable to open files.\n");
            exit(1);
        }

        while (fscanf(fpID, "%d", &numVal) != EOF) {
            if (*IDNum == numVal) {
                break;
            } else {
                lineNum++;
            }
        }

        char* email = NULL;
        int lineNumber = 1;
        size_t len = 0;
        while ((len = getline(&email, &len, fplog)) != EOF) {
            email[len - 1] = '\0';
            if (lineNumber == lineNum) {
                break;
            } else {
                lineNumber++;
            }
        }

        MY_STRING s1 = my_string_init_c_string(email);
        MY_STRING s2 = NULL;
        char* remail = NULL;
        int counter = 0;

        while ((len = getline(&remail, &len, fp3)) != EOF) {
            remail[len - 1] = '\0';
            s2 = my_string_init_c_string(remail);
            if (my_string_equal(s1, s2)) {
                counter++;
            }
            my_string_destroy(&s2);
        }

        while ((*answer == 5 || *answer == 6 || *answer == 7) && (counter == 0)) {
            printf("Database is empty. Please select a different option:\n");
            noc = scanf("%d", answer);
            clearKeyboardBuffer();

            while (noc != 1) {
                printf("Not a number between 1-8! Enter again!\n");
                noc = scanf("%d", answer);
                clearKeyboardBuffer();
            }

            while (*answer < 1 || *answer > 8) {
                printf("Not a number between 1-8! Enter again!\n");
                scanf("%d", answer);
                clearKeyboardBuffer();
            }
        }

        my_string_destroy(&s1);
        free(remail);
        free(email);
        fclose(fplog);
        fclose(fpID);
        fclose(fp3);

    }
}

void menu_selection(int answer, char* arr, int* IDNum, sqlite3* db) {
    while (answer > 0) {
        if (answer == 1) {
            printf("Entering StorePassword()...\n");
            StorePassword(arr, IDNum, db);
            return;
        } else if (answer == 2) {
            printf("Entering TestPassword()...\n");
            TestPassword(arr, IDNum, db);
            return;
        } else if (answer == 3) {
            printf("Entering generatePass word()...\n");
            generatePassword(arr, IDNum, db);
            return;
        } else if (answer == 4) {
            printf("Entering ScramblePassword()...\n");
            ScramblePassword(arr, IDNum, db);
            return;
        } else if (answer == 5) {
            printf("Entering SearchPassword()...\n");
            SearchPassword(arr, IDNum, db);
            return;
        } else if (answer == 6) {
            printf("Entering UpdateCurrentPass()...\n");
            UpdateCurrentPass(arr, IDNum, db);
            return;
        } else if (answer == 7) {
            printf("Entering DeletePasswordContent()...\n");
            DeletePasswordContent(arr, IDNum, db);
            return;
        } else {
            printf("Returning to Main Menu...\n\n");
            return;
        }
    }
}