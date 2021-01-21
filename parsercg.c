// Created by Chris Jackson and Laurell Cuza on 6/17/20.
// Cop 3402C
// Section C001
// Homework #3: Parser and Code Generator

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_TABLE_SIZE 1000
#define MAX_CODE_LENGTH 1000
#define MAX_LEX_SIZE 1000
#define constant 1
#define variable 2
#define procedure 3

//List of defined symbol numbers for tokens
typedef enum {
    nulsym = 1,
    identsym = 2,
    numbersym = 3,
    plussym = 4,
    minussym = 5,
    multsym = 6,
    slashsym = 7,
    oddsym = 8,
    eqlsym = 9,
    neqsym = 10,
    lessym = 11,
    leqsym = 12,
    gtrsym = 13,
    geqsym = 14,
    lparentsym = 15,
    rparentsym = 16,
    commasym = 17,
    semicolonsym = 18,
    periodsym = 19,
    becomessym = 20,
    beginsym = 21,
    endsym = 22,
    ifsym = 23,
    thensym = 24,
    whilesym = 25,
    dosym = 26,
    callsym = 27,
    constsym = 28,
    varsym = 29,
    procsym = 30,
    writesym = 31,
    readsym = 32,
    elsesym = 33
} token_type;

//List of OP names with their appropriate numbers
typedef enum
{
    LIT = 1,
    OPR = 2,
    LOD = 3,
    STO = 4,
    CAL = 5,
    INC = 6,
    JMP = 7,
    JPC = 8,
    SIO1 = 9,
    SIO2 = 10,
    SIO3 = 11
}instructions;

//String array to help print out the tokens in their symbol name representation
char tokenSym[33][20] = {
    "nulsym",
    "identsym",
    "numbersym",
    "plussym",
    "minussym",
    "multsym",
    "slashsym",
    "oddsym",
    "eqlsym",
    "neqsym",
    "lessym",
    "leqsym",
    "gtrsym",
    "geqsym",
    "lparentsym",
    "rparentsym",
    "commasym",
    "semicolonsym",
    "periodsym",
    "becomessym",
    "beginsym",
    "endsym",
    "ifsym",
    "thensym",
    "whilesym",
    "dosym",
    "callsym",
    "constsym",
    "varsym",
    "procsym",
    "writesym",
    "readsym",
    "elsesym" };

//struct for the symbols in the symbol table
typedef struct
{
    int kind;    //1 = constant, 2 = variable, 3 = procedure
    char name[10];    //name of the symbol
    int val;    //value of the symbol
    int level;    //level symbol was declared in
    int addr;    //addresss of the symbol
    int mark;    //0 = accessible, 1 = inaccessible
} symbol;

//struct for the op, l and m of the intermediate code instructions
typedef struct
{
    int op;
    int l;
    int m;
} assembly;

//global counters of the token array, table array and code array
int tokenInc = 0, tableInc = 0, cx = 0;

//Function Declarations
void printTokenNames(char lexList[], FILE *output);
char* GetToken(char lexList[]);
void Enter(int type, char name[], int value, int level, int address, symbol symTable[]);
int symbolMark(int i, symbol symTable[]);
int symbolVal(int i, symbol symTable[]);
int symbolAddress(int i, symbol symTable[]);
int symbolLevel(int i, symbol symTable[]);
int symbolType(int i, symbol symTable[]);
int find(char ident[], symbol symTable[]);
int Factor(assembly code[], char lexList[], FILE *output, symbol symTable[], int sym, int currLevel, FILE *output2, FILE *outputFinal);
int Term(assembly code[], char lexList[], FILE *output, symbol symTable[], int sym, int currLevel, FILE *output2, FILE *outputFinal);
int Expression(assembly code[], char lexList[], FILE *output, symbol symTable[], int sym, int currLevel, FILE *output2, FILE *outputFinal);
int Condition(assembly code[], char lexList[], FILE *output, symbol symTable[], int sym, int currLevel, FILE *output2, FILE *outputFinal);
int Statement(assembly code[], char lexList[], FILE *output, symbol symTable[], int sym, int currLevel, FILE *output2, FILE *outputFinal);
int Block(assembly code[], char lexList[], FILE *output, symbol symTable[], int sym, int currLevel, FILE *output2, FILE *outputFinal);
void Program(assembly code[], char lexList[], FILE *output, symbol symTable[], FILE *output2, FILE *outputFinal);

