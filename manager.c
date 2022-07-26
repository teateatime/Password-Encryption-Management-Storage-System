#define  _GNU_SOURCE

#include "my_string.h"
#include "password.h"
#include "manager.h"
#include "support.h"

void printOptions(void);

void TestPassword(char* arr, int* IDNum, sqlite3* db) {
    char* line = NULL;
    size_t len = 0;

    printf("\nNeed to Measure Your Password Strength?\n");
    printf("Enter your password:\n");
    size_t size = getline(&line, &len, stdin);
    line[size - 1] = '\0';

    PASS pass = NULL;
    pass = Password_Str(line);
    pass = Password_Calc_Strength2(pass, arr);
    Password_Print_Strength(pass);

    password_destroy(&pass);
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
            printf("Entering generatePassword()...\n");
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

    return;
}

void generatePassword(char* arr, int* IDNum, sqlite3* db) {
    char* line = NULL;
    size_t len = 0;

    FILE* fp2 = NULL;

    fp2 = fopen("Text/Names_Storage.txt", "a+");

    if (!fp2) {
        printf("Error: Unable to open file.\n");
        exit(1);
    }

    printf("\nPlease enter your name: (first_name) (last_name)\n");
    size_t size = getline(&line, &len, stdin);
    line[size - 1] = '\0';

    fprintf(fp2, "%s\n", line);

    fclose(fp2);

    FILE* fp3 = NULL;

    fp3 = fopen("Text/Email_Storage.txt", "a+");

    if (!fp3) {
        printf("Error: Unable to open file.\n");
        exit(1);
    }

    char* email = NULL;

    printf("Please enter the email address you used to login: (Ex: email_address123@gmail.com)\n");

    size = getline(&email, &len, stdin);
    email[size - 1] = '\0';

    while (1) {
        int lineNum = 1;
        int numVal = 0;
        FILE* fpID = fopen("Text/ID_Storage.txt", "r");
        FILE* fpmail = fopen("Text/Mail_Storage.txt", "r");

        if (!fpmail || !fpID) {
            printf("Files cannot open!\n");
            exit(1);
        }

        while (fscanf(fpID, "%d", &numVal) != EOF) {
            if (*IDNum == numVal) {
                break;
            } else {
                lineNum++;
            }
        }

        size_t len = 0;
        char* mail = NULL;
        int lineNum2 = 1;
        while ((len = getline(&mail, &len, fpmail)) != EOF) {
            mail[len - 1] = '\0';
            if (lineNum2 == lineNum) {
                break;
            } else {
                lineNum2++;
            }
        }

        fclose(fpID);
        fclose(fpmail);

        MY_STRING s1 = my_string_init_c_string(email);
        MY_STRING s2 = my_string_init_c_string(mail);

        if (my_string_equal(s1, s2)) {
            free(mail);
            my_string_destroy(&s1);
            my_string_destroy(&s2);
            break;
        } else {
            printf("This is not the email that you login with! Try Again!\n");
            free(mail);
            my_string_destroy(&s1);
            my_string_destroy(&s2);
            size_t size = 0;
            free(email);
            size = getline(&email, &size, stdin);
            email[size - 1] = '\0';
        }
    }

    char* ref = NULL;
    printf("Please enter your reference word in order to generate your password:\n");
    size = getline(&ref, &len, stdin);
    ref[size - 1] = '\0';

    if (refWordValidCheck(ref, IDNum) == FALSE) {
        printf("Incorrect reference word!\n");
        printf("Please Try Again Later.\n");
        free(ref);

        int ans;
        printOptions();

        int nc = scanf("%d", &ans);
        clearKeyboardBuffer();
        while (nc != 1) {
            printf("Not a number between 1-8! Enter again!\n");
            nc = scanf("%d", &ans);
            clearKeyboardBuffer();
        }

        while (ans < 1 || ans > 8) {
            printf("Not a number between 1-8! Enter again!\n");
            scanf("%d", &ans);
            clearKeyboardBuffer();
        }

        free(line);
        free(email);
        fclose(fp3);

        DatabaseIsNotFull(&ans);
        DatabaseIsFull(&ans);
        DatabaseIsEmptyForUser(&ans, IDNum);

        while (ans > 0) {
            if (ans == 1) {
                printf("Entering StorePassword()...\n");
                StorePassword(arr, IDNum, db);
                return;
            } else if (ans == 2) {
                printf("Entering TestPassword()...\n");
                TestPassword(arr, IDNum, db);
                return;
            } else if (ans == 3) {
                printf("Entering generatePass word()...\n");
                generatePassword(arr, IDNum, db);
                return;
            } else if (ans == 4) {
                printf("Entering ScramblePassword()...\n");
                ScramblePassword(arr, IDNum, db);
                return;
            } else if (ans == 5) {
                printf("Entering SearchPassword()...\n");
                SearchPassword(arr, IDNum, db);
                return;
            } else if (ans == 6) {
                printf("Entering UpdateCurrentPass()...\n");
                UpdateCurrentPass(arr, IDNum, db);
                return;
            } else if (ans == 7) {
                printf("Entering DeletePasswordContent()...\n");
                DeletePasswordContent(arr, IDNum, db);
                return;
            } else {
                printf("Returning to Main Menu...\n\n");
                return;
            }
        }
    }

    free(ref);

    fprintf(fp3, "%s\n", email);

    fclose(fp3);

    char dest[100] = "INSERT or IGNORE INTO PASSWORD VALUES(";
    char tmp[100];
    char* errMesg = 0;

    int rNum = rand() % 999999 + 1;

    CreateNewPassIDAndWriteIt(&rNum);

    sprintf(tmp, "%d", *IDNum);

    strcat(dest, tmp);
    strcat(dest, ", '");
    sprintf(tmp, "%d", rNum);
    strcat(dest, tmp);
    strcat(dest, "', '");
    strcat(dest, line);
    strcat(dest, "', '");
    strcat(dest, email);
    strcat(dest, "', '");

    PASS p = NULL;
    printf("Generating a new password...\n");

    p = Update_Password(arr);
    Password_Print_Strength(p);

    char* s = getData(p);
    Encryption(s);
    strcat(dest, s);
    strcat(dest, "');");

    printf("%s\n", dest);

    int ret = sqlite3_exec(db, dest, 0, 0, &errMesg);

    if (ret != SQLITE_OK) {
        printf("Error in SQL statement: %s\n", errMesg);
        sqlite3_free(errMesg);
        sqlite3_close(db);
        return;
    }

    printf("Data inserted in PASSWORD table successfully\n");
    printf("Encrypted Password String: %s\n", s);
    printf("Password ID for this pass is: %d\n", rNum);

    password_destroy(&p);
    free(line);
    free(email);

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
            printf("Entering generatePassword()...\n");
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

    return;
}

