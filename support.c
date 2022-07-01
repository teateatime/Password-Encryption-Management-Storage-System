#define  _GNU_SOURCE

#include "my_string.h"
#include "password.h"
#include "support.h"

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
        printf("This ID does not belong to yours, enter again!\n");
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
Boolean CheckIfPasswordID_IsYours(int* num, int IDNum) {
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
        checkIfLoginEmailExists(line, IDNum);
    }
    fclose(fpID);
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