FILE* parserCodeGen(FILE *AOutput, FILE *IntermediateCode, FILE *outputFinal) {
    symbol symTable[MAX_TABLE_SIZE];    //array to hold symbol table
	FILE *input;
	outputFinal = fopen("output.txt", "a");
    input = fopen("LOutput.txt", "r");    //file for input
    char lexList[MAX_LEX_SIZE], string[100], c = ' ';    //string for token list, temp string and char variable to hold the current char in token list
    assembly code[MAX_CODE_LENGTH];    //array for intermediate code

    while (1)    //loop will break when all tokens are found
    {
        fscanf(input, "%s", string);    //get next string in input file

        if (strcmp(string, "List:") == 0)    //once we've reached portion of input that contains the tokens
        {
            c = fgetc(input);    //get the next character so \n is not in our token list
            int i = 0;
            while (1)    //loop until all tokens are found
            {
                c = fgetc(input);    //get the next character
                if (c == EOF)    //break inner loop if we've reached the end of file
                    break;
                else
                {
                    lexList[i] = c;    //insert character int to string
                    i++;
                }
            }
            lexList[i] = '\0';    //now that all tokens are found, set the current index of token string to null terminator character
            break;    //break out of outer loop
        }
    }

    fclose(input);    //close input file
    fprintf(AOutput, "\n\n");
    FILE *symbolicfile = fopen("symbolictable.txt", "w");
    printTokenNames(lexList, symbolicfile);    //print the tokens in their symbol name representation
    fclose(symbolicfile);
    Program(code, lexList, IntermediateCode, symTable, AOutput, outputFinal);    //call program to start parsing recursive loop
    fprintf(AOutput, "\n\n");
    
    fprintf(AOutput, "LINE\tOP\tL\tM\n");

    char ops[10];
    //loop to print out the intermediate code with op name
    for (int i = 0; i <= cx; i++)
    {
        //determine string output based on opcode
        switch (code[i].op) {    //determine what the current code index op is
        case 1:
            strcpy(ops, "LIT");
            break;
        case 2:
            strcpy(ops, "OPR");
            break;
        case 3:
            strcpy(ops, "LOD");
            break;
        case 4:
            strcpy(ops, "STO");
            break;
        case 5:
            strcpy(ops, "CAL");
            break;
        case 6:
            strcpy(ops, "INC");
            break;
        case 7:
            strcpy(ops, "JMP");
            break;
        case 8:
            strcpy(ops, "JPC");
            break;
        case 9:
            strcpy(ops, "SIO");
            break;
        case 10:
            strcpy(ops, "SIO");
            break;
        case 11:
            strcpy(ops, "SIO");
            break;
        }
        fprintf(AOutput, "%d\t%s\t%d\t%d\n", i, ops, code[i].l, code[i].m); //print the current code index op name, l and m to assembly output
        if (code[i].op == 11)    //when current code index is SIO 0 3 and has been printed, break loop
            break;
    }

    //print intermediate code to intermediate code output file
    for (int i = 0; i <= cx; i++)
    {
        fprintf(IntermediateCode, "%d ", code[i].op);
        fprintf(IntermediateCode, "%d ", code[i].l);
        fprintf(IntermediateCode, "%d ", code[i].m);
        fprintf(IntermediateCode, "\n");
    }
	fclose(outputFinal);
	fclose(input);
    return IntermediateCode;
}

