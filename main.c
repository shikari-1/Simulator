#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "opcodefunc.h"
#include "opcodetype.h"

#define PAGE_SIZE 512
#define DYADIC 5
#define MONADIC 10
#define ZEROADD 12

int PC = 0;
char IR[17];

FILE *mptr;

int instructionFetch();
OpcodeType opcodeDecode();
int runOpcodeFunction(OpcodeType);


int main(int argc, char const *argv[])
{
    
    if (argc == 1){
        printf("Enter input file while executing.\n");
        exit(0);
    }

    if (argc > 1) {
        int miliseconds = 500;
        if(argc == 3)
            miliseconds = atoi(argv[2]);

        FILE *fptr;
        // object file
        fptr = fopen(argv[1],"r");

        // memory file
        mptr = fopen("memory.txt", "w+");
        fseek(fptr,0,SEEK_END);
        long prog_size = ftell(fptr);
        
        if(prog_size > PAGE_SIZE)
        {
            printf("\nError : Memory Overflow");
            return 0;
        }

        // clearing Memory
        char mem[PAGE_SIZE*3*8];
        int counter = 0;
        for(counter = 0; counter < 3*PAGE_SIZE*8 ; counter++)
            mem[counter]='0';
        fseek(mptr,0,SEEK_SET);
        fwrite(mem,1,counter,mptr);
        fseek(mptr,0,SEEK_SET);

        // saving program to memory
        fseek(fptr,0,SEEK_SET);
        char * program = malloc(prog_size);
        fscanf(fptr,"%s",program);
        fclose(fptr);
        fwrite(program,1,prog_size,mptr);
        fseek(mptr,0,SEEK_SET);

        // initialising PC with base address
        PC = 0;

        // Program Execution
        OpcodeType op;

        int i = 1;
        while(1)
        {
            
            display(IR,&PC);
            if(!instructionFetch())
            {
                printf("\nError : Program Stopped\n\n");
                break;
            }

            op = opcodeDecode();
            
            if(op == HLT0)
            {
                printf("\nProgram Finished\n\n");
                break;
            }

            if(!runOpcodeFunction(op))
            {
                printf("\nError : Program Stopped\n\n");
                break;                
            }

            usleep(miliseconds*1000);
        }

        free(program);
        fclose(mptr);
    }

    return 0;
}

int instructionFetch()
{
    fseek(mptr, PC*8, SEEK_SET);
    fread(IR, 1, 16, mptr);

    if (!strcmp(IR, "0000000000000000"))
        return 0;

    PC += 2;

    return 1;
}


OpcodeType opcodeDecode()
{
    char opcode[13];
    int counter = 0;
    int oplength = 12;

    for(counter = 0; counter < oplength; counter++)
    {
        if(counter < DYADIC && IR[counter] == '0')
            oplength = DYADIC;
        else if(counter < MONADIC && IR[counter] == '0')
            oplength = MONADIC;
        opcode[counter] = IR[counter];
    }

    opcode[counter]='\0';

    if(!strcmp(opcode,"00000"))
        return MOV0;
    else if(!strcmp(opcode,"00001"))
        return MOV1;
    else if(!strcmp(opcode,"00010"))
        return LDB0;
    else if(!strcmp(opcode,"00011"))
        return LDB1;
    else if(!strcmp(opcode,"00100"))
        return LDW0;
    else if(!strcmp(opcode,"00101"))
        return LDW1;
    else if(!strcmp(opcode,"00110"))
        return STB0;
    else if(!strcmp(opcode,"00111"))
        return STB1;
    else if(!strcmp(opcode,"01000"))
        return STW0;
    else if(!strcmp(opcode,"01001"))
        return STW1;
    else if(!strcmp(opcode,"01010"))
        return ADD0;
    else if(!strcmp(opcode,"01011"))
        return ADD1;
    else if(!strcmp(opcode,"01100"))
        return SUB0;
    else if(!strcmp(opcode,"01101"))
        return SUB1;
    else if(!strcmp(opcode,"01110"))
        return MUL0;
    else if(!strcmp(opcode,"01111"))
        return MUL1;
    else if(!strcmp(opcode,"10000"))
        return DIV0;
    else if(!strcmp(opcode,"10001"))
        return DIV1;
    else if(!strcmp(opcode,"10010"))
        return CMP0;
    else if(!strcmp(opcode,"10011"))
        return CMP1;
    else if(!strcmp(opcode,"1111100000"))
        return CAL0;
    else if(!strcmp(opcode,"1111100001"))
        return INC0;
    else if(!strcmp(opcode,"1111100010"))
        return DEC0;
    else if(!strcmp(opcode,"1111100011"))
        return JMP0;
    else if(!strcmp(opcode,"1111100100"))
        return JMP1;
    else if(!strcmp(opcode,"1111100101"))
        return JLT0;
    else if(!strcmp(opcode,"1111100110"))
        return JLT1;
    else if(!strcmp(opcode,"1111100111"))
        return JGT0;
    else if(!strcmp(opcode,"1111101000"))
        return JGT1;
    else if(!strcmp(opcode,"1111101001"))
        return JEQ0;
    else if(!strcmp(opcode,"1111101010"))
        return JEQ1;
    else if(!strcmp(opcode,"1111101011"))
        return JNE0;
    else if(!strcmp(opcode,"1111101100"))
        return JNE1;
    else if(!strcmp(opcode,"1111101101"))
        return PSH0;
    else if(!strcmp(opcode,"1111101110"))
        return PSH1;
    else if(!strcmp(opcode,"1111101111"))
        return POP0;
    else if(!strcmp(opcode,"1111110000"))
        return POP1;
    else if(!strcmp(opcode,"111111111100"))
        return RET0;
    else if(!strcmp(opcode,"111111111101"))
        return HLT0;
    else if(!strcmp(opcode,"111111111110"))
        return MSF0;
    else
        return INVALID;
}

