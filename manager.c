#define  _GNU_SOURCE

#include "my_string.h"
#include "password.h"
#include "manager.h"
#include "support.h"
#include "utility.h"
#include "store.h"

void SearchPassword(char* arr, int* IDNum, sqlite3* db) {
    char* line = NULL;
    size_t len = 0;
    char* item;
    char* squote = "'";
    int num = 0;

    int ans;
    printf("\nSearching what kind of password(s) you stored up?\n");
    printf("Press (1) if you want to search by email address\n");
    printf("Press (2) if you want to search by Account ID\n");
    printf("Press (3) if you want to search by Password ID\n");

    int noc2 = scanf("%d", &ans);
    clearKeyboardBuffer();

    while (noc2 != 1) {
        printf("Not a number between 1-3! Enter again!\n");
        noc2 = scanf("%d", &ans);
        clearKeyboardBuffer();
    }

    while (ans < 1 || ans > 3) {
        printf("Not a number between 1-3! Enter again!\n");
        scanf("%d", &ans);
        clearKeyboardBuffer();
    }

    char dest[120] = "select * from PASSWORD where ";
    char* errMesg = 0;

    if (ans == 1) {
        printf("Please enter your email address:\n");
        len = getline(&line, &len, stdin);

        item = "Email";

        EmailChecker(line);
        checkIfLoginEmailExists(line, IDNum);

        int x = 0;
        while (line[x] != '\0') {
            if (line[x] == '\n' || line[x] == '\r') {
                line[x] = '\0';
            }
            x++;
        }

        strcat(dest, item);
        strcat(dest, " = ");
        strcat(dest, squote);
        strcat(dest, line);
        strcat(dest, squote);

        int ret = sqlite3_exec(db, dest, PrintDataInDB, 0, &errMesg);

        if (ret != SQLITE_OK) {
            printf("Error in SQL statement: %s\n", errMesg);
            sqlite3_free(errMesg);
            sqlite3_close(db);
            return;
        }

        printf("Do you want to checkout what the decrypted password string for a specific password?\n");
        printf("Press (1) if yes\n");
        printf("Press (2) if no\n");

        int n = 0;
        int rett = scanf("%d", &n);
        clearKeyboardBuffer();

        while (rett != 1) {
            printf("Not a number between 1-2! Enter again!\n");
            rett = scanf("%d", &n);
            clearKeyboardBuffer();
        }

        while (n < 1 || n > 2) {
            printf("Not a number between 1-2! Enter again!\n");
            scanf("%d", &n);
            clearKeyboardBuffer();
        }

        if (n == 1) {
            printf("Please enter the Password ID number of a password that you own that you want to decrypt:\n");
            scanf("%d", &num);
            clearKeyboardBuffer();
            printf("\n");

            PasswordIDChecker(&num);
            if (CheckIfPasswordID_IsYours(&num, *IDNum, db) == FALSE) {
                printf("This ID either does not belong to you or it does not exist!\n");
                printf("Please Try Again Later.\n");

                free(line);

                int answer;
                printOptions();

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

                DatabaseIsNotFull(&answer);
                DatabaseIsFull(&answer);
                DatabaseIsEmptyForUser(&answer, IDNum);

                menu_selection(answer, arr, IDNum, db);
                return;
            }

            char* ref = NULL;
            printf("Please enter your reference word in order to decrypt your password:\n");
            printf("Otherwise, enter any other string or character to exit out if you changed your mind.\n");
            size_t siz = getline(&ref, &siz, stdin);
            ref[siz - 1] = '\0';

            if (!refWordValidCheck(ref, IDNum)) {
                printf("Incorrect reference word!\n");
                printf("Please Try Again Later.\n");
                free(ref);
                free(line);

                int answer;
                printOptions();

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

                DatabaseIsNotFull(&answer);
                DatabaseIsFull(&answer);
                DatabaseIsEmptyForUser(&answer, IDNum);

                menu_selection(answer, arr, IDNum, db);
                return;
            }

            RetrieveData(db, num);
            free(ref);
        }
    } else if (ans == 2) {
        printf("Please enter your Account ID number:\n");
        scanf("%d", &num);
        clearKeyboardBuffer();
        printf("\n");

        CheckIfAccIDExist(&num);
        CheckIfAccIDIsYours(IDNum, &num);
        item = "ID";
        char ntmp[50];
        sprintf(ntmp, "%d", num);

        strcat(dest, item);
        strcat(dest, " = ");
        strcat(dest, squote);
        strcat(dest, ntmp);
        strcat(dest, squote);

        int ret = sqlite3_exec(db, dest, PrintDataInDB, 0, &errMesg);

        if (ret != SQLITE_OK) {
            printf("Error in SQL statement: %s\n", errMesg);
            sqlite3_free(errMesg);
            sqlite3_close(db);
            return;
        }

        printf("Do you want to checkout what the decrypted password string for a specific password?\n");
        printf("Press (1) if yes\n");
        printf("Press (2) if no\n");

        int n = 0;
        int x = scanf("%d", &n);
        clearKeyboardBuffer();

        while (x != 1) {
            printf("Not a number between 1-2! Enter again!\n");
            x = scanf("%d", &n);
            clearKeyboardBuffer();
        }

        while (n < 1 || n > 2) {
            printf("Not a number between 1-2! Enter again!\n");
            scanf("%d", &n);
            clearKeyboardBuffer();
        }

        if (n == 1) {
            printf("Please enter the Password ID number of a password that you own that you want to decrypt:\n");
            scanf("%d", &num);
            clearKeyboardBuffer();
            printf("\n");

            PasswordIDChecker(&num);
            if (CheckIfPasswordID_IsYours(&num, *IDNum, db) == FALSE) {
                printf("This ID either does not belong to you or it does not exist!\n");
                printf("Please Try Again Later.\n");

                int answer;
                printOptions();

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

                DatabaseIsNotFull(&answer);
                DatabaseIsFull(&answer);
                DatabaseIsEmptyForUser(&answer, IDNum);

                menu_selection(answer, arr, IDNum, db);
                return;
            }

            char* ref = NULL;
            printf("Please enter your reference word in order to decrypt your password:\n");
            printf("Otherwise, enter any other string or character to exit out if you changed your mind.\n");
            size_t siz = getline(&ref, &siz, stdin);
            ref[siz - 1] = '\0';

            if (!refWordValidCheck(ref, IDNum)) {
                printf("Incorrect reference word!\n");
                printf("Please Try Again Later.\n");
                free(ref);

                int answer;
                printOptions();

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

                DatabaseIsNotFull(&answer);
                DatabaseIsFull(&answer);
                DatabaseIsEmptyForUser(&answer, IDNum);

                menu_selection(answer, arr, IDNum, db);
                return;
            }

            RetrieveData(db, num);
            free(ref);
        }
    } else if (ans == 3) {
        printf("Please enter the Password ID number for a password that you own that you want to decrypt:\n");
        scanf("%d", &num);
        clearKeyboardBuffer();
        printf("\n");

        PasswordIDChecker(&num);
        if (CheckIfPasswordID_IsYours(&num, *IDNum, db) == FALSE) {
            printf("This ID either does not belong to you or it does not exist!\n");
            printf("Please Try Again Later.\n");

            int answer;
            printOptions();

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

            DatabaseIsNotFull(&answer);
            DatabaseIsFull(&answer);
            DatabaseIsEmptyForUser(&answer, IDNum);

            menu_selection(answer, arr, IDNum, db);
            return;
        }

        item = "PASS_ID ";
        char ntmp[50];
        sprintf(ntmp, "%d", num);

        strcat(dest, item);
        strcat(dest, " = ");
        strcat(dest, squote);
        strcat(dest, ntmp);
        strcat(dest, squote);

        int ret = sqlite3_exec(db, dest, PrintDataInDB, 0, &errMesg);

        if (ret != SQLITE_OK) {
            printf("Error in SQL statement: %s\n", errMesg);
            sqlite3_free(errMesg);
            sqlite3_close(db);
            return;
        }

        int val = 0;
        printf("Enter (1) if you want to see your decrypted password for this\n");
        printf("Enter (2) to skip this section\n");
        int r = scanf("%d", &val);
        clearKeyboardBuffer();

        while (r != 1) {
            printf("Not a number between 1-2! Enter again!\n");
            r = scanf("%d", &val);
            clearKeyboardBuffer();
        }

        while (val < 1 || val > 2) {
            printf("Not a number between 1-2! Enter again!\n");
            scanf("%d", &val);
            clearKeyboardBuffer();
        }

        if (val == 1) {
            char* ref = NULL;
            printf("Please enter your reference word in order to decrypt your password:\n");
            printf("Otherwise, enter any other string or character to exit out if you changed your mind.\n");
            size_t siz = getline(&ref, &siz, stdin);
            ref[siz - 1] = '\0';

            if (!refWordValidCheck(ref, IDNum)) {
                printf("Incorrect reference word!\n");
                printf("Please Try Again Later.\n");
                free(ref);

                int answer;
                printOptions();

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

                DatabaseIsNotFull(&answer);
                DatabaseIsFull(&answer);
                DatabaseIsEmptyForUser(&answer, IDNum);

                menu_selection(answer, arr, IDNum, db);
                return;
            }

            RetrieveData(db, num);
            free(ref);
        }
    }

    // printf("%s\n", dest);

    int answer;
    printOptions();

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

    free(line);

    DatabaseIsNotFull(&answer);
    DatabaseIsFull(&answer);
    DatabaseIsEmptyForUser(&answer, IDNum);

    menu_selection(answer, arr, IDNum, db);

    return;
}