void Program(assembly code[], char lexList[], FILE *output, symbol symTable[], FILE *output2, FILE *outputFinal)
{
    char token[12];
    int sym, currLevel = 0;

    strcpy(token, GetToken(lexList));   //get next token
    sscanf(token, "%d", &sym);    //convert token string to integer
    sym = Block(code, lexList, output, symTable, sym, currLevel, output2, outputFinal);    //call function to parse block
    if (sym != periodsym)    //if current token is not a period symbol, print error and end program
    {
        fprintf(output2, "Error Number: 5, period expected");
		fprintf(outputFinal, "Error Number: 5, period expected");
        exit(0);
    }

    if (lexList[tokenInc] != '\0')    //if current token is not the end of the token list, print error and end program
    {
        fprintf(output2, "Error Number: 8, nothing can come after period");
		fprintf(outputFinal, "Error Number: 8, nothing can come after period");
        exit(14);
    }
    fprintf(output2, "No Errors, program syntactically correct");

    code[cx].op = SIO3;    //set current code index op to SIO3 value
    code[cx].l = 0;    //set current code index l to 0
    code[cx].m = 3;    //set current code index m to 3}
}
//function to parse block
int Block(assembly code[], char lexList[], FILE *output, symbol symTable[], int sym, int currLevel, FILE *output2, FILE *outputFinal)
    {
    char token[12], name[10];
    int num, varCount = 0, i, jumpAddr, n = 0;

    jumpAddr = cx;    //save current index
    code[cx].op = JMP;    //set current code index op to JMP value
    code[cx].l = 0;    //set current code index l to 0
    code[cx].m = 0;    //set current code index m to 0
    cx++;    //increment code index

    if (sym == constsym)    //if current token is a constant symbol
    {    //do at least once
        do {
            strcpy(token, GetToken(lexList));    //get next token
            sscanf(token, "%d", &sym);    //convert token string to integer

            if (sym != identsym)    //if current token is not an identifier, print error and end program
            {
                fprintf(output2, "Error: 3, const, var, procedure must be followed by an identifier");
				fprintf(outputFinal, "Error: 3, const, var, procedure must be followed by an identifier");
                exit(1);
            }

            strcpy(token, GetToken(lexList));    //get identifier name
            strcpy(name, token);

            i = find(name, symTable);    //find identifer in symbol table

            //if identifier was found, and it's mark is 0 and it's level is the same as the current level, print error and end program
            if (i != 0 && symbolMark(i, symTable) == 0 && symbolLevel(i, symTable) == currLevel)
            {
                fprintf(output2, "Error: 17, const %s already declared", name);
				fprintf(outputFinal, "Error: 17, const %s already declared", name);
                exit(15);
            }

            strcpy(token, GetToken(lexList));    //get next token
            sscanf(token, "%d", &sym);    //convert token string to integer

            if (sym != eqlsym)    //if current token is not an equals symbol, print error and end program
            {
                fprintf(output2, "Error: 2, identifier must be followed by a =");
				fprintf(outputFinal, "Error: 2, identifier must be followed by a =");
                exit(2);
            }

            strcpy(token, GetToken(lexList));    //get next token
            sscanf(token, "%d", &sym);    //convert token strint to integer

            if (sym != numbersym)    //if current token is not a number symbol, print error and end program
            {
                fprintf(output2, "Error: 1, = must be followed by a number");
				fprintf(outputFinal, "Error: 1, = must be followed by a number");
                exit(3);
            }

            strcpy(token, GetToken(lexList));    //get next token
            sscanf(token, "%d", &num);    //convert token string to integer
            Enter(constant, name, num, currLevel, 0, symTable);    //enter the identifier into the symbol table with correct parameters
            n++;
            strcpy(token, GetToken(lexList));    //get next token
            sscanf(token, "%d", &sym);    //convert token string to integer
        } while (sym == commasym);    //loop until current token is not a comma symbol

        if (sym != semicolonsym)    //if current token is not a semicolon symbol, print error and end program
        {
            fprintf(output2, "Error: 4, semicolon or comma missing");
			fprintf(outputFinal, "Error: 4, semicolon or comma missing");
            exit(4);
        }

        strcpy(token, GetToken(lexList));    //get next token
        sscanf(token, "%d", &sym);    //convert token string to integer
    }
    
    if (sym == varsym)    //if current token is a variable sybmol
    {
        do {    //do at least once
            strcpy(token, GetToken(lexList));    //get next token
            sscanf(token, "%d", &sym);    //convert token string to integer

            if (sym != identsym)    //if current token is not an identifier, print error and end program
            {
                fprintf(output2, "Error: 3, const, var, procedure must be followed by an identifier");
				fprintf(outputFinal, "Error: 3, const, var, procedure must be followed by an identifier");
                exit(5);
            }

            strcpy(token, GetToken(lexList));    //get identifier name
            strcpy(name, token);

            i = find(name, symTable);    //find identifier in symbol table
            //if identifier was found and the mark is 0 and the level is the same as current level, print error and end program
            if (i != 0 && symbolMark(i, symTable) == 0 && symbolLevel(i, symTable) == currLevel)
            {
                fprintf(output2, "Error: 18, var %s already declared", name);
				fprintf(outputFinal, "Error: 18, var %s already declared", name);
                exit(16);
            }
            Enter(variable, name, 0, currLevel, 4 + varCount, symTable);    //enter identifier into symbol table with correct parameters
            n++;
            varCount++;    //increment variable count
            strcpy(token, GetToken(lexList));    //get next token
            sscanf(token, "%d", &sym);    //convert token string to integer
        } while (sym == commasym);    //loop until current token is not a comma symbol

        if (sym != semicolonsym)    //if current token is not a semicolon symbol, print error and end program
        {
            fprintf(output2, "Error: 4, semicolon or comma missing");
			fprintf(outputFinal, "Error: 4, semicolon or comma missing");
            exit(6);
        }
        strcpy(token, GetToken(lexList));    //get next token
        sscanf(token, "%d", &sym);    //convert token string to integer
    }

    while (sym == procsym)    //loop until current token is not a procedure symbol
    {
        strcpy(token, GetToken(lexList));    //get next token
        sscanf(token, "%d", &sym);    //convert token string to integer

        if (sym != identsym)    //if current token is not an identifier, print error and end program
        {
            fprintf(output2, "Error: 3, const, var, procedure must be followed by an identifier");
			fprintf(outputFinal, "Error: 3, const, var, procedure must be followed by an identifier");
            exit(3);
        }

        strcpy(token, GetToken(lexList));    //get identifier name
        strcpy(name, token);

        i = find(name, symTable);    //find identifier in symbol table
        //if identifier was found and the mark is 0 and the level is the same as current level, print error and end program
        if (i != 0 && symbolMark(i, symTable) == 0 && symbolLevel(i, symTable) == currLevel)
        {
            fprintf(output2, "Error: 19, procedure %s already declared", name);
			fprintf(outputFinal, "Error: 19, procedure %s already declared", name);
            exit(19);
        }

        Enter(procedure, name, 0, currLevel, cx, symTable);    //enter identifier into symbol table with correct parameters
        n++;
        strcpy(token, GetToken(lexList));    //get next token
        sscanf(token, "%d", &sym);    //conver token string to integer

        if (sym != semicolonsym)    //if current token is not a semicolon symbol, print error and end program
        {
            fprintf(output2, "Error: 4, semicolon or comma missing");
			fprintf(outputFinal, "Error: 4, semicolon or comma missing");
            exit(4);
        }

        strcpy(token, GetToken(lexList));    //get next token
        sscanf(token, "%d", &sym);    //conver token string to integer
        sym = Block(code, lexList, output, symTable, sym, currLevel + 1, output2, outputFinal);    //call function to parse block and increase current level by 1

        if (sym != semicolonsym)    //if current token is not a semicolon symbol, print error and end program
        {
            fprintf(output2, "Error: 20, semicolon missing at end of block");
			fprintf(outputFinal, "Error: 20, semicolon missing at end of block");
            exit(20);
        }

        strcpy(token, GetToken(lexList));    //get next token
        sscanf(token, "%d", &sym);    //convert token string to integer
    }

    code[jumpAddr].m = cx;    //set saved code index m to the current index

    code[cx].op = INC;    //set current code index op to INC value
    code[cx].l = 0;    //set current code index l to 0
    code[cx].m = 4 + varCount;    //set current code index m to the amount of reserved space we need for VM plus our variable count
    cx++;    //increment code index

    sym = Statement(code, lexList, output, symTable, sym, currLevel, output2, outputFinal);    //call function to parse statement

    if (currLevel != 0)    //if the current level is not 0
    {
        code[cx].op = OPR;    //set current code index op to OPR value
        code[cx].l = 0;    //set current code index l to 0
        code[cx].m = 0;    //set current code index m to 0
        cx++;    //increment code index
    }

    //loop to mark identifiers declared in the current level to 1
    for (int j = n; j > 0; j--)
    {
        symTable[tableInc].mark = 1;    //mark identifier 1
        tableInc--;    //decrease our table pointer
    }

    return sym;
}

