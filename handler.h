#ifndef HANDLER
#define HANDLER

#include <vector>
#include <iostream>

void pwd();
void change_directory(char const *path);
int exit_program();
void show_path();
void append_path(const std::string path);
void external_execution(std::vector<std::string>);
void redirect_output(std::vector<std::string>);

#endif
