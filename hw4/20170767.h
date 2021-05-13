#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define BUFFSIZE 100
#define BYTE_SIZE 256

#define STR_INT "int"
#define STR_INT_LEN 3
#define STR_DOUBLE "double"
#define STR_DOUBLE_LEN 6
#define STR_CHAR "char"
#define STR_CHAR_LEN 4
#define STR_STRUCT "struct"
#define STR_STRUCT_LEN 6

#define STRUCT_MODE 100
#define OTHER_MODE 200

typedef struct _Node_ {
    char name[BUFFSIZE];
    void *address;
    int size;

    struct _Node_ *next;
    struct _Node_ *prev;
} MetaDataNode;

typedef struct _Heap_ {
    MetaDataNode *head;
    MetaDataNode *tail;
    int count;
    int total_size;
    void *base;
    void *now;
} MyHeap;

void dump_mem(const void *mem, size_t len);

bool allocateData(MyHeap *heap);

bool deallocateData(MyHeap *heap);

bool removeAsName(MyHeap * heap, char * name);

bool insertStruct(MyHeap *heap, char *name);

bool insertChar(MyHeap *heap, char *name, int mode);

bool insertInteger(MyHeap *heap, char *name, int mode);

bool insertDouble(MyHeap *heap, char *name, int mode);

bool isItValid(MyHeap *heap, int size);

bool listInitialize(MyHeap *heap);

static MetaDataNode *toNode(MyHeap *heap, int index);

bool listAdd(MyHeap *heap, char *name, int size);

int listFinalize(MyHeap *heap);

bool listRemove(MyHeap *heap, int index);

void listDisplay(MyHeap *heap);

int findNode(MyHeap *heap, char *name);

int getSizeBeforeMe(MyHeap *heap, int index);

void modifyAddress(MyHeap *heap, int sizeOfDelete, int startIndex);