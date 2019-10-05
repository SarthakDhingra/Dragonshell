### Design Choices
- this program was executed procedurally
- if statements were used to handle the input
- one file was used to simplify Makefile
- when possible, separate functions were created for tasks to containerize code

#### System calls used
- execve()
- access()
- dup2()
- \_exit()
- open()
- close()
- wait()
- pipe()
- fork()
- kill()
- sigaction()
- getcwd()
- chdir()

### Testing implementation
- Generated combinations of various linux commands to test on Dragonshell

### Sources
- http://man7.org/linux/man-pages//man2/
- https://stackoverflow.com/
- http://www.cplusplus.com/
- https://en.cppreference.com/w/
- https://www.geeksforgeeks.org/