void StorePassword(char* arr, int* IDNum, sqlite3* db) {
    char* line = NULL;
    size_t len = 0;

    char* line2 = NULL;
    size_t len2 = 0;

    FILE* fp2 = NULL;

    fp2 = fopen("Text/Names_Storage.txt", "a+");

    if (!fp2) {
        printf("Error: Unable to open file.\n");
        exit(1);
    }

    printf("\nNeed to store your password?\n");

    printf("Please enter your name: (first_name) (last_name)\n");
    size_t size = getline(&line, &len, stdin);
    line[size - 1] = '\0';

    fprintf(fp2, "%s\n", line);

    fclose(fp2);

    FILE* fp3 = NULL;

    fp3 = fopen("Text/Email_Storage.txt", "a+");

    if (!fp3) {
        printf("Error: Unable to open file.\n");
        exit(1);
    }

    char* email = NULL;

    printf("Please enter the email address you used to login: (Ex: email_address123@gmail.com)\n");

    size = getline(&email, &len, stdin);
    email[size - 1] = '\0';

    while (1) {
        int lineNum = 1;
        int numVal = 0;
        FILE* fpID = fopen("Text/ID_Storage.txt", "r");
        FILE* fpmail = fopen("Text/Mail_Storage.txt", "r");

        if (!fpmail || !fpID) {
            printf("Files cannot open!\n");
            exit(1);
        }

        while (fscanf(fpID, "%d", &numVal) != EOF) {
            if (*IDNum == numVal) {
                break;
            } else {
                lineNum++;
            }
        }

        size_t len = 0;
        char* mail = NULL;
        int lineNum2 = 1;
        while ((len = getline(&mail, &len, fpmail)) != EOF) {
            mail[len - 1] = '\0';
            if (lineNum2 == lineNum) {
                break;
            } else {
                lineNum2++;
            }
        }

        fclose(fpID);
        fclose(fpmail);

        MY_STRING s1 = my_string_init_c_string(email);
        MY_STRING s2 = my_string_init_c_string(mail);

        if (my_string_equal(s1, s2)) {
            free(mail);
            my_string_destroy(&s1);
            my_string_destroy(&s2);
            break;
        } else {
            printf("This is not the email that you login with! Try Again!\n");
            free(mail);
            my_string_destroy(&s1);
            my_string_destroy(&s2);
            size_t size = 0;
            free(email);
            size = getline(&email, &size, stdin);
            email[size - 1] = '\0';
        }
    }

    char* ref = NULL;
    printf("Please enter your reference word in order to store your password:\n");
    size = getline(&ref, &len, stdin);
    ref[size - 1] = '\0';

    if (refWordValidCheck(ref, IDNum) == FALSE) {
        printf("Incorrect reference word!\n");
        printf("Please Try Again Later.\n");
        free(ref);

        int ans;
        printOptions();

        int nc = scanf("%d", &ans);
        clearKeyboardBuffer();
        while (nc != 1) {
            printf("Not a number between 1-8! Enter again!\n");
            nc = scanf("%d", &ans);
            clearKeyboardBuffer();
        }

        while (ans < 1 || ans > 8) {
            printf("Not a number between 1-8! Enter again!\n");
            scanf("%d", &ans);
            clearKeyboardBuffer();
        }

        free(line);
        free(email);

        fclose(fp3);

        DatabaseIsNotFull(&ans);
        DatabaseIsFull(&ans);
        DatabaseIsEmptyForUser(&ans, IDNum);

        while (ans > 0) {
            if (ans == 1) {
                printf("Entering StorePassword()...\n");
                StorePassword(arr, IDNum, db);
                return;
            } else if (ans == 2) {
                printf("Entering TestPassword()...\n");
                TestPassword(arr, IDNum, db);
                return;
            } else if (ans == 3) {
                printf("Entering generatePass word()...\n");
                generatePassword(arr, IDNum, db);
                return;
            } else if (ans == 4) {
                printf("Entering ScramblePassword()...\n");
                ScramblePassword(arr, IDNum, db);
                return;
            } else if (ans == 5) {
                printf("Entering SearchPassword()...\n");
                SearchPassword(arr, IDNum, db);
                return;
            } else if (ans == 6) {
                printf("Entering UpdateCurrentPass()...\n");
                UpdateCurrentPass(arr, IDNum, db);
                return;
            } else if (ans == 7) {
                printf("Entering DeletePasswordContent()...\n");
                DeletePasswordContent(arr, IDNum, db);
                return;
            } else {
                printf("Returning to Main Menu...\n\n");
                return;
            }
        }
    }

    free(ref);

    fprintf(fp3, "%s\n", email);

    fclose(fp3);

    char dest[100] = "INSERT or IGNORE INTO PASSWORD VALUES(";
    char* errMesg = 0;
    char tmp[100];

    int rNum = rand() % 999999 + 1;

    CreateNewPassIDAndWriteIt(&rNum);

    sprintf(tmp, "%d", *IDNum);

    strcat(dest, tmp);
    strcat(dest, ", '");
    sprintf(tmp, "%d", rNum);
    strcat(dest, tmp);
    strcat(dest, "', '");
    strcat(dest, line);
    strcat(dest, "', '");
    strcat(dest, email);
    strcat(dest, "', '");

    printf("Enter your password:\n");
    size_t size2 = getline(&line2, &len2, stdin);
    line2[size2 - 1] = '\0';

    PASS pass2 = Password_Str(line2);

    while (CheckSpaces(pass2)) {
        printf("Password has a space, it should not have that!\n");
        printf("Please enter another password.\n");
        len = getline(&line2, &len, stdin);
        line2[len - 1] = '\0';
        password_destroy(&pass2);
        pass2 = Password_Str(line2);
    }

    password_destroy(&pass2);

    PASS pass = NULL;
    pass = Password_Str(line2);
    Password_Calc_Strength(pass, line2);

    char* s = getData(pass);
    if (getStrength(pass) < 100) {
        printf("Your password strength is kind of weak, its strength is at %d.\n", getStrength(pass));
        printf("A decent password would normally have a strength of at least 100.\n");
        printf("Would you like us to create and generate a new password for you?\n");
        printf("Enter (1) if yes\n");
        printf("Enter (2) if no\n");

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
            printf("Generating a new password...\n");
            password_destroy(&pass);
            pass = Update_Password(arr);
            Password_Print_Strength(pass);

            s = getData(pass);
        }
    }

    Encryption(s);

    strcat(dest, s);
    strcat(dest, "');");

    printf("%s\n", dest);

    int ret = sqlite3_exec(db, dest, 0, 0, &errMesg);

    if (ret != SQLITE_OK) {
        printf("Error in SQL statement: %s\n", errMesg);
        sqlite3_free(errMesg);
        sqlite3_close(db);
        return;
    }

    printf("Data inserted in PASSWORD table successfully\n");
    printf("Encrypted Password String: %s\n", s);
    printf("Password ID for this pass is: %d\n", rNum);

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

    password_destroy(&pass);
    free(line);
    free(line2);
    free(email);

    DatabaseIsNotFull(&answer);
    DatabaseIsFull(&answer);
    DatabaseIsEmptyForUser(&answer, IDNum);

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
            printf("Entering generatePassword()...\n");
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

    return;
}