void UpdateCurrentPass(char* arr, int* IDNum, sqlite3* db) {
    char* line = NULL;
    size_t len = 0;
    size_t size = 0;
    const char* data = "PrintDataInDB function called";
    printf("\nUpdating one of your current passwords?\n");

    char dest[120] = "UPDATE PASSWORD SET Password = '";
    char* errMesg = 0;
    int num = 0;

    printf("Enter the Password ID number of a password that you own that you want to update:\n");
    int r = scanf("%d", &num);
    clearKeyboardBuffer();

    while (r != 1) {
        printf("Not a number between 1 and 999999! Enter again!\n");
        r = scanf("%d", &num);
        clearKeyboardBuffer();
    }

    while (num < 1 || num > 999999) {
        printf("Not a number between 1 and 999999! Enter again!\n");
        scanf("%d", &num);
        clearKeyboardBuffer();
    }

    CheckIfPassIDIsYoursBeforeUpdating(&num, IDNum, db);

    char* ref = NULL;
    printf("Please enter your reference word in order to update your password:\n");
    printf("Otherwise, enter any other string or character to exit out if you changed your mind.\n");
    size = getline(&ref, &len, stdin);
    ref[size - 1] = '\0';

    if (refWordValidCheck(ref, IDNum) == FALSE) {
        printf("Incorrect reference word!\n");
        printf("Please Try Again Later.\n");
        free(ref);

        int answer;
        printOptions();

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

        DatabaseIsNotFull(&answer);
        DatabaseIsFull(&answer);
        DatabaseIsEmptyForUser(&answer, IDNum);

        menu_selection(answer, arr, IDNum, db);
        return;
    }

    printf("Enter your new password:\n");
    size = getline(&line, &len, stdin);
    line[size - 1] = '\0';

    PASS p = NULL;
    char tmp[100];

    sprintf(tmp, "%d", num);

    Encryption(line);
    p = Password_Str(line);
    char* li = getData(p);

    strcat(line, "' WHERE PASS_ID =");
    strcat(dest, line);
    strcat(dest, tmp);
    strcat(dest, "; ");
    strcat(dest, "SELECT * from PASSWORD WHERE PASS_ID =");
    strcat(dest, tmp);

    // printf("%s\n", dest);
    printf("Encrypted and stored successfully!\n");
    printf("Encrypted password string: %s", li);
    printf("\n");
    password_destroy(&p);

    int ret = sqlite3_exec(db, dest, PrintUpdatedDataInDB, (void*)data, &errMesg);

    if (ret != SQLITE_OK) {
        printf("Error in SQL statement: %s\n", errMesg);
        sqlite3_free(errMesg);
        sqlite3_close(db);
        return;
    }

    int answer;
    printOptions();

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

    free(line);
    free(ref);

    DatabaseIsNotFull(&answer);
    DatabaseIsFull(&answer);
    DatabaseIsEmptyForUser(&answer, IDNum);

    menu_selection(answer, arr, IDNum, db);

    return;
}