//function to parse statements
int Statement(assembly code[], char lexList[], FILE *output, symbol symTable[], int sym, int currLevel, FILE *output2, FILE *outputFinal)
{
    char token[12];
    int i, temp, jump;

    if (sym == identsym)    //if current token is an identifier
    {
        strcpy(token, GetToken(lexList));    //get the name of the identifier
        i = find(token, symTable);    //find identifier in symbol table

        if (i == 0)    //if identifier wasn't found, print error and end program
        {
            fprintf(output2, "Error: 6, undeclared identifier");
			fprintf(outputFinal, "Error: 6, undeclared identifier");
            exit(13);
        }

		if (symbolType(i, symTable) != variable)    //if identifier kind is not variable, print error and end program
		{
			fprintf(output2, "Error: 7, assignment to constant not allowed");
			fprintf(outputFinal, "Error: 7, assignment to constant not allowed");
			exit(14);
		}

        strcpy(token, GetToken(lexList));    //get next token
        sscanf(token, "%d", &sym);    //convert token string to integer

        if (sym != becomessym)    //if current token is not a become symbol, print error and end program
        {
            fprintf(output2, "Error: 10, := must come after identifier in statement part");
			fprintf(outputFinal, "Error: 10, := must come after identifier in statement part");
            exit(7);
        }

        strcpy(token, GetToken(lexList));    //get next token
        sscanf(token, "%d", &sym);    //convert token string to integer
        sym = Expression(code, lexList, output, symTable, sym, currLevel, output2, outputFinal);    //call function to parse expression

        code[cx].op = STO;    //set current code index op to STO value
        code[cx].l = currLevel - symbolLevel(i, symTable);    //subtract to find how many levels down to go and put result in l of current code index
        code[cx].m = symbolAddress(i, symTable);    //set current code index m to the address of the identifier
        cx++;    //increment code index
    }

    else if (sym == callsym)    //if current token is a call symbol
    {
        strcpy(token, GetToken(lexList));    //get next token
        sscanf(token, "%d", &sym);    //convert token string to integer

        if (sym != identsym)    //if the current token is not an identifier, print error and end program
        {
            fprintf(output2, "Error: 21, call must be followed by an identifier");
			fprintf(outputFinal, "Error: 21, call must be followed by an identifier");
            exit(21);
        }

        strcpy(token, GetToken(lexList));    //get identifier name
        i = find(token, symTable);    //find identifier in symbol table

        if (i == 0)    //if identifier wasn't found, print error and end program
        {
            fprintf(output2, "Error: 6, undeclared identifier");
			fprintf(outputFinal, "Error: 6, undeclared identifier");
            exit(13);
        }

        if (symbolType(i, symTable) == procedure)    //if the identifier kind is procedure
        {
            code[cx].op = CAL;    //set current code index op to CAL value
            code[cx].l = currLevel - symbolLevel(i, symTable);    //subtract to find how many levels down to go and set result to the current code index l
            code[cx].m = symbolAddress(i, symTable);    //set current code index m to identifier address
            cx++;    //increment code index
        }

        //if identifier kind was not a procedure, print error and end program
        else
        {
            fprintf(output2, "Error: 21, call must be followed by an identifier");
			fprintf(outputFinal, "Error: 21, call must be followed by an identifier");
            exit(21);
        }

        strcpy(token, GetToken(lexList));    //get next token
        sscanf(token, "%d", &sym);    //convert token string to integer
    }

    else if (sym == beginsym)    //if current token is a begin symbol
    {
        strcpy(token, GetToken(lexList));    //get next token
        sscanf(token, "%d", &sym);    //convert token string to integer
        sym = Statement(code, lexList, output, symTable, sym, currLevel, output2, outputFinal);    //call function to parse statement

        while (sym == semicolonsym)    //while the current token is a semicolon symbol
        {
            strcpy(token, GetToken(lexList));    //get next token
            sscanf(token, "%d", &sym);    //convert token string to integer
            sym = Statement(code, lexList, output, symTable, sym, currLevel, output2, outputFinal);    //call function to parse statement
        }

        if (sym != endsym)    //if current token is not an end symbol
        {
            fprintf(output2, "Error: 10, begin must be closed with end");
			fprintf(outputFinal, "Error: 10, begin must be closed with end");
            exit(8);
        }
        strcpy(token, GetToken(lexList));    //get next token
        sscanf(token, "%d", &sym);    //convert token string to integer
    }

    else if (sym == ifsym)    //if current token is an if symbol
    {
        strcpy(token, GetToken(lexList));    //get next token
        sscanf(token, "%d", &sym);    //convert token string to integer
        sym = Condition(code, lexList, output, symTable, sym, currLevel, output2, outputFinal);    //call function to parse condition
        temp = cx;    //save current code index in temp variable
        code[cx].op = JPC;    //set current code index op to JPC value
        code[cx].l = 0;    //set current code index l to 0
        code[cx].m = 0;    //set current code index m to 0
        cx++;    //increment code index

        if (sym != thensym)    //if current token is not then sym, print error and end program
        {
            fprintf(output2, "Error: 9, if condition must be followed by then");
			fprintf(outputFinal, "Error: 9, if condition must be followed by then");
            exit(9);
        }

        strcpy(token, GetToken(lexList));    //get next token
        sscanf(token, "%d", &sym);    //convert token string to integer
        sym = Statement(code, lexList, output, symTable, sym, currLevel, output2, outputFinal);    //call function to parse statement

        if (sym == elsesym)    //if current token is an else symbol
        {
            code[temp].m = cx;    //set code at saved index m to the current index
            strcpy(token, GetToken(lexList));    //get next token
            sscanf(token, "%d", &sym);    //convert token string to integer
            temp = cx;    //save current code index
            code[cx].op = JMP;    //set current code index op to JMP value
            code[cx].l = 0;    //set current code index l to 0
            code[cx].m = 0;    //set current code index m to 0
            cx++;
            sym = Statement(code, lexList, output, symTable, sym, currLevel, output2, outputFinal);    //call function to parse statement
            code[temp].m = cx;    //set code at saved index m to current index
        }

        //if current token isn't an else symbol
        else
        {
            code[temp].m = cx;    //set code at saved index m to current index
        }
    }

    else if (sym == whilesym)    //if current token is a while symbol
    {
        temp = cx;    //save current code index
        strcpy(token, GetToken(lexList));    //get next token
        sscanf(token, "%d", &sym);    //convert token string to integer
        sym = Condition(code, lexList, output, symTable, sym, currLevel, output2, outputFinal);    //call function to parse condition

        if (sym != dosym)    //if current token is not a do symbol, print an error and end program
        {
            fprintf(output2, "Error: 11, while must be followed by do");
			fprintf(outputFinal, "Error: 11, while must be followed by do");
            exit(10);
        }

        jump = cx;    //save the current code index
        code[cx].op = JPC;    //set current code index op to JPC value
        code[cx].l = 0;    //set current code index l to 0
        code[cx].m = 0;    //set current code index m to 0
        cx++;    //increment code index
        strcpy(token, GetToken(lexList));    //get next token
        sscanf(token, "%d", &sym);    //convert token string to integer
        sym = Statement(code, lexList, output, symTable, sym, currLevel, output2, outputFinal);    //call function to parse statement
        code[cx].op = JMP;    //set current code index op to JMP value
        code[cx].l = 0;    //set current code index l to 0
        code[cx].m = temp;    //set current code index m to the index saved in temp
        cx++;    //increment code index
        code[jump].m = cx;    //set code m at index saved in jump to current index
    }

    else if (sym == readsym)    //if the current token is a read symbol
    {
        strcpy(token, GetToken(lexList));    //get next token
        sscanf(token, "%d", &sym);    //convert token string to integer

        if (sym != identsym)    //if current token is not an identifier, print error and end program
        {
            fprintf(output2, "Error: 12, read must be followed by an identifier");
			fprintf(outputFinal, "Error: 12, read must be followed by an identifier");
            exit(11);
        }

        strcpy(token, GetToken(lexList));    //get identifier name
        i = find(token, symTable);    //find identifier in symbol table

        if (i == 0)    //if identifier was not found in symbol table, print error and end program
        {
            fprintf(output2, "Error: 6, undeclared identifier");
			fprintf(outputFinal, "Error: 6, undeclared identifier");
            exit(17);
        }

        if (symbolType(i, symTable) == constant)    //if identifier kind is constant, print error and end program
        {
            fprintf(output2, "Error: 12, read must be followed by identifier");
			fprintf(outputFinal, "Error: 12, read must be followed by identifier");
            exit(18);
        }
        code[cx].op = SIO2;    //set current code index op to SIO2 value
        code[cx].l = 0;    //set current code index l to 0
        code[cx].m = 2;    //set current code index m to 2
        cx++;    //increment code index
        code[cx].op = STO;    //set current code index op to STO value
        code[cx].l = currLevel - symbolLevel(i, symTable);    //subtract to find how many levels down to go and set result in current code index l
        code[cx].m = symbolAddress(i, symTable);    //set current code index m to identifier address
        cx++;    //increment code index
        strcpy(token, GetToken(lexList));    //get next token
        sscanf(token, "%d", &sym);    //convert token string to integer
    }

    else if (sym == writesym)    //if current token is a write symbol
    {
        strcpy(token, GetToken(lexList));    //get next token
        sscanf(token, "%d", &sym);    //convert token string to integer

		if (sym != identsym)
		{
			fprintf(output2, "Error: 13, write must be followed by an identifier");
			fprintf(outputFinal, "Error: 13, write must be followed by an identifier");
			exit(13);
		}

        sym = Expression(code, lexList, output, symTable, sym, currLevel, output2, outputFinal);

        code[cx].op = SIO1;    //set current code index op to SIO1 value
        code[cx].l = 0;    //set current code index l to 0
        code[cx].m = 1;    //set current code index m to 1
        cx++;    //increment code index
    }


    return sym;
}

