//import libraries
#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <cstdio>
#include "handler.h"
#include <typeinfo>
#include <csignal>

//set namespace
using namespace std;

/**
 * @brief Tokenize a string
 *
 * @param str - The string to tokenize
 * @param delim - The string containing delimiter character(s)
 * @return std::vector<std::string> - The list of tokenized strings. Can be empty
 */
std::vector<std::string> tokenize(const std::string &str, const char *delim) {
  char* cstr = new char[str.size() + 1];
  std::strcpy(cstr, str.c_str());

  char* tokenized_string = strtok(cstr, delim);

  std::vector<std::string> tokens;
  while (tokenized_string != NULL)
  {
    tokens.push_back(std::string(tokenized_string));
    tokenized_string = strtok(NULL, delim);
  }
  delete[] cstr;

  return tokens;
}

//returns true if input has an arrow, indicating output redirection, and false otherwise
bool check_arrow(vector<string> input) {

    //initializes bool to false
    bool redirect = false;

    //iterates through input
    for (string token: input) {
        if (token == ">") {
            redirect = true;
        }
    }

    //returns bool
    return redirect;
}

//returns true if input has pipe  and false otherwise
bool check_pipe(vector<string> input) {

    //initializes bool to false
    bool pipe = false;

    //iterates through input
    for (string token: input) {
        if (token == "|") {
            pipe = true;
        }
    }

    //returns bool
    return pipe;
}



//function to run dragon shell
void loop(void){


    //declare variables
    char input[256];
    vector<string> input_list;
    vector<string> actions;

    //run loop for dragonshell until exit
    while(true){

        //print shell
        cout << "dragonshell > ";
        //get input
        cin.getline(input,sizeof(input));

        //tokenize input for multiple commands
        actions = tokenize(input, ";");

        //iterate through distinct commands
        for (string action: actions) {

            //tokenize individual command based on space
            input_list = tokenize(action," ");

            //check if redirect needed
            bool redirect = check_arrow(input_list);

            //check if pipe needed
            bool pipe = check_pipe(input_list);

            if (input_list[0] == "^C") {
                cout << "wow";
            }

            //handle redirect
            else if (redirect) {
                vector<string> output = tokenize(input, ">");
                vector<string> command = tokenize(output[0], " ");
                vector<string> location = tokenize(output[1], " ");
                redirect_output(command, location[0]);
            }

            //handle pipe
            else if (pipe) {
                
                vector<string> output = tokenize(input, "|");
                vector<string> cmd1 = tokenize(output[0], " ");
                vector<string> cmd2 = tokenize(output[1], " ");
                execute_pipe(cmd1, cmd2);
            }

            //hanlde pwd
            else if (input_list[0] == "pwd") {
                pwd();
            }

            //hanlde exit
            else if (input_list[0] == "exit") {
                exit_program();
                return;
            }

            //handle cd
            else if (input_list[0] == "cd") {
                //return error is not enough arguments
                if (input_list.size() < 2) {
                    cout << "expected argument to be \"cd\"\n";
                }

                change_directory(input_list[1].c_str());
            }

            //handle $PATH
            else if (input_list[0] == "$PATH") {
                show_path();
            }

            //handle a2path
            else if (input_list[0] == "a2path") {
                //handle errors
                if (input_list.size() < 2) {
                    cout << "error not enough arguments" << "\n";
                }

                vector<string> new_path = tokenize(input_list[1],":");
                append_path(new_path[1]);
            }

            //handle running an external program
            else {
                external_execution(input_list);

            }

        }


    }

}

int main(int argc, char **argv) {

  // print the string prompt without a newline, before beginning to read
  // tokenize the input, run the command(s), and print the result
  // do this in a loop

  //signal handling
  struct sigaction sa;
  sa.sa_handler = &handle_signal;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;

  if (sigaction(SIGINT, &sa, NULL) == -1) {
      cout << "signal error";
  }



  //print welcome message
  cout << "Welcome to DragonShell!";
  cout << "\n";

  //call function to start dragonshell
  loop();



  return 0;
}