void ScramblePassword(char* arr, int* IDNum, sqlite3* db) {
    char* line = NULL;
    size_t len = 0;

    char* line2 = NULL;
    size_t len2 = 0;

    FILE* fp2 = NULL;

    fp2 = fopen("Text/Names_Storage.txt", "a+");

    if (!fp2) {
        printf("Error: Unable to open file.\n");
        exit(1);
    }

    printf("\nPlease enter your name: (first_name) (last_name)\n");
    size_t size = getline(&line, &len, stdin);
    line[size - 1] = '\0';

    fprintf(fp2, "%s\n", line);

    fclose(fp2);

    FILE* fp3 = NULL;

    fp3 = fopen("Text/Email_Storage.txt", "a+");

    if (!fp3) {
        printf("Error: Unable to open file.\n");
        exit(1);
    }

    char* email = NULL;

    printf("Please enter the email address you used to login: (Ex: email_address123@gmail.com)\n");

    size = getline(&email, &len, stdin);
    email[size - 1] = '\0';

    while (1) {
        int lineNum = 1;
        int numVal = 0;
        FILE* fpID = fopen("Text/ID_Storage.txt", "r");
        FILE* fpmail = fopen("Text/Mail_Storage.txt", "r");

        if (!fpmail || !fpID) {
            printf("Files cannot open!\n");
            exit(1);
        }

        while (fscanf(fpID, "%d", &numVal) != EOF) {
            if (*IDNum == numVal) {
                break;
            } else {
                lineNum++;
            }
        }

        size_t len = 0;
        char* mail = NULL;
        int lineNum2 = 1;
        while ((len = getline(&mail, &len, fpmail)) != EOF) {
            mail[len - 1] = '\0';
            if (lineNum2 == lineNum) {
                break;
            } else {
                lineNum2++;
            }
        }

        fclose(fpID);
        fclose(fpmail);

        MY_STRING s1 = my_string_init_c_string(email);
        MY_STRING s2 = my_string_init_c_string(mail);

        if (my_string_equal(s1, s2)) {
            free(mail);
            my_string_destroy(&s1);
            my_string_destroy(&s2);
            break;
        } else {
            printf("This is not the email that you login with! Try Again!\n");
            free(mail);
            my_string_destroy(&s1);
            my_string_destroy(&s2);
            size_t size = 0;
            free(email);
            size = getline(&email, &size, stdin);
            email[size - 1] = '\0';
        }
    }

    char* ref = NULL;
    printf("Please enter your reference word in order to generate your password:\n");
    size = getline(&ref, &len, stdin);
    ref[size - 1] = '\0';

    if (refWordValidCheck(ref, IDNum) == FALSE) {
        printf("Incorrect reference word!\n");
        printf("Please Try Again Later.\n");
        free(ref);

        int ans;
        printOptions();

        int nc = scanf("%d", &ans);
        clearKeyboardBuffer();
        while (nc != 1) {
            printf("Not a number between 1-8! Enter again!\n");
            nc = scanf("%d", &ans);
            clearKeyboardBuffer();
        }

        while (ans < 1 || ans > 8) {
            printf("Not a number between 1-8! Enter again!\n");
            scanf("%d", &ans);
            clearKeyboardBuffer();
        }

        free(line);
        free(email);

        fclose(fp3);

        DatabaseIsNotFull(&ans);
        DatabaseIsFull(&ans);
        DatabaseIsEmptyForUser(&ans, IDNum);

        while (ans > 0) {
            if (ans == 1) {
                printf("Entering StorePassword()...\n");
                StorePassword(arr, IDNum, db);
                return;
            } else if (ans == 2) {
                printf("Entering TestPassword()...\n");
                TestPassword(arr, IDNum, db);
                return;
            } else if (ans == 3) {
                printf("Entering generatePass word()...\n");
                generatePassword(arr, IDNum, db);
                return;
            } else if (ans == 4) {
                printf("Entering ScramblePassword()...\n");
                ScramblePassword(arr, IDNum, db);
                return;
            } else if (ans == 5) {
                printf("Entering SearchPassword()...\n");
                SearchPassword(arr, IDNum, db);
                return;
            } else if (ans == 6) {
                printf("Entering UpdateCurrentPass()...\n");
                UpdateCurrentPass(arr, IDNum, db);
                return;
            } else if (ans == 7) {
                printf("Entering DeletePasswordContent()...\n");
                DeletePasswordContent(arr, IDNum, db);
                return;
            } else {
                printf("Returning to Main Menu...\n\n");
                return;
            }
        }
    }

    free(ref);

    fprintf(fp3, "%s\n", email);

    fclose(fp3);

    char dest[100] = "INSERT or IGNORE INTO PASSWORD VALUES(";
    char tmp[100];
    char* errMesg = 0;

    int rNum = rand() % 999999 + 1;

    CreateNewPassIDAndWriteIt(&rNum);

    sprintf(tmp, "%d", *IDNum);

    strcat(dest, tmp);
    strcat(dest, ", '");
    sprintf(tmp, "%d", rNum);
    strcat(dest, tmp);
    strcat(dest, "', '");
    strcat(dest, line);
    strcat(dest, "', '");
    strcat(dest, email);
    strcat(dest, "', '");

    printf("Enter your password:\n");
    size_t size2 = getline(&line2, &len2, stdin);
    line2[size2 - 1] = '\0';

    PASS pass2 = Password_Str(line2);

    while (CheckSpaces(pass2)) {
        printf("Password has a space, it should not have that!\n");
        printf("Please enter another password.\n");
        len = getline(&line2, &len, stdin);
        line2[len - 1] = '\0';
        password_destroy(&pass2);
        pass2 = Password_Str(line2);
    }

    password_destroy(&pass2);

    PASS p = Update_Password_By_Scramble(arr, line2);

    char* t = getData(p);

    printf("Scrambled Password for you to use and store: %s\n", t);

    Encryption(t);

    strcat(dest, t);
    strcat(dest, "');");

    printf("%s\n", dest);

    int ret = sqlite3_exec(db, dest, 0, 0, &errMesg);

    if (ret != SQLITE_OK) {
        printf("Error in SQL statement: %s\n", errMesg);
        sqlite3_free(errMesg);
        sqlite3_close(db);
        return;
    }

    printf("Data inserted in PASSWORD table successfully\n");
    printf("Encrypted Password String: %s\n", t);
    printf("Password ID for this pass is: %d\n", rNum);

    password_destroy(&p);
    free(line);
    free(line2);
    free(email);

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
            printf("Entering generatePassword()...\n");
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

    return;
}

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
            printf("Please enter your Password ID number:\n");
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

            char* ref = NULL;
            printf("Please enter your reference word in order to decrypt your password:\n");
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
            printf("Please enter your Password ID number:\n");
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

            char* ref = NULL;
            printf("Please enter your reference word in order to decrypt your password:\n");
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

            RetrieveData(db, num);
            free(ref);
        }
    } else if (ans == 3) {
        printf("Please enter your Password ID number:\n");
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
            printf("Entering generatePassword()...\n");
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

    printf("Enter the ID number of the password stored that you want to update:\n");
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

    printf("Enter your new password:\n");
    size = getline(&line, &len, stdin);
    line[size - 1] = '\0';

    PASS p = NULL;
    char tmp[100];

    sprintf(tmp, "%d", num);

    Encryption(line);
    p = Password_Str(line);
    char* li = getData(p);
    password_destroy(&p);

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

    return;
}

void DeletePasswordContent(char* arr, int* IDNum, sqlite3* db) {
    const char* data = "PrintDataInDB function called";
    printf("\nDeleting one of your current passwords?\n");

    char dest[120] = "DELETE from PASSWORD WHERE PASS_ID =";
    char* errMesg = 0;
    int num = 0;

    printf("Enter your ID number of the password stored that you want to delete:\n");
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
            printf("Entering generatePassword()...\n");
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

    return;
}

void printOptions(void) {
    printf("\n");
    printf("Press (1) if you want to store the password you want\n");
    printf("Press (2) if you want to test your password strength again\n");
    printf("Press (3) if you want us to generate a password for you and store it for safe keeping\n");
    printf("Press (4) if you want to scramble your current password to create a new pass and store that for safe keeping\n");
    printf("Press (5) if you want to search your password(s) inside the database\n");
    printf("Press (6) if you want to update your password(s) inside the database\n");
    printf("Press (7) if you want to delete your password(s) inside the database\n");
    printf("Press (8) to return to main\n");
}
