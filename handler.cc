#include <vector>
#include <iostream>
#include <unistd.h>
#include "handler.h"


using namespace std;

char cwd[256];

void pwd(){
    getcwd(cwd, sizeof(cwd));
    cout << cwd;
    cout << "\n";
}

// void change_directory(std::string &path){
//     cout << path;
// }
//
// int exit_program(){
//     cout << "Exiting";
//     return 0;
// }
