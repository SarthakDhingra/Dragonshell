#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

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

    //create loop
    do{
        printf("dragonshell > ");
        scanf("%s", input);

        if (strcmp("exit", input) == 0) {
            printf("Exiting\n");
            status = 0;
        }

        if (strcmp("pwd", input) == 0) {
            getcwd(cwd, sizeof(cwd));
            printf("%s", cwd);
        }



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


//questions:
//can I use strlen/sizeof (in glibc?). If not what to use instead
//can I use newline in loop
//is loop the best way to do it
//can we use strcmp, if not how should I compare strings
//what headers can i not use (functions from them)
