#include <tty.h>
#include <utils.h>
#include <keyboard.h>
#include <common.h>
#include <shell.h>

static shell_t shell = {0};

char *shell_getstr(char *buf, uint16_t len) {
    uint16_t i = 0;

    while(i < len - 1) {
        int c = getchar();

        if(c == 0) {
            if (i == 0) {
                return NULL;
            }
            break;
        }

        if(c == UP_ARROW_PRESSED) {
            if(shell.history_index > 0) {
                terminal_clear_line();
                strcpy(buf, shell.history[--shell.history_index]);
                i = strlen(buf);
                printf("$ %s", buf);
            }
            continue;
        }
        if(c == DOWN_ARROW_PRESSED) {
            if(shell.history_index < shell.history_count) {
                terminal_clear_line();
                strcpy(buf, shell.history[++shell.history_index]);
                i = strlen(buf);
                printf("$ %s", buf);
            }
            continue;
        }

        if(c == '\n') {
            buf[i++] = '\0';
            terminal_putc(c);
            terminal_update_cursor();
            break;
        }

        if(c == '\b') {
            if(i > 0) {
                i--;
                terminal_putc(c);
                terminal_update_cursor();
            }
            continue;
        }

        buf[i++] = c;
        terminal_putc(c);
        terminal_update_cursor();

    }
    if(buf != NULL && buf != shell.prompt && *buf != '\0' && *buf != '\n') {
        strcpy(shell.history[shell.history_index++], buf);
        shell.history_count++;
    }
    return buf;
}

void launch_shell(void) {
    const char prefix[] = "$ ";
    char prompt[256] = {0};
    do {
        printf("%s", prefix);
        shell_getstr(prompt, sizeof(prompt));

        if(strcmp(prompt, "get_esp") == 0) {
            printf("%x\n", get_esp());
        }
        else if(strcmp(prompt, "get_stack") == 0) {
            uintptr_t esp = get_esp();
            print_stack((uintptr_t *)esp, 12);
        }
        else if (strcmp(prompt, "fetch") == 0) {
            fetch();
        }
        else if (strcmp(prompt, "clear") == 0) {
            terminal_clear();
        }
        else if (strcmp(prompt, "help") == 0) {
            print_help();
        }
        else if (strcmp(prompt, "exit") == 0) { }
        else {
            printf("Command `%s` not found.\n", prompt);
        }
    } while(strcmp(prompt, "exit") != 0);

}
