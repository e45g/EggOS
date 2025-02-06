#pragma once

#define SHELL_PROMPT_MAX 256
#define SHELL_HISTORY_MAX 256

typedef struct {
    char prompt[SHELL_PROMPT_MAX];
    char history[SHELL_HISTORY_MAX][SHELL_PROMPT_MAX];
    int history_index;
    int history_count;
} shell_t;


void print_help(void);
void launch_shell(void);
void fetch(void);
