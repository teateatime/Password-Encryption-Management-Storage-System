#define _GNU_SOURCE

#include "my_string.h"
#include "password.h"
#include "pems.h"
#include "login.h"
#include "manager.h"
#include "support.h"
#include "utility.h"

void start_app(void) {
    int rNum = 0;
    int IDNum = 0;
    srand(time(NULL));

    FILE* fplogin;
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

    CheckIfTwoUserInputValid(r, &userInput);

    while (userInput == 2 && b == 1) {
        printf("Account has not been created yet, please enter 1 instead.\n");
        scanf("%d", &userInput);
        clearKeyboardBuffer();

        CheckIfTwoUserInputValid(r, &userInput);
    }

    fclose(fplogin);
    checkLoginInput(&userInput, &rNum, &IDNum);

    char* stri5 = NULL;
    char* stri6 = NULL;
    char* stri7 = NULL;
    char* stri8 = NULL;

    printf("\nAccount Created!\n");
    printf("Do you want to login or exit out of the program?\n");
    printf("Enter (1) to login\n");
    printf("Enter (2) to exit out\n");

    r = scanf("%d", &userInput);
    clearKeyboardBuffer();

    CheckIfTwoUserInputValid(r, &userInput);

    if (userInput == 1) {
        while (1) {
            size_t l = 0;
            printf("Please enter your first and last name and your password to continue\n");
            printf("Enter your first name: (Case Sensitive)\n");
            l = getline(&stri7, &l, stdin);
            stri7[l - 1] = '\0';
            printf("Enter your last name: (Case Sensitive)\n");
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

            if (LoginChecker(stri7)) {
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
                // fclose(fplogin);

                printf("Your Account ID# is %d\n", IDNum);
                PEMS(&IDNum);
                break;
            } else {
                free(stri7);
                free(stri8);
                free(stri5);
                free(stri6);
                printf("Incorrect Info! Try Again!\n");
            }
        }
    } else {
        printf("Exiting out program...\nHave a nice day.\n");
        return;
    }
}

void checkLoginInput(int* userInput, int* rNum, int* IDNum) {
    size_t l;
    char* stri = NULL;
    char* stri2 = NULL;
    char* stri3 = NULL;
    char* stri4 = NULL;

    if (*userInput == 1) {
        char* ref = NULL;
        FILE* fplogin = fopen("Text/LoginStorage.txt", "a+");
        FILE* fpID = fopen("Text/ID_Storage.txt", "a+");
        FILE *fpref = fopen("Text/reference.txt", "a+");
        FILE *fpmail = fopen("Text/Mail_Storage.txt", "a+");

        if (!fplogin || !fpID || !fpref || !fpmail) {
            printf("Error: Failed to open files!\n");
            exit(1);
        }
        printf("Enter your first name: (Case Sensitive)\n");
        l = getline(&stri, &l, stdin);
        stri[l - 1] = '\0';
        fprintf(fplogin, "%s ", stri);
        printf("Enter your last name: (Case Sensitive)\n");
        l = getline(&stri2, &l, stdin);
        stri2[l - 1] = '\0';
        fprintf(fplogin, "%s ", stri2);
        printf("Enter your email address:\n");
        l = getline(&stri3, &l, stdin);
        stri3[l - 1] = '\0';

        CheckIfEmailExistsAlready(stri3);

        while (!is_valid(stri3)) {
            printf("Not a valid email address!\nPlease enter again:\n");
            l = getline(&stri3, &l, stdin);
            stri3[l - 1] = '\0';
            CheckIfEmailExistsAlready(stri3);
        }

        printf("Enter your password:\n");
        l = getline(&stri4, &l, stdin);
        stri4[l - 1] = '\0';

        while (!password_validate(stri4)) {
            printf("Password is not valid. Its missing either an upper/lowercase letter, number or special character.\n");
            printf("Please enter your password again:\n");
            l = getline(&stri4, &l, stdin);
            stri4[l - 1] = '\0';
        }

        PASS p = Password_Str(stri4);

        while (CheckSpaces(p)) {
            printf("Password has a space, it should not have that!\n");
            printf("Please enter another password.\n");
            l = getline(&stri4, &l, stdin);
            stri4[l - 1] = '\0';
            password_destroy(&p);
            p = Password_Str(stri4);
        }

        printf("Enter your reference word so you can access/manipulate your passwords later on:\n");
        l = getline(&ref, &l, stdin);
        ref[l - 1] = '\0';

        fprintf(fplogin, "%s ", stri4);
        fprintf(fplogin, "%s\n", stri3);
        fprintf(fpmail, "%s\n", stri3);

        *rNum = rand() % 999999 + 1;

        CreateNewAccID(rNum);

        fprintf(fpID, "%d\n", *rNum);
        fprintf(fpref, "%s\n", ref);

        password_destroy(&p);
        free(stri);
        free(stri2);
        free(stri3);
        free(stri4);
        free(ref);
        fclose(fpref);
        fclose(fplogin);
        fclose(fpID);
        fclose(fpmail);
    } else if (*userInput == 2) {
        FILE* fplogin = fopen("Text/LoginStorage.txt", "r");

        if (!fplogin) {
            printf("Error: Failed to open file!\n");
            exit(1);
        }

        while (1) {
            printf("Please enter your first and last name and your password to continue\n");
            printf("Enter your first name: (Case Sensitive)\n");
            l = getline(&stri, &l, stdin);
            stri[l - 1] = '\0';
            printf("Enter your last name: (Case Sensitive)\n");
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

            if (LoginChecker(stri)) {
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

                *IDNum = number;

                printf("Your Account ID# is %d\n", *IDNum);

                printf("Logging in...\n");

                PEMS(IDNum);
                exit(0);
                break;
            } else {
                printf("Incorrect Info! Try Again!\n");
            }
        }
        return;
    }
    return;
}