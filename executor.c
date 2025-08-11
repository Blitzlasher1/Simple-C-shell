// executor.c
#include "headers.h"


char *builtin_str[] = {
    "cd",
    "exit"
};


int shell_cd(char **args);
int shell_exit(char **args);


int (*builtin_func[]) (char **) = {
    &shell_cd,
    &shell_exit
};


int shell_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

/**
 * @brief Built-in command: change directory.
 * @param args List of arguments. args[0] is "cd", args[1] is the directory.
 * @return Always returns 1, to continue the shell loop.
 */
int shell_cd(char **args) {
    if (args[1] == NULL) {
        
        fprintf(stderr, "shell: expected argument to \"cd\"\n");
    } else {
        
        if (chdir(args[1]) != 0) {
            perror("shell"); 
        }
    }
    return 1; 
}

/**
 * @brief Built-in command: exit the shell.
 * @param args List of arguments.
 * @return Always returns 0, to terminate the shell loop.
 */
int shell_exit(char **args) {
    return 0; 
}
/**
 * @brief Launches a program and waits for it to terminate.
 * @param args Null terminated list of arguments.
 * @return Always returns 1, to continue the shell loop.
 */
// executor.c
// ... (other functions)

/**
 * @brief Launches a program and waits for it to terminate (unless background).
 * @param args Null terminated list of arguments.
 * @param is_background Flag to determine if the process should run in the background.
 * @return Always returns 1, to continue the shell loop.
 */
// executor.c
// ... (other functions)

/**
 * @brief Launches a program and waits for it to terminate (unless background).
 * @param cmd A parsed_command struct containing all command data.
 * @return Always returns 1, to continue the shell loop.
 */
int shell_launch(parsed_command* cmd) {
    pid_t pid;
    int status;
    int in_fd, out_fd;

    pid = fork();
    if (pid == 0) {
        

        
        if (cmd->input_file != NULL) {
            in_fd = open(cmd->input_file, O_RDONLY);
            if (in_fd == -1) {
                perror("shell: open input failed");
                exit(EXIT_FAILURE);
            }
            if (dup2(in_fd, STDIN_FILENO) == -1) {
                perror("shell: dup2 input failed");
                exit(EXIT_FAILURE);
            }
            close(in_fd);
        }

        
        if (cmd->output_file != NULL) {
            out_fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (out_fd == -1) {
                perror("shell: open output failed");
                exit(EXIT_FAILURE);
            }
            if (dup2(out_fd, STDOUT_FILENO) == -1) {
                perror("shell: dup2 output failed");
                exit(EXIT_FAILURE);
            }
            close(out_fd);
        }

        
        if (execvp(cmd->args[0], cmd->args) == -1) {
            perror("shell");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        
        perror("shell");
    } else {
        
        if (cmd->is_background == 0) {
            do {
                waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        } else {
            printf("Running background process with PID: %d\n", pid);
        }
    }

    return 1;
}
/**
 * @brief Executes a command.
 * @param args Null terminated list of arguments.
 * @return 1 if the shell should continue, 0 if it should exit.
 */
// executor.c
// ... (your existing built-ins and functions)

/**
 * @brief Executes a pipeline of commands.
 * @param p The pipeline struct containing all commands.
 * @return Always returns 1, to continue the shell loop.
 */
int execute(pipeline* p) {
    if (p->num_commands == 0) {
        return 1;
    }

    int i;
    pid_t pid;
    int status;
    int in_fd = STDIN_FILENO;
    int out_fd = STDOUT_FILENO;
    int fd[2]; 

    
    if (p->num_commands == 1) {
        
        if (shell_builtins_num() > 0) {
            for (i = 0; i < shell_builtins_num(); i++) {
                if (strcmp(p->commands[0]->args[0], shell_builtin_names[i]) == 0) {
                    return (*shell_builtin_funcs[i])(p->commands[0]->args);
                }
            }
        }
        return shell_launch(p->commands[0]);
    }

    
    for (i = 0; i < p->num_commands; i++) {
        parsed_command* cmd = p->commands[i];

        
        if (i < p->num_commands - 1) {
            if (pipe(fd) == -1) {
                perror("shell");
                exit(EXIT_FAILURE);
            }
            out_fd = fd[1]; 
        } else {
            out_fd = STDOUT_FILENO;
        }

        
        pid = fork();
        if (pid == 0) {
            

            
            if (in_fd != STDIN_FILENO) {
                dup2(in_fd, STDIN_FILENO);
                close(in_fd);
            }

            
            if (out_fd != STDOUT_FILENO) {
                dup2(out_fd, STDOUT_FILENO);
                close(out_fd);
            }

            
            if (cmd->input_file != NULL) {
                in_fd = open(cmd->input_file, O_RDONLY);
                if (in_fd == -1) {
                    perror("shell");
                    exit(EXIT_FAILURE);
                }
                dup2(in_fd, STDIN_FILENO);
                close(in_fd);
            }

            if (cmd->output_file != NULL) {
                out_fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (out_fd == -1) {
                    perror("shell");
                    exit(EXIT_FAILURE);
                }
                dup2(out_fd, STDOUT_FILENO);
                close(out_fd);
            }

            // Execute the command
            if (execvp(cmd->args[0], cmd->args) == -1) {
                perror("shell");
            }
            exit(EXIT_FAILURE);
        } else if (pid < 0) {
            // Error forking
            perror("shell");
            exit(EXIT_FAILURE);
        }

        // Parent process
        // Close the file descriptors and set up for the next command
        if (in_fd != STDIN_FILENO) close(in_fd);
        if (out_fd != STDOUT_FILENO) close(out_fd);

        // The output of this command will be the input for the next
        in_fd = fd[0];
    }

    // Wait for all child processes to finish
    for (i = 0; i < p->num_commands; i++) {
        wait(&status);
    }

    return 1;
}
// executor.c
// ... (your existing shell_cd and shell_exit functions)

/**
 * @brief Prints help information.
 * @param args List of arguments. Not used.
 * @return Always returns 1, to continue the shell loop.
 */
int shell_help(char **args) {
    printf("Blitz Shell\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following are built in:\n");
    printf("  cd\n");
    printf("  help\n");
    printf("  exit\n");
    printf("Use the man command for information on other programs.\n");
    return 1;
} 
 