//function for parsing conditions
int Condition(assembly code[], char lexList[], FILE *output, symbol symTable[], int sym, int currLevel, FILE *output2, FILE *outputFinal)
{
    char token[12];

    if (sym == oddsym)    //if current token is an odd symbol
    {
        strcpy(token, GetToken(lexList));    //get next token
        sscanf(token, "%d", &sym);    //convert token string to integer
        sym = Expression(code, lexList, output, symTable, sym, currLevel, output2, outputFinal);    //call function to parse expressions
        code[cx].op = OPR;    //set current code index op to OPR value
        code[cx].l = 0;    //set current code index l to 0
        code[cx].m = 6;    //set current code index m to 6
        cx++;    //increment code index
    }

    //if current token is not an odd symbol, get expression and check which relation operator is used
    else
    {
        sym = Expression(code, lexList, output, symTable, sym, currLevel, output2, outputFinal);    //call function to parse expressions

        if (sym == eqlsym)    //if current token is an equal symbol
        {
            strcpy(token, GetToken(lexList));    //get next token
            sscanf(token, "%d", &sym);    //convert token string to integer
            sym = Expression(code, lexList, output, symTable, sym, currLevel, output2, outputFinal);    //call function to parse expressions
            code[cx].op = OPR;    //set current code index op to OPR value
            code[cx].l = 0;    //set current code index l to 0
            code[cx].m = 8;    //set current code index m to 8
            cx++;    //increment code index
        }

        else if (sym == neqsym)    //if currrent token is a not equal symbol
        {
            strcpy(token, GetToken(lexList));    //get next token
            sscanf(token, "%d", &sym);    //convert token string to integer
            sym = Expression(code, lexList, output, symTable, sym, currLevel, output2, outputFinal);    //call function to parse expressions
            code[cx].op = OPR;    //set current code index op to OPR value
            code[cx].l = 0;    //set current code index l to 0
            code[cx].m = 9;    //set current code index m to 9
            cx++;    //increment code index
        }

        else if (sym == lessym)    //if current token is less than symbol
        {
            strcpy(token, GetToken(lexList));    //get next token
            sscanf(token, "%d", &sym);    //convert token string to integer
            sym = Expression(code, lexList, output, symTable, sym, currLevel, output2, outputFinal);    //call function to parse expressions
            code[cx].op = OPR;    //set current code index op to OPR value
            code[cx].l = 0;    //set current code index l to 0
            code[cx].m = 10;    //set current code index m to 10
            cx++;    //increment code index
        }

        else if (sym == leqsym)    //if current code index is less than or equal to symbol
        {
            strcpy(token, GetToken(lexList));    //get next token
            sscanf(token, "%d", &sym);    //convert token string to integer
            sym = Expression(code, lexList, output, symTable, sym, currLevel, output2, outputFinal);    //call function to parse expressions
            code[cx].op = OPR;    //set current code index op to OPR value
            code[cx].l = 0;    //set current code index l to 0
            code[cx].m = 11;    //set current code index m to 11
            cx++;    //increment code index
        }

        else if (sym == gtrsym)    //if current token is greater than symbol
        {
            strcpy(token, GetToken(lexList));    //get next token
            sscanf(token, "%d", &sym);    //convert token string to integer
            sym = Expression(code, lexList, output, symTable, sym, currLevel, output2, outputFinal);    //call function to parse expressions
            code[cx].op = OPR;    //set currend code index op to OPR value
            code[cx].l = 0;    //set current code index l to 0
            code[cx].m = 12;    //set current code index m to 12
            cx++;    //increment code index
        }

        else if (sym == geqsym)    //if current token is greater than or equal to symbol
        {
            strcpy(token, GetToken(lexList));    //get next token
            sscanf(token, "%d", &sym);    //convert token string to integer
            sym = Expression(code, lexList, output, symTable, sym, currLevel, output2, outputFinal);    //call function to parse expression
            code[cx].op = OPR;    //set current code index op to OPR value
            code[cx].l = 0; ;    //set current code index l to 0
            code[cx].m = 13;    //set current code index m to 13
            cx++;    //increment code index
        }

        //if current token is none of these symbols, no relational operator was found, so printf error and end program
        else
        {
            fprintf(output2, "Error: 14, relational operation expected");
			fprintf(outputFinal, "Error: 14, relational operation expected");
            exit(12);
        }
    }

    return sym;
}

