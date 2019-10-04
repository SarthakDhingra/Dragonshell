#ifndef HANDLER
#define HANDLER

#include <vector>
#include <iostream>

void pwd();
void change_directory(char const *path);
void exit_program();
void show_path();
void append_path(const std::string path);
void external_execution(std::vector<std::string>);
void redirect_output(std::vector<std::string>, std::string);
void execute_pipe(std::vector<std::string>, std::vector<std::string>);
void handle_signal(int);
void background_process(std::vector<std::string>);
#endif
