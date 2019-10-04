//import libraries
#include <vector>
#include <iostream>
#include <unistd.h>
#include "handler.h"
#include <signal.h>
#include <typeinfo>
#include <fcntl.h>
#include <sys/wait.h>

//set namespace
using namespace std;

//declare / initialize variables
char cwd[256];
vector<string> path = {"/bin/","/usr/bin/"};
vector<pid_t> pids = {};

//function to handle pwd
void pwd(){

    //get cwd using getwd system call
    getcwd(cwd, sizeof(cwd));

    //output results
    cout << cwd;
    cout << "\n";
}

//function to handle cd
void change_directory(char const *path){

    //use chdir system call to change directory
    int dir = chdir(path);

    //return error if necessary
    if (dir < 0) {
        cout << "No such file or directorysss\n";
    }
}

//function to handle exiting shell
void exit_program(){

    //print message
    for (pid_t pid: pids) {
        kill(pid, SIGTERM);
    }

    pids.clear();

    cout << "Exiting\n";

    _exit(0);

}

//function to display path
void show_path() {

    //print message
    cout << "Current PATH: ";

    //iterate through directories in PATH
    for (string dir : path) {
        //display path
        cout << dir;

        //if not last element append colon
        if (dir != path.back()){
            cout << ":";
        }
    }

    //display newline
    cout << "\n";
}

//function to execute command
void execute(const char* cmd, vector<string> input) {



    //declare / initialize variables
    char *argv[input.size()+1];
    char *env[] = {NULL};
    int i = 0;
    int pid;

    //append command and arguments to array of character pointers
    for (string test : input) {
        argv[i] = (char *)test.c_str();
        i++;
    }

    //terminatate array with null
    argv[i] = NULL;

    pid = fork();



    //if error in fork return error
    // if (pid == -1) {
    //     perror("fork error");
    // }


    //enter child process
    if (pid == 0) {

        pids.push_back(pid);


        //return error if necessary
        if(execve(cmd, argv, env) == -1){
            perror("execve");
        }
        _exit(0);

    }

    //if parent wait for child
    else {
        wait(NULL);
    }

}


//function to handle external program execution
void external_execution(vector<string> input) {
    //declare and initialize things


    const char* cmd_1 = input[0].c_str();
    const char* cmd_2;
    int succ = 0;

    //check if commands exists in cwd, if so execute
    if (access(cmd_1, F_OK) == 0){
        succ = 1;
        execute(cmd_1, input);
    }

    //check if commands exists in path, if so execute
    for (string wow: path) {
        wow += cmd_1;
        cmd_2 = wow.c_str();

        if (access(cmd_2, F_OK) == 0){

            succ = 1;
            execute(cmd_2, input);
            break;
        }

    }

    //report error if command doesnt exist
    if (!succ) {
        cout << "Error: Program not found";
    }
}

//append directories to path
void append_path(string item) {
    path.push_back(item);

}

//function to handle output redirection
void redirect_output(vector<string> output, string location) {
    //declare and init variables
    pid_t pid;
    const char * loc = (char*)location.c_str();

    //report fork error if necessary
    if ((pid = fork()) == -1) {
        perror("fork error");
    }

    //enter child process
    else if (pid == 0) {

        pids.push_back(pid);
        //open file
        int file_desc = open(loc, O_CREAT | O_WRONLY, 0666);

        //report error if necessary
        if(file_desc < 0) {
            cout << "Error opening the file" << "\n";
        }

        //create alias for stdou with file, and execute command
        dup2(file_desc, 1);
        external_execution(output);
        close(file_desc);
        _exit(0);

    }

    //if parent wait for child
    else {
        wait(NULL);
    }

}

//function to handle pipe
void execute_pipe(vector<string> cmd1, vector<string> cmd2) {
    //declare and inititialize variables
    int p[2];
    pid_t p1,p2;

    //return error if pipe failure
    if (pipe(p) < 0) {
        cout << "error";
    }

    //fork
    p2 = fork();

    //first child process is running
    if (p2 == 0) {

        pids.push_back(p2);

        //have child start reading pipe
        close(p[1]);
        dup2(p[0],0);
        close(p[0]);

        //execute command that requires input
        external_execution(cmd2);

        //exit process
        _exit(0);
    }

    //if parent
    else {

        //create fork
        p1 = fork();

        //if child
        if (p1 == 0){

            pids.push_back(p1);

            //have child write to pipe
            close(p[0]);
            dup2(p[1], 1);
            close(p[1]);

            //execute command that creates input for other command
            external_execution(cmd1);

            //exit process
            _exit(0);

        }

    }

    //close both pipe channels
    close(p[0]);
    close(p[1]);
    wait(NULL);
    wait(NULL);

}

//handle keyboard interrupt
void handle_signal(int signum) {

    //kill foreground processes
    for (pid_t pid : pids) {
        kill(pid, signum);
    }

    //clear pids global vector
    pids.clear();

}

//function to run process in background
void background_process(vector<string> input) {

    //init variables
    pid_t pid = fork();

    //return fork error if needed
    if (pid < 0) {
        perror("fork error");
    }

    //enter child process
    if (pid==0) {

        pids.push_back(pid);

        //close stdout and stderr
        close(1);
        close(2);

        //execute process to be run in background
        external_execution(input);

    }
    //enter parent
    else {

        cout << "PID " << pid << " is running in the background" << "\n";

    }

}
