#include "opcodefunc.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define PAGE_SIZE 512

int GP[15] = {0};
int SP[3] = {0};

unsigned int MAR = 0, MDR = 0, STP = 0, ALUSIG = 0;

void updateRfromV(char* REG, int val)
{
	*getR(REG) = val;
}

void updateRfromR(char * REG1, char * REG2)
{
	*getR(REG1) = *getR(REG2);
}

int* getR(char* reg)
{
    char reg1[5];
    fetch(reg,1,4,reg1);
    int regIndex=binaryToInt(reg1);
    if(reg[0]=='0')
        return &(SP[regIndex]);
    else
        return &(GP[regIndex]);
}

void fetch(char* instruction, int start, int end, char* value)
{
    strncpy(value,instruction+start,end-start+1);
    value[end-start+1]='\0';
}

void intToBinary(int n, char* value, int size)
{
	int co;
	for(co = 0; co < size ; co++)
		value[co] = '0';
	value[co] = '\0';
    int remainder;
 	int j = size - 1;
    while(n != 0) {
        value[j] = 48 + n%2;
        j--;
        n = n/2;
    }
}

int binaryToInt(char binary[])
{
    int length=strlen(binary);
    int integer=0,i;
    for(i=0;i<strlen(binary);i++)
    {
        integer+=(binary[i]-48)*pow(2,length-i-1);
    }
    return integer;
}

void callMOV0(char * IR,FILE* mptr, int* PC)
{
	char REG[6];
	fetch(IR,5,9,REG);

	char VALUE[7];
	fetch(IR,10,15,VALUE);

	updateRfromV(REG,binaryToInt(VALUE));
}

void callMOV1(char *IR, FILE* mptr, int* PC){

    char REG1[6];
    fetch(IR, 5, 9, REG1);

    char REG2[6];
    fetch(IR, 10, 14, REG2);

    updateRfromR(REG1,REG2);

}

void callLDB0(char *IR, FILE *mptr, int* PC)
{
    char REG[6];
    char VALUE[7];
    char VALUE2[9];
    fetch(IR, 5, 9, REG);
    fetch(IR, 10, 15, VALUE);
    MAR = PAGE_SIZE+binaryToInt(VALUE);

    int cur_pos = ftell(mptr);
    fseek(mptr,MAR*8,SEEK_SET);
    fread(VALUE2, 1, 8, mptr);
    VALUE2[8] = '\0';

    MDR = binaryToInt(VALUE2);

    updateRfromV(REG,MDR);
    fseek(mptr, cur_pos, SEEK_SET);
}

void callLDB1(char *IR, FILE *mptr, int* PC)
{
    char REG1[6];
    char REG2[6];
    char VALUE[9];

    fetch(IR, 5, 9, REG1);
    fetch(IR, 10, 14, REG2);

    MAR = PAGE_SIZE+*getR(REG2);
    int cur_pos = ftell(mptr);
    fseek(mptr,MAR*8,SEEK_SET);
    fread(VALUE, 1, 8, mptr);
    VALUE[8] = '\0';

    MDR = binaryToInt(VALUE);
    updateRfromV(REG1,MDR);

    fseek(mptr, cur_pos, SEEK_SET);
}

void callLDW0(char* IR, FILE* mptr, int* PC)
{
    char REG[6];
    char VALUE[7];
    char VALUE2[17];
    fetch(IR, 5, 9, REG);
    fetch(IR, 10, 15, VALUE);

    MAR = PAGE_SIZE+binaryToInt(VALUE);

    int cur_pos = ftell(mptr);
    fseek(mptr,MAR*8,SEEK_SET);
    fread(VALUE2, 1, 16, mptr);
    VALUE2[16] = '\0';

    MDR = binaryToInt(VALUE2);

    updateRfromV(REG,MDR);
    fseek(mptr, cur_pos, SEEK_SET);
}

