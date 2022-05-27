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

void TestPassword(char* arr, sqlite3* db) {
    char* line = NULL;
    size_t len = 0;

    printf("Need to Measure Your Password Strength?\n");
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

    while (answer > 0) {
        if (answer == 1) {
            printf("Entering StorePassword()...\n");
            StorePassword(arr, db);
            return;
        } else if (answer == 2) {
            printf("Entering TestPassword()...\n");
            TestPassword(arr, db);
            return;
        } else if (answer == 3) {
            printf("Entering generatePassword()...\n");
            generatePassword(arr, db);
            return;
        } else if (answer == 4) {
            printf("Entering ScramblePassword()...\n");
            ScramblePassword(arr, db);
            return;
        } else if (answer == 5) {
            printf("Entering SearchPassword()...\n");
            SearchPassword(arr, db);
            return;
        } else if (answer == 6) {
            printf("Entering UpdateCurrentPass()...\n");
            UpdateCurrentPass(arr, db);
            return;
        } else if (answer == 7) {
            printf("Entering DeletePasswordContent()...\n");
            DeletePasswordContent(arr, db);
            return;
        } else {
            printf("Returning to Main Menu...\n\n");
            return;
        }
    }

    return;
}

void generatePassword(char* arr, sqlite3* db) {
    char* line = NULL;
    size_t len = 0;

    FILE* fp2 = NULL;

    fp2 = fopen("Names_Storage.txt", "a+");

    if (!fp2) {
        printf("Error: Unable to open file.\n");
        exit(1);
    }

    printf("Please enter your name: (first_name) (last_name)\n");
    size_t size = getline(&line, &len, stdin);
    line[size - 1] = '\0';

    fprintf(fp2, "%s\n", line);

    fclose(fp2);

    char dest[100] = "INSERT INTO PASSWORD VALUES(";
    char tmp[100];
    char* errMesg = 0;

    int rNum = rand() % 999999 + 1;

    IDChecker(rNum);

    sprintf(tmp, "%d", rNum);

    strcat(dest, tmp);
    strcat(dest, ", '");
    strcat(dest, line);
    strcat(dest, "', '");

    PASS p = NULL;
    printf("Generating a new password...\n");

    p = Update_Password(arr);
    Password_Print_Strength(p);

    char* s = getData(p);
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

    password_destroy(&p);
    free(line);

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

    while (answer > 0) {
        if (answer == 1) {
            printf("Entering StorePassword()...\n");
            StorePassword(arr, db);
            return;
        } else if (answer == 2) {
            printf("Entering TestPassword()...\n");
            TestPassword(arr, db);
            return;
        } else if (answer == 3) {
            printf("Entering generatePassword()...\n");
            generatePassword(arr, db);
            return;
        } else if (answer == 4) {
            printf("Entering ScramblePassword()...\n");
            ScramblePassword(arr, db);
            return;
        } else if (answer == 5) {
            printf("Entering SearchPassword()...\n");
            SearchPassword(arr, db);
            return;
        } else if (answer == 6) {
            printf("Entering UpdateCurrentPass()...\n");
            UpdateCurrentPass(arr, db);
            return;
        } else if (answer == 7) {
            printf("Entering DeletePasswordContent()...\n");
            DeletePasswordContent(arr, db);
            return;
        } else {
            printf("Returning to Main Menu...\n\n");
            return;
        }
    }

    return;
}

