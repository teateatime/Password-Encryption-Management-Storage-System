#define _GNU_SOURCE

#include "password.h"
#include "my_string.h"

PASS Password_init_default(void) {
    password* pass = NULL;

    pass = (password*)calloc(1, sizeof(password));

    if (pass){
        pass->size = 0;
        pass->capacity = 8;
        pass->data = (char*) malloc(sizeof(char) * pass->capacity);

        if (pass->data == NULL) {
            printf("Did not work\n");
            return NULL;
        }
    }

    if (!pass->data || pass->data == NULL){
        free(pass);
    }

    if (pass == NULL){
        free(pass);
        exit(1);
    }

    return (PASS) pass;
}

void password_destroy(PASS* pItem){
    password* pPass = (password*) *pItem;

    if (pItem == NULL) {
        return;
    }

    free(pPass->data);
    free(pPass);
    pPass = NULL;
}

PASS Password_Str(char* c_string){
    password* pass = NULL;
    int i = 0;

    pass = (password*) calloc(1, sizeof(password));

    if (pass){
        pass->size = strlen(c_string);
        pass->capacity = strlen(c_string) + 1;
        pass->data = (char*) calloc(1, sizeof(char) * pass->capacity);

        if (pass->data == NULL) {
            printf("Did not work\n");
            free(pass);
            return NULL;
        }

        for (i = 0; i < pass->size; i++) {
            pass->data[i] = c_string[i];
        }
    }

    if (pass == NULL){
        free(pass);
        exit(1);
    }

    return (PASS) pass;
}

void Password_Print_Strength(char* str) {
    password* pass = (password*)str;
    int i = 0;
    if (pass == NULL) {
        printf("ERROR: Password_Strength returns NULL\n");
        exit(1);
    } 
    else {
        for (i = 0; i < pass->size; i++) {
            if (pass->data[i] == '\n') {
                pass->data[i] = '\0';
            }
        }
        printf("Password Strength of %s is: %d\n", pass->data, pass->strength);
        return;
    }
}

PASS Password_Calc_Strength(password* str, char* s) {
    password* pass = (password*)str;

    if (pass == NULL) {
        printf("ERROR: Password_Strength returns NULL\n");
        exit(1);
    } 
    else {
        if (!(CheckAlpha(pass))) {
            pass->strength = 0;
            return (PASS) pass;
        }
        if (!(CheckNum(pass))) {
            pass->strength = 0;
            return (PASS) pass;
        }
        if (!(CheckSpecialChars(pass))) {
            pass->strength = 0;
            return (PASS) pass;
        }
        if (CheckSpaces(pass)) {
            pass->strength = 0;
            return (PASS) pass;
        }

        int i = 0;
        for (i = 0; i <= pass->size - 1; i++) {
            if ((pass->data[i] == pass->data[i + 1])) {
                continue;
            } else {
                if ((!(isdigit(pass->data[i]))) && (!(isalpha(pass->data[i])))) {
                    pass->strength += 5;
                }
                pass->strength += 10;
            }
        }

        char * tok = strtok(s, "\n");

        for (int i = 0; i < strlen(tok); i++) {
            if (tok[i] == '\r') {
                tok[i] = '\n';
                strcat(pass->data, "\n");
            }
        }

        if (strlen(tok) <= pass->size) {
            while (tok != NULL) {
                for (int i = 0; i < strlen(tok); i++) {
                    if (tok[i] == '\r') {
                        tok[i] = '\n';
                    }
                }

                if (is_substr(tok, pass)) {
                    for (int i = 0; i < strlen(pass->data); i++) {
                        pass->strength -= 5;
                    }
                    break;
                }

                tok = strtok(NULL, "\n");
            }
        }

        return (PASS) pass;
    }
}

