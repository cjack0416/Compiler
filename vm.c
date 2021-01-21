// Chris Jackson
// Laurell Cuza
// COP 3402 Section 1
// Summer 2020
// Homework #1

#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX_DATA_STACK_HEIGHT 1000
#define MAX_CODE_LENGTH 500
int stack[MAX_DATA_STACK_HEIGHT];
typedef struct{
    int op; //opcode
    int l;  //L
    int m;  //M
} instruction;

//Finds base L levels down
int base(int l, int base){
    int b1;
    b1 = base;
    while(l>0){
        b1 = stack[b1-1];
        l--;
    }
    return b1;
}
FILE* virtualMachine(FILE *input){
    //define variables and initial values
    int sp = MAX_DATA_STACK_HEIGHT;
    int bp = sp - 1;
    int pc = 0;
    int halt = 1;
    int stackMade = 0;
    int numStacks[MAX_DATA_STACK_HEIGHT] = { 0 };
    int stackSize = 0;
    instruction IR;
    instruction code[MAX_CODE_LENGTH];
    //open input and output files
    FILE *output = fopen("VOutput.txt", "w");
    int i = 0;
    char ops[10];
    int counter = 0;
    //scanning input text and printing out initial instruction list
    while (counter <= MAX_CODE_LENGTH)
    {
        fscanf(input, "%d", &code[i].op);
        fscanf(input, "%d", &code[i].l);
        fscanf(input, "%d", &code[i].m);

        if (code[i].op == 11)
            break;
        i++;
    }
    //Starts second output list
    fprintf(output, "\n\n\n\n\t\t\tpc\tbp\tsp\tstack\nInitial values\t\t");
    int AR = 0, write = 0; //AR flag
    //fetch and execution in while loop
    while (halt) {
        

        fprintf(output, "%d\t%d\t%d", pc, bp, sp); //pointer values for each instruction
        if (stackMade) //condition if stack exists
        {
            fprintf(output, "\t");
            int j = MAX_DATA_STACK_HEIGHT - 1;
            int k = 0;
            counter = 0;
            //print stack
            for (int i = 1; i <= stackSize; i++)
            {
                //print new activation record marker
                if (i == counter + numStacks[k] + 1 && AR > 0)
                {
                    fprintf(output, "| ");
                    counter += numStacks[k];
                    k++;
                }
                fprintf(output, "%d ", stack[j]);
                j--;
            }

			if (write)
			{
				fprintf(output, "\n%d", write);
				printf("\n%d\n", write);
				write = 0;
			}
        }

        IR = code[pc]; //fetch
        fprintf(output, "\n%d ", pc); //prints line number
        pc = pc + 1; //increase pc
        //instruction execution
        switch (IR.op) {
            //puts literal M on top of stack
        case 1:
            strcpy(ops, "lit");
            sp = sp - 1;
            stack[sp] = IR.m;
            numStacks[AR]++; //add to the size of the current activation record stack
            stackSize++; //add to total size of stacks combined
            break;

            //opr execution
        case 2:
            strcpy(ops, "opr");
            switch (IR.m) {

                //return
            case 0:
                sp = bp + 1;
                pc = stack[sp - 4];
                bp = stack[sp - 3];
                stackSize = MAX_DATA_STACK_HEIGHT - sp; //reduce total stack size
                AR--; //subtract total activtion records
                break;
                
                //negative
            case 1:
                stack[sp] = -1 * stack[sp];
                break;

                //add
            case 2:
                sp = sp + 1;
                stack[sp] = stack[sp] + stack[sp - 1];
                numStacks[AR]--;
                stackSize--;
                break;

                //subtract
            case 3:
                sp = sp + 1;
                stack[sp] = stack[sp] - stack[sp - 1];
                numStacks[AR]--;
                stackSize--;
                break;

                //multiplication
            case 4:
                sp = sp + 1;
                stack[sp] = stack[sp] * stack[sp - 1];
                numStacks[AR]--;
                stackSize--;
                break;

                //division
            case 5:
                sp = sp + 1;
                stack[sp] = stack[sp] / stack[sp - 1];
                numStacks[AR]--;
                stackSize--;
                break;

                //odd
            case 6:
                stack[sp] = stack[sp] % 2;
                break;

                //modular
            case 7:
                sp = sp + 1;
                stack[sp] = stack[sp] % stack[sp - 1];
                numStacks[AR]--;
                stackSize--;
                break;

                //equal to
            case 8:
                sp = sp + 1;
                stack[sp] = (stack[sp] == stack[sp - 1]);
                numStacks[AR]--;
                stackSize--;
                break;

                //not equal to
            case 9:
                sp = sp + 1;
                stack[sp] = (stack[sp] != stack[sp - 1]);
                numStacks[AR]--;
                stackSize--;
                break;

                //less than
            case 10:
                sp = sp + 1;
                stack[sp] = (stack[sp] < stack[sp - 1]);
                numStacks[AR]--;
                stackSize--;
                break;

                //less than or equal to
            case 11:
                sp = sp + 1;
                stack[sp] = (stack[sp] <= stack[sp - 1]);
                numStacks[AR]--;
                stackSize--;
                break;

                //greater than
            case 12:
                sp = sp + 1;
                stack[sp] = (stack[sp] > stack[sp - 1]);
                numStacks[AR]--;
                stackSize--;
                break;

                //greater than or equal to
            case 13:
                sp = sp + 1;
                stack[sp] = (stack[sp] >= stack[sp - 1]);
                numStacks[AR]--;
                stackSize--;
                break;
            }
            break;

            //load of value of top of stack from the stack location at offset M
        case 3:
            strcpy(ops, "lod");
            sp = sp - 1;
            stack[sp] = stack[base(IR.l, bp) - IR.m];
            numStacks[AR]++;
            stackSize++;
            break;

            //store value at top of stack in location at offset M
        case 4:
            strcpy(ops, "sto");
            stack[base(IR.l, bp) - IR.m] = stack[sp];
            sp = sp + 1;
            numStacks[AR]--; //subtract the size of the stack of the current activation record
            stackSize--; // subtract total combined stack size
            break;

            //call procedure at index M
        case 5:
            strcpy(ops, "cal");
            stack[sp - 1] = 0;
            stack[sp - 2] = base(IR.l, bp);
            stack[sp - 3] = bp;
            stack[sp - 4] = pc;
            bp = sp - 1;
            pc = IR.m;
            AR++; //increase number of activation records
            break;

            //alocate M locals
        case 6:
            strcpy(ops, "inc");
            sp = sp - IR.m;
            stackMade = 1; //set flag that a stack was made;
            numStacks[AR] += IR.m; //increase stack of the current activation record by M
            stackSize += IR.m; //increase combined stack size by M
            break;

            //jump to M
        case 7:
            strcpy(ops, "jmp");
            pc = IR.m;
            break;

            //jump to instruction m if top stack element is 0
        case 8:
            strcpy(ops, "jpc");
            if (stack[sp] == 0)
                pc = IR.m;
            sp = sp + 1;
            numStacks[AR]--;
            stackSize--;
            break;

            //write top stack element to the screen
        case 9:
            strcpy(ops, "sio");
			write = stack[sp];
            sp = sp + 1;
            numStacks[AR]--;
            stackSize--;
            break;

            //read input from the user and store it on top of stack
        case 10:
            strcpy(ops, "sio");
            sp = sp - 1;
            numStacks[AR]++;
            stackSize++;
            scanf("%d", &stack[sp]);
            break;

            //end program
        case 11:
            strcpy(ops, "sio");
            halt = 0;
            break;
        }

        fprintf(output, "%s %d %d\t\t", ops, IR.l, IR.m); //prints instruction executed

        if (halt == 0)
        {
            fprintf(output, "%d\t%d\t%d\n", pc, bp, sp); //prints pc bp and sp when there is no stack
        }
        
    }

    fclose(output);
    return output;
}
