#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "handler.h"

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
    int SIZE = 256;
    char input[SIZE];
    int status = 1;
    char cwd[SIZE];

    char *input_split[10];

    //create loop
    do{
        //print shell
        printf("dragonshell > ");
        scanf("%s", input);

        //toeknize input
        tokenize(input, " ", input_split);

        for (int i=0;i<10;i++){
            printf("\n%s %d", input_split[i],i);
        }

        // printf("%s", input_split[1]);

        // //handle exit case
        // if (strcmp("exit", &input[0]) == 0) {
        //     status = exit_program();
        // }
        //
        // //handle pwd case
        // if (strcmp("pwd", &input[0]) == 0) {
        //     pwd();
        // }
        //
        // if (strcmp("cd", &input[0]) == 0) {
        //     printf("%s", &input[1]);
        //
        // }



    } while(status != 0);
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
