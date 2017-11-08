#ifndef OPCODEFUNC_H
#define OPCODEFUNC_H

#include <stdio.h>

typedef enum {
    NEGA = 1,
    ZERO = 1<<1,
    DSEN = 1<<2
} FLAGS;

void updateRfromV(char* REG, int val);
void updateRfromR(char * REG1, char * REG2);
int* getR(char* reg);
void fetch(char* instruction, int start, int end, char* value);
void intToBinary(int n, char* value, int size);
int binaryToInt(char binary[]);

void callMOV0(char *, FILE *, int *);
void callMOV1(char *, FILE *, int *);
void callLDB0(char *, FILE *, int *);
void callLDB1(char *, FILE *, int *);
void callLDW0(char *, FILE *, int *);
void callLDW1(char *, FILE *, int *);
void callSTB0(char *, FILE *, int *);
void callSTB1(char *, FILE *, int *);
void callSTW0(char *, FILE *, int *);
void callSTW1(char *, FILE *, int *);
void callADD0(char *, FILE *, int *);
void callADD1(char *, FILE *, int *);
void callSUB0(char *, FILE *, int *);
void callSUB1(char *, FILE *, int *);
void callMUL0(char *, FILE *, int *);
void callMUL1(char *, FILE *, int *);
void callDIV0(char *, FILE *, int *);
void callDIV1(char *, FILE *, int *);
void callCMP0(char *, FILE *, int *);
void callCMP1(char *, FILE *, int *);
void callCAL0(char *, FILE *, int *);
void callINC0(char *, FILE *, int *);
void callDEC0(char *, FILE *, int *);
void callJMP0(char *, FILE *, int *);
void callJMP1(char *, FILE *, int *);
void callJLT0(char *, FILE *, int *);
void callJGT1(char *, FILE *, int *);
void callJGT0(char *, FILE *, int *);
void callJGT1(char *, FILE *, int *);
void callJEQ0(char *, FILE *, int *);
void callJEQ1(char *, FILE *, int *);
void callJNE0(char *, FILE *, int *);
void callJNE1(char *, FILE *, int *);
void callPSH0(char *, FILE *, int *);
void callPSH1(char *, FILE *, int *);
void callPOP0(char *, FILE *, int *);
void callPOP1(char *, FILE *, int *);
void callRET0(char *, FILE *, int *);
void callMSF0(char *, FILE *, int *);
void POP(FILE *);
void display(char* , int *);

#endif