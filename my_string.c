#include "my_string.h"
#include "status.h"

MY_STRING my_string_init_default(void){
  my_string* pMy_string;

  pMy_string = (my_string*) malloc(sizeof(my_string));

  if (pMy_string){
     pMy_string->size = 0;
     pMy_string->capacity = 7;
     pMy_string->data = (char*) malloc(sizeof(char) * pMy_string->capacity);

     if (pMy_string->data == NULL) {
        printf("Did not work\n");
        return NULL;
     }
  }

  if (!pMy_string->data || pMy_string->data == NULL){
     free(pMy_string);
  }

  if (pMy_string == NULL){
      free(pMy_string);
      exit(1);
  }

  return (MY_STRING) pMy_string;
}

void my_string_destroy(MY_STRING* pItem){
    // check if item is not null before freeing	  
    my_string* pMy_string = (my_string*) *pItem;

    if (pItem == NULL) {
        return;
    }

    free(pMy_string->data);
    free(pMy_string);
    pMy_string = NULL;
}

MY_STRING my_string_init_c_string(const char* c_string){
    my_string* string_o;
    int i = 0;

    string_o = (my_string*) malloc(sizeof(my_string));
    
    if (string_o){
        string_o->size = strlen(c_string);
        string_o->capacity = strlen(c_string) + 1;
        string_o->data = (char*) malloc(sizeof(char) * string_o->capacity);

        if (string_o->data == NULL) {
            printf("Did not work\n");
            return NULL;
        }

        for (i = 0; i < string_o->capacity; i++) {
            string_o->data[i] = c_string[i];
        }
    }

    if (string_o == NULL){
        free(string_o);
        exit(1);
    }

    return (MY_STRING) string_o;
}

int my_string_get_size(MY_STRING hMy_string){
    my_string* pMy_string = (my_string *) hMy_string;
    return pMy_string->size;
}

int my_string_get_capacity(MY_STRING hMy_string){
    my_string* pMy_string = (my_string *) hMy_string;
    return pMy_string->capacity;
}

int my_string_compare(MY_STRING hLeft_string, MY_STRING hRight_string){
    int i = 0;
    int n = 0;
    int j = 0;

    my_string* s1 = ((my_string*)hLeft_string);
    my_string* s2 = ((my_string*)hRight_string);
   
    for (i = 0; n < s1->size - 1; i++) {
        n++;
        if (n == s1->size - 1) {
            break;
        }
    }

    for (i = 0; j < s2->size - 1; i++) {
        j++;
        if (j == s2->size - 1) { 
            break;
        }
    }
    
    i = 0;

    while ((s1->data[i] != '\0') && (s2->data[i] != '\0')) {
        if (s1->data[i] == s2->data[i]) {
            i++;
        }
        else {
            return n - j;
        }
    }

    return n - j;
}

Status my_string_insertion(MY_STRING hMy_string, FILE* fp){
    my_string* pMy_string = (my_string*)hMy_string;
    if (pMy_string) {
        if (pMy_string->data){
               for (int i = 0; i < pMy_string->size; i++) {
                   fprintf(fp, "%c", pMy_string->data[i]);
                   if (pMy_string->data[i] == '\0') {
                       break;
                   }
               }
        }
        if (pMy_string->data == NULL) {
            free(pMy_string->data);
            free(pMy_string);
            return FAILURE;
        }
    }

    if (pMy_string == NULL) {
        return FAILURE;
    }

    return SUCCESS;
}

Status my_string_extraction(MY_STRING hMy_string, FILE* fp){
    my_string* pMy_string = (my_string*)hMy_string;
    int ret;
    int i = 0;
    int j = 0;
    int twice = 2;
    char ch = 'a';
    char *arr = NULL;

    ret = fscanf(fp, "%c", &ch);

    if (ch == ' ' || ch == '\n') {
        ungetc(ch, fp);
    }

    while (ret != EOF && ch != ' ' && ch != '\n' && ch != '\t') {
        // Check if the array needs to be resized, if it does resize operation. Then put char in array
        if (pMy_string->size >= pMy_string->capacity) {
            arr = (char*)malloc(sizeof(char) * pMy_string->capacity * twice);
            if (arr == NULL) {
                return FAILURE;
            }
            for (i = 0; i < pMy_string->size; i++) {
                arr[i] = pMy_string->data[i];
            }

            free(pMy_string->data);
            pMy_string->data = arr;
            pMy_string->capacity *= twice;
        }

        pMy_string->data[j] = ch;
        j++;
        pMy_string->size = j;
        ret = fscanf(fp, "%c", &ch);
    }

    if (pMy_string == NULL) {
        return FAILURE;
    }

    if (j == 0) {
        return FAILURE;
    }


    return SUCCESS;
}

