// Chris Jackson and Laurell Cuza
// Compiler Driver
// COP 3402-C001

#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//Declarations
FILE* lexAnalyzer(FILE *input, FILE *output);
FILE* parserCodeGen(FILE *AOutput, FILE *IntermediateCode, FILE *output);
FILE* virtualMachine(FILE *input);
//Method to copy elements of one file to another file
void cpytoFile(FILE *file1, FILE *file2){
    char c = fgetc(file1);
    while (c != EOF)
    {
        fprintf(file2,"%c", c);
        c = fgetc(file1);
    }
}
//Method to print elements of a file to the screen
void printToScreen(FILE *file){
    char s = fgetc(file);
    while(s!=EOF) {
       printf("%c",s);
        s = fgetc(file);
    }
}
// Compiles the input
void drive(FILE *output1, FILE *output2, FILE *output3, FILE *outputFinal, FILE *inputFile){
    //Call to method in lex.c
	outputFinal = fopen("output.txt", "a");
    output1 = lexAnalyzer(inputFile, outputFinal);
    FILE *intermediateCode = fopen("IntermediateCode.txt", "w");
    output2 = fopen("AOutput.txt", "w");
    //Call to method in parsercg.c
    intermediateCode = parserCodeGen(output2, intermediateCode, outputFinal);
    fclose(intermediateCode);
    fclose(output2);
	fclose(outputFinal);
    intermediateCode = fopen("IntermediateCode.txt", "r");
    //Call to method in vm.c
    output3 = virtualMachine(intermediateCode);
    fclose(intermediateCode);
    output1 = fopen("LOutput.txt", "r");
    outputFinal = fopen("output.txt", "a");
    fprintf(outputFinal, "\n");
    cpytoFile(output1, outputFinal);
    fclose(output1);
    fclose(outputFinal);
    FILE *symbols = fopen("symbolictable.txt", "r");
    outputFinal = fopen("output.txt", "a");
    fprintf(outputFinal, "\n\n");
    cpytoFile(symbols, outputFinal);
    fclose(symbols);
    fclose(outputFinal);
    output2 = fopen("AOutput.txt", "r");
    outputFinal = fopen("output.txt", "a");
    fprintf(outputFinal, "\n\n");
    cpytoFile(output2, outputFinal);
    fclose(output2);
    fclose(outputFinal);
    output3 = fopen("VOutput.txt", "r");
    outputFinal = fopen("output.txt", "a");
    cpytoFile(output3, outputFinal);
    fclose(output3);
    fclose(outputFinal);
}
int main(int argc, char * argv[]) {
	if (argc == 1)
	{
		printf("No file name inputed");
		exit(1);
	}

	if (argc > 2)
	{
		printf("Too many arguments");
		exit(2);
	}
	
	printf("\n||||| Read the \"README.txt\" before using compiler |||||\n\n");
	printf("%s\n", argv[1]);
	FILE *inputFile = fopen(argv[1], "r");
    FILE *output1;
    FILE *output3;
    FILE *output2;
    FILE *outputFinal;
    
        char directive[25];
        printf("\nDirective: ");
        gets(directive);
		printf("%s", directive);
        if(strcmp(directive, "./compile") == 0){
            printf("in\n");
			outputFinal = fopen("output.txt", "w");
			fprintf(outputFinal, "Input File:\n");
			cpytoFile(inputFile, outputFinal);
			fclose(inputFile);
			fclose(outputFinal);
			inputFile = fopen(argv[1], "r");
			drive(output1, output2, output3, outputFinal, inputFile);
            printf("out\n");
        }
        else if(strcmp(directive, "./compile -l") == 0){
			outputFinal = fopen("output.txt", "w");
			fprintf(outputFinal, "Input File:\n");
			cpytoFile(inputFile, outputFinal);
			fclose(inputFile);
			fclose(outputFinal);
			inputFile = fopen(argv[1], "r");
			drive(output1, output2, output3, outputFinal, inputFile);
            printf("\n\n");
            output1 = fopen("LOutput.txt", "r");
            printToScreen(output1);
            fclose(output1);
            printf("\n\n");
        }
        else if(strcmp(directive, "./compile -a") == 0){
			outputFinal = fopen("output.txt", "w");
			fprintf(outputFinal, "Input File:\n");
			cpytoFile(inputFile, outputFinal);
			fclose(inputFile);
			fclose(outputFinal);
			inputFile = fopen(argv[1], "r");
			drive(output1, output2, output3, outputFinal, inputFile);
            printf("\n\n");
            output2 = fopen("AOutput.txt", "r");
            printToScreen(output2);
            fclose(output2);
            printf("\n\n");
        }
        else if(strcmp(directive, "./compile -v") == 0){
			outputFinal = fopen("output.txt", "w");
			fprintf(outputFinal, "Input File:\n");
			cpytoFile(inputFile, outputFinal);
			fclose(inputFile);
			fclose(outputFinal);
			inputFile = fopen(argv[1], "r");
			drive(output1, output2, output3, outputFinal, inputFile);
            printf("\n\n");
            output3 = fopen("VOutput.txt", "r");
            printToScreen(output3);
            fclose(output3);
            printf("\n\n");
        }
        else if(strcmp(directive, "./compile -l -a") == 0){
			outputFinal = fopen("output.txt", "w");
			fprintf(outputFinal, "Input File:\n");
			cpytoFile(inputFile, outputFinal);
			fclose(inputFile);
			fclose(outputFinal);
			inputFile = fopen(argv[1], "r");
			drive(output1, output2, output3, outputFinal, inputFile);
            printf("\n\n");
            output1 = fopen("LOutput.txt", "r");
            printToScreen(output1);
            fclose(output1);
            printf("\n\n");
            output2 = fopen("AOutput.txt", "r");
            printToScreen(output2);
            fclose(output2);
            printf("\n\n");
        }
        else if(strcmp(directive, "./compile -a -l") == 0){
			outputFinal = fopen("output.txt", "w");
			fprintf(outputFinal, "Input File:\n");
			cpytoFile(inputFile, outputFinal);
			fclose(inputFile);
			fclose(outputFinal);
			inputFile = fopen(argv[1], "r");
			drive(output1, output2, output3, outputFinal, inputFile);
            printf("\n\n");
            output2 = fopen("AOutput.txt", "r");
            printToScreen(output2);
            fclose(output2);
            printf("\n\n");
            output1 = fopen("LOutput.txt", "r");
            printToScreen(output1);
            fclose(output1);
            printf("\n\n");
        }
        else if(strcmp(directive, "./compile -l -v") == 0){
			outputFinal = fopen("output.txt", "w");
			fprintf(outputFinal, "Input File:\n");
			cpytoFile(inputFile, outputFinal);
			fclose(inputFile);
			fclose(outputFinal);
			inputFile = fopen(argv[1], "r");
			drive(output1, output2, output3, outputFinal, inputFile);
            printf("\n\n");
            output1 = fopen("LOutput.txt", "r");
            printToScreen(output1);
            fclose(output1);
            printf("\n\n");
            output3 = fopen("VOutput.txt", "r");
            printToScreen(output3);
            fclose(output3);
            printf("\n\n");
        }
        else if(strcmp(directive, "./compile -v -l") == 0){
		outputFinal = fopen("output.txt", "w");
		fprintf(outputFinal, "Input File:\n");
		cpytoFile(inputFile, outputFinal);
		fclose(inputFile);
		fclose(outputFinal);
		inputFile = fopen(argv[1], "r");
		drive(output1, output2, output3, outputFinal, inputFile);
            printf("\n\n");
            output3 = fopen("VOutput.txt", "r");
            printToScreen(output3);
            fclose(output3);
            printf("\n\n");
            output1 = fopen("LOutput.txt", "r");
            printToScreen(output1);
            fclose(output1);
            printf("\n\n");
        }
        else if(strcmp(directive, "./compile -a -v") == 0){
		outputFinal = fopen("output.txt", "w");
		fprintf(outputFinal, "Input File:\n");
		cpytoFile(inputFile, outputFinal);
		fclose(inputFile);
		fclose(outputFinal);
		inputFile = fopen(argv[1], "r");
		drive(output1, output2, output3, outputFinal, inputFile);
            printf("\n\n");
            output2 = fopen("AOutput.txt", "r");
            printToScreen(output2);
            fclose(output2);
            printf("\n\n");
            output3 = fopen("VOutput.txt", "r");
            printToScreen(output3);
            fclose(output3);
            printf("\n\n");
        }
        else if(strcmp(directive, "./compile -v -a") == 0){
		outputFinal = fopen("output.txt", "w");
		fprintf(outputFinal, "Input File:\n");
		cpytoFile(inputFile, outputFinal);
		fclose(inputFile);
		fclose(outputFinal);
		inputFile = fopen(argv[1], "r");
		drive(output1, output2, output3, outputFinal, inputFile);
            printf("\n\n");
            output3 = fopen("VOutput.txt", "r");
            printToScreen(output3);
            fclose(output3);
            printf("\n\n");
            output2 = fopen("AOutput.txt", "r");
            printToScreen(output2);
            fclose(output2);
            printf("\n\n");
        }
        else if(strcmp(directive, "./compile -l -a -v") == 0){
		outputFinal = fopen("output.txt", "w");
		fprintf(outputFinal, "Input File:\n");
		cpytoFile(inputFile, outputFinal);
		fclose(inputFile);
		fclose(outputFinal);
		inputFile = fopen(argv[1], "r");
		drive(output1, output2, output3, outputFinal, inputFile);
            printf("\n\n");
            output1 = fopen("LOutput.txt", "r");
            printToScreen(output1);
            fclose(output1);
            printf("\n\n");
            output2 = fopen("AOutput.txt", "r");
            printToScreen(output2);
            fclose(output2);
            printf("\n\n");
            output3 = fopen("VOutput.txt", "r");
            printToScreen(output3);
            fclose(output3);
            printf("\n\n");
        }
        else if(strcmp(directive, "./compile -a -v -l") == 0){
		outputFinal = fopen("output.txt", "w");
		fprintf(outputFinal, "Input File:\n");
		cpytoFile(inputFile, outputFinal);
		fclose(inputFile);
		fclose(outputFinal);
		inputFile = fopen(argv[1], "r");
		drive(output1, output2, output3, outputFinal, inputFile);
            printf("\n\n");
            output2 = fopen("AOutpu.txtt", "r");
            printToScreen(output2);
            fclose(output2);
            printf("\n\n");
            output3 = fopen("VOutput.txt", "r");
            printToScreen(output3);
            fclose(output3);
            printf("\n\n");
            output1 = fopen("LOutput.txt", "r");
            printToScreen(output1);
            fclose(output1);
            printf("\n\n");
        }
        else if(strcmp(directive, "./compile -v -l -a") == 0){
		outputFinal = fopen("output.txt", "w");
		fprintf(outputFinal, "Input File:\n");
		cpytoFile(inputFile, outputFinal);
		fclose(inputFile);
		fclose(outputFinal);
		inputFile = fopen(argv[1], "r");
		drive(output1, output2, output3, outputFinal, inputFile);
            printf("\n\n");
            output3 = fopen("VOutput.txt", "r");
            printToScreen(output3);
            fclose(output3);
            printf("\n\n");
            output1 = fopen("LOutput.txt", "r");
            printToScreen(output1);
            fclose(output1);
            printf("\n\n");
            output2 = fopen("AOutput.txt", "r");
            printToScreen(output2);
            fclose(output2);
            printf("\n\n");
        }
        else if(strcmp(directive, "./compile -v -a -l") == 0){
		outputFinal = fopen("output.txt", "w");
		fprintf(outputFinal, "Input File:\n");
		cpytoFile(inputFile, outputFinal);
		fclose(inputFile);
		fclose(outputFinal);
		inputFile = fopen(argv[1], "r");
		drive(output1, output2, output3, outputFinal, inputFile);
            printf("\n\n");
            output3 = fopen("VOutput.txt", "r");
            printToScreen(output3);
            fclose(output3);
            printf("\n\n");
            output2 = fopen("AOutput.txt", "r");
            printToScreen(output2);
            fclose(output2);
            printf("\n\n");
            output1 = fopen("LOutput.txt", "r");
            printToScreen(output1);
            fclose(output1);
            printf("\n\n");
        }
        else if(strcmp(directive, "./compile -l -v -a") == 0){
		outputFinal = fopen("output.txt", "w");
		fprintf(outputFinal, "Input File:\n");
		cpytoFile(inputFile, outputFinal);
		fclose(inputFile);
		fclose(outputFinal);
		inputFile = fopen(argv[1], "r");
		drive(output1, output2, output3, outputFinal, inputFile);
            printf("\n\n");
            output1 = fopen("LOutput.txt", "r");
            printToScreen(output1);
            fclose(output1);
            printf("\n\n");
            output3 = fopen("VOutput.txt", "r");
            printToScreen(output3);
            fclose(output3);
            printf("\n\n");
            output2 = fopen("AOutput.txt", "r");
            printToScreen(output2);
            fclose(output2);
            printf("\n\n");
        }
        else if(strcmp(directive, "./compile -a -l -v") == 0){
		outputFinal = fopen("output.txt", "w");
		fprintf(outputFinal, "Input File:\n");
		cpytoFile(inputFile, outputFinal);
		fclose(inputFile);
		fclose(outputFinal);
		inputFile = fopen(argv[1], "r");
		drive(output1, output2, output3, outputFinal, inputFile);
            printf("\n\n");
            output2 = fopen("AOutput.txt", "r");
            printToScreen(output2);
            fclose(output2);
            printf("\n\n");
            output1 = fopen("LOutput.txt", "r");
            printToScreen(output1);
            fclose(output1);
            printf("\n\n");
            output3 = fopen("VOutput.txt", "r");
            printToScreen(output3);
            fclose(output3);
            printf("\n\n");
        } 
        else{
            printf("Invalid input. Please re-read README.txt and try again \n");
        }
		fclose(inputFile);
    return 0;
}
