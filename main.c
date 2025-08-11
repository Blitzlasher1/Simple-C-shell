#include "headers.h"


int shell_cd(char **args);
int shell_help(char **args);
int shell_exit(char **args);

char *shell_builtin_names[] = {
    "cd",
    "help",
    "exit"
};

int (*shell_builtin_funcs[]) (char **) = {
    &shell_cd,
    &shell_help,
    &shell_exit
};

int shell_builtins_num() {
    return sizeof(shell_builtin_names) / sizeof(char *);
}

/**
 * @brief Reads a line of input from stdin.
 * @return The line from stdin.
 */
char *read_line() {
    char *line = NULL;
    size_t bufsize = 0;
    getline(&line, &bufsize, stdin);
    return line;
}

/**
 * @brief Main loop of the shell.
 */
void shell_loop() {
    char *line;
    pipeline* p;
    int status;

    do {
        printf("> ");
        line = read_line();

        p = parse_line(line);

        if (p != NULL) {
            status = execute(p);
        } else {
            status = 1;
        }

        
        free(line);
        if (p != NULL) {
            for (int i = 0; i < p->num_commands; i++) {
                if (p->commands[i] != NULL) {
                    if (p->commands[i]->args != NULL) {
                        for (int j = 0; p->commands[i]->args[j] != NULL; j++) {
                            free(p->commands[i]->args[j]);
                        }
                        free(p->commands[i]->args);
                    }

                    free(p->commands[i]->input_file);
                    free(p->commands[i]->output_file);
                    free(p->commands[i]);
                }
            }
            free(p->commands);
            free(p);
        }
    } while (status);
}

/**
 * @brief Main function of the shell.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Exit status.
 */
int main(int argc, char **argv) {
    shell_loop();
    return EXIT_SUCCESS;
}


