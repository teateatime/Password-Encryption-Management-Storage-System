#define _GNU_SOURCE

#include "my_string.h"
#include "password.h"
#include "manager.h"

int PEMS(int* IDNum);

int main(int argc, char* argv[]) {
    FILE* fplogin;
    char* stri = NULL;
    char* stri2 = NULL;
    char* stri3 = NULL;
    char* stri4 = NULL;
    int rNum = 0;
    int IDNum = 0;
    srand(time(NULL));

    fplogin = fopen("Text/LoginStorage.txt", "r");

    if (!fplogin) {
        printf("Error: Failed to open file!\n");
        exit(1);
    }

    Boolean b = 0;

    fseek(fplogin, 0, SEEK_END);
    int size_of_file = ftell(fplogin);

    if (size_of_file == 0) {
        b = 1;
    }

    printf("Is this your first time using PEMS?\n");
    printf("Press (1) if it is\n");
    printf("Press (2) to login\n");

    int userInput = 0;
    int r = scanf("%d", &userInput);
    clearKeyboardBuffer();

    while (r != 1) {
        printf("Not a number between 1-2! Enter again!\n");
        r = scanf("%d", &userInput);
        clearKeyboardBuffer();
    }

    while (userInput < 1 || userInput > 2) {
        printf("Not a number between 1-2! Enter again!\n");
        scanf("%d", &userInput);
        clearKeyboardBuffer();
    }

    while (userInput == 2 && b == 1) {
        printf("Account has not been created yet, please enter 1 instead.\n");
        scanf("%d", &userInput);
        clearKeyboardBuffer();

        while (r != 1) {
            printf("Not a number between 1-2! Enter again!\n");
            r = scanf("%d", &userInput);
            clearKeyboardBuffer();
        }

        while (userInput < 1 || userInput > 2) {
            printf("Not a number between 1-2! Enter again!\n");
            scanf("%d", &userInput);
            clearKeyboardBuffer();
        }
    }

    fclose(fplogin);

    size_t l;
    if (userInput == 1) {
        char* ref = NULL;
        fplogin = fopen("Text/LoginStorage.txt", "a+");
        FILE* fpID = fopen("Text/ID_Storage.txt", "a+");
        FILE *fpref = fopen("Text/reference.txt", "a+");
        FILE *fpmail = fopen("Text/Mail_Storage.txt", "a+");

        if (!fplogin || !fpID || !fpref || !fpmail) {
            printf("Error: Failed to open files!\n");
            exit(1);
        }
        printf("Enter your first name:\n");
        l = getline(&stri, &l, stdin);
        stri[l - 1] = '\0';
        fprintf(fplogin, "%s ", stri);
        printf("Enter your last name:\n");
        l = getline(&stri2, &l, stdin);
        stri2[l - 1] = '\0';
        fprintf(fplogin, "%s ", stri2);
        printf("Enter your email address:\n");
        l = getline(&stri3, &l, stdin);
        stri3[l - 1] = '\0';

        emailValid(stri3);

        printf("Enter your password:\n");
        l = getline(&stri4, &l, stdin);
        stri4[l - 1] = '\0';
        printf("Enter your reference word so you can access/manipulate your passwords later on:\n");
        l = getline(&ref, &l, stdin);
        ref[l - 1] = '\0';

        fprintf(fplogin, "%s ", stri4);
        fprintf(fplogin, "%s\n", stri3);
        fprintf(fpmail, "%s\n", stri3);

        rNum = rand() % 999999 + 1;

        IDChecker3(&rNum);

        fprintf(fpID, "%d\n", rNum);
        fprintf(fpref, "%s\n", ref);

        free(ref);
        fclose(fpref);
        fclose(fplogin);
        fclose(fpID);
        fclose(fpmail);
    } else if (userInput == 2) {
        fplogin = fopen("Text/LoginStorage.txt", "r");

        if (!fplogin) {
            printf("Error: Failed to open file!\n");
            exit(1);
        }

        while (1) {
            printf("Please enter your first and last name and your password to continue\n");
            printf("Enter your first name:\n");
            l = getline(&stri, &l, stdin);
            stri[l - 1] = '\0';
            printf("Enter your last name:\n");
            l = getline(&stri2, &l, stdin);
            stri2[l - 1] = '\0';
            printf("Enter your email address:\n");
            l = getline(&stri3, &l, stdin);
            stri3[l - 1] = '\0';
            printf("Enter your password:\n");
            l = getline(&stri4, &l, stdin);
            stri4[l - 1] = '\0';

            int truth_counter = 0;

            strcat(stri, " ");
            strcat(stri2, " ");
            strcat(stri4, " ");
            strcat(stri, stri2);
            strcat(stri, stri4);
            strcat(stri, stri3);

            if (NameChecker2(stri)) {
                truth_counter++;
            }

            if (truth_counter == 1) {
                FILE* fplog = fopen("Text/LoginStorage.txt", "r");

                if (!fplog) {
                    printf("File failed to open.\n");
                    exit(1);
                }

                MY_STRING s1 = my_string_init_c_string(stri);
                MY_STRING s2 = NULL;
                char* strfp = NULL;
                int lineNum = 1;
                size_t lenSize = 0;
                while ((lenSize = getline(&strfp, &lenSize, fplog)) != EOF) {
                    strfp[lenSize - 1] = '\0';
                    s2 = my_string_init_c_string(strfp);
                    if (my_string_equal(s1, s2)) {
                        my_string_destroy(&s2);
                        break;
                    } else {
                        my_string_destroy(&s2);
                        lineNum++;
                    }
                }

                fclose(fplog);

                FILE* fpID = fopen("Text/ID_Storage.txt", "r");

                if (!fpID) {
                    printf("File failed to open.\n");
                    exit(1);
                }

                int number;
                int lineNum2 = 1;
                while (fscanf(fpID, "%d", &number) != EOF) {
                    if (lineNum2 == lineNum) {
                        break;
                    } else {
                        lineNum2++;
                    }
                }

                free(stri);
                free(stri2);
                free(stri3);
                free(stri4);

                free(strfp);
                my_string_destroy(&s1);
                fclose(fplogin);
                fclose(fpID);

                IDNum = number;

                printf("Your Account ID# is %d\n", IDNum);

                printf("Logging in...\n");

                PEMS(&IDNum);
                break;
            } else {
                printf("Incorrect Info! Try Again!\n");
            }
        }
        return 0;
    }

    fplogin = fopen("Text/LoginStorage.txt", "r");

    if (!fplogin) {
        printf("Error: Failed to open file!\n");
        exit(1);
    }

    free(stri);
    free(stri3);
    free(stri2);
    free(stri4);
    char* stri5 = NULL;
    char* stri6 = NULL;
    char* stri7 = NULL;
    char* stri8 = NULL;

    printf("\nAccount Created!\n");
    while (1) {
        printf("Please enter your first and last name and your password to continue\n");
        printf("Enter your first name:\n");
        l = getline(&stri7, &l, stdin);
        stri7[l - 1] = '\0';
        printf("Enter your last name:\n");
        l = getline(&stri8, &l, stdin);
        stri8[l - 1] = '\0';
        printf("Enter your email address:\n");
        l = getline(&stri5, &l, stdin);
        stri5[l - 1] = '\0';

        printf("Enter your password:\n");
        l = getline(&stri6, &l, stdin);
        stri6[l - 1] = '\0';

        int truth_counter = 0;

        strcat(stri7, " ");
        strcat(stri8, " ");
        strcat(stri6, " ");
        strcat(stri7, stri8);
        strcat(stri7, stri6);
        strcat(stri7, stri5);

        if (NameChecker2(stri7)) {
            truth_counter++;
        }

        if (truth_counter == 1) {
            FILE* fplog = fopen("Text/LoginStorage.txt", "r");

            if (!fplog) {
                printf("File failed to open.\n");
                exit(1);
            }

            MY_STRING s1 = my_string_init_c_string(stri7);
            MY_STRING s2 = NULL;
            char* strfp = NULL;
            int lineNum = 1;
            size_t lenSize = 0;
            while ((lenSize = getline(&strfp, &lenSize, fplog)) != EOF) {
                strfp[lenSize - 1] = '\0';
                s2 = my_string_init_c_string(strfp);
                if (my_string_equal(s1, s2)) {
                    my_string_destroy(&s2);
                    break;
                } else {
                    my_string_destroy(&s2);
                    lineNum++;
                }
            }

            free(strfp);
            my_string_destroy(&s1);
            fclose(fplog);

            FILE* fpID = fopen("Text/ID_Storage.txt", "r");

            if (!fpID) {
                printf("File failed to open.\n");
                exit(1);
            }

            int number;
            int lineNum2 = 1;
            while (fscanf(fpID, "%d", &number) != EOF) {
                if (lineNum2 == lineNum) {
                    break;
                } else {
                    lineNum2++;
                }
            }

            IDNum = number;

            printf("Logging in...\n");

            free(stri7);
            free(stri8);
            free(stri5);
            free(stri6);

            fclose(fpID);
            fclose(fplogin);

            printf("Your Account ID# is %d\n", IDNum);
            PEMS(&IDNum);
            break;
        } else {
            printf("Incorrect Info! Try Again!\n");
        }
    }

    return 0;
}

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
    printf("Welcome to Password Encryption Manangement System!!!\n\n");

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

    answerChecker2(&answer);
    answerChecker3(&answer, IDNum);

    while (answer > 0 && answer < 9) {
        if (answer == 1) {
            printf("Entering StorePassword()...\n");
            StorePassword(arr, IDNum, db);
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
            answerChecker3(&answer, IDNum);
        } else if (answer == 2) {
            printf("Entering TestPassword()...\n");
            TestPassword(arr, IDNum, db);
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
            answerChecker3(&answer, IDNum);
        } else if (answer == 3) {
            printf("Entering generatePassword()...\n");
            generatePassword(arr, IDNum, db);
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
            answerChecker3(&answer, IDNum);
        } else if (answer == 4) {
            printf("Entering ScramblePassword()...\n");
            ScramblePassword(arr, IDNum, db);
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
            answerChecker3(&answer, IDNum);
        } else if (answer == 5) {
            printf("Entering SearchPassword()...\n");
            SearchPassword(arr, IDNum, db);
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
            answerChecker3(&answer, IDNum);
        } else if (answer == 6) {
            printf("Entering UpdateCurrentPass()...\n");
            UpdateCurrentPass(arr, IDNum, db);
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
            answerChecker3(&answer, IDNum);
        } else if (answer == 7) {
            printf("Entering DeletePasswordContent()...\n");
            DeletePasswordContent(arr, IDNum, db);
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
            answerChecker3(&answer, IDNum);
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