//function to parse expressions
int Expression(assembly code[], char lexList[], FILE *output, symbol symTable[], int sym, int currLevel, FILE *output2, FILE *outputFinal)
{
    char token[12];

    if (sym == plussym)    //if the current token is a addition symbol
    {
        strcpy(token, GetToken(lexList));    //get next token
        sscanf(token, "%d", &sym);    //convert token string to an integer
    }

    if (sym == minussym)    //if the current token is a subtraction symbol
    {
        strcpy(token, GetToken(lexList));    //get next token
        sscanf(token, "%d", &sym);    //convert token string to an integer
        sym = Term(code, lexList, output, symTable, sym, currLevel, output2, outputFinal);    //call function to parse terms
        code[cx].op = OPR;    //set current code index op to OPR value
        code[cx].l = 0;    //set current code index l to 0
        code[cx].m = 1;    //set current code index m to 1
        cx++;    //increment code index
    }

    sym = Term(code, lexList, output, symTable, sym, currLevel, output2, outputFinal);    //call function to parse terms

    while (sym == plussym || sym == minussym)    //loop until current token is not a addition or subtraction symbol
    {
        if (sym == plussym)    //if current token is an addition symbol
        {
            strcpy(token, GetToken(lexList));    //get next token
            sscanf(token, "%d", &sym);    //convert token string to integer
            sym = Term(code, lexList, output, symTable, sym, currLevel, output2, outputFinal);    //call function to parse terms
            code[cx].op = OPR;    //set current code index op to OPR value
            code[cx].l = 0;    //set current code index l to 0
            code[cx].m = 2;    //set current code index m to 2
            cx++;    //increment code index
        }

        if (sym == minussym)    //if current token is a subtraction symbol
        {
            strcpy(token, GetToken(lexList));    //get next token
            sscanf(token, "%d", &sym);    //convert token string to integer
            sym = Term(code, lexList, output, symTable, sym, currLevel, output2, outputFinal);    //call function to parse term
            code[cx].op = OPR;    //set current code index op to OPR value
            code[cx].l = 0;    //set current code index l to 0
            code[cx].m = 3;    //set current code index m to 3
            cx++;    //increment code index
        }
    }

    return sym;
}