Status my_string_push_back(MY_STRING hMy_string, char item){
    my_string* pMy_string = (my_string*)hMy_string;
    char* temp;
    int i;
    int twice = 2;

    if (pMy_string->size >= pMy_string->capacity){
        temp = (char*)malloc(sizeof(char) * pMy_string->capacity * twice);

        if (temp == NULL) {
            return FAILURE;
        }

    	for (i = 0; i < pMy_string->size; i++){
		    temp[i] = pMy_string->data[i];
	    }

	free(pMy_string->data);
	pMy_string->capacity *= twice;
	pMy_string->data = temp;

    }

    pMy_string->data[pMy_string->size] = item;
    pMy_string->size++;
    return SUCCESS;
}

Status my_string_pop_back(MY_STRING hMy_string){
    my_string* pMy_string = (my_string*)hMy_string;
    int i = pMy_string->size;

    if (pMy_string->size <= 0) {
        return FAILURE;
    }

    pMy_string->data[i - 1] = '\0';
    pMy_string->size--;
    return SUCCESS;
}

char* my_string_at(MY_STRING hMy_string, int index){
    my_string* pMy_string = (my_string*)hMy_string;

    if(index < 0 || index >= pMy_string->size){
        return NULL;
    }

    return &(pMy_string->data[index]);
}

char* my_string_c_str(MY_STRING hMy_string){
    my_string* pMy_string = (my_string*)hMy_string;
    char* temp = NULL;

    if (pMy_string->size >= pMy_string->capacity) {
        temp = (char*)malloc(sizeof(char) * pMy_string->capacity + 1);
        temp = pMy_string->data;
        free(pMy_string->data);
        pMy_string->data = temp;
    }

    pMy_string->data[pMy_string->size] = '\0';
    return my_string_at(pMy_string, 0);
}

Boolean my_string_empty(MY_STRING hMy_string){
   my_string* pMy_string = (my_string*)hMy_string;

   if(pMy_string->size == 0 || pMy_string->data == NULL){
      return TRUE;
   }
   
   return FALSE;
}

Status my_string_concat(MY_STRING hResult, MY_STRING hAppend) {
    my_string* pResult = (my_string*)hResult;
    my_string* pAppend = (my_string*)hAppend;
    int i = 0;

    if (pResult->capacity <= (pResult->size + pAppend->size)) {

        pResult->data = (char*)realloc(pResult->data, sizeof(char) * (pResult->size + pAppend->size + 1));

        if (pResult->data == NULL) {
            return FAILURE;
        }

        pResult->capacity = pResult->size + pAppend->size + 1;
    }

    int OldSize = pResult->size;
    for (i = 0; i < pAppend->size; i++) {
        pResult->data[pResult->size] = pAppend->data[i];
        pResult->size++;
    }

    if (pResult->size != OldSize + pAppend->size) {
        return FAILURE;
    }

    return SUCCESS;
}

Status my_string_assignment(MY_STRING* pLeft, MY_STRING Right) {
    my_string* pRight = (my_string*)Right;
    my_string* ppLeft = (my_string*)*pLeft;
    int i;

    if (ppLeft == NULL) {
        my_string* pTemp;

        pTemp = (my_string*)malloc(sizeof(my_string));

        if (pTemp) {
            pTemp->size = 0;
            pTemp->capacity = pRight->capacity;
            pTemp->data = (char*)malloc(sizeof(char) * pTemp->capacity);

            if (pTemp->data == NULL) {
                printf("Did not work\n");
                return FAILURE;
            }
        }

        for (i = 0; i < pRight->size; i++) {
            pTemp->data[i] = pRight->data[i];
            pTemp->size++;
        }
        
        ppLeft = pTemp;
        *pLeft = ppLeft;
    }

    else {
        int i = 0;
        if (pRight->size >= ppLeft->capacity) {
            while (pRight->size >= ppLeft->capacity) {
                ppLeft->data = (char*)realloc(ppLeft->data, sizeof(char) * pRight->capacity * 2);

                if (ppLeft->data == NULL) {
                    return FAILURE;
                }

                ppLeft->capacity *= 2;
            }
            return SUCCESS;
        }
        else {
            for (i = 0; i < pRight->size; i++) {
                ppLeft->data[i] = pRight->data[i];
                ppLeft->size = i;
            }
        }
    }

    return SUCCESS;
}

Boolean my_string_equal(MY_STRING hLeft_string, MY_STRING hRight_string) {
    int i = 0;
    int n = 0;
    int j = 0;

    my_string* s1 = ((my_string*)hLeft_string);
    my_string* s2 = ((my_string*)hRight_string);
   
    for (i = 0; n < s1->size - 1; i++) {
        n++;
        if (n == s1->size - 1) {
            break;
        }
    }

    for (i = 0; j < s2->size - 1; i++) {
        j++;
        if (j == s2->size - 1) { 
            break;
        }
    }

    i = 0;
    int x = 0;

    if (n != j) {
        return FALSE;
    }

    while (i != s1->size) {
        if (s1->data[i] == s2->data[i]) {
            i++;
        }
        else {
            i++;
            x++;
        }
    }

    if (x == 0) {
        return TRUE;
    }

    return FALSE;
}