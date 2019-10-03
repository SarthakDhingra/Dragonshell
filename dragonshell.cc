#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <cstdio>
#include "handler.h"
#include <typeinfo>

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

bool check_arrow(vector<string> input) {

    bool check = false;

    for (string piece: input) {
        if (piece == ">") {
            check=true;
        }
    }

    return check;
}

bool check_pipe(vector<string> input) {

    bool check = false;

    for (string piece: input) {
        if (piece == "|") {
            check=true;
        }
    }

    return check;
}




void loop(void){

  char input[256];
  int status = 1;
  vector<string> input_list;
  vector<string> actions;


  while(status){
    cout << "dragonshell > ";
    cin.getline(input,sizeof(input));

    actions = tokenize(input, ";");

    for (string action: actions) {

        input_list = tokenize(action," ");

        bool redirect = check_arrow(input_list);

        bool pipe = check_pipe(input_list);

        if (redirect) {
            vector<string> output = tokenize(input, ">");
            vector<string> command = tokenize(output[0], " ");
            vector<string> location = tokenize(output[1], " ");
            redirect_output(command, location[0]);
        }

        else if (pipe) {
            vector<string> output = tokenize(input, ">");
            vector<string> cmd1 = tokenize(output[0], " ");
            vector<string> cmd2 = tokenize(output[1], " ");
            execute_pipe(cmd1, cmd2);
        }

        else if (input_list[0] == "pwd") {
          pwd();
        }

        else if (input_list[0] == "exit") {
          status = exit_program();
          return;
        }

        else if (input_list[0] == "cd") {
          if (input_list.size() < 2) {
            cout << "expected argument to be \"cd\"\n";
          }
          change_directory(input_list[1].c_str());
        }

        else if (input_list[0] == "$PATH") {
            show_path();
        }

        else if (input_list[0] == "a2path") {
            vector<string> new_path = tokenize(input_list[1],":");
            append_path(new_path[1]);
        }

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

  cout << "Welcome to DragonShell!";
  cout << "\n";



  loop();

  return 0;
}