//function to parse terms
int Term(assembly code[], char lexList[], FILE *output, symbol symTable[], int sym, int currLevel, FILE *output2, FILE *outputFinal)
{
    char token[12];

    sym = Factor(code, lexList, output, symTable, sym, currLevel, output2, outputFinal);    //call function to parse a factor

    while (sym == multsym || sym == slashsym)    //if the current token is a multiplication symbol or division symbol, loop until there are no more.
    {
        if (sym == multsym)    //if the current token is a multiplication symbol
        {
            strcpy(token, GetToken(lexList));    //get next token
            sscanf(token, "%d", &sym);    //convert token string to an integer
            sym = Factor(code, lexList, output, symTable, sym, currLevel, output2, outputFinal);    //call function to parse a factor
            code[cx].op = OPR;    //set current code index op to OPR value
            code[cx].l = 0;    //set current code index l to 0
            code[cx].m = 4;    //set current code index m to 4
            cx++;    //increment code index
        }

        if (sym == slashsym)    //if the current token is a division symbol
        {
            strcpy(token, GetToken(lexList));    //get the next token
            sscanf(token, "%d", &sym);    //convert token string to an integer
            sym = Factor(code, lexList, output, symTable, sym, currLevel, output2, outputFinal);    //call function to parse a factor
            code[cx].op = OPR;    //set current code index op to OPR value
            code[cx].l = 0;    //set current code index l to 0
            code[cx].m = 5;    //set current code index m to 5
            cx++;    //increment code index
        }
    }

    return sym;
}

