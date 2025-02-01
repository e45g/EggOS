#include <tty.h>
#include <vga.h>
#include <utils.h>
#include <keyboard.h>
#include <common.h>

void print_help(void) {
    printf("\n========================== GENERIC ==========================");
    printf("\n= help           : Displays this help message                =");
    printf("\n= fetch          : Displays the logo                        =");
    printf("\n= exit           : Exits the shell                          =");
    printf("\n= get_esp        : Displays the current ESP                  =");
    printf("\n= get_stack      : Displays the current stack               =");
    printf("\n= clear          : Clears the screen                        =");
    printf("\n=============================================================\n");
}


void fetch() {
    terminal_set_color(VGA_COLOR_RED, VGA_COLOR_BLACK);
    printf("               __\n");
    printf("              / _)  ");

    terminal_set_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
    printf("T-RexOS\n");
    terminal_set_color(VGA_COLOR_RED, VGA_COLOR_BLACK);

    printf("     _.----._/ /  \n");
    printf("    /         /   \n");
    printf(" __/ (  | (  |    \n");
    printf("/__.-'|_|--|_|    \n");

    terminal_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
}

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

    } while(strcmp(prompt, "exit") != 0);

}