void StorePassword(char* arr, sqlite3* db) {
    char* line = NULL;
    size_t len = 0;

    char* line2 = NULL;
    size_t len2 = 0;

    FILE* fp2 = NULL;

    fp2 = fopen("Names_Storage.txt", "a+");

    if (!fp2) {
        printf("Error: Unable to open file.\n");
        exit(1);
    }

    printf("Need to store your password?\n");

    printf("Please enter your name: (first_name) (last_name)\n");
    size_t size = getline(&line, &len, stdin);
    line[size - 1] = '\0';

    fprintf(fp2, "%s\n", line);

    fclose(fp2);

    char dest[100] = "INSERT INTO PASSWORD VALUES(";
    char* errMesg = 0;
    char tmp[100];

    int rNum = rand() % 999999 + 1;

    IDChecker(rNum);

    sprintf(tmp, "%d", rNum);

    strcat(dest, tmp);
    strcat(dest, ", '");
    strcat(dest, line);
    strcat(dest, "', '");

    printf("Enter your password:\n");
    size_t size2 = getline(&line2, &len2, stdin);
    line2[size2 - 1] = '\0';

    PASS pass = NULL;
    pass = Password_Str(line2);

    char* s = getData(pass);

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

    answerChecker(&answer);
    answerChecker2(&answer);

    while (answer > 0) {
        if (answer == 1) {
            printf("Entering StorePassword()...\n");
            StorePassword(arr, db);
            return;
        } else if (answer == 2) {
            printf("Entering TestPassword()...\n");
            TestPassword(arr, db);
            return;
        } else if (answer == 3) {
            printf("Entering generatePassword()...\n");
            generatePassword(arr, db);
            return;
        } else if (answer == 4) {
            printf("Entering ScramblePassword()...\n");
            ScramblePassword(arr, db);
            return;
        } else if (answer == 5) {
            printf("Entering SearchPassword()...\n");
            SearchPassword(arr, db);
            return;
        } else if (answer == 6) {
            printf("Entering UpdateCurrentPass()...\n");
            UpdateCurrentPass(arr, db);
            return;
        } else if (answer == 7) {
            printf("Entering DeletePasswordContent()...\n");
            DeletePasswordContent(arr, db);
            return;
        } else {
            printf("Returning to Main Menu...\n\n");
            return;
        }
    }

    return;
}

void ScramblePassword(char* arr, sqlite3* db) {
    char* line = NULL;
    size_t len = 0;

    char* line2 = NULL;
    size_t len2 = 0;

    FILE* fp2 = NULL;

    fp2 = fopen("Names_Storage.txt", "a+");

    if (!fp2) {
        printf("Error: Unable to open file.\n");
        exit(1);
    }

    printf("Please enter your name: (first_name) (last_name)\n");
    size_t size = getline(&line, &len, stdin);
    line[size - 1] = '\0';

    fprintf(fp2, "%s\n", line);

    fclose(fp2);

    char dest[100] = "INSERT INTO PASSWORD VALUES(";
    char tmp[100];
    char* errMesg = 0;

    int rNum = rand() % 999999 + 1;

    IDChecker(rNum);

    sprintf(tmp, "%d", rNum);

    strcat(dest, tmp);
    strcat(dest, ", '");
    strcat(dest, line);
    strcat(dest, "', '");

    printf("Enter your password:\n");
    size_t size2 = getline(&line2, &len2, stdin);
    line2[size2 - 1] = '\0';

    PASS p = Update_Password_By_Scramble(arr, line2);

    char* t = getData(p);

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

    password_destroy(&p);
    free(line);
    free(line2);

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

    while (answer > 0) {
        if (answer == 1) {
            printf("Entering StorePassword()...\n");
            StorePassword(arr, db);
            return;
        } else if (answer == 2) {
            printf("Entering TestPassword()...\n");
            TestPassword(arr, db);
            return;
        } else if (answer == 3) {
            printf("Entering generatePassword()...\n");
            generatePassword(arr, db);
            return;
        } else if (answer == 4) {
            printf("Entering ScramblePassword()...\n");
            ScramblePassword(arr, db);
            return;
        } else if (answer == 5) {
            printf("Entering SearchPassword()...\n");
            SearchPassword(arr, db);
            return;
        } else if (answer == 6) {
            printf("Entering UpdateCurrentPass()...\n");
            UpdateCurrentPass(arr, db);
            return;
        } else if (answer == 7) {
            printf("Entering DeletePasswordContent()...\n");
            DeletePasswordContent(arr, db);
            return;
        } else {
            printf("Returning to Main Menu...\n\n");
            return;
        }
    }

    return;
}