//function for the parsing factors
int Factor(assembly code[], char lexList[], FILE *output, symbol symTable[], int sym, int currLevel, FILE *output2, FILE *outputFinal)
{
    char token[12];
    int i;

    if (sym == identsym)    //parsing factor that is an identifier
    {
        strcpy(token, GetToken(lexList));    //copy the identifier name
        i = find(token, symTable);    //find the identifier in the symbol table

        if (i == 0)    //if the identifier wasn't found, print error and exit program
        {
            fprintf(output2, "Error: 6, undeclared identifier");
			fprintf(outputFinal, "Error: 6, undeclared identifier");
            exit(13);
        }

        if (symbolType(i, symTable) == variable)    //if the kind of the identifier is a variable
        {
            code[cx].op = LOD;    //set the current code index op to the LOD value
            code[cx].l = currLevel - symbolLevel(i, symTable);    //set the current code index l. Subtraction determines how many levels down VM should go
            code[cx].m = symbolAddress(i, symTable);    //set the current code index m to the address of the identifier
            cx++;    //increment code index
        }

        else if (symbolType(i, symTable) == constant)    //if the kind of the identifier is a constant
        {
            code[cx].op = LIT;    //set the current code index op to LIT value
            code[cx].l = 0;    //set the current code index l to 0
            code[cx].m = symbolVal(i, symTable);    //set the currend code index m to the value of the identifier
            cx++;    //increment code index
        }
        strcpy(token, GetToken(lexList));    //get next token
        sscanf(token, "%d", &sym);    //convert token string to an integer
    }

    else if (sym == numbersym)    //if the token is a number
    {
        strcpy(token, GetToken(lexList));    //get next token
        sscanf(token, "%d", &sym);    //convert token string to an integer
        code[cx].op = LIT;    //set current code index op to the LIT value
        code[cx].l = 0;    //set current code index l to 0
        code[cx].m = sym;    //set current code index m to the number we got from last GetToken() call
        cx++;    //increment code index
        strcpy(token, GetToken(lexList));    //get next token
        sscanf(token, "%d", &sym);    //conert token string to an integer
    }

    else if (sym == lparentsym)    //if the token is a left parenthesis
    {
        strcpy(token, GetToken(lexList));    //get next token
        sscanf(token, "%d", &sym);    //convert token string to an integer
        sym = Expression(code, lexList, output, symTable, sym, currLevel, output2, outputFinal);    //Call the expression function
        if (sym != rparentsym)    //if the current token isn't a right parenthesis, print error and end program
        {
            fprintf(output2, "Error: 15, right parenthesis missing");
			fprintf(outputFinal, "Error: 15, right parenthesis missing");
            exit(21);
        }
        strcpy(token, GetToken(lexList));    //get next token
        sscanf(token, "%d", &sym);    //convert token string to an integer
    }

    //if the token was none of the previous possible tokens, syntax is wrong. Print error and exit program.
    else
    {
        fprintf(output2, "Error: 16, the preceding factor can not begin with this symbol");
		fprintf(outputFinal, "Error: 16, the preceding factor can not begin with this symbol");
        exit(22);
    }

    return sym;
}

//function to find if the identifier is in the symbol table
int find(char ident[], symbol symTable[])
{
    int i;

    //loop through each of the indexes until you find the identifier.
    for (i = tableInc; i > 0; i--)
    {
        if (strcmp(symTable[i].name, ident) == 0)
            break;    //stop looping if identifier is found
    }

    return i;    //return the index the identifier was found at. If the identifier wasn't found, 0 will be returned
}

//funtion to print the tokens in their symbol name representation
void printTokenNames(char lexList[], FILE *output)
{
    char temp[12];
    int j = 0;
    int num;

    //loop through all the tokens
    for (int i = 0; lexList[i] != '\0'; i++)
    {
        if (lexList[i] == ' ') //once you reach the end of the token
        {
            temp[j] = '\0'; //insert the null terminator into the end of the current token string
            j = 0;
            sscanf(temp, "%d", &num);    //converts the token from a string to an integer
            fprintf(output, "%s ", tokenSym[num - 1]);    //print the symbol name from the correct index in the global string array

            if (num == 2 || num == 3)    //If the token is an identifier or a number, print the identifier name or literal number
            {
                i++;
                while (lexList[i] != ' ') //once you reach the end of the name or number, save it in the temp string
                {
                    temp[j] = lexList[i];
                    j++;
                    i++;
                }

                temp[j] = '\0';    //insert the null terminator into the end of the current token string
                j = 0;
                fprintf(output, "%s ", temp);    //print the name or number
            }
        }

        //if we haven't reached a space, keeping looping the array to get the full token
        else
        {
            temp[j] = lexList[i];
            j++;
        }
    }
}

//function to get the next token for the parser
char* GetToken(char lexList[])
{
    static char token[12];
    int i = 0;

    //keep looping through token array till you get a space which means you've gotten the full token
    while (lexList[tokenInc] != ' ' && lexList[tokenInc] != '\0')
    {
        token[i] = lexList[tokenInc];    //add token character to token string
        tokenInc++;
        i++;
    }
    token[i] = '\0';    //instert null terminator to end of our token string
    //increase the pointer for the token list to skip the space and start at next token when we call GetToken() again.
    if (lexList[tokenInc] != '\0')    //Don't increase the token list pointer if we've reached the null terminator so we know there are no more tokens left
        tokenInc++;
    return token;
}

//function to enter an identifier into the symbol table
void Enter(int type, char name[], int value, int level, int address, symbol symTable[])
{
    tableInc++;
    symTable[tableInc].kind = type;    //set the current identifiers kind
    strcpy(symTable[tableInc].name, name);    //copy the name for the identifier
    symTable[tableInc].val = value;    //set the value of the identifier
    symTable[tableInc].level = level;    //set the level of the identifier
    symTable[tableInc].addr = address;    //set the address of the identifier
    symTable[tableInc].mark = 0;    //mark the identifier as 0 so we know it can be accessed
}

//return the mark of identifier at symbol table index i
int symbolMark(int i, symbol symTable[])
{
    return symTable[i].mark;
}

//return the value of identifier at symbol table index i
int symbolVal(int i, symbol symTable[])
{
    return symTable[i].val;
}

//return the address of identifier at symbol table index i
int symbolAddress(int i, symbol symTable[])
{
    return symTable[i].addr;
}

//return the level of identifier at symbol table index i
int symbolLevel(int i, symbol symTable[])
{
    return symTable[i].level;
}

//return the kind of identifier at symbol table index i
int symbolType(int i, symbol symTable[])
{
    return symTable[i].kind;
}

