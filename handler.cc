#include <vector>
#include <iostream>
#include <unistd.h>
#include "handler.h"
#include <signal.h>
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

void execute(const char* cmd, vector<string> input) {

    char *argv[input.size()+1];
    char *env[] = {NULL};
    int i = 0;
    int pid;

    for (string test : input) {
        argv[i] = (char *)test.c_str();
        i++;
    }

    argv[i] = NULL;

    if ((pid = fork()) == -1) {
        perror("fork error");
    }

    else if (pid == 0) {
        if(execve(cmd, argv, env) == -1){
            perror("execve");
        }
        _exit(0);

    }

    else {
        wait(NULL);
    }

}

void external_execution(vector<string> input) {
    const char* cmd_1 = input[0].c_str();
    const char* cmd_2;
    int succ = 0;

    if (access(cmd_1, F_OK) == 0){
        succ = 1;
        execute(cmd_1, input);
    }

    for (string wow: path) {
        wow += cmd_1;
        cmd_2 = wow.c_str();

        if (access(cmd_2, F_OK) == 0){
            succ = 1;
            execute(cmd_2, input);
            break;
        }

    }


    if (!succ) {
        cout << "Error: Program not found";
    }
}

void append_path(string item) {
    path.push_back(item);

}

//argv** thats const char size tokenzied array +1
// iterate through tokenized input and add to argv
//terminate argv w NULL
//iterate through paths and create const char*

//kil process
