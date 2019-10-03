#include <vector>
#include <iostream>
#include <unistd.h>
#include "handler.h"
#include <signal.h>
#include <typeinfo>
#include <fcntl.h>
#include <sys/wait.h>

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

void redirect_output(vector<string> output, string location) {
    int pid;

    const char * loc = (char*)location.c_str();

    if ((pid = fork()) == -1) {
        perror("fork error");
    }

    else if (pid == 0) {
        int file_desc = open(loc, O_CREAT | O_WRONLY);
        if(file_desc < 0) {
            cout << "Error opening the file" << "\n";
        }

        dup2(file_desc, 1);
        external_execution(output);
        close(file_desc);
        _exit(0);

    }

    else {
        wait(NULL);
    }

}

void execute_pipe(vector<string> cmd1, vector<string> cmd2) {
    int p[2];
    int p1,p2;
    int i = 0;
    char *argv1[cmd1.size()+1];
    char *argv2[cmd2.size()+1];
    char *env[] = {NULL};

    for (string cmd : cmd1) {
        argv1[i] = (char *)cmd.c_str();
        i++;
    }

    argv1[i] = NULL;
    i = 0;

    for (string cmd : cmd2) {
        argv2[i] = (char *)cmd.c_str();
        i++;
    }

    argv2[i] = NULL;

    if (pipe(p) < 0) {
        cout << "error";
    }

    p1 = fork();

    if (p1 < 0) {
        cout << "fork error";
    }

    //first child process is running
    if (p1 == 0) {
        //write process to pipe
        close(p[0]);
        dup2(p[1], 1);
        close(p[1]);
        _exit(0);

        if (execve(argv1[0], argv1,env) < 0) {
            //before this should probable check if file exists
            cout << "couldn't execute process 1";
        }
    }
    //parent process
    else {
        //create second child
        p2 = fork();

        if (p2 < 1) {
            cout << "fork error";
        }

        //child process is running
        if (p2 == 0){
            //only need to acess read end
            close(p[1]);
            dup2(p[0],1);
            close(p[1]);
            _exit(0);

            if (execve(argv2[0], argv2,env) < 0) {
                 cout << "couldn't execute process 2";
            }
        } else {
            //parrent is executing
            //wait for two children
            wait(NULL);
            wait(NULL);

        }



    }




}

//argv** thats const char size tokenzied array +1
// iterate through tokenized input and add to argv
//terminate argv w NULL
//iterate through paths and create const char*

//kil process
