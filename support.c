#define  _GNU_SOURCE

#include "my_string.h"
#include "password.h"
#include "support.h"
#include "utility.h"

/* Creates a new account ID and checks to see if its unique,
otherwise keep on generating one where we get a new id
*/
void CreateNewAccIDAndWriteIt(int n) {
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
            CreateNewAccIDAndWriteIt(n);
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

void CheckIfAccIDExist(int* num) {
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
        CheckIfAccIDExist(num);
    }

    fclose(fp2);

    return;
}

void CreateNewAccID(int* n) {
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
            CreateNewAccID(n);
        }
    }

    fclose(fp2);

    return;
}

void CheckIfAccIDIsYours(int* IDNum, int* num) {
    while (*IDNum != *num) {
        printf("This is not your Account ID, enter again!\n");
        scanf("%d", num);
        clearKeyboardBuffer();
    }
    return;
}

void CreateNewPassIDAndWriteIt(int* n) {
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
            CreateNewPassIDAndWriteIt(n);
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

void CheckIfPassIDExist(int* num) {
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
        CheckIfPassIDExist(num);
    }

    fclose(fp2);

    return;
}

Boolean CheckIfPassIDIsYoursBeforeUpdating(int* num, int* acc, sqlite3* db) {
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
            int ret = sqlite3_exec(db, sql, PrintUpdatedDataInDB, (void*)data, &errMesg);

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
        CheckIfPassIDIsYoursBeforeUpdating(num, acc, db);
        return FALSE;
    }
}

// Checks if the password ID that you input exists or not
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

// Check if the password ID is yours or not
Boolean CheckIfPasswordID_IsYours(int* num, int IDNum, sqlite3* db) {
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
    int lineNum2 = 1;
    while ((len = getline(&email, &len, fpemail)) != EOF) {
        email[len - 1] = '\0';
        if (lineNum2 == lineNum) {
            break;
        } else {
            lineNum2++;
        }
    }

    fclose(fpemail);

    int val = 1;
    while (fscanf(fpID, "%d", &numVal) != EOF) {
        if (IDNum == numVal) {
            break;
        } else {
            val++;
        }
    }

    fclose(fpID);

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

    fclose(fpmail);

    if (!my_string_equal(s1, m)) {
        free(email);
        free(mail);
        my_string_destroy(&s1);
        my_string_destroy(&m);

        return FALSE;
    }

    free(email);
    free(mail);
    my_string_destroy(&s1);
    my_string_destroy(&m);
    return TRUE;
}

void EmailChecker(char* line) {
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

        EmailChecker(line);
    }

    free(rname);
    my_string_destroy(&s1);

    return;
}

Boolean LoginChecker(char* line) {
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

void checkIfLoginEmailExists(char* line, int* IDNum) {
    EmailChecker(line);
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
        printf("Error: This email ain't yours, please enter a different email\n");
        len = getline(&line, &len, stdin);
        line[len] = '\0';
        checkIfLoginEmailExists(line, IDNum);
    }

    fclose(fpID);
    EmailChecker(line);
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

Boolean CheckIfEmailExistsAlready(char* stri3) {
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
        CheckIfEmailExistsAlready(stri3);
    }

    fclose(fp2);
    free(rname);

    return b;
}

char* CheckIfItsYourEmail(char* email, int IDNum) {
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
        CheckIfItsYourEmail(email, IDNum);
    }

    return email;
}