PASS Password_Calc_Strength2(password* str, char* s) {
    password* pass = (password*)str;

    if (pass == NULL) {
        printf("ERROR: Password_Strength returns NULL\n");
        exit(1);
    } 
    else {
        if (!(CheckAlpha(pass))) {
            printf("Error: Password does not have either a Upper or Lower case character\n");
            pass->strength = 0;
            return (PASS) pass;
        }
        if (!(CheckNum(pass))) {
            printf("Error: Password does not have a numerical character\n");
            pass->strength = 0;
            return (PASS) pass;
        }
        if (!(CheckSpecialChars(pass))) {
            printf("Error: Password does not have special characters\n");
            pass->strength = 0;
            return (PASS) pass;
        }
        if (CheckSpaces(pass)) {
            printf("Error: Password has a space\n");
            pass->strength = 0;
            return (PASS) pass;
        }

        MY_STRING s1 = my_string_init_c_string(pass->data);
        MY_STRING s2 = NULL;
        size_t len = 0;
        int counter = 0;
        char* rpass = NULL;

        FILE* fp = fopen("Text/dictionary.txt", "r");

        if (!fp) {
            printf("File cannot open.\n");
            exit(1);
        }

        while ((len = getline(&rpass, &len, fp)) != EOF) {
            s2 = my_string_init_c_string(rpass);
            if (my_string_equal(s1, s2)) {
                counter++;
            }

            if (counter == 1) {
                my_string_destroy(&s1);
                my_string_destroy(&s2);

                printf("Error: Password is too common\n");
                pass->strength = 0;

                free(rpass);

                fclose(fp);
                return (PASS) pass;
            }
            my_string_destroy(&s2);
        }

        my_string_destroy(&s1);

        free(rpass);
        fclose(fp);

        int i = 0;
        for (i = 0; i <= pass->size - 1; i++) {
            if ((pass->data[i] == pass->data[i + 1])) {
                continue;
            } else {
                if ((!(isdigit(pass->data[i]))) && (!(isalpha(pass->data[i])))) {
                    pass->strength += 5;
                }
                pass->strength += 10;
            }
        }

        char * tok = strtok(s, "\n");

        for (int i = 0; i < strlen(tok); i++) {
            if (tok[i] == '\r') {
                tok[i] = '\n';
                strcat(pass->data, "\n");
            }
        }

        if (strlen(tok) <= pass->size) {
            while (tok != NULL) {
                for (int i = 0; i < strlen(tok); i++) {
                    if (tok[i] == '\r') {
                        tok[i] = '\n';
                    }
                }

                if (is_substr(tok, pass)) {
                    for (int i = 0; i < strlen(pass->data); i++) {
                        pass->strength -= 5;
                    }
                    break;
                }

                tok = strtok(NULL, "\n");
            }
        }

        return (PASS) pass;
    }
}

PASS Update_Password(char* s) {
    if (s == NULL) {
        printf("s is NULL\n");
        exit(1);
    }
    password* bad = NULL;
    int strength = 0;
    char list[] = "1234567890qwertyuiopasdfghjklzxcvbnm!@#%^&*()_-+=QWERTYUIOPASDFGHJKLZXCVBNM[]{};:\"<>,.?/|";
    char arr[16] = {'a'};
    srand(time(0));

    while (strength < 200) {
        int i = 0;
        for (i = 0; i < 16; i++) {
            char count = list[rand() % (sizeof(list))];
            arr[i] = count;
        }
        arr[i] = '\0';

        password* pass = Password_Str(arr);

        pass = Password_Calc_Strength(pass, s);

        if (pass->strength > 180) {
            return (PASS) pass;
        } else {
            pass->strength = 0;
            free(pass->data);
            free(pass);
            strength = 0;
        }
    }
    return bad;
}

PASS Update_Password_By_Scramble(char* s, char* p) {
    if (s == NULL) {
        printf("s is NULL\n");
        exit(1);
    }
    if (p == NULL) {
        printf("p is NULL\n");
        exit(1);
    }
    MY_STRING str = my_string_init_c_string(p);

    char* s1 = my_string_c_str(str);
    srand(time(NULL)); int r; int x;

    int size = my_string_get_size(str);
    int i = 0;
    while (i < size) {
        r = rand() % size;
        x = rand() % size;

        if (r != x) {
            s1[r] ^= s1[x];
            s1[x] = s1[r] ^ s1[x];
            s1[r] ^= s1[x];
            i++;
        }
    }

    password* p2 = Password_Str(s1);
    my_string_destroy(&str);

    return (PASS) p2;
}

