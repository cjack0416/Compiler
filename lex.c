// Created by Chris Jackson and Laurell Cuza on 6/17/20.
// Cop 3402C
// Section C001
// Homework #2: Lex Analyzer

// Importing libraries
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX_ARRAY_LENGTH 5000 // Constant array length defined
//enumeration
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
// Token structure
typedef struct tokens{
    int type;
    char lexeme[20];
} token;
// Creates tokens for lexeme starting with a letter
token letterTokenizer(char letter[]){
    token temp;
    // Checks all reserved words
    if(strcmp(letter, "const") == 0){
        temp.type = constsym;
        strcpy(temp.lexeme, "const");
        return temp;
    }
    else if(strcmp(letter, "var") == 0){
        temp.type = varsym;
        strcpy(temp.lexeme, "var");
        return temp;
    }
    else if (strcmp(letter, "procedure") == 0) {
        temp.type = procsym;
        strcpy(temp.lexeme, "procedure");
        return temp;
    }
    else if(strcmp(letter, "if") == 0){
        temp.type = ifsym;
        strcpy(temp.lexeme, "if");
        return temp;
    }
    else if(strcmp(letter, "then") == 0){
        temp.type = thensym;
        strcpy(temp.lexeme, "then");
        return temp;
    }
    else if (strcmp(letter, "begin") == 0){
        temp.type = beginsym;
        strcpy(temp.lexeme, "begin");
        return temp;
    }
    else if (strcmp(letter, "end") == 0) {
        temp.type = endsym;
        strcpy(temp.lexeme, "end");
        return temp;
    }
    else if(strcmp(letter, "while") == 0){
        temp.type = whilesym;
        strcpy(temp.lexeme, "while");
        return temp;
    }
    else if(strcmp(letter, "do") == 0){
        temp.type = dosym;
        strcpy(temp.lexeme, "do");
        return temp;
    }
    else if (strcmp(letter, "call") == 0) {
        temp.type = callsym;
        strcpy(temp.lexeme, "call");
        return temp;
    }
    else if(strcmp(letter, "read") == 0){
        temp.type = readsym;
        strcpy(temp.lexeme, "read");
        return temp;
    }
    else if(strcmp(letter, "write") == 0){
        temp.type = writesym;
        strcpy(temp.lexeme, "write");
        return temp;
    }
    else if (strcmp(letter, "else") == 0) {
        temp.type = elsesym;
        strcpy(temp.lexeme, "else");
        return temp;
    }
    else if(strcmp(letter, "odd") == 0){
        temp.type = oddsym;
        strcpy(temp.lexeme, "odd");
        return temp;
    }


    // If string does not match reserved words, then it is tokenized as an identifier
    else{
        temp.type = identsym;
        strcpy(temp.lexeme, letter);
        return temp;
    }
}
// Creates tokens for lexeme starting with a number
token numberTokenizer(char number[]){
    token temp;
    temp.type = numbersym;
    strcpy(temp.lexeme, number);
    return temp;
}
// Creates tokens for lexeme starting with a symbol
token symbolTokenizer(char symbol[]){
    token temp;
    // Compares string to all possible special characters and special character arrangements
    if(strcmp(symbol, "<=") == 0 || strcmp(symbol, "=<") == 0){
        temp.type = leqsym;
        strcpy(temp.lexeme, symbol);
        return temp;
    }
    else if(strcmp(symbol, ">=") == 0 || strcmp(symbol, "=>") == 0){
        temp.type = geqsym;
        strcpy(temp.lexeme, symbol);
        return temp;
    }
    else if(strcmp(symbol, ":=") == 0){
        temp.type = becomessym;
        strcpy(temp.lexeme, ":=");
        return temp;
    }
    else if(strcmp(symbol, "<>") == 0){
        temp.type = neqsym;
        strcpy(temp.lexeme, "<>");
        return temp;
    }
    else if(strcmp(symbol, "+") == 0){
        temp.type = plussym;
        strcpy(temp.lexeme, "+");
        return temp;
    }
    else if(strcmp(symbol, "-") == 0){
        temp.type = minussym;
        strcpy(temp.lexeme, "-");
        return temp;
    }
    else if(strcmp(symbol, "*") == 0){
        temp.type = multsym;
        strcpy(temp.lexeme, "*");
        return temp;
    }
    else if(strcmp(symbol, "/") == 0){
        temp.type = slashsym;
        strcpy(temp.lexeme, "/");
        return temp;
    }
    else if(strcmp(symbol, "(") == 0){
        temp.type = lparentsym;
        strcpy(temp.lexeme, "(");
        return temp;
    }
    else if(strcmp(symbol, ")") == 0){
        temp.type = rparentsym;
        strcpy(temp.lexeme, ")");
        return temp;
    }
    else if(strcmp(symbol, "=") == 0){
        temp.type = eqlsym;
        strcpy(temp.lexeme, "=");
        return temp;
    }
    else if(strcmp(symbol, ",") == 0){
        temp.type = commasym;
        strcpy(temp.lexeme, ",");
        return temp;
    }
    else if(strcmp(symbol, ".") == 0){
        temp.type = periodsym;
        strcpy(temp.lexeme, ".");
        return temp;
    }
    else if(strcmp(symbol, "<") == 0){
        temp.type = lessym;
        strcpy(temp.lexeme, "<");
        return temp;
    }
    else if(strcmp(symbol, ">") == 0){
        temp.type = gtrsym;
        strcpy(temp.lexeme, ">");
        return temp;
    }
    else{
        temp.type = semicolonsym;
        strcpy(temp.lexeme, ";");
        return temp;
    }
}
// print function for lexeme table
void printLexemeTable(token tokens[], FILE *output, int numOfTokens){
    fprintf(output, "\n\nLexeme Table:\n");
    fprintf(output, "lexeme\t\ttoken type\n");
    for(int i = 0; i<numOfTokens; i++){
        fprintf(output, "%s\t\t%d\n", tokens[i].lexeme, tokens[i].type);
    }
}
// print function for lexeme list
void printLexemeList(token tokens[], FILE *output, int numOfTokens){
    fprintf(output, "\n\nLexeme List:\n");
    for(int i = 0; i<numOfTokens; i++){
        if(tokens[i].type == 2 || tokens[i].type == 3){
            // Allows for printing of lexeme when it is identifier or number
            fprintf(output, "%d %s ", tokens[i].type, tokens[i].lexeme);
        }
        else
            fprintf(output, "%d ", tokens[i].type);
    }
}
FILE* lexAnalyzer(FILE* input, FILE* outputfinal){

    int i = 0; // chararray index
    char c = '\0'; // temp char variable
    char letters[20]; // temp string that starts with letter
    char numbers[20]; // temp string that starts with number
    char validSymbols[3]; //temp string that starts with symbol
    char chararray[MAX_ARRAY_LENGTH]; //temp char array for input characters
    token tokenstream[MAX_ARRAY_LENGTH]; // Array of token structs
    FILE *output = fopen("LOutput.txt", "w"); // Opens file
    while(1){
        c = fgetc(input); // Gets char from file and saves
        if(c == EOF) // Breaks if at end of file
           break;
        else{
            chararray[i] = c; // Enters acquired char in array
            i++;
        }
    }
    chararray[i] = '\0';
    i = 0; // reinitialize i
    int counterLetters = 0; //letter counter
    int counterNumbers = 0; // number counter
    int l = 0; // letter array index
    int n = 0; // number array index
    int t = 0; //token array index
    int s = 0; // symbol array index
    // Loops through char array
    while(chararray[i] != '\0'){
        // If current char is letter
        if(('A' <= chararray[i] && chararray[i] <= 'Z') || ('a' <=chararray[i] && chararray[i]<= 'z')){
            // Loops until chararray[i] is not a letter or number
            while(('A' <= chararray[i] && chararray[i] <= 'Z') || ('a' <=chararray[i] && chararray[i]<= 'z') ||
                ('0' <= chararray[i] && chararray[i] <= '9')){
                counterLetters++;
                if(counterLetters > 11){ // Checks identifier length error
                    letters[l] = chararray[i];
                    letters[l + 1] = '\0';
                    printLexemeTable(tokenstream, output, t);
                    fprintf(output, "%s\t\tERROR", letters);
                    printLexemeList(tokenstream, output, t);
                    fprintf(output, "ERROR ");
                    fprintf(output, "\nERROR: Identifier Name Too Long.");
					fprintf(outputfinal, "\nERROR: Identifier Name Too Long.");
                    exit(1);
                }
                letters[l] = chararray[i]; // Transfers char to temp string
                l++;
                i++;
            }
           
            letters[l] = '\0';

            tokenstream[t] = letterTokenizer(letters); // Creates new token and saves in array
            t++;
            l = 0; // reinitialize l
            counterLetters = 0; // reinitialize counter
        }
        // If chararray[i] is a number
        else if('0' <= chararray[i] && chararray[i] <= '9'){
            // Loops until chararray[i] is not a letter or number
            while((97 <= chararray[i] && chararray[i] <= 122) || (65 <=chararray[i] && chararray[i]<= 90) ||
                (48 <= chararray[i] && chararray[i] <= 57)){
                counterNumbers++;
                // If letter appears after number, then it prints error of invalid identifier name
                if((97 <= chararray[i] && chararray[i] <= 122) || (65 <=chararray[i] && chararray[i]<= 90)){
                    numbers[n] = chararray[i];
                    numbers[n + 1] = '\0';
                    printLexemeTable(tokenstream, output, t);
                    fprintf(output, "%s\t\tERROR", numbers);
                    printLexemeList(tokenstream, output, t);
                    fprintf(output, "ERROR ");
                    fprintf(output, "\nERROR: Identifier Can Not Start With a Number.");
					fprintf(outputfinal, "\nERROR: Identifier Can Not Start With a Number.");
                    exit(2);
                }
                // Prints error message if number is longer than 5 digits
                if(counterNumbers > 5){
                    numbers[n] = chararray[i];
                    numbers[n + 1] = '\0';
                    printLexemeTable(tokenstream, output, t);
                    fprintf(output, "%s\t\tERROR", numbers);
                    printLexemeList(tokenstream, output, t);
                    fprintf(output, "ERROR ");
                    fprintf(output, "\nERROR: Number is Too Long.");
					fprintf(outputfinal, "\nERROR: Number is Too Long.");
                    exit(3);
                }
                numbers[n] = chararray[i]; // Adds char to number temp string
                i++;
                n++;
            }
            // Checks if string is a size less than 5 so we can place NULL terminator at end of string so no garbage values are examined
            
            numbers[n] = '\0';
            
            tokenstream[t] = numberTokenizer(numbers); // Creates new token and saves in array
            t++;
            n = 0; // Reinitializes n
            counterNumbers = 0; // Reinitializes counter
        }
        // If chararray[i] is a special symbol
        else if( chararray[i] == '+' || chararray[i] == '-' || chararray[i] == '*' || chararray[i] == '/' || chararray[i] == '(' ||
            chararray[i] == ')' || chararray[i] == '=' || chararray[i] == ',' || chararray[i] == '.' || chararray[i] == '<' ||
            chararray[i] == '>' || chararray[i] == ';' || chararray[i] == ':'){
            // Checks for /* indicator of comments to be ignored
            if(chararray[i] == '/' && chararray[i+1] == '*'){
                // Loops til it finds signal '*/' that indicates end of comments
                while(1){
                    if (chararray[i] == '*' && chararray[i + 1] == '/')
                        break;
                    else
                    {
                        i++;
                        continue;
                    }
                }
                i+=2; // moves index pass '*/'
            }
            // Checks if symbol is a two symbol op such as '<=', '=>', '<>', ':=', '=<', and '>='
            if((chararray[i] == '<' && chararray[i+1] == '=') || (chararray[i] == '=' && chararray[i+1] == '<') || (chararray[i] == '>' &&
                chararray[i+1] == '=') || (chararray[i] == '=' && chararray[i+1] == '>') || (chararray[i] == ':' && chararray[i+1] == '=')
                || (chararray[i] == '<' && chararray[i+1] == '>')){
                // Saves chars in symbols string
                validSymbols[s] = chararray[i];
                validSymbols[s+1] = chararray[i+1];
                validSymbols[s+2] = '\0'; // Null terminator at end of string
                tokenstream[t] = symbolTokenizer(validSymbols); // Creates new token and saves in array
                t++;
                i+=2; // Moves pass double symbol
                // Resets symbols temp string
                validSymbols[s] = '\0';
                validSymbols[s+1] = '\0';
            }
            // Loops until chararray[i] is not a special symbol
            while(chararray[i] == '+' || chararray[i] == '-' || chararray[i] == '*' || chararray[i] == '/' || chararray[i] == '(' ||
                chararray[i] == ')' || chararray[i] == '=' || chararray[i] == ',' || chararray[i] == '.' || chararray[i] == '<' ||
                chararray[i] == '>' || chararray[i] == ';' || chararray[i] == ':'){
                validSymbols[s] = chararray[i]; // char to symbol temp string
                validSymbols[s+1] = '\0'; // Sets Null terminator in string
                tokenstream[t] = symbolTokenizer(validSymbols); // Creates new token and saves in array
                t++;
                i++;
                validSymbols[s] = '\0'; // Resets symbols temp string
            }
        }
        // If chararrray[i] is a space, new tab, or, new line
        else if(chararray[i] == ' ' || chararray[i] == '\t' || chararray[i] == '\n' || chararray[i] == '\r'){
            i++;
            continue;
        }
        // If chararray[i] is an invalid character
        else{
            printLexemeTable(tokenstream, output, t);
            fprintf(output, "%c\t\tERROR", chararray[i]);
            printLexemeList(tokenstream, output, t);
            fprintf(output, "ERROR ");
            fprintf(output, "\nERROR: Invalid Symbol.");
            exit(4);
        }
    }
    // Prints lexeme table
    printLexemeTable(tokenstream, output, t);
    // Prints lexeme list
    printLexemeList(tokenstream, output, t);
    fclose(output); // Closes file "output.txt"
	fclose(outputfinal);
    return output;
}
