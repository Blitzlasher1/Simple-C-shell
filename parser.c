// parser.c
#include "headers.h"

/**
 * @brief Splits a line into tokens, checks for redirection/background flags.
 * @param line The input string to parse.
 * @return A parsed_command structure containing tokens, redirection files, and flags.
 */

 char* safe_strdup(const char* s) {
    if (s == NULL) return NULL;
    size_t len = strlen(s) + 1;
    char* new_s = malloc(len);
    if (new_s == NULL) return NULL;
    return (char*)memcpy(new_s, s, len);
}

pipeline* parse_line(char* line) {
    if (line == NULL) return NULL;

    pipeline* p = malloc(sizeof(pipeline));
    if (!p) {
        perror("shell: allocation error");
        return NULL;
    }
    p->commands = NULL;
    p->num_commands = 0;

    char* line_copy = safe_strdup(line);
    if (line_copy == NULL) {
        free(p);
        return NULL;
    }

    
    char* command_str;
    char* line_ptr = line_copy;
    char* pipe_delimiter = "|";
    while ((command_str = strtok_r(line_ptr, pipe_delimiter, &line_ptr)) != NULL) {

        p->num_commands++;
        p->commands = realloc(p->commands, p->num_commands * sizeof(parsed_command*));
        if (!p->commands) {
            perror("shell: realloc error");
            
            free(p);
            free(line_copy);
            return NULL;
        }

        parsed_command* cmd = malloc(sizeof(parsed_command));
        if (!cmd) {
            perror("shell: allocation error");
            
            free(p);
            free(line_copy);
            return NULL;
        }

        cmd->input_file = NULL;
        cmd->output_file = NULL;
        cmd->is_background = 0;
        cmd->args = NULL;

        
        int bufsize = LSH_BUFSIZE;
        int position = 0;
        char** tokens = malloc(bufsize * sizeof(char*));
        if (!tokens) {
            perror("shell: allocation error");
            free(cmd);
            free(p);
            free(line_copy);
            return NULL;
        }

        char* token;
        char* inner_line_ptr = command_str;
        char* inner_delimiter = " \t\r\n\a";
        while ((token = strtok_r(inner_line_ptr, inner_delimiter, &inner_line_ptr)) != NULL) {

            
            if (strcmp(token, ">") == 0) {
                token = strtok_r(NULL, inner_delimiter, &inner_line_ptr);
                if (token == NULL) {
                    fprintf(stderr, "shell: expected filename after '>'\n");
                    free(tokens);
                    free(cmd);
                    free(p);
                    free(line_copy);
                    return NULL;
                }
                cmd->output_file = safe_strdup(token);
                continue;
            }

            
            if (strcmp(token, "<") == 0) {
                token = strtok_r(NULL, inner_delimiter, &inner_line_ptr);
                if (token == NULL) {
                    fprintf(stderr, "shell: expected filename after '<'\n");
                    free(tokens);
                    free(cmd);
                    free(p);
                    free(line_copy);
                    return NULL;
                }
                cmd->input_file = safe_strdup(token);
                continue;
            }

            
            tokens[position] = safe_strdup(token);
            position++;

            if (position >= bufsize) {
                bufsize += LSH_BUFSIZE;
                tokens = realloc(tokens, bufsize * sizeof(char*));
                if (!tokens) {
                    perror("shell: allocation error");
                    
                    free(cmd);
                    free(p);
                    free(line_copy);
                    return NULL;
                }
            }
        }
        tokens[position] = NULL;
        cmd->args = tokens;
        p->commands[p->num_commands - 1] = cmd;
    }

    
    if (p->num_commands > 0) {
        parsed_command* last_cmd = p->commands[p->num_commands - 1];
        int last_arg_idx = 0;
        while (last_cmd->args[last_arg_idx] != NULL) {
            last_arg_idx++;
        }
        if (last_arg_idx > 0 && strcmp(last_cmd->args[last_arg_idx - 1], "&") == 0) {
            last_cmd->is_background = 1;
            last_cmd->args[last_arg_idx - 1] = NULL;
        }
    }

    
    free(line_copy);

    return p;
}