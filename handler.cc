#include <vector>
#include <iostream>
#include <unistd.h>
#include "handler.h"


using namespace std;

char cwd[256];

vector<string> path = {"/bin/", "/usr/bin/"};

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
