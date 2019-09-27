#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "handler.h"
#include <stdlib.h>

/**
 * @brief Tokenize a C string
 *
 * @param str - The C string to tokenize
 * @param delim - The C string containing delimiter character(s)
 * @param argv - A char* array that will contain the tokenized strings
 * Make sure that you allocate enough space for the array.
 */
void tokenize(char* str, const char* delim, char ** argv) {
  char* token;
  token = strtok(str, delim);
  for(size_t i = 0; token != NULL; ++i){
    argv[i] = token;
  token = strtok(NULL, delim);
  }
}

//functiont to loop until exit
void loop(void){

    //initialize / declare variables
    int status = 1;
    char *input_split[10];
    char *input = NULL;
    size_t size = 0;

    //create loop
    do{
        //print shell
        printf("dragonshell > ");

        //read input
        ssize_t num  = getline(&input, &size, stdin);

        //handle error
        if (num == -1) {
            perror("getline");
        }

        //toeknize input
        tokenize(input, " ", input_split);

        // printf("%s", input_split[0]);

        printf("%d", strcmp("cd",input_split[0]));

        //handle exit case
        if (strcmp("exit ", input_split[0]) == 0) {
            status = exit_program();
        }

        //handle pwd case
        if (strcmp("pwd", input_split[0]) == 0) {
            pwd();
        }

        if (strcmp("cd", input_split[0]) == 0) {
            change_directory(input_split[1]);
        }



    } while(status != 0);

    //free memory
    free(input);
}

int main(int argc, char **argv) {

    // print the string prompt without a newline, before beginning to read
    // tokenize the input, run the command(s), and print the result
    // do this in a loop

    //write welcome message
    printf("Welcome to DragonShell\n");

    //create loop
    loop();

    return 0;
}
