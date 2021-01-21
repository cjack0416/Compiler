"# PL-0 Compiler" 

Instructions on how to run tiny- PL/0 Compiler

1. Make sure files:

  -"CompilerDriver.c"
  -"lex.c"
  -"parsercg.c"
  -"vm.c"

  are all in the same directory.

2. The input file is to be named "input.txt"

3. The output file with all outputs will be available in the same directory in
   which the codes are stored, under "output.txt".

4. In command line type "gcc CompilerDriver.c lex.c parsercg.c vm.c" to compile.

5. Then, type "./a.out input.txt" on the command line and press enter to execute.

6. Once you begin running the executable file, you will enter "./compile" and it can be followed by       
   " -l", " -a", and or " -v".

    - " -l" will print out the lexeme list and table to the screen.
    - " -a" will print out the intermediate code to the screen.
    - " -v" will print out the stack implementation of the virtual machine
      to the screen.

    ex. 1:
    "./compile -l -v" will compile and print "in" + the lexeme list/table +
    the stack implementation of the virtual machine + "out"

    ex. 2:
    "./compile -a -l -v" will compile and print "in" + the intermediate code + the lexeme list/table +
    the stack implementation of the virtual machine + "out"

    ex. 3:
    "./compile" will compile and print "in" and "out". Results will still be available in the same 
    directory as the codes named                   
    "output.txt"

7. The program will exit after directive is fulfilled. 

8. To run new directive, repeat steps 4-6.