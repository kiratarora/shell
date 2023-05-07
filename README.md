# Shell

This is the final project for Computer Science 377. 

Here, I have implemented a shell script that apart from running regular shell commands such as ```ls``` and ```quit```, can also run pipelining commands and certain Bash Builtin Commands such as ```cd```, ``` echo```, and ```pwd```.

Here is a brief description of the project, along with a video to demonstarate the functionality:

To implement pipelining commands, the code simply creates a pipe that a command can write to so that the next command can read from it as an input for itself. 

```
if (i != commands.size() - 1) {
    pipe(fd);
}
```

The above line of code simply creates a pipe for the commands to use to exchange outputs. 

```
if (prev_fd != -1) {
    close(STDIN_FILENO);
    dup2(prev_fd, STDIN_FILENO);
    close(prev_fd);
} 

if (i != commands.size() - 1) {
    close(STDOUT_FILENO);
    dup2(fd[1], STDOUT_FILENO);
    close(fd[0]);
}
```

This part of the code shows how the output from the previous command is redirected to the pipe, and then that output is redirected to the next command's input. 


To implement the ```pwd``` command, I simply get the cwd and print that. Similarly for the ```cd``` command, I check if the path exists, and then change the directory to that folder. 