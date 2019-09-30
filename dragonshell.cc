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


void loop(void){

  char input[256];
  int status = 1;
  vector<string> input_list;

  while(status){
    cout << "dragonshell > ";
    cin.getline(input,sizeof(input));

    input_list = tokenize(input," ");

    if (input_list[0] == "pwd") {
      pwd();
    }

    if (input_list[0] == "exit") {
      status = exit_program();
    }

    if (input_list[0] == "cd") {
      if (input_list.size() < 2) {
        cout << "expected argument to \"cd\"\n";
      }
      change_directory(input_list[1].c_str());
    }

    if (input_list[0] == "$PATH") {
        show_path();
    }

    if (input_list[0] == "a2path") {
        vector<string> new_path = tokenize(input_list[1],":");
        append_path(new_path[1]);
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
