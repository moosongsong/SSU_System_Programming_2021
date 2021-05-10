#include "20170767.h"

int main() {
    MyHeap *heap = calloc(sizeof(MyHeap), 1);

    if (listInitialize(heap)) {
        return 0;
    }

    int menuNum = 0;
    bool result = false;

    while (1) {
        printf("Do you want to allocate data (1) or deallocate data (2)?\n");
        scanf("%d", &menuNum);

        switch (menuNum) {
            case 1:
                result = allocateData(heap);
                break;
            case 2:
                result = deallocateData(heap);
                break;
	    default:
		result = false;
		break;
        }
//        if (menuNum > 2) {
//            listFinalize(heap);
//           break;
//        }

        if (result == true) {
            dump_mem(heap->base, BYTE_SIZE);
//            listDisplay(heap);
        }

    }
    return 0;
}

void dump_mem(const void *mem, size_t len) {
    printf("Here is the mem dump!\n");
    const char *buffer = mem;
    size_t i;
    for (i = 0; i < len; i++) {
        if (i > 0 && i % 16 == 0) {
            printf("\n");
        }
        printf("%02x ", buffer[i] & 0xff);
    }
    puts("");
}

bool allocateData(MyHeap *heap) {
    char typeBuffer[BUFFSIZE] = {0,};
    char nameBuffer[BUFFSIZE] = {0,};

    printf("What is the type of data you want to allocate and what will be the name of the data?\n");
    scanf("%s %s", typeBuffer, nameBuffer);

    bool result = false;

    if(findNode(heap, nameBuffer)!=-1){
	    printf("%s is already exist.\n", nameBuffer);
	    return false;
    }

    if (strncmp(typeBuffer, STR_STRUCT, STR_STRUCT_LEN) == 0) {// struct
        result = insertStruct(heap, nameBuffer);
    } else if (strncmp(typeBuffer, STR_CHAR, STR_CHAR_LEN) == 0) {// char
        result = insertChar(heap, nameBuffer, OTHER_MODE);
    } else if (strncmp(typeBuffer, STR_INT, STR_INT_LEN) == 0) {//int
        result = insertInteger(heap, nameBuffer, OTHER_MODE);
    } else if (strncmp(typeBuffer, STR_DOUBLE, STR_DOUBLE_LEN) == 0) {//double
        result = insertDouble(heap, nameBuffer, OTHER_MODE);
    } else {
        result = false;
    }

    if(result == true){
	    printf("%s has been allocated successfully\n", nameBuffer);
    }else{
	    printf("allocation is failed\n");
    }

    return result;
}

bool deallocateData(MyHeap *heap) {

    if (heap->count == 0) {
        printf("Empty Heap\n");
        return false;
    }

    char nameBuffer[BUFFSIZE] = {0,};
    printf("What is the name of data you want to deallocate?\n");
    scanf("%s", nameBuffer);
    
    int result = removeAsName(heap, nameBuffer);

    if(result == true){
	    printf("%s has been deallocated successfully\n", nameBuffer);
    }else{
	    printf("deallocation is failed\n");
    }

    return result;
}

bool removeAsName(MyHeap *heap, char *name) {
    int index = 0;
    int result = false;

    while (1) {
        index = findNode(heap, name);

        char *tempChar;
        int *tempInt;
        double *tempDouble;

        if (index == -1) {
            break;
        }

        MetaDataNode *node = toNode(heap, index);
        MetaDataNode *nextNode = node->next;

        if (nextNode == heap->tail) {
            heap->now = (heap->now) - node->size;

            switch (node->size) {
                case 1:
                    tempChar = node->address;
                    *tempChar = 0;
                    break;
                case 4:
                    tempInt = node->address;
                    *tempInt = 0;
                    break;
                case 8:
                    tempDouble = node->address;
                    *tempDouble = 0;
                    break;
            }
        } else {
            void *tempStr = calloc(sizeof(char), BYTE_SIZE);
            memcpy(tempStr, (nextNode->address), BYTE_SIZE - (getSizeBeforeMe(heap, index) + node->size));
            memcpy((node->address), tempStr, BYTE_SIZE - getSizeBeforeMe(heap, index));
            modifyAddress(heap, node->size, index);
            heap->now = heap->now - node->size;
            free(tempStr);
        }
        listRemove(heap, index);
        result = true;
    }

    return result;
}

bool insertStruct(MyHeap *heap, char *name) {
    int howMany = 0;
    bool result = false;

    printf("How many data should be in the struct\n");
    scanf("%d", &howMany);

    char typeBuffer[BUFFSIZE] = {0,};
    printf("please specify each type and its value\n");

    for (int i = 0; i < howMany; ++i) {
        scanf("%s", typeBuffer);
        if (strncmp(typeBuffer, STR_CHAR, STR_CHAR_LEN) == 0) {// char
            result = insertChar(heap, name, STRUCT_MODE);
        } else if (strncmp(typeBuffer, STR_INT, STR_INT_LEN) == 0) {//int
            result = insertInteger(heap, name, STRUCT_MODE);
        } else if (strncmp(typeBuffer, STR_DOUBLE, STR_DOUBLE_LEN) == 0) {//double
            result = insertDouble(heap, name, STRUCT_MODE);
        } else {
            result = false;
        }

        if (result == false) {
            removeAsName(heap, name);
            return false;
        }
    }

    return true;
}

