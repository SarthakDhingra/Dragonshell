//import libraries
#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <cstdio>
#include <typeinfo>
#include <csignal>
#include <fstream>
#include <unistd.h>
#include <signal.h>
#include <typeinfo>
#include <fcntl.h>
#include <sys/wait.h>

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


//declare / initialize global variables
vector<string> path = {"/bin/","/usr/bin/"};
vector<pid_t> pids = {};

//function to handle pwd
void pwd(){
    char cwd[256];

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
        cout << "dragonshell: No such file or directory\n";
    }
}

//function to handle exiting shell
void exit_program(){

    //print message
    for (pid_t pid: pids) {
        cout << pid;
        kill(pid, SIGTERM);
    }

    //clear vector of pids
    pids.clear();

    //print exist statement
    cout << "Exiting\n";

    //terminate
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
    pid_t pid;

    //append command and arguments to array of character pointers
    for (string test : input) {
        argv[i] = (char *)test.c_str();
        i++;
    }

    //terminatate array with null
    argv[i] = NULL;

    //create fork
    pid = fork();


    //enter child process
    if (pid == 0) {

        //add pids to global vector
        pids.push_back(pid);


        //execute cmd and return error if needed
        if(execve(cmd, argv, env) == -1){
            perror("execve");
        }

        //terminate
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
    int success = 0;

    //check if commands exists in cwd, if so execute
    if (access(cmd_1, F_OK) == 0){
        success = 1;
        execute(cmd_1, input);
    }

    //check if commands exists in path, if so execute
    for (string wow: path) {
        wow += cmd_1;
        cmd_2 = wow.c_str();

        //check if command exists in path
        if (access(cmd_2, F_OK) == 0){
            //execute command
            success = 1;
            execute(cmd_2, input);
            break;
        }

    }

    //report error if command doesnt exist
    if (!success) {
        cout << "dragonshell: Command not found\n";
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

        //append pid to global vector
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

        //append pid to global vector
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

            //append pids to global vector
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

    //wait for processes
    wait(NULL);
    wait(NULL);

}

//handle keyboard interrupt
void handle_signal(int signum) {

    //kill foreground processes
    for (pid_t pid : pids) {
        kill(pid, signum);
    }


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

        //close stdout and stderr
        close(1);
        close(2);

        //execute command
        external_execution(input);

    }
    //enter parent
    else {
        //append pid to global vector
        pids.push_back(pid);

        //output background message
        cout << "PID " << pid << " is running in the background" << "\n";

    }

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

//returns true if input has ampersand and false otherwise
bool check_background(vector<string> input) {

    //initializes bool to false
    bool background = false;

    //iterates through input
    for (string token: input) {
        if (token == "&") {
            background = true;
        }
    }

    //returns bool
    return background;
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

        //if ctrl D EOF signal received handle
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
            bool redirect = check_arrow(input_list);

            //check if pipe needed
            bool pipe = check_pipe(input_list);

            //check if need to run background process
            bool background = check_background(input_list);

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

  // print the string prompt without a newline, before beginning to read
  // tokenize the input, run the command(s), and print the result
  // do this in a loop

  //signal handling
  struct sigaction sa;

  sa.sa_flags = SA_RESTART;
  sigemptyset(&sa.sa_mask);
  sa.sa_handler = &handle_signal;

  sigaction(SIGINT, &sa, NULL);
  sigaction(SIGTSTP, &sa, NULL);

  //print welcome message
  cout << "Welcome to DragonShell!";
  cout << "\n";

  //call function to start dragonshell
  loop();

  return 0;
}