void SearchPassword(char* arr, sqlite3* db) {
    char* line = NULL;
    size_t len = 0;
    char* item;
    char* squote = "'";
    int num = 0;

    int ans;
    printf("What are you searching for?\n");
    printf("Press (1) if you want to search by name\n");
    printf("Press (2) if you want to search by ID\n");

    int noc2 = scanf("%d", &ans);
    clearKeyboardBuffer();

    while (noc2 != 1) {
        printf("Not a number between 1-2! Enter again!\n");
        noc2 = scanf("%d", &ans);
        clearKeyboardBuffer();
    }

    while (ans < 1 || ans > 2) {
        printf("Not a number between 1-2! Enter again!\n");
        scanf("%d", &ans);
        clearKeyboardBuffer();
    }

    char dest[120] = "select * from PASSWORD where ";
    char* errMesg = 0;

    if (ans == 1) {
        printf("Please enter the name you want to search for:\n");
        len = getline(&line, &len, stdin);

        item = "Name";

        NameChecker(line);

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
    } else if (ans == 2) {
        printf("Please enter your ID number:\n");
        scanf("%d", &num);
        clearKeyboardBuffer();

        IDChecker2(&num);
        item = "ID";
        char ntmp[50];
        sprintf(ntmp, "%d", num);

        strcat(dest, item);
        strcat(dest, " = ");
        strcat(dest, squote);
        strcat(dest, ntmp);
        strcat(dest, squote);
    }

    // printf("%s\n", dest);

    int ret = sqlite3_exec(db, dest, PrintData, 0, &errMesg);

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

    while (answer > 0) {
        if (answer == 1) {
            printf("Entering StorePassword()...\n");
            StorePassword(arr, db);
            return;
        } else if (answer == 2) {
            printf("Entering TestPassword()...\n");
            TestPassword(arr, db);
            return;
        } else if (answer == 3) {
            printf("Entering generatePassword()...\n");
            generatePassword(arr, db);
            return;
        } else if (answer == 4) {
            printf("Entering ScramblePassword()...\n");
            ScramblePassword(arr, db);
            return;
        } else if (answer == 5) {
            printf("Entering SearchPassword()...\n");
            SearchPassword(arr, db);
            return;
        } else if (answer == 6) {
            printf("Entering UpdateCurrentPass()...\n");
            UpdateCurrentPass(arr, db);
            return;
        } else if (answer == 7) {
            printf("Entering DeletePasswordContent()...\n");
            DeletePasswordContent(arr, db);
            return;
        } else {
            printf("Returning to Main Menu...\n\n");
            return;
        }
    }

    return;
}

void UpdateCurrentPass(char* arr, sqlite3* db) {
    char* line = NULL;
    size_t len = 0;
    size_t size = 0;
    const char* data = "PrintData function called";
    printf("Updating one of your current passwords?\n");

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

    IDChecker2(&num);

    printf("Enter your new password:\n");
    size = getline(&line, &len, stdin);
    line[size - 1] = '\0';

    char tmp[100];
    sprintf(tmp, "%d", num);

    strcat(line, "' WHERE ID=");
    strcat(dest, line);
    strcat(dest, tmp);
    strcat(dest, "; ");
    strcat(dest, "SELECT * from PASSWORD WHERE ID=");
    strcat(dest, tmp);

    // printf("%s\n", dest);

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

    answerChecker(&answer);
    answerChecker2(&answer);

    while (answer > 0) {
        if (answer == 1) {
            printf("Entering StorePassword()...\n");
            StorePassword(arr, db);
            return;
        } else if (answer == 2) {
            printf("Entering TestPassword()...\n");
            TestPassword(arr, db);
            return;
        } else if (answer == 3) {
            printf("Entering generatePassword()...\n");
            generatePassword(arr, db);
            return;
        } else if (answer == 4) {
            printf("Entering ScramblePassword()...\n");
            ScramblePassword(arr, db);
            return;
        } else if (answer == 5) {
            printf("Entering SearchPassword()...\n");
            SearchPassword(arr, db);
            return;
        } else if (answer == 6) {
            printf("Entering UpdateCurrentPass()...\n");
            UpdateCurrentPass(arr, db);
            return;
        } else if (answer == 7) {
            printf("Entering DeletePasswordContent()...\n");
            DeletePasswordContent(arr, db);
            return;
        } else {
            printf("Returning to Main Menu...\n\n");
            return;
        }
    }

    return;
}