void DeletePasswordContent(char* arr, int* IDNum, sqlite3* db) {
    const char* data = "PrintDataInDB function called";
    printf("\nDeleting one of your current passwords?\n");

    char dest[120] = "DELETE from PASSWORD WHERE PASS_ID =";
    char* errMesg = 0;
    int num = 0;

    printf("Enter the Password ID number of a password that you own that you want to delete:\n");
    int r = scanf("%d", &num);
    clearKeyboardBuffer();

    while (r != 1) {
        printf("Not a number between 1 and 999999! Enter again!\n");
        r = scanf("%d", &num);
        clearKeyboardBuffer();
    }

    while (num < 1 || num > 999999) {
        printf("Not a number between 1 and 999999! Enter again!\n");
        scanf("%d", &num);
        clearKeyboardBuffer();
    }

    CheckIfPassIDIsYoursBeforeUpdating(&num, IDNum, db);

    char* ref = NULL;
    printf("Please enter your reference word in order to delete your password:\n");
    printf("Otherwise, enter any other string or character to exit out if you changed your mind.\n");
    size_t siz = getline(&ref, &siz, stdin);
    ref[siz - 1] = '\0';

    if (!refWordValidCheck(ref, IDNum)) {
        printf("Incorrect reference word!\n");
        printf("Please Try Again Later.\n");
        free(ref);

        int answer;
        printOptions();

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

        DatabaseIsNotFull(&answer);
        DatabaseIsFull(&answer);
        DatabaseIsEmptyForUser(&answer, IDNum);

        menu_selection(answer, arr, IDNum, db);
        return;
    }

    char tmp[100];
    sprintf(tmp, "%d", num);

    strcat(dest, tmp);
    strcat(dest, "; ");
    strcat(dest, "SELECT * from PASSWORD WHERE PASS_ID =");
    strcat(dest, tmp);

    // printf("%s\n", dest);

    int ret = sqlite3_exec(db, dest, PrintUpdatedDataInDB, (void*)data, &errMesg);

    if (ret != SQLITE_OK) {
        printf("Error in SQL statement: %s\n", errMesg);
        sqlite3_free(errMesg);
        sqlite3_close(db);
        return;
    }

    FILE* fp = fopen("Text/Names_Storage.txt", "r");
    FILE* fp2 = fopen("Text/Password_ID.txt", "r");
    FILE* fp3 = fopen("Text/Email_Storage.txt", "r");
    FILE* fptmp = fopen("Text/delete.txt", "w");

    if (!fp || !fp2 || !fp3 || !fptmp) {
        printf("Error: Unable to open files.\n");
        exit(1);
    }

    int number;
    int lineNum = 1;
    while (fscanf(fp2, "%d", &number) != EOF) {
        if (num == number) {
            break;
        } else {
            fprintf(fptmp, "%d\n", number);
            lineNum++;
        }
    }

    while (fscanf(fp2, "%d", &number) != EOF) {
        fprintf(fptmp, "%d\n", number);
    }

    fclose(fp2);
    fclose(fptmp);

    fptmp = fopen("Text/delete.txt", "r");
    fp2 = fopen("Text/Password_ID.txt", "w");

    if (!fp2 || !fptmp) {
        printf("Error: Unable to open files.\n");
        exit(1);
    }

    while (fscanf(fptmp, "%d", &number) != EOF) {
        fprintf(fp2, "%d\n", number);
    }

    fclose(fp2);
    fclose(fptmp);

    fptmp = fopen("Text/delete.txt", "w");

    if (!fptmp) {
        printf("File failed to open\n");
        exit(1);
    }

    char* line = NULL;
    size_t len = 0;
    int lineNum2 = 1;
    while ((len = getline(&line, &len, fp)) != EOF) {
        if (lineNum2 == lineNum) {
            printf("Named removed: %s", line);
        }
        if (lineNum2 != lineNum) {
            fprintf(fptmp, "%s", line);
        }
        lineNum2++;
    }

    fclose(fp);
    fclose(fptmp);

    fptmp = fopen("Text/delete.txt", "r");
    fp = fopen("Text/Names_Storage.txt", "w");

    if (!fp || !fptmp) {
        printf("Error: Unable to open files.\n");
        exit(1);
    }

    while ((len = getline(&line, &len, fptmp)) != EOF) {
        fprintf(fp, "%s", line);
    }

    fclose(fp);
    fclose(fptmp);

    lineNum2 = 1;
    fptmp = fopen("Text/delete.txt", "w");

    if (!fptmp) {
        printf("Error: Unable to open files.\n");
        exit(1);
    }

    char* line2 = NULL;
    while ((len = getline(&line2, &len, fp3)) != EOF) {
        if (lineNum2 == lineNum) {
            printf("Email removed at line %d: %s", lineNum2, line2);
        }
        if (lineNum2 != lineNum) {
            fprintf(fptmp, "%s", line2);
        }
        lineNum2++;
    }

    fclose(fptmp);
    fclose(fp3);

    fptmp = fopen("Text/delete.txt", "r");
    fp3 = fopen("Text/Email_Storage.txt", "w");

    if (!fp3 || !fptmp) {
        printf("Error: Unable to open files.\n");
        exit(1);
    }

    while ((len = getline(&line2, &len, fptmp)) != EOF) {
        fprintf(fp3, "%s", line2);
    }

    fclose(fp3);
    fclose(fptmp);

    printf("Deleted Successfully!\n");

    int answer;
    printOptions();

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

    free(line);
    free(line2);
    free(ref);

    DatabaseIsNotFull(&answer);
    DatabaseIsFull(&answer);
    DatabaseIsEmptyForUser(&answer, IDNum);

    menu_selection(answer, arr, IDNum, db);

    return;
}