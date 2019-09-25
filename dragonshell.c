#include <stddef.h>
#include <string.h>
#include <unistd.h>

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
    char *message = "dragonshell > ";
    int length = sizeof(char)*strlen(message);
    int SIZE = 100;
    char data[SIZE];

    //create loop
    do{

        write(1, message, length);

        read(1,data,100);

    } while(strlen(data)>0);
}

int main(int argc, char **argv) {

    // print the string prompt without a newline, before beginning to read
    // tokenize the input, run the command(s), and print the result
    // do this in a loop

    //declare message pointer and byte length
    char *welcome = "Welcome to Dragonshell!";
    int length = sizeof(char)*strlen(welcome);

    //write welcome message
    write(1,welcome,length);

    //write welcome message
    write(1,"\n",1);

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
