#include <tsh.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <vector>
#include <unistd.h>

using namespace std;

vector<vector<char*>> simple_shell::parse_command(char* cmd) {
    vector<vector<char*>> commands;
    stringstream ss(cmd);
    string command;
    while (getline(ss, command, '|')) {
        vector<char*> args;
        stringstream ss_command(command);
        string arg;
        while (ss_command >> arg) {
            char* cstr = new char[arg.length() + 1];
            strcpy(cstr, arg.c_str());
            args.push_back(cstr);
        }
        commands.push_back(args);
    }
    return commands;
}

void simple_shell::exec_command(vector<vector<char*>> commands) {
    int fd[2]; // file descriptors for the pipe
    int prev_fd = -1; // file descriptor of previous command's output
    for (int i = 0; i < commands.size(); i++) {
        char** argv = new char*[commands[i].size()];
        for (int j = 0; j < commands[i].size(); j++) {
            argv[j] = new char[strlen(commands[i][j]) + 1];
            strcpy(argv[j], commands[i][j]);
            isQuit(argv[j]);
        }
        argv[commands[i].size()] = NULL; // null terminate the argument list
        if (strcmp(argv[0], "cd") == 0) {
            cd_command(argv);
            continue; // skip the fork/exec process for cd command
        }
        if (i != commands.size() - 1) {
            pipe(fd); // create a pipe for communication between the processes
        }

        int pid = fork(); // forking the parent to get the child process
        if( pid < 0){ 
            cout << "Fork failed to execute" << endl;
            exit(1);
        }else if (pid == 0) {
            // child process
            if (prev_fd != -1) {
                close(STDIN_FILENO); // close standard input
                dup2(prev_fd, STDIN_FILENO); // redirect input from previous command's output
                close(prev_fd); // close the previous command's output
            }
            if (i != commands.size() - 1) {
                close(STDOUT_FILENO); // close standard output
                dup2(fd[1], STDOUT_FILENO); // redirect output to the next command's input
                close(fd[0]); // close the read end of the pipe
            }
            if (strcmp(argv[0], "echo") == 0) {
                // handle the "echo" command
                for (int j = 1; j < commands[i].size(); j++) {
                    cout << commands[i][j] << " ";
                }
                cout << endl;
                exit(0);
            } 
            else if (strcmp(argv[0], "pwd") == 0) {
                char cwd[1024];
                getcwd(cwd, sizeof(cwd));
                cout << cwd << endl;
                exit(0);
            } 
            else{
                execvp(argv[0], argv); 
                exit(1); // terminate child process if execvp fails
            }
        } else {
            // parent process waiting for child process to finish
            int wait = waitpid(pid, NULL, 0); 
            if (i != commands.size() - 1) {
                close(fd[1]); // close the write end of the pipe
                prev_fd = fd[0]; // save the read end of the pipe for the next command
            }
        }
    }
}

void simple_shell::cd_command(char** argv) {
    if (argv[1] == NULL) {
        cout << "cd: No directory specified" << endl;
    } else {
        if (chdir(argv[1]) != 0) {
            perror("cd");
        }
    }
}

bool simple_shell::isQuit(char* cmd) {
    if (strcmp(cmd, "quit") == 0){
        exit(0);
    } else { 
        return false; 
    };
}
