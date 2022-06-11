#define  _GNU_SOURCE

#include "my_string.h"
#include "password.h"
#include "manager.h"

void clearKeyboardBuffer(void){
   char ch;
   scanf("%c", &ch);
   while (ch != '\n' && ch != '\0') {
      scanf("%c", &ch);
   }
}

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
    printf("\n");
    printf("Press (1) if you want to store the password you want\n");
    printf("Press (2) if you want to test your password strength again\n");
    printf("Press (3) if you want us to generate a password for you and store it for safe keeping\n");
    printf("Press (4) if you want to scramble your current password and store it for safe keeping\n");
    printf("Press (5) if you want to search your password(s) inside the database\n");
    printf("Press (6) if you want to update your password(s) inside the database\n");
    printf("Press (7) if you want to delete your password(s) inside the database\n");
    printf("Press (8) to return to main\n");

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

    answerChecker(&answer);
    answerChecker2(&answer);
    answerChecker3(&answer, IDNum);

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

    fprintf(fp3, "%s\n", email);

    fclose(fp3);

    char dest[100] = "INSERT or IGNORE INTO PASSWORD VALUES(";
    char tmp[100];
    char* errMesg = 0;

    int rNum = rand() % 999999 + 1;

    IDChecker5(&rNum);

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
    printf("Encrpyted Password String: %s\n", s);

    password_destroy(&p);
    free(line);
    free(email);

    int answer;
    printf("\n");
    printf("Press (1) if you want to store the password you want\n");
    printf("Press (2) if you want to test your password strength\n");
    printf("Press (3) if you want us to generate another password for you and store it for safe keeping again\n");
    printf("Press (4) if you want to scramble your current password and store it for safe keeping\n");
    printf("Press (5) if you want to search your password(s) inside the database\n");
    printf("Press (6) if you want to update your password(s) inside the database\n");
    printf("Press (7) if you want to delete your password(s) inside the database\n");
    printf("Press (8) to return to main\n");
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

    answerChecker(&answer);
    answerChecker2(&answer);
    answerChecker3(&answer, IDNum);

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

    fprintf(fp3, "%s\n", email);

    fclose(fp3);

    char dest[100] = "INSERT or IGNORE INTO PASSWORD VALUES(";
    char* errMesg = 0;
    char tmp[100];

    int rNum = rand() % 999999 + 1;

    IDChecker5(&rNum);

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

    PASS pass = NULL;
    pass = Password_Str(line2);
    Password_Calc_Strength(pass, line2);

    char* s = getData(pass);
    if (getStrength(pass) <= 100) {
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
    printf("Encrpyted Password String: %s\n", s);

    int answer;
    printf("\n");
    printf("Press (1) if you want to store the password you want again\n");
    printf("Press (2) if you want to test your password strength\n");
    printf("Press (3) if you want us to generate a password for you and store it for safe keeping\n");
    printf("Press (4) if you want to scramble your current password and store it for safe keeping\n");
    printf("Press (5) if you want to search your password(s) inside the database\n");
    printf("Press (6) if you want to update your password(s) inside the database\n");
    printf("Press (7) if you want to delete your password(s) inside the database\n");
    printf("Press (8) to return to main\n");
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

    answerChecker(&answer);
    answerChecker2(&answer);
    answerChecker3(&answer, IDNum);

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

    fprintf(fp3, "%s\n", email);

    fclose(fp3);

    char dest[100] = "INSERT or IGNORE INTO PASSWORD VALUES(";
    char tmp[100];
    char* errMesg = 0;

    int rNum = rand() % 999999 + 1;

    IDChecker5(&rNum);

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

    PASS p = Update_Password_By_Scramble(arr, line2);

    char* t = getData(p);

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
    printf("Encrpyted Password String: %s\n", t);

    password_destroy(&p);
    free(line);
    free(line2);
    free(email);

    int answer;
    printf("\n");
    printf("Press (1) if you want to store the password you want\n");
    printf("Press (2) if you want to test your password strength\n");
    printf("Press (3) if you want us to generate another password for you and store it for safe keeping\n");
    printf("Press (4) if you want to scramble your current password and store it for safe keeping again\n");
    printf("Press (5) if you want to search your password(s) inside the database\n");
    printf("Press (6) if you want to update your password(s) inside the database\n");
    printf("Press (7) if you want to delete your password(s) inside the database\n");
    printf("Press (8) to return to main\n");

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

    answerChecker(&answer);
    answerChecker2(&answer);
    answerChecker3(&answer, IDNum);

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
    printf("\nWhat are you searching for?\n");
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
        printf("Please enter the email address that you want to search for:\n");
        len = getline(&line, &len, stdin);

        item = "Email";

        NameChecker(line);
        emailChecker(line, IDNum);

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

        int ret = sqlite3_exec(db, dest, PrintData, 0, &errMesg);

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
            if (PasswordIDChecker2(&num, *IDNum) == FALSE) {
                printf("Incorrect email!\n");
                printf("Please Try Again Later.\n");

                free(line);

                int answer;
                printf("\n");
                printf("Press (1) if you want to store the password you want\n");
                printf("Press (2) if you want to test your password strength again\n");
                printf("Press (3) if you want us to generate a password for you and store it for safe keeping\n");
                printf("Press (4) if you want to scramble your current password and store it for safe keeping\n");
                printf("Press (5) if you want to search your password(s) inside the database\n");
                printf("Press (6) if you want to update your password(s) inside the database again\n");
                printf("Press (7) if you want to delete your password(s) inside the database\n");
                printf("Press (8) to return to main\n");

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

                answerChecker(&answer);
                answerChecker2(&answer);
                answerChecker3(&answer, IDNum);

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
            printf("Please enter your reference word in order to update your password:\n");
            size_t siz = getline(&ref, &siz, stdin);
            ref[siz - 1] = '\0';

            if (!refWordValidCheck(ref, IDNum)) {
                printf("Incorrect reference word!\n");
                printf("Please Try Again Later.\n");
                free(ref);

                int answer;
                printf("\n");
                printf("Press (1) if you want to store the password you want\n");
                printf("Press (2) if you want to test your password strength again\n");
                printf("Press (3) if you want us to generate a password for you and store it for safe keeping\n");
                printf("Press (4) if you want to scramble your current password and store it for safe keeping\n");
                printf("Press (5) if you want to search your password(s) inside the database\n");
                printf("Press (6) if you want to update your password(s) inside the database again\n");
                printf("Press (7) if you want to delete your password(s) inside the database\n");
                printf("Press (8) to return to main\n");

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

                answerChecker(&answer);
                answerChecker2(&answer);
                answerChecker3(&answer, IDNum);

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

        IDChecker2(&num);
        IDChecker4(IDNum, &num);
        item = "ID";
        char ntmp[50];
        sprintf(ntmp, "%d", num);

        strcat(dest, item);
        strcat(dest, " = ");
        strcat(dest, squote);
        strcat(dest, ntmp);
        strcat(dest, squote);

        int ret = sqlite3_exec(db, dest, PrintData, 0, &errMesg);

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
            if (PasswordIDChecker2(&num, *IDNum) == FALSE) {
                printf("Incorrect email!\n");
                printf("Please Try Again Later.\n");

                int answer;
                printf("\n");
                printf("Press (1) if you want to store the password you want\n");
                printf("Press (2) if you want to test your password strength again\n");
                printf("Press (3) if you want us to generate a password for you and store it for safe keeping\n");
                printf("Press (4) if you want to scramble your current password and store it for safe keeping\n");
                printf("Press (5) if you want to search your password(s) inside the database\n");
                printf("Press (6) if you want to update your password(s) inside the database again\n");
                printf("Press (7) if you want to delete your password(s) inside the database\n");
                printf("Press (8) to return to main\n");

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

                answerChecker(&answer);
                answerChecker2(&answer);
                answerChecker3(&answer, IDNum);

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
            printf("Please enter your reference word in order to update your password:\n");
            size_t siz = getline(&ref, &siz, stdin);
            ref[siz - 1] = '\0';

            if (!refWordValidCheck(ref, IDNum)) {
                printf("Incorrect reference word!\n");
                printf("Please Try Again Later.\n");
                free(ref);

                int answer;
                printf("\n");
                printf("Press (1) if you want to store the password you want\n");
                printf("Press (2) if you want to test your password strength again\n");
                printf("Press (3) if you want us to generate a password for you and store it for safe keeping\n");
                printf("Press (4) if you want to scramble your current password and store it for safe keeping\n");
                printf("Press (5) if you want to search your password(s) inside the database\n");
                printf("Press (6) if you want to update your password(s) inside the database again\n");
                printf("Press (7) if you want to delete your password(s) inside the database\n");
                printf("Press (8) to return to main\n");

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

                answerChecker(&answer);
                answerChecker2(&answer);
                answerChecker3(&answer, IDNum);

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
        if (PasswordIDChecker2(&num, *IDNum) == FALSE) {
            printf("Incorrect email!\n");
            printf("Please Try Again Later.\n");

            int answer;
            printf("\n");
            printf("Press (1) if you want to store the password you want\n");
            printf("Press (2) if you want to test your password strength again\n");
            printf("Press (3) if you want us to generate a password for you and store it for safe keeping\n");
            printf("Press (4) if you want to scramble your current password and store it for safe keeping\n");
            printf("Press (5) if you want to search your password(s) inside the database\n");
            printf("Press (6) if you want to update your password(s) inside the database again\n");
            printf("Press (7) if you want to delete your password(s) inside the database\n");
            printf("Press (8) to return to main\n");

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

            answerChecker(&answer);
            answerChecker2(&answer);
            answerChecker3(&answer, IDNum);

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

        int ret = sqlite3_exec(db, dest, PrintData, 0, &errMesg);

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
            printf("Please enter your reference word in order to update your password:\n");
            size_t siz = getline(&ref, &siz, stdin);
            ref[siz - 1] = '\0';

            if (!refWordValidCheck(ref, IDNum)) {
                printf("Incorrect reference word!\n");
                printf("Please Try Again Later.\n");
                free(ref);

                int answer;
                printf("\n");
                printf("Press (1) if you want to store the password you want\n");
                printf("Press (2) if you want to test your password strength again\n");
                printf("Press (3) if you want us to generate a password for you and store it for safe keeping\n");
                printf("Press (4) if you want to scramble your current password and store it for safe keeping\n");
                printf("Press (5) if you want to search your password(s) inside the database\n");
                printf("Press (6) if you want to update your password(s) inside the database again\n");
                printf("Press (7) if you want to delete your password(s) inside the database\n");
                printf("Press (8) to return to main\n");

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

                answerChecker(&answer);
                answerChecker2(&answer);
                answerChecker3(&answer, IDNum);

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
    printf("\n");
    printf("Press (1) if you want to store the password you want\n");
    printf("Press (2) if you want to test your password strength again\n");
    printf("Press (3) if you want us to generate a password for you and store it for safe keeping\n");
    printf("Press (4) if you want to scramble your current password and store it for safe keeping\n");
    printf("Press (5) if you want to search your password(s) inside the database again\n");
    printf("Press (6) if you want to update your password(s) inside the database\n");
    printf("Press (7) if you want to delete your password(s) inside the database\n");
    printf("Press (8) to return to main\n");

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

    answerChecker(&answer);
    answerChecker2(&answer);
    answerChecker3(&answer, IDNum);

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
    const char* data = "PrintData function called";
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

    IDChecker7(&num, IDNum, db);

    char* ref = NULL;
    printf("Please enter your reference word in order to update your password:\n");
    size = getline(&ref, &len, stdin);
    ref[size - 1] = '\0';

    if (refWordValidCheck(ref, IDNum) == FALSE) {
        printf("Incorrect reference word!\n");
        printf("Please Try Again Later.\n");
        free(ref);

        int answer;
        printf("\n");
        printf("Press (1) if you want to store the password you want\n");
        printf("Press (2) if you want to test your password strength again\n");
        printf("Press (3) if you want us to generate a password for you and store it for safe keeping\n");
        printf("Press (4) if you want to scramble your current password and store it for safe keeping\n");
        printf("Press (5) if you want to search your password(s) inside the database\n");
        printf("Press (6) if you want to update your password(s) inside the database again\n");
        printf("Press (7) if you want to delete your password(s) inside the database\n");
        printf("Press (8) to return to main\n");

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

        answerChecker(&answer);
        answerChecker2(&answer);
        answerChecker3(&answer, IDNum);

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

    char tmp[100];
    char* li = line;
    sprintf(tmp, "%d", num);

    Encryption(line);

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

    int ret = sqlite3_exec(db, dest, PrintData2, (void*)data, &errMesg);

    if (ret != SQLITE_OK) {
        printf("Error in SQL statement: %s\n", errMesg);
        sqlite3_free(errMesg);
        sqlite3_close(db);
        return;
    }

    int answer;
    printf("\n");
    printf("Press (1) if you want to store the password you want\n");
    printf("Press (2) if you want to test your password strength again\n");
    printf("Press (3) if you want us to generate a password for you and store it for safe keeping\n");
    printf("Press (4) if you want to scramble your current password and store it for safe keeping\n");
    printf("Press (5) if you want to search your password(s) inside the database\n");
    printf("Press (6) if you want to update your password(s) inside the database again\n");
    printf("Press (7) if you want to delete your password(s) inside the database\n");
    printf("Press (8) to return to main\n");

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

    answerChecker(&answer);
    answerChecker2(&answer);
    answerChecker3(&answer, IDNum);

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
    const char* data = "PrintData function called";
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

    IDChecker7(&num, IDNum, db);

    char* ref = NULL;
    printf("Please enter your reference word in order to update your password:\n");
    size_t siz = getline(&ref, &siz, stdin);
    ref[siz - 1] = '\0';

    if (!refWordValidCheck(ref, IDNum)) {
        printf("Incorrect reference word!\n");
        printf("Please Try Again Later.\n");
        free(ref);

        int answer;
        printf("\n");
        printf("Press (1) if you want to store the password you want\n");
        printf("Press (2) if you want to test your password strength again\n");
        printf("Press (3) if you want us to generate a password for you and store it for safe keeping\n");
        printf("Press (4) if you want to scramble your current password and store it for safe keeping\n");
        printf("Press (5) if you want to search your password(s) inside the database\n");
        printf("Press (6) if you want to update your password(s) inside the database again\n");
        printf("Press (7) if you want to delete your password(s) inside the database\n");
        printf("Press (8) to return to main\n");

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

        answerChecker(&answer);
        answerChecker2(&answer);
        answerChecker3(&answer, IDNum);

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

    int ret = sqlite3_exec(db, dest, PrintData2, (void*)data, &errMesg);

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
    printf("\n");
    printf("Press (1) if you want to store the password you want\n");
    printf("Press (2) if you want to test your password strength again\n");
    printf("Press (3) if you want us to generate a password for you and store it for safe keeping\n");
    printf("Press (4) if you want to scramble your current password and store it for safe keeping\n");
    printf("Press (5) if you want to search your password(s) inside the database\n");
    printf("Press (6) if you want to update your password(s) inside the database\n");
    printf("Press (7) if you want to delete your password(s) inside the database again\n");
    printf("Press (8) to return to main\n");

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

    answerChecker(&answer);
    answerChecker2(&answer);
    answerChecker3(&answer, IDNum);

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

/* Checks if database is empty or not, so we prevent users
from searching/deleting.
*/
void answerChecker(int* answer) {
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
void answerChecker2(int* answer) {
    int noc;
    if (*answer == 1 || *answer == 3 || *answer == 4) {
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

        while ((size >= 999999 || size2 >= 999999) && (*answer == 1 || *answer == 3 || *answer == 4)) {
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

void answerChecker3(int* answer, int* IDNum) {
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

int PrintData(void *data, int argc, char **argv, char **ColName) {
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

int PrintData2(void *data, int argc, char **argv, char **ColName) {
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

Boolean is_substr_char(char* s, char* str) {
    int slen = strlen(s);
    int strl = strlen(str);

    int end = strl - slen + 1;

    for (int i = 0; i < end; i++) {
        bool found = 1;

        for (int j = 0; j < slen; j++) {
            if (s[j] != str[i + j]) {
                found = false;
                break;
            }
        }

        if (found) { return true; }
    }

    return false;
}

void IDChecker(int n) {
    int numVal = 0;
    FILE* fp2;

    fp2 = fopen("Text/ID_Storage.txt", "r");

    if (!fp2) {
        printf("Error: Unable to open file.\n");
        exit(1);
    }

    while (fscanf(fp2, "%d", &numVal) != EOF) {
        if (n == numVal) {
            n = rand() % 999999 + 1;
            IDChecker(n);
        }
    }

    fclose(fp2);

    fp2 = fopen("Text/ID_Storage.txt", "a+");

    if (!fp2) {
        printf("Error: Unable to open file.\n");
        exit(1);
    }

    fprintf(fp2, "%d\n", n);

    fclose(fp2);

    return;
}

void IDChecker2(int* num) {
    int numVal = 0;
    int counter1 = 0;
    int counter2 = 0;
    FILE* fp2;
    fp2 = fopen("Text/ID_Storage.txt", "r");

    while (fscanf(fp2, "%d", &numVal) != EOF) {
        counter1++;
        if (*num != numVal) {
            counter2++;
        }
    }

    if (counter1 == counter2) {
        printf("Error: ID number does not exist. Please Try Again\n");
        scanf("%d", num);
        clearKeyboardBuffer();
        IDChecker2(num);
    }

    fclose(fp2);

    return;
}

void IDChecker3(int* n) {
    int numVal = 0;
    FILE* fp2;

    fp2 = fopen("Text/ID_Storage.txt", "r");

    if (!fp2) {
        printf("Error: Unable to open file.\n");
        exit(1);
    }

    while (fscanf(fp2, "%d", &numVal) != EOF) {
        if (*n == numVal) {
            *n = rand() % 999999 + 1;
            IDChecker3(n);
        }
    }

    fclose(fp2);

    return;
}

void IDChecker4(int* IDNum, int* num) {
    while (*IDNum != *num) {
        printf("This ID does not belong to yours, enter again!\n");
        scanf("%d", num);
        clearKeyboardBuffer();
    }
    return;
}

void IDChecker5(int* n) {
    int numVal = 0;
    FILE* fp2;

    fp2 = fopen("Text/Password_ID.txt", "r");

    if (!fp2) {
        printf("Error: Unable to open file.\n");
        exit(1);
    }

    while (fscanf(fp2, "%d", &numVal) != EOF) {
        if (*n == numVal) {
            *n = rand() % 999999 + 1;
            IDChecker5(n);
        }
    }

    fclose(fp2);

    fp2 = fopen("Text/Password_ID.txt", "a+");

    if (!fp2) {
        printf("Error: Unable to open file.\n");
        exit(1);
    }

    fprintf(fp2, "%d\n", *n);

    fclose(fp2);

    return;
}

void IDChecker6(int* num) {
    int numVal = 0;
    int counter1 = 0;
    int counter2 = 0;
    FILE* fp2;
    fp2 = fopen("Text/Password_ID.txt", "r");

    while (fscanf(fp2, "%d", &numVal) != EOF) {
        counter1++;
        if (*num != numVal) {
            counter2++;
        }
    }

    if (counter1 == counter2) {
        printf("Error: Password ID number does not exist. Please Try Again\n");
        scanf("%d", num);
        clearKeyboardBuffer();
        IDChecker6(num);
    }

    fclose(fp2);

    return;
}

Boolean IDChecker7(int* num, int* acc, sqlite3* db) {
    const char* data = "PrintData function called";
    char sql[100] = "select * from PASSWORD where PASS_ID =";
    char* errMesg = 0;
    char str[20] = "AND ID=";
    char tmp[100];

    sqlite3_stmt *selectstmt;
    Boolean truth = FALSE;

    sprintf(tmp, "%d", *num);
    strcat(sql, tmp);
    strcat(sql, " ");
    strcat(sql, str);
    sprintf(tmp, "%d", *acc);
    strcat(sql, tmp);

    int result = sqlite3_prepare_v2(db, sql, -1, &selectstmt, NULL);
    if (result == SQLITE_OK) {
        if (sqlite3_step(selectstmt) == SQLITE_ROW) {
            int ret = sqlite3_exec(db, sql, PrintData2, (void*)data, &errMesg);

            if (ret != SQLITE_OK) {
                printf("Error in SQL statement: %s\n", errMesg);
                sqlite3_free(errMesg);
                sqlite3_close(db);
                return FALSE;
            }
            
            truth = TRUE;
        }
        else {
            truth = FALSE;
        }
    }
    sqlite3_finalize(selectstmt);

    if (truth) {
        return TRUE;
    } else {
        printf("No such Password ID exists or it does not belong to yours.\n");
        printf("Enter again:\n");
        scanf("%d", num);
        clearKeyboardBuffer();
        IDChecker7(num, acc, db);
        return FALSE;
    }
}

void PasswordIDChecker(int* num) {
    int numVal = 0;
    int counter1 = 0;
    int counter2 = 0;
    FILE* fp2;
    fp2 = fopen("Text/Password_ID.txt", "r");

    while (fscanf(fp2, "%d", &numVal) != EOF) {
        counter1++;
        if (*num != numVal) {
            counter2++;
        }
    }

    if (counter1 == counter2) {
        printf("Error: Password ID number does not exist. Please Try Again\n");
        scanf("%d", num);
        clearKeyboardBuffer();
        PasswordIDChecker(num);
    }

    fclose(fp2);

    return;
}

Boolean PasswordIDChecker2(int* num, int IDNum) {
    int numVal = 0;
    int lineNum = 1;
    FILE* fp2;
    FILE* fpemail;
    FILE* fpID = fopen("Text/ID_Storage.txt", "r");
    FILE* fpmail = fopen("Text/Mail_Storage.txt", "r");
    fp2 = fopen("Text/Password_ID.txt", "r");
    fpemail = fopen("Text/Email_Storage.txt", "r");


    if (!fp2 || !fpemail || !fpmail || !fpID) {
        printf("Files cannot open!\n");
        exit(1);
    }

    while (fscanf(fp2, "%d", &numVal) != EOF) {
        if (*num == numVal) {
            break;
        } else {
            lineNum++;
        }
    }

    fclose(fp2);

    char* email = NULL;
    size_t len = 0;
    printf("Enter your email address that you used to login to confirm that this is you:\n");
    len = getline(&email, &len, stdin);
    email[len - 1] = '\0';

    int val = 1;
    while (fscanf(fpID, "%d", &numVal) != EOF) {
        if (IDNum == numVal) {
            break;
        } else {
            val++;
        }
    }

    MY_STRING s1 = my_string_init_c_string(email);
    char* mail = NULL;
    int val2 = 1;
    while ((len = getline(&mail, &len, fpmail)) != EOF) {
        mail[len - 1] = '\0';
        if (val2 == val) {
            break;
        }
        val2++;
    }
    MY_STRING m = my_string_init_c_string(mail);

    if (!my_string_equal(s1, m)) {
        free(email);
        free(mail);
        fclose(fpemail);
        fclose(fpID);
        fclose(fpmail);
        my_string_destroy(&s1);
        my_string_destroy(&m);

        return FALSE;
    }

    MY_STRING s2 = NULL;

    char* remail = NULL;
    int lineNum2 = 1;
    while ((len = getline(&remail, &len, fpemail)) != EOF) {
        remail[len - 1] = '\0';
        s2 = my_string_init_c_string(remail);
        if (lineNum2 == lineNum) {
            break;
        } else {
            lineNum2++;
            my_string_destroy(&s2);
        }
    }

    if (my_string_equal(s1, s2)) {
        free(remail);
        free(email);
        free(mail);
        fclose(fpemail);
        fclose(fpID);
        fclose(fpmail);
        my_string_destroy(&s1);
        my_string_destroy(&s2);
        my_string_destroy(&m);
        return TRUE;
    } else {
        free(remail);
        free(email);
        free(mail);
        fclose(fpemail);
        fclose(fpID);
        fclose(fpmail);
        my_string_destroy(&s1);
        my_string_destroy(&s2);
        my_string_destroy(&m);
        return FALSE;
    }
}

void NameChecker(char* line) {
    int counter1 = 0, counter2 = 0;
    char* rname = NULL;
    size_t len = 0;
    FILE* fp2;

    MY_STRING s1 = my_string_init_c_string(line);
    MY_STRING s2;

    fp2 = fopen("Text/Email_Storage.txt", "r");

    if (!fp2) {
        printf("Error: Unable to open file.\n");
        exit(1);
    }

    while ((len = getline(&rname, &len, fp2)) != EOF) {
        s2 = my_string_init_c_string(rname);
        counter1++;
        if (!my_string_equal(s1, s2)) {
            counter2++;
        }
        my_string_destroy(&s2);
    }

    fclose(fp2);
    if (counter1 == counter2) {
        printf("Error: Email does not exist. Please Try Again\n");
        len = getline(&line, &len, stdin);
        line[len] = '\0';

        NameChecker(line);
    }

    free(rname);
    my_string_destroy(&s1);

    return;
}

Boolean NameChecker2(char* line) {
    int counter1 = 0, counter2 = 0;
    char* rname = NULL;
    size_t len = 0;
    FILE* fp2;

    MY_STRING s1 = my_string_init_c_string(line);
    MY_STRING s2;

    fp2 = fopen("Text/LoginStorage.txt", "r");

    if (!fp2) {
        printf("Error: Unable to open file.\n");
        exit(1);
    }

    while ((len = getline(&rname, &len, fp2)) != EOF) {
        rname[len - 1] = '\0';
        s2 = my_string_init_c_string(rname);
        counter1++;
        if (my_string_equal(s1, s2)) {
            counter2++;
        }
        my_string_destroy(&s2);
    }

    fclose(fp2);
    free(rname);
    my_string_destroy(&s1);

    if (counter2 == 1) {
        return true;
    } else {
        return false;
    }
}

void emailChecker(char* line, int* IDNum) {
    FILE *fplog = fopen("Text/LoginStorage.txt", "r");
    FILE *fpID = fopen("Text/ID_Storage.txt", "r");

    if (!fplog || !fpID) {
        printf("Files cannot open.\n");
        exit(1);
    }

    char* remail = NULL;
    int lineNumber = 1;
    int rID = 0;
    char* ret = 0;
    size_t len = 0;
    while ((len = getline(&remail, &len, fplog)) != EOF) {
        ret = strstr(remail, line);
        if (ret) {
            break;
        } else {
            lineNumber++;
        }
    }

    fclose(fplog);
    free(remail);

    int lineNumber2 = 1;
    while (fscanf(fpID, "%d", &rID) != EOF) {
        if (lineNumber2 == lineNumber) {
            break;
        } else {
            lineNumber2++;
        }
    }

    if (*IDNum != rID) {
        printf("You do not have access to this email, it ain't yours, enter a different email\n");
        len = getline(&line, &len, stdin);
        line[len] = '\0';
        emailChecker(line, IDNum);
    }
    fclose(fpID);
    return;
}

void Encryption(char* line) {
    for (int i = 0; (i < 100 && line[i] != '\0'); i++) {
        line[i] = line[i] + 3;
    }
}

char* Decryption(char* line) {
     for (int i = 0; (i < 100 && line[i] != '\0'); i++) {
        line[i] = line[i] - 3;
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

Boolean refWordValidCheck(char* ref, int* IDNum) {
    FILE* fpID = fopen("Text/ID_Storage.txt", "r");
    FILE* fpref = fopen("Text/reference.txt", "r");

    if (!fpref || !fpID) {
        printf("Files failed to open!\n");
        exit(1);
    }

    int number;
    int lineNum = 1;
    int lineNum2 = 1;
    while (fscanf(fpID, "%d", &number) != EOF) {
        if (*IDNum == number) {
            break;
        } else {
            lineNum++;
        }
    }

    char* rref = NULL;
    size_t len = 0;
    while ((len = getline(&rref, &len, fpref)) != EOF) {
        rref[len - 1] = '\0';
        if (lineNum2 == lineNum) {
            break;
        } else {
            lineNum2++;
        }
    }

    fclose(fpref);
    fclose(fpID);

    MY_STRING s1 = my_string_init_c_string(ref);
    MY_STRING s2 = my_string_init_c_string(rref);

    if (my_string_equal(s1, s2)) {
        free(rref);
        my_string_destroy(&s1);
        my_string_destroy(&s2);
        return TRUE;
    } else {
        free(rref);
        my_string_destroy(&s1);
        my_string_destroy(&s2);
        return FALSE;
    }
}

Boolean emailValid(char* stri3) {
    char* rname = NULL;
    size_t len = 0;
    FILE* fp2;

    fp2 = fopen("Text/LoginStorage.txt", "r");

    if (!fp2) {
        printf("Error: Unable to open file.\n");
        exit(1);
    }

    fseek(fp2, 0, SEEK_END);
    int size = ftell(fp2);

    Boolean b = FALSE;
    char* ret;

    rewind(fp2);
    while ((len = getline(&rname, &len, fp2)) != EOF) {
        rname[len - 1] = '\0';
        ret = strstr(rname, stri3);
        if (ret) {
            b = FALSE;
            break;
        } else {
            b = TRUE;
        }
    }

    if (b == FALSE && size > 0) {
        printf("Email already exists in someone else's account!\n");
        printf("Please enter something else:\n");
        len = getline(&stri3, &len, stdin);
        stri3[len - 1] = '\0';
        emailValid(stri3);
    }

    fclose(fp2);
    free(rname);

    return b;
}

char* emailValid2(char* email, int IDNum) {
    int numVal = 0;
    int lineNum = 1;
    FILE* fpID = fopen("Text/ID_Storage.txt", "r");
    FILE* fpmail = fopen("Text/Mail_Storage.txt", "r");

    if (!fpmail || !fpID) {
        printf("Files cannot open!\n");
        exit(1);
    }

    while (fscanf(fpID, "%d", &numVal) != EOF) {
        if (IDNum == numVal) {
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

        return email;
    } else {
        printf("This is not the email that you login with! Try Again!\n");
        free(mail);
        my_string_destroy(&s1);
        my_string_destroy(&s2);
        size_t size = 0;
        size = getline(&email, &size, stdin);
        email[size - 1] = '\0';
        emailValid2(email, IDNum);
    }

    return email;
}
