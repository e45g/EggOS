#include <tty.h>
#include <vga.h>
#include <utils.h>
#include <keyboard.h>
#include <common.h>
#include <shell.h>

static char history[256][256] = {0};

void launch_shell(void) {
    char prefix[] = "$ ";
    char prompt[256];
    do {
        printf("%s", prefix, prompt);
        getstr(prompt, sizeof(prompt));

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

        strcpy(history[0], prompt);

    } while(strcmp(prompt, "exit") != 0);

}