void DeletePasswordContent(char* arr, sqlite3* db) {
    const char* data = "PrintData function called";
    printf("Deleting one of your current passwords?\n");

    char dest[120] = "DELETE from PASSWORD WHERE ID=";
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

    IDChecker2(&num);

    char tmp[100];
    sprintf(tmp, "%d", num);

    strcat(dest, tmp);
    strcat(dest, "; ");
    strcat(dest, "SELECT * from PASSWORD WHERE ID=");
    strcat(dest, tmp);

    // printf("%s\n", dest);

    int ret = sqlite3_exec(db, dest, PrintData2, (void*)data, &errMesg);

    if (ret != SQLITE_OK) {
        printf("Error in SQL statement: %s\n", errMesg);
        sqlite3_free(errMesg);
        sqlite3_close(db);
        return;
    }

    FILE* fp = fopen("Names_Storage.txt", "r");
    FILE* fp2 = fopen("ID_Storage.txt", "r");
    FILE* fptmp = fopen("delete.txt", "w");

    if (!fp || !fp2 || !fptmp) {
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

    fptmp = fopen("delete.txt", "r");
    fp2 = fopen("ID_Storage.txt", "w");

    if (!fp2 || !fptmp) {
        printf("Error: Unable to open files.\n");
        exit(1);
    }

    while (fscanf(fptmp, "%d", &number) != EOF) {
        fprintf(fp2, "%d\n", number);
    }

    fclose(fp2);
    fclose(fptmp);

    fptmp = fopen("delete.txt", "w");

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

    fptmp = fopen("delete.txt", "r");
    fp = fopen("Names_Storage.txt", "w");

    if (!fp || !fptmp) {
        printf("Error: Unable to open files.\n");
        exit(1);
    }

    while ((len = getline(&line, &len, fptmp)) != EOF) {
        fprintf(fp, "%s", line);
    }

    fclose(fp);
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

    answerChecker(&answer);
    answerChecker2(&answer);

    while (answer > 0) {
        if (answer == 1) {
            printf("Entering StorePassword()...\n");
            StorePassword(arr, db);
            return;
        } else if (answer == 2) {
            printf("Entering TestPassword()...\n");
            TestPassword(arr, db);
            return;
        } else if (answer == 3) {
            printf("Entering generatePassword()...\n");
            generatePassword(arr, db);
            return;
        } else if (answer == 4) {
            printf("Entering ScramblePassword()...\n");
            ScramblePassword(arr, db);
            return;
        } else if (answer == 5) {
            printf("Entering SearchPassword()...\n");
            SearchPassword(arr, db);
            return;
        } else if (answer == 6) {
            printf("Entering UpdateCurrentPass()...\n");
            UpdateCurrentPass(arr, db);
            return;
        } else if (answer == 7) {
            printf("Entering DeletePasswordContent()...\n");
            DeletePasswordContent(arr, db);
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
        printf("%s: = %s\n", ColName[i], argv[i] ? argv[i] : "NULL");
        i++;
    }
    printf("\n");
    return 0;
}

void IDChecker(int n) {
    int numVal = 0;
    FILE* fp2;

    fp2 = fopen("ID_Storage.txt", "r");

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

    fp2 = fopen("ID_Storage.txt", "a+");

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
    fp2 = fopen("ID_Storage.txt", "r");

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

void NameChecker(char* line) {
    int counter1 = 0, counter2 = 0;
    char* rname = NULL;
    size_t len = 0;
    FILE* fp2;

    MY_STRING s1 = my_string_init_c_string(line);
    MY_STRING s2;

    fp2 = fopen("Names_Storage.txt", "r");

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
        printf("Error: Name does not exist. Please Try Again\n");
        len = getline(&line, &len, stdin);
        line[len] = '\0';

        NameChecker(line);
    }

    free(rname);
    my_string_destroy(&s1);

    return;
}