char* Password_C_Str(password* str) {
    password* pass = (password*)str;

    char* tmp = NULL;

    for (int i = 0; i < pass->size; i++) {
        tmp[i] = pass->data[i];
    }

    return tmp;
}

Boolean CheckSpaces(password* str) {
    password* pass = (password*)str;

    Boolean found_space = false;
    for (int i = 0; i < pass->size; i++) {
        found_space = found_space || (pass->data[i] == ' ');

        if (found_space) { break; }
    }

    return found_space;
}

Boolean is_substr(char* s, password* str) {
    password* pass = (password*)str;
    int slen = strlen(s);
    int strl = strlen(pass->data);

    int end = strl - slen + 1;

    for (int i = 0; i < end; i++) {
        bool found = 1;

        for (int j = 0; j < slen; j++) {
            if (s[j] != pass->data[i + j]) {
                found = false;
                break;
            }
        }

        if (found) { return true; }
    }

    return false;
}

Boolean CheckPassValid(password* str) {
    password* pass = (password*)str;

    Boolean found_lower = false, found_upper = false;
    Boolean found_num_char = false;
    Boolean found_spc_char = false;
    for (int i = 0; i < pass->size; i++) {
        found_lower = found_lower || (pass->data[i] >= 'a' && pass->data[i] <= 'z');
        found_upper = found_upper || (pass->data[i] >= 'A' && pass->data[i] <= 'Z');
        found_num_char = found_num_char || (pass->data[i] >= '0' && pass->data[i] <= '9');
        found_spc_char = found_spc_char || ((pass->data[i] >= '!' && pass->data[i] <= '/') ||
        (pass->data[i] >= ':' && pass->data[i] <= '@') ||
        (pass->data[i] >= '[' && pass->data[i] <= '`') ||
        (pass->data[i] >= '{' && pass->data[i] <= '~'));

        if (found_lower && found_upper && found_num_char && found_spc_char) {
            break;
        }
    }

    return (found_lower && found_upper && found_num_char && found_spc_char);
}

Boolean CheckAlpha(password* str) {
    password* pass = (password*)str;

    Boolean found_lower = false, found_upper = false;
    for (int i = 0; i < pass->size; i++) {
        found_lower = found_lower || (pass->data[i] >= 'a' && pass->data[i] <= 'z');
        found_upper = found_upper || (pass->data[i] >= 'A' && pass->data[i] <= 'Z');

        if (found_lower && found_upper) {
            break;
        }
    }

    return (found_lower && found_upper);
}

Boolean CheckNum(password* str) {
    password* pass = (password*)str;

    Boolean found_num_char = false;
    for (int i = 0; i < pass->size; i++) {
        found_num_char = found_num_char || (pass->data[i] >= '0' && pass->data[i] <= '9');

        if (found_num_char) { break; }
    }

    return found_num_char;
}

Boolean CheckSpecialChars(password* str) {
    password* pass = (password*)str;

    Boolean found_spc_char = false;
    for (int i = 0; i < pass->size; i++) {
        found_spc_char = found_spc_char || ((pass->data[i] >= '!' && pass->data[i] <= '/') ||
        (pass->data[i] >= ':' && pass->data[i] <= '@') ||
        (pass->data[i] >= '[' && pass->data[i] <= '`') ||
        (pass->data[i] >= '{' && pass->data[i] <= '~'));

        if (found_spc_char) { break; }
    }

    return (found_spc_char);
}

Boolean CheckSpecialChars2(char str) {

    Boolean found_spc_char = false;
    for (int i = 0; i < 1; i++) {
        found_spc_char = found_spc_char || ((str >= '!' && str <= '/') ||
        (str >= ':' && str <= '@') ||
        (str >= '[' && str <= '`') ||
        (str >= '{' && str <= '~'));

        if (found_spc_char) { break; }
    }

    return (found_spc_char);
}

int getSize(PASS pass) {
    password* str = (password*)pass;
    return str->size;
}

int getCapacity(PASS pass) {
    password* str = (password*)pass;
    return str->capacity;
}

int getStrength(PASS pass) {
    password* str = (password*)pass;
    return str->strength;
}

char* getData(PASS pass) {
    password* str = (password*)pass;
    return str->data;
}