int runOpcodeFunction(OpcodeType ot)
{
    switch(ot)
    {
        case MOV0:
            callMOV0(IR,mptr,&PC);
            break;

        case MOV1:
            callMOV1(IR,mptr,&PC);
            break;

        case LDB0:
            callLDB0(IR,mptr,&PC);
            break;

        case LDB1:
            callLDB1(IR,mptr,&PC);
            break;

        case LDW0:
            callLDW0(IR,mptr,&PC);
            break;

        case LDW1:
            callLDW1(IR,mptr,&PC);
            break;

        case STB0:
            callSTB0(IR,mptr,&PC);
            break;

        case STB1:
            callSTB1(IR,mptr,&PC);
            break;

        case STW0:
            callSTW0(IR,mptr,&PC);
            break;

        case STW1:
            callSTW1(IR,mptr,&PC);
            break; 

        case ADD0:
            callADD0(IR,mptr,&PC);
            break;

        case ADD1:
            callADD1(IR,mptr,&PC);
            break;

        case SUB0:
            callSUB0(IR,mptr,&PC);
            break;

        case SUB1:
            callSUB1(IR,mptr,&PC);
            break;

        case MUL0:
            callMUL0(IR,mptr,&PC);
            break;    

        case MUL1:
            callMUL1(IR,mptr,&PC);
            break;

        case DIV0:
            callDIV0(IR,mptr,&PC);
            break;

        case DIV1:
            callDIV1(IR,mptr,&PC);
            break;

        case CMP0:
            callCMP0(IR,mptr,&PC);
            break;

        case CMP1:
            callCMP1(IR,mptr,&PC);
            break;

        case CAL0:
            callCAL0(IR,mptr,&PC);
            break;

        case INC0:
            callINC0(IR,mptr,&PC);
            break;

        case DEC0:
            callDEC0(IR,mptr,&PC);
            break;

        case JMP0:
            callJMP0(IR,mptr,&PC);
            break;

        case JMP1:
            callJMP1(IR,mptr,&PC);
            break;

        case JLT0:
            callJLT0(IR,mptr,&PC);
            break;

        case JLT1:
            callJGT1(IR,mptr,&PC);
            break;

        case JGT0:
            callJGT0(IR,mptr,&PC);
            break;

        case JGT1:
            callJGT1(IR,mptr,&PC);
            break;

        case JEQ0:
            callJEQ0(IR,mptr,&PC);
            break;

        case JEQ1:
            callJEQ1(IR,mptr,&PC);
            break;

        case JNE0:
            callJNE0(IR,mptr,&PC);
            break;

        case JNE1:
            callJNE1(IR,mptr,&PC);
            break;

        case PSH0:
            callPSH0(IR,mptr,&PC);
            break;

        case PSH1:
            callPSH1(IR,mptr,&PC);
            break;

        case POP0:
            callPOP0(IR,mptr,&PC);
            break;

        case POP1:
            callPOP1(IR,mptr,&PC);
            break;

        case RET0:
            callRET0(IR,mptr,&PC);
            break;

        case MSF0:
            callMSF0(IR,mptr,&PC);
            break;

        default:
            return 0;
    }

    return 1;
}