void callLDW1(char* IR, FILE* mptr, int* PC)
{
	char REG1[6];
    char REG2[6];
    char VALUE[17];

    fetch(IR, 5, 9, REG1);
    fetch(IR, 10, 14, REG2);

    MAR = PAGE_SIZE+*getR(REG2);
    int cur_pos = ftell(mptr);
    fseek(mptr,MAR*8,SEEK_SET);
    fread(VALUE, 1, 16, mptr);
    VALUE[16] = '\0';

    MDR = binaryToInt(VALUE);
    updateRfromV(REG1,MDR);

    fseek(mptr, cur_pos, SEEK_SET);
}

void callADD0(char* IR,FILE* mptr, int* PC)
{
    char REG[6];
    fetch(IR,5,9,REG);

    char VALUE[7];
    fetch(IR,10,15,VALUE);

    updateRfromV(REG,binaryToInt(VALUE)+(*getR(REG)));
}

void callADD1(char* IR,FILE* mptr, int* PC)
{
    char REG[6];
    char REG1[6];
    fetch(IR,5,9,REG);
    fetch(IR,10,14,REG1);

    updateRfromV(REG,(*getR(REG1))+(*getR(REG)));
}

void callSUB0(char* IR,FILE* mptr, int* PC)
{
    char REG[6];
    fetch(IR,5,9,REG);

    char VALUE[7];
    fetch(IR,10,15,VALUE);

    updateRfromV(REG,(*getR(REG))-binaryToInt(VALUE));
}

void callMUL0(char* IR,FILE* mptr, int* PC)
{
    char REG[6];
    fetch(IR,5,9,REG);

    char VALUE[7];
    fetch(IR,10,15,VALUE);

    updateRfromV(REG,(*getR(REG))*binaryToInt(VALUE));
}

void callDIV0(char* IR,FILE* mptr, int* PC)
{
    char REG[6];
    fetch(IR,5,9,REG);

    char VALUE[7];
    fetch(IR,10,15,VALUE);

    updateRfromV(REG,(*getR(REG))/binaryToInt(VALUE));
}

void callSUB1(char* IR,FILE* mptr, int* PC)
{
    char REG[6];
    char REG1[6];
    fetch(IR,5,9,REG);
    fetch(IR,10,14,REG1);

    updateRfromV(REG,(*getR(REG))-(*getR(REG1)));
}

void callMUL1(char* IR,FILE* mptr, int* PC)
{
    char REG[6];
    char REG1[6];
    fetch(IR,5,9,REG);
    fetch(IR,10,14,REG1);

    updateRfromV(REG,(*getR(REG))*(*getR(REG1)));
}

void callDIV1(char* IR,FILE* mptr, int* PC)
{
    char REG[6];
    char REG1[6];
    fetch(IR,5,9,REG);
    fetch(IR,10,14,REG1);

    updateRfromV(REG,(*getR(REG))/(*getR(REG1)));
}

void callINC0(char* IR,FILE* mptr, int* PC)
{
    char REG[6];
    fetch(IR,5,9,REG);

    updateRfromV(REG,1+(*getR(REG)));
}

void callDEC0(char* IR,FILE* mptr, int* PC)
{
    char REG[6];
    fetch(IR,5,9,REG);

    updateRfromV(REG,(*getR(REG))-1);
}

void callJMP0(char *IR, FILE *mptr, int *PC)
{
    char VALUE[7];
    fetch(IR,10,15,VALUE);
    *PC=binaryToInt(VALUE);
}

void callJMP1(char *IR, FILE *mptr, int *PC)
{
    char REG[6];
    fetch(IR,10,14,REG);
    *PC = *getR(REG);
}

void callJLT0(char *IR, FILE *mptr, int *PC)
{
	char VALUE[7];
    fetch(IR,10,15,VALUE);
    int flags = SP[2];

    // masking to check if negative bit is on
    if(flags & NEGA)
    {
    	*PC = binaryToInt(VALUE);
    }
}

void callJLT1(char *IR, FILE *mptr, int *PC)
{
	char REG[6];
    fetch(IR,10,14,REG);
    int flags = SP[2];

    // masking to check if negative bit is on
    if(flags & NEGA)
    {
    	*PC = *getR(REG);
    }
}

