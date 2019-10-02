#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <typeinfo>
#include <fcntl.h>

using namespace std;

int main(int argc, char *argv[]) {
    int file_desc = open("test.txt", O_CREAT | O_WRONLY);
    if(file_desc < 0) {
        cout << "Error opening the file" << "\n";
    }

    dup2(file_desc, 1);
    cout << "Tester" << "\n";
    close(file_desc);
    return 0;
}
