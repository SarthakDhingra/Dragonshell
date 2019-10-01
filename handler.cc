#include <vector>
#include <iostream>
#include <unistd.h>
#include "handler.h"

#include <typeinfo>


using namespace std;

char cwd[256];

vector<string> path = {"/bin/","/usr/bin/"};

void pwd(){
  getcwd(cwd, sizeof(cwd));
  cout << cwd;
  cout << "\n";
}

void change_directory(char const *path){

  int rc = chdir(path);

  if (rc < 0) {
    cout << "No such file or directory\n";
  }
}

int exit_program(){
  cout << "Exiting\n";
  return 0;
}

void show_path() {
    cout << "Current PATH: ";

    for (vector<string>::iterator i = path.begin(); i != path.end(); ++i) {
        cout << *i;

        if (*i != path.back()){
            cout << ":";
        }
    }

    cout << "\n";
}

void execute(const char* cmd, vector<string> args) {

    cout << "FOUND";

}

void external_execution(vector<string> input) {
    const char* cmd_1 = input[0].c_str();
    const char* cmd_2;
    int succ = 0;

    for (vector<string>::iterator i = path.begin(); i != path.end(); ++i) {

        cmd_2 = *i + cmd_1;
        succ = 0;
        if (access(cmd_2, F_OK) == 0){
            succ =1;
            execute(cmd_2, input);
            return;
        }
    }

    if (access(cmd_1, F_OK) == 0){
        succ = 1;
        execute(cmd_1, input);
        return;
    }

    if (!succ) {
        cout << "Error: Program not found";
    }
}

void append_path(string item) {
    path.push_back(item);

}