void callMSF0(char *IR, FILE *mptr, int *PC)
{
    int i =1;
    char cur_value[17], next_value[17];
    int prev_ins = ftell(mptr);
    fseek(mptr, 2*PAGE_SIZE*8, SEEK_SET);
    fread(cur_value, 1, 16, mptr);
    cur_value[16] = '\0';
    while(i<512){
        fread(next_value, 1, 16, mptr);
        next_value[16]='\0';
        fseek(mptr, -16, SEEK_CUR);
        fwrite(cur_value, 1, 16, mptr);
        strcpy(cur_value, next_value);
        i++;
    }
    fseek(mptr, prev_ins, SEEK_SET);
}

void callJGT0(char *IR, FILE *mptr, int *PC)
{
    char VALUE[7];
    fetch(IR,10,15,VALUE);
    int flags = SP[2];

    // masking to check if negative bit is on
    if(!(flags & NEGA) && !(flags & ZERO))
    {
        *PC = binaryToInt(VALUE);
    }
}

void callJGT1(char *IR, FILE *mptr, int *PC)
{
    char REG[6];
    fetch(IR,10,14,REG);
    int flags = SP[2];

    // masking to check if zero bit is on
    if(!(flags & NEGA) && !(flags & ZERO))
    {
        *PC = *getR(REG);
    }
}

void callJEQ0(char *IR, FILE *mptr, int *PC)
{
    char VALUE[7];
    fetch(IR,10,15,VALUE);
    int flags = SP[2];

    // masking to check if zero bit is on
    if(flags & ZERO)
    {
        *PC = binaryToInt(VALUE);
    }
}

void callJEQ1(char *IR, FILE *mptr, int *PC)
{
    char REG[6];
    fetch(IR,10,14,REG);
    int flags = SP[2];

    // masking to check if zero bit is on
    if(flags & ZERO)
    {
        *PC = *getR(REG);
    }
}

void callJNE0(char *IR, FILE *mptr, int *PC)
{
    char VALUE[7];
    fetch(IR,10,15,VALUE);
    int flags = SP[2];

    // masking to check if zero bit is off
    if(!(flags & ZERO))
    {
        *PC = binaryToInt(VALUE);
    }
}

void callJNE1(char *IR, FILE *mptr, int *PC)
{
    char REG[6];
    fetch(IR,10,14,REG);
    int flags = SP[2];

    // masking to check if zero bit is off
    if(!(flags & ZERO))
    {
        *PC = *getR(REG);
    }
}

void callPSH0(char *IR, FILE *mptr, int *PC)
{
    char VALUE[7];
    char VALUE2[17];
    fetch(IR, 10, 15, VALUE);

    // push given value to STACK
    int prev_ins = ftell(mptr);
    intToBinary(binaryToInt(VALUE),VALUE2,16);
    callMSF0(IR, mptr, PC);
    fseek(mptr, 2*PAGE_SIZE*8, SEEK_SET);
    fwrite(VALUE2, 1, 16, mptr);
    fseek(mptr, prev_ins, SEEK_SET);

    // update STP
    STP = binaryToInt(VALUE);
}

void callPSH1(char *IR, FILE *mptr, int *PC)
{
    char REG1[6];
    char regValue[17];
    fetch(IR, 10, 15, REG1);

    // push value of reg to STACK
    int prev_ins = ftell(mptr);
    int val = *getR(REG1);
    intToBinary(val, regValue, 16);
    callMSF0(IR, mptr, PC);
    fseek(mptr, 2*PAGE_SIZE*8, SEEK_SET);
    fwrite(regValue,1,16,mptr);
    fseek(mptr, prev_ins, SEEK_SET);

    // update value of STP
    STP = val;
}

void callCAL0(char *IR, FILE *mptr, int *PC)
{
    char VALUE[7];
    char VALUE2[17];
    fetch(IR,10,15,VALUE);

    // push value of PC to STACK
    int prev_ins = ftell(mptr);
    fseek(mptr,2*PAGE_SIZE*8,SEEK_SET);
    intToBinary(*PC,VALUE2,16);
    fwrite(VALUE2,1,16,mptr);
    fseek(mptr,prev_ins,SEEK_SET);

    // update STP and PC
    STP = *PC;
    *PC = binaryToInt(VALUE);
}

