//
// Created by 20170767 on 2021-03-16.
//

#ifndef HW1_20170767_H
#define HW1_20170767_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

#define CHAR_SIZE 8
#define INT_SIZE 32
#define FLOAT_SIZE 32
#define DOUBLE_SIZE 64

union unionForFloat{
    float sample1;
    int sample2;
};

union unionForDouble{
    double sample1;
    long long sample2;
};

int stringToSignedChar(FILE * fp);
int stringToAscii(FILE * fp);
int stringToUnsignedChar(FILE * fp);
int stringToSignedInt(FILE * fp);
int stringToUnsignedInt(FILE * fp);
int stringToFloat(FILE * fp);
int stringToDouble(FILE * fp);

#endif //HW1_20170767_H
