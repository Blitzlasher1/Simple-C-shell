// headers.h
#ifndef SHELL_H
#define SHELL_H
#define _GNU_SOURCE
#define LSH_BUFSIZE 64 

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <unistd.h> 
#include <sys/wait.h> 
#include <sys/types.h>
#include <fcntl.h>    
#include <sys/stat.h> 



typedef struct {
    char** args;
    char* input_file;
    char* output_file;
    int is_background;
} parsed_command;

typedef struct {
    parsed_command** commands;
    int num_commands;
} pipeline;



void shell_loop();
char* read_line();

pipeline* parse_line(char* line);
int execute(pipeline* p);



int shell_launch(parsed_command* cmd);












#define TOKEN_DELIMITERS " \t\r\n\a" 

#endif 

int shell_builtins_num();
extern char *shell_builtin_names[];
extern int (*shell_builtin_funcs[])(char **);