bool insertChar(MyHeap *heap, char *name, int mode) {
    char *charBuffer = heap->now;

    if (isItValid(heap, sizeof(char)) == false) {
        if (mode == STRUCT_MODE) {
            scanf(" %c", charBuffer);
        }
        return false;
    }

    if (mode == OTHER_MODE)
        printf("please specify a value for the data type\n");
    scanf(" %c", charBuffer);

    listAdd(heap, name, sizeof(char));

    return true;
}

bool insertInteger(MyHeap *heap, char *name, int mode) {
    int *intBuffer = heap->now;

    if (isItValid(heap, sizeof(int)) == false) {
        if (mode == STRUCT_MODE) {
            scanf("%d", intBuffer);
        }
        return false;
    }

    if (mode == OTHER_MODE)
        printf("please specify a value for the data type\n");

    scanf("%d", intBuffer);

    // heap item
    listAdd(heap, name, sizeof(int));

    return true;
}

bool insertDouble(MyHeap *heap, char *name, int mode) {
    double *doubleBuffer = heap->now;

    if (isItValid(heap, sizeof(double)) == false) {
        if (mode == STRUCT_MODE) {
            scanf("%lf", doubleBuffer);
        }
        return false;
    }

    if (mode == OTHER_MODE)
        printf("please specify a value for the data type\n");
    scanf("%lf", doubleBuffer);

    listAdd(heap, name, sizeof(double));

    return true;
}

bool isItValid(MyHeap *heap, int size) {
    if (heap->total_size >= BYTE_SIZE || (heap->total_size + size) >= BYTE_SIZE) {
        printf("There is not enough memory for the data, you can only use %d byte(s).\n",
               (BYTE_SIZE - heap->total_size));
        return false;
    }
    return true;
}

static MetaDataNode *toNode(MyHeap *heap, int index) {
    MetaDataNode *node = NULL;

    if (index < (heap->count >> 1)) {
        node = heap->head->next;
        for (int i = 0; i < index; i++)
            node = node->next;
    } else {
        node = heap->tail->prev;
        for (int i = heap->count - 1; i > index; i--)
            node = node->prev;
    }
    return node;
}

bool listInitialize(MyHeap *heap) {
    heap->head = malloc(sizeof(MetaDataNode));

    if (heap->head == NULL)
        return false;

    heap->tail = malloc(sizeof(MetaDataNode));
    if (heap->tail == NULL) {
        free(heap->head);
        return false;
    }

    heap->head->next = heap->tail;
    heap->head->prev = heap->head;

    heap->tail->prev = heap->head;
    heap->tail->next = heap->tail;

    heap->count = 0;
    heap->total_size = 0;

    heap->base = calloc(BYTE_SIZE, sizeof(char));
    heap->now = heap->base;

    return 0;
}

bool listAdd(MyHeap *heap, char *name, int size) {
    MetaDataNode *node = malloc(sizeof(MetaDataNode));

    if (node == NULL)
        return false;

    strcpy(node->name, name);
    node->address = heap->now;
    node->size = size;
    node->next = heap->tail;
    node->prev = heap->tail->prev;

    heap->tail->prev->next = node;
    heap->tail->prev = node;

    (heap->count)++;
    heap->total_size = heap->total_size + size;
    heap->now = heap->now + size;

    return true;
}

bool listRemove(MyHeap *heap, int index) {
    MetaDataNode *target = toNode(heap, index);

    if (target == NULL)
        return false;

    MetaDataNode *prev = target->prev;
    MetaDataNode *next = target->next;

    prev->next = next;
    next->prev = prev;

    free(target);
    (heap->count)--;
    heap->total_size = heap->total_size - (target->size);

    return true;
}

void listDisplay(MyHeap *heap) {
    printf("[head]");
    for (MetaDataNode *cur = heap->head->next; cur != heap->tail; cur = cur->next)
        printf("<->[%s, %p, %d]", cur->name, cur->address, cur->size);
    printf("<->[tail]\n");
}

int listFinalize(MyHeap *heap) {
    if (heap->head == NULL || heap->tail == NULL)
        return -1;

    while (heap->head->next != heap->tail) {
        MetaDataNode *target = heap->head->next;
        heap->head->next = target->next;
        free(target);
    }

    free(heap->base);
    free(heap->head);
    free(heap->tail);
    return 0;
}

int findNode(MyHeap *heap, char *name) {
    int index = 0;

    for (MetaDataNode *cur = heap->head->next; cur != heap->tail; cur = cur->next) {
        if (strcmp(name, cur->name) == 0)
            return index;
        index++;
    }
    return -1;
}

int getSizeBeforeMe(MyHeap *heap, int index) {
    MetaDataNode *node;
    int size = 0;

    node = heap->head->next;

    for (int i = 0; i < index; i++) {
        size = size + (node->size);
        node = node->next;
    }

    return size;
}

void modifyAddress(MyHeap *heap, int sizeOfDelete, int startIndex) {
    MetaDataNode *node = heap->head->next;

    for (int i = 0; i < startIndex; i++)
        node = node->next;

    for (MetaDataNode *cur = node; cur != heap->tail; cur = cur->next) {
        node->address = node->address - sizeOfDelete;
        node = node->next;
    }
}
