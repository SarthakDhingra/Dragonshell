//import libraries
#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <cstdio>
#include <typeinfo>
#include "handler.h"
#include <csignal>


//set namespace
using namespace std;

//function to tokenize a string
vector<string> tokenize(const string &str, const char *delim) {
  char* cstr = new char[str.size() + 1];
  strcpy(cstr, str.c_str());

  char* tokenized_string = strtok(cstr, delim);

  vector<string> tokens;
  while (tokenized_string != NULL)
  {
    tokens.push_back(std::string(tokenized_string));
    tokenized_string = strtok(NULL, delim);
  }
  delete[] cstr;

  return tokens;
}

//returns true if input has ampersand and false otherwise
bool check_symbol(vector<string> input, string ch) {

    //initializes bool to false
    bool symbol = false;

    //iterates through input
    for (string token: input) {
        if (token == ch) {
            symbol = true;
        }
    }

    //returns bool
    return symbol;
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

        //handle Ctrl-D
        if (cin.eof()){
            exit_program();
            return;
        }

        //tokenize input for multiple commands
        actions = tokenize(input, ";");

        //iterate through distinct commands
        for (string action: actions) {

            //tokenize individual command based on space
            input_list = tokenize(action," ");

            //check if redirect needed
            bool redirect = check_symbol(input_list, ">");

            //check if pipe needed
            bool pipe = check_symbol(input_list, "|");

            //check if need to run background process
            bool background = check_symbol(input_list, "&");

            //handle background process
            if (background) {

                //remove ampersand
                input_list.pop_back();
                //execute background process
                background_process(input_list);

            }

            //handle redirect
            else if (redirect) {
                vector<string> output = tokenize(action, ">");
                vector<string> command = tokenize(output[0], " ");
                vector<string> location = tokenize(output[1], " ");
                redirect_output(command, location[0]);
            }

            //handle pipe
            else if (pipe) {
                vector<string> output = tokenize(action, "|");
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
                    cout << "dragonshell: expected argument to \"cd\"\n";
                }
                else {
                    change_directory(input_list[1].c_str());
                }


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

                //append new path to $PATH
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

//main function
int main(int argc, char **argv) {

    //signal handling for Ctrl-c and Ctrl-D
    struct sigaction sa;

    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = &handle_signal;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTSTP, &sa, NULL);

    //print welcome message
    cout << "                                   ______________                                           					\n";
    cout << "                             ,===:'.,            `-._                                       					\n";
    cout << "                                  `:.`---.__         `-._                                   					\n";
    cout << "                                    `:.     `--.         `.                                 					\n";
    cout << "                                      \\.        `.         `.                               					\n";
    cout << "                              (,,(,    \\.         `.   ____,-`.,                            					\n";
    cout << "                           (,'     `/   \\.   ,--.___`.'                                     					\n";
    cout << "                       ,  ,'  ,--.  `,   \\.;'         `                                     					\n";
    cout << "                        `{D, {    \\  :    \\;                                                					\n";
    cout << "                          V,,'    /  /    //                                                					\n";
    cout << "                          j;;    /  ,' ,-//.    ,---.      ,                                					\n";
    cout << "                          \\;'   /  ,' /  _  \\  /  _  \\   ,'/                                					\n";
    cout << "                                \\   `'  / \\  `'  / \\  `.' /                                 					\n";
    cout << "                                 `.___,'   `.__,'   `.__,'                                  					\n";
    cout << "                                                                                            					\n";
    cout << "__      __   _                    _         ___                           ___ _        _ _ 						\n";
    cout << "\\ \\    / /__| |__ ___ _ __  ___  | |_ ___  |   \\ _ _ __ _ __ _ ___ _ _   / __| |_  ___| | | 					\n";
    cout << " \\ \\/\\/ / -_) / _/ _ \\ '  \\/ -_) |  _/ _ \\ | |) | '_/ _` / _` / _ \\ ' \\  \\__ \\ ' \\/ -_) | | 			\n";
    cout << "  \\_/\\_/\\___|_\\__\\___/_|_|_\\___|  \\__\\___/ |___/|_| \\__,_\\__, \\___/_||_| |___/_||_\\___|_|_|			\n";
    cout << "                                                         |___/													\n";
    cout << "																												\n";
    cout << "																												\n";
    cout << "																												\n";


  //call function to start dragonshell
  loop();

  return 0;
}