void  callSTB0(char *IR, FILE *mptr, int *PC)
{
	char VALUE[7];
	fetch(IR,5,10,VALUE);
	char REG[6];
	fetch(IR,11,15,REG);

	MAR = PAGE_SIZE+binaryToInt(VALUE);
	MDR = *getR(REG);

	int cur_pos = ftell(mptr);

	char VALUE2[9];
	intToBinary(binaryToInt(VALUE),VALUE2,8);

	fseek(mptr,MAR*8,SEEK_SET);
	fwrite(VALUE2,1,8,mptr);
	fseek(mptr,cur_pos,SEEK_SET);
}

void callSTB1(char *IR, FILE *mptr, int* PC)
{
    char REG1[6];
    char REG2[6];
    char VALUE[9];

    fetch(IR, 5, 9, REG1);
    fetch(IR, 10, 14, REG2);

    MAR = PAGE_SIZE+*getR(REG1);
	MDR = *getR(REG2);

	intToBinary(MDR,VALUE,8);

    int cur_pos = ftell(mptr);
    fseek(mptr,MAR*8,SEEK_SET);
    fread(VALUE, 1, 8, mptr);
    VALUE[8] = '\0';
    fseek(mptr, cur_pos, SEEK_SET);
}

void POP(FILE *mptr)
{
    int i = 1;
    int prev_ins = ftell(mptr);
    char value[17];
    fseek(mptr, (2*PAGE_SIZE+16)*8, SEEK_SET);
    while(i<512){
        fread(value, 1, 16, mptr);
        value[16] = '\0';
        fseek(mptr, -32, SEEK_CUR);
        fwrite(value, 1, 16, mptr);
        fseek(mptr, 16, SEEK_CUR);
        i++;
    }
    fseek(mptr, prev_ins, SEEK_SET);
}

void callPOP0(char *IR, FILE *mptr, int *PC)
{
    char VALUE1[7];
    fetch(IR, 10, 15, VALUE1);
    char VALUE2[17];

    // read top value
    int prev_ins = ftell(mptr);
    fseek(mptr, 2*PAGE_SIZE*8, SEEK_SET);
    fread(VALUE2, 1, 16, mptr);
    VALUE2[16] = '\0';
    fseek(mptr, prev_ins, SEEK_SET);

    // pop top value
    POP(mptr);


    MAR = PAGE_SIZE+binaryToInt(VALUE1);
    MDR = binaryToInt(VALUE2);

    // write value poped to memory
    prev_ins = ftell(mptr);
    fseek(mptr, MAR*8, SEEK_SET);
    fwrite(VALUE2, 1, 16, mptr);
    fseek(mptr, prev_ins, SEEK_SET);

    // give new value to STP
    prev_ins = ftell(mptr);
    fseek(mptr, 2*PAGE_SIZE*8, SEEK_SET);
    fread(VALUE2, 1, 16, mptr);
    VALUE2[16] = '\0';
    fseek(mptr, prev_ins, SEEK_SET);
    STP = binaryToInt(VALUE2);

}

void callPOP1(char *IR, FILE *mptr, int *PC)
{
    char REG1[6];
    fetch(IR, 10, 14, REG1);
    char VALUE2[17];

    // read top value
    int prev_ins = ftell(mptr);
    fseek(mptr, 2*PAGE_SIZE*8, SEEK_SET);
    fread(VALUE2, 1, 16, mptr);
    VALUE2[16] = '\0';
    fseek(mptr, prev_ins, SEEK_SET);

    // pop top value
    POP(mptr);

    // update reg value with popped value
    *getR(REG1) = binaryToInt(VALUE2);

    // give new value to STP
    prev_ins = ftell(mptr);
    fseek(mptr, 2*PAGE_SIZE*8, SEEK_SET);
    fread(VALUE2, 1, 16, mptr);
    VALUE2[16] = '\0';
    fseek(mptr, prev_ins, SEEK_SET);
    STP = binaryToInt(VALUE2);

}

