#define  _GNU_SOURCE

#include "my_string.h"
#include "password.h"
#include "store.h"
#include "support.h"
#include "utility.h"
#include "manager.h"

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

    menu_selection(answer, arr, IDNum, db);

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
    printf("Otherwise, enter any other string or character to exit out if you changed your mind.\n");
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

        menu_selection(ans, arr, IDNum, db);
        return;
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

    menu_selection(answer, arr, IDNum, db);
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
    printf("Otherwise, enter any other string or character to exit out if you changed your mind.\n");
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

        menu_selection(ans, arr, IDNum, db);
        return;
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

    menu_selection(answer, arr, IDNum, db);
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
    printf("Otherwise, enter any other string or character to exit out if you changed your mind.\n");
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

        menu_selection(ans, arr, IDNum, db);
        return;
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

    PASS pass3 = NULL;
    pass3 = Password_Str(line2);
    Password_Calc_Strength(pass3, line2);

    if (getStrength(pass3) < 100) {
        printf("Your password strength is kind of weak, its strength is at %d.\n", getStrength(pass3));
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
            password_destroy(&pass3);
            pass3 = Update_Password(arr);
            Password_Print_Strength(pass3);
            free(line2);
            line2 = getData(pass3);

            PASS p2 = Update_Password_By_Scramble(arr, line2);
            char* t2 = getData(p2);

            printf("Scrambled Password for you to use and store: %s\n", t2);

            Encryption(t2);

            strcat(dest, t2);
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
            printf("Encrypted Password String: %s\n", t2);
            printf("Password ID for this pass is: %d\n", rNum);

            password_destroy(&p2);
            free(line);
            password_destroy(&pass3);
            free(email);

            int answer2;
            printOptions();

            int noc2 = scanf("%d", &answer2);
            clearKeyboardBuffer();
            while (noc2 != 1) {
                printf("Not a number between 1-8! Enter again!\n");
                noc2 = scanf("%d", &answer2);
                clearKeyboardBuffer();
            }

            while (answer2 < 1 || answer2 > 8) {
                printf("Not a number between 1-8! Enter again!\n");
                scanf("%d", &answer2);
                clearKeyboardBuffer();
            }

            DatabaseIsNotFull(&answer2);
            DatabaseIsFull(&answer2);
            DatabaseIsEmptyForUser(&answer2, IDNum);

            menu_selection(answer2, arr, IDNum, db);
            return;
        }
    }

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
    password_destroy(&pass3);
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

    menu_selection(answer, arr, IDNum, db);
    return;
}