void callRET0(char *IR, FILE *mptr, int *PC)
{
    char VALUE2[17];

    // read top value
    int prev_ins = ftell(mptr);
    fseek(mptr, 2*PAGE_SIZE*8, SEEK_SET);
    fread(VALUE2, 1, 16, mptr);
    VALUE2[16] = '\0';
    fseek(mptr, prev_ins, SEEK_SET);

    // pop top value
    POP(mptr);

    // update PC with popped value
    *PC = binaryToInt(VALUE2);

    // give new value to STP
    prev_ins = ftell(mptr);
    fseek(mptr, 2*PAGE_SIZE*8, SEEK_SET);
    fread(VALUE2, 1, 16, mptr);
    VALUE2[16] = '\0';
    fseek(mptr, prev_ins, SEEK_SET);
    STP = binaryToInt(VALUE2);

}

void  callSTW0(char *IR, FILE *mptr, int *PC)
{
	char VALUE[7];
	fetch(IR,5,10,VALUE);
	char REG[6];
	fetch(IR,11,15,REG);

	MAR = PAGE_SIZE+binaryToInt(VALUE);
	MDR = *getR(REG);

	int cur_pos = ftell(mptr);

	char VALUE2[16];
	intToBinary(binaryToInt(VALUE),VALUE2,16);

	fseek(mptr,MAR*8,SEEK_SET);
	fwrite(VALUE2,1,16,mptr);
	fseek(mptr,cur_pos,SEEK_SET);
}

void callSTW1(char *IR, FILE *mptr, int* PC)
{
    char REG1[6];
    char REG2[6];
    char VALUE[17];

    fetch(IR, 5, 9, REG1);
    fetch(IR, 10, 14, REG2);

    MAR = PAGE_SIZE+*getR(REG1);
	MDR = *getR(REG2);

	intToBinary(MDR,VALUE,16);

    int cur_pos = ftell(mptr);
    fseek(mptr,MAR*8,SEEK_SET);
    fread(VALUE, 1, 16, mptr);
    VALUE[16] = '\0';

    fseek(mptr, cur_pos, SEEK_SET);
}

void callCMP0(char *IR, FILE *mptr, int* PC)
{
	char REG[6];
	char VALUE[7];
	fetch(IR,5,9,REG);
	fetch(IR,10,15,VALUE);

	int val = binaryToInt(VALUE);
	int regval = *getR(REG);

	if(regval - val < 0)
    {
		SP[2] = SP[2] | NEGA;
        SP[2] = SP[2] & ~ZERO;
    }
	else if(regval - val == 0)
    {
		SP[2] = SP[2] | ZERO;
        SP[2] = SP[2] & ~NEGA;
    }
    else
    {
        SP[2] = SP[2] & ~ZERO;
        SP[2] = SP[2] & ~NEGA;
    }
}

void callCMP1(char *IR, FILE *mptr, int* PC)
{
	char REG1[6];
	char REG2[6];
	fetch(IR,5,9,REG1);
	fetch(IR,10,15,REG2);

	int regval1 = *getR(REG1);
	int regval2 = *getR(REG2);

    if(regval1 - regval2 < 0)
    {
        SP[2] = SP[2] | NEGA;
        SP[2] = SP[2] & ~ZERO;
    }
    else if(regval1 - regval2 == 0)
    {
        SP[2] = SP[2] | ZERO;
        SP[2] = SP[2] & ~NEGA;
    }
    else
    {
        SP[2] = SP[2] & ~ZERO;
        SP[2] = SP[2] & ~NEGA;
    }
}

void display(char *IR, int *PC)
{

    //system("cl");
    int i;
    for (i = 0; i < 10; i+=2)
    {
        printf("GP%d  : %d\t\t GP%d  : %d\n",i, GP[i], i+1, GP[i+1]);
    }

    for (i = 10; i < 16; i+=2)
    {
        printf("GP%d : %d\t\t GP%d : %d\n",i, GP[i], i+1, GP[i+1]);
    }

    printf("\n");
    printf("PC   : %d\n", *PC);
    printf("IR   : %s\n", IR);
    printf("STP  : %d\n", STP);
    printf("MAR  : %d\n", MAR);
    printf("MDR  :%d\n", MDR);
    printf("FLAG : %d\n", SP[2]);

    if(SP[2] & NEGA)
    	printf("N : 1");
    else
    	printf("N : 0");

    if(SP[2] & ZERO)
    	printf("\tZ : 1\n");
    else
    	printf("\tZ : 0\n");
}


